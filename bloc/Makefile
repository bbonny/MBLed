PROGRAM = main

# One of "ram" or "flash"
MODE = flash

# One of "hd" or "md" or "cl"
DENSITY = hd

# Object files
OBJS = main.o hardware_init.o\
	lib_image.o\
	image.o spi_led.o\
	image_glip.o spi_led_glip.o\
	debug.o checksum.o\
	buzzer.o \
	graph.o \
	manager.o \
	command.o \
	music.o \
	demo.o	snake.o snake_sound.o\
	all_demos.o\
	util.o \
	anim.o \
	queue.o tasks.o list.o port.o heap_1.o\
	synchro.o\
	scheduler.o irda.o packet.o priorityqueue.o  receive.o send.o\
	ping.o algo.o network.o position.o election.o\
	turn.o\
	watchdog.o


include ../Makefile.inc

#CFLAGS += -DGLIP
#CFLAGS  += -DGLIP_DEBUG
CFLAGS += -DMBLED


CFLAGS += -I./inc
CFLAGS += -I../common 
CFLAGS += -I./inc/buzzer
CFLAGS += -I./inc/image
CFLAGS += -I./inc/image/alpha_num_library
CFLAGS += -I./inc/image/graphic_library
CFLAGS += -I./inc/irda
CFLAGS += -I./inc/network
CFLAGS += -I./inc/synchro
CFLAGS += -I./inc/utils
CFLAGS += -I./inc/anim
CFLAGS += -I./inc/manager 
CFLAGS += -I../dataAnim
CFLAGS += -I../dataMusic
CFLAGS += -I../applications
CFLAGS += -I./inc/watchdog


VPATH += ../applications
VPATH += ../dataAnim
VPATH += ../dataMusic
VPATH += ./src
VPATH += ../common 
VPATH += ./src/manager
VPATH += ./src/buzzer
VPATH += ./src/image
VPATH += ./src/irda
VPATH += ./src/network
VPATH += ./src/synchro
VPATH += ./src/utils
VPATH += ./src/firmware
VPATH += ./src/watchdog
