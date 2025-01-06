#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>

#define DHTPIN 4      
#define DHTTYPE DHT11      
DHT dht(DHTPIN, DHTTYPE); 

#define LEDPIN 15         


// Replace with your network credentials
const char* ssid = "TOPNET_1650";
const char* password = "mf3dl239v3";

// MQTT Broker address
const char* mqttServer = "192.168.1.19";  
const int mqttPort = 1883;              
const char* mqttUser = "mqtt_user";       
const char* mqttPassword = "mqtt_passwrd"; 



WiFiClient espClient;
PubSubClient client(espClient);

// Connect to Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
}

// Callback function for handling received messages 
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == "esp32/led") {
    if (message == "ON") {
      digitalWrite(LEDPIN, HIGH);  // Turn LED on
    } else if (message == "OFF") {
      digitalWrite(LEDPIN, LOW);  // Turn LED off
    }
  }
}

// Connect to the MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT!");
      // Subscribe to LED control topic
      client.subscribe("esp32/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);  // Initialize the LED pin as output

  // Initialize DHT sensor
  dht.begin();

  setup_wifi();    // Connect to WiFi
  client.setServer(mqttServer, mqttPort);  // Set the MQTT server
  client.setCallback(callback);   // Set the callback function for received messages

  // Ensure we're connected to the MQTT broker
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any of the readings failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Publish temperature and humidity to MQTT topics
  publishData(temperature, humidity);
  
  // Wait before next reading
  delay(1000);
}


// Function to publish temperature and humidity
void publishData(float temperature, float humidity) {
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  client.publish("esp32/temperature", tempString); // Publish temperature

  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  client.publish("esp32/humidity", humString); // Publish humidity

  Serial.print("Published Temperature: ");
  Serial.println(temperature);
  Serial.print("Published Humidity: ");
  Serial.println(humidity);
}

// Callback function for handling MQTT messages
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(message);

}
