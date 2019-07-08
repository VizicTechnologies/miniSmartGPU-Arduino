/*********************************************************
VIZIC TECHNOLOGIES. COPYRIGHT 2013.
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

#include <MINISMARTGPU.h>     //include the MINISMARTGPU library!

MINISMARTGPU lcd;             //create our object called LCD

//Connect 2 push buttons to arduino to play, one to defined pin MOVELEFT and other to defined pin MOVERIGHT, both must reach GND when pressed.
//defines for controls
#define MOVERIGHT 8        //control inputs
#define MOVELEFT  9        //control inputs

// defines for balls
#define radiusBall1 10     //ball1 size
#define colourBall1 GREEN  //ball1 colour
#define radiusBall2 8      //ball2 size
#define colourBall2 YELLOW //ball2 colour
#define DEFSPEEDB1 2; //ball1 moving speed - amount of pixels that ball moves each time
#define DEFSPEEDB2 2; //ball2 moving speed - amount of pixels that ball moves each time 

//variables used by move ball methods
int dirx1=1;      //xball1 initial positive direction
int diry1=1;      //yball1 initial positive direction
int xBall1=20;    //x initial position of ball1
int yBall1=30;    //y position of ball1
int dirx2=-1;     //xball2 initial negative direction
int diry2=1;      //yball2 initial positive direction
int xBall2=80;    //x initial position of ball2
int yBall2=40;    //y position of ball2
int speedBall1,speedBall2;

//variables used by Pong method
char score[7]={0,0,' ','P','T','S',0x00}; //array to save score
char points;            
char gameOver;          //game over flag
char speedCounter;      //Counter that saves the speed 
char ball1Active;       //acrive ball flag
char ball2Active;       //active ball flag 
int barSize = 40;       //size of bar in pixels 
int bar=80;             //initial x position of the bar   
int barCenter = (barSize/2)+bar; //we need to know the center of the bar


/***************************************************/
//function that move or update the actual position of the ball1
void moveBall1(){
   lcd.drawCircle(xBall1,yBall1,radiusBall1,BLACK,UNFILL);       // Erase previous ball position
   xBall1+=(dirx1*speedBall1);                                   // Calculate new x coordinate for ball1 
   yBall1+=(diry1*speedBall1);                                   // Calculate new y coordinate for ball1  
   lcd.drawCircle(xBall1,yBall1,radiusBall1,colourBall1,UNFILL); // Draw new ball position
   if((xBall1+speedBall1+radiusBall1)>158 | (xBall1-speedBall1-radiusBall1)<=1){       // if ball reaches the left or right corner, we invert moving direction 
    dirx1= dirx1*(-1);
   }
   if((yBall1+speedBall1+radiusBall1)>121 | (yBall1-speedBall1-radiusBall1)<=15){      // if ball reaches the top or bottom corner, we invert moving direction 
    if((yBall1-speedBall1-radiusBall1)<=15){                    // Bounce on top, only invert moving direction
      diry1= diry1*(-1);                                        // We invert the moving direction by multiplying by -1
    }else{                                                      // Bounce on bottom, check if inside the bar       
      if((xBall1+speedBall1+radiusBall1)>bar & (xBall1-speedBall1-radiusBall1)<(bar+barSize)){  //if bounce inside the bar
        diry1= diry1*(-1);                                      // We invert the moving direction by multiplying by -1
        points++;                                               // Increase player current points
        speedCounter++;                                         // Increase the speed counter
        if(speedCounter>9){                                     // If we reach 10 counts we increase the ball1 bouncing speed
          speedBall1++;          
          speedCounter=0;                                       // Reset the counter
        }                           
      }else{                                                   // Bounce outside the bar
        ball1Active=0;                                         // Clear ball1 active flag
        lcd.drawCircle(xBall1,yBall1,radiusBall1,BLACK,UNFILL);// Delete this ball because bounce outside of the bar        
        if(ball1Active==0 & ball2Active==0){                   // if we have lost both balls     
          gameOver=1;                                          // Set game over flag
        }         
      }
    }
   } 
}

/***************************************************/
//function that move or update the actual position of the ball2
void moveBall2(){
   lcd.drawCircle(xBall2,yBall2,radiusBall2,BLACK,FILL);       // Erase previous ball position
   xBall2+=(dirx2*speedBall2);                                 // Calculate new x coordinate for ball2 
   yBall2+=(diry2*speedBall2);                                 // Calculate new y coordinate for ball2
   lcd.drawCircle(xBall2,yBall2,radiusBall2,colourBall2,FILL); // Draw new ball position
   if((xBall2+speedBall2+radiusBall2)>158 | (xBall2-speedBall2-radiusBall2)<=1){       // if ball reaches the left or right corner, we invert moving direction 
    dirx2= dirx2*(-1);
   }
   if((yBall2+speedBall2+radiusBall2)>121 | (yBall2-speedBall2-radiusBall2)<=15){      // if ball reaches the top or bottom corner, we invert moving direction 
    if((yBall2-speedBall2-radiusBall2)<=15){                    // Bounce on top, only invert moving direction
      diry2= diry2*(-1);        
    }else{                                                      // Bounce on bottom, check if inside the bar       
      if((xBall2+speedBall2+radiusBall2)>bar & (xBall2-speedBall2-radiusBall2)<(bar+barSize)){  //if bounce inside the bar
        diry2= diry2*(-1);                                      // We invert the moving direction by multiplying by -1
        points++;                                               // Increase player current points
        speedCounter++;                                         // Increase the speed counter
        if(speedCounter>9){                                     // If we reach 10 counts we increase the ball1 bouncing speed
          speedBall2++;          
          speedCounter=0;                                       // Reset the counter
        }                           
      }else{                                                   // Bounce outside the bar
        ball2Active=0;                                         // Clear ball1 active flag
        lcd.drawCircle(xBall2,yBall2,radiusBall2,BLACK,FILL);  // Delete this ball because bounce outside of the bar        
        if(ball1Active==0 & ball2Active==0){                   // if we have lost both balls     
          gameOver=1;                                          // Set game over flag
        }         
      }
    }
   } 
}

