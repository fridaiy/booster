

#ifndef booster_base_Compiler_hpp
#define booster_base_Compiler_hpp

#ifdef __GNUC__
  #define GPP_ATTRIBUTE(x) __attribute__((x))
#else
  #define GPP_ATTRIBUTE(x)
#endif

#endif 
