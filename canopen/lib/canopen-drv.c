/**
 * Autor: A. Kalberer
 * 
 * Helper functions for Faulhaber drive with CANopen
 * 
 * - adds PDO support to libcanopen (https://github.com/rscada/libcanopen)
 * 
 * 
 * */


#include "../include/canopen-drv.h"
#include "../include/canopen.h"
#include <stdio.h>
#include <errno.h>


int canopen_frame_fill_pdo_set_position(canopen_frame_t *frame, uint8_t function_code, uint8_t node, uint16_t control, uint32_t targetpos){

	if(frame == NULL)
		return -1;
	
	frame->rtr = CANOPEN_FLAG_NORMAL;
	frame->function_code = function_code;
	frame->type = CANOPEN_FLAG_STANDARD;
	frame->id = node;

	frame->payload.data[0] = (control & 0x00FF);
	frame->payload.data[1] = ((control & 0xFF00) >> 8);
	frame->payload.data[2] = (targetpos & 0x000000FF);
	frame->payload.data[3] = ((targetpos & 0x0000FF00) >> 8);
	frame->payload.data[4] = ((targetpos & 0x00FF0000) >> 16);
	frame->payload.data[5] = ((targetpos & 0xFF000000) >> 24);
	frame->data_len = 6;

	return 0;
}

int canopen_pdo_set_abs_position(int sock, uint8_t node, uint8_t function_code, uint16_t control, uint32_t targetpos){
	canopen_frame_t canopen_frame;
	struct can_frame can_frame; 
	
	canopen_frame_fill_pdo_set_position(&canopen_frame, function_code, node, control, targetpos);
	if (canopen_frame_pack(&canopen_frame, &can_frame) != 0)
    	{
		return -1;
   	}	
//  	canopen_frame_dump_short(&canopen_frame);
    	// send the frame to the CAN bus
    	
    	int bytes_sent = write(sock, &can_frame, sizeof(can_frame));
	if (bytes_sent < 0)
    	{
        	return -1;
    	}

    	if (bytes_sent < (int)sizeof(struct can_frame))
    	{
      		return -1;
    	}
    	return 0;  
}

int canopen_pdo_get_act_position(int sock, uint8_t node, uint8_t function_code){
	canopen_frame_t canopen_frame;
	struct can_frame can_frame;
	
	canopen_frame_fill_pdo_get_position(&canopen_frame, function_code, node);
	if(canopen_frame_pack(&canopen_frame, &can_frame) != 0){
		return -1;
	}
	
	int bytes_sent = write(sock, &can_frame, sizeof(can_frame));
	if(bytes_sent < 0){
		return -2;
	}
	if(bytes_sent < (int)sizeof(struct can_frame)){
		return -3;
	}
	
	return 0;
  
}

int canopen_frame_fill_pdo_get_position(canopen_frame_t *frame, uint8_t function_code, uint8_t node){
	if(frame == NULL){
		return -1;
	}
		
	frame->rtr = CANOPEN_FLAG_RTR;
	frame->function_code = function_code;
	frame->type = CANOPEN_FLAG_STANDARD;
	frame->id = node;

	frame->data_len = 0;

	return 0;
	
}

int canopen_read_socket(int sock, uint8_t function_code, uint16_t *status, uint32_t *actpos){
	canopen_frame_t canopen_read_frame;
	struct can_frame can_read_frame;
	
	int bytes_read = read(sock, &can_read_frame, sizeof(can_read_frame));
	if(bytes_read < 0){
		return -4;
	}
	if(bytes_read < (int)sizeof(struct can_frame)){
		return -5;
	}
	if(canopen_frame_parse(&canopen_read_frame, &can_read_frame) != 0){
		return -6;
	}
	if(canopen_read_frame.function_code != function_code){
		return -7;		//TODO define return values properly
	}
	if(canopen_read_frame.data_len > 2){
		*actpos = canopen_read_frame.payload.data[2] & 0x00FF;
		*actpos |= ((canopen_read_frame.payload.data[3] & 0x00FF) << 8);
		if(canopen_read_frame.data_len > 4){
			*actpos |= ((canopen_read_frame.payload.data[4] & 0x00FF) << 16);
			*actpos |= ((canopen_read_frame.payload.data[5] & 0x00FF) << 24);
		}
	}
	*status = canopen_read_frame.payload.data[0] & 0x00FF;
	*status |= ((canopen_read_frame.payload.data[1] & 0x00FF) << 8);
	
	return canopen_read_frame.data_len;
}

