
void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  Bridge.begin();
  digitalWrite(13,LOW);
  delay(50);
  digitalWrite(13,HIGH);
  Console.begin();
  while(!Console){
    digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(1000);
  }
  digitalWrite(13,LOW);
  Console.println("Ready.");
}

void loop() {
  // put your main code here, to run repeatedly:
  String msg = "";
  while(Console.available()){
    char c = Console.read();
    if(c!='\n' && c!='\r'){
      msg+=c;
    }
    delay(10);
  }
  if(msg!=""){
    Console.println();
    Console.println("Hello " + msg);
  }
  delay(10);
}
