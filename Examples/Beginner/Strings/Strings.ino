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

    //config strings
    lcd.setTextColour(SG_YELLOW);    
    lcd.setTextSize(SG_FONT0);
    lcd.string(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,"0123456789",0);                   //write a string on the screen
    delay(500);    
    lcd.setTextSize(SG_FONT1);
    lcd.string(0,25,LCD_WIDTH-1,LCD_HEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(500);    
    lcd.setTextSize(SG_FONT2);
    lcd.string(0,60,LCD_WIDTH-1,LCD_HEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(500);    
    lcd.setTextSize(SG_FONT3);
    lcd.string(0,100,LCD_WIDTH-1,LCD_HEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(2000);  
    lcd.erase();
    
    //config strings
    lcd.setTextColour(SG_GREEN);      
    lcd.setTextSize(SG_FONT4);
    lcd.string(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,"0123456789",0);                  //write a string on the screen
    delay(500);        
    lcd.setTextSize(SG_FONT5);
    lcd.string(0,50,LCD_WIDTH-1,LCD_HEIGHT-1,"0123456789",0);                 //write a string on the screen
    delay(2000);  
    lcd.erase();                            
}
