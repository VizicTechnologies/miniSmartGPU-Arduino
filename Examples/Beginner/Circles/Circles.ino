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
void loop() { //main loop draw random colour, size and fill circles
    SG_POINT point;
    SG_RADIUS radius;
    SG_COLOUR colour;
    SG_FILLGEOM fill;
    
    lcd.baudChange(SG_BAUD6);        //For fast drawing we need a big baudRate. Note: if using Software Serial, use SG_BAUD2 or less!
    
    while(1){//forever
      randomSeed(random(0,65536));   //set different seeds to obtain a good random number 
      point.x=random(0,LCD_WIDTH);   //get a random number 0-319
      point.y=random(0,LCD_HEIGHT);  //get a random number 0-239
      radius=random(1,81);           //get a random number 1-80
      colour=random(0,65536);        //get a random number 0-65535
      fill=(SG_FILLGEOM)random(0,2); //get a random number 0-1
      
      //draw the circle
      if(lcd.drawCircle(point.x,point.y,radius,colour,fill) != SG_OK){  //draw a circle
        while(1){ delay(100);}       //loop forever if different than 'O'--OK
      }      
    }
}
