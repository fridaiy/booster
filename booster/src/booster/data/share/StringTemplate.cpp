

#include "StringTemplate.hpp"

#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace data { namespace share {

StringTemplate::VectorValueProvider::VectorValueProvider(const std::vector<booster::String> *params)
  : m_params(params)
{}

booster::String StringTemplate::VectorValueProvider::getValue(const Variable& variable, v_uint32 index) {
  (void) variable;
  return m_params->at(index);
}

StringTemplate::MapValueProvider::MapValueProvider(const std::unordered_map<booster::String, booster::String> *params)
  : m_params(params)
{}

booster::String StringTemplate::MapValueProvider::getValue(const Variable& variable, v_uint32 index) {
  (void) index;
  auto it = m_params->find(variable.name);
  if(it != m_params->end()) {
    return it->second;
  }
  return nullptr;
}

StringTemplate::SingleValueProvider::SingleValueProvider(const booster::String& value)
  : m_value(value)
{}

booster::String StringTemplate::SingleValueProvider::getValue(const Variable& variable, v_uint32 index) {
  (void) variable;
  (void) index;
  return m_value;
}

StringTemplate::StringTemplate(const booster::String& text, std::vector<Variable>&& variables)
  : m_text(text)
  , m_variables(variables)
{
  v_buff_size prevPos = 0;
  for(size_t i = 0; i < m_variables.size(); i++) {
    const auto& var = m_variables[i];

    if(var.posStart < prevPos) {
      throw std::runtime_error("[booster::data::share::StringTemplate::StringTemplate()]: Error. The template variable positions can't intersect.");
    }

    if(var.posEnd < var.posStart) {
      throw std::runtime_error("[booster::data::share::StringTemplate::StringTemplate()]: Error. The template variable can't have a negative size.");
    }

    if(static_cast<size_t>(var.posEnd) >= m_text->size()) {
      throw std::runtime_error("[booster::data::share::StringTemplate::StringTemplate()]: Error. The template variable can't out-bound the template text.");
    }
  }
}

void StringTemplate::format(stream::ConsistentOutputStream* stream, ValueProvider* valueProvider) const {

  v_buff_size prevPos = 0;
  for(v_uint32 i = 0; i < m_variables.size(); i++) {

    const auto& var = m_variables[i];
    const auto& value = valueProvider->getValue(var, i);

    if(!value) {
      throw std::runtime_error("[booster::data::share::StringTemplate::format()]: "
                               "Error. No value provided for the parameter name=" + *var.name);
    }

    if(prevPos < var.posStart) {
      stream->writeSimple(m_text->data() + prevPos, var.posStart - prevPos);
    }

    stream->writeSimple(value->data(), static_cast<v_buff_size>(value->size()));

    prevPos = var.posEnd + 1;

  }

  if(static_cast<size_t>(prevPos) < m_text->size()) {
    stream->writeSimple(m_text->data() + prevPos, static_cast<v_buff_size>(m_text->size()) - prevPos);
  }

}

void StringTemplate::format(stream::ConsistentOutputStream* stream, const std::vector<booster::String>& params) const {
  VectorValueProvider vp(&params);
  format(stream, &vp);
}

void StringTemplate::format(stream::ConsistentOutputStream* stream, const std::unordered_map<booster::String, booster::String>& params) const {
  MapValueProvider vp(&params);
  format(stream, &vp);
}

void StringTemplate::format(stream::ConsistentOutputStream* stream, const booster::String& singleValue) const {
  SingleValueProvider vp(singleValue);
  format(stream, &vp);
}

booster::String StringTemplate::format(ValueProvider* valueProvider) const {
  stream::BufferOutputStream stream;
  format(&stream, valueProvider);
  return stream.toString();
}

booster::String StringTemplate::format(const std::vector<booster::String>& params) const {
  stream::BufferOutputStream stream;
  format(&stream, params);
  return stream.toString();
}

booster::String StringTemplate::format(const std::unordered_map<booster::String, booster::String>& params) const {
  stream::BufferOutputStream stream;
  format(&stream, params);
  return stream.toString();
}

booster::String StringTemplate::format(const booster::String& singleValue) const {
  stream::BufferOutputStream stream;
  format(&stream, singleValue);
  return stream.toString();
}

const std::vector<StringTemplate::Variable>& StringTemplate::getTemplateVariables() const {
  return m_variables;
}

void StringTemplate::setExtraData(const std::shared_ptr<void>& data) {
  m_extra = data;
}

std::shared_ptr<void> StringTemplate::getExtraData() const {
  return m_extra;
}

}}}
