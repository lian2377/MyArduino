
#include <Process.h>

Process linux;

void setup() {
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.begin(9600);
  while(!Serial){
    digitalWrite(13,HIGH);
    delay(10);
    digitalWrite(13,LOW);
  }
  digitalWrite(13,HIGH);
  Bridge.begin();
  digitalWrite(13,LOW);
  Serial.println("Ready.");
}

void loop() {
  linux.runShellCommand("ifconfig");
  String result = "";
  while(linux.available()){
    char c = linux.read();
    result+=c;
  }
  if(result!=""){
    Serial.println(result);
  }
  delay(2000);
}
