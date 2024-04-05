#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AccelStepper.h>

#define LED_INDICATOR_PIN 18
#define INTERFACE 4 // number of pins for servo motor
#define STEPS_PER_REV 2048

const char* ssid = "Verizon_YD3BVC"; 
const char* password = "herd-ace9-broke";

AsyncWebServer server(80);
AccelStepper stepper(INTERFACE, 3, 1, 2, 0);

int stepper_position = LOW;

void setup() {
    //Set up serial connection and LED pin
    Serial.begin(9600);
    stepper.setMaxSpeed(200.0);
    stepper.setAcceleration(50.0);
    stepper.setSpeed(200);
    stepper.setCurrentPosition(0);
    
    //Set up pins
    pinMode(LED_INDICATOR_PIN, OUTPUT);
    digitalWrite(LED_INDICATOR_PIN, stepper_position);

    //Network Connection, wait in loop until connected
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting");
    }

    //Print network status
    Serial.println("Connected:");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.SSID());

    // assign response for for status uri, return servo state
    server.on("/servo/status", HTTP_GET, [](AsyncWebServerRequest* request){
      Serial.println("Request Received:");
      Serial.println("GET /");
      request->send(200, "text/html", (stepper_position == LOW) ? "LOW" : "HIGH");
    });

    // assign response for state uri, move to high state, return status
    server.on("/servo/high", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request GET /led1/on");
      stepper.moveTo(STEPS_PER_REV * 3.5); // move 3.5 revolutions (needed steps = steps per revolution * revolutions)
      stepper_position = HIGH;
      digitalWrite(LED_INDICATOR_PIN, stepper_position);
      request->send(200, "text/html", "HIGH");
    });

    // assign response for state uri, move to low state, return status
    server.on("/servo/low", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.println("Request GET /led1/off");
      stepper.moveTo(0); // move back from high (will move back 3.5 revolutions)
      stepper_position = LOW;
      digitalWrite(LED_INDICATOR_PIN, stepper_position);
      request->send(200, "text/html", "LOW");
    });

    server.begin();

}

void loop() {

  stepper.run();
  
}