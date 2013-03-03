#include "HTTP.h"

#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string) (Serial.println(string))
#endif

#ifndef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string)
#endif

HTTP::HTTP(const char* _host){
  host = _host;
  null_body = NULL;
}

int HTTP::begin(byte mac[]){
  return Ethernet.begin(mac);
}

// GET path
int HTTP::get(const char* path){
  return get(String(path));
}

int HTTP::get(String path){
  String response = "";
  char* headers[] = {};
  return request(String("GET"), path, headers, 0, null_body, false, &response);
}

//GET path with response
int HTTP::get(const char* path, String* response){
  return get(String(path), response);
}

int HTTP::get(String path, String* response){
  char* headers[] = {};
  return request(String("GET"), path, headers, 0, null_body, true, response);
}

//GET path with headers
int HTTP::get(const char* path, char** headers, int num_headers){
  return get(String(path), headers, num_headers);
}

int HTTP::get(String path, char** headers, int num_headers){
  String response = "";
  return request(String("GET"), path, headers, num_headers, null_body, false, &response);
}

/*
int HTTP::get(String path, String* [1]){
  String response = "";
  return request(String("GET"), path, headers, 1, null_body, response);
}
*/

//GET path with headers and response
int HTTP::get(const char* path, char** headers, int num_headers, String* response){
  return get(String(path), headers, num_headers, response);
}

int HTTP::get(String path, char** headers, int num_headers, String* response){
  return request(String("GET"), path, headers, 1, null_body, true, response);
}

/*
int HTTP::get(String path, String* [1], String* response){
  return request(String("GET"), path, headers, 1, body, response);
}
*/

// POST path and body
int HTTP::post(const char* path, String body){
  post(String(path), body);
}

int HTTP::post(String path, String body){
  String response = "";
  char* headers[] = {};
  return request(String("POST"), path, headers, 0, body, false, &response);
}

// POST path and body and response
int HTTP::post(const char* path, String body, String* response){
  post(String(path), body, response);
}

int HTTP::post(String path, String body, String* response){
  char* headers[] = {};
  return request(String("POST"), path, headers, 0, body, true, response);
}

// POST path and body and headers
int HTTP::post(const char* path, String body, char** headers, int num_headers){
  post(String(path), body, headers, num_headers);
}

int HTTP::post(String path, String body, char** headers, int num_headers){
  String response = "";
  return request(String("POST"), path, headers, num_headers, body, false, &response);
}

// POST path and body and headers and response
int HTTP::post(const char* path, String body, char** headers, int num_headers, String* response){
  post(String(path), body, headers, num_headers, response);
}

int HTTP::post(String path, String body, char** headers, int num_headers, String* response){
  return request(String("POST"), path, headers, num_headers, body, true, response);
}


// The mother- generic request method.
//
int HTTP::request(String method, String path, char** headers, int num_headers,
                        String body, bool read_response, String* response){

  HTTP_DEBUG_PRINT("HTTP: connect");

  if(http_client.connect(host, 80)){
    HTTP_DEBUG_PRINT("HTTP: connected");
    // Make a HTTP request:
    String request_line = String(method) + String(" ") + path + String(" HTTP/1.1");
    http_client.println(request_line);
    HTTP_DEBUG_PRINT(String("REQUEST LINE: ") + request_line);
    for(int i=0; i<num_headers; i++){
      http_client.println(headers[i]);
      HTTP_DEBUG_PRINT(String("HEADER: ") + String(headers[i]));
    }

    http_client.println("Host: " + String(host));
    HTTP_DEBUG_PRINT(String("HEADER: Host: ") + String(host));
    ///
    http_client.println("Connection: close");
    HTTP_DEBUG_PRINT("HEADER: Connection: close");

    if(body != NULL){
      http_client.println(String("Content-Length: ") + body.length());
      HTTP_DEBUG_PRINT(String("HEADER: Content-Length: ") + body.length());
      http_client.println("Content-Type: application/x-www-form-urlencoded");
      HTTP_DEBUG_PRINT("HEADER: Content-Type: application/x-www-form-urlencoded");
    }

    http_client.println();

    if(body != NULL){
      HTTP_DEBUG_PRINT("HTTP: post body ");
      http_client.print(body);
      HTTP_DEBUG_PRINT(body);
      http_client.println();
    }
    //make sure you write all those bytes.
    http_client.flush();
    //aaaaaand give it some time
    delay(10);

    if(read_response){
      HTTP_DEBUG_PRINT("HTTP: call readResponse");
      readResponse(response);
      HTTP_DEBUG_PRINT("HTTP: return readResponse");
    }

    //cleanup
    HTTP_DEBUG_PRINT("HTTP: stop client");
    http_client.stop();
    HTTP_DEBUG_PRINT("HTTP: client stopped");

    return 0;
  }else{
    HTTP_DEBUG_PRINT("HTTP Connection failed");
    return 1;
  }
}

void HTTP::readResponse(String* response) {

  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean httpBody = false;

  while (http_client.connected()) {
    if (http_client.available()) {

      char c = http_client.read();

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
