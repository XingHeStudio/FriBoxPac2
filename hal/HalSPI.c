#include "HalSPI.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"

void HalSPIDmaWrite(uint8_t *buff, uint16_t len)
{
    DMA1_Channel5->CMAR = (uint32_t)buff;
    DMA1_Channel5->CNDTR = len;
    //SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    //SPI_Cmd(SPI2, ENABLE);
    //while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    while(DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET);

    DMA_Cmd(DMA1_Channel5, DISABLE);
}

void HalSPIInitialize(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    /* < Configure SD_SPI pins: SCK MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*!< SD_SPI Config */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //谷豕??SPI米ㄓ?辰?辰?????辰米?那y?Y?㏒那?:SPI谷豕???a???????辰豕???1∟  SPI_Direction_1Line_Tx
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//谷豕??SPI1∟℅¯?㏒那?:谷豕???a?¯SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//谷豕??SPI米?那y?Y∩車D?:SPI﹞⊿?赤?車那?8?????芍11
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//????芍?∩?DD那㊣?車米??豕足?:那㊣?車D邦????
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//那y?Y2???車迆米迆?t??那㊣?車??
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSD?o?車谷車2?t㏒“NSS1邦??㏒??1那?豕赤?t㏒“那1車?SSI??㏒?1邦角赤:?迆2?NSSD?o?車DSSI??????
    SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//?“辰?2“足??那?∟﹞??米米??米:2“足??那?∟﹞??米?米?a16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//???“那y?Y∩?那?∩車MSB???1那?LSB???a那?:那y?Y∩?那?∩車MSB???a那?
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC?米????米??角??那?
    SPI_Init(SPI1, &SPI_InitStructure);

    SPI_Cmd(SPI1, ENABLE); 
    
    
    /* < Configure SD_SPI pins: SCK MOSI*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    /*!< SD_SPI Config */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //谷豕??SPI米ㄓ?辰?辰?????辰米?那y?Y?㏒那?:SPI谷豕???a???????辰豕???1∟  SPI_Direction_1Line_Tx
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//谷豕??SPI1∟℅¯?㏒那?:谷豕???a?¯SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//谷豕??SPI米?那y?Y∩車D?:SPI﹞⊿?赤?車那?8?????芍11
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//????芍?∩?DD那㊣?車米??豕足?:那㊣?車D邦????
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//那y?Y2???車迆米迆?t??那㊣?車??
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSD?o?車谷車2?t㏒“NSS1邦??㏒??1那?豕赤?t㏒“那1車?SSI??㏒?1邦角赤:?迆2?NSSD?o?車DSSI??????
    SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//?“辰?2“足??那?∟﹞??米米??米:2“足??那?∟﹞??米?米?a16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//???“那y?Y∩?那?∩車MSB???1那?LSB???a那?:那y?Y∩?那?∩車MSB???a那?
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC?米????米??角??那?
    SPI_Init(SPI2, &SPI_InitStructure);

    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SPI2->DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)NULL;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_Cmd(SPI2, ENABLE); 

}


void HalSPIPoll(void)
{
    
}

unsigned char HalSPIReadWrite(HalSPIPort_t port, unsigned char data)
{
    SPI_TypeDef *spiType;
    
    if(port == HAL_SPI_PORT_1)
    {
        spiType = SPI1;
    }
    else
    {
        spiType = SPI2;
    }
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(spiType, SPI_I2S_FLAG_TXE) == RESET);
	/* Send byte through the SPIx peripheral */
	SPI_I2S_SendData(spiType, data);
	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(spiType, SPI_I2S_FLAG_RXNE) == RESET);
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(spiType);
}

