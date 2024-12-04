#include "./controller/MyController.hpp"
#include "./AppComponent.hpp"

#include <booster/network/Server.hpp>

#include <iostream>

void run() {


  AppComponent components;


  BOOSTER_COMPONENT(std::shared_ptr<booster::web::server::HttpRouter>, router);


  router->addController(std::make_shared<MyController>());


  BOOSTER_COMPONENT(std::shared_ptr<booster::network::ConnectionHandler>, connectionHandler);


  BOOSTER_COMPONENT(std::shared_ptr<booster::network::ServerConnectionProvider>, connectionProvider);

  booster::network::Server server(connectionProvider, connectionHandler);


  BOOSTER_LOGi("MyApp", "Server running on port {}", connectionProvider->getProperty("port").toString())

  server.run();

}

int main(int argc, const char * argv[]) {

  booster::Environment::init();

  run();

  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << booster::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << booster::Environment::getObjectsCreated() << "\n\n";

  booster::Environment::destroy();
  return 0;
}
