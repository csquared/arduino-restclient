# RestClient for Arduino

HTTP Request library for Arduino and the Ethernet shield.

# Install

Clone (or download and unzip) the repository to `~/Documents/Arduino/libraries`
where `~/Documents/Arduino` is your sketchbook directory.

    > cd ~/Documents/Arduino
    > mkdir libraries
    > cd libraries
    > git clone https://github.com/csquared/arduino-http.git HTTP

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
RestClient server = RestClient("arduino-http-lib-test.herokuapp.com");
```

Use a local IP and an explicit port:
```c++
RestClient server = RestClient("192.168.1.50",5000);
```

### RestClient::dhcp

Sets up `EthernetClient` with a mac address of `DEADBEEFFEED`

```c++
  server.dhcp()
```

Note: you can have multiple RestClient objects but only need to call
this once.

### RestClient::begin

It just wraps the `EthernetClient` call to `begin` and DHCPs.
Use this if you need to explicitly set the mac address.

```c++
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  if (server.begin(mac) == 0) {
     Serial.println("Failed to configure Ethernet using DHCP");
  }
```

### RestClient::get

Start making requests!

```c++
server.get("/"));
```

Pass in a string by reference for the response:
```
String response = "";
server.get("/", &response);
```

### RestClient::post
```
String response = "";
server.post("/", &response);
```

## Full Example

I test every way of calling the library (against a public heroku app)[https://github.com/csquared/arduino-http-test].

This means you can cut and paste the following into Arduino IDE and you can cofirm you are up and running
with the library.


```c++

#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"


}

```

# Thanks

[ricardochimal](https://github.com/ricardochimal) For all his c++ help.  Couldn't have done this without you!

[theycallmeswift](https://github.com/theycallmeswift) Helping incept and debug v1.0
