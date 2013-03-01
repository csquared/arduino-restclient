# HTTP for Arduino

HTTP Request library for Arduino and the Ethernet shield.

# Install

Clone (or download and unzip) the repository to `~/Documents/Arduino/libraries`
where `~/Documents/Arduino` is your sketchbook directory.

    > cd ~/Documents/Arduino
    > mkdir libraries
    > cd libraries
    > git clone https://github.com/csquared/arduino-http.git HTTP

# Usage

Let's connect to a Heroku app.

## Require It

```c++
#include <Ethernet.h>
#include <SPI.h>
#include "HTTP.h"
```

## Set a base url

    HTTP heroku = HTTP("http://proxy.herokuapp.com");

## Start the Ethernet client with DHCP

    if (heroku.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
    }

## GET requests

    char* headers[] = {"Host: myapp.herokuapp.com"};
    String response;
    int error = heroku.get("/status", headers, 1, &response);
    Serial.println(response)


## POST requests

    char* headers[] = {"Host: myapp.herokuapp.com"};
    String body = "hello, world";
    String response;
    int error = heroku.get("/status", headers, 1, body, &response);
    Serial.println(response)

# Thanks

@ricardochimal For all his c++ help.  Couldn't have done this without you!
