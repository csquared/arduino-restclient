#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class HTTP {

  public:
    HTTP(const char* host);

    void dhcp();
    int begin(byte*);

    int request(String method, String path, char** headers, int num_headers, String body, bool, String* response);

    // GET path
    int get(const char*);
    int get(String);

    // GET path and response
    int get(const char*, String*);
    int get(String, String*);

    // GET path and headers
    int get(const char*, char**, int);
    int get(String, char**, int);

    // GET path and headers and response
    int get(const char*, char**, int, String*);
    int get(String, char**, int, String*);

    // POST path and body
    int post(const char* path, String body);
    int post(String path, String body);

    // POST path and body and response
    int post(const char* path, String body, String*);
    int post(String path, String body, String*);

    // POST path and body and headers
    int post(const char* path, String body, char **, int);
    int post(String path, String body, char **, int);

    // POST path and body and headers and response
    int post(const char* path, String body, char **, int, String*);
    int post(String path, String body, char **, int, String*);

  private:
    EthernetClient http_client;
    void readResponse(String*);
    const char* host;
    String null_body;
};
