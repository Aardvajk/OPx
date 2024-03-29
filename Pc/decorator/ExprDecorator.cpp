#include "ExprDecorator.h"

#include "application/Context.h"

#include "operators/Operators.h"

#include "nodes/IdNode.h"
#include "nodes/TypeNode.h"
#include "nodes/LiteralNodes.h"
#include "nodes/CallNode.h"
#include "nodes/ProxyCallNode.h"
#include "nodes/ConstructNode.h"
#include "nodes/AddrOfNode.h"
#include "nodes/DerefNode.h"
#include "nodes/ThisNode.h"
#include "nodes/AssignNode.h"
#include "nodes/UnaryNode.h"
#include "nodes/BinaryNode.h"
#include "nodes/LogicalNode.h"
#include "nodes/IncDecNodes.h"
#include "nodes/VarNode.h"
#include "nodes/CommaNode.h"
#include "nodes/InlineVarNode.h"
#include "nodes/TernaryNode.h"
#include "nodes/TypeCastNode.h"
#include "nodes/SubscriptNode.h"
#include "nodes/UncheckedCastNode.h"

#include "visitors/SymFinder.h"
#include "visitors/TypeVisitor.h"
#include "visitors/QueryVisitors.h"
#include "visitors/NameVisitors.h"

#include "types/Type.h"
#include "types/TypeCompare.h"

#include "decorator/CommonDecorator.h"
#include "decorator/TypeDecorator.h"
#include "decorator/VarDecorator.h"
#include "decorator/FuncDecorator.h"

#include "operators/OperatorCallDecorate.h"

#include <pcx/str.h>
#include <pcx/base64.h>
#include <pcx/scoped_counter.h>
#include <pcx/scoped_push.h>

namespace
{

template<typename T> NodePtr decorateIncDec(Context &c, T &node, bool pre)
{
    node.expr = ExprDecorator::decorate(c, node.expr);

    if(!TypeVisitor::assertType(c, node.expr.get())->primitive())
    {
        NodePtr dn(IdNode::create(node.location(), { "std", pre ? "prefix" : "postfix" }));
        NodePtr tn(new TypeNode(node.location(), dn));

        Visitor::visit<TypeDecorator>(tn.get(), c);

        auto dt = TypeVisitor::assertType(c, tn.get());

        NodePtr cn(new ConstructNode(node.location(), dt));
        cn->setProperty("type", dt);

        NodeList params = { cn };
        return OperatorCallDecorate::generate(c, node, node.expr, params, node.token.text());
    }

    return { };
}

template<typename T> void decorateComplexReturnTemp(Context &c, T &node, Type *type)
{
    if(!type->returnType->primitiveOrRef() && c.tree.current()->container()->type() == Sym::Type::Func)
    {
        auto info = c.tree.current()->container()->property<FuncInfo*>("info");

        auto temp = pcx::str("#temp_return", info->labels++);
        node.setProperty("temp", temp);

        info->temps.push_back(Temp(temp, c.generics.convert(c, type->returnType)));
    }
}

}

ExprDecorator::ExprDecorator(Context &c, Type *expectedType, Flags flags) : c(c), expectedType(expectedType), flags(flags)
{
}

void ExprDecorator::visit(IdNode &node)
{
    if(node.arrow)
    {
        node.parent = new DerefNode(node.location(), node.parent);
        node.arrow = false;
    }

    Visitor::visit<NameVisitors::ResolveOpName>(&node, c);

    if(node.parent)
    {
        node.parent = ExprDecorator::decorate(c, node.parent);

        auto t = TypeVisitor::queryType(c, node.parent.get());
        if(t && t->ptr)
        {
            throw Error(node.location(), "cannot access via pointer - ", node.description());
        }
    }

    std::vector<Type*> generics;
    if(!node.generics.empty())
    {
        for(auto &g: node.generics)
        {
            Visitor::visit<TypeDecorator>(g.get(), c);
            generics.push_back(TypeVisitor::assertType(c, g.get()));
        }

        node.setProperty("generics", generics);
    }

    std::vector<Sym*> sv;
    if(expectedType && expectedType->function())
    {
        sv = CommonDecorator::searchCallable(c, &node, expectedType);
    }
    else
    {
        SymFinder::find(c, SymFinder::Type::Global, c.tree.current(), &node, sv);
    }

    if(sv.empty())
    {
        throw Error(node.location(), "not found - ", node.description());
    }
    else if(sv.size() > 1)
    {
        throw Error(node.location(), "ambiguous - ", node.description());
    }

    if(!sv.front()->accessibleFrom(c.tree.current()))
    {
        throw Error(node.location(), "not accessible - ", node.description());
    }

    auto sym = sv.front();
    node.setProperty("sym", sym);

    if(!node.generics.empty())
    {
        c.genericUsages.insert(c, GenericUsage(node.location(), sym, generics));
    }
}

