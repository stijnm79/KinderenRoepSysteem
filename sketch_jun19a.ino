#include <UIPEthernet.h> // Used for Ethernet

// **** ETHERNET SETTING ****
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };
IPAddress ip(192, 168, 1, 200);
EthernetServer server(80);

String HTTP_req;

const char html1[] PROGMEM = "<!DOCTYPE HTML><html><title>KRS</title></head><body><h1>Roep de kinderen</h1><form method='get'><input type='checkbox' id='Lotte' name='Kind1' value='Lotte'><label for='Lotte'> Lotte</label><br>";
const char html2[] PROGMEM = "<input type='checkbox' id='Dries' name='Kind2' value='Dries'><label for='Dries'> Dries</label><br><input type='checkbox' id='Sander' name='Kind3' value='Sander'><label for='Sander'> Sander</label><br>";
const char html3[] PROGMEM = "<input type='checkbox' id='Joren' name='Kind4' value='Joren'><label for='Joren'> Joren</label><br><br><input type='submit' value='Roep'></form></body></html>";

const char* const html[] PROGMEM = {html1, html2, html3};
char buffer[201];
void setup() {
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  //Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
  //Serial.println(HTTP_req);
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client)
  {
    Serial.println("-> New Connection");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //Serial.print(c);
        HTTP_req += c;
        //Serial.println(HTTP_req.indexOf("roep"));

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank)
        {
          Serial.println("http_req:");
          Serial.println(HTTP_req);
          Serial.print("Get /?Kind: ");
          Serial.println(HTTP_req.indexOf("Kind"));
          if (HTTP_req.indexOf("Kind") > -1) {
            Serial.println("->Roep gevonden");
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          for (int i = 0; i < 3; i++ ) {
            strcpy_P(buffer, (char*)pgm_read_word(&(html[i])));
            client.println( buffer );
          }

          // client.println("<html><head><title>test</title></head><body>test</body></html>");

          HTTP_req = "";
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }

    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("   Disconnected\n");
  }
}