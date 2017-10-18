#include "PMS7003.h"
#include "Sys.h"
#include "ili9341.h"

#define PMS7003_SET_PIN   0x00
#define PMS7003_RESET_PIN 0x01

#define PM2P5_X_POS 0
#define PM2P5_Y_POS 180

typedef struct
{
    uint8_t precode[2];
    uint8_t frameLen[2];
    uint8_t standardData[6];
    uint8_t pm1p0[2];
    uint8_t pm2p5[2];
    uint8_t pm10[2];
    uint8_t other[12];
    uint8_t version;
    uint8_t errcode;
    uint8_t sum[2];
}PMS7003Data_t;

static uint8_t g_buff[sizeof(PMS7003Data_t)];
static bool g_gotFrame = false;

static uint16_t checkSum(const uint8_t *data, uint8_t dlen)
{
    uint8_t i;
    uint16_t sum = 0;

    for(i = 0; i < dlen; i++)
    {
        sum += data[i];
    }
    return sum;
}

static void pmDataUpdate(uint16_t pm2p5, uint16_t pm10)
{
    char text[24] = "";

    SysLog("pm2.5 = %d, pm10 = %d", pm2p5, pm10);
    //sprintf(text, "    %d  ", pm2p5);
    //Ili9341LCDDisplayString(PM2P5_X_POS, PM2P5_Y_POS, 24, 48, text, LCD_COLOR_GREEN);

    sprintf(text, "PM2.5:%03d", pm2p5);
    Ili9341LCDDisplayString(0, 100, 24, 48, text, LCD_COLOR_GRAY);
    sprintf(text, "PM10:%03d", pm10);
    Ili9341LCDDisplayString(0, 150, 24, 48, text, LCD_COLOR_GRAY);
}

static void dataRecv(uint8_t *data, uint16_t len)
{
    uint16_t i;
    uint16_t check;
    PMS7003Data_t *pmsData;
    static uint8_t buffcount = 0;

    if(g_gotFrame)
    {
        return;
    }

    for(i = 0; i < len; i++)
    {
        g_buff[buffcount++] = data[i];

        if(buffcount == 1)
        {
            if(data[i] != 0x42)
            {
                buffcount = 0;
            }
        }
        else if(buffcount == 2)
        {
            if(data[i] != 0x4d)
            {
                buffcount = 0;
            }
        }
        else if(buffcount == 32)
        {
            pmsData = (PMS7003Data_t *)g_buff;
            check = pmsData->sum[0];
            check = (check << 8) + pmsData->sum[1];
            if(check == checkSum(g_buff, 30))
            {
                g_gotFrame = true;
            }
            buffcount = 0;
        }
    }
}

static void frameHandle(void)
{
    PMS7003Data_t *pmsData;
    uint16_t pm2p5, pm10;
    
    if(g_gotFrame)
    {
        pmsData = (PMS7003Data_t *)g_buff;
        pm2p5 = (pmsData->pm2p5[0] << 8) + pmsData->pm2p5[1];
        pm10 = (pmsData->pm10[0] << 8) + pmsData->pm10[1];
        pmDataUpdate(pm2p5, pm10);
        g_gotFrame = false;
    }
}

void PMS7003Initailize(void)
{
    HalUartConfig_t config;
    config.baudrate = 9600;
    config.flowControl = 0;
    config.parity = 0;
    config.wordLength = USART_WordLength_8b;
    config.recvCb = dataRecv;
    HalUartConfig(HAL_UART_PORT_2, &config);

    HalGPIOConfig(PMS7003_SET_PIN, HAL_IO_OUTPUT);
    HalGPIOConfig(PMS7003_RESET_PIN, HAL_IO_OUTPUT);

    HalGPIOSetLevel(PMS7003_SET_PIN, 1);
    HalGPIOSetLevel(PMS7003_RESET_PIN, 1);
    
    //Ili9341LCDDisplayString(0, 100, 24, 48, "   PM2.5", LCD_COLOR_GREEN);

    pmDataUpdate(0, 0);
}

void PMS7003Poll(void)
{
    frameHandle();
}

