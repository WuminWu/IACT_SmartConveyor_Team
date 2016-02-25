#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <mqueue.h>
#include "MQTTClient.h"
#include "include/MC_MqttClient.h"
#include "include/MC_Logic_Thread.h"
#include "include/MC_Cmd_Format.h"
// WiringPi
#include "hardware/rbpIac.h"
#include "include/common.h"
#include "include/MC_Mqueue.h"
#include <fcntl.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

bool CVR_checkTopic(char *topicName, char *compare)
{
	if(strcmp(topicName, compare)==0)
		return true;
	else
		return false;
}

int CVR_DispatchNormal(unsigned int  *payload32, MC_Context_Struct *pMcContext)
{
    int compute = ((*payload32 & BIT_MASK_PALLET_POSITION) >> SHIFT_PALLET_POSITION);
	int result;
	switch(compute)
	{
	 	case VALUE_POSITION1 :
			printf("Here is VALUE_POSITION1\n");
			result = mq_send(pMcContext->mqueueClientArray[0], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
		case VALUE_POSITION2 :
			printf("Here is VALUE_POSITION2\n");
			result = mq_send(pMcContext->mqueueClientArray[1], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
		case VALUE_POSITION3 :
			printf("Here is VALUE_POSITION3\n");
			result = mq_send(pMcContext->mqueueClientArray[2], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
		case VALUE_POSITION4 :
			printf("Here is VALUE_POSITION4\n");
			result = mq_send(pMcContext->mqueueClientArray[3], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
		case VALUE_POSITION5 :
			printf("Here is VALUE_POSITION5\n");
			result = mq_send(pMcContext->mqueueClientArray[4], (char *)payload32, PAYLOAD_LEN, 0);
			if(result == -1)
				printf("ERROR : mq_send Failed\n");
			break;
	}
	printf("DispatchPoistion Command is : 0x%08x , value = 0x%02x\n", *payload32,compute);	
	return 0;
}

unsigned int CVR_eventPayloadFormat(unsigned int *aMsg32)
{
	unsigned int eventPayload32 = 0;
	unsigned int moduleType = 0;
	unsigned int eventModuleID = MUDULE_ID;
	unsigned int sid = 0;
	unsigned int eventType = 0;
	unsigned int actionType = 0;
	unsigned int eventPosition = 0;
	unsigned int NA = 0;
	
	unsigned int computePosition = ((*aMsg32 & BIT_MASK_PALLET_POSITION) >> SHIFT_PALLET_POSITION);
	unsigned int computeTypeAndAction = ((*aMsg32 & BIT_MASK_ControlType_Action) >> SHIFT_ControlType_Action);
	unsigned int computeSID = ((*aMsg32 & BIT_MASK_SEQUENCE_ID) >> SHIFT_SEQUENCE_ID);

	switch(computeTypeAndAction)
	{
	 	case VALUE_CONVEYOR_MOVE_FORWARD :
			NA = 0 << SHIFT_NA;
			eventPosition = computePosition << SHIFT_POSITION;
			actionType = ACTION_MOVE_FORWARD_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_CONVEYOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID |sid | eventType | actionType | eventPosition |NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
			
		case VALUE_CONVEYOR_MOVE_REVERSE :

			break;
			
		case VALUE_CYLINDER_UP :
			NA = 0 << SHIFT_NA;
			eventPosition = computePosition << SHIFT_POSITION;
			actionType = ACTION_CYLINDER_UP_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_CYLINDER << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_CONVEYOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID |sid | eventType | actionType | eventPosition |NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
			
		case VALUE_CYLINDER_DOWN :
			NA = 0 << SHIFT_NA;
			eventPosition = computePosition << SHIFT_POSITION;
			actionType = ACTION_CYLINDER_DOWN_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_CYLINDER << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_CONVEYOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID |sid | eventType | actionType | eventPosition |NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			break;
			
		case VALUE_CONVEYOR_MOVE_BOT_FORWARD:
			NA = 0 << SHIFT_NA;
			eventPosition = computePosition << SHIFT_POSITION;
			actionType = ACTION_MOVE_BOT_STOP_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_CONVEYOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID |sid | eventType | actionType | eventPosition |NA;
			printf("eventPayloadFormat : eventPayload32 = 0x%08x\n", eventPayload32);
			
			break;
			
		case VALUE_CONVEYOR_MOVE_BOT_REVERSE:

			break;
			
		case VALUE_CONVEYOR_MOVE_BOT_STOP:
			NA = 0 << SHIFT_NA;
			actionType = ACTION_MOVE_BOT_STOP_DONE << SHIFT_ACTION_DONE;
			eventType = EVT_ACTION_DONE << SHIFT_EVENT_TYPE;
			sid = computeSID << SHIFT_SID;
			eventModuleID = eventModuleID << SHIFT_MODULE_ID;
			moduleType = VALUE_MODULE_CONVEYOR_TYPE << SHIFT_MODULE_TYPE;
			eventPayload32 = moduleType | eventModuleID |sid | eventType | actionType | eventPosition |NA;
			break;
	}
	return eventPayload32;
}

int CVR_publishEvent(void *aPContext, unsigned int aPyload32)
{
	unsigned int payload32 = aPyload32;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)aPContext;
	int rc = publishMsg(*(pMcContext->pClient), PUBLISH_TOPIC_EVENT, (char *)(&payload32), sizeof(payload32));
	return rc;
}

void CVR_normalCmdParser(void *aPContext, unsigned int *payload32)
{
	unsigned int evtPayload32 = 0;
	int result = 0;
	int computeTypeAndAction = ((*payload32 & BIT_MASK_ControlType_Action) >> SHIFT_ControlType_Action);
	int computePosition = ((*payload32 & BIT_MASK_PALLET_POSITION) >> SHIFT_PALLET_POSITION);
	printf("computeTypeAndAction = 0x%08x\n", computeTypeAndAction);

	switch(computeTypeAndAction)
	{
	 	case VALUE_CONVEYOR_MOVE_FORWARD :
			printf("normalCmdParser : VALUE_CONVEYOR_MOVE_FORWARD\n");
			if(computePosition==VALUE_POSITION1)
			{				
				rbpMotor(CONV_MOTOR1);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION2)
			{
				rbpMotor(CONV_MOTOR2);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);				
			}
			else if(computePosition==VALUE_POSITION3)
			{
				rbpMotor(CONV_MOTOR3);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);				
			}
			else if(computePosition==VALUE_POSITION4)
			{
				rbpMotor(CONV_MOTOR4);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);				
			}
			break;
		case VALUE_CYLINDER_UP :
			printf("normalCmdParser : VALUE_CYLINDER_UP\n");
			if(computePosition==VALUE_POSITION1)
			{
				rbpCylinder(CONV_CYLINDER1, ON);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION2)
			{
				rbpCylinder(CONV_CYLINDER2, ON);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION3)
			{
				rbpCylinder(CONV_CYLINDER3, ON);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION4)
			{
				rbpCylinder(CONV_CYLINDER4, ON);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			break;
		case VALUE_CYLINDER_DOWN :
			printf("normalCmdParser : VALUE_CYLINDER_DOWN\n");
			if(computePosition==VALUE_POSITION1)
			{
				rbpCylinder(CONV_CYLINDER1, OFF);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION2)
			{
				rbpCylinder(CONV_CYLINDER2, OFF);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION3)
			{
				rbpCylinder(CONV_CYLINDER3, OFF);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			else if(computePosition==VALUE_POSITION4)
			{
				rbpCylinder(CONV_CYLINDER4, OFF);
				evtPayload32 = CVR_eventPayloadFormat(payload32);
				result = CVR_publishEvent(aPContext, evtPayload32);	
				printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			}
			break;
		case VALUE_CONVEYOR_MOVE_BOT_FORWARD :
			ConveyorMotorMove(CONV_MOTOR5);
			evtPayload32 = CVR_eventPayloadFormat(payload32);
			result = CVR_publishEvent(aPContext, evtPayload32);	
			printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			break;
		case VALUE_CONVEYOR_MOVE_BOT_REVERSE :
			break;
		case VALUE_CONVEYOR_MOVE_BOT_STOP :
			ConveyorMotorStop(CONV_MOTOR5);
			evtPayload32 = CVR_eventPayloadFormat(payload32);
			result = CVR_publishEvent(aPContext, evtPayload32);	
			printf("evtPayload32 = 0x%08x, and result = %d\n", evtPayload32, result);
			break;
	}
	
}

int CVR_DispatchDiag(unsigned int *payload32, MC_Context_Struct *pMcContext)
{
	int computreTypeAndAction = ((*payload32 & BIT_MASK_ControlType_Action) >> SHIFT_ControlType_Action);
	int computePosition = ((*payload32 & BIT_MASK_PALLET_POSITION) >> SHIFT_PALLET_POSITION);
	switch(computreTypeAndAction)
	{
	 	case VALUE_CONVEYOR_MOVE_FORWARD :
			printf("DispatchDiag : VALUE_CONVEYOR_MOVE_FORWARD\n");
			if(computePosition==VALUE_POSITION1)
				ConveyorMotorMove(CONV_MOTOR1);
			else if(computePosition==VALUE_POSITION2)
				ConveyorMotorMove(CONV_MOTOR2);
			else if(computePosition==VALUE_POSITION3)
				ConveyorMotorMove(CONV_MOTOR3);
			else if(computePosition==VALUE_POSITION4)
				ConveyorMotorMove(CONV_MOTOR4);
			break;
		case VALUE_CONVEYOR_MOVE_REVERSE :
			printf("DispatchDiag : VALUE_CONVEYOR_MOVE_REVERSE\n");
			if(computePosition==VALUE_POSITION1)
				printf("DispatchDiag : Position 1\n");
			else if(computePosition==VALUE_POSITION2)
				printf("DispatchDiag : Position 2\n");
			else if(computePosition==VALUE_POSITION3)
				printf("DispatchDiag : Position 3\n");
			else if(computePosition==VALUE_POSITION4)
				printf("DispatchDiag : Position 4\n");
			break;
		case VALUE_CONVEYOR_MOVE_STOP :
			printf("DispatchDiag : VALUE_CONVEYOR_MOVE_STOP\n");
			if(computePosition==VALUE_POSITION1)
				ConveyorMotorStop(CONV_MOTOR1);
			else if(computePosition==VALUE_POSITION2)
				ConveyorMotorStop(CONV_MOTOR2);
			else if(computePosition==VALUE_POSITION3)
				ConveyorMotorStop(CONV_MOTOR3);
			else if(computePosition==VALUE_POSITION4)
				ConveyorMotorStop(CONV_MOTOR4);
			break;
		case VALUE_CONVEYOR_MOVE_BOT_FORWARD :
			printf("DispatchDiag : VALUE_CONVEYOR_MOVE_BOT_FORWARD\n");	
			ConveyorMotorMove(CONV_MOTOR5);
			break;
		case VALUE_CONVEYOR_MOVE_BOT_REVERSE :
			printf("DispatchDiag : VALUE_CONVEYOR_MOVE_BOT_REVERSE\n");
			break;
		case VALUE_CONVEYOR_MOVE_BOT_STOP :
			printf("DispatchDiag : VALUE_CONVEYOR_MOVE_BOT_STOP\n");
			ConveyorMotorStop(CONV_MOTOR5);
			break;
		case VALUE_CYLINDER_UP :
			printf("DispatchDiag : VALUE_CYLINDER_UP\n");
			if(computePosition==VALUE_POSITION1)
				rbpCylinder(CONV_CYLINDER1, ON);
			else if(computePosition==VALUE_POSITION2)
				rbpCylinder(CONV_CYLINDER2, ON);			
			else if(computePosition==VALUE_POSITION3)
				rbpCylinder(CONV_CYLINDER3, ON);
			else if(computePosition==VALUE_POSITION4)
				rbpCylinder(CONV_CYLINDER4, ON);
			break;
		case VALUE_CYLINDER_DOWN :
			printf("DispatchDiag : VALUE_CYLINDER_DOWN\n");
			if(computePosition==VALUE_POSITION1)
				rbpCylinder(CONV_CYLINDER1, OFF);
			else if(computePosition==VALUE_POSITION2)
				rbpCylinder(CONV_CYLINDER2, OFF);
			else if(computePosition==VALUE_POSITION3)
				rbpCylinder(CONV_CYLINDER3, OFF);
			else if(computePosition==VALUE_POSITION4)
				rbpCylinder(CONV_CYLINDER4, OFF);
			break;
		case VALUE_CYLINDER_STOP :
			printf("DispatchDiag : VALUE_CYLINDER_STOP\n");
			if(computePosition==VALUE_POSITION1)
				printf("DispatchDiag : Position 1\n");
			else if(computePosition==VALUE_POSITION2)
				printf("DispatchDiag : Position 2\n");
			else if(computePosition==VALUE_POSITION3)
				printf("DispatchDiag : Position 3\n");
			else if(computePosition==VALUE_POSITION4)
				printf("DispatchDiag : Position 4\n");
			break;
		case VALUE_READ_CONVEYOR_SENSOR :
			printf("DispatchDiag : VALUE_SENSOR_MODE\n");
			if(computePosition==VALUE_SENSOR1)
				printf("VALUE_SENSOR_MODE : Sensor 1 = %d\n", rbpSensorRead(VALUE_SENSOR1));
			else if(computePosition==VALUE_SENSOR2)
				printf("VALUE_SENSOR_MODE : Sensor 2 = %d\n", rbpSensorRead(VALUE_SENSOR2));
			else if(computePosition==VALUE_SENSOR3)
				printf("VALUE_SENSOR_MODE : Sensor 3 = %d\n", rbpSensorRead(VALUE_SENSOR3));
			else if(computePosition==VALUE_SENSOR4)
				printf("VALUE_SENSOR_MODE : Sensor 4 = %d\n", rbpSensorRead(VALUE_SENSOR4));
			else if(computePosition==VALUE_SENSOR5)
				printf("VALUE_SENSOR_MODE : Sensor 5 = %d\n", rbpSensorRead(VALUE_SENSOR5));
			else if(computePosition==VALUE_SENSOR6)
				printf("VALUE_SENSOR_MODE : Sensor 6 = %d\n", rbpSensorRead(VALUE_SENSOR6));
			else if(computePosition==VALUE_SENSOR7)
				printf("VALUE_SENSOR_MODE : Sensor 7 = %d\n", rbpSensorRead(VALUE_SENSOR7));
			else if(computePosition==VALUE_SENSOR8)
				printf("VALUE_SENSOR_MODE : Sensor 8 = %d\n", rbpSensorRead(VALUE_SENSOR8));
			else if(computePosition==VALUE_SENSOR9)
				printf("VALUE_SENSOR_MODE : Sensor 9 = %d\n", rbpSensorRead(VALUE_SENSOR9));
			else if(computePosition==VALUE_SENSOR10)
				printf("VALUE_SENSOR_MODE : Sensor 10 = %d\n", rbpSensorRead(VALUE_SENSOR10));
			else if(computePosition==VALUE_SENSOR11)
				printf("VALUE_SENSOR_MODE : Sensor 11 = %d\n", rbpSensorRead(VALUE_SENSOR11));
			else if(computePosition==VALUE_SENSOR12)
				printf("VALUE_SENSOR_MODE : Sensor 12 = %d\n", rbpSensorRead(VALUE_SENSOR12));
			else if(computePosition==VALUE_SENSOR13)
				printf("VALUE_SENSOR_MODE : Sensor 13 = %d\n", rbpSensorRead(VALUE_SENSOR13));
			else if(computePosition==VALUE_SENSOR14)
				printf("VALUE_SENSOR_MODE : Sensor 14 = %d\n", rbpSensorRead(VALUE_SENSOR14));				
		default : 
			break;
	}
	printf("DispatchDiag Command is : 0x%08x , value = 0x%02x\n", *((unsigned int*)payload32),computreTypeAndAction);
	return 0;
}

void CVR_MC_Station1_Logic_Thread(void *pContext) 
{
	printf("MC_Station1_Logic_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE];
	unsigned int *msg_ptr = (unsigned int *)buffer;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext;
	do{
        /* receive the message */
        ssize_t bytes_read = mq_receive(pMcContext->mqueueServerArray[0], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread 1\n");
		printf("mq_receive : msg_ptr = 0x%08x\n", *((unsigned int*)msg_ptr));
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");
	
		CVR_normalCmdParser(pContext ,msg_ptr);
		
	}while(!pMcContext -> bThread_exit[0]);
		printf("MC_Station1_Logic_Thread_CVR---\n");
}

void CVR_MC_Station2_Logic_Thread(void *pContext )
{
	printf("MC_Station2_Logic_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE]; 
	unsigned int *msg_ptr = (unsigned int *)buffer;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext ;
	do{
        /* receive the message */
        ssize_t bytes_read = mq_receive(pMcContext->mqueueServerArray[1], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread 2\n");
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");

		CVR_normalCmdParser(pContext, msg_ptr);
	
	}while(!pMcContext -> bThread_exit[1]);
	printf("MC_Station2_Logic_Thread_CVR---\n");
}

void CVR_MC_Station3_Logic_Thread(void *pContext)
{
	printf("MC_Station3_Logic_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE]; 
	unsigned int *msg_ptr = (unsigned int *)buffer;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext ;
	do{
        /* receive the message */
        ssize_t bytes_read = mq_receive(pMcContext->mqueueServerArray[2], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread 3\n");
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");
			
		CVR_normalCmdParser(pContext, msg_ptr);
		
	}while(!pMcContext -> bThread_exit[2]);
	printf("MC_Station3_Logic_Thread_CVR---\n");
}

void CVR_MC_Station4_Logic_Thread(void *pContext )
{
	printf("MC_Station4_Logic_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE];
	unsigned int *msg_ptr = (unsigned int *)buffer;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext ;
	do{
        /* receive the message */
        ssize_t	bytes_read = mq_receive(pMcContext->mqueueServerArray[3], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread 4\n");
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");

		CVR_normalCmdParser(pContext, msg_ptr);	
			
	}while(!pMcContext -> bThread_exit[3]);
	printf("MC_Station4_Logic_Thread_CVR---\n");
}

void CVR_MC_Station5_Logic_Thread(void *pContext )
{
	printf("MC_Station5_Logic_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE];
	unsigned int *msg_ptr = (unsigned int *)buffer;
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext ;
	do{
        /* receive the message */
        ssize_t	bytes_read = mq_receive(pMcContext->mqueueServerArray[4], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread 4\n");
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");

		CVR_normalCmdParser(pContext, msg_ptr);	
			
	}while(!pMcContext -> bThread_exit[4]);
	printf("MC_Station4_Logic_Thread_CVR---\n");
}

void CVR_MC_Sensor_Key_Detected_Thread(void *pContext )
{
	printf("MC_Sensor_Key_Detected_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE]; 
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext ;
	do{
        /* receive the message */
        ssize_t	bytes_read = mq_receive(pMcContext->mqueueServerArray[5], buffer, MAX_SIZE, NULL);
		printf("Msg Recieved in Logic_Thread 5\n");
		if(bytes_read==-1)
			printf("ERROR : mq_receive FAILED\n");

	}while(!pMcContext -> bThread_exit[5]);
	printf("MC_Sensor_Key_Detected_Thread_CVR---\n");
}

void CVR_MC_CMD_Dispatch_Thread(void *pContext)
{
	printf("MC_CMD_Dispatch_Thread_CVR+++\n");
	char buffer[CMD_BUFFER_SIZE]; 
	MC_Context_Struct *pMcContext = (MC_Context_Struct *)pContext;
	struct mqttMsg
	{
		char *topicName;
		int topicLen;
		MQTTClient_message *message;
	}msg;

	do{
		ssize_t bytes_read;
		int i;
		
        /* receive the message */
        bytes_read = mq_receive(pMcContext->mqueueServerArray[MQUEUE_RECEIVER_THREAD_NUM], buffer, MAX_SIZE, NULL);
		if(bytes_read != sizeof(msg))
			printf("MC_CMD_Dispatch_Thread : ERROR : mq_receive Failed, bytes_read = %d\n", bytes_read);
		printf("Msg Recieved in MC_CMD_Dispatch_Thread\n");
		memcpy(&msg, buffer, sizeof(msg));
		
		if(CVR_checkTopic(msg.topicName, SUBSCRIBE_TOPIC_FROM_SCC_INIT))
		{
			printf("MC_CMD_Dispatch_Thread : mq_receive : Topic = %s\n", msg.topicName);
			char *ip = "192.168.1.13,C";
			int rc = publishMsg(*(pMcContext->pClient), PUBLISH_TOPIC_INIT, ip, strlen(ip));
			printf("INIT : rc = %d\n", rc);
		}
		else if(CVR_checkTopic(msg.topicName, SUBSCRIBE_TOPIC_SELF_IP))
		{
			unsigned int *payload32 = msg.message->payload;
			if(msg.message->payloadlen != PAYLOAD_LEN)
			{
				printf("ERROR : payloadlen error\n");
				return;
			}
			int compute = ((*payload32 & BIT_MASK_MODE) >> SHIFT_MODE);
			printf("compute = %d\n", compute);
			switch(compute)
			{
				case VALUE_MODE_DIAG : 
					pMcContext->mode = 1;
					printf("Here is DIAG Mode\n");
					CVR_DispatchDiag(msg.message->payload, pMcContext);
					break;
				case VALUE_MODE_NORMAL : 
					pMcContext->mode = 0;
					printf("Here is NORMAL Mode\n");
					CVR_DispatchNormal(msg.message->payload, pMcContext);
					break;
				default : 
					break;
			}
		}
		memcpy(buffer, msg.message->payload, msg.message->payloadlen);
/*
		for(i=3; i>=0; i--)		
			printf("msg.payload = 0x%02x\n",buffer[i]);
*/

		//for(i=3; i>=0; i--)
		//	printf("mq_receive : buffer[%d] = 0x%02x\n", i, buffer[i]);

	}while(!pMcContext -> bThread_exit[6]);
	printf("MC_CMD_Dispatch_Thread_CVR---\n");	
}
