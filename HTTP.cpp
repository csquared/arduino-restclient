#include "HTTP.h"

HTTP::HTTP(const char* _host){
  host = _host;
}

int HTTP::begin(byte mac[]){
  return Ethernet.begin(mac);
}

/*
int HTTP::post(const char* path, String body){
  String response = "";
  return this->post(path, body, {}, 0, &response);
}

int HTTP::post(const char* path, String body, String* response){
  return this->post(path, body, {}, 0, response);
}

int HTTP::post(const char* path, char** headers, int num_headers,
                String body, String* response){

  Serial.println("connect HTTP POST");
  Serial.println(path);

  if(http_client.connect(host, 80)){
    Serial.println("connected");
    // Make a HTTP request:
    String request_line = String("POST ") + String(path) + String(" HTTP/1.1");
    http_client.println(request_line);
    Serial.println(request_line);

    for(int i=0; i < num_headers; i++){
      http_client.println(headers[i]);
   //   Serial.println(headers[i]);
    }
    http_client.println(String("Content-Length: ") + body.length());
    Serial.println(String("Content-Length: ") + body.length());
    http_client.println("Content-Type: application/x-www-form-urlencoded");
    http_client.println("Connection: close");
    http_client.println();
    http_client.print(body);
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

int HTTP::get(const char* path, char** headers, int num_headers, String* response){
  return this->request(String("GET"), path, headers, num_headers, response);
}
*/

int HTTP::get(const char* path){
  return get(String(path));
}

int HTTP::get(String path){
  String response = "";
  char* headers[] = {};
  String body = NULL;

  return request(String("GET"), path, headers, 0, body, &response);
}

int HTTP::get(const char* path, String* response){
  return get(String(path), response);
}

int HTTP::get(String path, String* response){
  char* headers[] = {};
  String body = NULL;

  return request(String("GET"), path, headers, 0, body, response);
}


int HTTP::request(String method, String path, char** headers, int num_headers, String body, String* response){

  Serial.print("Connect HTTP");
  Serial.println();

  if(http_client.connect(host, 80)){
    Serial.println("connected");
    // Make a HTTP request:
    String request_line = String(method) + String(" ") + path + String(" HTTP/1.1");
    http_client.println(request_line);
    //Serial.println(request_line);
    for(int i=0; i<num_headers; i++){
      http_client.println(headers[i]);
   //   Serial.println(headers[i]);
    }

    http_client.println("Host: " + String(host));

    if(body != NULL){
      http_client.println(String("Content-Length: ") + body.length());
      Serial.println(String("Content-Length: ") + body.length());
      http_client.println("Content-Type: application/x-www-form-urlencoded");
      http_client.println("Connection: close");
    }

    http_client.println();

    if(body != NULL){
      http_client.print(body);
      http_client.println();
    }
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


/*
int HTTP::get(const char* path, char** headers,
                int num_headers, String* response){

  Serial.println("connect HTTP GET");
  Serial.println(path);

  if(http_client.connect(host, 80)){
    Serial.println("connected");
    // Make a HTTP request:
    String request_line = String("GET ") + String(path) + String(" HTTP/1.1");
    http_client.println(request_line);
    //Serial.println(request_line);
    for(int i=0; i<num_headers; i++){
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
*/

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
