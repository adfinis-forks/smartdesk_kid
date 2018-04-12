/**
  ******************************************************************************
  * @file    NRF24L01.h
  * @author  Le Quang Thanh
  * @version V1.0.0
  * @date    12-March-2017
  * @brief   This file contains all the definition for the chip nRF24L01
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, I SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 www.mlabviet.com</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF24L01_H
#define __NRF24L01_H
/* Includes ------------------------------------------------------------------*/
/** 
  * @brief  Memory Map definition
  */
#define NRF_CONFIG      0x00	//Configuration Register
#define EN_AA       	0x01	//Enable Auto Acknowledgment,Enhanced ShockBurst
#define EN_RXADDR   	0x02	//Enabled RX Addresses
#define SETUP_AW    	0x03	//Setup of Address Widths
#define SETUP_RETR  	0x04	//Setup of Automatic Retransmissi
#define RF_CH       	0x05	//RF Channel 0-127
#define RF_SETUP    	0x06	//RF Setup Register
#define NRF_STATUS  	0x07		//
#define OBSERVE_TX  	0x08		//Transmit observe register
#define CD          	0x09		//Carrier Detect.

/*
 - Thanh ghi dia chi nhan
 - Toi da 5 Byte
*/
#define RX_ADDR_P0  	0x0A	//Receive address data pipe 0,5 Bytes maximum length	
#define RX_ADDR_P1  	0x0B 	//Receive address data pipe 0,5 Bytes maximum length
#define RX_ADDR_P2  	0x0C
#define RX_ADDR_P3  	0x0D
#define RX_ADDR_P4  	0x0E
#define RX_ADDR_P5  	0x0F

/*
 - Thanh Ghi Dia chi truyen
 - Chi su dung khi cau hinh la thiet bi chi phat
 - Do dai 5 bytes
 - set thanh ghi RX_ADDR_P0 bang voi dia chi nay de thuc hien automatic acknowledge,
 neu thiet bi nay uu tien truyen, va chuc nang Enhanced ShockBurst duoc enable
*/
#define TX_ADDR     	0x10

#define RX_PW_P0    	0x11
#define RX_PW_P1    	0x12
#define RX_PW_P2    	0x13
#define RX_PW_P3    	0x14
#define RX_PW_P4    	0x15
#define RX_PW_P5    	0x16
#define FIFO_STATUS 	0x17
#define DYNPD	    	0x1C
#define FEATURE	    	0x1D
/** 
  * @brief  Bit Mnemonics definition
  */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR       3
#define RF_PWR      6
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0
#define DPL_P5	    5
#define DPL_P4	    4
#define DPL_P3	    3
#define DPL_P2	    2
#define DPL_P1	    1
#define DPL_P0	    0
#define EN_DPL	    2
#define EN_ACK_PAY  1
#define EN_DYN_ACK  0
/** 
  * @brief  Instruction Mnemonics definition
  */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

/** 
  * @brief  Non-P omissions definition
  */
#define LNA_HCURR   0

/** 
  * @brief  P model memory Map definition
  */
#define RPD         0x09
#define W_TX_PAYLOAD_NO_ACK  0xB0

/** 
  * @brief  P model bit Mnemonics definition
  */
#define RF_DR_LOW   5
#define RF_DR_HIGH  3
#define RF_PWR_LOW  1
#define RF_PWR_HIGH 2

/** 
  * @brief  param of NRF_CONFIG register
  */
#define CONFIG_DEFAULT		(uint8_t)(1<<3)  
#define CONFIG_PRIM_TX 		(uint8_t)0x00
#define CONFIG_PRIM_RX		(uint8_t)(1<<0)
#define CONFIG_POWER_DOWN	(uint8_t)0x00
#define CONFIG_POWER_UP		(uint8_t)(1<<1)
#define CONFIG_CRC_1BYTE	(uint8_t)0x00
#define CONFIG_CRC_2BYTE	(uint8_t)(1<<2)
#define CONFIG_MAX_RT_EN	(uint8_t)(0x00)
#define CONFIG_MAX_RT_DIS	(uint8_t)(1<<4)
#define CONFIG_TX_DS_EN		(uint8_t)(0x00)
#define CONFIG_TX_DS_DIS	(uint8_t)(1<<5)
#define CONFIG_RX_DR_EN		(uint8_t)(0x00)
#define CONFIG_RX_DR_DIS	(uint8_t)(1<<6)

