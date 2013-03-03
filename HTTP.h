#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class HTTP {

  public:
    HTTP(const char* host);

    int begin(byte*);

    int request(String method, String path, char** headers, int num_headers, String body, bool, String* response);

    // path
    int get(const char*);
    int get(String);

    // path and response
    int get(const char*, String*);
    int get(String, String*);

    // path and headers
    int get(const char*, char**, int);
    int get(String, char**, int);
    //int get(String, String* [1]);

    // path and headers and response
    int get(const char*, char**, int, String*);
    int get(String, char**, int, String*);

    /*
    int post(const char* path, String body);
    int post(const char* path, String body, String* response);
    int post(const char* path, char** headers, int, String, String* response);
    char put(char resource);
    char delete(char resource);
    */
  private:
    EthernetClient http_client;
    void readResponse(String*);
    const char* host;
    String null_body;
};
