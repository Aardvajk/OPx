#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Context;

namespace Process
{

void processUnit(Context &c, const std::string &path);
void processExe(Context &c, const std::string &path);
void processMappedExe(Context &c, const std::string &path);

}

#endif // PROCESS_H
