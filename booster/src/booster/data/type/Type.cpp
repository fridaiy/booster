

#include "Type.hpp"

namespace booster { namespace data { namespace type {

namespace __class {
  
  const ClassId Void::CLASS_ID("Void");
  
  Type* Void::getType(){
    static Type type(CLASS_ID);
    return &type;
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClassId

std::mutex& ClassId::getClassMutex() {
  static std::mutex classMutex;
  return classMutex;
}

std::vector<const char*>& ClassId::getClassNames() {
  static std::vector<const char*> classNames;
  return classNames;
}

v_int32 ClassId::registerClassName(const char* name) {
  std::lock_guard<std::mutex> lock(getClassMutex());
  getClassNames().push_back(name);
  return static_cast<v_int32>(getClassNames().size()) - 1;
}

ClassId::ClassId(const char* pName)
  : name(pName)
  , id(registerClassName(pName))
{}

int ClassId::getClassCount() {
  std::lock_guard<std::mutex> lock(getClassMutex());
  return static_cast<int>(getClassNames().size());
}

std::vector<const char*> ClassId::getRegisteredClassNames() {
  std::lock_guard<std::mutex> lock(getClassMutex());
  return getClassNames();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Type

Type::Type(const ClassId& pClassId, const Info& typeInfo)
  : classId(pClassId)
  , nameQualifier(typeInfo.nameQualifier)
  , params(typeInfo.params)
  , polymorphicDispatcher(typeInfo.polymorphicDispatcher)
  , interpretationMap(typeInfo.interpretationMap)
  , parent(typeInfo.parent)
  , isCollection(typeInfo.isCollection)
  , isMap(typeInfo.isMap)
{}

const Type::AbstractInterpretation* Type::findInterpretation(const std::vector<std::string>& names) const {
  for(const std::string& name : names) {
    auto it = interpretationMap.find(name);
    if(it != interpretationMap.end()) {
      return it->second;
    }
  }
  return nullptr;
}

bool Type::extends(const Type* other) const {
  const Type* curr = this;
  while(curr != nullptr) {
    if(curr == other) {
      return true;
    }
    curr = curr->parent;
  }
  return false;
}

}}}
