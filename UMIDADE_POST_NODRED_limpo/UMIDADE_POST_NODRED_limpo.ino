#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient httpClient;

const char *WIFI_SSID = "HackaTruck";
const char *WIFI_PASSWORD = "eiaTruck2016";
const char *URL = "http://192.168.128.246:1880/umidadePost";

int output_value ;

const int pinoSensor = A0; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO

int analogSoloSeco = 400; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 150; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)
 
void setup(){ 
    Serial.begin(9600); //INICIALIZA A SERIAL
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected");
    Serial.println("Lendo a umidade do solo..."); //IMPRIME O TEXTO NO MONITOR SERIAL
    delay(2000);
}

void loop(){
  valorLido = analogRead(pinoSensor); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
  valorLido = map(valorLido,750, 180, 0 , 100); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
  Serial.print("Umidade do solo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(valorLido); //IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
  Serial.println("%"); //IMPRIME O CARACTERE NO MONITOR SERIAL
  
  String data = "teste="+String(valorLido);

    httpClient.begin(client, URL);
    httpClient.addHeader("Content-Type", "application/x-www-form-urlencoded");
    httpClient.POST(data);
    String content = httpClient.getString();
    httpClient.end();
    Serial.print(data); 
    Serial.println(content);
    delay(1000);
    
}