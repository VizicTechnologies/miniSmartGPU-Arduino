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

void loop() { //main loop draw random colour and size triangles
    int x1,y1,x2,y2,x3,y3,colour,fill;

    lcd.baudChange(2000000);         //for fast drawing we need a big baudRate

    while(1){//forever
      x1=random(0,LCDWIDTH);         //get a random number 0-159
      y1=random(0,LCDHEIGHT);        //get a random number 0-127
      x2=random(0,LCDWIDTH);         //get a random number 0-159
      y2=random(0,LCDHEIGHT);        //get a random number 0-127       
      x3=random(0,LCDWIDTH);         //get a random number 0-159
      y3=random(0,LCDHEIGHT);        //get a random number 0-127             
      colour=random(0,65536);        //get a random number 0-65535
      fill=random(0,2);              //get a random number 0-1      
      
      //draw the triangle
      if(lcd.drawTriangle(x1,y1,x2,y2,x3,y3,colour,fill) != 'O'){ //draw random triangles
        while(1);                    //loop forever if different than 'O'--OK        
      }
    }
}
