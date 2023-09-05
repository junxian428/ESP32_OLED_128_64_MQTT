#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "";
const char* password = "";
const char* mqttServer = "192.168.0.103";
const int mqttPort = 1883;


Adafruit_SSD1306 display(128, 64, &Wire, 4);


WiFiClient espClient;
PubSubClient client(espClient);

String receivedMessage = ""; // String to store the received message

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); // Set the callback function for handling incoming MQTT messages
  client.subscribe("led/control"); // Subscribe to the "led/control" topic
    // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println("MQTT Sub/Pub Starts");
  display.display();

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  receivedMessage = ""; // Clear the received message string before appending new data

  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    receivedMessage += (char)payload[i]; // Append each character to the receivedMessage string
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.print("Received message as a string: ");
  Serial.println(receivedMessage);
    display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.println(receivedMessage);
  display.display();
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
      client.subscribe("led/control"); // Subscribe again after reconnecting
    } else {
      Serial.print("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}
