// Include the ESP8266 WiFi library. (Works a lot like the
// Arduino WiFi library.)
#include <ESP8266WiFi.h>
#include <Phant.h>

const char WiFiSSID[] = "";
const char WiFiPSK[] = "";

//const char WiFiSSID[] = "iPhone (2)";
//const char WiFiPSK[] = "bxmwfkg1twmb1";

//const char WiFiSSID[] = "**virus";
//const char WiFiPSK[] = "goodgirL0";

const int LED_PIN = 5; // Thing's onboard, green LED
const char phantHost[] = "data.sparkfun.com";
const unsigned long postRate = 4000;

void setup() {
  initHardware();
  connectWiFi();
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  postToLog("Starting!");
  String line = getLatestFromStream();
  postToLog("Response: " + line);
  postToLog("Finishing!");
  delay(postRate);
}

void connectWiFi() {
  byte ledStatus = LOW;
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFiSSID, WiFiPSK);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, ledStatus);
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH; //toggle it
    delay(1000);
  }
}

void initHardware() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);
}

String getLatestFromStream() {
  char streamPublicKey[] = "XGNZrmp7pKCZEKnNwlal";
  char streamPrivateKey[] = "1JAzk1MaM5tPlMpedZ8Z";

  Phant phant(phantHost, streamPublicKey, streamPrivateKey);
  String response = makePhantRequest(phantGetFixed(phantHost, streamPublicKey));

  return "Response: " + response;
}

void postToLog(String contents) {
  char logPublicKey[] = "QGXZgMOq7qI0lbEW9p2E";
  char logPrivateKey[] = "JqAYN1Mk9kCwaMvbN5qv";

  Phant phant(phantHost, logPublicKey, logPrivateKey);
  phant.add("status", contents);
  String response = makePhantRequest(phant.post());
}

String makePhantRequest(String request) {
  digitalWrite(LED_PIN, HIGH);

  WiFiClient client;
  if (!client.connect(phantHost, 80)) {
    return "failure to connect to host: " + String(phantHost);
  }
  client.print(request);
  
  String line;
  int index = 0;
  while((!client.available()) && (index <1000)) {
      delay(10);
      index++;
  }
  while(client.available()) {
      line +=client.readStringUntil('\r');
  }

  //turn the LED off.
  digitalWrite(LED_PIN, LOW);
  return line;
}

String phantGetFixed(String _host, String _pub) {

  String result = "GET /output/" + _pub + ".csv\nHTTP/1.1\n";
  result += "Host: " + _host + "\n";
  result += "Connection: close\n";

  return result;

}

