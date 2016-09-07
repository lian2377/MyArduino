#include <YunClient.h>

YunClient client;

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  while(!Serial);
  
  Serial.println("Connecting...");
  if(client.connect("192.168.2.5",80)){
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: 192.168.2.5");
    client.println("Connection: close");
    client.println();
  }else{
    Serial.println("connection failed");
  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }

}
