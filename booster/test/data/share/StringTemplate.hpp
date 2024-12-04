

#ifndef booster_data_share_StringTemplate_hpp
#define booster_data_share_StringTemplate_hpp

#include "booster/data/stream/Stream.hpp"
#include "booster/Types.hpp"

#include <unordered_map>
#include <vector>

namespace booster { namespace data { namespace share {


class StringTemplate {
public:


  struct Variable {


    v_buff_size posStart;


    v_buff_size posEnd;


    booster::String name;


    std::shared_ptr<void> extra;
  };

public:


  class ValueProvider {
  public:

    virtual ~ValueProvider() = default;


    virtual booster::String getValue(const Variable& variable, v_uint32 index) = 0;
  };


  class VectorValueProvider : public ValueProvider {
  private:
    const std::vector<booster::String>* m_params;
  public:
    VectorValueProvider(const std::vector<booster::String>* params);
    booster::String getValue(const Variable& variable, v_uint32 index) override;
  };


  class MapValueProvider : public ValueProvider {
  private:
    const std::unordered_map<booster::String, booster::String>* m_params;
  public:
    MapValueProvider(const std::unordered_map<booster::String, booster::String>* params);
    booster::String getValue(const Variable& variable, v_uint32 index) override;
  };


  class SingleValueProvider : public ValueProvider {
  private:
    booster::String m_value;
  public:
    SingleValueProvider(const booster::String& value);
    booster::String getValue(const Variable& variable, v_uint32 index) override;
  };

private:
  booster::String m_text;
  std::vector<Variable> m_variables;
  std::shared_ptr<void> m_extra;
public:


  StringTemplate(const booster::String& text, std::vector<Variable>&& variables);


  void format(stream::ConsistentOutputStream* stream, ValueProvider* valueProvider) const;


  void format(stream::ConsistentOutputStream* stream, const std::vector<booster::String>& params) const;


  void format(stream::ConsistentOutputStream* stream, const std::unordered_map<booster::String, booster::String>& params) const;


  void format(stream::ConsistentOutputStream* stream, const booster::String& singleValue) const;


  booster::String format(ValueProvider* valueProvider) const;


  booster::String format(const std::vector<booster::String>& params) const;


  booster::String format(const std::unordered_map<booster::String, booster::String>& params) const;


  booster::String format(const booster::String& singleValue) const;


  const std::vector<Variable>& getTemplateVariables() const;


  void setExtraData(const std::shared_ptr<void>& data);


  std::shared_ptr<void> getExtraData() const;

};

}}}

#endif // booster_data_share_StringTemplate_hpp
