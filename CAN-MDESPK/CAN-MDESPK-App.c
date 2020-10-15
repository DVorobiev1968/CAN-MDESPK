/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#if defined(WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#include "getopt.h"
void pause(void)
{
	system("PAUSE");
}
#else
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#endif

#include "canfestival.h"
//#include <can_driver.h>
//#include <timers_driver.h>

#include "Master.h"
#include "Slave.h"
#include "CAN-MDESPK-App.h"

// global variables
char* LibraryPath;

UNS32 OnMasterMap1Update(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	eprintf("OnMasterMap1Update:%d\n", MasterMap1);
	return 0;
}

s_BOARD SlaveBoard = {"0", "125K"};
s_BOARD MasterBoard = {"1", "125K"};

#if !defined(WIN32) || defined(__CYGWIN__)
void catch_signal(int sig)
{
  signal(SIGTERM, catch_signal);
  signal(SIGINT, catch_signal);
  eprintf("Got Signal %d\n",sig);
}
#endif

void help(void)
{
  printf("**************************************************************\n");
  printf("*  MDESPKMasterSlave                                           *\n");
  printf("*                                                            *\n");
  printf("*  A simple example for PC. It does implement 2 CanOpen      *\n");
  printf("*  nodes in the same process. A master and a slave. Both     *\n");
  printf("*  communicate together, exchanging periodically NMT, SYNC,  *\n");
  printf("*  SDO and PDO. Master configure heartbeat producer time     *\n");
  printf("*  at 1000 ms for slave node-id 0x02 by concise DCF.         *\n");                                  
  printf("*                                                            *\n");
  printf("*   Usage:                                                   *\n");
  printf("*   ./MDESPKMasterSlave  [OPTIONS]                             *\n");
  printf("*                                                            *\n");
  printf("*   OPTIONS:                                                 *\n");
  printf("*     -l : Can library [\"libcanfestival_can_virtual.so\"]     *\n");
  printf("*                                                            *\n");
  printf("*    Slave:                                                  *\n");
  printf("*     -s : bus name [\"0\"]                                    *\n");
  printf("*     -S : 1M,500K,250K,125K,100K,50K,20K,10K,none(disable)  *\n");
  printf("*                                                            *\n");
  printf("*    Master:                                                 *\n");
  printf("*     -m : bus name [\"1\"]                                    *\n");
  printf("*     -M : 1M,500K,250K,125K,100K,50K,20K,10K,none(disable)  *\n");
  printf("*                                                            *\n");
  printf("**************************************************************\n");
}

/***************************  INIT  *****************************************/
void InitNodes(CO_Data* d, UNS32 id)
{
	/****************************** INITIALISATION SLAVE *******************************/
	if(strcmp(SlaveBoard.baudrate, "none")) {
		setNodeId(&MDESPKSlave_Data, 0x02);

		/* init */
		setState(&MDESPKSlave_Data, Initialisation);
	}

	/****************************** INITIALISATION MASTER *******************************/
	if(strcmp(MasterBoard.baudrate, "none")){
 		RegisterSetODentryCallBack(&MDESPKMaster_Data, 0x2000, 0, &OnMasterMap1Update);
		
		/* Defining the node Id */
		setNodeId(&MDESPKMaster_Data, 0x01);

		/* init */
		setState(&MDESPKMaster_Data, Initialisation);
			
	}
}

void print_env()
{
	char temp_str[100];

	if (strlen(getenv("CAN_HOME")) > 1)
	{
		sprintf(temp_str,"%s/drivers/can_virtual/libcanfestival_can_virtual.so",getenv("CAN_HOME"));
		LibraryPath=(char*)malloc(strlen(temp_str));
		strcpy(LibraryPath,temp_str);
	}
	else
		LibraryPath="/home/user26/CanFestival-3/drivers/can_virtual/libcanfestival_can_virtual.so";

	printf("CANFestival path: %s\n",LibraryPath);
//	exit(1);
}
/***************************  EXIT  *****************************************/
void Exit(CO_Data* d, UNS32 id)
{
	if(strcmp(MasterBoard.baudrate, "none")){
		
		masterSendNMTstateChange(&MDESPKMaster_Data, 0x02, NMT_Reset_Node);
    
    	//Stop master
		setState(&MDESPKMaster_Data, Stopped);
	}
}

