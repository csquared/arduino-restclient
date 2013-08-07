# RestClient for Arduino

HTTP Request library for Arduino and the Ethernet shield.

# Install

Clone (or download and unzip) the repository to `~/Documents/Arduino/libraries`
where `~/Documents/Arduino` is your sketchbook directory.

    > cd ~/Documents/Arduino
    > mkdir libraries
    > cd libraries
    > git clone https://github.com/csquared/arduino-restclient.git RestClient

# Usage

### Include

You need to have the `Ethernet` library already included.

```c++
#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"
```

### RestClient(host/ip, [port])

Constructor to create an RestClient object to make requests against.

Use domain name and default to port 80:
```c++
RestClient client = RestClient("arduino-http-lib-test.herokuapp.com");
```

Use a local IP and an explicit port:
```c++
RestClient client = RestClient("192.168.1.50",5000);
```

### dhcp()

Sets up `EthernetClient` with a mac address of `DEADBEEFFEED`

```c++
  client.dhcp()
```

Note: you can have multiple RestClient objects but only need to call
this once.

Note: if you have multiple Arduinos on the same network, you'll need
to give each one a different mac address.

### begin(byte mac[])

It just wraps the `EthernetClient` call to `begin` and DHCPs.
Use this if you need to explicitly set the mac address.

```c++
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  if (client.begin(mac) == 0) {
     Serial.println("Failed to configure Ethernet using DHCP");
  }
```

### Manual Ethernet Setup

You can skip the above methods and just configure the EthernetClient yourself:

```c++
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  //the IP address for the shield:
  byte ip[] = { 192, 168, 2, 11 };
  Ethernet.begin(mac,ip);
```

```c++
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  Ethernet.begin(mac);
```

This is especially useful for debugging network connection issues.

## RESTful methods

All methods return an HTTP status code or 0 if there was an error.

### `get(const char*)`
### `get(const char*, String * response)`

Start making requests!

```c++
int statusCode = client.get("/"));
```

Pass in a string by reference for the response:
```
String response = "";
int statusCode = client.get("/", &response);
```

### post(const char* path, const char* body)
### post(const char* path, String* response)
### post(const char* path, const char* body, String* response)

```
String response = "";
int statusCode = client.post("/", &response);
statusCode = client.post("/", "foo=bar");
response = "";
statusCode = client.post("/", "foo=bar", &response);
```

### put(const char* path, const char* body)
### put(const char* path, String* response)
### put(const char* path, const char* body, String* response)

```
String response = "";
int statusCode = client.put("/", &response);
statusCode = client.put("/", "foo=bar");
response = "";
statusCode = client.put("/", "foo=bar", &response);
```

### del(const char* path)
### put(const char* path, const char* body)
### put(const char* path, String* response)
### put(const char* path, const char* body, String* response)

```
String response = "";
int statusCode = client.del("/", &response);
```

## Full Example

