#pragma once
#include <stdint.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
#define RF_A7129_Master 0
const uint8_t  RF_A7129_BitCount_Tab[16]={0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
const uint8_t  RF_A7129_ID_Tab[8]={0x34,0x75,0xC5,0x8C,0xC7,0x33,0x45,0xE7};   //ID code
const uint8_t  RF_A7129_PN9_Tab[]=
{   0xFF,0x83,0xDF,0x17,0x32,0x09,0x4E,0xD1,
    0xE7,0xCD,0x8A,0x91,0xC6,0xD5,0xC4,0xC4,
    0x40,0x21,0x18,0x4E,0x55,0x86,0xF4,0xDC,
    0x8A,0x15,0xA7,0xEC,0x92,0xDF,0x93,0x53,
    0x30,0x18,0xCA,0x34,0xBF,0xA2,0xC7,0x59,
    0x67,0x8F,0xBA,0x0D,0x6D,0xD8,0x2D,0x7D,
    0x54,0x0A,0x57,0x97,0x70,0x39,0xD2,0x7A,
    0xEA,0x24,0x33,0x85,0xED,0x9A,0x1D,0xE0
};	// This table are 64bytes PN9 pseudo random code.


/*********************************************************************
**  I/O Declaration
*********************************************************************/
#define RF_A7129_SCS 		14 		//SPI SCS
#define RF_A7129_SCK 		33 		//SPI SCK
#define RF_A7129_SDIO 		32 		//SPI SDIO
#define RF_A7129_CKO 		36 		//CKO
#define RF_A7129_GIO1 		34 		//GIO1
#define RF_A7129_GIO2 		39 		//GIO2

#define RF_A7129_SCS_H()             pinMode(RF_A7129_SCS,OUTPUT);\
                                     digitalWrite(RF_A7129_SCS,HIGH);

#define RF_A7129_SCS_L()             pinMode(RF_A7129_SCS,OUTPUT);\
                                     digitalWrite(RF_A7129_SCS,LOW);

#define RF_A7129_SCK_H()             pinMode(RF_A7129_SCK,OUTPUT);\
                                     digitalWrite(RF_A7129_SCK,HIGH);

#define RF_A7129_SCK_L()             pinMode(RF_A7129_SCK,OUTPUT);\
                                     digitalWrite(RF_A7129_SCK,LOW);

#define RF_A7129_SDIO_H()             pinMode(RF_A7129_SDIO,OUTPUT);\
                                     digitalWrite(RF_A7129_SDIO,HIGH);

#define RF_A7129_SDIO_L()             pinMode(RF_A7129_SDIO,OUTPUT);\
                                     digitalWrite(RF_A7129_SDIO,LOW);

#define RF_A7129_CKO_H()             pinMode(RF_A7129_CKO,OUTPUT);\
                                     digitalWrite(RF_A7129_CKO,HIGH);

#define RF_A7129_CKO_L()             pinMode(RF_A7129_CKO,OUTPUT);\
                                     digitalWrite(RF_A7129_CKO,LOW);

#define RF_A7129_GIO1_H()             pinMode(RF_A7129_GIO1,OUTPUT);\
                                     digitalWrite(RF_A7129_GIO1,HIGH);

#define RF_A7129_GIO1_L()             pinMode(RF_A7129_GIO1,OUTPUT);\
                                     digitalWrite(RF_A7129_GIO1,LOW);

#define RF_A7129_GIO2_H()             pinMode(RF_A7129_GIO2,OUTPUT);\
                                     digitalWrite(RF_A7129_GIO2,HIGH);

#define RF_A7129_GIO2_L()             pinMode(RF_A7129_GIO2,OUTPUT);\
                                     digitalWrite(RF_A7129_GIO2,LOW);
                               
/*********************************************************************
**  Constant Declaration
*********************************************************************/
#define RF_A7129_TIMEOUT     50			//50ms
#define RF_A7129_t0hrel      1000		//1ms

/*********************************************************************
**  Global Variable Declaration
*********************************************************uint32_t************/
extern uint8_t       RF_A7129_timer;
extern uint8_t       RF_A7129_TimeoutFlag;
extern uint16_t     RF_A7129_RxCnt;
extern uint32_t    RF_A7129_Err_ByteCnt;
extern uint32_t     RF_A7129_Err_BitCnt;
extern uint16_t     RF_A7129_TimerCnt0;
extern uint8_t       *RF_A7129_Uartptr;
extern uint8_t       RF_A7129_UartSendCnt;
extern uint8_t       RF_A7129_CmdBuf[11];
extern uint8_t      RF_A7129_tmpbuf[64];                


const uint16_t  RF_A7129_Config[]=		//433MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x1221,		//SYSTEM CLOCK register,
	0x0A21,		//PLL1 register,
	0xDA05,		//PLL2 register,	433.301MHz
	0x0000,		//PLL3 register,
	0x0A20,		//PLL4 register,
	0x0024,		//PLL5 register,
	0x0000,		//PLL6 register,
	0x0001,		//CRYSTAL register,
	0x0000,		//PAGEA,
	0x0000,		//PAGEB,
	0x18D4,		//RX1 register, 	IFBW=100KHz, ETH=1	
	0x7009,		//RX2 register, 	by preamble
	0x4400,		//ADC register,	   	
	0x0800,		//PIN CONTROL register,		Use Strobe CMD
	0x4845,		//CALIBRATION register,
	0x20C0		//MODE CONTROL register, 	Use FIFO mode
};

