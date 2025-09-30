#ifndef W25Q80DV_H
#define W25Q80DV_H

#include "stm32f4xx_hal.h" // STM32 HAL ���̺귯�� ��� ����

// W25Q80DV ��ɾ� ����
#define W25Q80DV_WRITE_ENABLE  0x06
#define W25Q80DV_PAGE_PROGRAM  0x02
#define W25Q80DV_READ_DATA     0x03
#define W25Q80DV_READ_ID_CMD  0x9F // JEDEC ID �б� ��ɾ�

// �Լ� ����
void W25Q80DV_WriteEnable(SPI_HandleTypeDef *hspi);
void W25Q80DV_PageProgram(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t *data, uint16_t length);
void W25Q80DV_ReadData(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t *data, uint16_t length);

#endif // W25Q80DV_H