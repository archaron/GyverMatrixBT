#if (MCU_TYPE == 1)


#include <ESP8266WiFi.h>
#include <time.h>
#include "debug.h"

int8_t TIME_ZONE = 3; 
#ifdef USE_SUMMER_TIME_DST
uint8_t DST = 1;
#else
uint8_t DST = 0;
#endif

time_t now;
time_t nowish = 1510592825;



void wifi_init() {

  WiFi.mode(WIFI_STA);
  DEBUG_PRINTF("\n\n\tWiFi Setup -- \n"  ); 
  
  WiFi.hostname( (String("GyverMatrix") +  WiFi.macAddress()).c_str());
    
  if (!digitalRead(BUTT_UP) && !digitalRead(BUTT_DOWN)) {
    DEBUG_PRINTF("\tSmartConfig started\n");
  
    WiFi.disconnect(true);
    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    led_blink(5,300);

    WiFi.beginSmartConfig();


    int  smartCount = 0;
    bool smartDone  = false;
   
    while (smartCount++ < 60) { // Пробуем получить настройки 1 минуту
      if (WiFi.smartConfigDone()) {
        #if ( DEBUG == 1) 
          DEBUG_PRINTF("\nSmartConfig done.\n");
          DEBUG_PRINTF( "\n\tREADING NVR\n");
          DEBUG_PRINTF( "\tSSID = %s\n"   , WiFi.SSID().c_str() );
          DEBUG_PRINTF( "\tPSK  = %s\n\n" , WiFi.psk().c_str()  );
        #endif      
        smartDone = true;
        break;
      }
      led_blink(1,10);
      delay(1000);

      DEBUG_PRINTF(">");
    }
    
    if (!smartDone) {
      DEBUG_PRINTF("\tSmart config failed");
      digitalWrite(LED_BUILTIN, LOW);
      delay(2000);
    }
  }

  if (WiFi.SSID()!=NULL) {
    DEBUG_PRINTF("\tWaiting for WiFi ");
    int connectCount = 0;
    while (connectCount++ < 100) {
      if (WiFi.status() == WL_CONNECTED) {
        led_blink(10,10);
        DEBUG_PRINTF("\tWiFi Connected.\n\tIP Address: %s", WiFi.localIP().toString().c_str() );    
        break;
      }
      delay(500);
      DEBUG_PRINTF(".");
      led_blink(2,100);
    }

    if (WiFi.status() != WL_CONNECTED) {
      DEBUG_PRINTF("\tWiFi connect FAILED.\n");
      digitalWrite(LED_BUILTIN, LOW);
    } else {
      led_blink(3,50);  
    }
    
    NTPConnect();
    
  } else {
    DEBUG_PRINTF("\n\tNo stored credentials, skipping wifi init.\n");
  }
}

void led_blink(int count, int ms) {
  digitalWrite(LED_BUILTIN, HIGH);
  for (int i=0; i <= count; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(ms);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(ms);
  }
}



void NTPConnect(void)
{

  DateTime n = rtc.now();
  DEBUG_PRINTF("\n\tRTC Time: %02d:%02d:%02d %02d.%02d.%02d\n",  n.hour(), n.minute(), n.second(), n.day(), n.month(), n.year());
  
  DEBUG_PRINTF("\n\tSetting time using SNTP ");
  configTime(TIME_ZONE * 3600, DST * 3600, "pool.ntp.org", "time.nist.gov");

  int  timeCount = 0;
  bool   ntpDone = false;
  
  now = time(nullptr);

  while (timeCount++ < 15) {
    if (now > nowish) {
      DEBUG_PRINTF("\tdone!\n");
      ntpDone = true;
      break;
    }
    delay(500);
    led_blink(1, 20);
    DEBUG_PRINTF(".");
    now = time(nullptr);
  }

  if (ntpDone) {
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    DEBUG_PRINTF("\tCurrent time: %s\n", asctime(&timeinfo));

    #if (USE_CLOCK) 
      rtc.adjust(DateTime(now));
    #endif
  }
}
#else
  void wifi_init()   {
    
  }
#endif