const uint16_t  RF_A7129_Config_PageA[]=   //433MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x1706,		//TX1 register, 	Fdev = 37.5kHz
	0x0000,		//WOR1 register,
	0x0000,		//WOR2 register,
	0x1187,		//RFI register, 	Enable Tx Ramp up/down  
	0x8170,		//PM register,		CST=1
	0x0302,		//RTH register,
	0x400F,		//AGC1 register,	
	0x0AC0,		//AGC2 register, 
	0x0045,		//GIO register, 	GIO2=WTR, GIO1=FSYNC
	0xD281,		//CKO register
	0x0004,		//VCB register,
	0x0A21,		//CHG1 register, 	430MHz
	0x0022,		//CHG2 register, 	435MHz
	0x003F,		//FIFO register, 	FEP=63+1=64bytes
	0x1507,		//CODE register, 	Preamble=4bytes, ID=4bytes
	0x0000		//WCAL register,
};

const uint16_t  RF_A7129_Config_PageB[]=   //433MHz, 10kbps (IFBW = 100KHz, Fdev = 37.5KHz), Crystal=12.8MHz
{
	0x0B37,		//TX2 register,
	0x8400,		//IF1 register, 	Enable Auto-IF, IF=200KHz
	0x0000,		//IF2 register,
	0x0000,		//ACK register,
	0x0000		//ART register,
};
#define RF_A7129_SYSTEMCLOCK_REG 	0x00
#define RF_A7129_PLL1_REG 			0x01
#define RF_A7129_PLL2_REG 			0x02
#define RF_A7129_PLL3_REG 			0x03
#define RF_A7129_PLL4_REG			0x04
#define RF_A7129_PLL5_REG			0x05
#define RF_A7129_PLL6_REG			0x06
#define RF_A7129_CRYSTAL_REG			0x07
#define RF_A7129_PAGEA_REG	  		0x08
#define RF_A7129_PAGEB_REG			0x09
#define RF_A7129_RX1_REG  			0x0A
#define RF_A7129_RX2_REG  			0x0B
#define RF_A7129_ADC_REG  			0x0C
#define RF_A7129_PIN_REG 			0x0D
#define RF_A7129_CALIBRATION_REG  	0x0E
#define RF_A7129_MODE_REG  			0x0F

#define RF_A7129_TX1_PAGEA           0x00
#define RF_A7129_WOR1_PAGEA          0x01
#define RF_A7129_WOR2_PAGEA          0x02
#define RF_A7129_RFI_PAGEA           0x03
#define RF_A7129_PM_PAGEA            0x04
#define RF_A7129_RTH_PAGEA           0x05
#define RF_A7129_AGC1_PAGEA          0x06
#define RF_A7129_AGC2_PAGEA          0x07
#define RF_A7129_GIO_PAGEA           0x08
#define RF_A7129_CKO_PAGEA           0x09
#define RF_A7129_VCB_PAGEA           0x0A
#define RF_A7129_CHG1_PAGEA          0x0B
#define RF_A7129_CHG2_PAGEA          0x0C
#define RF_A7129_FIFO_PAGEA			0x0D
#define RF_A7129_CODE_PAGEA			0x0E
#define RF_A7129_WCAL_PAGEA			0x0F

#define RF_A7129_TX2_PAGEB			0x00
#define	RF_A7129_IF1_PAGEB			0x01
#define RF_A7129_IF2_PAGEB			0x02
#define	RF_A7129_ACK_PAGEB			0x03
#define	RF_A7129_ART_PAGEB			0x04


#define RF_A7129_CMD_Reg_W			0x00	//000x,xxxx	control register write
#define RF_A7129_CMD_Reg_R			0x80	//100x,xxxx	control register read
#define RF_A7129_CMD_ID_W			0x20	//001x,xxxx	ID write
#define RF_A7129_CMD_ID_R			0xA0	//101x,xxxx	ID Read
#define RF_A7129_CMD_FIFO_W			0x40	//010x,xxxx	TX FIFO Write
#define RF_A7129_CMD_FIFO_R			0xC0	//110x,xxxx	RX FIFO Read
#define RF_A7129_CMD_RF_RST			0x70	//x111,xxxx	RF reset
#define RF_A7129_CMD_TFR				0x60	//0110,xxxx	TX FIFO address pointrt reset
#define RF_A7129_CMD_RFR				0xE0	//1110,xxxx	RX FIFO address pointer reset

#define RF_A7129_CMD_SLEEP			0x10	//0001,0000	SLEEP mode
#define RF_A7129_CMD_IDLE			0x12	//0001,0010	IDLE mode
#define RF_A7129_CMD_STBY			0x14	//0001,0100	Standby mode
#define RF_A7129_CMD_PLL				0x16	//0001,0110	PLL mode
#define RF_A7129_CMD_RX				0x18	//0001,1000	RX mode
#define RF_A7129_CMD_TX				0x1A	//0001,1010	TX mode
//#define CMD_DEEP_SLEEP	0x1C	//0001,1100 Deep Sleep mode(tri-state)
#define RF_A7129_CMD_DEEP_SLEEP		0x1F	//0001,1111 Deep Sleep mode(pull-high)

#ifdef __cplusplus
}
#endif

class RF_A7129
{
public:
    RF_A7129();
   void RF_A7129_Send();                  
   void RF_A7129_Receive();
   
};

