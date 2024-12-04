

#include "./Object.hpp"

namespace booster { namespace data { namespace type {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BaseObject

void BaseObject::set(v_int64 offset, const Void& value) {
  Void* property = reinterpret_cast<Void*>((reinterpret_cast<v_int64>(m_basePointer)) + offset);
  *property = value;
}

Void BaseObject::get(v_int64 offset) const {
  Void* property = reinterpret_cast<Void*>((reinterpret_cast<v_int64>(m_basePointer)) + offset);
  return *property;
}

Void& BaseObject::getAsRef(v_int64 offset) const {
  Void* property = reinterpret_cast<Void*>((reinterpret_cast<v_int64>(m_basePointer)) + offset);
  return *property;
}

void BaseObject::setBasePointer(void* basePointer) {
  m_basePointer = basePointer;
}

void* BaseObject::getBasePointer() const {
  return m_basePointer;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BaseObject::Properties

BaseObject::Property* BaseObject::Properties::pushBack(Property* property) {
  m_map.insert({property->name, property});
  m_unqualifiedMap.insert({property->unqualifiedName, property});
  m_list.push_back(property);
  return property;
}

void BaseObject::Properties::pushFrontAll(Properties* properties) {
  m_map.insert(properties->m_map.begin(), properties->m_map.end());
  m_unqualifiedMap.insert(properties->m_unqualifiedMap.begin(), properties->m_unqualifiedMap.end());
  m_list.insert(m_list.begin(), properties->m_list.begin(), properties->m_list.end());
}

const std::unordered_map<std::string, BaseObject::Property*>& BaseObject::Properties::getMap() const {
  return m_map;
}

const std::unordered_map<std::string, BaseObject::Property*>& BaseObject::Properties::getUnqualifiedMap() const {
  return m_unqualifiedMap;
}

const std::list<BaseObject::Property*>& BaseObject::Properties::getList() const {
  return m_list;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BaseObject::Property

BaseObject::Property::Property(v_int64 pOffset, std::string pName, std::string pUName, const Type* pType)
  : offset(pOffset)
  , name(std::move(pName))
  , unqualifiedName(std::move(pUName))
  , type(pType)
{}

void BaseObject::Property::set(BaseObject* object, const Void& value) const {
  object->set(offset, value);
}

Void BaseObject::Property::get(BaseObject* object) const {
  return object->get(offset);
}

Void& BaseObject::Property::getAsRef(BaseObject* object) const {
  return object->getAsRef(offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Object

namespace __class {

  const ClassId AbstractObject::CLASS_ID("Object");

}

const type::Type* DTO::getParentType() {
  return nullptr;
}

const char* DTO::Z__CLASS_TYPE_NAME() {
  return "DTO";
}

booster::data::type::BaseObject::Properties* DTO::Z__CLASS_GET_FIELDS_MAP() {
  static data::type::BaseObject::Properties map;
  return &map;
}

BaseObject::Properties* DTO::Z__CLASS_EXTEND(BaseObject::Properties* properties, BaseObject::Properties* extensionProperties) {
  properties->pushFrontAll(extensionProperties);
  return properties;
}
  
}}}