/** 
  * @brief  param of EN_AA  Enable Auto Acknowledgment  register
  */
#define EN_AA_DEFAULT	(uint8_t)(0x3F)
#define EN_AA_PIPE0_DIS (uint8_t)(0x00)
#define EN_AA_PIPE0_EN	(uint8_t)(1<<0)
#define EN_AA_PIPE1_DIS (uint8_t)(0x00)
#define EN_AA_PIPE1_EN	(uint8_t)(1<<1)
#define EN_AA_PIPE2_DIS (uint8_t)(0x00)
#define EN_AA_PIPE2_EN	(uint8_t)(1<<2)
#define EN_AA_PIPE3_DIS (uint8_t)(0x00)
#define EN_AA_PIPE3_EN	(uint8_t)(1<<3)
#define EN_AA_PIPE4_DIS (uint8_t)(0x00)
#define EN_AA_PIPE4_EN	(uint8_t)(1<<4)
#define EN_AA_PIPE5_DIS (uint8_t)(0x00)
#define EN_AA_PIPE5_EN	(uint8_t)(1<<5)
/** 
  * @brief  param of EN_RXADDR Enabled RX Addresses  register
  
  */
#define EN_RXADDR_DEFAULT (uint8_t)(0x03)
#define EN_RXADDR_PIPE0_OFF (uint8_t)(x00)
#define EN_RXADDR_PIPE0_ON (uint8_t)(1<<0)
#define EN_RXADDR_PIPE1_OFF (uint8_t)(x00)
#define EN_RXADDR_PIPE1_ON (uint8_t)(1<<1)
#define EN_RXADDR_PIPE2_OFF (uint8_t)(x00)
#define EN_RXADDR_PIPE2_ON (uint8_t)(1<<2)
#define EN_RXADDR_PIPE3_OFF (uint8_t)(x00)
#define EN_RXADDR_PIPE3_ON (uint8_t)(1<<3)
#define EN_RXADDR_PIPE4_OFF (uint8_t)(x00)
#define EN_RXADDR_PIPE4_ON (uint8_t)(1<<4)
#define EN_RXADDR_PIPE5_OFF (uint8_t)(x00)
#define EN_RXADDR_PIPE5_ON (uint8_t)(1<<5)

/** 
  * @brief  param of SETUP_AW Setup of Address Widths  register
  
  */
#define SETUP_AW_DEFAULT 	(uint8_t)(0x03)
#define SETUP_AW_ILLEGAL 	(uint8_t)(0x00)
#define SETUP_AW_3BYTES		(uint8_t)(0x01)
#define SETUP_AW_4BYTES		(uint8_t)(0x02)
#define SETUP_AW_5BYTES		(uint8_t)(0x03)

/** 
  * @brief  param of for SETUP_RETR( Automatic Retransmission)  register
  
  */
