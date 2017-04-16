#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void function(String payloadStr);
// Update these with values suitable for your network.

const char* ssid = "....";
const char* password = "....";
const char* mqtt_server = "broker.mqttdashboard.com";

#define DIRA 5 // D1
#define PWMA 0 // D3
#define DIRB 4 // D2
#define PWMB 2 // D4

WiFiClient espClient;
PubSubClient client(espClient);

char clientID[15];
char destinationTopic[] = "....";
int signals;
void callback(char* topic, byte* payload, unsigned int length) {
  String payloadStr = String();
  String topicStr = String(topic);
  
  // Convert byte to String
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  
  //Serial.print("Message arrived [");
  //Serial.print(topicStr);
  //Serial.print("] : ");
  Serial.println(payloadStr);
  function(payloadStr);
  
  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String clientId = ".....";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(destinationTopic, "hello, world");
      // Subscribing...
      client.subscribe(destinationTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(DIRA, OUTPUT); // D1
  pinMode(PWMA, OUTPUT); // D3
  pinMode(DIRB, OUTPUT); // D2
  pinMode(PWMB, OUTPUT); // D4
  pinMode(D0, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D0, HIGH);
  digitalWrite(D5, LOW);
}

void loop() {
    
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
void function(String payloadStr){
  
  if (payloadStr == "Connect" || payloadStr[0] == '1' || payloadStr[0] == '0'){ // status LED
    digitalWrite(D0, LOW);
    digitalWrite(D5, HIGH);
  }
  
  int sensorValue = analogRead(A0);
  if(sensorValue >= 700){
    digitalWrite(D6, HIGH);
  }
  else {
    digitalWrite(D6, LOW);
  // MOTOR A = LEFT MOTOR
  // MOTOR B = RIGHT MOTOR
  } // Forward
  if (payloadStr[0] == '1'){
    if (payloadStr[2] == '1'){ // Turn Left
      analogWrite(PWMB, 1023);
      digitalWrite(DIRB, 1);
      digitalWrite(DIRA, 0);
      digitalWrite(D7, HIGH);
      digitalWrite(D8, LOW);
    } 
    else if (payloadStr[3] == '1'){ // Turn Right
      analogWrite(PWMA, 1023);
      digitalWrite(DIRA, 1);
      digitalWrite(DIRB, 0);
      digitalWrite(D8, HIGH);
      digitalWrite(D7, LOW);
    }
    else{ // Forward
      analogWrite(PWMA, 1023);
      digitalWrite(DIRA, 1);
      analogWrite(PWMB, 1023);
      digitalWrite(DIRB, 1);
      digitalWrite(D7, LOW);
      digitalWrite(D8, LOW);
    }
  } // BACK
  else if (payloadStr[1] == '1'){ // Turn LEFT
    if (payloadStr[2] == '1'){
      analogWrite(PWMB, 0);
      digitalWrite(DIRB, 1);
      digitalWrite(DIRA, 0);
    }
    else if (payloadStr[3] == '1'){ // Turn Right
      analogWrite(PWMA, 0);
      digitalWrite(DIRA, 1);
      digitalWrite(DIRB, 0);
    }
    else{ // Backward
      analogWrite(PWMA, 0);
      digitalWrite(DIRA, 1);
      analogWrite(PWMB, 0);
      digitalWrite(DIRB, 1);
    }
  }  // Rotate LEFT
  else if (payloadStr[2] == '1'){
    analogWrite(PWMB, 1023);
    digitalWrite(DIRB, 1);
    analogWrite(PWMA, 0);
    digitalWrite(DIRA, 1);
  } // Rotate RIGHT
  else if (payloadStr[3] == '1'){
    analogWrite(PWMA, 1023);
    digitalWrite(DIRA, 1);
    analogWrite(PWMB, 0);
    digitalWrite(DIRB, 1);
  }
  else { // STOP
    digitalWrite(DIRA, 0);
    digitalWrite(DIRB, 0);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);
  }
}