/***************************************************/
/***************************************************/
//The game methodology
void pong(){
  unsigned char i;
  char score[7]={0,0,' ','P','T','S',0x00};
  char buffer[3];
  
  //reset all parameters
  gameOver=0;            //reset flag
  points=0;              //reset points
  speedCounter=0;        //reset speed
  speedBall1=DEFSPEEDB1; //ball1 moving speed - amount of pixels that ball moves each time
  speedBall2=DEFSPEEDB2; //ball2 moving speed - amount of pixels that ball moves each time   
  ball1Active=1;         //acrive ball flag
  ball2Active=1;         //active ball flag 
  yBall1=30+radiusBall1; //y initial position of ball1 
  yBall2=40+radiusBall2; //y initial position of ball2
  
  lcd.drawRectangle(0,15,159,127,MAGENTA,UNFILL);                        //draw corners
  lcd.drawLine(bar,122,bar+barSize,122,WHITE);                           //draw Bar 
  
  while(gameOver==0){                                                    //while game over flag is zero                          
    buffer[0]=(points/10)+0x30, buffer[1]=(points%10)+0x30, buffer[2]=0; //fill buffer that counts
    lcd.setTextColour(RED);
    lcd.setTextSize(FONT0);
    lcd.setTextBackFill(FILL);
    lcd.string(3,2,30,30,buffer,0);                                      //display current points
      for(i=0;i<5;i++){                                                  //check 5 times if the player touches the screen
        if(!(digitalRead(MOVELEFT) & digitalRead(MOVERIGHT))){           //if a button is pressed then we move the bar to pressed button side
          lcd.drawLine(bar,122,bar+barSize,122,BLACK);                   //erase previous Bar    
          if(!digitalRead(MOVERIGHT)){                                   //if the pressed button is MOVERIGHT, move the bar to the right           
            bar+=2;                                                      //move the bar to the right  8 pixels
            if((bar+barSize)>158){                                       //if the bar reach the right corner
              bar=158-barSize;      
            }              
            barCenter=bar+(barSize/2);                                   //set new center position of the bar
          }else{                                                         //move the bar to the left                                   
            bar-=2;                                                      //move the bar to the left 8 pixels 
            if(bar<1){                                                   //if the bar reach the left corner
              bar=1;  
            }              
            barCenter=bar+(barSize/2);                                   //set new center position of the bar
          }
          lcd.drawLine(bar,122,bar+barSize,122,WHITE);                   //draw the new bar at the new position
        }
      }
      //we update the balls
      if(ball1Active){                                                   //if we haven't lost the ball1
        moveBall1(); 
      }
      if(ball2Active){                                                   //if we haven't lost the ball2
        moveBall2();      
      }
      delay(20);
  }
  //game over - proceed to show final score
  lcd.setTextSize(FONT2);
  lcd.setTextBackFill(UNFILL);  
  lcd.string(40,45,136,70,"Game Over",0);
  score[0]=(points/10)+0x30;                                            //convert points to ascii format and store them on the score buffer
  score[1]=(points%10)+0x30;                                            //convert points to ascii format and store them on the score buffer
  lcd.setTextColour(YELLOW);
  lcd.string(57,60,136,70,score,0);
  lcd.setTextColour(GREEN);  
  lcd.setTextSize(FONT0);  
  lcd.string(27,75,159,127,"Press button to Exit",0);          
  delay(2000);
  while(digitalRead(MOVELEFT) & digitalRead(MOVERIGHT));                //wait for a button to be pressed
  lcd.erase();                                                          //erase the screen and exit function
}

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor

  //configure input controls
  pinMode(MOVELEFT, INPUT_PULLUP);           // set pin to input (buttons)
  pinMode(MOVERIGHT, INPUT_PULLUP);          // set pin to input (buttons)
}

void loop() { //main loop
  lcd.baudChange(2000000); //set a fast baud! is recommended to use fast baud rates

  while(1){                // Loop forever
    //Draw a cover
    lcd.drawTriangle(12,10,40,20,25,40,YELLOW,FILL);
    lcd.drawRectangle(120,15,145,40,MAGENTA,FILL);
    lcd.drawCircle(28,100,14,CYAN,FILL);
    lcd.drawTriangle(130,115,115,95,145,85,RED,FILL);
    lcd.setTextBackFill(UNFILL);
    lcd.setTextSize(FONT3);
    lcd.setTextColour(WHITE);    
    lcd.string(48,35,159,127,"PONG",0);
    lcd.setTextSize(FONT0);
    lcd.setTextColour(GREEN);    
    lcd.string(23,65,159,127,"Press button to begin",0);
    delay(1000);
    while(digitalRead(MOVELEFT) & digitalRead(MOVERIGHT)); //wait for a button to be pressed
    lcd.erase();
    //start the game
    pong();               // Play until game over
  }
}
