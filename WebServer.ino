#include <WiFiNINA.h>

extern WiFiClient client;
extern const char* webPageBody[];
extern int webPageBodyLength;

// webpage
const char* webPageBody[] = 
{
  "<!DOCTYPE html>",
  "<html>",
  "<head>",
  "<meta charset=\"UTF-8\">",
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">",
  "<meta http-equiv=\"refresh\" content=\"2\">",
  "<title>Sensor Values Dashboard</title>",
  "<style>",
  ":root {",
    "--color-high: #00FF00;",
    "--color-medium: #FFFF00;",
    "--color-low: #FF0000;",
  "}",
  ".circle {",
    "width: 100px;",
    "height: 100px;",
    "border-radius: 50%;",
    "background-color: var(--color-medium); /* Default color */",
    "display: inline-flex;",
    "align-items: center;",
    "justify-content: center;",
    "margin-right: 20px;",
    "font-size: 24px;",
    "color: #fff; /* Text color */",
    "transition: background-color 0.3s ease; /* Smooth color transition */",
  "}",
  ".circle[data-value='high'] {",
    "background-color: var(--color-high);",
  "}",
  ".circle[data-value='medium'] {",
    "background-color: var(--color-medium);",
  "}",
  ".circle[data-value='low'] {",
    "background-color: var(--color-low);",
  "}",
  "</style>",
  "</head>",
  "<body>",
  "<h1>Sensor Values Dashboard</h1>",
  "<div class='circle' data-value='high'>100</div>",
  "<div class='circle' data-value='medium'>75</div>",
  "<div class='circle' data-value='low'>50</div>",
  //create the buttons
  "<p>Click <a href=\"/H\">here</a> turn the LED on</p><br>",
  "<p>Click <a href=\"/L\">here</a> turn the LED off</p><br><br>",
  
};
int webPageBodyLength = sizeof(webPageBody) / sizeof(webPageBody[0]);

// script inside webpage
const char* webPageScript[] =
{
  "<script>",
  // Example of updating sensor values dynamically
  // Replace these values with actual sensor readings or update them via JavaScript as needed
  "document.querySelectorAll('.circle').forEach(circle => {",
    "const value = parseInt(circle.textContent);",
    "if (value >= 80) {",
      "circle.dataset.value = 'high';",
    "} else if (value >= 50) {",
      "circle.dataset.value = 'medium';",
    "} else {",
      "circle.dataset.value = 'low';",
    "}",
  "});",
  "</script>",
  "</body>",
  "</html>",
};
int webPageScriptLength = sizeof(webPageScript) / sizeof(webPageScript[0]);

void printWEB() 
{
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {

            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
           
            printWebPage(client, webPageBody, webPageBodyLength);

            printWebPage(client, webPageScript, webPageScriptLength);
            printCircle(client, readTemperatureFahrenheit(), TEMP);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /H")) {
        WiFiDrv::analogWrite(25, random(0,255));      
        }
        if (currentLine.endsWith("GET /L")) {
        WiFiDrv::analogWrite(25, 0);     
        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

void printWebPage(WiFiClient& client, const char *print[], int len)
{
  for (int i = 0; i < len; i++)
  {
    client.print(print[i]);
  }
}