// esse trecho especifica a biblioteca conforme o modelo do esp.
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif
//////////////////////////////////////////////////////////////////////////////
#include <ESPAsyncWebServer.h>
// libraries: 
// https://github.com/me-no-dev/ESPAsyncTCP
// https://github.com/me-no-dev/ESPAsyncWebServer
//--------------------------------------------------------------------------
#define ESP12_LED 2 // GPIO2, D4 // Debug led
//---------------------------------------------------------------------------
AsyncWebServer server(80); // server port 80
//WiFiServer server(8082);
//----------------Login---------------------------------
const char* ssid = "TVC"; // your network SSID (name)
const char* password = "504b2014"; // your network password (use for WPA, or use as key for WEP)
//----------------IP definition (not work)--------------
IPAddress ip(192,168,10,175);
IPAddress gateway(192,168,10,1);
IPAddress subnet(255,255,255,0);
//////////////////////////////////////////////////////////////////////////////
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}
//////////////////////////////////////////////////////////////////////////////
void setup(void)
{
  
  Serial.begin(115200);
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, password);
  WiFi.softAP("ESP8266", ""); // ID do WIFI
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());

  pinMode(ESP12_LED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  /*
  if (MDNS.begin("ESP")) { //esp.local/
    Serial.println("MDNS responder started");
  }
  */


  server.on("/", [](AsyncWebServerRequest * request)
  { 
   String message = "hello world"; 
  request->send(200, "text/plain", message);
  });

   server.on("/page1", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
   String message = "Welcome to page1"; 
  request->send(200, "text/plain", message);
  });

  server.onNotFound(notFound);

  server.begin();  // it will start webserver
}


void loop(void)
{
   digitalWrite(ESP12_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is active low on the ESP-01)
  delay(100);                      // Wait for a second
  digitalWrite(ESP12_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(100); 
}
