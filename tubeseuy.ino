#include "DHT.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WifFi parameters
#define WLAN_SSID "ka"
#define WLAN_PASS "abcde12345"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "kumaradm"
#define AIO_KEY         "aio_eEfK095iFdFKJ2RXNgSlB4ctqkhd"

WiFiClient client;

// Setup the MQTT client class by passing the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");

#define DHTTYPE DHT11
#define DHTPIN 2
DHT dht(DHTPIN,DHTTYPE);

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));
  Serial.println(); Serial.println();
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  dht.begin();
}

uint32_t x=0;

void loop() {
  MQTT_connect();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.print("Humidity:  : ");
  Serial.println(h);

  temperature.publish(t);
  humidity.publish(h);
  delay(4000);
}

void MQTT_connect(){
  int8_t ret;

  if(mqtt.connected()){
    return;
  }
  Serial.print("Connecting to MQTT...");
  uint8_t retries = 3;
  while((ret = mqtt.connect()) !=0 ){
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if(retries == 0){
      while(1);
    }
  }
  Serial.println("MQTT Connected!");
}
