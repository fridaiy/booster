

#ifndef booster_data_resource_TemporaryFile_hpp
#define booster_data_resource_TemporaryFile_hpp

#include "./Resource.hpp"
#include "booster/Types.hpp"

namespace booster { namespace data { namespace resource {


class TemporaryFile : public Resource {
private:


  struct FileHandle {

    booster::String fileName;

    FileHandle(const booster::String& fullFileName)
      : fileName(fullFileName)
    {}

    ~FileHandle();
  };

private:
  static booster::String constructRandomFilename(const booster::String &dir, v_int32 randomWordSizeBytes, const booster::String &extension);
private:
  std::shared_ptr<FileHandle> m_handle;
public:


  TemporaryFile() = default;


  TemporaryFile(const booster::String& tmpDirectory, v_int32 randomWordSizeBytes = 8);


  TemporaryFile(const booster::String& tmpDirectory, const booster::String& tmpFileName);


  TemporaryFile(const booster::String& tmpDirectory, v_int32 randomWordSizeBytes, const booster::String& extension);


  std::shared_ptr<data::stream::OutputStream> openOutputStream() override;


  std::shared_ptr<data::stream::InputStream> openInputStream() override;


  booster::String getInMemoryData() override;


  v_int64 getKnownSize() override;


  booster::String getLocation() override;


  bool moveFile(const booster::String& fullFileName);

};

}}}

#endif //booster_data_resource_TemporaryFile_hpp
