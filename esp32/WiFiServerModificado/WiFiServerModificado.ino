#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid     = "Softex_Conv";
const char* password = "Softex2023";

const int LED = 17;
WiFiServer server(80);

Servo servo ;
int pos = 0;


void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    delay(10);
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
    servo.attach(13);

}



void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
           // client.println("HTTP/1.1 200 OK");
            client.println("LED ON");
          /*  client.println();
            client.print("Hello World");
            client.println();
           */ 
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        Serial.println(currentLine);
       if (currentLine.endsWith("LED_ON")) {
          digitalWrite(LED, HIGH);               // GET /H turns the LED on
          client.println("Acendendo o LED");
        }
         else if (currentLine.endsWith("LED OFF")) {
          digitalWrite(LED, LOW);                // GET /L turns the LED off
          client.println("LED OFF");
        } 
         else if (currentLine.endsWith("MOTOR")) {
            for (pos = 0; pos <= 180; pos += 1) {
              servo.write(pos);
              delay(15);
            }
          }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
