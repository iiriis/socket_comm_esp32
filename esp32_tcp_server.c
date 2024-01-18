
#include <WiFi.h>
#include <AsyncTCP.h>


AsyncServer server(80);


typedef struct {
    uint64_t data;
    char name[30];
    float temperature;
} myData;

// WiFi credentials
#define WIFI_SSID "ESP32-AP"
#define WIFI_PASSWORD "password"

char buf[20];
int i=0;
myData m2 = {0};

AsyncClient* connectedClient = NULL;
bool isConnected = false;

void setup() {
  Serial.begin(115200);

  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

  server.onClient([](void* arg, AsyncClient* client) {
    Serial.println("New client connected");

    isConnected = true;
    connectedClient = client;

    client->onData([](void* arg, AsyncClient* client, void* data, size_t len) {
      Serial.write((uint8_t*)data, len);
      client->write((const char *)"Hello from ESP32!", 17);
    });

    client->onDisconnect([](void* arg, AsyncClient* client) {
      Serial.println("Client disconnected");

      isConnected = false;
      connectedClient = NULL;
    });
  }, NULL);

  server.begin();

  Serial.print("TCP Listening on IP: ");
  Serial.println(WiFi.softAPIP());


  strcpy(m2.name, "Avijit");
  m2.temperature = 3.6987;
}



void loop() {

if(digitalRead(0)) {


  m2.data = millis();

  if(isConnected)
  {
    connectedClient->write((const char*)&m2, sizeof(myData));
    Serial.printf("Temp = %f, data = %llu, name = %s\n", m2.temperature, m2.data, m2.name);
  }
}

  delay(1);
}

