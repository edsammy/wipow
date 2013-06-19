#include <SPI.h>
#include <WiFly.h>
#include <NotifyrClient.h>

String NOTIFYR_KEY = "OkmzlILxg7jqBfQCgbZlDplNNENWwekZnhjhGy7XtUs";

NotifyrClient notifyr;

void setup() {
    Serial.begin(9600);
    
    Serial.println("Initializing WiFi...");
    SPI.begin();
    SpiSerial.begin();
    
    Serial.print("Connecting to Notifyr (");
    Serial.print(NOTIFYR_KEY);
    Serial.println(")...");
    
    if (notifyr.connect(NOTIFYR_KEY, "my-channel")) {
        Serial.println("Connected!");
        notifyr.bind(handleEvent);
    } else {
        Serial.println("Connection failed...");
        for (;;) ; // do nothing forever
    }
}

void loop() {
    notifyr.listen();
}

void handleEvent(String data) {
    Serial.println("Event!!!");
    Serial.println(data);
}
