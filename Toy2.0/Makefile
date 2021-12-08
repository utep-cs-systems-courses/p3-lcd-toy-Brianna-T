# makfile configuration
COMMON_OBJECTS  = 
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: main.elf 

#additional rules for files
main.elf: ${COMMON_OBJECTS} main.o switches.o led.o stateMachine.o buzzer.o p2InterruptHandler.o wdtInterruptHandler.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd -lShape -lCircle

load: main.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf

led.o: led.c led.h
switches.o: switches.c switches.h

stateMachine.o: switches.h led.h stateMachine.h

buzzer.o: buzzer.c buzzer.h

p2InterruptHandler.o: switches.h
wdtInterruptHandler.o: stateMachine.h
