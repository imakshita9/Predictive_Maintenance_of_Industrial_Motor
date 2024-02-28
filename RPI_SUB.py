#Program for subscribing on topic "cdac/sensor/data" for receiving the data and storing in json file on RPI

import paho.mqtt.client as mqtt
import uuid
import time
import json

def on_connect(client, userdata, flags, rc):
    try:
        print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
        
        client.subscribe("cdac/sensor/data")        #subscribing on topic
    except Exception as e:
        print("Exception in Subscriber block",e)
        
        
# The callback for when a PUBLISH message is received from the server
def on_receive(client,userdata,msg):
    try:
        print(f'TOPIC: {msg.topic}, Message: {str(msg.payload.decode())}')
        sensor_payload = json.loads(msg.payload)
        if msg.topic == "cdac/sensor/data":
            data_temp = json.loads(msg.payload.decode())    #loading and decoding the temperature readings

        if(data_temp["temperature"] > 20 and data_temp["temperature"] < 45):    #Ambient temperature range for 3 phase motor
                print("motor temperature is normal \n ")
        elif(data_temp["temperature"] > 45 and data_temp["temperature"] < 60):
                print ("Motor overload..!! \n ")
        elif(data_temp["temperature"] > 60):
                print ("Motor overheating may cause critical issue..!! \n ")
                sensor_payload["temperature"] = data_temp["temperature"]
        
        if msg.topic == "cdac/sensor/data":
            data_vib = json.loads(msg.payload.decode())     #loading and decoding the vibration readings
            if(data_vib["vibration"] > 2  and data_vib["vibration"] < 4):
                print("Motor Vibration level is normal\n")
            elif(data_vib["vibration"] > 5):
                print ("Motor Vibration level is Abnormal\n ")
                sensor_payload["vibration"] = data_vib["vibration"]
        with open("sensor_data.json", "a") as f:
            f.write(json.dumps(sensor_payload))     #storing the payload in json file
            f.write("\n")
            f.close()                
                    
    except Exception as e:
        print("Exception in on message block",e)
try:
    mqtt_client_id=uuid.uuid4().hex
    """
    Client(client_id="", clean_session=True, userdata=None, protocol=MQTTv311, transport="tcp")
    """
    client = mqtt.Client(client_id=mqtt_client_id)
    print(client)
    client.on_connect = on_connect
    client.on_message = on_receive
    client.connect("192.168.1.6", 1883, 60)         #establishing connection with the RPI on port 1883
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
    client.loop_forever()
except Exception as e:
    print("Excption in main",e)
