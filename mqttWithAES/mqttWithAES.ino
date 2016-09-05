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
uint8_t key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

char* padding(char* data, int dataLen, int* outBlockCount)
{
  int bytesEachBlock = 128 / 8; // block size
  int paddCount = bytesEachBlock - (dataLen % bytesEachBlock);
  int newLen = dataLen;
  if ( paddCount < 16)
    newLen = dataLen + paddCount;
  *outBlockCount = newLen / bytesEachBlock;
  char * newData = (char*)malloc(newLen + 1);
  memcpy(newData, data, dataLen);
  memset(newData + dataLen, 5, newLen - dataLen);
  newData[newLen] = 0;
  return newData;
}

void unpadding(char* padded, int blockCount)
{
  for (int i = 15; i >= 0; --i)
  {
    if (padded[16 * (blockCount - 1) + i] == 0x5)
      padded[16 * (blockCount - 1) + i] = 0;
    else
      break;
  } 
}

int aes256_enc(char* plaintext, unsigned int len, uint8_t* key, char* &outCipherText)
{
  int blockCount = 0;

  char * padded = padding(plaintext, len, &blockCount);

  for (int i = 0 ; i < blockCount; ++i)
  {
    aes256_enc_single(key, padded + 16 * i);
  }
  outCipherText = padded;
  return 16 * blockCount;
}

int aes256_dec(char* ciphertext, unsigned int len, uint8_t* key, char* &outPlainText)
{
  int blockCount = 0;
  char * padded = padding(ciphertext, len, &blockCount);

  for (int i = 0 ; i < blockCount; ++i)
  {
    aes256_dec_single(key, padded + 16 * i);
  }

  unpadding(padded, blockCount);

  outPlainText = padded;
  return strlen(padded);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  char *plain = NULL;
  aes256_dec((char*)payload, length, key, plain);
  Serial.print("decrypted:");
  Serial.println(plain);
  free(plain);
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
      char *edata = NULL;
      aes256_enc(data, strlen(data), key, edata);
      client.publish("myTopic",edata);
      // ... and resubscribe
      client.subscribe("myTopic");
      free(edata);
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
