

#ifndef booster_network_virtual__Interface_hpp
#define booster_network_virtual__Interface_hpp

#include "./Socket.hpp"

#include <list>
#include <unordered_map>
#include <chrono>

namespace booster { namespace network { namespace virtual_ {


class Interface : public booster::base::Countable {
private:
  static std::recursive_mutex m_registryMutex;
  static std::unordered_map<booster::String, std::weak_ptr<Interface>> m_registry;
private:
  static void registerInterface(const std::shared_ptr<Interface>& _interface);
  static void unregisterInterface(const booster::String& name);
public:


  class ListenerLock {
    friend Interface;
  private:
    Interface* m_interface;
  private:
    ListenerLock(Interface* _interface);
  public:
    ~ListenerLock();
  };

public:


  class ConnectionSubmission {
    friend Interface;
  private:
    std::shared_ptr<Socket> m_socket;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_valid;
  private:
    void invalidate();
  public:


    ConnectionSubmission(bool valid) : m_valid(valid) {}


    void setSocket(const std::shared_ptr<Socket>& socket);


    std::shared_ptr<Socket> getSocket();


    std::shared_ptr<Socket> getSocketNonBlocking();


    bool isValid();
    
  };

private:
  std::shared_ptr<Socket> acceptSubmission(const std::shared_ptr<ConnectionSubmission>& submission);
  void unbindListener(ListenerLock* listenerLock);
private:
  booster::String m_name;
  std::atomic<ListenerLock*> m_listenerLock;
  std::mutex m_listenerMutex;
  std::mutex m_mutex;
  std::condition_variable m_condition;
  std::list<std::shared_ptr<ConnectionSubmission>> m_submissions;
private:

  Interface(const booster::String& name);

  Interface(const Interface& other) = delete;
  Interface(Interface&& other) = delete;
  Interface& operator=(const Interface&) = delete;
  Interface& operator=(Interface&&) = delete;

public:


  ~Interface() override;


  static std::shared_ptr<Interface> obtainShared(const booster::String& name);


  std::shared_ptr<ListenerLock> bind();


  std::shared_ptr<ConnectionSubmission> connect();


  std::shared_ptr<ConnectionSubmission> connectNonBlocking();


  std::shared_ptr<Socket> accept(const bool& waitingHandle = true,
                                 const std::chrono::duration<v_int64, std::micro>& timeout = std::chrono::minutes (10));


  std::shared_ptr<Socket> acceptNonBlocking();


  void dropAllConnection();


  void notifyAcceptors();


  booster::String getName() {
    return m_name;
  }
  
};
  
}}}

#endif
