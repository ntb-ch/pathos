#ifndef _CANOPEN_FAULHABER_DRV_H_
#define _CANOPEN_FAULHABER_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

int init_can_nodes(int sock);
int init_faulhaber_motor(int sock, int node);

// homing
int setspeed_homing_faulhaber_motor(int sock, int node, int speed);
int homing_faulhaber_motor(int sock, int node, int homingMethod);

// switch operation mode
int set_position_profile_mode_faulhaber(int sock, int node);

// set motion parameters
int set_acc_dec_faulhaber(int sock, int node, int acc, int dec);
int set_target_speed_faulhaber(int sock, int node, int speed);
int set_max_speed_faulhaber(int sock, int node, int speed);


// int is_switch_active(int sock, int node, int switchId);


#ifdef __cplusplus
}
#endif

#endif /* _CANOPEN_FAULHABER_DRV_H_ */