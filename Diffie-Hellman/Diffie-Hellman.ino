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
#include <Process.h>

uint32_t p=23, g=5, a=6, A=0, B=0, key=0;

String serverAddr = "192.168.2.5";
String targetAddr = "192.168.2.10";

YunClient yClient;
PubSubClient client(yClient);

Process shell;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  B = 0;
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    B = B * 256;
    B += (uint32_t)payload[i];
  }
  Serial.println("Received B is " + B);
}

void reconnect() {
  shell.runShellCommand("ifconfig apcli0 |grep netmask |cut -d ' ' -f 2");
  String myAddr = "";
  while(shell.available()){
    char c = shell.read();
    myAddr+=c;
  }
  Serial.println("myMacAddr is " + myAddr);
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(("DHKE_" + myAddr).c_str(),"" + A);
      // ... and resubscribe
      client.subscribe(("DHKE_" + targetAddr).c_str());
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
  digitalWrite(13 ,LOW);
  A = g^a % p;
  Serial.println("A is " + A);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }else{
    if(B > 0){
      key = B^a % p;
      Serial.println("Key is " + key);
      B=0;
    }
    client.publish("myTopic","im not dead.");
    delay(5000);
  }
  client.loop();
}
