#include "A7129.h"
#include "Ticker.h"

Ticker t1;
void Timer1isr()
{
    RF_A7129_timer++;
    if(RF_A7129_timer >= RF_A7129_TIMEOUT)
    {
        RF_A7129_TimeoutFlag=1;
    }
}

uint8_t       RF_A7129_timer;
uint8_t       RF_A7129_TimeoutFlag;
uint16_t     RF_A7129_RxCnt;
uint32_t    RF_A7129_Err_ByteCnt;
uint32_t     RF_A7129_Err_BitCnt;
uint16_t     RF_A7129_TimerCnt0;
uint8_t       *RF_A7129_Uartptr;
uint8_t       RF_A7129_UartSendCnt;
uint8_t       RF_A7129_CmdBuf[11];
uint8_t      RF_A7129_tmpbuf[64]; 
/*********************************************************************
** Err_State
*********************************************************************/
static void Err_State(void)
{
    //ERR display
    //Error Proc...
    //...
    while(1);
}
/************************************************************************
**  A7129_WriteReg
************************************************************************/
void A7129_WriteReg(uint8_t address, uint16_t dataWord)
{
    uint8_t i;

    RF_A7129_SCS_L();
    address |= RF_A7129_CMD_Reg_W;
    for(i=0; i<8; i++)
    {
        if(address & 0x80)
        {
            RF_A7129_SDIO_H();
        }
        else
        {
            RF_A7129_SDIO_L();
        }

        RF_A7129_SCK_H();
        delayMicroseconds(1);
        RF_A7129_SCK_L();
        address<<=1;
    }
    delayMicroseconds(1);

    //send data word
    for(i=0; i<16; i++)
    {
        if(dataWord & 0x8000)
        {
            RF_A7129_SDIO_H();
        }
        else
        {
            RF_A7129_SDIO_L();    
        }
        RF_A7129_SCK_H();   
        delayMicroseconds(1);
        RF_A7129_SCK_L();   
        dataWord<<=1;
    }
    RF_A7129_SCS_H();
}
/************************************************************************
**  A7129_WritePageA
************************************************************************/
void A7129_WritePageA(uint8_t address, uint16_t dataWord)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 12) | RF_A7129_Config[RF_A7129_CRYSTAL_REG]);
    A7129_WriteReg(RF_A7129_CRYSTAL_REG, tmp);
    A7129_WriteReg(RF_A7129_PAGEA_REG, dataWord);
}
/************************************************************************
**  A7129_WritePageB
************************************************************************/
void A7129_WritePageB(uint8_t address, uint16_t dataWord)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 7) | RF_A7129_Config[RF_A7129_CRYSTAL_REG]);
    A7129_WriteReg(RF_A7129_CRYSTAL_REG, tmp);
    A7129_WriteReg(RF_A7129_PAGEB_REG, dataWord);
}
/************************************************************************
**  A7129_ReadReg
************************************************************************/
uint16_t A7129_ReadReg(uint8_t address)
{
    uint8_t i;
    uint16_t tmp;

    RF_A7129_SCS_L();  
    address |= RF_A7129_CMD_Reg_R;
    for(i=0; i<8; i++)
    {
        if(address & 0x80)
        {
            RF_A7129_SDIO_H();
        }
        else
        {
            RF_A7129_SDIO_L();
        }

		delayMicroseconds(1); 
        RF_A7129_SCK_H();
		delayMicroseconds(1); 
        RF_A7129_SCK_L();

        address<<=1;
    }
		delayMicroseconds(1); 
    
    //read data code
    
   RF_A7129_SDIO_H();		//SDIO pull high
   pinMode(RF_A7129_SDIO,INPUT);
    for(i=0; i<16; i++)
    {
        if(digitalRead(RF_A7129_SDIO))
        {
            tmp = (tmp << 1) | 0x01;
        }
        else
        {
            tmp = tmp << 1;

        }
        RF_A7129_SCK_H();
        delayMicroseconds(1);
        RF_A7129_SCK_L();
    }
    RF_A7129_SCS_H();
    return tmp;
}



/*********************************************************************
** A7129_Config
*********************************************************************/
void A7129_Config(void)
{
	uint8_t i;
	uint16_t tmp;

    for(i=0; i<8; i++)
        A7129_WriteReg(i, RF_A7129_Config[i]);

	for(i=10; i<16; i++)
        A7129_WriteReg(i, RF_A7129_Config[i]);

    for(i=0; i<16; i++)
        A7129_WritePageA(i, RF_A7129_Config_PageA[i]);

	for(i=0; i<5; i++)
        A7129_WritePageB(i, RF_A7129_Config_PageA[i]);

	//for check        
	tmp = A7129_ReadReg(RF_A7129_SYSTEMCLOCK_REG);
	if(tmp != RF_A7129_Config[RF_A7129_SYSTEMCLOCK_REG])
	{
		Err_State();	
	}
}

