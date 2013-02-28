#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class HTTP {

  public:
    HTTP(const char* base_url);

    int begin(byte*);
    int get(const char*, char**, String* );
    /*
    char get(char resource);
    char post(char resource);
    char put(char resource);
    */
  private:
    EthernetClient http_client;
    String readResponse();
    const char* base_url;
};
