#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include "libraries\\ArduinoJson.h"
#include "FS.h"
#include "WebServer.h"
#include "KittleService.h"

#define kittleUpdateDelayInMilliseconds 1000
unsigned long lastKittleUpdateTime;
char hostString[16] = {0};
const char* ssid = "Galatea";
const char* password = "betopeque16";

DNSServer dnsServer;
WebServer* webServer;
KittleService* kittleSvc;

void setup() {
  delay(2000);
  sprintf(hostString, "iPava_%06X", ESP.getChipId());
  webServer = new WebServer(hostString);
  webServer->start();

  Serial.begin(9600);
  Serial.println("Starting...");

  setupHostname();
  //setupWifi();
  setupAccessPoint();
  setupFileSystem();
  setupMDNS();
  initializeKittle();
}

void loop() {
  /*
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println('Reconnecting..........');
    setupWifi();
  }
  */
  dnsServer.processNextRequest();
  webServer->handle();
  updateKittleStatus();
}

void setupFileSystem() {
  SPIFFS.begin();
}

void setupHostname() {
  Serial.printf("Setting hostname: %s\n", hostString);
  WiFi.hostname(hostString);
}

void setupAccessPoint() {
  WiFi.mode(WIFI_AP);

  IPAddress localIP(192, 168, 1, 1);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(localIP, gateway, subnet);
  WiFi.softAP(hostString);
  Serial.println("Access point ip: " + WiFi.softAPIP().toString());

  dnsServer.start(53, "*", localIP);
}

void setupWifi() {
	WiFi.setAutoConnect(false);
	WiFi.setAutoReconnect(false);

  Serial.printf("Connecting to %s\n", ssid);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected with IP: " + WiFi.localIP().toString());
}

void setupMDNS() {
  if (!MDNS.begin(hostString, WiFi.softAPIP())) {
    Serial.println("Error setting up MDNS responder!");
    return;
  }

  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS service added");
}

void initializeKittle()
{
  lastKittleUpdateTime = millis();
  kittleSvc = KittleService::getInstance();  
}

void updateKittleStatus()
{ 
  if (millis() >= (lastKittleUpdateTime + kittleUpdateDelayInMilliseconds)) //If more than a second passed
  {
    kittleSvc->UpdateClientRequest();
    lastKittleUpdateTime = millis();  
  }
}