/*********************************************************************
** Strobe Command
*********************************************************************/
static void StrobeCMD(uint8_t cmd)
{
    uint8_t i;

    RF_A7129_SCS_L();    
    for(i=0; i<8; i++)
    {
        if(cmd & 0x80)
        {
            RF_A7129_SDIO_H();
        }
        else
        {
            RF_A7129_SDIO_L();
        }

        delayMicroseconds(1);//微妙级延时
        RF_A7129_SCK_H();
        delayMicroseconds(1);//微妙级延时
        RF_A7129_SCK_L();
        cmd<<=1;
    }
    RF_A7129_SCS_H();    
}
/************************************************************************
**  ByteRead
************************************************************************/
uint8_t ByteRead(void)
{
	uint8_t i,tmp;

    //read data code
    RF_A7129_SDIO_H();	//SDIO pull high
    pinMode(RF_A7129_SDIO,INPUT);
    for(i=0; i<8; i++)
    {
        if(digitalRead(RF_A7129_SDIO))
        {
            tmp = (tmp << 1) | 0x01;
        }
        else
        {
            tmp = tmp << 1;
        }
        RF_A7129_SCK_H();
        delayMicroseconds(1);
        RF_A7129_SCK_L();
    }
    return tmp;
}

/************************************************************************
**  ByteSend
************************************************************************/
void ByteSend(uint8_t src)
{
    uint8_t i;

    for(i=0; i<8; i++)
    {
        if(src & 0x80)
        {
            RF_A7129_SDIO_H();      
        }
        else
        {
            RF_A7129_SDIO_L();   
        }

		delayMicroseconds(1);
        RF_A7129_SCK_H();
		delayMicroseconds(1);
        RF_A7129_SCK_L();
        src<<=1;
    }
}

/************************************************************************
**  WriteID
************************************************************************/
void A7129_WriteID(void)
{
	uint8_t i;
	uint8_t d1, d2, d3, d4;

    RF_A7129_SCS_L();
	ByteSend(RF_A7129_CMD_ID_W);
	for(i=0; i<4; i++)
    {
		ByteSend(RF_A7129_ID_Tab[i]);

    }
    RF_A7129_SCS_H();    
    RF_A7129_SCS_L();    
	ByteSend(RF_A7129_CMD_ID_R);
	d1=ByteRead();
	d2=ByteRead();
	d3=ByteRead();
	d4=ByteRead();
    RF_A7129_SCS_H();    
	
    if((d1!=RF_A7129_ID_Tab[0]) || (d2!=RF_A7129_ID_Tab[1]) || (d3!=RF_A7129_ID_Tab[2]) || (d4!=RF_A7129_ID_Tab[3]))
    {
        Err_State();
    }
}
/************************************************************************
**  A7129_ReadPageA
************************************************************************/
uint16_t A7129_ReadPageA(uint8_t address)
{
    uint16_t tmp;

    tmp = address;
    tmp = ((tmp << 12) | RF_A7129_Config[RF_A7129_CRYSTAL_REG]);
    A7129_WriteReg(RF_A7129_CRYSTAL_REG, tmp);
    tmp = A7129_ReadReg(RF_A7129_PAGEA_REG);
    return tmp;
}

