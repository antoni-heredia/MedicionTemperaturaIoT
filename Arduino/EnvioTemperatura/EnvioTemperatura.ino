

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
  //Actualizar el timestamp para luego guardar la temperatura con la hora
  int timestamp;
  if (Firebase.setTimestamp(firebaseData, path + "/ultimaActualizacion"))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());

    //Timestamp saved in millisecond, get its seconds from intData()
    Serial.print("TIMESTAMP (Seconds): ");
    Serial.println(firebaseData.intData());

    //Or print the total milliseconds from doubleData()
    //Due to bugs in Serial.print in Arduino library, use printf to print double instead.
    printf("TIMESTAMP (milliSeconds): %.0lf\n", firebaseData.doubleData());
    timestamp = firebaseData.intData();
    //Or print it from payload directly
    Serial.print("TIMESTAMP (milliSeconds): ");
    Serial.println(firebaseData.payload());

    //Due to some internal server error, ETag cannot get from setTimestamp
    //Try to get ETag manually
    Serial.println("ETag: " + Firebase.getETag(firebaseData, path + "/Set/Timestamp"));
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }    
  Serial.println("temperatura: " + String(dht.readTemperature()));

  if (Firebase.setFloat(firebaseData, path+"/"+String(timestamp), dht.readTemperature())) {
    Serial.println("temperatura: " + String(dht.readTemperature()));

    Serial.println(firebaseData.dataPath());
  
    Serial.println(firebaseData.pushName());
  
    Serial.println(firebaseData.dataPath() + "/"+ firebaseData.pushName());
  
  } else {
    Serial.println("error " + firebaseData.errorReason());
  }
  
  //delay(300000);
  delay(1000);
}
