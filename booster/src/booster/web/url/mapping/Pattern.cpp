

#include "Pattern.hpp"

#include "booster/data/stream/BufferStream.hpp"

namespace booster { namespace web { namespace url { namespace mapping {

const char* Pattern::Part::FUNCTION_CONST = "const";
const char* Pattern::Part::FUNCTION_VAR = "var";
const char* Pattern::Part::FUNCTION_ANY_END = "tail";

std::shared_ptr<Pattern> Pattern::parse(p_char8 data, v_buff_size size){
  
  if(size <= 0){
    return nullptr;
  }
  
  auto result = Pattern::createShared();

  v_buff_size lastPos = 0;
  v_buff_size i = 0;
  
  while(i < size){
    
    v_char8 a = data[i];
    
    if(a == '/'){
      
      if(i - lastPos > 0){
        auto part = Part::createShared(Part::FUNCTION_CONST, booster::String(reinterpret_cast<const char*>(&data[lastPos]), i - lastPos));
        result->m_parts->push_back(part);
      }
      
      lastPos = i + 1;
      
    } else if(a == '*'){
      lastPos = i + 1;
      if(size > lastPos){
        auto part = Part::createShared(Part::FUNCTION_ANY_END, booster::String(reinterpret_cast<const char*>(&data[lastPos]), size - lastPos));
        result->m_parts->push_back(part);
      }else{
        auto part = Part::createShared(Part::FUNCTION_ANY_END, booster::String(0));
        result->m_parts->push_back(part);
      }
      return result;
    
    } else if(a == '{'){
      
      lastPos = i + 1;
      while(i < size && data[i] != '}'){
        i++;
      }
      
      if(i > lastPos){
        auto part = Part::createShared(Part::FUNCTION_VAR, booster::String(reinterpret_cast<const char*>(&data[lastPos]), i - lastPos));
        result->m_parts->push_back(part);
      }else{
        auto part = Part::createShared(Part::FUNCTION_VAR, booster::String(0));
        result->m_parts->push_back(part);
      }
      
      lastPos = i + 1;
      
    }
    
    i++;
    
  }
  
  if(i - lastPos > 0){
    auto part = Part::createShared(Part::FUNCTION_CONST, booster::String(reinterpret_cast<const char*>(&data[lastPos]), i - lastPos));
    result->m_parts->push_back(part);
  }
  
  return result;
}

std::shared_ptr<Pattern> Pattern::parse(const char* data){
  return parse(reinterpret_cast<p_char8>(const_cast<char*>(data)), static_cast<v_buff_size>(std::strlen(data)));
}

std::shared_ptr<Pattern> Pattern::parse(const booster::String& data){
  return parse(reinterpret_cast<p_char8>(data->data()), static_cast<v_buff_size>(data->size()));
}
  
v_char8 Pattern::findSysChar(booster::utils::parser::Caret& caret) {
  auto data = caret.getData();
  for (v_buff_size i = caret.getPosition(); i < caret.getDataSize(); i++) {
    v_char8 a = static_cast<v_char8>(data[i]);
    if(a == '/' || a == '?') {
      caret.setPosition(i);
      return a;
    }
  }
  caret.setPosition(caret.getDataSize());
  return 0;
}
  
bool Pattern::match(const StringKeyLabel& url, MatchMap& matchMap) {
  
  booster::utils::parser::Caret caret(reinterpret_cast<const char*>(url.getData()), url.getSize());
  
  if (m_parts->empty()) {
    return !caret.skipChar('/');    
  }
  
  auto curr = std::begin(*m_parts);
  const auto end = std::end(*m_parts);
  while(curr != end){
    const std::shared_ptr<Part>& part = *curr;
    ++curr;
    caret.skipChar('/');
    
    if(part->function == Part::FUNCTION_CONST){
      
      if(!caret.isAtText(part->text->data(), static_cast<v_buff_size>(part->text->size()), true)) {
        return false;
      }
      
      if(caret.canContinue() && !caret.isAtChar('/')){
        if(caret.isAtChar('?') && (curr == end || (*curr)->function == Part::FUNCTION_ANY_END)) {
          matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getDataSize() - caret.getPosition());
          return true;
        }
        return false;
      }
      
    }else if(part->function == Part::FUNCTION_ANY_END){
      if(caret.getDataSize() > caret.getPosition()){
        matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getDataSize() - caret.getPosition());
      }
      return true;
    }else if(part->function == Part::FUNCTION_VAR){
      
      if(!caret.canContinue()){
        return false;
      }

      auto label = caret.putLabel();
      v_char8 a = findSysChar(caret);
      if(a == '?') {
        if(curr == end || (*curr)->function == Part::FUNCTION_ANY_END) {
          matchMap.m_variables[part->text] = StringKeyLabel(url.getMemoryHandle(), label.getData(), label.getSize());
          matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getDataSize() - caret.getPosition());
          return true;
        }
        caret.findChar('/');
      }
      
      matchMap.m_variables[part->text] = StringKeyLabel(url.getMemoryHandle(), label.getData(), label.getSize());
      
    }
    
  }
  
  caret.skipChar('/');
  return !caret.canContinue();
  
}

booster::String Pattern::toString() {
  booster::data::stream::BufferOutputStream stream;
  for (const std::shared_ptr<Part>& part : *m_parts) {
    if(part->function == Part::FUNCTION_CONST) {
      stream.writeSimple("/", 1);
      stream.writeSimple(part->text);
    } else if(part->function == Part::FUNCTION_VAR) {
      stream.writeSimple("/{", 2);
      stream.writeSimple(part->text);
      stream.writeSimple("}", 1);
    } else if(part->function == Part::FUNCTION_ANY_END) {
      stream.writeSimple("/*", 2);
    }
  }
  return stream.toString();
}
  
}}}}
