// Mini Project 3: ESP32 / DHT22 / ThingSpeak
// Demonstration transport: HTTP. Use only a disposable test channel.
// Keep real API keys out of public Wokwi projects and submitted source.
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

const char* WRITE_API_KEY = "PASTE_CHANNEL_WRITE_KEY";
constexpr int DHT_PIN = 15;
constexpr float TEMP_LIMIT = 35.0;
constexpr float HUM_LIMIT = 80.0;
constexpr unsigned long INTERVAL_MS = 20000;
DHT dht(DHT_PIN, DHT22);
unsigned long lastCycle = 0;
bool firstCycle = true;

bool connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return true;
  WiFi.begin("Velxio-GUEST");
  unsigned long started = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - started < 10000) {
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi unavailable; retry next cycle.");
    return false;
  }
  Serial.println("WiFi connected.");
  return true;
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  Serial.println("IoT Weather Station | T limit 35 C | RH limit 80 %");
  delay(2100); // DHT22 startup and minimum sampling interval
}

void loop() {
  if (!firstCycle && millis() - lastCycle < INTERVAL_MS) {
    delay(10);
    return;
  }
  firstCycle = false;
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity) ||
      temperature < -40 || temperature > 80 ||
      humidity < 0 || humidity > 100) {
    Serial.println("Invalid DHT22 reading; no cloud upload.");
    lastCycle = millis();
    return;
  }
  int alarm = (temperature > TEMP_LIMIT || humidity > HUM_LIMIT) ? 1 : 0;
  Serial.printf("T=%.1f C | RH=%.1f %% | alarm=%d\n", temperature, humidity, alarm);
  if (connectWiFi()) {
    if (String(WRITE_API_KEY) == "PASTE_CHANNEL_WRITE_KEY") {
      Serial.println("SENSOR DEMO ONLY: add channel Write API Key for cloud uploads.");
    } else {
      WiFiClient client;
      HTTPClient http;
      http.setTimeout(10000);
      if (http.begin(client, "http://api.thingspeak.com/update")) {
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String body = "api_key=" + String(WRITE_API_KEY)
          + "&field1=" + String(temperature, 1)
          + "&field2=" + String(humidity, 1)
          + "&field3=" + String(alarm);
        int status = http.POST(body);
        String response = http.getString();
        response.trim();
        if (status == 200 && response.toInt() > 0) {
          Serial.printf("UPLOAD OK | entry_id=%ld\n", response.toInt());
        } else {
          Serial.printf("UPLOAD FAILED | HTTP=%d | response=%s\n", status, response.c_str());
        }
        http.end();
      } else Serial.println("Could not initialize HTTP request.");
    }
  }
  lastCycle = millis(); // at least 20 seconds AFTER request completes
}

