

#ifndef booster_data_mapping_ObjectMapper_hpp
#define booster_data_mapping_ObjectMapper_hpp

#include <utility>

#include "booster/Types.hpp"

#include "booster/data/stream/Stream.hpp"

#include "booster/utils/parser/Caret.hpp"
#include "booster/utils/parser/ParsingError.hpp"

namespace booster { namespace data { namespace mapping {


struct ErrorStack {


  std::list<booster::String> stack;


  void push(const booster::String& error);


  void splice(ErrorStack& errorStack);


  booster::String stacktrace() const;


  bool empty() const;

};


class MappingError : public std::runtime_error {
private:
  ErrorStack m_stack;
public:


  MappingError(const ErrorStack& errorStack);


  MappingError(ErrorStack&& errorStack);


  const ErrorStack& errorStack() const;


  ErrorStack& errorStack();

};


class ObjectMapper {
public:


  class Info {
  public:


    Info(const booster::String& pMimeType, const booster::String& pMimeSubtype)
      : httpContentType(pMimeType + "/" + pMimeSubtype)
      , mimeType(pMimeType)
      , mimeSubtype(pMimeSubtype)
    {}


    const booster::String httpContentType;


    const booster::String mimeType;


    const booster::String mimeSubtype;

  };
private:
  Info m_info;
public:


  ObjectMapper(const Info& info);


  const Info& getInfo() const;


  virtual void write(data::stream::ConsistentOutputStream* stream, const booster::Void& variant, ErrorStack& errorStack) const = 0;


  virtual booster::Void read(booster::utils::parser::Caret& caret, const booster::Type* type, ErrorStack& errorStack) const = 0;


  booster::String writeToString(const booster::Void& variant) const;


  template<class Wrapper>
  Wrapper readFromCaret(booster::utils::parser::Caret& caret) const {
    auto type = Wrapper::Class::getType();
    ErrorStack errorStack;
    const auto& result = read(caret, type, errorStack).template cast<Wrapper>();
    if(!errorStack.empty()) {
      throw MappingError(std::move(errorStack));
    }
    return result;
  }


  template<class Wrapper>
  Wrapper readFromString(const booster::String& str) const {
    auto type = Wrapper::Class::getType();
    booster::utils::parser::Caret caret(str);
    ErrorStack errorStack;
    const auto& result = read(caret, type, errorStack).template cast<Wrapper>();
    if(!errorStack.empty()) {
      throw MappingError(std::move(errorStack));
    }
    return result;
  }
  
};
  
}}}

#endif
