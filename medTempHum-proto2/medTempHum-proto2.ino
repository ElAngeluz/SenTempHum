/*
 * ThingSpeak -> Arduino Yun via Wi-Fi
 * Arduino Requirements:   
   * Arduino Yun 
   * Arduino 1.5.4 IDE or newer   
 * Network Requirements: 
   * Router with Wi-Fi 
   * DHCP enabled on Router 
  
Created: Feb 24, 2017 by Pedro Abel @Abeluzrivera 
*/  

#include <SimpleDHT.h>
#include "Bridge.h" 
#include "HttpClient.h" 

int pinDHT11 = 2;
SimpleDHT11 dht11;

#define ARRAY_SIZE 2
String key = "XXXX";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Bridge.begin();
  Console.begin();
}

void postToThingSpeak(int value[]) {
  Process p;
  String cmd = "curl -d \'key="+key;
  for (int i=0;i<ARRAY_SIZE;i++) {
    cmd = cmd + "&field"+ (i+1) + "=" + value[i];
  }
  cmd = cmd + "\' http://api.thingspeak.com/update&quot";
  p.runShellCommand(cmd);
  Console.println(cmd);// solo per vedere il codice
  p.close();
}

void loop() {
  // put your main code here, to run repeatedly:
  // start working...

  Serial.println("=================================");
  Serial.println("Sample DHT11...");  

  // read without samples.
  byte temperature = 0;
  byte humidity = 0;

  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Read DHT11 failed.");
    return;
  } 

  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" %"); 

  int vol[ARRAY_SIZE];
  vol[0] = (int)temperature;
  vol[1] = (int)humidity;

  postToThingSpeak(vol);

  // DHT11 sampling rate is 20HZ.
  delay(20000);
}




