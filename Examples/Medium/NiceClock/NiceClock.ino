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

#include <MINISMARTGPU.h>     //include the MINISMARTGPU library!

MINISMARTGPU lcd;             //create our object called LCD

SG_AXIS LCD_WIDTH, LCD_HEIGHT; //Variables to handle the screen resolution

#define secCol        SG_RED      //seconds hand colour
#define minCol        SG_BLACK    //minutes hand colour
#define hourCol       SG_BLACK    //hours hand colour
#define halfx         80       //this point represent the x center of the clock where math is done
#define halfy         64       //this point represent the y center of the clock where math is done


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
  int secs=0;
  int mins=48;
  int hours=4;
  int xs,ys,xm,ym,xh,yh;
  int angleH,angleM,angleS;
  int handHour=23;//hand size
  int handMin=26;//hand size
  int handSec=31;//hand size  
  
  lcd.baudChange(SG_BAUD6);                          //set high baud for fast drawing
  lcd.imageBMPSD(0,0,"oldClk");                     //draw the clock body
  
 while(1){
  //Do some Math to get the second point of the clock hands. (first point is always the center of the clock)
  angleS=secs*6;                           //get the current seconds in angle form, a circle have 360 degrees divided by 60 seconds = 6, then we multiply the 6 by the current seconds to get current angle
  xs=(sin((angleS*3.14)/180)) * handSec;   //get X component of the second's hand
  ys=(cos((angleS*3.14)/180)) * handSec;   //get Y component of the second's hand
  angleM=mins*6;                           //get the current minutes in angle form, a circle have 360 degrees divided by 60 minutes = 6, then we multiply the 6 by the current minutes to get current angle
  xm=(sin((angleM*3.14)/180)) * handMin;   //get X component of the minutes's hand
  ym=(cos((angleM*3.14)/180)) * handMin;   //get Y component of the minutes's hand 
  angleH=hours*30;                         //get the current hours in angle form, a circle have 360 degrees divided by 12 hours = 30, then we multiply the 30 by the current hours to get current angle
  xh=(sin((angleH*3.14)/180)) * handHour;  //get X component of the hours's hand
  yh=(cos((angleH*3.14)/180)) * handHour;  //get Y component of the hours's hand
    
  //Draw current time hands  
  lcd.drawLine(halfx,halfy,halfx+xm,halfy-ym,minCol);  // Draw the minutes hand, first point is the center of the clock, and the second is the point obtained by doing math
  lcd.drawLine(halfx,halfy,halfx+xh,halfy-yh,hourCol); // Draw the hours hand, first point is the center of the clock, and the second is the point obtained by doing math
  lcd.drawLine(halfx,halfy,halfx+xs,halfy-ys,secCol);  // Draw the seconds hand, first point is the center of the clock, and the second is the point obtained by doing math
  lcd.drawCircle(halfx,halfy,2,secCol,SG_FILL);           // Draw the center of the second's hand
  
  delay(1000);                                    // wait for one second delay (we dont need to explain why we're waiting one second, right?)

  secs++;                                         // increase seconds
  if(secs==60){                                   // if we reach 60 seconds
    mins++;                                       // increase the minutes
    if(mins==60){                                 // if we reach 60 minutes
      hours++;                                    // increase the minutes
      if(hours==12){                              // if we reach 12 hours
        hours=0;                                  // clear hours
      } 
      mins=0;                                     // clear minutes
    }            
    secs=0;                                       // clear seconds
  }                      
 
  //Erase all hands         
  lcd.drawLine(halfx,halfy,halfx+xs,halfy-ys,SG_WHITE); // Erase Second's hand
  lcd.drawLine(halfx,halfy,halfx+xm,halfy-ym,SG_WHITE); // Erase Minute's hand
  lcd.drawLine(halfx,halfy,halfx+xh,halfy-yh,SG_WHITE); // Erase Hour's hand            
 }
}