/*********************************************************************
** A7129_Cal
*********************************************************************/
void A7129_Cal(void)
{
    uint8_t fb, fcd, fbcf;	//IF Filter
	uint8_t vb,vbcf;			//VCO Current
	uint8_t vcb, vccf;		//VCO Band
	uint16_t tmp;

    //IF calibration procedure @STB state
	A7129_WriteReg(RF_A7129_MODE_REG, RF_A7129_Config[RF_A7129_MODE_REG] | 0x0802);			//IF Filter & VCO Current Calibration
    do{
        tmp = A7129_ReadReg(RF_A7129_MODE_REG);
    }while(tmp & 0x0802);
	
    //for check(IF Filter)
    tmp = A7129_ReadReg(RF_A7129_CALIBRATION_REG);
    fb = tmp & 0x0F;
	fcd = (tmp>>11) & 0x1F;
    fbcf = (tmp>>4) & 0x01;
    if(fbcf)
    {
        Err_State();
    }

	//for check(VCO Current)
    tmp = A7129_ReadPageA(RF_A7129_VCB_PAGEA);
	vcb = tmp & 0x0F;
	vccf = (tmp>>4) & 0x01;
	if(vccf)
	{
        Err_State();
    }
    
    
    //RSSI Calibration procedure @STB state
	A7129_WriteReg(RF_A7129_ADC_REG, 0x4C00);									//set ADC average=64
    A7129_WritePageA(RF_A7129_WOR2_PAGEA, 0xF800);								//set RSSC_D=40us and RS_DLY=80us
	A7129_WritePageA(RF_A7129_TX1_PAGEA, RF_A7129_Config_PageA[RF_A7129_TX1_PAGEA] | 0xE000);	//set RC_DLY=1.5ms
    A7129_WriteReg(RF_A7129_MODE_REG, RF_A7129_Config[RF_A7129_MODE_REG] | 0x1000);			//RSSI Calibration
    do{
        tmp = A7129_ReadReg(RF_A7129_MODE_REG);
    }while(tmp & 0x1000);
	A7129_WriteReg(RF_A7129_ADC_REG, RF_A7129_Config[RF_A7129_ADC_REG]);
    A7129_WritePageA(RF_A7129_WOR2_PAGEA, RF_A7129_Config_PageA[RF_A7129_WOR2_PAGEA]);
	A7129_WritePageA(RF_A7129_TX1_PAGEA, RF_A7129_Config_PageA[RF_A7129_TX1_PAGEA]);


    //VCO calibration procedure @STB state
	A7129_WriteReg(RF_A7129_PLL1_REG, RF_A7129_Config[RF_A7129_PLL1_REG]);
	A7129_WriteReg(RF_A7129_PLL2_REG, RF_A7129_Config[RF_A7129_PLL2_REG]);
	A7129_WriteReg(RF_A7129_MODE_REG, RF_A7129_Config[RF_A7129_MODE_REG] | 0x0004);		//VCO Band Calibration
	do{
		tmp = A7129_ReadReg(RF_A7129_MODE_REG);
	}while(tmp & 0x0004);
	
	//for check(VCO Band)
	tmp = A7129_ReadReg(RF_A7129_CALIBRATION_REG);
	vb = (tmp >>5) & 0x07;
	vbcf = (tmp >>8) & 0x01;
	if(vbcf)
	{
		Err_State();
	}
}
/*********************************************************************
** A7129_WriteFIFO
*********************************************************************/
void A7129_WriteFIFO(void)
{
	uint8_t i;

	StrobeCMD(RF_A7129_CMD_TFR);		//TX FIFO address pointer reset
    RF_A7129_SCS_L();
	ByteSend(RF_A7129_CMD_FIFO_W);	//TX FIFO write command
	for(i=0; i <64; i++)
		ByteSend(RF_A7129_PN9_Tab[i]);
    RF_A7129_SCS_H();
}
/*********************************************************************
** RxPacket
*********************************************************************/
void RxPacket(void)
{
    uint8_t i;
    uint8_t recv;
    uint8_t tmp;

	RF_A7129_RxCnt++;

	StrobeCMD(RF_A7129_CMD_RFR);		//RX FIFO address pointer reset
    RF_A7129_SCS_L();
	ByteSend(RF_A7129_CMD_FIFO_R);	//RX FIFO read command
	for(i=0; i <64; i++)
	{
		RF_A7129_tmpbuf[i] = ByteRead();
	}
    RF_A7129_SCS_H();

	for(i=0; i<64; i++)
	{
		recv = RF_A7129_tmpbuf[i];
		tmp = recv ^ RF_A7129_PN9_Tab[i];
		if(tmp!=0)
		{
			RF_A7129_Err_ByteCnt++;
			RF_A7129_Err_BitCnt += (RF_A7129_BitCount_Tab[tmp>>4] + RF_A7129_BitCount_Tab[tmp & 0x0F]);
		}
    }
}

static void InitRF(void)
{
    //initial pin
    RF_A7129_SCS_H();
    RF_A7129_SCK_L();
    RF_A7129_SDIO_H();
    RF_A7129_CKO_H();
    RF_A7129_GIO1_H();
    RF_A7129_GIO2_H();

    StrobeCMD(RF_A7129_CMD_RF_RST);	//reset A7129 chip
    A7129_Config();			//config A7129 chip
    delay(1);			//for crystal stabilized
    A7129_WriteID();		//write ID code
    A7129_Cal();			//IF and VCO calibration
}

RF_A7129::RF_A7129()
{
    if(RF_A7129_Master)
    {
        t1.attach_ms(1,Timer1isr);
    }
}

void RF_A7129::RF_A7129_Send()
{
    InitRF();

    A7129_WriteFIFO(); 	//write data to TX FIFO
    StrobeCMD(RF_A7129_CMD_TX);
    delayMicroseconds(10);
    pinMode(RF_A7129_GIO2,INPUT);
    while(digitalRead(RF_A7129_GIO2)); 		//wait transmit completed
    StrobeCMD(RF_A7129_CMD_RX);
    delayMicroseconds(10);

    RF_A7129_timer=0;
    RF_A7129_TimeoutFlag=0;
    while((digitalRead(RF_A7129_GIO2)==1)&&(RF_A7129_TimeoutFlag==0)); 	//wait receive completed
    if(RF_A7129_TimeoutFlag)
    {
        StrobeCMD(RF_A7129_CMD_STBY);
    }
    else
    {
        RxPacket();
        delay(50);
    }



}

void RF_A7129::RF_A7129_Receive()
{
    InitRF();

    RF_A7129_RxCnt = 0;
    RF_A7129_Err_ByteCnt = 0;
    RF_A7129_Err_BitCnt = 0;

    StrobeCMD(RF_A7129_CMD_RX);
    delayMicroseconds(10);
    pinMode(RF_A7129_GIO2,INPUT);
    while(digitalRead(RF_A7129_GIO2)); 		//wait receive completed
    RxPacket();

    A7129_WriteFIFO(); 	//write data to TX FIFO
    StrobeCMD(RF_A7129_CMD_TX);
    delayMicroseconds(10);
    pinMode(RF_A7129_GIO2,INPUT);
    while(digitalRead(RF_A7129_GIO2)); 		//wait transmit completed

    delay(40);


}
