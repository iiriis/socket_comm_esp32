
#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "Why ?";
const char * password = "B003";

AsyncUDP udp;

typedef struct {
    uint32_t data;
    char name[30];
    float temperature;
} myData;


// WiFi credentials
#define WIFI_SSID "ESP32-AP"
#define WIFI_PASSWORD "password"

AsyncUDPPacket *packet_send;
bool isConnected = false;

myData m2 = {0};

void setup()
{  
    Serial.begin(115200);

    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    
    if(udp.listen(1234)) {

        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.softAPIP());
        
        udp.onPacket([](AsyncUDPPacket packet) {
            
            myData *m1;
            m1 = (myData *)packet.data();

            printf("Temp = %f, data = %lu, name = %s\n", m1->temperature, m1->data, m1->name);

            // myData m2 = {0};
            
            packet_send = new AsyncUDPPacket(packet);

            isConnected = true;

            strcpy(m2.name, "Avijit");
            m2.temperature = 3.6987;
            m2.data = millis();

            packet.write((const uint8_t*)&m2, sizeof(myData));
        });
    }
}

void loop()
{

    delay(1);
  if(isConnected){
    strcpy(m2.name, "inloop");
    m2.temperature = 3.6987;
    m2.data = millis();

    // Serial.println("Sending...");
    packet_send->write((const uint8_t*)&m2, sizeof(myData));
    }
    

    // Serial.printf("Broadcasted\n");
}