void ExprDecorator::visit(StringLiteralNode &node)
{
    auto name = pcx::str("#global_", c.globalId++, "_", pcx::base64::encode(c.sources.path(c.scanner.sourceId())));

    c.globals.back()[name] = node.clone();
    node.setProperty("global", name);
}

void ExprDecorator::visit(CallNode &node)
{
    auto t = Type::makeFunction(c.types.nullType());

    if(!flags[Flag::SkipParams])
    {
        for(auto &p: node.params)
        {
            p = ExprDecorator::decorate(c, p);
        }
    }

    for(auto &p: node.params)
    {
        t.args.push_back(TypeVisitor::assertType(c, p.get()));
    }

    node.target = ExprDecorator::decorate(c, node.target, &t);

    auto type = TypeVisitor::assertType(c, node.target.get());

    if(node.target->findProperty("generics").value<std::vector<Type*> >().size() != type->genericTypeCount())
    {
        throw Error(node.location(), "generic function missing types - ", node.description());
    }

    if(type->method)
    {
        bool constant = false;

        auto n = Visitor::query<QueryVisitors::GetParent, NodePtr>(node.target.get());
        if(n)
        {
            constant = TypeVisitor::assertType(c, n.get())->constant;
        }
        else
        {
            constant = c.tree.current()->container()->property<Type*>("type")->constMethod;
        }

        if(constant)
        {
            t.constMethod = true;
            node.target = ExprDecorator::decorate(c, node.target, &t);
        }
    }

    if(auto dt = Visitor::query<QueryVisitors::DirectType, Type*>(node.target.get()))
    {
        rn = new ConstructNode(node.location(), dt, node.params);
        rn->setProperty("type", dt->primitive() ? dt : c.types.insert(dt->addReference()));

        if(dt->primitive())
        {
            if(node.params.size() > 1)
            {
                throw Error(rn->location(), "too many parameters - ", rn->description());
            }
        }
    }
    else
    {
        if(!type->returnType)
        {
            rn = OperatorCallDecorate::generate(c, node, node.target, node.params, "()");
        }
        else
        {
            auto gp = pcx::scoped_push(c.generics, GenericParams(node.target->findProperty("generics").value<std::vector<Type*> >()));
            decorateComplexReturnTemp(c, node, type);
        }
    }
}

void ExprDecorator::visit(ProxyCallNode &node)
{
    if(node.thisNode)
    {
        node.thisNode = ExprDecorator::decorate(c, node.thisNode);
    }

    if(!flags[Flag::SkipParams])
    {
        for(auto &p: node.params)
        {
            p = ExprDecorator::decorate(c, p);
        }
    }

    decorateComplexReturnTemp(c, node, node.sym->property<Type*>("type"));
}

void ExprDecorator::visit(AddrOfNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);
    node.setProperty("type", c.types.insert(TypeVisitor::assertType(c, node.expr.get())->addPointer()));
}

void ExprDecorator::visit(DerefNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);

    if(!TypeVisitor::assertType(c, node.expr.get())->ptr)
    {
        throw Error(node.location(), "cannot deref a non-pointer - ", node.description());
    }

    node.setProperty("type", c.types.insert(TypeVisitor::assertType(c, node.expr.get())->removePointer()));
}

void ExprDecorator::visit(ThisNode &node)
{
    auto s = c.tree.current()->container();

    if(!s->property<Type*>("type")->method)
    {
        throw Error(node.location(), "this outside method");
    }

    auto t = Type::makePrimary(s->parent());

    t.ref = true;
    t.constant = s->property<Type*>("type")->constMethod;

    node.setProperty("type", c.types.insert(t));
}

