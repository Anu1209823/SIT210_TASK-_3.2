#include <WiFiNINA.h>

// Replace with your Wi-Fi credentials
char ssid[] = "Redmi 10";      // Your Wi-Fi network name (SSID)
char pass[] = "12345678";      // Your Wi-Fi network password

WiFiClient client;

// The host name for the IFTTT server
char HOST_NAME[] = "maker.ifttt.com";

// The path to your specific IFTTT event and key
String PATH_NAME = "/trigger/Sun-soaked/with/key/iUWC0GduVWblSjNxYXECdYYHk8m5c2ZZbK3s47xSX-w"; // Update your EVENT-NAME and API Key

const int ldrPin = A0;       // Analog pin for the LDR
int threshold = 500;         // Adjust this threshold based on ambient light conditions

void setup() {
  // Initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial);

  // Connect to web server on port 80:
  while (!client.connect(HOST_NAME, 80)) {
    // If not connected, print an error message
    Serial.println("Connection failed");
  }
  // If connected, print a success message
  Serial.println("Connected");
}

void loop() {
  int ldrValue = analogRead(ldrPin); // Read LDR sensor value
  
  // Convert ldrValue to sunlight intensity (you might need to calibrate this)
  // Compare with the threshold to determine if sunlight is hitting the terrarium
  bool isSunlight = (ldrValue > threshold);

  if (isSunlight) {
    // Make an HTTP request for sunlight start:
    client.println("GET " + PATH_NAME + "?value1=Sunlight%20Start HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
  } else {
    // Make an HTTP request for sunlight stop:
    client.println("GET " + PATH_NAME + "?value1=Sunlight%20Stop HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();
  }

  while (client.connected()) {
    if (client.available()) {
      // Read an incoming byte from the server and print it to the serial monitor
      char c = client.read();
      Serial.print(c);
    }
  }

  // The server's disconnected, stop the client:
  client.stop();
  Serial.println();
  Serial.println("Disconnected");

  delay(10000); // Delay for a certain interval before checking again
}
