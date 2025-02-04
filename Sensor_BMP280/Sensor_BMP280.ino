
/*

   Exemplo IBM Watson IoT Platform
   Hardware: new NodeMCU LoLin V3 + BMP280
   Logica:
   1. efetua conexao com a rede WiFi
   2. obtem as grandezas de temperatura, pressao e altitude do sensor BMP280
   3. conecta no servidor MQTT quickstart do IBM Watson IoT Platform
   4. publica a JSON string para o topico quickstart:MAC_ADDRESS
   referencias Bluemix e IoTF: 
   Author: Ant Elder
   https://developer.ibm.com/recipes/tutorials/connect-an-esp8266-with-the-arduino-sdk-to-the-ibm-iot-foundation/
   
   referencias conversao float para string
   http://www.hobbytronics.co.uk/arduino-float-vars
   http://forum.carriots.com/index.php/topic/61-wireless-gardening-with-arduino-cc3000-wifi-modules/page-2
   
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

//declaração objetos e variáveis globais. 
Adafruit_BMP280 bmp;

//atualize SSID e senha WiFi
const char* ssid = "HackaTruckVisitantes";
const char* password = "";

//D4 only for Lolin board
#define LED_BUILTIN D4

//Atualize os valores de Org, device type, device id e token
// #define ORG "ORG_ID"
// #define DEVICE_TYPE "Tipo_Dispositivo"
// #define DEVICE_ID "ID_dispositivo"
// #define TOKEN "token Dispositivo"

//broker messagesight server
// char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
// char topic[] = "iot-2/evt/status/fmt/json";
// char authMethod[] = "use-token-auth";
// char token[] = TOKEN;
// char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

float pressao = 0.0;
float altitude = 0.0;
float temperatura = 0.0;

char pressaostr[6];
char altitudestr[6];
char temperaturastr[6];


//WiFiClient wifiClient;
//PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
  Wire.begin((int) SDA , (int) SCL);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Iniciando...");
/*
  Serial.print("Conectando na rede WiFi "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("[INFO] Conectado WiFi IP: ");
  Serial.println(WiFi.localIP());
*/
  pinMode(LED_BUILTIN, OUTPUT);

  //--------------------------BMP280------------------------

  if (!bmp.begin(0x76))
  {
    Serial.println("Sensor BMP 280 nao encontrado. Reinicialize o Arduino.");
    while (1) 
     yield();
  }

}

void loop() {

  /*if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }*/

  // Lê e imprime os valores de temperatura, pressão e altitude
  Serial.print("Temperatura = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print("Pressao = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" m");
  // Pisca o LED embutido
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  getData();

  // Conversao Floats para Strings
  char TempString[32];  //  array de character temporario

  // dtostrf( [Float variable] , [Minimum SizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
  dtostrf(temperatura, 2, 1, TempString);
  String temperaturastr =  String(TempString);
  dtostrf(pressao, 2, 2, TempString);
  String pressaostr =  String(TempString);
  dtostrf(altitude, 2, 2, TempString);
  String altitudestr =  String(TempString);

  // Prepara JSON para IOT Platform
  //int length = 0;

  /*String payload = "{\"d\":{\"temperatura\": \"" + String(temperaturastr) + "\",\"pressao\":\"" + String(pressaostr) + "\",\"altitude\":\"" + String(altitudestr) + "\"}}";

  length = payload.length();
  Serial.print(F("\nData length"));
  Serial.println(length);

  Serial.print("Sending payload: ");
  Serial.println(payload);

  if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("Publish ok");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  } else {
    Serial.println("Publish failed");
  }
  */
  delay(500);
}
void getData() {
 // Lê os valores do sensor BMP280 e os armazena nas variáveis globais
  pressao = (float)bmp.readPressure();
  altitude = (float)bmp.readAltitude();
  temperatura = (float)bmp.readTemperature();
// Converte os valores em strings usando dtostrf
  dtostrf(pressao, 6, 0, pressaostr);
  dtostrf(altitude, 6, 0, altitudestr);
  dtostrf(temperatura, 6, 0, temperaturastr);
}
