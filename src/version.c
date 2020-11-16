volatile char *Build_Revision      = "4";
volatile char *Build_Date          = "2019/02/14 13:12:16";

#ifndef DEBUGGING
	#if 1
		#warning Source is modified
		volatile char *Version_state = "C";
	#else
		volatile char *Version_state = "R";
	#endif
#else
	volatile char *Version_state = "D";
	#warning Debugging enabled
	#if 1
		#warning Source is modified
	#endif
#endif