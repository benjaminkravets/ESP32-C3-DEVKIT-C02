#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define LED_INDICATOR_PIN 18

const char* ssid = "Verizon_YD3BVC"; 
const char* password = "herd-ace9-broke";

AsyncWebServer server(80);

int LED_state = LOW;

String getHTML() {
  String html = "";
  if (LED_state == LOW)
    html += "OFF";
  else
    html += "ON";

  return html;
}

void setup() {
    //Set up serial connection and LED pin
    Serial.begin(9600);
    pinMode(LED_INDICATOR_PIN, LED_state);
    digitalWrite(LED_INDICATOR_PIN, LED_state);

    //Network Connection
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting");
    }

    //Print network status
    Serial.println("Connected:");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.SSID());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
                              Serial.println("Request Received:");
                              Serial.println("GET /");
                              request->send(200, "text/html", getHTML());
    });

}

void loop() {

}