I test every way of calling the library (against a public heroku app)[https://github.com/csquared/arduino-http-test].

This means you can cut and paste the following into Arduino IDE and you can cofirm you are up and running
with the library.


```c++

#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"

int test_delay = 1000; //so we don't spam the API
boolean describe_tests = true;

RestClient client = RestClient("arduino-http-lib-test.herokuapp.com");
//RestClient client  = RestClient("10.0.1.47",5000);


//Setup
void setup() {
  Serial.begin(9600);
  // Connect via DHCP
  Serial.println("dhcp");
  client.dhcp();

/*
  Can still fall back to manual config:

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  //the IP address for the shield:
  byte ip[] = { 192, 168, 2, 11 };
  Ethernet.begin(mac,ip);
*/

  Serial.println("Setup!");
}

void test_connect(int error){
   delay(test_delay);
   if(error == 1){
    Serial.println("TEST RESULT: fail (connect)");
   }else{
    Serial.println("TEST RESULT: ok (connect)");
   }
}

String response;
void test_response(){
  //Serial.println(response);
  if(response == "OK"){
   Serial.println("TEST RESULT: ok (response body)");
  }else{
   Serial.println("TEST RESULT: fail (response body = " + response + ")");
  }
  response = "";
}

void describe(char * description){
  if(describe_tests) Serial.println(description);
}

//reusable test variables
char* post_body = "POSTDATA";

void GET_tests(){
  describe("Test GET with path");
  test_connect(client.get("/get"));

  describe("Test GET with path and response");
  test_connect(client.get("/get", &response));
  test_response();

  describe("Test GET with path and header");
  client.setHeader("X-Test-Header: true");
  test_connect(client.get("/get-header"));

  describe("Test GET with path and header and response");
  client.setHeader("X-Test-Header: true");
  test_connect(client.get("/get-header", &response));
  test_response();

  describe("Test GET with 2 headers and response");
  client.setHeader("X-Test-Header1: one");
  client.setHeader("X-Test-Header2: two");
  test_connect(client.get("/get-headers", &response));
  test_response();
}

void POST_tests(){
    // POST TESTS
  describe("Test POST with path and body");
  test_connect(client.post("/data", post_body));

  describe("Test POST with path and body and response");
  test_connect(client.post("/data", post_body, &response));
  test_response();

  describe("Test POST with path and body and header");
  client.setHeader("X-Test-Header: true");
  test_connect(client.post("/data-header", post_body));

  describe("Test POST with path and body and header and response");
  client.setHeader("X-Test-Header: true");
  test_connect(client.post("/data-header", post_body, &response));
  test_response();

  describe("Test POST with 2 headers and response");
  client.setHeader("X-Test-Header1: one");
  client.setHeader("X-Test-Header2: two");
  test_connect(client.post("/data-headers", post_body, &response));
  test_response();
}

void PUT_tests(){
  describe("Test PUT with path and body");
  test_connect(client.put("/data", post_body));

  describe("Test PUT with path and body and response");
  test_connect(client.put("/data", post_body, &response));
  test_response();

  describe("Test PUT with path and body and header");
  client.setHeader("X-Test-Header: true");
  test_connect(client.put("/data-header", post_body));

  describe("Test PUT with path and body and header and response");
  client.setHeader("X-Test-Header: true");
  test_connect(client.put("/data-header", post_body, &response));
  test_response();

  describe("Test PUT with 2 headers and response");
  client.setHeader("X-Test-Header1: one");
  client.setHeader("X-Test-Header2: two");
  test_connect(client.put("/data-headers", post_body, &response));
  test_response();
}

void DELETE_tests(){
  describe("Test DELETE with path");
  test_connect(client.del("/data"));

  describe("Test DELETE with path and body");
  test_connect(client.del("/data", post_body));

  describe("Test DELETE with path and body and response");
  test_connect(client.del("/data", post_body, &response));
  test_response();

  describe("Test DELETE with path and body and header");
  client.setHeader("X-Test-Header: true");
  test_connect(client.del("/data-header", post_body));

  describe("Test DELETE with path and body and header and response");
  client.setHeader("X-Test-Header: true");
  test_connect(client.del("/data-header", post_body, &response));
  test_response();

  describe("Test DELETE with 2 headers and response");
  client.setHeader("X-Test-Header1: one");
  client.setHeader("X-Test-Header2: two");
  test_connect(client.del("/data-headers", post_body, &response));
  test_response();
}


// Run the tests!
void loop(){
  GET_tests();
  POST_tests();
  PUT_tests();
  DELETE_tests();
}

```

## Debug Mode

If you're having trouble, you can always open `RestClient.cpp` and throw at the top:

```c++
#define HTTP_DEBUG
```

Everything happening in the client will get printed to the Serial port.

# Thanks

[ricardochimal](https://github.com/ricardochimal) For all his c++ help.  Couldn't have done this without you!

[theycallmeswift](https://github.com/theycallmeswift) Helping incept and debug v1.0
