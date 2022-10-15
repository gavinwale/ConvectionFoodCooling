#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

boolean fan_on = false;
// Adjust based on needs. Currently: 3 seconds between readings, 24 deg C final temp.
static unsigned long timeBetween = 2000;
float perfectTemp = 26.0;

#define FIVE 5
#define THREE 3
#define FOUR 4

void setup() {
  Serial.begin(9600);
  
  pinMode(FIVE,OUTPUT);
  pinMode(THREE,OUTPUT);
  pinMode(FOUR,OUTPUT);

  digitalWrite(THREE,HIGH);
  digitalWrite(FOUR,LOW);
  digitalWrite(FIVE,LOW);
}

static bool measureTemp(float *temperature, float *humidity)
{
  static unsigned long timeMeasured = millis();

  if (millis() - timeMeasured > timeBetween)
  {
    if (dht_sensor.measure(temperature, humidity) == true)
    {
      timeMeasured = millis();
      return(true);
    }
  }
  return(false);
}

void loop() {
  float temperature;
  float humidity;

  if(measureTemp(&temperature, &humidity) == true)
  {
    if (temperature > perfectTemp) {
      digitalWrite(FIVE,HIGH);
      if(! fan_on) {
        Serial.print("FAN ON - Temperature over ");
        Serial.println(perfectTemp);
        fan_on = true;
      }
    } else {
      digitalWrite(FIVE,LOW);
      if(fan_on) {
        Serial.println("FAN OFF - Perfect Temp reached!");
        fan_on = false;
      }
    }
    Serial.print("T = ");
    Serial.print(temperature);
    Serial.print(" deg. C");
  }
}
