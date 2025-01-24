const int LED = 17;
void setup()
{
pinMode(LED,OUTPUT);
Serial.begin(115200);
delay(1000);
Serial.println("ESP32 Led Test");
}
void loop()
{
digitalWrite(LED,HIGH);
delay(1000);
digitalWrite(LED,LOW);
delay(1000);
}