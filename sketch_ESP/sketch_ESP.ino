#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Replace with your network credentials
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

//-----------------NTC----------------------------------------------------
const double VCC = 2.294;           // NodeMCU on board 3.3v vcc
const double R2 = 9980;             // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148; // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741;
//---------------------------------------------------------------------------

ESP8266WebServer server(80); // instantiate server at port 80 (http port)

String page = "";
String text = "";
double data;
void setup(void)
{

  pinMode(A0, INPUT);

  delay(100);
  Serial.begin(115200);
  WiFi.begin(ssid, password); // begin WiFi connection
  WiFi.config(ip, gateway, subnet);//Definicao do IP
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.localIP());

  server.on("/data.txt", []()
  {
    text = (String)data;
    server.send(200, "text/html", text);
  });
  server.on("/", []()
  {
    page = "<h1>Sensor to Node MCU Web Server</h1><h1>Data:</h1> <h1 id=\"data\">""</h1>\r\n";
    page += "<script>\r\n";
    page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 100);\r\n";
    page += "function loadData(url, callback){\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function(){\r\n";
    page += " if(this.readyState == 4 && this.status == 200){\r\n";
    page += " callback.apply(xhttp);\r\n";
    page += " }\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", url, true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}\r\n";
    page += "function updateData(){\r\n";
    page += " document.getElementById(\"data\").innerHTML = this.responseText;\r\n";
    page += "}\r\n";
    page += "</script>\r\n";
    server.send(200, "text/html", page);
  });
  server.begin();
  Serial.println("Web server started!");
}

void loop(void)
{
  double Vout, Rth, temperature, adc_value;
  adc_value = analogRead(A0);
  Vout = (adc_value * VCC) / adc_resolution;
  Rth = (VCC * R2 / Vout) - R2;
  temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)), 3)))); // Temperature in kelvin
  temperature = temperature - 273.15;                                  // Temperature in degree celsius
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
  delay(1000);

  data = temperature;
  server.handleClient();
}
