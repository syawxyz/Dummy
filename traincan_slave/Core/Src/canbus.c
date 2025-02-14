/*
 * canbus.c
 *
 *  Created on: Feb 14, 2025
 *      Author: syawxyz
 */

#include "canbus.h"
#include "can.h"
#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxCANData[8];
uint8_t RxCANData[8];

uint32_t TxMailbox;

CAN_FilterTypeDef canfilterconfig;

void CANFilterInit (void)
{
	CAN_FilterTypeDef canfilterconfig;

	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 0;
	canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;
	canfilterconfig.FilterIdHigh = 0x0000;
	canfilterconfig.FilterIdLow = 0x0000;
	canfilterconfig.FilterMaskIdHigh = 0x0000;
	canfilterconfig.FilterMaskIdLow = 0x0000;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(&hcan, &canfilterconfig);
}

void SendRemoteResponse (uint32_t StdId)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData [8];
	uint32_t value;
	uint32_t TxMailbox;

	value = HAL_GetTick();
	TxHeader.DLC = 4;
	TxHeader.StdId = StdId;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	TxData [0] = value && 0xff;
	TxData [1] = (value >> 8) & 0xff;
	TxData [2] = (value >> 16) & 0xff;
	TxData [3] = (value >> 24) & 0xff;


//	TxData [0] = 41;

	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}

void MotorRoutine (int16_t value)
{
	if (value > 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		if (value > 499)
			value = 499;
	}
	else if (value < 0)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		if (value > -499)
			value = -499;
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	}

	TIM3->CCR1 = abs(value);
}
