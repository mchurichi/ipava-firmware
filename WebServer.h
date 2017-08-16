#include <ESP8266WebServer.h>

class WebServer {
  public:
    WebServer(String deviceId);
    void start();
    void handle();
  private:
    String deviceId;
    ESP8266WebServer webServer;
    void setupRoutes();
    void handleRootRedirect();
    void handleStatus();
    void handleWifiScan();
    void handleWifiStatus();
};