#if (MCU_TYPE == 1)

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include "debug.h"

AsyncWebServer server(80);

void setupWebServer()
{

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    DEBUG_PRINTF("\nAn Error has occurred while mounting SPIFFS\n");
    return;
  }

  server.serveStatic("/css", SPIFFS, "/css/");
  server.serveStatic("/js", SPIFFS, "/js/");
  server.serveStatic("/images", SPIFFS, "/images/");
  
  // Serve index page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

   // Start server
  server.begin();
}

// Replaces placeholder with LED state value
String processor(const String& var){
 /* Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  */
}

#endif
