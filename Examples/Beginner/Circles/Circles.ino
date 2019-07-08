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

void loop() { //main loop draw random colour, size and fill circles
    int x,y,radius,colour,fill;

    lcd.baudChange(2000000);         //for fast drawing we need a big baudRate
    
    while(1){//forever
      randomSeed(random(0,65536));   //set different seeds to obtain a good random number 
      x=random(0,LCDWIDTH);          //get a random number 0-159
      y=random(0,LCDHEIGHT);         //get a random number 0-127
      radius=random(1,30);           //get a random number 1-29
      colour=random(0,65536);        //get a random number 0-65535
      fill=random(0,2);              //get a random number 0-1
      
      //draw the circle
      if(lcd.drawCircle(x,y,radius,colour,fill) != 'O'){  //draw a circle
        while(1);                    //loop forever if different than 'O'--OK
      }       
    }
}
