//https://blogmasterwalkershop.com.br/embarcados/nodemcu/nodemcu-como-criar-um-web-server-e-conectar-a-uma-rede-wifi

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#error "Board not found"
#endif
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include <WebSocketsServer.h>
//-------------------WEB-----------------------------------------------------
//WiFiServer server(8086); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.15:8082)
AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);

//-----------------NTC----------------------------------------------------
const double VCC = 2.294;             // NodeMCU on board 3.3v vcc
const double R2 = 9980;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741;


//---------------------------------------------------------------------------
//#define outputpin A0
//const int ledPin = 2;//---------------Set LED GPIO
#define LED1 13
#define LED2 2

/*
  //----------------Login---------------------------------
  const char *ssid = "GL INTERNET_C140";          // WIFI password
  const char *password = "Engenhari@2019"; // ID Password
  //----------------IP definition ------------------------
  IPAddress ip(10, 0, 0, 200);
  IPAddress gateway(10, 0, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
*/
//----------------Login---------------------------------
const char *ssid = "TVC";          // WIFI password
const char *password = "504b2014"; // ID
//----------------IP definition ------------------------
IPAddress ip(192, 168, 10, 175);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
//---------------------------------------------------------------------
//----------------Web Page---------------------------------------------
//---------------------------------------------------------------------
char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<script>
var connection = new WebSocket('ws://'+location.hostname+':81/');
var button_1_status = 0;
var button_2_status = 0;
function button_1_on()
{ 
  window.location = 'http://'+location.hostname+'/led1/on';
  button_1_status = 1; 
  //console.log("LED 1 is ON");
  //connection.open = () => connection.send("LED 1 is ON!!");
  send_data();
}
function button_1_off()
{
  window.location = 'http://'+location.hostname+'/led1/off';
  button_1_status = 0;
  //console.log("LED 1 is OFF");
  //connection.open = () => connection.send("LED 1 is OFF!!");
  send_data();  
}
function button_2_on()
{ 
  console.log("LED 2 is ON");
}
function button_2_off()
{
  console.log("LED 2 is OFF");
}
function send_data()
{
  var full_data = '{"LED1" :'+button_1_status+',"LED2":'+button_2_status+'}';
  connection.open = () => connection.send(full_data);
}
</script>
<body>
<center>
<form action="get"> </form>
<h1>Teste<h1>
  <h3>Led 1</h3>
  <button onclick= "button_1_on()" >On</button><button onclick="button_1_off()" >Off</button>
  <h3>Led 2</h3>
  <button onclick="window.location = 'http://'+location.hostname+'/led2/off';button_2_on">On</button>
  <button onclick="window.location = 'http://'+location.hostname+'/led2/on';button_2_off">Off</button>
 <form action="/get">input1: <input type="text" name="input1"><input type="submit" value="Submit"></form><br>
</center>
</body>
</html>

)=====";
  //<button onclick="window.location = 'http://'+location.hostname+'/led1/on'">on</button>
  //<button onclick="window.location = 'http://'+location.hostname+'/led1/off'">off</button>
//--------------------------------------------------------------------
//---------------Page Not found---------------------------------------
//--------------------------------------------------------------------
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);
      
     DynamicJsonDocument doc(200);
    // deserialize the data
    DeserializationError error = deserializeJson(doc, message);
    // parse the parameters we expect to receive (TO-DO: error handling)
      // Test if parsing succeeds.
    if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
    }
    int LED1_status = doc["LED1"];
    int LED2_status = doc["LED2"];
    digitalWrite(LED1,LED1_status);
    digitalWrite(LED2,LED2_status);
    }
}
//--------------------------------------------------------------------
//---------------void setup-------------------------------------------
//--------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  //pinMode(2, OUTPUT); // Led
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  //pinMode(outputpin,INPUT);

  Serial.println("");            //PULA UMA LINHA NA JANELA SERIAL
  Serial.print("Conectando a "); 
  Serial.print(ssid);            
  WiFi.begin(ssid, password); 
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED)
  {                    //ENQUANTO STATUS FOR DIFERENTE DE CONECTADO
    delay(50);        //INTERVALO DE 500 MILISEGUNDOS
    Serial.print("."); //ESCREVE O CARACTER NA SERIAL
  }
  
  Serial.println("");                        //PULA UMA LINHA NA JANELA SERIAL
  Serial.print("Conectado a rede sem fio "); //ESCREVE O TEXTO NA SERIAL
  Serial.println(ssid);                      //ESCREVE O NOME DA REDE NA SERIAL

  server.begin();                            //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"

  Serial.println("Servidor iniciado");       //ESCREVE O TEXTO NA SERIAL

  Serial.print("NodeMCU IP: "); //ESCREVE O TEXTO NA SERIAL
  Serial.print("http://");                          //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP());                   //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)
  Serial.println("");
  WiFi.softAP("ESP8266", "");
  //WiFi.softAP(ssidAP,passwordAP);
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());
  
  if (MDNS.begin("ESP")) { //esp.local/
      Serial.println("MDNS responder started");
  }
  //------------------server----------------------------------
   server.on("/", [](AsyncWebServerRequest * request)
  { 
   String message = "hello world"; 
   
  request->send_P(200, "text/html", webpage);
  });
  //------------------Led 1------------------------------------
   server.on("/led1/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
  digitalWrite(LED1,HIGH);
  //analogRead(outputpin);  
  request->send_P(200, "text/html", webpage);
  });
  server.on("/led1/off", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
  digitalWrite(LED1,LOW); 
  request->send_P(200, "text/html", webpage);
  });
  //------------------Led 2------------------------------------
   server.on("/led2/on", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
  digitalWrite(LED2,HIGH);
  //analogRead(outputpin);  
  request->send_P(200, "text/html", webpage);
  });
  server.on("/led2/off", HTTP_GET, [](AsyncWebServerRequest * request)
  { 
  digitalWrite(LED2,LOW); 
  request->send_P(200, "text/html", webpage);
  });
  //-------------------------------------------------------------  
  server.onNotFound(notFound);// void notFound
  server.begin();  // it will start webserver
  websockets.begin();
  websockets.onEvent(webSocketEvent);
}
//-------------------------------------------------------------------
//---------------void loop-------------------------------------------
//-------------------------------------------------------------------
void loop()
{
  websockets.loop();
  //-----------------NTC----------------------------------------------------
  double Vout, Rth, temperature, adc_value; 
  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin
  temperature = temperature - 273.15;  // Temperature in degree celsius
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
  delay(5000);
}
