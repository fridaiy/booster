

#ifndef booster_network_virtual__Pipe_hpp
#define booster_network_virtual__Pipe_hpp

#include "booster/async/CoroutineWaitList.hpp"

#include "booster/data/stream/Stream.hpp"

#include "booster/data/buffer/FIFOBuffer.hpp"
#include "booster/data/buffer/IOBuffer.hpp"

#include "booster/concurrency/SpinLock.hpp"

#include <mutex>
#include <condition_variable>

namespace booster { namespace network { namespace virtual_ {


class Pipe : public booster::base::Countable {
public:


  class Reader : public booster::data::stream::InputStream {
    friend Pipe;
  public:
    static data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
  private:

    class WaitListListener : public booster::async::CoroutineWaitList::Listener {
    private:
      Pipe* m_pipe;
    public:

      WaitListListener(Pipe* pipe)
        : m_pipe(pipe)
      {}

      void onNewItem(booster::async::CoroutineWaitList& list) override {
        std::lock_guard<std::mutex> lock(m_pipe->m_mutex);
        if (m_pipe->m_fifo.availableToRead() > 0 || !m_pipe->m_open) {
          list.notifyAll();
        }
      }

    };

  private:
    Pipe* m_pipe;
    booster::data::stream::IOMode m_ioMode;
    

    v_io_size m_maxAvailableToRead;

    booster::async::CoroutineWaitList m_waitList;
    WaitListListener m_waitListListener;
  protected:
    
    Reader(Pipe* pipe, booster::data::stream::IOMode ioMode = booster::data::stream::IOMode::BLOCKING)
      : m_pipe(pipe)
      , m_ioMode(ioMode)
      , m_maxAvailableToRead(-1)
      , m_waitListListener(pipe)
    {
      m_waitList.setListener(&m_waitListListener);
    }

  public:


    void setMaxAvailableToRead(v_io_size maxAvailableToRead);


    v_io_size read(void *data, v_buff_size count, async::Action& action) override;


    void setInputStreamIOMode(booster::data::stream::IOMode ioMode) override;


    booster::data::stream::IOMode getInputStreamIOMode() override;


    booster::data::stream::Context& getInputStreamContext() override;


    void notifyWaitList();
    
  };


  class Writer : public booster::data::stream::OutputStream {
    friend Pipe;
  public:
    static data::stream::DefaultInitializedContext DEFAULT_CONTEXT;
  private:
    class WaitListListener : public booster::async::CoroutineWaitList::Listener {
    private:
      Pipe* m_pipe;
    public:
      WaitListListener(Pipe* pipe)
        : m_pipe(pipe)
      {}

      void onNewItem(booster::async::CoroutineWaitList& list) override {
        std::lock_guard<std::mutex> lock(m_pipe->m_mutex);
        if (m_pipe->m_fifo.availableToWrite() > 0 || !m_pipe->m_open) {
          list.notifyAll();
        }
      }
    };
  private:
    Pipe* m_pipe;
    booster::data::stream::IOMode m_ioMode;
    

    v_io_size m_maxAvailableToWrtie;

    booster::async::CoroutineWaitList m_waitList;
    WaitListListener m_waitListListener;
  protected:
    
    Writer(Pipe* pipe, booster::data::stream::IOMode ioMode = booster::data::stream::IOMode::BLOCKING)
      : m_pipe(pipe)
      , m_ioMode(ioMode)
      , m_maxAvailableToWrtie(-1)
      , m_waitListListener(pipe)
    {
      m_waitList.setListener(&m_waitListListener);
    }

  public:


    void setMaxAvailableToWrite(v_io_size maxAvailableToWrite);


    v_io_size write(const void *data, v_buff_size count, async::Action& action) override;


    void setOutputStreamIOMode(booster::data::stream::IOMode ioMode) override;


    booster::data::stream::IOMode getOutputStreamIOMode() override;


    booster::data::stream::Context& getOutputStreamContext() override;


    void notifyWaitList();
    
  };
  
private:
  bool m_open;
  Writer m_writer;
  Reader m_reader;

  booster::data::buffer::IOBuffer m_buffer;
  booster::data::buffer::SynchronizedFIFOBuffer m_fifo;

  std::mutex m_mutex;
  std::condition_variable m_conditionRead;
  std::condition_variable m_conditionWrite;
public:


  Pipe();


  static std::shared_ptr<Pipe> createShared();


  virtual ~Pipe() override;


  Writer* getWriter();



  Reader* getReader();


  void close();
  
};
  
}}}

#endif