void ExprDecorator::visit(AssignNode &node)
{
    node.target = ExprDecorator::decorate(c, node.target);

    auto t = TypeVisitor::assertType(c, node.target.get());

    node.expr = ExprDecorator::decorate(c, node.expr, t);

    if(t->primitiveOrRef())
    {
        if(!TypeCompare(c).convertible(TypeVisitor::assertType(c, node.expr.get()), t))
        {
            throw Error(node.expr->location(), t->text(), " expected - ", node.expr->description());
        }
    }
    else
    {
        NodeList params = { node.expr };
        rn = OperatorCallDecorate::generate(c, node, node.target, params, "=");
    }
}

void ExprDecorator::visit(UnaryNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);

    auto type = TypeVisitor::assertType(c, node.expr.get());
    if(!type->primitive())
    {
        NodeList params;
        rn = OperatorCallDecorate::generate(c, node, node.expr, params, node.token.text());
    }
}

void ExprDecorator::visit(BinaryNode &node)
{
    node.left = ExprDecorator::decorate(c, node.left);
    node.right = ExprDecorator::decorate(c, node.right);

    auto lt = TypeVisitor::assertType(c, node.left.get());
    auto rt = TypeVisitor::assertType(c, node.right.get());

    if(!lt->primitive() || !rt->primitive())
    {
        NodeList params = { node.right };
        rn = OperatorCallDecorate::generate(c, node, node.left, params, node.token.text());
    }
    else if(auto tok = Operators::opEqToOp(node.token))
    {
        auto an = new AssignNode(node.location(), node.left);
        rn = an;

        an->expr = new BinaryNode(node.location(), tok, node.left, node.right);
    }
}

void ExprDecorator::visit(LogicalNode &node)
{
    auto g = pcx::scoped_counter(c.potentiallySkipped);

    node.left = ExprDecorator::decorate(c, node.left);
    node.right = ExprDecorator::decorate(c, node.right);
}

void ExprDecorator::visit(PreIncDecNode &node)
{
    if(auto n = decorateIncDec(c, node, true))
    {
        rn = n;
    }
}

void ExprDecorator::visit(PostIncDecNode &node)
{
    if(auto n = decorateIncDec(c, node, false))
    {
        rn = n;
    }
}

void ExprDecorator::visit(CommaNode &node)
{
    node.left = ExprDecorator::decorate(c, node.left);
    node.right = ExprDecorator::decorate(c, node.right);
}

void ExprDecorator::visit(InlineVarNode &node)
{
    Visitor::visit<FuncDecorator>(node.body.get(), c);
}

void ExprDecorator::visit(TernaryNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);

    auto g = pcx::scoped_counter(c.potentiallySkipped);

    node.left = ExprDecorator::decorate(c, node.left);
    node.right = ExprDecorator::decorate(c, node.right);

    auto lt = TypeVisitor::assertType(c, node.left.get());
    auto rt = TypeVisitor::assertType(c, node.right.get());

    if(!TypeCompare(c).compatible(lt, rt))
    {
        throw Error(node.right->location(), lt->text(), " expected - ", node.right->description());
    }
}

void ExprDecorator::visit(TypeCastNode &node)
{
    node.expr = ExprDecorator::decorate(c, node.expr);
}

void ExprDecorator::visit(SubscriptNode &node)
{
    node.target = ExprDecorator::decorate(c, node.target);

    if(!flags[Flag::SkipParams])
    {
        for(auto &p: node.params)
        {
            p = ExprDecorator::decorate(c, p);
        }
    }

    if(TypeVisitor::assertType(c, node.target.get())->primitive())
    {
        if(node.params.size() != 1)
        {
            throw Error(node.location(), "invalid parameters - ", node.description());
        }

        auto bn = new BinaryNode(node.location(), Token(Token::Type::Add, { }, "+"), node.target, node.params.front());
        NodePtr n(bn);

        rn = new DerefNode(node.location(), n);
        rn = ExprDecorator::decorate(c, rn);
    }
    else
    {
        rn = OperatorCallDecorate::generate(c, node, node.target, node.params, "[]");
    }
}

void ExprDecorator::visit(UncheckedCastNode &node)
{
    Visitor::visit<TypeDecorator>(node.type.get(), c);
    node.expr = ExprDecorator::decorate(c, node.expr);

    rn = new TypeCastNode(node.location(), TypeVisitor::assertType(c, node.type.get()), node.expr);
}

NodePtr ExprDecorator::decorate(Context &c, NodePtr node, Type *expectedType, Flags flags)
{
    ExprDecorator ed(c, expectedType, flags);
    node->accept(ed);

    return ed.result() ? ed.result() : node;
}
