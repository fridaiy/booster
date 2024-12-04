

#ifndef booster_base_CommandLineArguments_hpp
#define booster_base_CommandLineArguments_hpp

#include "booster/Environment.hpp"

namespace booster { namespace base {


class CommandLineArguments {
public:

  
  class Parser {
  public:

    
    static bool hasArgument(int argc, const char * argv[], const char* argName);
    
    

    
    static v_int32 getArgumentIndex(int argc, const char * argv[], const char* argName);

    
    static const char* getArgumentStartingWith(int argc, const char * argv[], const char* argNamePrefix, const char* defaultValue = nullptr);

    
    static const char* getNamedArgumentValue(int argc, const char * argv[], const char* argName, const char* defaultValue = nullptr);
    
  };
  
private:
  int m_argc;
  const char ** m_argv;
public:

  
  CommandLineArguments();

  
  CommandLineArguments(int argc, const char * argv[]);

  
  bool hasArgument(const char* argName) const {
    return Parser::hasArgument(m_argc, m_argv, argName);
  }

  
  v_int32 getArgumentIndex(const char* argName) const {
    return Parser::getArgumentIndex(m_argc, m_argv, argName);
  }

  
  const char* getArgumentStartingWith(const char* argNamePrefix, const char* defaultValue = nullptr) const {
    return Parser::getArgumentStartingWith(m_argc, m_argv, argNamePrefix, defaultValue);
  }

  
  const char* getNamedArgumentValue(const char* argName, const char* defaultValue = nullptr) const {
    return Parser::getNamedArgumentValue(m_argc, m_argv, argName, defaultValue);
  }
  
};
  
}}

#endif 
