# RestClient for Arduino ESP8266 WiFi modules

HTTP Request library for Arduino and the ESP8266 WiFi SOC modules

This library now supports SSL!  To use with SSL, you need to include the SHA1 fingerprint of the certificate of the site you are connecting to.  You can get this by using a desktop browser and inspecting the SSL cert used at the site.  Please note: this is FRAGILE, if the site updates their SSL, your code will break.  But, there is not enough memory on the ESP8266 to store all the rool certs, so this is a working method.  Se the example below.

This library is derived almost entirely from the great work done here: https://github.com/csquared/arduino-restclient

# Install

Clone (or download and unzip) the repository to `~/Documents/Arduino/libraries`
where `~/Documents/Arduino` is your sketchbook directory.

    > cd ~/Documents/Arduino
    > mkdir libraries
    > cd libraries
    > git clone https://github.com/dakaz/esp8266-restclient.git RestClient

# Usage

### Include

You need to have the `ESP8266` board support already included.

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

Use a local IP, an explicit port to an SSL site and (must include the 1 to turn on SSL):
```c++
RestClient client = RestClient("www.kudoso.com",443, 1);
```

Use a local IP, an explicit port to an SSL site and verify the certificate with its fingerprint:
```c++
RestClient client = RestClient("www.kudoso.com",443, "EE 16 77 79 55 58 92 46 FB 18 40 99 2E 17 7E AB 32 0A 4A 88");
```

### dhcp()

Sets up `EthernetClient` with a mac address of `DEADBEEFFEED`. Returns `true` or `false` to indicate if setting up DHCP
was successful or not

## RESTful methods

All methods return an HTTP status code or 0 if there was an error.

### `get(const char* path)`
### `get(const char* path, String* response)`

Start making requests!

```c++
int statusCode = client.get("/"));
```

Pass in a string *by reference* for the response:
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
### del(const char* path, const char* body)
### del(const char* path, String* response)
### del(const char* path, const char* body, String* response)

```
String response = "";
int statusCode = client.del("/", &response);
```

## Full Example

I test every way of calling the library (against a public heroku app)[https://github.com/csquared/arduino-http-test].

You can find the file in File->Examples->RestClient->full_test_suite

## Debug Mode

If you're having trouble, you can always open `RestClient.cpp` and throw at the top:

```c++
#define HTTP_DEBUG
```

Everything happening in the client will get printed to the Serial port.

# Thanks

[ricardochimal](https://github.com/ricardochimal) For all his c++ help.  Couldn't have done this without you!

[theycallmeswift](https://github.com/theycallmeswift) Helping incept and debug v1.0
