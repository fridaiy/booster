

#ifndef booster_utils_parser_Caret_hpp
#define booster_utils_parser_Caret_hpp

#include "booster/Types.hpp"

namespace booster { namespace utils { namespace parser {


class Caret {
public:
  static const char* const ERROR_INVALID_INTEGER;
  static const char* const ERROR_INVALID_FLOAT;
  static const char* const ERROR_INVALID_BOOLEAN;
  static const char* const ERROR_NO_OPEN_TAG;
  static const char* const ERROR_NO_CLOSE_TAG;
  static const char* const ERROR_NAME_EXPECTED;
public:


  class Label {
  private:
    Caret* m_caret;
    v_buff_size m_start;
    v_buff_size m_end;
  public:


    Label(Caret* caret);


    void start();


    void end();


    const char* getData();


    v_buff_size getSize();


    v_buff_size getStartPosition();


    v_buff_size getEndPosition();


    booster::String toString();


    std::string std_str();

    explicit operator bool() const;
    
  };


  class StateSaveGuard {
  private:
    Caret& m_caret;
    v_buff_size m_savedPosition;
    const char* m_savedErrorMessage;
    v_int64 m_savedErrorCode;
  public:


    StateSaveGuard(Caret& caret);


    ~StateSaveGuard();


    v_buff_size getSavedPosition();


    const char* getSavedErrorMessage();


    v_int64 getSavedErrorCode();

  };

private:
  const char* m_data;
  v_buff_size m_size;
  v_buff_size m_pos;
  const char* m_errorMessage;
  v_int64 m_errorCode;
  std::shared_ptr<std::string> m_dataMemoryHandle;
public:
  Caret(const char* text);
  Caret(const char* parseData, v_buff_size dataSize);
  Caret(const booster::String& str);
public:
  
  static std::shared_ptr<Caret> createShared(const char* text);
  static std::shared_ptr<Caret> createShared(const char* parseData, v_buff_size dataSize);
  static std::shared_ptr<Caret> createShared(const booster::String& str);

  virtual ~Caret();


  const char* getData();


  const char* getCurrData();


  v_buff_size getDataSize();


  std::shared_ptr<std::string> getDataMemoryHandle();


  void setPosition(v_buff_size position);


  v_buff_size getPosition();


  void setError(const char* errorMessage, v_int64 errorCode = 0);


  const char* getErrorMessage();


  v_int64 getErrorCode();


  bool hasError();


  void clearError();


  Label putLabel();


  void inc();


  void inc(v_buff_size amount);


  bool skipBlankChars();


  bool skipChar(char c);


  bool findChar(char c);


  bool skipCharsFromSet(const char* set);


  bool skipCharsFromSet(const char* set, v_buff_size setSize);


  v_buff_size findCharFromSet(const char* set);


  v_buff_size findCharFromSet(const char* set, v_buff_size setSize);


  bool findRN();


  bool skipRN();


  bool isAtRN();


  bool findROrN();


  bool skipRNOrN();


  bool skipAllRsAndNs();


  v_int64 parseInt(int base = 10);


  v_uint64 parseUnsignedInt(int base = 10);


  v_float32 parseFloat32();


  v_float64 parseFloat64();


  bool isAtText(const char* text, bool skipIfTrue = false);


  bool isAtText(const char* text, v_buff_size textSize, bool skipIfTrue = false);


  bool isAtTextNCS(const char* text, bool skipIfTrue = false);


  bool isAtTextNCS(const char* text, v_buff_size textSize, bool skipIfTrue = false);


  Label parseStringEnclosed(char openChar, char closeChar, char escapeChar);


  bool findText(const char* text);


  bool findText(const char* text, v_buff_size textSize);


  bool isAtCharFromSet(const char* set) const;


  bool isAtCharFromSet(const char* set, v_buff_size setSize) const;


  bool isAtChar(char c) const;


  bool isAtBlankChar() const;


  bool isAtDigitChar() const;


  bool canContinueAtChar(char c) const;


  bool canContinueAtChar(char c, v_buff_size skipChars);


  bool canContinue() const;

};
  
}}}

#endif
