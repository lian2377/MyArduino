
#include <Process.h>

Process linux;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Serial.begin(9600);
  while(!Serial){
    digitalWrite(13,HIGH);
    delay(10);
    digitalWrite(13,LOW);
  }
  digitalWrite(13,LOW);
  Serial.print('?');
  while(true){
    String pswd = "";
    while(Serial.available()){
      char c = Serial.read();
      if(c!='\n' && c!='\r'){
        pswd+=c;
      }
      delay(10);
    }
    if(pswd=="cmd"){
      break;
    }
  }
  digitalWrite(13,HIGH);
  Bridge.begin();
  digitalWrite(13,LOW);
  Serial.println("Ready.");
}

void loop() {
  // put your main code here, to run repeatedly:
  String cmd = "";
  while(Serial.available()){
    char c = Serial.read();
    if(c!='\n' && c!='\r'){
      cmd+=c;
    }
    delay(10);
  }
  if(cmd!=""){
    linux.runShellCommand(cmd);
    String result = "";
    while(linux.available()){
      char c = linux.read();
      result+=c;
    }
    if(result!=""){
      Serial.println(result);
    }
  }
  delay(10);
}
