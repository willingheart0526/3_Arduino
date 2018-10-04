#ifndef AREST_FUNS_H
#define AREST_FUNS_H

#include "Arduino.h"

// Unique ID to identify the device for cloud.arest.io
char* device_id = "YY01";

// WiFi parameters
const char* ssid = "IOTAP";
const char* password = "raspberry";


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80
#define LED_BUILTIN           2

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Variables to be exposed to the API
float temperature;
float humidity;
String local_ip = "";

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Functions
void callback(char* topic, byte* payload, unsigned int length);

// Convert IP address to String
String ipToString(IPAddress address)
{
  return String(address[0]) + "." +
         String(address[1]) + "." +
         String(address[2]) + "." +
         String(address[3]);
}

void uploadData() {
  rest.publish(client, "temperature", temperature);
}

int ledControl(String msg) {
  Serial.println("ledControl: " + msg);
  String data = msg.substring(0,msg.length()-2);

  int count = data.toInt();

  pinMode(LED_BUILTIN, OUTPUT);
 for(int i = 0; i < count; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(500);                       // wait for a second
  }

}


void setupARest(void)
{
  // Start Serial
  //  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);
  rest.variable("local_ip", &local_ip);

  rest.function("led", ledControl);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id(device_id);
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Local server started on IP:");

  // Print the IP address
  Serial.println(WiFi.localIP());
  local_ip = ipToString(WiFi.localIP());

}

void loopARest() {

  // Connect to the cloud
  rest.handle(client);

  // Handle Local aREST calls
  WiFiClient clientLocal = server.available();
  if (!clientLocal) {
    return;
  }
  while (!clientLocal.available()) {
    delay(1);
  }
  rest.handle(clientLocal);

}


// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}


#endif
