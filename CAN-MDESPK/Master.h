#include "MDESPKMaster.h"

void MDESPKMaster_heartbeatError(CO_Data* d, UNS8);
UNS8 MDESPKMaster_canSend(Message *);
void MDESPKMaster_initialisation(CO_Data* d);
void MDESPKMaster_preOperational(CO_Data* d);
void MDESPKMaster_operational(CO_Data* d);
void MDESPKMaster_stopped(CO_Data* d);
void MDESPKMaster_post_sync(CO_Data* d);
void MDESPKMaster_post_TPDO(CO_Data* d);
void MDESPKMaster_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
void MDESPKMaster_post_SlaveBootup(CO_Data* d, UNS8 nodeid);
