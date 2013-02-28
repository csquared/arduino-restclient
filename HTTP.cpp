#include "HTTP.h"

HTTP::HTTP(const char* url){
  base_url = url;
}

int HTTP::begin(byte mac[]){
  return Ethernet.begin(mac);
}


int HTTP::get(const char* path, char** headers, String* response){

  Serial.println("connect HTTP GET");
  Serial.println(path);

  if(http_client.connect(base_url, 80)){
    Serial.println("connected");
    // Make a HTTP request:
    String request_line = String("GET ") + String(path) + String(" HTTP/1.0");
    http_client.println(request_line);
    //Serial.println(request_line);
    int length = sizeof(headers) / sizeof(char*);
    //Serial.println(length);
    for(int i=0; i<=length; i++){
      http_client.println(headers[i]);
   //   Serial.println(headers[i]);
    }
    http_client.println();

    //make sure you write all those bytes.
    http_client.flush();
    //aaaaaand give it some time
    delay(10);

    Serial.println("read HTTP");
    response->concat(this->readResponse());

    //cleanup
    http_client.stop();

    return 0;
  }else{
    Serial.println("HTTP Connection failed");
    return 1;
  }

}

String HTTP::readResponse() {

  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean httpBody = false;

  //string for fetching data from address
  String httpRequest = "";

  while (http_client.connected()) {
    if (http_client.available()) {
      char c = http_client.read();

      if(httpBody){
        httpRequest.concat(c);
      }
      if (c == '\n' && httpBody){
        return httpRequest;
      }
      if (c == '\n' && currentLineIsBlank) {
        httpBody = true;
      }
      if (c == '\n') {
        // you're starting a new lineu
        currentLineIsBlank = true;
      }
      else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
  }

  return httpRequest;
}
