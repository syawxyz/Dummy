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

CAN_TxHeaderTypeDef TxHeaderRemote;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxCANData[8];
uint8_t RxCANData[8];

uint8_t sendmyRemote = 0;
uint8_t sendData[8];


void CANFilterInit (void)
{
	CAN_FilterTypeDef canfilterconfig;

	canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
	canfilterconfig.FilterBank = 0;
	canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canfilterconfig.FilterIdHigh = 0x0000;
	canfilterconfig.FilterIdLow = 0x0000;
	canfilterconfig.FilterMaskIdHigh = 0x0000;
	canfilterconfig.FilterMaskIdLow = 0x0000;
	canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
	canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;

	HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);
}

//void SendRemoteResponse (uint32_t StdId)
//{
//	CAN_TxHeaderTypeDef TxHeader;
//	uint8_t TxData [8] = {'i', 't', 's'};
//	uint32_t TxMailbox;
//
//
//	TxHeader.DLC = 2;
//	TxHeader.StdId = StdId;
//	TxHeader.IDE = CAN_ID_STD;
//	TxHeader.RTR = CAN_RTR_DATA;
//
//	TxData [0] = 41;
//
//	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
//	{
//		Error_Handler();
//	}
//}

//void MotorRoutine (int16_t value)
//{
//	if (value > 0)
//	{
//		HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
//		if (value > 499)
//			value = 499;
//	}
//	else if (value < 0)
//	{
//		HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_SET);
//		if (value > -499)
//			value = -499;
//	}
//	else
//	{
//		HAL_GPIO_WritePin(M0_GPIO_Port, M0_Pin, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(M1_GPIO_Port, M1_Pin, GPIO_PIN_RESET);
//	}
//
//	TIM1->CCR1 = abs(value);
//}

void CAN1_Tx(int16_t TxData)
{
	uint32_t TxMailbox;

	TxHeader.DLC = 2;
	TxHeader.StdId = 0x65D;
	TxHeaderRemote.StdId = 0x651;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	sendData[0] = TxData && 0xFF;
	sendData[1] = (TxData >> 8) & 0xFF;
	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, sendData, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}

void CAN1_Req(void)
{
	uint32_t TxMailBox;

	TxHeaderRemote.DLC = 4;
	TxHeaderRemote.StdId = 0x651;
	TxHeaderRemote.IDE = CAN_ID_STD;
	TxHeaderRemote.RTR = CAN_RTR_REMOTE;

	if (HAL_CAN_AddTxMessage(&hcan1, &TxHeaderRemote, TxCANData, &TxMailBox) != HAL_OK)
	{
		Error_Handler();
	}
}
