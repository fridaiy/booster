

#ifndef booster_network_Server_hpp
#define booster_network_Server_hpp

#include "booster/network/ConnectionHandler.hpp"
#include "booster/network/ConnectionProvider.hpp"

#include "booster/Types.hpp"

#include "booster/base/Countable.hpp"
#include "booster/Environment.hpp"

#include <atomic>
#include <thread>
#include <functional>

namespace booster { namespace network {


class Server : public base::Countable {
private:

  static void mainLoop(Server *instance);
  void conditionalMainLoop();

  bool setStatus(v_int32 expectedStatus, v_int32 newStatus);
  void setStatus(v_int32 status);

private:

  std::atomic<v_int32> m_status;
  std::function<bool()> m_condition;
  std::thread m_thread;
  std::mutex m_mutex;

  std::shared_ptr<ConnectionProvider> m_connectionProvider;
  std::shared_ptr<ConnectionHandler> m_connectionHandler;

  bool m_threaded;
  
public:


  Server(const std::shared_ptr<ConnectionProvider>& connectionProvider,
         const std::shared_ptr<ConnectionHandler>& connectionHandler);

  virtual ~Server() override;

 public:


  static const v_int32 STATUS_CREATED;


  static const v_int32 STATUS_STARTING;


  static const v_int32 STATUS_RUNNING;


  static const v_int32 STATUS_STOPPING;


  static const v_int32 STATUS_DONE;


  static std::shared_ptr<Server> createShared(const std::shared_ptr<ServerConnectionProvider>& connectionProvider,
                                              const std::shared_ptr<ConnectionHandler>& connectionHandler){
    return std::make_shared<Server>(connectionProvider, connectionHandler);
  }


  void run(std::function<bool()> conditional = nullptr);


  void run(bool startAsNewThread);


  void stop();


  v_int32 getStatus();

};

}}

#endif
