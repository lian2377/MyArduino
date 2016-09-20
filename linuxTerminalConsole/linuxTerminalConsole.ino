
#include <Process.h>

Process linux;

void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Bridge.begin();
  digitalWrite(13,LOW);
  Console.begin();
  while(!Console){
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(1000);
  }
  digitalWrite(13,LOW);
  while(true){
    String pswd = "";
    while(Console.available()){
      char c = Console.read();
      if(c!='\n' && c!='\r'){
        pswd+=c;
      }
      delay(10);
    }
    if(pswd=="cmd"){
      break;
    }else if(pswd!=""){
      byte b[128];
      pswd.getBytes(b,128);
      Console.print('?');
      for(int i=0;i<pswd.length();i++){
        Console.print(b[i]);
        Console.print(" ");
      }
      Console.println();
    }
    delay(10);
  }
  digitalWrite(13,HIGH);
  Console.println("Ready.");
}

void loop() {
  // put your main code here, to run repeatedly:
  String cmd = "";
  while(Console.available()){
    Console.print('.');
    char c = Console.read();
    if(c!='\n' && c!='\r'){
      cmd+=c;
    }
    Console.flush();
    delay(10);
  }
  if(cmd!=""){
    Console.println();
    Console.println("running shell " + cmd);
    linux.runShellCommand(cmd);
    Console.println("getting output");
    
    while(linux.available()){
      char c = linux.read();
      Console.print(c);
      delay(5);
    }
    Console.println("Done.");
  }
  delay(10);
}
