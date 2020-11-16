/*
 * States.h
 *
 *  Created on: 1 okt. 2020
 *      Author: mark
 */

#ifndef STATES_H_
#define STATES_H_

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


#endif /* STATES_H_ */
