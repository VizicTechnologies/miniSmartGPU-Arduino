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

void loop() { //main loop draw random colour and size lines
    int x1,y1,x2,y2,colour;

    lcd.baudChange(2000000);         //for fast drawing we need a big baudRate

    while(1){//forever
      randomSeed(random(0,65536));   //set different seeds to obtain a good random number 
      x1=random(0,LCDWIDTH);         //get a random number 0-159
      y1=random(0,LCDHEIGHT);        //get a random number 0-127
      x2=random(0,LCDWIDTH);         //get a random number 0-159
      y2=random(0,LCDHEIGHT);        //get a random number 0-127    
      colour=random(0,65536);        //get a random number 0-65535
      
      //draw the line
      if(lcd.drawLine(x1,y1,x2,y2,colour) != 'O'){  //draw a line
        while(1);                    //loop forever if different than 'O'--OK
      }                   
    }
}
