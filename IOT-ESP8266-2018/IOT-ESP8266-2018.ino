#include <ESP8266WiFi.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//Credenciales WIFI
const char* ssid = "ssid";
const char* password = "password";

//Sitio Web de almacenaje de datos
//const char* server = "www.loremipsu.com";
//const char* strurl = "/Sensor.php";

//Configuraciones de los perifericos
#define DHTPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT21   // DHT 21 AM2301
DHT dht(DHTPIN, DHTTYPE);

//Declaración de variables globales
String postStr;

WiFiClient client;

void setup() {
  //Representacion de los estados del arduino en el serial com
  Serial.begin(9600);
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.print("No conectado");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Inicialización de dht
   dht.begin();

   //Led para decir que esta encendido el dipositivo
   //pinMode(12,OUTPUT);
   //Led  para demostrar conexion wifi
   //pinMode(13,OUTPUT);
  
}
void loop() {
  //Led 12 para decir que equipo essta encendido
  //digitalWrite(13,HIGH);
  //digitalWrite(12,HIGH);
  
  //Lectura de variables fisicas
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  //Impresion de variables por serial

  Serial.println(t);
  Serial.println(h);

  //Accion en caso de que la lectura sea erronea
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Calculos de temperatura en grados celsius
  float hic = dht.computeHeatIndex(t, h, false);

  /*Metadatos y estructura necesaria para la creacion de un archivo html comprensible para el  webservice*/
  if (client.connect(server,80)) {
//    /*Nombre de variable temperatura para el servicio web BB.DD*/
//    postStr +="temperatura=";
//    postStr += String((int)t);
//    /*Nombre de variable humedad para el servicio web BB.DD*/
//    postStr +="&humedad=";
//    postStr += String((int)h);

  //postStr = "temperatura="+String((int)t)+"&humedad="+String((int)h);

  postStr = "temperatura="+String(t)+"&humedad="+String(h);

    //-------------------------------------------------------------------------------
    //Cuerpo del mensaje html

    client.print(String("POST ") + strurl + " HTTP/1.1" + "\r\n" + 
               "Host: " + server + "\r\n" +
               "Accept: */*" + "*\r\n" +
               "Content-Length: " + postStr.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "\r\n" + postStr);  

    //-------------------------------------------------------------------------------

  }
  client.stop();
  Serial.println("Waiting…");
  delay(1000);
}
