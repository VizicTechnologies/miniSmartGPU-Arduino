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

#define orange   0xFC0F
#define brown    0xBBCA
#define ligBlue  0x96DD

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
}

void loop() { //main loop
 
    //draw the house
    lcd.drawRectangle(38,58,112,126,orange,FILL);                    //draw a rectangle
    lcd.drawRectangle(38,58,112,126,BLACK,UNFILL);
    lcd.drawRectangle(45,75,68,93,ligBlue,FILL);
    lcd.drawRectangle(45,75,68,93,BLACK,UNFILL);
    lcd.drawLine(56,75,56,93,BLACK);
    lcd.drawLine(45,84,68,84,BLACK);    
    lcd.drawCircle(96,83,11,ligBlue,FILL);                           //draw a circle
    lcd.drawCircle(96,83,11,BLACK,UNFILL);    
    lcd.drawRectangle(68,100,89,126,brown,FILL);
    lcd.drawRectangle(68,100,89,126,BLACK,UNFILL);
    lcd.drawTriangle(33,59,76,22,117,59,brown,FILL);                 //draw a triangle         

    //draw trees
    lcd.drawRectangle(11,89,23,126,brown,FILL);
    lcd.drawRectangle(133,89,145,126,brown,FILL);
    lcd.drawCircle(139,75,15,GREEN,FILL);                            //draw a circle
    lcd.drawCircle(17,75,14,GREEN,FILL);                             //draw a circle
    
    //draw grass
    lcd.drawLine(0,127,159,127,GREEN);                               //draw a line    
      
    //loop forever
    while(1);
}
