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

char imagesOnSDCard[11][9]={"tulips","penguins","abstract","nature","cube","sunset","drop","fractal","jelly","koala","flower"}; //array containing the names of the different called images

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
}

void loop() { //main loop
  char pic, reply;
  
  while(1){   //Loop forever in the slide show!
    reply=lcd.imageBMPSD(0,0,imagesOnSDCard[pic]); //Load image from SD card, all images are 160x128(full screen) so we load them from top left corner X:0,Y:0
    if(reply != 'O'){ //if not successfull
      lcd.string(10,20,LCDWIDTH-1,LCDHEIGHT-1,"ERROR on microSD card!",0);   
      while(1);
    }
    
    //Wait some seconds to change slide
    delay(2000);
    
    pic++;                               //increase image selector
    if(pic>10){                          //if we reach the position of the last image, we restart to image 0
      pic=0;                 
    }        
  }
}
