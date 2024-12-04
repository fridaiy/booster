



#ifndef booster_macro_component_hpp
#define booster_macro_component_hpp

#include "./basic.hpp"
#include "booster/Environment.hpp"

#define BOOSTER_MACRO_GET_COMPONENT_1(TYPE) \
(*(reinterpret_cast<TYPE*>(booster::Environment::getComponent(typeid(TYPE).name()))))

#define BOOSTER_MACRO_GET_COMPONENT_2(TYPE, QUALIFIER) \
(*((TYPE*) booster::Environment::getComponent(typeid(TYPE).name(), QUALIFIER)))

#define BOOSTER_GET_COMPONENT(...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_SELECTOR(BOOSTER_MACRO_GET_COMPONENT_, (__VA_ARGS__)) (__VA_ARGS__))


#define BOOSTER_MACRO_COMPONENT_1(TYPE, NAME) \
TYPE& NAME = (*(reinterpret_cast<TYPE*>(booster::Environment::getComponent(typeid(TYPE).name()))))

#define BOOSTER_MACRO_COMPONENT_2(TYPE, NAME, QUALIFIER) \
TYPE& NAME = (*((TYPE*) booster::Environment::getComponent(typeid(TYPE).name(), QUALIFIER)))


#define BOOSTER_COMPONENT(TYPE, ...) \
BOOSTER_MACRO_EXPAND(BOOSTER_MACRO_MACRO_SELECTOR(BOOSTER_MACRO_COMPONENT_, (__VA_ARGS__)) (TYPE, __VA_ARGS__))


#define BOOSTER_CREATE_COMPONENT(TYPE, NAME) \
booster::Environment::Component<TYPE> NAME = booster::Environment::Component<TYPE>

#endif
