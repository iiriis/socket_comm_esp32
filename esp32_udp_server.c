#include "WiFi.h"
#include "AsyncUDP.h"

const char * ssid = "Ganesh PG 2.5g";
const char * password = "master123";

AsyncUDP udp;

typedef struct {
    uint32_t data;
    char name[30];
    float temperature;
} myData;


// WiFi credentials
#define WIFI_SSID "ESP32-AP"
#define WIFI_PASSWORD "password"

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

            myData m2 = {0};

            strcpy(m2.name, "Avijit");
            m2.temperature = 3.6987;
            m2.data = 10;

            packet.write((const uint8_t*)&m2, sizeof(myData));
        });
    }
}

void loop()
{
    delay(1000);
    // Serial.printf("Broadcasted\n");
}