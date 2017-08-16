class WiFi {
  public:
    
  private:
}

void setupAccessPoint() {
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