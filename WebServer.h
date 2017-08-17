#include <ESP8266WebServer.h>
#include "KittleService.h"

class WebServer {
  public:
    WebServer(String deviceId);
    void handle();
  private:
    String deviceId;
    ESP8266WebServer webServer;
    void setupRoutes();
    void handleRootRedirect();
    void handleStatus();
    void handleWifiScan();
    void handleWifiStatus();
    //kittle stuff
    KittleService* kittleSvc;
    unsigned long lastKittleUpdateTime;
    void initializeKittle();
    void beginKittleRequest();
    void updateKittleStatus();
    
};
