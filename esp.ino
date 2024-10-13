#include <WiFi.h>
#include <PubSubClient.h>

// Configurações de Wi-Fi
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Configurações do MQTT
const char* mqtt_server = "broker.hivemq.com"; // Insira o endereço do seu servidor MQTT
const int mqtt_port = 1883; // Porta padrão do MQTT
const char* mqtt_topic = "SEU/TOPICO";

// Criação dos objetos WiFi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Variável que será incrementada
int count = 0;

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

  // Publica o número incrementado a cada 3 segundos
  String message = String(count);
  Serial.print("Publicando: ");
  Serial.println(message);
  client.publish(mqtt_topic, message.c_str());

  count++;
  delay(3000); // Espera 3 segundos antes de publicar o próximo valor
}