#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class HTTP {

  public:
    HTTP(const char* base_url);

    int begin(byte*);
    int get(const char*, char**, int, String* );
    int post(const char* path, char** headers, int, String, String* response);
    /*
    char put(char resource);
    char delete(char resource);
    */
  private:
    EthernetClient http_client;
    String readResponse();
    const char* base_url;
};
