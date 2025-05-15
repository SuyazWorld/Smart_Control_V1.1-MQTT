#include <WiFi.h>
#include <MQTT.h>
WiFiClient clientwifi; //karena kita pakai wifi connect ke mqtt, jadi kita buat objek wifi untuk mqtt
MQTTClient clientmqtt; //kita juga buat objek mqtt
const char clientID[] = "POPO123"; //this device client id, harus char, bisa sih string tapi nanti diubah lagi dari string ke char
const char ssid[] = "Wifi kost lantai 1";
const char pass[] = "enamenamtigaenam";
// const char ssid[] = "LumaByStelaProject";
// const char pass[] = "Terimakasihsudahpesan!";
// const char ssid[] = "wadudu";
// const char pass[] = "bgstt123";
const char* brokerserver =  "broker.emqx.io";

String topic1 = "switch/#"; //subscribe kesini
String topic_masuk_baru;
String Data_payload_baru;

const byte relay[] = {13,26,14,27}; 
unsigned long previousMillis = 0; // Stores the last time the LED was updated
const long interval = 1000;     // Interval at which to blink (milliseconds)
String r1,r2,r3,r4;

void setup() {
  Serial.begin(115200);

  for(int i = 0; i < 4; i++){
    pinMode(relay[i], OUTPUT);
  }
  for(int i = 0; i < 4; i++){
    digitalWrite(relay[i], HIGH);
  }

  WiFi.begin(ssid, pass);
  clientmqtt.begin(brokerserver, clientwifi); //start mqtt with object mqtt //parameter("broker", wifiobject)

  clientmqtt.onMessage(pesansubscribemasuk);
  reconnect();
  
}

void loop() {
  clientmqtt.loop(); //untuk mengecek apakah ada data yang diterima
  if(!clientmqtt.connected()){
    reconnect();
  }
  pesanclient_doit();
}
void reconnect(){
  //checking wifi
  // while (!WiFi.isConnected())  // Untuk ESP32 (tergantung library)
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    Serial.println("menghubungkan wifi");
    delay(500);
  }
  Serial.println("Wifi TERHUBUNG");
  //put setwill, this is used for device connection status, if device still not connected to mqtt
  // Konversi clientID ke String agar bisa digunakan dalam setWill
  String topicStatus = "switch/Status/" + String(clientID);
  // Gunakan topicStatus.c_str() untuk memastikan format sesuai, libray mqtt ini hanya menerima char saja
  clientmqtt.setWill(topicStatus.c_str(), "OFFLINE", true, 1); //topic, payload, retain,  //topicnya di fungsi setwill ini tidak bisa String melainkan harus char
  //checking mqtt
  while(!clientmqtt.connect(clientID)){
    Serial.print(".");
    Serial.println("menghubungkan MQTT");
    delay(500);
  }
  Serial.println("WIFI & MQTT BROKER TERHUBUNG");
  //lalu kita publish status online, untuk memberitahu client lain kalau device ini online
  clientmqtt.publish(topicStatus.c_str(), "ONLINE", true, 1);
  //subscribe
  clientmqtt.subscribe(topic1, 1); //topic, qos
}
void pesansubscribemasuk(String &topicmasuk, String &payloadmasuk){
  topic_masuk_baru = topicmasuk;
  Data_payload_baru = payloadmasuk;
  // Serial.println("");
  // Serial.println("Ada data masuk, Topic: " + topicmasuk + ", data: " + payloadmasuk);
}
void pesanclient_doit(){
  if(topic_masuk_baru == "switch/1/status"){
    if(Data_payload_baru == "ON"){
      digitalWrite(relay[0], LOW);
    } else if(Data_payload_baru == "OFF"){
      digitalWrite(relay[0], HIGH);
    }
    Serial.println("Relay 1 updated to: " + Data_payload_baru);
  }
  else if(topic_masuk_baru == "switch/2/status"){
    if(Data_payload_baru == "ON"){
      digitalWrite(relay[1], LOW);
    } else if(Data_payload_baru == "OFF"){
      digitalWrite(relay[1], HIGH);
    }
    Serial.println("Relay 2 updated to: " + Data_payload_baru);
  }
  else if(topic_masuk_baru == "switch/3/status"){
    if(Data_payload_baru == "ON"){
      digitalWrite(relay[2], LOW);
    } else if(Data_payload_baru == "OFF"){
      digitalWrite(relay[2], HIGH);
    }
    Serial.println("Relay 3 updated to: " + Data_payload_baru);
  }
  else if(topic_masuk_baru == "switch/4/status"){
    if(Data_payload_baru == "ON"){
      digitalWrite(relay[3], LOW);
    } else if(Data_payload_baru == "OFF"){
      digitalWrite(relay[3], HIGH);
    }
    Serial.println("Relay 4 updated to: " + Data_payload_baru);
  }

  // Setelah selesai, kosongkan agar tidak dieksekusi terus-menerus
  topic_masuk_baru = "";
  Data_payload_baru = "";
}
