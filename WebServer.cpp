#include "WebServer.h"
#include <ESP8266WiFi.h>
#include "FS.h"
#include "libraries\\ArduinoJson.h"
#include "KittleService.h"

#define kittleUpdateDelayInMilliseconds 1000

WebServer::WebServer(String deviceId) : webServer(80) {
  this->deviceId = deviceId;
  this->setupRoutes();
  this->webServer.begin();
  initializeKittle();
}

void WebServer::handle() {
  this->webServer.handleClient();
  updateKittleStatus();
}

void WebServer::setupRoutes() {
  this->webServer.on("/", HTTP_GET, [&]() { this->handleRootRedirect(); });
  this->webServer.on("/api/status", HTTP_GET, [&]() { this->handleStatus(); });
  this->webServer.on("/api/wifi/scan", HTTP_GET, [&]() { this->handleWifiScan(); });
  this->webServer.on("/api/wifi/status", HTTP_GET, [&]() { this->handleWifiStatus(); });
  this->webServer.on("/api/beginrequest", HTTP_GET, [&]() { this->beginKittleRequest(); });
  
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
  accessPoint.set("macAddress", String(WiFi.softAPmacAddress().c_str()));

  root.printTo(jsonString);
  this->webServer.send(200, "application/json", jsonString);  
}

void WebServer::initializeKittle()
{
  this->lastKittleUpdateTime = millis();
  this->kittleSvc = KittleService::getInstance();  
}

void WebServer::beginKittleRequest()
{
  this->kittleSvc->StartClientRequest(80);  
}

void WebServer::updateKittleStatus()
{ 
  if (millis() >= (this->lastKittleUpdateTime + kittleUpdateDelayInMilliseconds)) //If more than a second passed
  {
    this->kittleSvc->UpdateClientRequest();
    this->lastKittleUpdateTime = millis();  
  }
}
