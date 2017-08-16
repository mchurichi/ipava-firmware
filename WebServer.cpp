#include "WebServer.h"
#include <ESP8266WiFi.h>
#include "FS.h"
#include "libraries\\ArduinoJson.h"

WebServer::WebServer(String deviceId) : webServer(80) {
  this->deviceId = deviceId;
}

void WebServer::start() {
  this->setupRoutes();
  this->webServer.begin();
}

void WebServer::handle() {
  this->webServer.handleClient();
}

void WebServer::setupRoutes() {
  this->webServer.on("/", HTTP_GET, [&]() { this->handleRootRedirect(); });
  this->webServer.on("/api/status", HTTP_GET, [&]() { this->handleStatus(); });
  this->webServer.on("/api/wifi/scan", HTTP_GET, [&]() { this->handleWifiScan(); });
  this->webServer.on("/api/wifi/status", HTTP_GET, [&]() { this->handleWifiStatus(); });
  this->webServer.serveStatic("/", SPIFFS, "/");
  this->webServer.onNotFound([&]() { this->handleRootRedirect(); });  
}

void WebServer::handleRootRedirect() {
  this->webServer.sendHeader("Location", "/index.html", true);
  this->webServer.send(302, "text/plain", "");
}

void WebServer::handleStatus() {
  String jsonString;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root.set("deviceId", this->deviceId);
  root.set("auth", true);
  root.set("success", true);

  JsonObject& response = root.createNestedObject("response");
  response.set("calentando", true); //llenoDeAgua();
  response.set("temperatura", 123.45); //getTemperaturaDelAgua();

  root.printTo(jsonString);
  this->webServer.send(200, "application/json", jsonString);
}

void WebServer::handleWifiScan() {
  String jsonString;
  StaticJsonBuffer<1000> jsonBuffer;
  
  JsonObject& root = jsonBuffer.createObject();
  root.set("deviceId", this->deviceId);
  root.set("auth", true);
  root.set("success", true);

  JsonObject& response = root.createNestedObject("response");
  JsonArray& networks = response.createNestedArray("networks");
  
  int found = WiFi.scanNetworks();
  for (int i = 0; i < found; i++) {
    JsonObject& networkObj = jsonBuffer.createObject();
    networkObj.set("ssid", WiFi.SSID(i));
    networkObj.set("rssi", WiFi.RSSI(i));
    networkObj.set("encType", WiFi.encryptionType(i));
    networks.add(networkObj);
  }

  root.printTo(jsonString);
  this->webServer.send(200, "application/json", jsonString);  
}

void WebServer::handleWifiStatus() {
  String jsonString;
  StaticJsonBuffer<1000> jsonBuffer;
  
  JsonObject& root = jsonBuffer.createObject();
  root.set("deviceId", this->deviceId);
  root.set("auth", true);
  root.set("success", true);

  JsonObject& response = root.createNestedObject("response");

  JsonObject& station = response.createNestedObject("station");
  station.set("connected", WiFi.isConnected());
  station.set("ssid", WiFi.SSID());
  station.set("ip", WiFi.localIP().toString());
  station.set("subnetMask", WiFi.subnetMask().toString());
  station.set("gateway", WiFi.gatewayIP().toString());
  station.set("dns1", WiFi.dnsIP().toString());
  station.set("dns2", WiFi.dnsIP(1).toString());
  station.set("macAddress", String(WiFi.macAddress().c_str()));

  JsonObject& accessPoint = response.createNestedObject("accessPoint");
  accessPoint.set("ssid", this->deviceId);
  accessPoint.set("ip", WiFi.softAPIP().toString());
  accessPoint.set("clients", WiFi.softAPgetStationNum());
  // station.set("subnetMask", WiFi.subnetMask());
  // station.set("gateway", WiFi.gatewayIP());
  // station.set("DNS 1", WiFi.dnsIP());
  // station.set("DNS 2", WiFi.dnsIP(1));
  accessPoint.set("macAddress", String(WiFi.softAPmacAddress().c_str()));

  root.printTo(jsonString);
  this->webServer.send(200, "application/json", jsonString);  
}

// void sendFile(String path) {
//   File f = SPIFFS.open(path, "r");
//   if (!f) {
//     server.send(200, "text/html", "index not found! :(");
//   } else {
//     server.streamFile(f, "text/html");
//   }
//   f.close();
// }