#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AccelStepper.h>

#define LED_INDICATOR_PIN 18
#define FULLSTEP 4
#define STEPS_PER_REV 2048

const char* ssid = "Verizon_YD3BVC"; 
const char* password = "herd-ace9-broke";

AsyncWebServer server(80);
AccelStepper stepper(FULLSTEP, 3, 1, 2, 0);

int LED_state = LOW;

void setup() {
    //Set up serial connection and LED pin
    Serial.begin(9600);
    stepper.setMaxSpeed(200.0);
    stepper.setAcceleration(50.0);
    stepper.setSpeed(200);
    stepper.setCurrentPosition(0);
    
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


    server.on("/led1/status", HTTP_GET, [](AsyncWebServerRequest* request){
                              Serial.println("Request Received:");
                              Serial.println("GET /");
                              request->send(200, "text/html", ((LED_state == LOW) ? "OFF" : "ON"));
    });

    
    server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request GET /led1/on");
      stepper.moveTo(STEPS_PER_REV * 3.5);
      LED_state = HIGH;
      digitalWrite(LED_INDICATOR_PIN, LED_state);
      request->send(200, "text/html", "ON");
    });

    server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request GET /led1/off");
      stepper.moveTo(0);
      LED_state = LOW;
      digitalWrite(LED_INDICATOR_PIN, LED_state);
      request->send(200, "text/html", "OFF");
    });

    server.begin();

}

void loop() {

  stepper.run();
  
}