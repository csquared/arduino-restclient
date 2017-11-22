/* RestClient simple GET request
 *
 * by Chris Continanza (csquared)
 */

#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
EthernetClient ethclient;

RestClient client = RestClient("arduino-http-lib-test.herokuapp.com", ethclient);

//Setup
void setup() {
  Serial.begin(9600);
  // Connect via DHCP
  if(Ethernet.begin(mac)) {
    Serial.println("connected to network via DHCP");
    Serial.print("IP received:"); Serial.println(Ethernet.localIP());
  }
/*
  // Can still fall back to manual config:
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  //the IP address for the shield:
  byte ip[] = { 192, 168, 2, 11 };
  Ethernet.begin(mac,ip);
*/
  Serial.println("Setup!");
}

String response;
void loop(){
  response = "";
  int statusCode = client.get("/get", &response);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Response body from server: ");
  Serial.println(response);
  delay(1000);
}
