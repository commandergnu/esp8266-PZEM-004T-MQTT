#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <AsyncMqttClient.h>
#include <Ticker.h>
#include <PZEM004Tv30.h>
#include <SoftwareSerial.h>

SoftwareSerial pzemSWSerial(14, 12); //gpio pzem
PZEM004Tv30 pzem;

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
int tm=1100;

const char* PARAM_INPUT = "threshold_input";

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;


unsigned long previousMillis = 0;     
const long interval = 1500;    

// Relays
#define RelayPin1 15  //gpio

//WiFi Status LED
#define wifiLed    16  //gpio

int toggleState_1 = 1; //Define integer to remember the toggle state for relay 1



// Update these with values suitable for your network.

const char* ssid = ""; //WiFI Name
const char* password = ""; //WiFi Password
const char* mqttServer = "";
const char* mqttUserName = ""; // MQTT username
const char* mqttPwd = ""; // MQTT password
const char* clientID = "EspClient000023"; // client id
const char* availabilityTopic = "ESP_Watmeter/status/LWT";

#define sub1 "ESP_Watmeter_Switch"

#define sub2 "ESP_Watmeter_voltage"
#define sub3 "ESP_Watmeter_current"
#define sub4 "ESP_Watmeter_power"
#define sub5 "ESP_Watmeter_energy"
#define sub6 "ESP_Watmeter_frequency"
#define sub7 "ESP_Watmeter_pf"


#define pub1 "ESP_Watmeter_State"

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (80)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void reconnect() {
 while (!client.connected()) {
 if (client.connect(clientID, mqttUserName, mqttPwd, availabilityTopic, 0, true, "Offline")) {
 client.publish(availabilityTopic, "Online", true);
 Serial.println("MQTT connected");
      // ... and resubscribe
      client.subscribe(sub1);
     
     } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

}
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if (strstr(topic, sub1))
  
  {
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
    // Switch on the Relay if an 1 was received as first character
    if ((char)payload[0] == '0') {
      digitalWrite(RelayPin1, LOW);   
      
                                     
      toggleState_1 = 0;
      client.publish(pub1, "0");
    } else {
      digitalWrite(RelayPin1, HIGH);  
      
      toggleState_1 = 1;
      client.publish(pub1, "1");
    }    
  
  }


  else
  {
    Serial.println("unsubscribed topic");
  
  }

}



  void setup() {
                    
  Serial.begin(115200);
    
  Serial.println();
  pzem = PZEM004Tv30(pzemSWSerial);
  pinMode(RelayPin1, OUTPUT);
  
  
  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  
  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);

 setup_wifi();
   client.setServer(mqttServer, 1883);
   client.setCallback(callback);
 }

 
void loop() {
  if (!client.connected()) {
    digitalWrite(wifiLed, HIGH);
    reconnect();
  }
  else{
    digitalWrite(wifiLed, LOW);
    
  }
  client.loop();
  PzemSend();
  
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
  previousMillis = currentMillis;
 
  
  
   
   }
   
    Serial.print("Custom Address:");
    Serial.println(pzem.readAddress(), HEX);

    // Read the data from the sensor
    float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();
     
    // Check if the data is valid
    
     if(isnan(voltage)){
        Serial.println("Error reading voltage");
    } else if (isnan(current)) {
        Serial.println("Error reading current");
    } else if (isnan(power)) {
        Serial.println("Error reading power");
    } else if (isnan(energy)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else
    
      {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy);       Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency);    Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);

    } 
  
    Serial.println();
    delay(2000);
}
  void PzemSend(){
  
  float voltage = pzem.voltage();
    {
       
       client.publish(sub2, String(voltage).c_str(), true);
     
       tm = 1100;
      tm--;
        
     }

   float current = pzem.current();
   {
       
       client.publish(sub3, String(current).c_str(), true);
     
       tm = 1100;
       tm--;
     }
   float power = pzem.power();
   {
       
       client.publish(sub4, String(power).c_str(), true);
     
       tm = 1100;
       tm--;
        
     }
   float energy = pzem.energy();
   {
       
       client.publish(sub5, String(energy).c_str(), true);
     
       tm = 1100;
       tm--;
        
     }
   float frequency = pzem.frequency();
   {
       
       client.publish(sub6, String(frequency).c_str(), true);
     
       tm = 1100;
       tm--;
        
     }
   float pf = pzem.pf();
   {
       
       client.publish(sub7, String(pf).c_str(), true);
     
       tm = 1100;
       tm--;
        
     }
      
   }
 
    
   
  
  
  
