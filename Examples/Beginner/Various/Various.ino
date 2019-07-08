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

char message[]="Hello World";

void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
}

void loop() { //main loop
    delay(1000);                                                                                     //wait some time 
    lcd.drawLine(50,50,120,100,WHITE);                                                               //draw a line
    delay(1000);
    lcd.drawRectangle(10,10,120,90,RED,UNFILL);                                                      //draw a rectangle
    delay(1000);
    lcd.drawCircle(80,60,50,GREEN,UNFILL);                                                           //draw a circle
    delay(1000);
    lcd.drawTriangle(15,15,80,120,100,70,BLUE,UNFILL);                                               //draw a triangle
    delay(1000);             
    lcd.setTextSize(FONT0);    
    lcd.string(10,15,145,120,"This is the string test for MINISMARTGPU lcd160x128",0);               //write a string on the screen
    delay(1000);    
    lcd.setTextSize(FONT2);
    lcd.string(10,60,149,127,message,0);                                                             //write the string previously created (char message[]="Hello World";)
    delay(1000);
    lcd.putLetter(80,80,'E');                                                                        //write a single letter 'E'
    delay(1000);
    lcd.putPixel(100,70,CYAN);                                                                       //draw a pixel
    delay(3000);
    lcd.imageBMPSD(0,0,"flower");                                                                    //call the image "flower.bmp" previously stored on the micro SD card
    delay(1000);    
    lcd.stringSD(5,20,149,117,BEGINNING,ALLCONTENTS,"text1",0);                                      //call the text file "text1.txt" previously stored on the micro SD card
    delay(1000);   
    lcd.drawRectangle(10,10,90,80,RED,FILL);                                                         //draw a rectangle
    delay(1000);
    lcd.drawCircle(90,70,40,GREEN,FILL);                                                             //draw a circle
    delay(1000);
    lcd.drawTriangle(15,15,110,50,90,70,BLUE,FILL);                                                  //draw a triangle 
    delay(3000);
    lcd.erase();                                                                                     //erase screen
}
