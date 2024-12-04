

#ifndef booster_data_share_MemoryLabel_hpp
#define booster_data_share_MemoryLabel_hpp

#include "booster/data/type/Primitive.hpp"
#include "booster/utils/String.hpp"

#include <cstring>
#include <memory>

namespace booster { namespace data { namespace share {
  

class MemoryLabel {
public:
  typedef booster::data::type::String String;
protected:
  mutable std::shared_ptr<std::string> m_memoryHandle;
  mutable const void* m_data;
  v_buff_size m_size;
public:


  MemoryLabel()
    : m_memoryHandle(nullptr)
    , m_data(nullptr)
    , m_size(0)
  {}


  MemoryLabel(std::nullptr_t)
    : m_memoryHandle(nullptr)
    , m_data(nullptr)
    , m_size(0)
  {}


  MemoryLabel(const std::shared_ptr<std::string>& ptr) :
    MemoryLabel(
      ptr,
      ptr ? ptr->data() : nullptr,
      ptr ? static_cast<v_buff_size>(ptr->size()) :  0
    )
  {}


  MemoryLabel(const std::shared_ptr<std::string>& memHandle, const void* data, v_buff_size size);


  const void* getData() const {
    return m_data;
  }


  v_buff_size getSize() const {
    return m_size;
  }


  std::shared_ptr<std::string> getMemoryHandle() const {
    return m_memoryHandle;
  }


  void captureToOwnMemory() const {
    if(!m_memoryHandle || m_memoryHandle->data() != reinterpret_cast<const char*>(m_data) || static_cast<v_buff_size>(m_memoryHandle->size()) != m_size) {
      m_memoryHandle = std::make_shared<std::string>(reinterpret_cast<const char*>(m_data), m_size);
      m_data = m_memoryHandle->data();
    }
  }


  bool equals(const char* data) const {
    auto len = data != nullptr ? static_cast<v_buff_size>(std::strlen(data)) : 0;
    return utils::String::compare(m_data, m_size, data, len) == 0;
  }


  bool equals(const void* data, v_buff_size size) const {
    return utils::String::compare(m_data, m_size, data, size) == 0;
  }


  String toString() const {
    return String(reinterpret_cast<const char*>(m_data), m_size);
  }


  std::string std_str() const {
    return std::string(reinterpret_cast<const char*>(m_data), static_cast<size_t>(m_size));
  }

  inline bool operator==(std::nullptr_t) const {
    return m_data == nullptr;
  }

  inline bool operator!=(std::nullptr_t) const {
    return m_data != nullptr;
  }

  inline explicit operator bool() const {
    return m_data != nullptr;
  }
  
};


class StringKeyLabel : public MemoryLabel {
public:
  
  StringKeyLabel() : MemoryLabel() {}

  StringKeyLabel(std::nullptr_t) : MemoryLabel() {}


  StringKeyLabel(const std::shared_ptr<std::string>& ptr) : MemoryLabel(ptr) {}

  StringKeyLabel(const std::shared_ptr<std::string>& memoryHandle, const char* data, v_buff_size size);
  StringKeyLabel(const char* constText);
  StringKeyLabel(const String& str);

  inline bool operator==(std::nullptr_t) const {
    return m_data == nullptr;
  }

  inline bool operator!=(std::nullptr_t) const {
    return m_data != nullptr;
  }

  inline bool operator==(const char* str) const {
    return equals(str);
  }

  inline bool operator!=(const char* str) const {
    return !operator==(str);
  }

  inline bool operator==(const String& str) const {
    if(m_data == nullptr) return str == nullptr;
    if(str == nullptr) return false;
    return equals(str->data(), static_cast<v_buff_size>(str->size()));
  }

  inline bool operator!=(const String& str) const {
    return !operator==(str);
  }

  inline bool operator==(const StringKeyLabel &other) const {
    return utils::String::compare(m_data, m_size, other.m_data, other.m_size) == 0;
  }

  inline bool operator!=(const StringKeyLabel &other) const {
    return !operator==(other);
  }

  inline bool operator < (const StringKeyLabel &other) const {
    return utils::String::compare(m_data, m_size, other.m_data, other.m_size) < 0;
  }

  inline bool operator > (const StringKeyLabel &other) const {
    return utils::String::compare(m_data, m_size, other.m_data, other.m_size) > 0;
  }

};


class StringKeyLabelCI : public MemoryLabel {
public:
  
  StringKeyLabelCI() : MemoryLabel() {}

  StringKeyLabelCI(std::nullptr_t) : MemoryLabel() {}

  StringKeyLabelCI(const std::shared_ptr<std::string>& ptr) : MemoryLabel(ptr) {}

  StringKeyLabelCI(const std::shared_ptr<std::string>& memoryHandle, const char* data, v_buff_size size);
  StringKeyLabelCI(const char* constText);
  StringKeyLabelCI(const String& str);

  inline bool operator==(std::nullptr_t) const {
    return m_data == nullptr;
  }

  inline bool operator!=(std::nullptr_t) const {
    return m_data != nullptr;
  }

  inline bool operator==(const char* str) const {
    auto len = str != nullptr ? static_cast<v_buff_size>(std::strlen(str)) : 0;
    return utils::String::compareCI_ASCII(m_data, m_size, str, len) == 0;
  }

  inline bool operator!=(const char* str) const {
    return !operator==(str);
  }

  inline bool operator==(const String& str) const {
    if(m_data == nullptr) return str == nullptr;
    if(str == nullptr) return false;
    return utils::String::compareCI_ASCII(m_data, m_size, str->data(), static_cast<v_buff_size>(str->size())) == 0;
  }

  inline bool operator!=(const String& str) const {
    return !operator==(str);
  }

  inline bool operator==(const StringKeyLabelCI &other) const {
    return utils::String::compareCI_ASCII(m_data, m_size, other.m_data, other.m_size) == 0;
  }

  inline bool operator!=(const StringKeyLabelCI &other) const {
    return !operator==(other);
  }

  inline bool operator < (const StringKeyLabelCI &other) const {
    return utils::String::compareCI_ASCII(m_data, m_size, other.m_data, other.m_size) < 0;
  }

  inline bool operator > (const StringKeyLabelCI &other) const {
    return utils::String::compareCI_ASCII(m_data, m_size, other.m_data, other.m_size) > 0;
  }

};
  
}}}

namespace std {
  
  template<>
  struct hash<booster::data::share::StringKeyLabel> {
    
    typedef booster::data::share::StringKeyLabel argument_type;
    typedef v_uint64 result_type;
    
    result_type operator()(booster::data::share::StringKeyLabel const& s) const noexcept {

      auto data = reinterpret_cast<const char*>(s.getData());
      result_type result = 0;
      for(v_buff_size i = 0; i < s.getSize(); i++) {
        auto c = data[i];
        result = (31 * result) + static_cast<result_type>(c);
      }

      return result;

    }
  };
  
  template<>
  struct hash<booster::data::share::StringKeyLabelCI> {
    
    typedef booster::data::share::StringKeyLabelCI argument_type;
    typedef v_uint64 result_type;
    
    result_type operator()(booster::data::share::StringKeyLabelCI const& s) const noexcept {

      auto data = reinterpret_cast<const char*>(s.getData());
      result_type result = 0;
      for(v_buff_size i = 0; i < s.getSize(); i++) {
        auto c = data[i] | 32;
        result = (31 * result) + static_cast<result_type>(c);
      }

      return result;

    }
  };
  
}

#endif