/****************************************************************************/
/***************************  MAIN  *****************************************/
/****************************************************************************/
int main(int argc,char **argv)
{

  int c;
  extern char *optarg;

  print_env();

  while ((c = getopt(argc, argv, "-m:s:M:S:l:")) != EOF)
  {
    switch(c)
    {
      case 's' :
        if (optarg[0] == 0)
        {
          help();
          exit(1);
        }
        SlaveBoard.busname = optarg;
        break;
      case 'm' :
        if (optarg[0] == 0)
        {
          help();
          exit(1);
        }
        MasterBoard.busname = optarg;
        break;
      case 'S' :
        if (optarg[0] == 0)
        {
          help();
          exit(1);
        }
        SlaveBoard.baudrate = optarg;
        break;
      case 'M' :
        if (optarg[0] == 0)
        {
          help();
          exit(1);
        }
        MasterBoard.baudrate = optarg;
        break;
      case 'l' :
        if (optarg[0] == 0)
        {
          help();
          exit(1);
        }
        LibraryPath = optarg;
        break;
      default:
        help();
        exit(1);
    }
  }

#if !defined(WIN32) || defined(__CYGWIN__)
  /* install signal handler for manual break */
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);
#endif
	// Also Windows needs to initialize the Timer!
	// Moving this line into the ifndef leads to a lethal crash!
	TimerInit();

#ifndef NOT_USE_DYNAMIC_LOADING
	if (LoadCanDriver(LibraryPath) == NULL)
	    printf("Unable to load library: %s\n",LibraryPath);
#endif		
	// Open CAN devices

	if(strcmp(SlaveBoard.baudrate, "none")){
		
		MDESPKSlave_Data.heartbeatError = MDESPKSlave_heartbeatError;
		MDESPKSlave_Data.initialisation = MDESPKSlave_initialisation;
		MDESPKSlave_Data.preOperational = MDESPKSlave_preOperational;
		MDESPKSlave_Data.operational = MDESPKSlave_operational;
		MDESPKSlave_Data.stopped = MDESPKSlave_stopped;
		MDESPKSlave_Data.post_sync = MDESPKSlave_post_sync;
		MDESPKSlave_Data.post_TPDO = MDESPKSlave_post_TPDO;
		MDESPKSlave_Data.storeODSubIndex = MDESPKSlave_storeODSubIndex;
		MDESPKSlave_Data.post_emcy = MDESPKSlave_post_emcy;

		if(!canOpen(&SlaveBoard,&MDESPKSlave_Data)){
			eprintf("Cannot open Slave Board (%s,%s)\n",SlaveBoard.busname, SlaveBoard.baudrate);
			goto fail_slave;
		}
	}
	if(strcmp(MasterBoard.baudrate, "none")){
		
		MDESPKMaster_Data.heartbeatError = MDESPKMaster_heartbeatError;
		MDESPKMaster_Data.initialisation = MDESPKMaster_initialisation;
		MDESPKMaster_Data.preOperational = MDESPKMaster_preOperational;
		MDESPKMaster_Data.operational = MDESPKMaster_operational;
		MDESPKMaster_Data.stopped = MDESPKMaster_stopped;
		MDESPKMaster_Data.post_sync = MDESPKMaster_post_sync;
		MDESPKMaster_Data.post_TPDO = MDESPKMaster_post_TPDO;
		MDESPKMaster_Data.post_emcy = MDESPKMaster_post_emcy;
		MDESPKMaster_Data.post_SlaveBootup=MDESPKMaster_post_SlaveBootup;
		
		if(!canOpen(&MasterBoard,&MDESPKMaster_Data)){
			eprintf("Cannot open Master Board (%s,%s)\n",MasterBoard.busname, MasterBoard.baudrate);
			goto fail_master;
		}
	}

	// Start timer thread
	StartTimerLoop(&InitNodes);

	// wait Ctrl-C
	pause();

	// Stop timer thread
	StopTimerLoop(&Exit);
	
	// Close CAN devices (and can threads)
	if(strcmp(MasterBoard.baudrate, "none")) canClose(&MDESPKMaster_Data);
fail_master:
	if(strcmp(SlaveBoard.baudrate, "none")) canClose(&MDESPKSlave_Data);
fail_slave:
	TimerCleanup();

	free(LibraryPath);
	return 0;
}
