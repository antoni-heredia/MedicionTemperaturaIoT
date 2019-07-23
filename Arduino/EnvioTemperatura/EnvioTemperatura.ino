

/*
   Created by K. Suwatchai (Mobizt)

   Email: k_suwatchai@hotmail.com

   Github: https://github.com/mobizt

   Copyright (c) 2019 mobizt

*/

//This example shows how to set and push timestamp (server time) which is the server variable that suopported by Firebase

#include <WiFi.h>
#include "FirebaseESP32.h"


#include "config.h"

//Define Firebase Data object
FirebaseData firebaseData;


// Incluimos librería
#include <DHT.h>
#include <Adafruit_Sensor.h>

// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 16
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  /*
    This option allows get and delete functions (PUT and DELETE HTTP requests) works for device connected behind the
    Firewall that allows only GET and POST requests.

    Firebase.enableClassicRequest(firebaseData, true);
  */

  dht.begin();

}

void loop()
{
    String path = "/temperaturas";
  if (Firebase.pushFloat(firebaseData, path, dht.readTemperature())) {
    Serial.println("temperatura: " + String(dht.readTemperature()));

    Serial.println(firebaseData.dataPath());
  
    Serial.println(firebaseData.pushName());
  
    Serial.println(firebaseData.dataPath() + "/"+ firebaseData.pushName());
  
  } else {
    Serial.println("error " + firebaseData.errorReason());
  }
  delay(1000);
}
