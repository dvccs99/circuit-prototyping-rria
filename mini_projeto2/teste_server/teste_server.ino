#include <WiFi.h>

const char* ssid = "wifi";
const char* password = "pwd";
const char* server_ip = "ip";
const int server_port = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");

  if (WiFi.status() !=WL_CONNECTED){
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to server at ");
  Serial.print(server_ip);
  Serial.print(":");
  Serial.println(server_port);

  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected to server");
    
    String testMessage = "ALARM";
    client.print(testMessage);
    Serial.println("Test message sent to server:");
    Serial.println(testMessage);

    unsigned long timeout = millis();
    while (!client.available()) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Server Timeout !");
        client.stop();
        return;
      }
    }

    while (client.available()) {
      String response = client.readStringUntil('\n');
      Serial.println("Server response:");
      Serial.println(response);
    }

    client.stop();
    Serial.println("Connection closed");
  } else {
    Serial.println("Failed to connect to server");
  }
}

void loop() {
}