#define SETUP_PETR_DEFAULT			(uint8_t)(0x03)
#define SETUP_PETR_RETRANSMIT_OFF	(uint8_t)(0x00)
#define SETUP_PETR_RETRANSMIT_1		(uint8_t)(0x01)	
#define SETUP_PETR_RETRANSMIT_2		(uint8_t)(0x02)
#define SETUP_PETR_RETRANSMIT_3		(uint8_t)(0x03)
#define SETUP_PETR_RETRANSMIT_4		(uint8_t)(0x04)
#define SETUP_PETR_RETRANSMIT_5		(uint8_t)(0x05)
#define SETUP_PETR_RETRANSMIT_6		(uint8_t)(0x06)
#define SETUP_PETR_RETRANSMIT_7		(uint8_t)(0x07)
#define SETUP_PETR_RETRANSMIT_8		(uint8_t)(0x08)
#define SETUP_PETR_RETRANSMIT_9		(uint8_t)(0x09)
#define SETUP_PETR_RETRANSMIT_10		(uint8_t)(0x0A)
#define SETUP_PETR_RETRANSMIT_11		(uint8_t)(0x0B)
#define SETUP_PETR_RETRANSMIT_12		(uint8_t)(0x0C)
#define SETUP_PETR_RETRANSMIT_13		(uint8_t)(0x0D)
#define SETUP_PETR_RETRANSMIT_14		(uint8_t)(0x0E)
#define SETUP_PETR_RETRANSMIT_15		(uint8_t)(0x0F)
#define SETUP_PETR_RETRANSMIT_DELAY_250US	(uint8_t)(0x00)
#define SETUP_PETR_RETRANSMIT_DELAY_500US	(uint8_t)(0x10)
#define SETUP_PETR_RETRANSMIT_DELAY_750US	(uint8_t)(0x20)
#define SETUP_PETR_RETRANSMIT_DELAY_1000US	(uint8_t)(0x30)
#define SETUP_PETR_RETRANSMIT_DELAY_1250US	(uint8_t)(0x40)
#define SETUP_PETR_RETRANSMIT_DELAY_1500US	(uint8_t)(0x50)
#define SETUP_PETR_RETRANSMIT_DELAY_1750US	(uint8_t)(0x60)
#define SETUP_PETR_RETRANSMIT_DELAY_2000US	(uint8_t)(0x70)
#define SETUP_PETR_RETRANSMIT_DELAY_2250US	(uint8_t)(0x80)
#define SETUP_PETR_RETRANSMIT_DELAY_2500US	(uint8_t)(0x90)
#define SETUP_PETR_RETRANSMIT_DELAY_2750US	(uint8_t)(0xA0)
#define SETUP_PETR_RETRANSMIT_DELAY_3000US	(uint8_t)(0xB0)
#define SETUP_PETR_RETRANSMIT_DELAY_3250US	(uint8_t)(0xC0)
#define SETUP_PETR_RETRANSMIT_DELAY_3500US	(uint8_t)(0xD0)
#define SETUP_PETR_RETRANSMIT_DELAY_3750US	(uint8_t)(0xE0)
#define SETUP_PETR_RETRANSMIT_DELAY_4000US	(uint8_t)(0xF0)
/** 
  * @brief  param for RF_SETUP RF Setup Register
  
  */
#define RF_SETUP_DEFAULT	(uint8_t)(0x0F)
#define RF_SETUP_LNA_0		(uint8_t)(0x00)
#define RF_SETUP_LNA_1		(uint8_t)(1<<0)
#define RF_SETUP_N18DBM		(uint8_t)(0x00<<1)
#define RF_SETUP_N12DBM		(uint8_t)(0x01<<1)
#define RF_SETUP_N6DBM		(uint8_t)(0x10<<1)
#define RF_SETUP_0DBM		(uint8_t)(0x11<<1)
#define RF_SETUP_AIR_1M		(uint8_t)(0<<3)
#define RF_SETUP_AIR_2M		(uint8_t)(1<<3)
/** 
  * @brief  param for NRF_STATUS Status Register FLAG
  
  */
#define STATUS_TX_FIFO_AVAILABLE	(uint8_t)(0x00<<0)
#define STATUS_TX_FIFO_FULL			(uint8_t)(0x01<<0)
#define STATUS_DATA_ON_PIPE0		(uint8_t)(0x00<<1)
#define STATUS_DATA_ON_PIPE1		(uint8_t)(0x01<<1)
#define STATUS_DATA_ON_PIPE2		(uint8_t)(0x02<<1)
#define STATUS_DATA_ON_PIPE3		(uint8_t)(0x03<<1)
#define STATUS_DATA_ON_PIPE4		(uint8_t)(0x04<<1)
#define STATUS_DATA_ON_PIPE5		(uint8_t)(0x05<<1)
#define STATUS_RX_FIFO_EMPTY		(uint8_t)(0x07<<1)			
#define STATUS_TX_RETRANSMIT_INTERRUPT 	(uint8_t)(0x00<<4)
#define STATUS_TX_FIFO_INTERRUPT				(uint8_t)(0x00<<5)
#define STATUS_RX_FIFO_INTERRUPT				(uint8_t)(0x00<<6)
/** 
  * @brief  param for FIFO_STATUS Register 
  
  */
#define FIFO_STATUS_RX_HAVE_DATA 	(0x00<<0)
#define FIFO_STATUS_RX_NO_DATA		(0x01<<0)
#define FIFO_STATUS_RX_AVAILABLE	(0x00<<1)
#define FIFO_STATUS_RX_FULL			(0x01<<1)
#define FIFO_STATUS_TX_HAVE_DATA	(0x00<<4)
#define FIFO_STATUS_TX_NO_DATA		(0x01<<4)
#define FIFO_STATUS_TX_AVAILABLE	(0x00<<5)
#define FIFO_STATUS_TX_FULL			(0x01<<5)
#define FIFO_STATUS_RX_REUSE_ON		(0x01<<6)



#endif
