

#include "CommandLineArguments.hpp"

#include <cstring>

namespace booster { namespace base {

CommandLineArguments::CommandLineArguments()
  : m_argc(0)
  , m_argv(nullptr)
{}

CommandLineArguments::CommandLineArguments(int argc, const char * argv[])
  : m_argc(argc)
  , m_argv(argv)
{}
  
bool CommandLineArguments::Parser::hasArgument(int argc, const char * argv[], const char* argName) {
  return getArgumentIndex(argc, argv, argName) >= 0;
}

v_int32 CommandLineArguments::Parser::getArgumentIndex(int argc, const char * argv[], const char* argName) {
  for(v_int32 i = 0; i < argc; i ++) {
    if(std::strcmp(argName, argv[i]) == 0){
      return i;
    }
  }
  return -1;
}

const char* CommandLineArguments::Parser::getArgumentStartingWith(int argc, const char * argv[], const char* argNamePrefix, const char* defaultValue) {
  for(v_int32 i = 0; i < argc; i ++) {
    if(std::strncmp(argNamePrefix, argv[i], std::strlen(argNamePrefix)) == 0){
      return argv[i];
    }
  }
  return defaultValue;
}

const char* CommandLineArguments::Parser::getNamedArgumentValue(int argc, const char * argv[], const char* argName, const char* defaultValue) {
  for(v_int32 i = 0; i < argc; i ++) {
    if(std::strcmp(argName, argv[i]) == 0){
      if(i + 1 < argc) {
        return argv[i + 1];
      }
    }
  }
  return defaultValue;
}
  
}}
