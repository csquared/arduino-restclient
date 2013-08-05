#include "RestClient.h"

#define HTTP_DEBUG

#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string)
#endif

RestClient::RestClient(const char* _host){
  RestClient(_host, 80);
}

RestClient::RestClient(const char* _host, int _port){
  host = _host;
  port = _port;
  num_headers = 0;
  contentTypeSet = false;
}

void RestClient::dhcp(){
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
  if (begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
}

int RestClient::begin(byte mac[]){
  return Ethernet.begin(mac);
}

// GET path
int RestClient::get(const char* path){
  return request("GET", path, NULL, NULL);
}

//GET path with response
int RestClient::get(const char* path, String* response){
  return request("GET", path, NULL, response);
}

// POST path and body
int RestClient::post(const char* path, const char* body){
  return request("POST", path, body, NULL);
}

// POST path and body with response
int RestClient::post(const char* path, const char* body, String* response){
  return request("POST", path, body, response);
}

// PUT path and body
int RestClient::put(const char* path, const char* body){
  return request("PUT", path, body, NULL);
}

// PUT path and body with response
int RestClient::put(const char* path, const char* body, String* response){
  return request("PUT", path, body, response);
}

// DELETE path
int RestClient::del(const char* path){
  return request("DELETE", path, NULL, NULL);
}

// DELETE path and response
int RestClient::del(const char* path, String* response){
  return request("DELETE", path, NULL, response);
}

// DELETE path and body
int RestClient::del(const char* path, const char* body ){
  return request("DELETE", path, body, NULL);
}

// DELETE path and body with response
int RestClient::del(const char* path, const char* body, String* response){
  return request("DELETE", path, body, response);
}

void RestClient::write(const char* c){
  HTTP_DEBUG_PRINT(c);
  client.print(c);
}

void RestClient::setHeader(const char* header){
  headers[num_headers] = header;
  num_headers++;
}

// The mother- generic request method.
//
int RestClient::request(const char* method, const char* path,
                  const char* body, String* response){

  HTTP_DEBUG_PRINT("HTTP: connect\n");

  if(client.connect(host, port)){
    HTTP_DEBUG_PRINT("HTTP: connected\n");
    HTTP_DEBUG_PRINT("REQUEST: \n");
    // Make a HTTP request line:
    write(method);
    write(" ");
    write(path);
    write(" HTTP/1.1\r\n");
    for(int i=0; i<num_headers; i++){
      write(headers[i]);
      write("\r\n");
    }
    write("Host: ");
    write(host);
    write("\r\n");
    write("Connection: close\r\n");

    if(body != NULL){
      char contentLength[30];
      sprintf(contentLength, "Content-Length: %d\r\n", strlen(body));
      write(contentLength);

      if(!contentTypeSet){
        write("Content-Type: application/x-www-form-urlencoded\r\n");
      }
    }

    write("\r\n");

    if(body != NULL){
      write(body);
      write("\r\n");
    }
    //make sure you write all those bytes.
    client.flush();
    //aaaaaand give it some time
    delay(10);

    if(response != NULL){
      HTTP_DEBUG_PRINT("HTTP: call readResponse\n");
      readResponse(response);
      HTTP_DEBUG_PRINT("HTTP: return readResponse\n");
    }

    //cleanup
    num_headers = 0;
    HTTP_DEBUG_PRINT("HTTP: stop client\n");
    client.stop();
    HTTP_DEBUG_PRINT("HTTP: client stopped\n");

    return 0;
  }else{
    HTTP_DEBUG_PRINT("HTTP Connection failed\n");
    return 1;
  }
}

void RestClient::readResponse(String* response) {

  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean httpBody = false;

  while (client.connected()) {
    if (client.available()) {

      char c = client.read();

      if(httpBody){
        response->concat(c);
      }
      if (c == '\n' && httpBody){
        return;
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

  return;
}
