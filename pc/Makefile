
CFLAGS += -DSIMULATED
CFLAGS += -I.
CFLAGS += -I..
CFLAGS += -I../../drivers

OBJECTS += simulated.c
OBJECTS += hal.c
OBJECTS += event.c
OBJECTS += ../cli.c
OBJECTS += ../test.c
OBJECTS += ../meas.c
OBJECTS += ../scheduler.c
OBJECTS += ../../drivers/hal_common.c
OBJECTS += ../../drivers/buffer.c


all:
	gcc -g $(CFLAGS) $(OBJECTS)
