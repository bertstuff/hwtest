#ifndef __ENERGY_POINT_H__
#define __ENERGY_POINT_H__

#include "stdint.h"
#include <Core/Process/process.h>
#include "States.h"

typedef struct Energy_point_data_t{
	State_t State;
	uint16_t MaxAmpere;
	uint16_t Ampere;
	uint16_t Power;
	uint32_t MaxWH;
	uint32_t WH;
}Energy_point_data_t;

typedef struct Energy_point_t{
	struct Energy_point_t * next;
	Energy_point_data_t data;
	uint8_t number;
	uint16_t device;
}Energy_point_t;

extern process_event_t event_state_update;

/*
 * @brief start Energy point discover process
 * @return void
 */
void Energy_point_init(void);

/*
 * @brief add new energy point to memory
 * @param energy_point info of energy point to save
 * @return void
 */
void Energy_point_new(struct Energy_point_t Energy_point);

/*
 * @brief returns true when the energy point is known in the memory
 * @param Energy_point_nr the energypoint to find
 * @return true when found
 */
bool Energy_point_exist(uint8_t Energy_point_nr);

/*
 * @brief return the device number of a known energypoint
 * @param Energy_point_nr the energypoint to find
 * @return device number of the energypoint or 0 when not found
 */
uint16_t Energy_point_device(uint8_t Energy_point_nr);

/*
 * @brief return the measurement data
 * @param Energy_point_nr the energypoint to find
 * @return Energy point data
 */
Energy_point_data_t * Energy_point_data(uint8_t Energy_point_nr);

/*
 * @brief loop though all known energy points
 * @param Energy_point prev Energy_point
 * @return next Energy_point
 */
struct Energy_point_t * Energy_point_loop(struct Energy_point_t * Energy_point);

#endif//__ENERGY_POINT_H__
