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

#define clockBodyCol  BLACK    //colour for the body clock
#define clockEdgeCol  GREEN    //colour for the edge of the clock
#define secCol        YELLOW   //seconds hand colour
#define minCol        MAGENTA  //minutes hand colour
#define hourCol       RED      //hours hand colour
#define halfx         80       //this point represent the x center of the clock where math is done
#define halfy         64       //this point represent the y center of the clock where math is done

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
}

void loop() { //main loop
  int secs=15;
  int mins=45;
  int hours=2;
  int xs,ys,xm,ym,xh,yh;
  int angleH,angleM,angleS;
  int handHour=35;//hand size
  int handMin=45;//hand size
  int handSec=48;//hand size  
  
  lcd.baudChange(2000000);                             //set high baud for fast drawing
  lcd.drawCircle(halfx,halfy,55,clockBodyCol,FILL);    //draw clock body
  lcd.drawCircle(halfx,halfy,55,clockEdgeCol,UNFILL);  //draw clock edge

  //String Conf
  lcd.setTextSize(FONT1);
  lcd.setTextColour(clockEdgeCol);
  
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
    
  lcd.putLetter(73,11,'1');  //Draw the 1                                 
  lcd.putLetter(81,11,'2');  //Draw the 2  
  
  //Draw current time hands  
  lcd.drawLine(halfx,halfy,halfx+xm,halfy-ym,minCol);  // Draw the minutes hand, first point is the center of the clock, and the second is the point obtained by doing math
  lcd.drawLine(halfx,halfy,halfx+xh,halfy-yh,hourCol); // Draw the hours hand, first point is the center of the clock, and the second is the point obtained by doing math
  lcd.drawLine(halfx,halfy,halfx+xs,halfy-ys,secCol);  // Draw the seconds hand, first point is the center of the clock, and the second is the point obtained by doing math
  lcd.drawCircle(halfx,halfy,2,secCol,FILL);           // Draw the center of the second's hand
  
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
  lcd.drawLine(halfx,halfy,halfx+xs,halfy-ys,clockBodyCol); // Erase Second's hand
  lcd.drawLine(halfx,halfy,halfx+xm,halfy-ym,clockBodyCol); // Erase Minute's hand
  lcd.drawLine(halfx,halfy,halfx+xh,halfy-yh,clockBodyCol); // Erase Hour's hand            
 }
}
