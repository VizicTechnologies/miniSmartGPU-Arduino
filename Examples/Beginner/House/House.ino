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

#define orange   0xFC0F
#define brown    0xBBCA
#define ligBlue  0x96DD

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
 
  //Obtain screen resolution
  lcd.getWidth(&LCD_WIDTH);
  lcd.getHeight(&LCD_HEIGHT);
}

void loop() { //main loop
 
    //draw the house
    lcd.drawRectangle(38,58,112,126,orange,SG_FILL);                    //draw a rectangle
    lcd.drawRectangle(38,58,112,126,SG_BLACK,SG_UNFILL);
    lcd.drawRectangle(45,75,68,93,ligBlue,SG_FILL);
    lcd.drawRectangle(45,75,68,93,SG_BLACK,SG_UNFILL);
    lcd.drawLine(56,75,56,93,SG_BLACK);
    lcd.drawLine(45,84,68,84,SG_BLACK);    
    lcd.drawCircle(96,83,11,ligBlue,SG_FILL);                           //draw a circle
    lcd.drawCircle(96,83,11,SG_BLACK,SG_UNFILL);    
    lcd.drawRectangle(68,100,89,126,brown,SG_FILL);
    lcd.drawRectangle(68,100,89,126,SG_BLACK,SG_UNFILL);
    lcd.drawTriangle(33,59,76,22,117,59,brown,SG_FILL);                 //draw a triangle         

    //draw trees
    lcd.drawRectangle(11,89,23,126,brown,SG_FILL);
    lcd.drawRectangle(133,89,145,126,brown,SG_FILL);
    lcd.drawCircle(139,75,15,SG_GREEN,SG_FILL);                            //draw a circle
    lcd.drawCircle(17,75,14,SG_GREEN,SG_FILL);                             //draw a circle
    
    //draw grass
    lcd.drawLine(0,127,159,127,SG_GREEN);                               //draw a line    
      
    //loop forever
    while(1);
}
