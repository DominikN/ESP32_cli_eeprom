#include <WiFi.h>
#include <Husarnet.h>
#include <SimpleCLI.h>
#include <Preferences.h>

const int CLI_PORT = 8001;
const int LED_PIN = 16;

HusarnetServer server(CLI_PORT);

Preferences preferences;

SimpleCLI cli;

Command conwifi;
Command husarnet_join;
Command setled;

void taskInternetCLI( void * parameter );

void conwifiCallback(cmd* c);
void husarnet_joinCallback(cmd* c);
void setledCallback(cmd* c);

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Read Wi-Fi and Husarnet credentials from NVS memory

  preferences.begin("my-app", false);
  String ssid = preferences.getString("ssid");
  String pass = preferences.getString("pass");
  String hostname = preferences.getString("hostname");
  String joincode = preferences.getString("joincode");
  preferences.end();


  // Configure a command line interface (CLI)

  conwifi = cli.addCmd("conwifi", conwifiCallback);
  conwifi.addArg("ssid");
  conwifi.addArg("pass");

  husarnet_join = cli.addCmd("husarnet_join", husarnet_joinCallback);
  husarnet_join.addPosArg("joincode");
  husarnet_join.addPosArg("hostname");

  setled = cli.addCmd("setled", setledCallback);
  setled.addPosArg("state");


  // Connect to Wi-Fi

  Serial.println("Connecting to Wi-Fi: ");
  if (ssid != "") {
    Serial.print("SSID:");
    Serial.println(ssid);
    Serial.print("PASS:");
    Serial.println(pass);

    WiFi.begin(ssid.c_str(), pass.c_str());
    for (int j = 0; j < 10; j++) {
      if (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (j == 9) {
          Serial.printf("\r\nwrong Wi-Fi credentials. Going to commandline:\r\n");
          Serial.println("ESP32 commandline");
          Serial.print("# ");
          return;
        }
      } else {
        Serial.println("done");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        break;
      }
    }
  } else {
    Serial.println("no credentials in memory");
  }


  // Connect to Husarnet network

  Serial.println("Connecting to Husarnet: ");
  if (hostname != "") {
    Serial.print("joincode:");
    Serial.println(joincode);
    Serial.print("hostname:");
    Serial.println(hostname);

    Husarnet.selfHostedSetup("default");
    Husarnet.join(joincode.c_str(), hostname.c_str());
    Husarnet.start();

    xTaskCreate(
      taskInternetCLI,  /* Task function. */
      "taskWifi",       /* String with name of task. */
      20000,            /* Stack size in bytes. */
      NULL,             /* Parameter passed as input of the task */
      2,                /* Priority of the task. */
      NULL);            /* Task handle. */

  } else {
    Serial.println("no credentials in memory");
  }

  Serial.println("ESP32 commandline");
  Serial.print("# ");
}

String input;

void loop() {

  // Check if user typed something into the serial monitor
  if (Serial.available()) {
    char c = Serial.read();
    input += c;
    if (c == '\r') {
      cli.parse(input);
      input = "";
    } else {
      Serial.print(c);
    }
  }

  // Check for parsing errors
  if (cli.errored()) {
    // Get error out of queue
    CommandError cmdError = cli.getError();

    // Print the error
    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    // Print correct command structure
    if (cmdError.hasCommand()) {
      Serial.print("Did you mean \"");
      Serial.print(cmdError.getCommand().toString());
      Serial.println("\"?");
    }
    Serial.print("\r\n# ");
  }
}


void conwifiCallback(cmd* c) {
  Command cmd(c);

  String ssid = cmd.getArg("ssid").getValue();
  String pass = cmd.getArg("pass").getValue();

  preferences.begin("my-app", false);
  preferences.putString("ssid", ssid);
  preferences.putString("pass", pass);
  preferences.end();

  Serial.println("\r\nWi-Fi credentials saved. Reset to connect.");
}

void husarnet_joinCallback(cmd* c) {
  Command cmd(c);

  String hostname = cmd.getArg("hostname").getValue();
  String joincode = cmd.getArg("joincode").getValue();

  preferences.begin("my-app", false);
  preferences.putString("hostname", hostname);
  preferences.putString("joincode", joincode);
  preferences.end();

  Serial.println("\r\nHusarnet credentials saved. Reset to connect.");
}

void setledCallback(cmd* c) {
  Command cmd(c);

  String state = cmd.getArg("state").getValue();

  switch (state.toInt()) {
    case 0:
      digitalWrite(LED_PIN, LOW);
      break;
    case 1:
      digitalWrite(LED_PIN, HIGH);
      break;
  }

  Serial.println("\r\nHusarnet credentials saved. Reset to connect.");
}

void taskInternetCLI( void * parameter )
{
  server.begin();

  while (1) {
    HusarnetClient client = server.available();
    if (client) {
      Serial.println("New Client.");
      String currentLine = "";
      Serial.printf("connected: %d\r\n", client.connected());
      while (client.connected()) {

        if (client.available()) {
          char c = client.read();
          currentLine += c;

          if (c == '\n') {
            cli.parse(currentLine);
            currentLine = "";
            client.println("ok");
          }
        }
      }
      
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    } else {
      delay(200);
    }
  }
}
