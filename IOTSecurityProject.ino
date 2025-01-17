#include <WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <mbedtls/aes.h>

#define DHTPIN 4      
#define DHTTYPE DHT11      
DHT dht(DHTPIN, DHTTYPE); 

#define LEDPIN 15         

// // Replace with your network credentials
// const char* ssid = "TOPNET_1650";
// const char* password = "mf3dl239v3";

const char* ssid = "Galaxy S9";
const char* password = "yessin123";

// MQTT Broker address
const char* mqttServer = "192.168.43.77";  
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
  float tem = dht.readTemperature();
  float hum = dht.readHumidity();

  // Vérifier si la lecture a échoué
  if (isnan(tem) || isnan(hum)) {
    Serial.println("Échec de la lecture du capteur !");
    return;
  }

  // Convertir les données en chaînes de caractères
  char temperature[17];  // 16 caractères + '\0' pour le terminateur de chaîne
  char humidity[17];  // 16 caractères + '\0' pour le terminateur de chaîne

  dtostrf(tem, 0, 6, temperature);  // Convertir float en string avec 6 décimales
  dtostrf(hum, 0, 6, humidity);  // Convertir float en string avec 6 décimales

  // S'assurer que la chaîne fait 16 caractères (paddé avec des espaces si nécessaire)
  if (strlen(temperature) < 16) {
    int len = strlen(temperature);
    for (int i = len; i < 16; i++) {
      temperature[i] = ' ';  // Padded with space
    }
    temperature[16] = '\0';  // Ensure null-termination
  }

  if (strlen(humidity) < 16) {
    int len = strlen(humidity);
    for (int i = len; i < 16; i++) {
      humidity[i] = ' ';  // Padded with space
    }
    humidity[16] = '\0';  // Ensure null-termination
  }

  // Encrypt and publish temperature and humidity
  encryptAndPublishData(temperature, humidity);
  
  // Wait before next reading
  delay(1000);
}


// Function to encrypt and publish temperature and humidity
void encryptAndPublishData(char* temperature, char* humidity) {

  // AES encryption key (16 bytes, must be 16 characters for AES-128)
  const char *key = "1234567890123456";

  uint8_t encryptedTemp[16];
  uint8_t encryptedHum[16];

  // Encrypt the temperature and humidity strings
  encryptAES(key, temperature, encryptedTemp);
  encryptAES(key, humidity, encryptedHum);

  // Convert encrypted data to hexadecimal string format
  char encryptedTempHex[33]; // 32 hex characters + 1 for null terminator
  char encryptedHumHex[33];
  hexToString(encryptedTemp, sizeof(encryptedTemp), encryptedTempHex);
  hexToString(encryptedHum, sizeof(encryptedHum), encryptedHumHex);

  // Publish the encrypted temperature and humidity to MQTT topics
  client.publish("esp32/encryptedTemperature", encryptedTempHex);
  client.publish("esp32/encryptedHumidity", encryptedHumHex);

  // Log to Serial Monitor
  Serial.print("Published Encrypted Temperature: ");
  Serial.println(encryptedTempHex);
  Serial.print("Published Encrypted Humidity: ");
  Serial.println(encryptedHumHex);
}

// Function to encrypt the message using AES
void encryptAES(const char *key, const char *message, uint8_t *output) {
  mbedtls_aes_context aes;
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, (const unsigned char *)key, 128);
  mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, (const unsigned char *)message, output);
  mbedtls_aes_free(&aes);
}

// Function to convert data to hexadecimal string
void hexToString(const uint8_t *data, size_t length, char *output) {
  for (size_t i = 0; i < length; i++) {
    sprintf(&output[i * 2], "%02x", data[i]);
  }
  output[length * 2] = '\0';  // Null-terminate the string
}