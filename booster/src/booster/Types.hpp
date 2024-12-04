

#ifndef booster_Types_hpp
#define booster_Types_hpp

#include "booster/data/type/Object.hpp"
#include "booster/data/mapping/Tree.hpp"

namespace booster {


  typedef booster::data::type::Type Type;


  typedef booster::data::type::ClassId ClassId;


  template <class T, class Clazz = booster::data::type::__class::Void>
  using ObjectWrapper = booster::data::type::ObjectWrapper<T, Clazz>;


  typedef booster::data::type::Tree Tree;


  typedef booster::data::type::Void Void;


  typedef booster::data::type::Any Any;


  typedef booster::data::type::String String;


  typedef booster::data::type::Int8 Int8;


  typedef booster::data::type::UInt8 UInt8;


  typedef booster::data::type::Int16 Int16;


  typedef booster::data::type::UInt16 UInt16;


  typedef booster::data::type::Int32 Int32;


  typedef booster::data::type::UInt32 UInt32;


  typedef booster::data::type::Int64 Int64;


  typedef booster::data::type::UInt64 UInt64;


  typedef booster::data::type::Float32 Float32;


  typedef booster::data::type::Float64 Float64;


  typedef booster::data::type::Boolean Boolean;


  typedef booster::data::type::BaseObject BaseObject;


  typedef booster::data::type::DTO DTO;


  template <class T>
  using Object = booster::data::type::DTOWrapper<T>;


  template <class T>
  using Enum = booster::data::type::Enum<T>;


  template <class T>
  using Vector = booster::data::type::Vector<T>;


  typedef booster::data::type::AbstractVector AbstractVector;


  template <class T>
  using List = booster::data::type::List<T>;


  typedef booster::data::type::AbstractList AbstractList;


  template <class T>
  using UnorderedSet = booster::data::type::UnorderedSet<T>;


  typedef booster::data::type::AbstractUnorderedSet AbstractUnorderedSet;


  template <class Key, class Value>
  using PairList = booster::data::type::PairList<Key, Value>;


  template <class Value>
  using Fields = booster::PairList<String, Value>;


  typedef Fields<booster::Void> AbstractFields;


  template <class Key, class Value>
  using UnorderedMap = booster::data::type::UnorderedMap<Key, Value>;


  template <class Value>
  using UnorderedFields = booster::UnorderedMap<String, Value>;


  typedef UnorderedFields<booster::Void> AbstractUnorderedFields;
  
}

#endif
