

#include "MemoryLabel.hpp"

#include <cstring>

namespace booster { namespace data { namespace share {

MemoryLabel::MemoryLabel(const std::shared_ptr<std::string>& memHandle, const void* data, v_buff_size size)
  : m_memoryHandle(memHandle)
  , m_data(data)
  , m_size(size)
{}
  
StringKeyLabel::StringKeyLabel(const std::shared_ptr<std::string>& memHandle, const char* data, v_buff_size size)
  : booster::data::share::MemoryLabel(memHandle, data, size)
{}
  
StringKeyLabel::StringKeyLabel(const char* constText)
  : booster::data::share::MemoryLabel(nullptr, constText, static_cast<v_buff_size>(std::strlen(constText)))
{}
  
StringKeyLabel::StringKeyLabel(const String& str)
  : booster::data::share::MemoryLabel(str.getPtr(), str->data(), static_cast<v_buff_size>(str->size()))
{}
  
StringKeyLabelCI::StringKeyLabelCI(const std::shared_ptr<std::string>& memHandle, const char* data, v_buff_size size)
  : booster::data::share::MemoryLabel(memHandle, data, size)
{}

StringKeyLabelCI::StringKeyLabelCI(const char* constText)
  : booster::data::share::MemoryLabel(nullptr, constText, static_cast<v_buff_size>(std::strlen(constText)))
{}

StringKeyLabelCI::StringKeyLabelCI(const String& str)
  : booster::data::share::MemoryLabel(str.getPtr(), str->data(), static_cast<v_buff_size>(str->size()))
{}
  
}}}
