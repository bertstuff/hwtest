#ifndef __POWER_H__
#define __POWER_H__
#include <Core/Process/process.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef enum{
	ST_OFF = 0,
	ST_ON = 1,
	ST_OVERCURRENT = 2,
	ST_NO_RESERVATION = 3,
	ST_WAIT_RESERVATION = 4,
	ST_END_RESERVATION = 5,
	ST_NO_KWH_LEFT = 6,
	ST_MANUAL = 7
}State_t;

typedef struct {
	uint16_t Number;
	State_t State;
	uint8_t Current_max;
	uint16_t Current_RMS;
	uint16_t Watt;
	uint32_t Watt_H;
	uint32_t Watt_H_send;
	uint32_t Watt_H_max;
	uint32_t Resvnr;
	struct tm Start_time;
	struct tm End_time;
	bool output_enabled;
	bool refresh_data;
	bool refresh_reservation;
}Energy_point_data_t;


extern Energy_point_data_t g_Mdata[8];
extern process_event_t event_test_input;

void Power_init(void);

void Relay_enable(uint8_t relay);
void Relay_disable(uint8_t relay);
bool Relay_state_known(uint16_t point_nr);
bool Get_Output_nr(uint16_t point_nr, uint8_t * number);
void power_enable(uint16_t point_nr);
void power_disable(uint16_t point_nr);

Energy_point_data_t * Get_Energy_point_data(uint16_t Energy_point);

PROCESS_NAME(MULTIpdtest_process);

#endif /* __POWER_H__ */
