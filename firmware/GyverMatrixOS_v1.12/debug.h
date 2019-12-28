
#if (DEBUG==1)
  // #ifdef DEBUG_ESP_PORT
  #define DEBUG_PRINTF(fmt, ...) Serial.printf( fmt, ##__VA_ARGS__ )
  // #endif
#endif

#ifndef DEBUG_PRINTF
#define DEBUG_PRINTF(...) do { (void)0; } while (0)
#endif