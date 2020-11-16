#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <Core/Memory/cmem.h>
#include <Core/Memory/list.h>
#include <core/Process/process.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include "Energy_point.h"

LIST(Energy_point_list);
PROCESS(Energy_point_process, "Energy point process");
process_event_t event_state_update;

void Energy_point_init(void){
	event_state_update = process_alloc_event(); //Claim a event number
	list_init(Energy_point_list);
	process_start(&Energy_point_process, NULL);
}

void Energy_point_new(struct Energy_point_t Energy_point){
	struct Energy_point_t * new_energy_point;
	struct Energy_point_t * loop_energy_point;
	loop_energy_point = list_head(Energy_point_list);
	while(loop_energy_point != NULL){
		if(loop_energy_point->number == Energy_point.number){
			//Energy number already exist. update device number
			loop_energy_point->device = Energy_point.device;
			return;
		}

		loop_energy_point = list_item_next(loop_energy_point);
	}
	//create new
	new_energy_point = cmem_malloc(sizeof(struct Energy_point_t));
	memset(new_energy_point,0,sizeof(struct Energy_point_t));
	memmove(new_energy_point, &Energy_point, sizeof(struct Energy_point_t));
	new_energy_point->next = NULL;
	new_energy_point->data.State = ST_NO_RESERVATION;
	list_add(Energy_point_list, new_energy_point);
}


bool Energy_point_exist(uint8_t Energy_point_nr){
	struct Energy_point_t * loop_energy_point;

	/* Testcode!! */
	loop_energy_point = list_head(Energy_point_list);
	if( loop_energy_point == NULL ) {
		struct Energy_point_t ep;
		ep.number = Energy_point_nr;
		ep.device = 1;
		ep.data.Ampere = 1000;
		ep.data.MaxAmpere = 2000;
		ep.data.State = ST_WAIT_RESERVATION;
		Energy_point_new(ep);
	}
	loop_energy_point = list_head(Energy_point_list);
	while(loop_energy_point != NULL){
		if(loop_energy_point->number == Energy_point_nr){
			return true;
		}

		loop_energy_point = list_item_next(loop_energy_point);
	}

	return false;
}

uint16_t Energy_point_device(uint8_t Energy_point_nr){
	struct Energy_point_t * loop_energy_point;
	loop_energy_point = list_head(Energy_point_list);
	while(loop_energy_point != NULL){
		if(loop_energy_point->number == Energy_point_nr){
			return loop_energy_point->device;
		}

		loop_energy_point = list_item_next(loop_energy_point);
	}

	return 0;
}

Energy_point_data_t * Energy_point_data(uint8_t Energy_point_nr){
	struct Energy_point_t * loop_energy_point;
	loop_energy_point = list_head(Energy_point_list);
	while(loop_energy_point != NULL){
		if(loop_energy_point->number == Energy_point_nr){
			return &loop_energy_point->data;
		}

		loop_energy_point = list_item_next(loop_energy_point);
	}

	return 0;
}

struct Energy_point_t * Energy_point_loop(struct Energy_point_t * Energy_point){
	if(Energy_point == NULL){
		return list_head(Energy_point_list);
	}
	return list_item_next(Energy_point);
}

PROCESS_THREAD(Energy_point_process, ev, data)
{

	//SCP_pack_t * SCP_packet;

	PROCESS_BEGIN();
	while(ev != PROCESS_EVENT_EXIT){

		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}
