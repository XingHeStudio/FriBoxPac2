#ifndef HAL_CONFIG_H
#define HAL_CONFIG_H

#define HAL_STATUS_LED_PIN 0x23  //pc3
#define HAL_STATUS_LED_ENABLE_LEVEL  0
#define HAL_STATUS_LED_DISABLE_LEVEL 1

//wifi module configs
#define HAL_WIFI_RESET_PIN   0x10
#define HAL_WIFI_RELOAD_PIN  0x11
#define HAL_WIFI_READY_PIN   0x25
#define HAL_WIFI_DEFAULT_BAUDRATE 115200

#define KB(x) ((x)*1024)
/*flash分区
* 0 ~ 5k     :boot
* 5k ~ 125k  :app
* 125k ~ 245k:ota
* 245k ~ 256k:args
*/
#define HAL_FLASH_SIZE (KB(256))  //256KB 120k(ota), 5k(boot)

#define HAL_FLASH_BASE_ADDR  0x8000000
#define HAL_BOOT_FLASH_ADDR  (HAL_FLASH_BASE_ADDR + 0)
#define HAL_APP_FLASH_ADDR   (HAL_FLASH_BASE_ADDR + KB(5))
#define HAL_OTA_FLASH_ADDR   (HAL_FLASH_BASE_ADDR + KB(125))
#define HAL_ARGS_FLASH_ADDR  (HAL_FLASH_BASE_ADDR + KB(245))

#define HAL_DEVICE_ID_FLASH_ADDR (HAL_ARGS_FLASH_ADDR + 0) //设备ID和pwd

#endif