int canopen_read_frame(int sock, canopen_frame_t* canopen_frame){
	struct can_frame can_read_frame;
	
	int bytes_read = read(sock, &can_read_frame, sizeof(can_read_frame));
	if(bytes_read < 0){
		return -4;
	}
	if(bytes_read < (int)sizeof(struct can_frame)){
		return -5;
	}
	if(canopen_frame_parse(canopen_frame, &can_read_frame) != 0){
		return -6;
	}
	
	return (*canopen_frame).data_len;
}

int canopen_pdo_send_2bytes(int sock, uint8_t node, uint8_t function_code, uint16_t userData){
	canopen_frame_t canopen_frame;
	struct can_frame can_frame; 
	
	canopen_frame_fill_pdo_2bytes(&canopen_frame, function_code, node, userData);
	if (canopen_frame_pack(&canopen_frame, &can_frame) != 0)
    	{
		return -1;
   	}
    	// send the frame to the CAN bus
     	int bytes_sent = write(sock, &can_frame, sizeof(can_frame));
	if (bytes_sent < 0)
    	{
        	return -1;
    	}

    	if (bytes_sent < (int)sizeof(struct can_frame))
    	{
      		return -1;
    	}
    	return 0;
}

int canopen_frame_fill_pdo_2bytes(canopen_frame_t *frame, uint8_t function_code, uint8_t node, uint16_t userData){

	if(frame == NULL)
		return -1;
	
	frame->rtr = CANOPEN_FLAG_NORMAL;
	frame->function_code = function_code;
	frame->type = CANOPEN_FLAG_STANDARD;
	frame->id = node;

	frame->payload.data[0] = (userData & 0x00FF);
	frame->payload.data[1] = ((userData & 0xFF00) >> 8);
	frame->data_len = 2;

	return 0;
}

int canopen_pdo_send_6bytes(int sock, uint8_t node, uint8_t function_code, uint16_t devCtrl, uint32_t data){
	canopen_frame_t canopen_frame;
	struct can_frame can_frame; 
	
	canopen_frame_fill_pdo_6bytes(&canopen_frame, function_code, node, devCtrl, data);
	if (canopen_frame_pack(&canopen_frame, &can_frame) != 0)
    	{
		return -1;
   	}
    	// send the frame to the CAN bus
     	int bytes_sent = write(sock, &can_frame, sizeof(can_frame));
	if (bytes_sent < 0)
    	{
        	return -1;
    	}

    	if (bytes_sent < (int)sizeof(struct can_frame))
    	{
      		return -1;
    	}
    	return 0;
}

int canopen_frame_fill_pdo_6bytes(canopen_frame_t *frame, uint8_t function_code, uint8_t node, uint16_t devCtrl, uint32_t data){

	if(frame == NULL)
		return -1;
	
	frame->rtr = CANOPEN_FLAG_NORMAL;
	frame->function_code = function_code;
	frame->type = CANOPEN_FLAG_STANDARD;
	frame->id = node;

	frame->payload.data[0] = (devCtrl & 0x00FF);
	frame->payload.data[1] = ((devCtrl & 0xFF00) >> 8);
	frame->payload.data[2] = (data & 0x000000FF);
	frame->payload.data[3] = ((data & 0x0000FF00) >> 8);
	frame->payload.data[4] = ((data & 0x00FF0000) >> 16);
	frame->payload.data[5] = ((data & 0xFF000000) >> 24);
	frame->data_len = 6;

	return 0;
}

int canopen_pdo_request(int sock, uint8_t node, uint8_t function_code){
	canopen_frame_t canopen_frame;
	struct can_frame can_frame;
	
	canopen_frame_fill_pdo_request(&canopen_frame, function_code, node);
	if(canopen_frame_pack(&canopen_frame, &can_frame) != 0){
		return -1;
	}
	
	int errsv = 0;
	int bytes_sent = write(sock, &can_frame, sizeof(can_frame));
	errsv = errno;
	if(bytes_sent < 0){
		if(bytes_sent == -1){
			
			return errsv;
		}
		else{
			return -4;
		}
	}
	if(bytes_sent < (int)sizeof(struct can_frame)){
		return -3;
	}
	
	return 0;
  
}

int canopen_frame_fill_pdo_request(canopen_frame_t *frame, uint8_t function_code, uint8_t node){
	if(frame == NULL){
		return -1;
	}
		
	frame->rtr = CANOPEN_FLAG_RTR;
	frame->function_code = function_code;
	frame->type = CANOPEN_FLAG_STANDARD;
	frame->id = node;

	frame->data_len = 0;

	return 0;
	
}
