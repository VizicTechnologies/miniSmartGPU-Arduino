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

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
}

void loop() { //main loop

    //config strings
    lcd.setTextColour(YELLOW);    
    lcd.setTextSize(FONT0);
    lcd.string(0,0,LCDWIDTH-1,LCDHEIGHT-1,"0123456789",0);                   //write a string on the screen
    delay(500);    
    lcd.setTextSize(FONT1);
    lcd.string(0,25,LCDWIDTH-1,LCDHEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(500);    
    lcd.setTextSize(FONT2);
    lcd.string(0,60,LCDWIDTH-1,LCDHEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(500);    
    lcd.setTextSize(FONT3);
    lcd.string(0,100,LCDWIDTH-1,LCDHEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(2000);  
    lcd.erase();
    
    //config strings
    lcd.setTextColour(GREEN);      
    lcd.setTextSize(FONT4);
    lcd.string(0,0,LCDWIDTH-1,LCDHEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(500);        
    lcd.setTextSize(FONT5);
    lcd.string(0,50,LCDWIDTH-1,LCDHEIGHT-1,"0123456789",0);                 //write a string on the screen
    delay(2000);  
    lcd.erase();                            
}
