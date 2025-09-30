#include "EEPROM.h"

extern SPI_HandleTypeDef hspi1;

#include "stm32f4xx_hal.h" // STM32 MCU�� �´� ��� ������ ����

#define EEPROM_CS_LOW()       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define EEPROM_CS_HIGH()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
#define EEPROM_WRITE_ENABLE  0x06
#define EEPROM_WRITE_CMD     0x02
#define EEPROM_READ_CMD      0x03
#define EEPROM_RDSR_CMD      0x05 // RDSR ��ɾ�

#define EEPROM_TOTAL_SIZE 131072 // 128 KB (1-Mbit)

extern SPI_HandleTypeDef hspi1; // SPI �ڵ鷯 ����

extern uint8_t read_data;

uint8_t T_buffer[256];
uint8_t R_buffer[256];

// ���� Ȱ��ȭ �Լ�
void EEPROM_WriteEnable(void) {
	EEPROM_CS_LOW();
	
	T_buffer[0] = 0x06;
	HAL_SPI_TransmitReceive(&hspi1, T_buffer, R_buffer, 1, 10); // RDSR ��ɾ� ����
	
	EEPROM_CS_HIGH();
}

// ���� �������͸� �о� WIP ��Ʈ�� Ȯ���ϴ� �Լ�
uint8_t EEPROM_ReadStatus(void) {    
  	uint32_t start_Tick, Delta_Tms;
  
    T_buffer[0] = 0x05;  // RDSR ��ɾ� ����
	
	start_Tick = uwTick;
	
	while(1)
	{
	  EEPROM_CS_LOW();
	  HAL_SPI_TransmitReceive(&hspi1, T_buffer, R_buffer, 2, 10); // ���� �������� �б� (��ɾ� + ������)
	  EEPROM_CS_HIGH();
	  if((R_buffer[1] & 0x01) == 0)
	  {
		return 0;
	  }
	  Delta_Tms = uwTick - start_Tick;
	  if(Delta_Tms >= 10)
	  {
		return 0xEE;
	  }
	}
}

// ���� �Լ�
byte EEPROM_WriteByte(uint32_t address, byte wr_buffer[], byte wr_length) {
  	byte retry;
	byte i;
	byte wr_rd_verify_error;
  
	
	wr_rd_verify_error = 0;
	for(retry=0; retry<3; retry++)
	{
		// 1. ���� Ȱ��ȭ �Լ�
		EEPROM_WriteEnable(); 
		  
		T_buffer[0] = 0x02;
		T_buffer[1] = (address >> 16) & 0xFF; //Upper address
		T_buffer[2] = (address >> 8) & 0xFF; //Middle address
		T_buffer[3] = address & 0xFF; //Lower address
		for(i=0; i < wr_length; i++)
		{
			T_buffer[4+i] = wr_buffer[i];	//������ ������۴� T_buffer[3]~
		}

		// 2. EEPROM�� �� �ּҿ� data�� ������.	
		EEPROM_CS_LOW(); // CS LOW
		HAL_SPI_TransmitReceive(&hspi1, T_buffer, R_buffer, wr_length+4, 100); // ���� �������� �б� (��ɾ� + ������)
		EEPROM_CS_HIGH(); // CS HIGH
		
		// 3. ���� �������͸� �о� WIP ��Ʈ�� Ȯ���ϴ� �Լ�
		EEPROM_ReadStatus();
		
		// 4. �� �����͸� �дµ�.
		T_buffer[0] = 0x03;               	// Read_Command(0x03)

		EEPROM_CS_LOW(); // CS LOW
		HAL_SPI_TransmitReceive(&hspi1, T_buffer, R_buffer, wr_length+4, 100); //2:2word, 10:10ms time out : read data : R_buffer[3]
		EEPROM_CS_HIGH(); // CS HIGH

		// 5. �� �����Ϳ� �б� �����͸� ���Ѵ�.
		for(i=0; i<wr_length; i++)
		{
			if(T_buffer[i+4] != R_buffer[i+4]) //CPU �ּ� 0x2000 0000���� ���� �� Write_Tx�� Fram���� ������ Rx_buffer���� 3�� ���غ��� 1���̶� Ʋ���� Flag_Error=1
			{
				wr_rd_verify_error++;    
				break;
			}
		}
		if(i == wr_length)
			return 0x00;   //����
	}
	return 0xEE;   //����
}

// �б� �Լ�
uint8_t EEPROM_ReadByte(uint32_t address, byte rd_buffer[], byte rd_length) {  
  	word i, chk=0;
  
    T_buffer[0] = 0x03;
    T_buffer[1] = (address >> 16) & 0xFF; //Upper address
    T_buffer[2] = (address >> 8) & 0xFF; //Middle address
    T_buffer[3] = address & 0xFF; //Lower address

	EEPROM_CS_LOW(); 
	HAL_SPI_TransmitReceive(&hspi1, T_buffer, R_buffer, rd_length+4, 10); // ���� �������� �б� (��ɾ� + ������)
    EEPROM_CS_HIGH();
	
	for(word i=0; i<rd_length; i++)
	{
		rd_buffer[i] = R_buffer[i+4];   //Fram���� ���� ���� �ٽ� cpu ����ü �ּ� 0x2000 0000�� �����Ѵ�.
	}
}

void EEPROM_RESET(void) {
    byte eeprom_wr_data[256];
    word i;
    uint32_t address = 0; // �ʱ�ȭ ���� �ּ�

    // �ʱ�ȭ�� ������ ���� (0x00)
    for(i = 0; i < 256; i++) {
        eeprom_wr_data[i] = 0x00;
    }

    // EEPROM�� ��� �ּҸ� �ʱ�ȭ
    for(address = 0; address < EEPROM_TOTAL_SIZE; address += 64) {
        EEPROM_WriteByte(address, eeprom_wr_data, 64); // 64����Ʈ�� �ʱ�ȭ //(word eeprom_address, byte wr_buf[], word wr_no);
    }
}