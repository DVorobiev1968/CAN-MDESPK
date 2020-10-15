#include "MDESPKSlave.h"

void MDESPKSlave_heartbeatError(CO_Data* d, UNS8);

UNS8 MDESPKSlave_canSend(Message *);

void MDESPKSlave_initialisation(CO_Data* d);
void MDESPKSlave_preOperational(CO_Data* d);
void MDESPKSlave_operational(CO_Data* d);
void MDESPKSlave_stopped(CO_Data* d);

void MDESPKSlave_post_sync(CO_Data* d);
void MDESPKSlave_post_TPDO(CO_Data* d);
void MDESPKSlave_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex);
void MDESPKSlave_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg);
