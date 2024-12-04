

#include "ObjectMapper.hpp"

#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace data { namespace mapping {

void ErrorStack::push(const booster::String& error) {
  stack.emplace_back(error);
}

void ErrorStack::splice(ErrorStack& errorStack) {
  stack.splice(stack.end(), errorStack.stack);
}

booster::String ErrorStack::stacktrace() const {
  stream::BufferOutputStream ss;
  for(auto& s : stack) {
    ss << s << "\n";
  }
  return ss.toString();
}

bool ErrorStack::empty() const {
  return stack.empty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MappingError

MappingError::MappingError(const ErrorStack& errorStack)
  : std::runtime_error(errorStack.empty() ? "[booster::data::mapping::MappingError]"
                                          : errorStack.stack.front().getValue("<empty-error-stack>"))
  , m_stack(errorStack)
{}

MappingError::MappingError(ErrorStack&& errorStack)
  : std::runtime_error(errorStack.empty() ? "[booster::data::mapping::MappingError]"
                                          : errorStack.stack.front().getValue("<empty-error-stack>"))
  , m_stack(std::move(errorStack))
{}

const ErrorStack& MappingError::errorStack() const {
  return m_stack;
}

ErrorStack& MappingError::errorStack() {
  return m_stack;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ObjectMapper

ObjectMapper::ObjectMapper(const Info& info)
  : m_info(info)
{}

const ObjectMapper::Info& ObjectMapper::getInfo() const {
  return m_info;
}

booster::String ObjectMapper::writeToString(const type::Void& variant) const {
  stream::BufferOutputStream stream;
  ErrorStack errorStack;
  write(&stream, variant, errorStack);
  if(!errorStack.empty()) {
    throw MappingError(std::move(errorStack));
  }
  return stream.toString();
}

}}}
