/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "usart.h"
/* USER CODE BEGIN 0 */
#include "motors.h"


/* USER CODE END 0 */

SPI_HandleTypeDef hspi2;

/* SPI2 init function */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
    /* SPI2 clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PA10     ------> SPI2_MISO
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */

  /* USER CODE END SPI2_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI2)
  {
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /**SPI2 GPIO Configuration
    PB13     ------> SPI2_SCK
    PA10     ------> SPI2_MISO
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
// Git test

enc_list_st encoders = {
    .enc_1 = {.cs_pin = CS_ENC_1_Pin, .cs_port = CS_ENC_1_GPIO_Port, .motor = &motor1}, // Encoder 1
    .enc_2 = {.cs_pin = CS_ENC_2_Pin, .cs_port = CS_ENC_2_GPIO_Port, .motor = &motor2}, // Encoder 2
    .enc_3 = {.cs_pin = CS_ENC_3_Pin, .cs_port = CS_ENC_3_GPIO_Port, .motor = &motor3}, // Encoder 3
    .enc_4 = {.cs_pin = CS_ENC_4_Pin, .cs_port = CS_ENC_4_GPIO_Port, .motor = &motor4}, // Encoder 4
    .enc_5 = {.cs_pin = CS_ENC_5_Pin, .cs_port = CS_ENC_5_GPIO_Port, .motor = &motor5}  // Encoder 5
};


void SPI_init(){
	HAL_GPIO_WritePin(encoders.enc_1.cs_port, encoders.enc_1.cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(encoders.enc_2.cs_port, encoders.enc_2.cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(encoders.enc_3.cs_port, encoders.enc_3.cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(encoders.enc_4.cs_port, encoders.enc_4.cs_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(encoders.enc_5.cs_port, encoders.enc_5.cs_pin, GPIO_PIN_SET);
}
void UART2_SendData(uint8_t *data, uint16_t size) {
    HAL_UART_Transmit(&huart2, data, size, HAL_MAX_DELAY);
}

static bool checksum_check(uint16_t value){
	uint8_t odd_parity = 1 << 1 & value;
	uint8_t even_parity = 1 & value;
	for (uint8_t even_i = 3, odd_i = 2; even_i < 16; even_i++, odd_i++){
		odd_parity ^= value & 1 << odd_i;
		even_parity ^= value & 1 << even_i;
	}
	return ((value >> 15) == odd_parity)&((value >> 14) == odd_parity);
}

void SPI_read_data(encoder_st* encoder){
    HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Receive(&hspi2, encoder->value, sizeof(encoder->value), HAL_MAX_DELAY);
    HAL_GPIO_WritePin(encoder->cs_port, encoder->cs_pin, GPIO_PIN_SET);
    encoder->is_valid = checksum_check(encoder->is_valid);
    UART2_SendData(encoder->value, sizeof(encoder->value));
}

void SPI_read_all(){
	encoder_st* ptr = &encoders;
	for (uint8_t i=0;i<sizeof(encoders)/sizeof(encoder_st);i++){
		SPI_read_data(&ptr[i]);
	}
}
/* USER CODE END 1 */
