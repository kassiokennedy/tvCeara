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

#include <WebSocketsServer.h>
//-----------------NTC----------------------------------------------------
const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741; 


//---------------------------------------------------------------------------
#define outputpin A0
const int ledPin = 2;//---------------Set LED GPIO
#define LED1 13
#define LED2 12

//----------------Login---------------------------------
const char *ssid = "GL INTERNET_C140";          // WIFI password
const char *password = "Engenhari@2019"; // ID Password
//----------------IP definition ------------------------
IPAddress ip(10, 0, 0, 200);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);
/*
  //----------------Login---------------------------------
  const char *ssid = "internet";          // WIFI password
  const char *password = "123456789"; // ID
  //----------------IP definition ------------------------
  IPAddress ip(192,168,0,175);
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
*/
//----------------Web Page----------------------------------------
char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>Teste<h1>
  <h3>Led 1</h3>
  <button onclick="window.location = 'http://'+location.hostname+'/led1/on'">on</button>
  <button onclick="window.location = 'http://'+location.hostname+'/led1/off'">off</button>
  <h3>Led 2</h3>
  <button onclick="window.location = 'http://'+location.hostname+'/led2/on'">on</button>
  <button onclick="window.location = 'http://'+location.hostname+'/led2/off'">off</button>
</center>
</body>
</html>
)=====";
//---------------------------------------------------------------------------
//WiFiServer server(8086); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.15:8082)
AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);
//---------------Page Not found-------------------------------------------
void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}
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
      /*  
      if(message == "LED 1 is OFF"){
        digitalWrite(LED1,LOW);
      }

      if(message == "LED 1 is ON"){
        digitalWrite(LED1,HIGH);
      }
*/


  }
}

//---------------void setup-------------------------------------------
void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT); // Led
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT); 
  pinMode(outputpin,INPUT);
  
  delay(10);            //INTERVALO DE 10 MILISEGUNDOS

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
//---------------void loop-------------------------------------------
void loop()
{
  /*
  int analogValue = analogRead(outputpin);
  float millivolts = (analogValue / 1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts / 10;
  Serial.print("in DegreeC=   ");
  Serial.println(celsius);
  delay(2000);                                                          //INTERVALO DE 1 MILISEGUNDO
  */
  websockets.loop();
  //-----------------NTC----------------------------------------------------
  double Vout, Rth, temperature, adc_value; 

  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;

/*  Steinhart-Hart Thermistor Equation:
 *  Temperature in Kelvin = 1 / (A + B[ln(R)] + C[ln(R)]^3)
 *  where A = 0.001129148, B = 0.000234125 and C = 8.76741*10^-8  */
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin

  temperature = temperature - 273.15;  // Temperature in degree celsius
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" degree celsius");
  delay(500);
/*  
  WiFiClient client = server.available(); //VERIFICA SE ALGUM CLIENTE ESTÁ CONECTADO NO SERVIDOR
  if (!client)
  {         //SE NÃO EXISTIR CLIENTE CONECTADO, FAZ
    return; //REEXECUTA O PROCESSO ATÉ QUE ALGUM CLIENTE SE CONECTE AO SERVIDOR
  }
/*
  Serial.println("Novo cliente se conectou!"); //ESCREVE O TEXTO NA SERIAL
  while (!client.available())
  {           //ENQUANTO CLIENTE ESTIVER CONECTADO
    delay(1); //INTERVALO DE 1 MILISEGUNDO
  }
  */
  //---------------------------------------------------------------------------------------------------
  /*
  String request = client.readStringUntil('\r'); //FAZ A LEITURA DA PRIMEIRA LINHA DA REQUISIÇÃO
  Serial.println(request);                       //ESCREVE A REQUISIÇÃO NA SERIAL
  client.flush();                                //AGUARDA ATÉ QUE TODOS OS DADOS DE SAÍDA SEJAM ENVIADOS AO CLIENTE

  client.println("HTTP/1.1 200 OK");         //ESCREVE PARA O CLIENTE A VERSÃO DO HTTP
  client.println("Content-Type: text/html"); //ESCREVE PARA O CLIENTE O TIPO DE CONTEÚDO(texto/html)
  client.println("");
  client.println("<!DOCTYPE HTML>");                                 //INFORMA AO NAVEGADOR A ESPECIFICAÇÃO DO HTML
  client.println("<head>"); 
  client.println("<title>ESP8266</title>"); 
  client.println("</head>"); 
  client.println("<html>");                                          //ABRE A TAG "html"
  client.println("<h1><center><font size='4'>Transmissor Digital</center></h1>");
  client.println("<center><font size='3'>Temperatura: " + String(celsius) + "C</center>"); //ESCREVE "Seja bem vindo!" NA PÁGINA
  client.println("</html>");                                         //FECHA A TAG "html"
*/
//---------------------------------------------------------------------------------------------------
}
