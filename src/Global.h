/*
 * Global.h
 *
 *  Created on: 21 mei 2014
 *      Author: mark
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

	#include <stdbool.h>
	#include <stdint.h>
	#include <stdbool.h>
	#include <Core/Type/type.h>

	typedef enum{
		O_CABINET_ALARM,
		O_CABINET_LOCK,
	}Output_t;

	typedef enum{
		O_CABINET_DOOR,
		O_CABINET_KEY,
	}input_t;

	extern bool g_out_of_order;
	extern int32_t g_ResvNr;
	extern int32_t g_price;
	extern void printstackpointer(char * text);
	extern uint16_t Server_prefix_nr;

#endif

