/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <SPI.h>
#include <YunClient.h>
#include <PubSubClient.h>
#include <AESLib.h>

String serverAddr = "50.71.0.121";
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  aes128_dec_single(key, payload);
  Serial.print("decrypted:");
  Serial.println(payload);
}

YunClient yClient;
PubSubClient client(yClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      char data[] = "hellow world";
      aes128_enc_single(key, data);
      client.publish("myTopic",data);
      // ... and resubscribe
      client.subscribe("myTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(13,OUTPUT);
  Bridge.begin();
  digitalWrite(13 ,HIGH);
  Serial.begin(9600);
  while(!Serial);

  client.setServer(serverAddr.c_str(), 1883);
  client.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }else{
    client.publish("myTopic","im not dead.");
    delay(5000);
  }
  client.loop();
}
