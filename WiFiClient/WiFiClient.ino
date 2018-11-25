/*
    This sketch sends data via HTTP GET requests to other nodes in local network.
    
*/

#include <ESP8266WiFi.h>

const char* ssid     = "paco";
const char* password = "E332C2F9";

const char* host6 = "192.168.1.106";
const char* host7 = "192.168.1.107";
const int httpPort = 80;
String urlLow = "/gpio/0";
String urlHigh = "/gpio/1";


void setup() {
  
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
}


void loop() {
  
  delay(5000);

  WiFiClient client;

  //STAGE 1

  //localhost on 
  digitalWrite(2,1);
  Serial.print("Localhost on");
  
  // host6 on
  Serial.print("connecting to ");
  Serial.println(host6);
  if (!client.connect(host6, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
  Serial.println("Connection successful");
  
  enviarPeticionServidor(client, host6, urlHigh);
  pintarRespuestaServidor(client);

  delay(1000);

  // host7 off
  Serial.print("connecting to ");
  Serial.println(host7);
  if (!client.connect(host7, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
  Serial.println("Connection successful");
  
  enviarPeticionServidor(client, host7, urlLow);
  pintarRespuestaServidor(client);
  
  delay(240000);

  //STAGE 2

  //host6 off
  Serial.print("connecting to ");
  Serial.println(host6);
  if (!client.connect(host6, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
  Serial.println("Connection successful");

  enviarPeticionServidor(client, host6, urlLow);
  pintarRespuestaServidor(client);
  
  delay(1000);
  
  // host7 on 
  Serial.print("connecting to ");
  Serial.println(host7);
  if (!client.connect(host7, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
  Serial.println("Connection successful");
  
  enviarPeticionServidor(client, host7, urlHigh);
  pintarRespuestaServidor(client);

  delay(240000);

  //STAGE 3
  
  //localhost off 
  digitalWrite(2,0);
  Serial.print("Localhost off");

  // host6 on
  Serial.print("connecting to ");
  Serial.println(host6);
  if (!client.connect(host6, httpPort)) {
    Serial.println("connection failed");
    return;
  }  
  Serial.println("Connection successful");
  
  enviarPeticionServidor(client, host6, urlHigh);
  pintarRespuestaServidor(client);
  
  delay(240000);
}

void enviarPeticionServidor(WiFiClient client, const char* host, String url){
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
    
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      }
  }  
}

void pintarRespuestaServidor(WiFiClient client){
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}



