void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Ready.");
}

void loop() {
  String msg = "";
  while(Serial.available()){
    char c = Serial.read();
    if(c!='\n' && c!='\r'){
      msg+=c;
    }
    delay(10);
  }
  if(msg!=""){
    Serial.println("hello"+msg);
  }

}
