#include <msp430.h>
#include "switches.h"

//switch on p2

void __interrupt_vec(PORT2_VECTOR) Port_2(){ //set up for interrupt for buttons
  if(P2IFG & SWITCHES){
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
