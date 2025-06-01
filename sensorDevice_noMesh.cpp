// Autores: Grupo Moskitto GS - FIAP 1ESPA
// Gabriel Kato, Gabriel Couto, João Vitor
// Versão v1.0 - revisão 2025-06-01

// Bibliotecas usadas no projeto (dependencias)
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// Configurações Wi-Fi
const char* ssid = "Wokwi-GUEST"; // Nome da rede Wi-Fi
const char* password = ""; // Senha da rede Wi-Fi

// Configurações do Broker MQTT
const char* mqttServer = "20.55.19.44"; // IP do Broker MQTT
const int mqttPort = 1883; // Porta do Broker MQTT (Orion Context Broker)
const char* mqttUser = ""; // Usuário do MQTT (se necessário)
const char* mqttPassword = ""; // Senha do MQTT (se necessário)
const char* mqttClientID = "fiware_001"; // ID do cliente MQTT

// Tópicos MQTT (configurado para dispositivo SensorDevice:001)
// (para adicionar mais dispositivos fazer as requisições POST no Postman de acordo com o FIWARE)
const char* topicTemperature = "/TEF/SensorDevice001/attrs/temperature";
const char* topicHumidity = "/TEF/SensorDevice001/attrs/humidity";
const char* topicWaterLevel = "/TEF/SensorDevice001/attrs/waterLevel";

// Pinos
const int WATERLVLSENSOR_PIN = 0; // Pino analógico para o sensor FD10
const int DHT_PIN = 8; // Pino digital para o sensor DHT22
const int default_D4 = 2; // Pino do LED onboard

// Inicialização dos sensores
DHTesp dht;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 // Inicia o monitor serial (115200 baud)
 Serial.begin(115200);
 
 // Conecta no Wi-Fi
 setupWiFi();

 // Inicializa o sensor DHT22
 dht.setup(DHT_PIN, DHTesp::DHT22);
 
 // Inicializa o cliente MQTT
 client.setServer(mqttServer, mqttPort);
 client.setCallback(mqttCallback);
 
 // Aguarda a conexão
 delay(5000);
}

void loop() {
 // Mantém a conexão MQTT
 if (!client.connected()) {
 reconnectMQTT();
 }
 client.loop();

 // Lê os valores dos sensores
 float temperature = dht.getTemperature();
 float humidity = dht.getHumidity();
 float waterLevel = analogRead(WATERLVLSENSOR_PIN);

 // Envia os dados via protocolo MQTT
 sendDataToFIWARE(temperature, humidity, waterLevel);

 // Aguarda 5 segundos antes de enviar novamente
 delay(5000);
}

void setupWiFi() {
 // Conecta-se à rede Wi-Fi
 WiFi.begin(ssid, password);
 Serial.print("Conectando no Wi-Fi");
 
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }

 // Exibe o IP local atribuído
 Serial.println();
 Serial.println("Conectado à rede Wi-Fi");
 Serial.print("IP: ");
 Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
 // Tenta reconectar ao Broker MQTT
 while (!client.connected()) {
 Serial.print("Tentando conectar ao MQTT...");
 
 if (client.connect(mqttClientID, mqttUser, mqttPassword)) {
 Serial.println("Conectado ao Broker MQTT");
 } else {
 Serial.print("Falha na conexão, tentando novamente em 5 segundos.");
 delay(5000);
 }
 }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
 // Função para receber mensagens/payloads remotos 
 String message = "";
 for (unsigned int i = 0; i < length; i++) {
 message += (char)payload[i];
 }
 Serial.print("Mensagem recebida: ");
 Serial.println(message);
}

void sendDataToFIWARE(float temperature, float humidity, float waterLevel) {
 // Formata a mensagem para temperatura e umidade
 String temperatureMsg = String(temperature);
 String humidityMsg = String(humidity);
 
 // Formata o valor do nível da água (converte leitura analógica para percentual)
 int waterlevelPercent = map(waterLevel, 0, 4095, 0, 100);
 String waterlevelMsg = String(waterlevelPercent);
 
 // Envia os dados de temperatura, umidade e nível da água via MQTT
 Serial.println("Enviando os dados para o FIWARE...");
 
 client.publish(topicTemperature, temperatureMsg.c_str());
 client.publish(topicHumidity, humidityMsg.c_str());
 client.publish(topicWaterLevel, waterlevelMsg.c_str());
 
 // Imprime os dados enviados ao FIWARE no Serial
 Serial.print("Nivel da Agua: ");
 Serial.println(waterlevelMsg);
 Serial.print("Umidade: ");
 Serial.println(humidityMsg);
 Serial.print("Temperatura: ");
 Serial.println(temperatureMsg);
}