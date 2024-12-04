

#ifndef booster_data_type_Tree_hpp
#define booster_data_type_Tree_hpp

#include "./Type.hpp"
#include "./Primitive.hpp"

namespace booster { namespace data { namespace mapping {

class Tree; // FWD

}}}

namespace booster { namespace data { namespace type {

namespace __class {


class Tree {
public:


  static const ClassId CLASS_ID;

  static Type *getType();

};

}

class Tree : public ObjectWrapper<mapping::Tree, __class::Tree> {
public:


  Tree();


  Tree(std::nullptr_t);


  Tree(const Tree& other);


  Tree(Tree&& other);


  Tree(const mapping::Tree& other);


  Tree(mapping::Tree&& other);

  Tree(const std::shared_ptr<mapping::Tree>& node, const Type* const type);

  Tree& operator = (std::nullptr_t);

  Tree& operator = (const Tree& other);
  Tree& operator = (Tree&& other);

  Tree& operator = (const mapping::Tree& other);
  Tree& operator = (mapping::Tree&& other);

  bool operator == (std::nullptr_t) const;
  bool operator != (std::nullptr_t) const;

  bool operator == (const Tree& other) const;
  bool operator != (const Tree& other) const;

  mapping::Tree* operator->();
  mapping::Tree* operator->() const;

  mapping::Tree& operator*();
  const mapping::Tree& operator*() const;

  mapping::Tree& operator [] (const String& key);
  const mapping::Tree& operator [] (const String& key) const;

  mapping::Tree& operator [] (v_uint64 index);
  const mapping::Tree& operator [] (v_uint64 index) const;

};

}}}

#endif //booster_data_type_Tree_hpp
