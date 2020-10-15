#! gmake
CC = gcc
CXX = g++
LD = g++
OPT_CFLAGS = -O2 -g3
CFLAGS = $(OPT_CFLAGS)
PROG_CFLAGS =  -fPIC
EXE_CFLAGS =  -lpthread -lrt -ldl
OS_NAME = Linux
ARCH_NAME = x86_64
PREFIX = /usr/local
TARGET = unix
CAN_DRIVER = can_virtual
TIMERS_DRIVER = timers_unix
CAN_PROJECT = CAN-MDESPK-App
CAN_HOME=/home/user26/CanFestival-3
CAN_INCLUDE=$(CAN_HOME)/include
SLAVE = Slave
MASTER = Master
SLAVE_OD = MDESPK$(SLAVE)
MASTER_OD = MDESPK$(MASTER)

INCLUDES = -I$(CAN_INCLUDE) -I$(CAN_INCLUDE)/$(TARGET) -I$(CAN_INCLUDE)/$(CAN_DRIVER) -I$(CAN_INCLUDE)/$(TIMERS_DRIVER)

MASTER_OBJS = $(SLAVE_OD).o $(MASTER_OD).o $(CAN_PROJECT).o $(SLAVE).o $(MASTER).o

OBJS = $(MASTER_OBJS) $(CAN_HOME)/src/libcanfestival.a $(CAN_HOME)/drivers/$(TARGET)/libcanfestival_$(TARGET).a

ifeq ($(TARGET),win32)
	CAN_PROJECT = $(CAN_PROJECT).exe
endif

ifeq ($(TIMERS_DRIVER),timers_win32)
	EXE_CFLAGS =
endif

ifeq ($(TIMERS_DRIVER),timers_xeno)
	PROGDEFINES = -DUSE_XENO
endif

ifeq ($(TIMERS_DRIVER),timers_rtai)
	PROGDEFINES = -DUSE_RTAI
endif

all: $(CAN_PROJECT)

$(CAN_HOME)/drivers/$(TARGET)/libcanfestival_$(TARGET).a:
	$(MAKE) -C $(CAN_HOME)/drivers/$(TARGET) libcanfestival_$(TARGET).a


$(CAN_PROJECT): $(OBJS)
	$(LD) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ $(OBJS) $(EXE_CFLAGS)
	
$(SLAVE_OD).c: $(SLAVE_OD).od
	$(MAKE) -C $(CAN_HOME)/objdictgen gnosis
	python $(CAN_HOME)/objdictgen/objdictgen.py $(SLAVE_OD).od $(SLAVE_OD).c
	
$(MASTER_OD).c: $(MASTER_OD).od
	$(MAKE) -C $(CAN_HOME)/objdictgen gnosis
	python $(CAN_HOME)/objdictgen/objdictgen.py $(MASTER_OD).od $(MASTER_OD).c

%.o: %.c
	$(CC) $(CFLAGS) $(PROG_CFLAGS) ${PROGDEFINES} $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(MASTER_OBJS)
	rm -f $(CAN_PROJECT)
		
mrproper: clean
	rm -f $(SLAVE_OD).c
	rm -f $(MASTER_OD).c
	
install: TestMasterSlave
	mkdir -p $(DESTDIR)$(PREFIX)/bin/
	cp $< $(DESTDIR)$(PREFIX)/bin/
	
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(CAN_PROJECT)
