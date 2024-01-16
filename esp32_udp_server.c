#include "WiFi.h"
#include "AsyncUDP.h"

// AP WiFi credentials
#define WIFI_SSID "ESP32-AP"
#define WIFI_PASSWORD "password"
#define SERVER_PORT 1234
// Async UDP object
AsyncUDP udp;

// structure to store the data fields that gets transmitted
typedef struct {
    uint32_t data;
    char name[30];
    float temperature;
} myData;

// structure to store the client packet
typedef struct {
  // AsyncUDPPacket object to store the client object
  AsyncUDPPacket* client_packet;
  bool isConnected = false;
}client_t;

// create an empty myData instance
myData m2 = {0};
// create a new client instance
client_t newClient;

void setup()
{  
    // enable serial communication
    Serial.begin(115200);

    // start the wifi in AP mode
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
    
    // open UDP server on port SERVER_PORT
    if(udp.listen(SERVER_PORT)) {
        // print AP IP
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.softAPIP());
        
        // activate asynchronous callback for the UDB on new data received from client
        udp.onPacket([](AsyncUDPPacket packet) {

            // store a copy of the client incoming packet
            newClient.client_packet = new AsyncUDPPacket(packet);
            // set the connected flag to true
            newClient.isConnected = true;
            // send the client an acknowledgement
            packet.printf("Hello Intruder !\n");
        });
    }
}

void loop()
{

  // small delay between transmitting
  delay(1);

  //check if client is connected
  if(newClient.isConnected){

    //fill the transmitting packet structure with dummy data
    strcpy(m2.name, "iiriis");
    m2.temperature = 3.6987;
    m2.data = millis();

    //unicast the structure to the client
    newClient.client_packet->write((const uint8_t*)&m2, sizeof(myData));
    }
    
}
