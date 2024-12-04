

#ifndef booster_web_url_mapping_Pattern_hpp
#define booster_web_url_mapping_Pattern_hpp

#include "booster/data/share/MemoryLabel.hpp"

#include "booster/utils/parser/Caret.hpp"

#include <list>
#include <unordered_map>

namespace booster { namespace web { namespace url { namespace mapping {
  
class Pattern : public base::Countable{
private:
  typedef booster::data::share::StringKeyLabel StringKeyLabel;
public:
  
  class MatchMap {
    friend Pattern;
  public:
    typedef std::unordered_map<StringKeyLabel, StringKeyLabel> Variables;
  private:
    Variables m_variables;
    StringKeyLabel m_tail;
  public:
    
    MatchMap() {}
    
    MatchMap(const Variables& vars, const StringKeyLabel& urlTail)
      : m_variables(vars)
      , m_tail(urlTail)
    {}
    
    booster::String getVariable(const StringKeyLabel& key) const {
      auto it = m_variables.find(key);
      if(it != m_variables.end()) {
        return it->second.toString();
      }
      return nullptr;
    }
    
    booster::String getTail() const {
      return m_tail.toString();
    }

    const Variables& getVariables() const {
      return m_variables;
    }
    
  };
  
private:
  
  class Part : public base::Countable{
  public:
    Part(const char* pFunction, const booster::String& pText)
      : function(pFunction)
      , text(pText)
    {}
  public:
    
    static const char* FUNCTION_CONST;
    static const char* FUNCTION_VAR;
    static const char* FUNCTION_ANY_END;
    
    static std::shared_ptr<Part> createShared(const char* function, const booster::String& text){
      return std::make_shared<Part>(function, text);
    }
    
    const char* function;
    const booster::String text;
    
  };
  
private:
  std::shared_ptr<std::list<std::shared_ptr<Part>>> m_parts{std::make_shared<std::list<std::shared_ptr<Part>>>()};
private:
  v_char8 findSysChar(booster::utils::parser::Caret& caret);
public:
  
  static std::shared_ptr<Pattern> createShared(){
    return std::make_shared<Pattern>();
  }
  
  static std::shared_ptr<Pattern> parse(p_char8 data, v_buff_size size);
  static std::shared_ptr<Pattern> parse(const char* data);
  static std::shared_ptr<Pattern> parse(const booster::String& data);
  
  bool match(const StringKeyLabel& url, MatchMap& matchMap);
  
  booster::String toString();
  
};
  
}}}}

#endif
