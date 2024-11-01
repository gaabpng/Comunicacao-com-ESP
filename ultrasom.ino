#include <WiFi.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>

// Configurações de Wi-Fi
const char* ssid = "nomeWifi";
const char* password = "SenhaWifi";

// Configurações do MQTT
const char* mqtt_server = "broker.hivemq.com"; // Insira o endereço do seu servidor MQTT
const int mqtt_port = 1883; // Porta padrão do MQTT
const char* mqtt_topic = "SENSOR/ULTRASSOM";

// Criação dos objetos WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Variável que será incrementada
int count = 0;

//Define os pinos para o trigger e echo
#define pino_trigger 23
#define pino_echo 22

//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);

// Função para conectar ao Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função de callback do MQTT (não será usada, mas precisa ser declarada)
void callback(char* topic, byte* message, unsigned int length) {}

// Função para conectar ao servidor MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  //Le as informacoes do sensor, em cm e pol
  float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec   );

  // Publica o número incrementado a cada 3 segundos
  String message = String(cmMsec);
  Serial.print("Publicando: ");
  Serial.println(message);
  client.publish(mqtt_topic, message.c_str());

  delay(3000); // Espera 3 segundos antes de publicar o próximo valor
}