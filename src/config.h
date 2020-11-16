#ifndef __CONF_H__
#define __CONF_H__

//Software versie
  #define PRODUCT_NAME "HWtestTool"
  #define MAJOR_VERSION 0
  #define MINOR_VERSION 0
  #define PRODUCTNR "00000"

  #define BOARD_SCREEN 0

  #define BOARD_LED1_PORT 1
  #define BOARD_LED2_PORT 1
  #define BOARD_LED1_PIN (1<<23)
  #define BOARD_LED2_PIN (1<<22)

  #define BOARD_SERVICE_PORT 2
  #define BOARD_SERVICE_PIN (1<<13)

  #define DEBUG_UART DEBUG_UART0


  #define PACKET_BUFFER_SIZE 32
#endif /* ___CONF_H__ */
