#ifndef _CANOPEN_FAULHABER_DRV_H_
#define _CANOPEN_FAULHABER_DRV_H_

#ifdef __cplusplus
extern "C" {
#endif

int init_can_nodes(int sock);
int init_faulhaber_motor(int sock, int node);
int homing_faulhaber_motor_actualPos(int sock, int node);
int homing_faulhaber_motor_endSwitch(int sock, int node);
int set_ramp_faulhaber(int sock, int node, int speed);
int set_max_speed_faulhaber(int sock, int node, int speed);

#ifdef __cplusplus
}
#endif

#endif /* _CANOPEN_FAULHABER_DRV_H_ */