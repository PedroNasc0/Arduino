#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
// Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"



//objetos das classes WiFiClient e HTTPClient 
WiFiClient client;
HTTPClient httpClient;
//configurações de Wi-Fi e URL que o ESP8266 usará para se conectar à rede Wi-Fi e enviar os dados para o servidor web.
const char *WIFI_SSID = "HackaTruck";
const char *WIFI_PASSWORD = "eiaTruck2016";
const char *URL = "http://127.0.0.1:1880/mandar";

int output_value ;
int sensor_pin = A0;

int const PULSE_SENSOR_PIN = 0;   // 'S' Signal pin connected to A0
int Signal;
int myBPM;             // Store incoming ADC data. Value can range from 0-1024
int Threshold = 550;       // Determine which Signal to "count as a beat" and which to ignore.

//inicia a comunicação serial para depuração, conecta-se à rede Wi-Fi especificada e aguarda até que a conexão seja estabelecida antes de continuar.

void setup(){

  Serial.begin(9600);          // For Serial Monitor

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold); 


  //pinMode(LED_BUILTIN,OUTPUT);  // Built-in LED will blink to your heartbeat 
  //Serial.begin(9600); 

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    Serial.println("Connected");
    delay(2000);
    // Set comm speed for serial plotter window
}
// faz o POST da variavel na URL do Node-Red.
void loop(){

  
  Signal = analogRead(PULSE_SENSOR_PIN); // Read the sensor value     

  delay(5000);

  // Lê o sinal do sensor
 // int sensorValue = analogRead(PULSE_SENSOR_PIN); 

  Signal = map(Signal, 0, 1023, 0, 100);
  // Converte para string
  String var = "BPM:"+String(Signal);

  Serial.println(var);
  // Inicia conexão HTTP
  httpClient.begin(client, URL);
  
  // Envia valor do sensor via POST
  httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
  httpClient.POST(var);

  httpClient.end();
  
  delay(2000);
}

