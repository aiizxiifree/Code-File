#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// constants won't change. They're used here to
// set pin numbers:
const int buttonForward = D0;   //Forward
const int buttonBackward = D1;  //Back
const int buttonLeft = D2;
const int buttonRight = D5;
//const int ledPin =  D1;      // the number of the LED pin
const char* ssid = "....";
const char* password = "....";
const char* mqtt_server = "broker.mqttdashboard.com";
int valForward = 0;
int valBackward = 0;
int valLeft = 0;
int valRight = 0;
//int valSpeed = 0;
//const int AnalogIn = A0;
const char* aaa2;


WiFiClient espClient;
PubSubClient client(espClient);

long Forwardcount = 0;
long Backwardcount = 0;
long Leftcount = 0;
long Rightcount = 0;
long lastMsg = 0;
char destinationTopic[]   = "....";

// variables will change:
int ForwardState = 0;         // variable for reading the pushbutton status
int BackwardState = 0;
int LeftState = 0; 
int RightState = 0; 
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
    String clientId = "...."; //
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(destinationTopic, "Connect");
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
//  pinMode(ledPin, OUTPUT);
  pinMode(buttonForward, INPUT);
  pinMode(buttonBackward, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonRight, INPUT);
  pinMode(D6, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  digitalWrite(D6, HIGH);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
  // forward
  ForwardState = digitalRead(buttonForward);
  if (ForwardState == LOW) {
    Forwardcount = 0;
  } else {
    Forwardcount = 1;
  }
  BackwardState = digitalRead(buttonBackward);
  if (BackwardState == LOW) {
    Backwardcount = 0;
  } else {
    Backwardcount = 1;
  }
  LeftState = digitalRead(buttonLeft);
  if (LeftState == LOW) {
    Leftcount = 0;
  } else {
    Leftcount = 1;
  }
  RightState = digitalRead(buttonRight);
  if (RightState == LOW) {
    Rightcount = 0;
  } else {
    Rightcount = 1;
  }
  valForward = digitalRead(Forwardcount);
  valBackward = digitalRead(Backwardcount);
  valLeft = digitalRead(Leftcount);
  valRight = digitalRead(Rightcount);
  // FBLRSpeed
  String publishStr = "";
  publishStr.concat(valForward);
  publishStr.concat(valBackward);
  publishStr.concat(valLeft);
  publishStr.concat(valRight);
  delay(1);

  //valSpeed = analogRead(A0);
  //publishStr.concat(valSpeed);
  //publishStr.concat('S');
  client.publish(destinationTopic,  publishStr.c_str());
  Serial.println(publishStr);
  delay(1);

}
