/*********************************************************
VIZIC TECHNOLOGIES. COPYRIGHT 2021.
THE DATASHEETS, SOFTWARE AND LIBRARIES ARE PROVIDED "AS IS." 
VIZIC EXPRESSLY DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER 
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,
OR NONINFRINGEMENT. IN NO EVENT SHALL VIZIC BE LIABLE FOR 
ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF 
PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, 
ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO 
ANY DEFENCE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
OR OTHER SIMILAR COSTS.
*********************************************************/

/********************************************************
 IMPORTANT : This Example is created for the Arduino 1.0 Software IDE
********************************************************/

#include <MINISMARTGPU.h>      //include the MINISMARTGPU library!

MINISMARTGPU lcd;              //create our object called LCD

SG_AXIS LCD_WIDTH, LCD_HEIGHT; //Variables to handle the screen resolution

typedef struct BALL{
  int ballX;           //X position of the ball
  int ballY;           //Y position of the ball
  int dirX;            //X positive or negative direction
  int dirY;            //Y positive or negative direction
  int radius;          //Ball Radius
  unsigned int colour; //Ball Colour
  int speedBall;       //Moving speed of the ball
  SG_FILLGEOM fill;    //SG_FILL or SG_UNFILL ball
};                     //Balls Struct

//declare a ball variable type BALL
struct BALL ball;

/***************************************************/
//Function that updates the current position of the balls
int moveBall(struct BALL *ball){
   lcd.drawCircle(ball->ballX,ball->ballY,ball->radius,SG_BLACK,ball->fill);     // Erase previous ball position
   ball->ballX +=((ball->dirX)*(ball->speedBall));                               // Calculate new x coordinate for ball 
   ball->ballY +=((ball->dirY)*(ball->speedBall));                               // Calculate new y coordinate for ball  
   lcd.drawCircle(ball->ballX,ball->ballY,ball->radius,ball->colour,ball->fill); // Draw new ball position
   if( (((ball->ballX)+(ball->speedBall)+(ball->radius))>(LCD_WIDTH-2)) || (((ball->ballX)-(ball->speedBall)-(ball->radius))<=1) ){  // if ball reaches the left or right corner, we invert moving direction 
    ball->dirX= ((ball->dirX)*(-1));                                             // Invert the moving direction by multiplying by -1
   }
   if( (((ball->ballY)+(ball->speedBall)+(ball->radius))>(LCD_HEIGHT-2)) || (((ball->ballY)-(ball->speedBall)-(ball->radius))<=1) ){ // if ball reaches the top or bottom corner, we invert moving direction 
    ball->dirY= ((ball->dirY)*(-1));                                             // Invert the moving direction by multiplying by -1
   }                       
}

/*********************************************************/
/*********************************************************/
void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
  
  //Obtain screen resolution
  lcd.getWidth(&LCD_WIDTH);
  lcd.getHeight(&LCD_HEIGHT);
}

/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
void loop() { //main loop 
  //fill ball parameter 
  ball.ballX=LCD_WIDTH/2;
  ball.ballY=LCD_HEIGHT/2;
  ball.dirX=1;
  ball.dirY=1;
  ball.radius=15;
  ball.colour=SG_GREEN;  
  ball.speedBall=2;
  ball.fill=SG_FILL;
  
  lcd.baudChange(SG_BAUD6); //For fast drawing we need a big baudRate. Note: if using Software Serial, use SG_BAUD2 or less!
  lcd.drawRectangle(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,SG_MAGENTA,SG_UNFILL); //draw corners
  
  while(1){              // Loop forever
    moveBall(&ball);     // move ball1
    delay(15);           // wait a little
  }
}
