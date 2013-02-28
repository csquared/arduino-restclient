# HTTP for Arduino

HTTP Request library for Arduino and the Ethernet shield.

## Usage

Let's connect to a Heroku app.

### Set a base url

    HTTP heroku = HTTP("http://proxy.herokuapp.com");

### Start the Ethernet client with DHCP

    if (heroku.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
    }

### Make a GET request

    //0 for success, 1 for error

    char* headers[] = {"Host: myapp.herokuapp.com"}
    String response;
    int error = heroku.get("/status", headers, &response);
    Serial.println(response)
