
#include <SPI.h>
#include <YunClient.h>
#include <PubSubClient.h>
#include <Process.h>
#include <PowerMod.h>

uint32_t p=23, g=5, a=6, A=0, B=0, key=0;

String serverAddr = "192.168.2.5";
String targetAddr = "192.168.2.3";
String myAddr = "";
String mqttClientName = "arduinoClientA";

YunClient yClient;
PubSubClient client(yClient);

Process shell;

void callback(char* topic, byte* payload, unsigned int length) {
  Console.print("Message arrived [");
  Console.print(topic);
  Console.print("] ");
  B = 0;
  String temp = "";
  for (int i=0;i<length;i++) {
    temp += (char)payload[i];
  }
  B = temp.toInt();
  Console.println("Received B is " + String(B));
  
  Console.println("DHKE_" + myAddr + "  A is " + String(A));
  if(client.publish(("DHKE_" + myAddr).c_str(), String(A).c_str())){
    Console.println("Publish completed.");
  }else{
    Console.println("Publish failed.");
  }
}

void reconnect() {
  
  // Loop until we're reconnected
  while (!client.connected()) {
    Console.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqttClientName.c_str())) {
      Console.println("connected");
      // Once connected, publish an announcement...
      
      // ... and resubscribe
      client.subscribe(("DHKE_" + targetAddr).c_str());
    } else {
      Console.print("failed, rc=");
      Console.print(client.state());
      Console.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(13,OUTPUT);
  digitalWrite(13 ,LOW);
  Bridge.begin();
  digitalWrite(13 ,HIGH);
  Console.begin();
  while(!Console);

  client.setServer(serverAddr.c_str(), 1883);
  client.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);
  digitalWrite(13 ,LOW);

  myAddr = "";
  shell.runShellCommand("ifconfig apcli0 |grep Mask |cut -d \':\' -f 2 |cut -d \' \' -f 1");
  while(shell.available()){
    char c = shell.read();
    if(c != '\n' && c != '\r'){
      myAddr+=c;
    }
  }
  Console.println("myMacAddr is " + myAddr);
  A = pow_mod(g,a,p);
  //A = round(pow(g,a)) % p;
  Console.println("A is " + String(A));
}

int count = 10;

void loop()
{
  if (!client.connected()) {
    reconnect();
  }else{
    if(B > 0){
      Console.println("A is " + String(A));
      key = pow_mod(B,a,p);
      Console.println("Key is " + String(key));
      if(--count <= 0){
        count=10;
        B=0;
      }
      delay(5000);
    }else{
      Console.println("DHKE_" + myAddr + "  A is " + String(A));
      if(client.publish(("DHKE_" + myAddr).c_str(), String(A).c_str())){
        Console.println("Publish completed.");
      }else{
        Console.println("Publish failed.");
      }
    }
    //client.publish("myTopic","im not dead.");
    delay(2000);
  }
  client.loop();
}
