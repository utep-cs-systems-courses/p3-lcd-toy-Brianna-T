#include <msp430.h>
#include "libTimer.h"
#include "stateMachine.h"
#include "buzzer.h"
#include "switches.h"
#include "led.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "shape.h"
#include "abCircle.h"

AbRectOutline fieldOutline={//arena
  abRectOutlineGetBounds, abRectOutlineCheck,{screenWidth/2-10,screenHeight/2-10}};

AbRect bar105={abRectGetBounds,abRectCheck,{10,5}};//user's bar, 10 5

Layer arenaLayer={(AbShape*)&fieldOutline,{screenWidth/2,screenHeight/2},{0,0},{0,0},COLOR_BLACK,0};//arena to be drawn

Layer layer1={(AbShape*)&circle8,{screenWidth/2,screenHeight/2},{0,0},{0,0},COLOR_YELLOW,
	      &arenaLayer,};//layer 1 has yellow circle=ball bouncing around
Layer layer0={(AbShape*)&bar105,{screenWidth/2,(screenHeight/2)+25},{0,0},{0,0},COLOR_RED,
	      &layer1,};//layer0 is user's bar

u_int bgColor=COLOR_BLUE;
int redrawScreen=1; //not needed rn
Region fieldfence; //fence about the arena

typedef struct MovLayer_s{
  Layer*layer;
  Vec2 velocity;
  struct MovLayer_s *next; //need this for MovLayerDraw
} MovLayer;

MovLayer m11= { &layer1, {2, 1}, 0}; //next pos circle
MovLayer m10= { &layer0, {0, 2}, &m11}; //next pos bar

void movLayerDraw(MovLayer *movLayers, Layer *layers){
  int row, col;
  MovLayer *movLayer;

  and_sr(~8); //disbales interrupts (GIE off)
  for(movLayer = movLayers; movLayer; movLayer=movLayer->next){ //each layer
    Layer *l=movLayer->layer;
    l->posLast=l->pos;
    l->pos=l->posNext;
  }
  or_sr(8); //GIE on

  for(movLayer=movLayers; movLayer; movLayer=movLayer->next){
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for(row =bounds.topLeft.axes[1]; row<= bounds.botRight.axes[1]; row++){
      for(col =bounds.botRight.axes[0]; col <=bounds.botRight.axes[0]; col++){
	Vec2 pixelPos= {col, row};
	u_int color= bgColor;
	Layer *probeLayer;
	for(probeLayer= layers; probeLayer;
	    probeLayer= probeLayer->next){ //'probe' all layers
	  if(abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)){
	    color= probeLayer->color;
	    break;
	  }//checking all layers, probing, at col,row
	}
	lcd_writeColor(color);
      }//forloops
    }
  }//first for loop
}//end

int main(void){
  configureClocks();
  switch_init();
  led_init();
  enableWDTInterrupts(); //timer interrupt
  buzzer_init();
  //buzzer_set_period()
  //buzz_buttons(); removed since not work

  lcd_init();
  layerInit(&layer0);
  layerDraw(&layer0);

  drawString5x7(10,10,"BounceB Game", COLOR_YELLOW, COLOR_BLACK); //title of game
  drawString5x7(10,150,"You are the Bar", COLOR_YELLOW, COLOR_BLACK); //message to user

  shapeInit(); //base class for abstract shapes
  layerGetBounds(&arenaLayer, &fieldfence); //use for keep in fence

  or_sr(0x18); //CPU off, GIE on, power save

  for(;;){
    redrawScreen=0;//only needed if to put LED to indicate CPU on, no need rn
    movLayerDraw(&m10,&layer0);
  }
}
