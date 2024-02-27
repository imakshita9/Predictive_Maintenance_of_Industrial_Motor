/* Program for interfacing MPU6050(vibration sensor) and Type-K Thermocouple Sensor with ESP32 and publishing the sensor readings to the RPI via MQTT
 * 
 *  work with MQTT protocol
 * 
 */
#include <WiFi.h>
#include <Adafruit_MPU6050.h>     //header file for MPU6050(vibration sensor)
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>         //MQTT header files for publish and subscribe of sensor data
#include <esp_wifi.h>             
#include <ArduinoJson.h> 
#include <Wire.h>
#include <max6675.h>          //header or library file for temperature sensor

Adafruit_MPU6050 mpu;         //creating an object mpu for vibration sensor

float frequency_Hz = 200.0;

DynamicJsonDocument temp_sensor_data_payload(1024);
DynamicJsonDocument vib_sensor_data_payload(1024);
DynamicJsonDocument combined_data_payload(1024);

char temp_data_format_for_mqtt_format[1024];
char vib_data_format_for_mqtt_format[1024];

const char* ssid =   "Milkyway";                                           //ssid - service set Identifier (Replace it with your ssid name)
const char* password =  "Pritish123";                                              // replace with ssid paasword
const char* mqttBroker = "192.168.245.64";                                           // broker address - replace it with your broker address/cloud broker - test.mosquitto.org
const int   mqttPort = 1883;                                                   // broker port number
const char* clientID = "diot";                                               // client-id 
const char* temp_topic_for_publish = "cdac/temp/data";                       // topic names
const char* vibration_topic_for_publish = "cdac/vibration/data";                // topic names
const char* combined_topic_for_publish = "cdac/sensor/data";                  //publishing both sensor data using one topic

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);
long lastReconnectAttempt = 0;

int tempDO = 19;                 //Define the pins that are interfacing with the MAX6675 thermocouple amplifier.
int tempCS = 23;
int tempCLK = 5;
MAX6675 temp(tempCLK, tempCS, tempDO);               //Create a MAX6675 object called thermocouple on the pins

boolean reconnect()
{
  if (client.connect(clientID)) 
  {
   Serial.println("Attempting to connect..");
  }
  return client.connected();
}

void setup() 
{
  mpu.begin();
  Serial.begin(9600);
  Serial.println("Attempting to connect...");
  WiFi.mode(WIFI_STA);                                        //for wifi 
//  esp_wifi_set_mac(WIFI_IF_STA,&newMACAddress[0]);
  WiFi.begin(ssid, password);                                     // Connect to WiFi.
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Couldn't connect to WiFi.");
  }
  
  Serial.println("ESP32 IP ADDRESS");
  Serial.print(WiFi.localIP());
  client.setServer(mqttBroker, mqttPort);                         // Connect to broker
  Serial.println("Connected to Broker");
  lastReconnectAttempt = 0;
}


void loop() 
{
  
  if (!client.connected())
  {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) 
    {                                                         // Try to reconnect.
      lastReconnectAttempt = now;
      if (reconnect())
      {                                          // Attempt to reconnect.192.168.
        lastReconnectAttempt = 0;
      }
    }
  }
  else 
  {                           // Connected.
    client.loop();
    float t = temp.readCelsius();
    temp_sensor_data_payload["temperature"] = t;
    combined_data_payload["temperature"]=t;
    Serial.println("Temperature"); 
    Serial.println(t);  
    serializeJson(temp_sensor_data_payload, temp_data_format_for_mqtt_format);                //serializing the data in JSON format
    client.publish(temp_topic_for_publish, temp_data_format_for_mqtt_format);                 //(topicname, payload)
    Serial.println("Message Published");
    delay(3000);

    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    sensors_event_t a,g,temp;
    mpu.getEvent(&a,&g,&temp);
    
    Serial.print("Acceleration X axis :");
    Serial.print(a.acceleration.x);
    Serial.print("Acceleration Y axis :");
    Serial.print(a.acceleration.y);
    Serial.print("Acceleration Z axis :");
    Serial.println(a.acceleration.z);

    Serial.print("Rotation X: ");               // Print out the values of GyroRange
    Serial.print(g.gyro.x);
    Serial.print(", Y: ");
    Serial.print(g.gyro.y);
    Serial.print(", Z: ");
    Serial.print(g.gyro.z);
    Serial.println(" rad/s");

    double accelerationX = a.acceleration.x / 16384.0;
    double accelerationY = a.acceleration.y / 16384.0;
    double accelerationZ = a.acceleration.z / 16384.0;
    
    double accelerationMagnitude = sqrt(pow(accelerationX, 2) + pow(accelerationY, 2) + pow(accelerationZ, 2));            //calculate acceleration magnitude do the RMS of g values.

    double vi = accelerationMagnitude * frequency_Hz * 9.81;
    //Serial.println(vi);
                                   
    vib_sensor_data_payload["vibration"] = vi;
    combined_data_payload["vibration"] = vi;    
    Serial.println("Vibration"); 
    Serial.println(vi); 
    serializeJson(vib_sensor_data_payload, vib_data_format_for_mqtt_format);
    client.publish(vibration_topic_for_publish, vib_data_format_for_mqtt_format);                 //(topicname, payload)
    Serial.println("Message Published");
    delay(3000);
    
    serializeJson(combined_data_payload, temp_data_format_for_mqtt_format);
    client.publish(combined_topic_for_publish, temp_data_format_for_mqtt_format);
    delay(3000);
  }
  
}
