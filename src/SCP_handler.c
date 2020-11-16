#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/Extensions/ini/minIni.h>
#include <Global.h>
#include <stdint.h>
#include <Power.h>
#include <Screen/SCR_Idle.h>

//prototypes
SCP_Data_t SCP_Openpoort(SCP_Device_t * packet_data);
SCP_Data_t SCP_var_int_send(SCP_Device_t * packet_data);
SCP_Data_t SCP_var_int_request(SCP_Device_t * packet_data);
SCP_Data_t SCP_boot(SCP_Device_t * packet_data);
SCP_Data_t SCP_bootOk(SCP_Device_t * packet_data);
SCP_Data_t SCP_ReserveringChanged(SCP_Device_t * Device);

//functions


void SCP_handlers_init(void){
	SCP_add_new_handler( msg_OpenPoort, SCP_Openpoort);
	SCP_add_new_handler( msg_var_int_request, SCP_var_int_request);
	SCP_add_new_handler( msg_var_int_send, SCP_var_int_send);
	SCP_add_new_handler( msg_Boot, SCP_boot);
	SCP_add_new_handler( msg_BootOk, SCP_boot);
	SCP_add_new_handler( msg_ReserveringChanged, SCP_ReserveringChanged);
}

SCP_Data_t SCP_boot(SCP_Device_t * Device){
	uint8_t i;
	printf("Boot Devid %d grId %d \r\n",Device->Info.SCP_devID, Device->Info.SCP_grID);
	if(Device->Info.SCP_grID == MULTIPD_PANEL){
		for(i=0; i<8 ; i++){
			if(g_Mdata[i].Number != 0){
				BUFFER_SCP_MSG_VAR_INT_SEND_IMPORTANT("Energy_point", g_Mdata[i].Number, Device->Info.SCP_devID);
			}
		}
	}
	return SCP_msg_BootOk(SCP_Get_device_info());
}

SCP_Data_t SCP_Openpoort(SCP_Device_t * Device){
	gate_action_t action;
	uint16_t Energy_point_number;
	uint8_t Output_number;
	Energy_point_data_t * Power_data;
	char ini_key_buffer[20];

	action = SCP_msg_OpenPoort__Action(Device->Cur_Packet);
	Energy_point_number = SCP_msg_OpenPoort__Poort(Device->Cur_Packet);
	if(Get_Output_nr(Energy_point_number, &Output_number) == false){
		return SCP_msg_Error();
	}
	Power_data = Get_Energy_point_data(Energy_point_number);


	sprintf(ini_key_buffer, "Manual_active%d", Output_number + 1);

	switch(action){
	case GATE_PULS:
		power_enable(Energy_point_number);
		break;
	case GATE_OPEN:
		ini_putl("Energy_point",ini_key_buffer,1,"Manual control active:",inifile_machine);
		power_enable(Energy_point_number);
		Power_data->State = ST_MANUAL;
		break;
	case GATE_CLOSED:
		ini_putl("Device",ini_key_buffer,2,"Manual control active:",inifile_machine);
		power_disable(Energy_point_number);
		Power_data->State = ST_MANUAL;
		break;
		break;
	case GATE_NORMAL:
	default:
		Power_data->State = ST_NO_RESERVATION;
		Power_data->refresh_data = true;
		break;
	}
	return SCP_msg_Ok();
}

SCP_Data_t SCP_var_int_send(SCP_Device_t * Device){
	uint8_t i;
	char * name = SCP_msg_var_int__varname(Device->Cur_Packet);
	int32_t value =	SCP_msg_var_int_send__value(Device->Cur_Packet);
	int16_t array_nr = SCP_array_number(name);
	Energy_point_data_t * Power_data = Get_Energy_point_data(array_nr);
	if(Power_data == NULL){
		return SCP_msg_Error();
	}

	printf("var int: %s\r\n",name);
	if(strncmp ( name, "State",5) == 0){
		if(value == 1){
			timer_set(&Reservation_tm,CLOCK_SECOND);
			for(i = 0;i<8;i++){
				g_Mdata[i].refresh_reservation = true;
			}
			power_enable(Power_data->Number);
		}
		return SCP_msg_Ok();
	}
	return SCP_msg_Error();
}

SCP_Data_t SCP_var_int_request(SCP_Device_t * Device){
	char * name = SCP_msg_var_int__varname(Device->Cur_Packet);
	int16_t array_nr = SCP_array_number(name);
	Energy_point_data_t * Power_data = Get_Energy_point_data(array_nr);
	if(Power_data == NULL){
		return SCP_msg_Error();
	}

	printf("var int: %s\r\n",name);
	if(strncmp ( name, "State",5) == 0){
		return SCP_msg_var_int_send(name, Power_data->State);
	}else if(strncmp ( name, "MaxCurrent",10) == 0){
		return SCP_msg_var_int_send(name, Power_data->Current_max);
	}else if(strncmp ( name, "Watt",4) == 0){
		return SCP_msg_var_int_send(name, Power_data->Watt);
	}else if(strncmp ( name, "MaxWattHour",11) == 0){
		return SCP_msg_var_int_send(name, Power_data->Watt_H_max);
	}else if(strncmp ( name, "CurrentWattHour",15) == 0){
		return SCP_msg_var_int_send(name, Power_data->Watt_H);
	}else if(strncmp ( name, "Current",7) == 0){
		return SCP_msg_var_int_send(name, Power_data->Current_RMS);
	}

	return SCP_msg_Error();
}

SCP_Data_t SCP_ReserveringChanged(SCP_Device_t * Device){
	uint16_t Energy_point_number = SCP_msg_ReserveringChanged__Machine(Device->Cur_Packet);
	Energy_point_data_t * Power_data = Get_Energy_point_data(Energy_point_number);
	if(Power_data == NULL){
		return SCP_msg_Error();
	}
	Power_data->Resvnr = SCP_msg_ReserveringChanged__ResvNr(Device->Cur_Packet);
	Power_data->refresh_data = true;
	return SCP_msg_Ok();
}
