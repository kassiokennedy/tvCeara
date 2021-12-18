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

//----------------Login---------------------------------
const char *ssid = "GL INTERNET_C140";          // WIFI password
const char *password = "Engenhari@2019"; // ID Password
//----------------IP definition ------------------------
IPAddress ip(10,0,0,200);
IPAddress gateway(10,0,0,1);
IPAddress subnet(255,255,255,0);

/*
//----------------Login---------------------------------
const char *ssid = "internet";          // WIFI password
const char *password = "123456789"; // ID 
//----------------IP definition ------------------------
IPAddress ip(192,168,0,175);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
*/
//---------------------------------------------------------------------------
WiFiServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.15:8082)
//---------------------------------------------------------------------------
int x = 0;
int outputpin = A0;

// Set LED GPIO
const int ledPin = 2;

void setup()
{
  pinMode(2, OUTPUT); // Led

  Serial.begin(115200);
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

/*
  Serial.println("");                        //PULA UMA LINHA NA JANELA SERIAL
  Serial.print("Conectado a rede sem fio "); //ESCREVE O TEXTO NA SERIAL
  Serial.println(ssid);                      //ESCREVE O NOME DA REDE NA SERIAL
*/
  server.begin();                            //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"

  Serial.println("Servidor iniciado");       //ESCREVE O TEXTO NA SERIAL

  Serial.print("NodeMCU IP: "); //ESCREVE O TEXTO NA SERIAL
  Serial.print("http://");                          //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP());                   //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)

}

void loop()
{
  int analogValue = analogRead(outputpin);
  float millivolts = (analogValue / 1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts / 10;
  Serial.print("in DegreeC=   ");
  Serial.println(celsius);
  delay(2000);                                                          //INTERVALO DE 1 MILISEGUNDO
  
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

}
