

#ifndef booster_data_stream_FileStream_hpp
#define booster_data_stream_FileStream_hpp

#include "Stream.hpp"

#include <cstdio>

namespace booster { namespace data{ namespace stream {


class FileInputStream : public InputStream {
public:
  static booster::data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
private:
  std::FILE* m_file;
  bool m_ownsFile;
  IOMode m_ioMode;
private:
  std::shared_ptr<void> m_capturedData;
public:

  FileInputStream(const FileInputStream&) = delete;
  FileInputStream &operator=(const FileInputStream&) = delete;


  FileInputStream(FileInputStream&& other);


  FileInputStream(std::FILE* file, bool ownsFile, const std::shared_ptr<void>& captureData = nullptr);


  FileInputStream(const char* filename, const std::shared_ptr<void>& captureData = nullptr);


  ~FileInputStream() override;


  std::FILE* getFile();


  v_io_size read(void *data, v_buff_size count, async::Action& action) override;


  void setInputStreamIOMode(IOMode ioMode) override;


  IOMode getInputStreamIOMode() override;


  Context& getInputStreamContext() override;


  void close();

  FileInputStream& operator=(FileInputStream&& other);

};


class FileOutputStream : public OutputStream {
public:
  static booster::data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
private:
  std::FILE* m_file;
  bool m_ownsFile;
  IOMode m_ioMode;
private:
  std::shared_ptr<void> m_capturedData;
public:

  FileOutputStream(const FileOutputStream&) = delete;
  FileOutputStream &operator=(const FileOutputStream&) = delete;


  FileOutputStream(FileOutputStream&& other);


  FileOutputStream(std::FILE* file, bool ownsFile, const std::shared_ptr<void>& captureData = nullptr);


  FileOutputStream(const char* filename, const char* mode = "wb", const std::shared_ptr<void>& captureData = nullptr);


  ~FileOutputStream() override;


  std::FILE* getFile();


  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


  void setOutputStreamIOMode(IOMode ioMode) override;


  IOMode getOutputStreamIOMode() override;


  Context& getOutputStreamContext() override;


  void close();

  FileOutputStream& operator=(FileOutputStream&& other);

};

}}}

#endif // booster_data_stream_FileStream_hpp
