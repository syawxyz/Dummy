/*
 * canbus.h
 *
 *  Created on: Feb 14, 2025
 *      Author: syawxyz
 */

#ifndef INC_CANBUS_H_
#define INC_CANBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

extern CAN_TxHeaderTypeDef TxHeaderRemote;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;

extern uint8_t TxCANData[8];
extern uint8_t RxCANData[8];

extern uint8_t sendData[8];
extern uint8_t sendmyRemote;

void CANFilterInit (void);
//void SendRemoteResponse (uint32_t StdId);
//void MotorRoutine (int16_t value);
void CAN1_Tx(int16_t TxData);
void CAN1_Req(void);




#endif /* INC_CANBUS_H_ */
