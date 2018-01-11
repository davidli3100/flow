#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <time.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>

#define ssid "Linkwo"
#define ssid_password "Avroc108"

#define firebase_url "https://aquifer-f22e3.firebaseio.com"
#define firebase_auth "MH3q9xcJsFxVTUUcYIRqLdfajpVSP1XQF9WS4gT5"
#define moistureRefresh 15 * 1000


WiFiClient client;

char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "KXF5IVGKOT7XF7BG";
long lastConnectionTime = 0;
boolean lastConnected = false;
int numFields =2;
int timezone = 5;
int dst = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("beginning");

    //connecting to wifi 
    WiFi.begin(ssid, ssid_password);
    Serial.println("connecting to wifi");
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }

  }//setup end
  
double analogValue = 0.0;
double analogVolts = 0.0;
unsigned long timeHolder = 0;


int HTTPPost(String fieldData[]){
  if (client.connect (thingSpeakAddress, 800)){
    String PostData="api_key=" + writeAPIKey;
    for (int field = 1; field < numFields+1; field++) {
      PostData += "&field" + String(field) + "=" + fieldData[field];
    }

    //posting via http
    Serial.println("Connecting to thingspeak for update");
    Serial.println();
    client.println( "POST /update HTTP/1.1" );
    client.println( "Host: api.thingspeak.com" );
    client.println( "Connection: close" );
    client.println( "Content-Type: application/x-www-form-urlencoded" );
    client.println( "Content-Length: " + String( PostData.length() ) );
    client.println();
    client.println( PostData );
    Serial.println( PostData );
    client.stop();
  }
}



void loop() {
  //for thing thingSpeak 
  String data[2];

    analogValue = analogRead(A0);
   Serial.println(analogValue);

    analogVolts = (analogValue * 3.08) / 1024;
      // convert the analog signal to voltage
  // the ESP2866 A0 reads between 0 and ~3 volts, producing a corresponding value
  // between 0 and 1024. The equation above will convert the value to a voltage value.

  int moisture = (analogValue * 100) / 400;
  data[1] = moisture;
    // now get our chart value by converting the analog (0-1024) value to a value between 0 and 100.
  // the value of 400 was determined by using a dry moisture sensor (not in soil, just in air).
  // When dry, the moisture sensor value was approximately 400. This value might need adjustment
  // for fine tuning of the moisture.

  time_t now = time(nullptr);
  Serial.println("Time Is ");
 // Serial.print(ctime(&now));
//  data[2] = ctime(&now);

  //printing moisture values to serial now
  Serial.println("Moisture is: ");
  Serial.print(moisture);
  delay(moistureRefresh-1000);

  HTTPPost(data);
  delay(moistureRefresh);
  /*
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["moisture"] = moisture;
  root["time"] = now;
  Serial.println("");
  Serial.println("Pushing to Firebase");
  Firebase.push("moisture", root);
  if(Firebase.success()){
    Serial.println("firebase works!");
    }
  if(Firebase.failed()){
      Serial.println("setting firebase failed");
      Serial.println(Firebase.error());
      return;
  }
  delay(moistureRefresh); */

} 