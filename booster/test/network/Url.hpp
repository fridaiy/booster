

#ifndef booster_network_Url_hpp
#define booster_network_Url_hpp

#include "booster/data/share/LazyStringMap.hpp"
#include "booster/utils/parser/Caret.hpp"
#include "booster/Types.hpp"

namespace booster { namespace network {



class Url {
public:

  typedef booster::data::share::StringKeyLabel StringKeyLabel;
public:

  typedef booster::data::share::LazyStringMultimap<booster::data::share::StringKeyLabel> Parameters;

public:


  struct Authority {

    booster::String userInfo;


    booster::String host;


    v_int32 port = -1;
  };
  
public:


  class Parser {
  public:
    

    static booster::String parseScheme(booster::utils::parser::Caret& caret);
    

    static Url::Authority parseAuthority(booster::utils::parser::Caret& caret);
    

    static booster::String parsePath(booster::utils::parser::Caret& caret);
    

    static void parseQueryParams(Url::Parameters& params, booster::utils::parser::Caret& caret);
    

    static void parseQueryParams(Url::Parameters& params, const booster::String& str);
    

    static Url::Parameters parseQueryParams(booster::utils::parser::Caret& caret);
    

    static Url::Parameters parseQueryParams(const booster::String& str);


    static Url parseUrl(booster::utils::parser::Caret& caret);


    static Url parseUrl(const booster::String& str);
    
  };
  
public:


  booster::String scheme;


  Authority authority;


  booster::String path;


  Parameters queryParams;
  
};
  
}}

#endif
