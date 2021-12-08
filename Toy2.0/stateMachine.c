#include <msp430.h>
#include "stateMachine.h"
#include "led.h"

void toggle_green(){ //dim green
  static char state=0;

  switch(state){
  case 0:
    red_on=1; //on
    state=1;
    break;
  case 1: //off
    red_on=0;
    state=0;
    break;
  }
  return 1; //changes LED
}

/*
void state_advance(){
  char changed=0;

  static enum (R=0, G=1)
*/
