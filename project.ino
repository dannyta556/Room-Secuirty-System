// This #include statement was automatically added by the Particle IDE.
#include <HttpClient.h>

#include "SPI.h"
#include <Wire.h>
#include <HttpClient.h>
#include "application.h"
// This #include statement was automatically added by the Particle IDE.

#define TIME_HEADER "T"
#define TIME_REQUEST 7
unsigned int nextTime = 0;

int counter;
HttpClient http;

const int pinSound = A1;
int sum;
int maxValue;
int minValue;
int avgValue;
unsigned long tl_timer;

http_header_t headers[] = {
  {"Accept","*/*"},
  {NULL,NULL}
};

http_request_t request;
http_response_t response;

void setup() {
    Serial.begin(9600);
    sum = 0;
    maxValue = 0;
    minValue = 10000;
    avgValue = 0;
    counter = 0;
    tl_timer = millis() + 10000;
}


void loop() {
    
    //Set up values
    int analogValue = analogRead(A0);
    
    //Calibration
    if (millis() < tl_timer) {
        Serial.println("Calibrating...");
        if(analogValue > maxValue){
            maxValue = analogValue;
        } else if (analogValue < minValue){
            minValue = analogValue;
        }
        
    //Working    
    } else {
        if(millis() >= nextTime){
            
            for(int i=0; i<32; i++){
                sum += analogRead(pinSound);
            }
            sum >>= 5;
            //Serial.println(sum);
            avgValue = maxValue + minValue / 2;
            nextTime = millis() + 1000;
            if(analogValue > avgValue + 100)
            {
                Serial.println("Lights are turned on.");
                
                request.hostname = "3.16.91.217";
                request.port = 5000;
                request.path = "/data?type=lights";
                
                http.get(request,response,headers);
                Serial.print("Application>\tResponse status: ");
                Serial.println(response.status);
                
            }
            if (sum >= 2000)
            {
                Serial.println("Suspicious noise made");
                
                request.hostname = "3.16.91.217";
                request.port = 5000;
                request.path = "/data?type=sound";
                
                http.get(request,response,headers);
                Serial.print("Application>\tResponse status: ");
                Serial.println(response.status);
            }

        }
        
    }
    
}