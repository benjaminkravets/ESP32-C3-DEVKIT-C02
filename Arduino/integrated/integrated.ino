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
    pinMode(LED_INDICATOR_PIN, OUTPUT);
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

    String html = "";
    html = "ON";



    server.on("/led1/status", HTTP_GET, [](AsyncWebServerRequest* request){
                              Serial.println("Request Received:");
                              Serial.println("GET /");
                              request->send(200, "text/html", html);
    });

    
    server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request GET /led1/on");
      LED_state = HIGH;
      digitalWrite(LED_INDICATOR_PIN, LED_state);
      request->send(200, "text/html", "ON");
    });

    server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request GET /led1/off");
      LED_state = LOW;
      digitalWrite(LED_INDICATOR_PIN, LED_state);
      request->send(200, "text/html", "OFF");
    });

    server.begin();

}

void loop() {

}