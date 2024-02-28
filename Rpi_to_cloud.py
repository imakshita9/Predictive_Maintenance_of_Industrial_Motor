#Program for publishing the sensor data to the AWS IOT Core and in DynamoDB

import time,os
import paho.mqtt.client as mqtt
import ssl
import json
import threading
import time
import uuid


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

#Attaching all the certificates and keys with the payload to publish data securely
root_ca = "/home/project/Desktop/project/Final/AmazonRootCA1.pem"       
device_certificate="/home/project/Desktop/project/Final/6289305d36ced6336382e898b435b66e75239c6641ab0cb8cce84441c35d666d-certificate.pem.crt"
private_key="/home/project/Desktop/project/Final/6289305d36ced6336382e898b435b66e75239c6641ab0cb8cce84441c35d666d-private.pem.key"
aws_iot_end_point = "a29061mcgm8s0y-ats.iot.us-east-2.amazonaws.com"
keep_alive_interval = 60        #after every 60 seconds will notify connection is alive
broker_secure_port_number = 8883    #Secure port number for publishing and subscribing of MQTT
sensor_sample_interval = 30     #Will publish the data after every 30 seconds
print(sensor_sample_interval)
client = mqtt.Client(client_id="testiotplat4567")
client.on_connect = on_connect
client.tls_set(ca_certs=root_ca, certfile=device_certificate, keyfile=private_key, tls_version=ssl.PROTOCOL_TLSv1_2)
client.tls_insecure_set(True)
client.connect(aws_iot_end_point,broker_secure_port_number, keep_alive_interval) #Taken from REST API endpoint - Use your own. 	


def send_data_to_aws_iot():
    try:
        while (1): 
            with open("sensor_data.json","r") as f:
                sensor_data_telemetry = f.read()        #reading the payload from json file
            print("Sending Data comming from ESP32")
            for line in sensor_data_telemetry.lstrip().split("\n"):
                stripped_line = line.lstrip()         #to remove all back slahses character
                if not stripped_line:
                    continue
                client.publish("d/mydevice/telemetry", payload=stripped_line , qos=1, retain=False)     #publishing the data on topic d/mydevice/telemetry
                time.sleep(100)
                print(stripped_line)
            time.sleep(sensor_sample_interval)
    except Exception as e:
        print("Exception occured in sending data ",e)
          
device_1_telemetry = threading.Thread(target=send_data_to_aws_iot, args=())
device_1_telemetry.start()
device_1_telemetry.join()    


