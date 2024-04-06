#include <esp_now.h>
#include <WiFi.h>

#define vrx 34
#define vry 33
#define sw 5
// potentiometer disabled
//#define pot 36
#define fwd 32  // left
#define rev 27  // right
#define lef 26  // back
#define rit 35  // forward

uint8_t broadcastAddress[] = {0xC0, 0x49, 0xEF, 0xEF, 0xEB, 0x28};  // 78:21:84:92:77:A4  C0:49:EF:EF:EB:28


typedef struct struct_message {
  int x;
  int y;
  int swi;
  int front;
  int back;
  int left;
  int right;
  int speed;
} struct_message;
struct_message data;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  pinMode(vrx, INPUT);
  pinMode(vry, INPUT);
  pinMode(sw, INPUT);
  // Pot is disabled
  //pinMode(pot, INPUT);
  pinMode(fwd, INPUT);
  pinMode(rev, INPUT);
  pinMode(lef, INPUT);
  pinMode(rit, INPUT);
}

void loop() {

  if(digitalRead(sw) == 1){
    data.swi = 0;
  } else{
    data.swi = 1;
  }
  
  data.x = analogRead(vrx);
  data.y = analogRead(vry);
  data.front = digitalRead(fwd);
  data.back = digitalRead(rev);
  data.left = digitalRead(lef);
  data.right = digitalRead(rit);
  //data.speed = map(analogRead(pot), 0, 4095, 0, 255);

    // Assign a fixed speed value or implement your desired speed control mechanism here
  data.speed = 50; // Example: Setting a fixed speed value of 100

  Serial.print(data.x);
  Serial.print("\t");
  Serial.print(data.y);
  Serial.print("\t");
  Serial.print(data.swi);
  Serial.print("\t");
  Serial.print(data.front);
  Serial.print("\t");
  Serial.print( data.back);
  Serial.print("\t");
  Serial.print(data.left);
  Serial.print("\t");
  Serial.print(data.right);
  Serial.print("\t");
  Serial.print(data.speed);
  Serial.println("\t");

  esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data));
  delay(100);

}