

#ifndef booster_data_resource_File_hpp
#define booster_data_resource_File_hpp

#include "./Resource.hpp"
#include "booster/data/stream/Stream.hpp"

namespace booster { namespace data { namespace resource {


class File : public Resource {
private:
  
  struct FileHandle {

    booster::String fileName;

    FileHandle(const booster::String& fullFileName)
      : fileName(fullFileName)
    {}

  };

public:
  static booster::String concatDirAndName(const booster::String& dir, const booster::String& filename);
private:
  std::shared_ptr<FileHandle> m_handle;
public:


  File() = default;


  File(const booster::String& fullFilename);


  File(const booster::String& directory, const booster::String& filename);


  std::shared_ptr<data::stream::OutputStream> openOutputStream() override;


  std::shared_ptr<data::stream::InputStream> openInputStream() override;


  booster::String getInMemoryData() override;


  v_int64 getKnownSize() override;


  booster::String getLocation() override;

};

}}}

#endif //booster_data_resource_File_hpp
