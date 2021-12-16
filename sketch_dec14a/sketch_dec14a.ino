//https://blogmasterwalkershop.com.br/embarcados/nodemcu/nodemcu-como-criar-um-web-server-e-conectar-a-uma-rede-wifi

//pagina http://192.168.1.151/
#include <ESP8266WiFi.h> //INCLUSÃO DA BIBLIOTECA NECESSÁRIA PARA FUNCIONAMENTO DO CÓDIGO

const char *ssid = "TVC";          // WIFI password
const char *password = "504b2014"; // ID Password

WiFiServer server(80); //CASO OCORRA PROBLEMAS COM A PORTA 80, UTILIZE OUTRA (EX:8082,8089) E A CHAMADA DA URL FICARÁ IP:PORTA(EX: 192.168.0.15:8082)
int x = 0;
int i = 0;
int outputpin = A0;
/*
// Set LED GPIO
const int ledPin = 2;
*/
void setup()
{
  pinMode(2, OUTPUT); // Led

  
  Serial.begin(115200); //INICIALIZA A SERIAL
  delay(10);            //INTERVALO DE 10 MILISEGUNDOS

  Serial.println("");            //PULA UMA LINHA NA JANELA SERIAL
  Serial.print("Conectando a "); //ESCREVE O TEXTO NA SERIAL
  Serial.print(ssid);            //ESCREVE O NOME DA REDE NA SERIAL

  WiFi.begin(ssid, password); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE SEM FIO
  
/*
  while (WiFi.status() != WL_CONNECTED)
  {                    //ENQUANTO STATUS FOR DIFERENTE DE CONECTADO
    delay(50);        //INTERVALO DE 500 MILISEGUNDOS
    Serial.print("."); //ESCREVE O CARACTER NA SERIAL
  }
*/
/*
  Serial.println("");                        //PULA UMA LINHA NA JANELA SERIAL
  Serial.print("Conectado a rede sem fio "); //ESCREVE O TEXTO NA SERIAL
  Serial.println(ssid);                      //ESCREVE O NOME DA REDE NA SERIAL
*/
  server.begin();                            //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA DEFINIDA EM "WiFiServer server(porta);"
/*  
  Serial.println("Servidor iniciado");       //ESCREVE O TEXTO NA SERIAL

  Serial.print("IP para se conectar ao NodeMCU: "); //ESCREVE O TEXTO NA SERIAL
  Serial.print("http://");                          //ESCREVE O TEXTO NA SERIAL
  Serial.println(WiFi.localIP());                   //ESCREVE NA SERIAL O IP RECEBIDO DENTRO DA REDE SEM FIO (O IP NESSA PRÁTICA É RECEBIDO DE FORMA AUTOMÁTICA)
*/
}

void loop()
{
  int analogValue = analogRead(outputpin);
  float millivolts = (analogValue / 1024.0) * 3300; //3300 is the voltage provided by NodeMCU
  float celsius = millivolts / 10;
  Serial.print("in DegreeC=   ");
  Serial.println(celsius);
  
  digitalWrite(2, HIGH); // Acende o Led
  delay(200);            // Aguarda 1 segundo
  digitalWrite(2, LOW);  // Apaga o Led
  delay(200);            // Aguarda 1 segundo

  i++;
  
  WiFiClient client = server.available(); //VERIFICA SE ALGUM CLIENTE ESTÁ CONECTADO NO SERVIDOR
  if (!client)
  {         //SE NÃO EXISTIR CLIENTE CONECTADO, FAZ
    return; //REEXECUTA O PROCESSO ATÉ QUE ALGUM CLIENTE SE CONECTE AO SERVIDOR
  }

  Serial.println("Novo cliente se conectou!"); //ESCREVE O TEXTO NA SERIAL
  while (!client.available())
  {           //ENQUANTO CLIENTE ESTIVER CONECTADO
    delay(1); //INTERVALO DE 1 MILISEGUNDO
  }
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
  client.println("<h1><center>Transmissor Digital</center></h1>");
  client.println("<center><font size='5'>Temperatura: " + String(celsius) + "C</center>"); //ESCREVE "Seja bem vindo!" NA PÁGINA
  //client.println("<h2><center>" + String(celsius) + " C</center></h2>");
  client.println("</html>");                                         //FECHA A TAG "html"
  delay(1);                                                          //INTERVALO DE 1 MILISEGUNDO
/*  
  Serial.println("Cliente desconectado");                            //ESCREVE O TEXTO NA SERIAL
  Serial.println("");                                                //PULA UMA LINHA NA JANELA SERIAL
*/
}
