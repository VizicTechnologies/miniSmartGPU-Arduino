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


/********************************************************
Connect the POTs to Analog input 0 and 1 (A0 and A1)
********************************************************/

//Defines for the Horizontal Slider
#define SLH_X_START       50  //Slider start left coordinate
#define SLH_Y_START       10  //Slider start top coordinate
#define SLH_WIDTH         80  //Slider width size in pixels
#define SLH_HEIGHT        30  //Slider height size in pixels

//Defines for the Vertical Slider
#define SLV_X_START       10  //Slider start left coordinate
#define SLV_Y_START       10  //Slider start top coordinate
#define SLV_WIDTH         30  //Slider width size in pixels
#define SLV_HEIGHT       100  //Slider height size in pixels


/**************************************************/
void updateHorizontalBar(unsigned int slValue){ //updates/draws a #defined horizontal bar with the received value from 0 - SLH_WIDTH
  if(slValue > SLH_WIDTH) slValue = SLH_WIDTH; //if received value is bigger, correct
  //draw the slider WHITE corner, we substract 1 and add 1 to make it a little bigger than the desired slider size
  lcd.drawRectangle(SLH_X_START -1, SLH_Y_START -1, SLH_X_START + SLH_WIDTH +1, SLH_Y_START + SLH_HEIGHT +1, WHITE, UNFILL);
  //erase the slider - BLACK
  lcd.drawRectangle(SLH_X_START + slValue, SLH_Y_START, SLH_X_START + SLH_WIDTH, SLH_Y_START + SLH_HEIGHT, BLACK, FILL);    
  //draw the current slider value - GREEN
  lcd.drawRectangle(SLH_X_START, SLH_Y_START, SLH_X_START + slValue, SLH_Y_START + SLH_HEIGHT, GREEN, FILL);       
}

/**************************************************/
void updateVerticalBar(unsigned int slValue){ //updates/draws a #defined vertical bar with the received value from 0 - SLV_HEIGHT
  if(slValue > SLV_HEIGHT) slValue = SLV_HEIGHT; //if received value is bigger, correct  
  //draw the slider WHITE corner, we substract 1 and add 1 to make it a little bigger than the desired slider size
  lcd.drawRectangle(SLV_X_START -1, SLV_Y_START -1, SLV_X_START + SLV_WIDTH +1, SLV_Y_START + SLV_HEIGHT +1, WHITE, UNFILL);
  //erase the slider - BLACK
  lcd.drawRectangle(SLV_X_START, SLV_Y_START, SLV_X_START + SLV_WIDTH, SLV_Y_START + SLV_HEIGHT - slValue, BLACK, FILL);  
  //draw the current slider value - YELLOW
  lcd.drawRectangle(SLV_X_START, SLV_Y_START + SLV_HEIGHT - slValue, SLV_X_START + SLV_WIDTH, SLV_Y_START + SLV_HEIGHT, YELLOW, FILL);      
}


/**************************************************/
/**************************************************/
//Initial setup
void setup() { //initial setup
  //Those two functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU processor
}


/**************************************************/
/****************** MAIN LOOP *********************/
/**************************************************/
/**************************************************/
void loop() { //main loop
  unsigned int horizontalSlideValue=0, verticalSlideValue=0;
  unsigned long aux=0;
  
  lcd.baudChange(1000000);         //set high baud for advanced applications
  
  //draw bars with initial values(zeros)
  updateHorizontalBar(0);
  updateVerticalBar(0);
  
  while(1){
    //read first pot value
    aux = analogRead(A0); 
    //aux has a value from 0 to 1023, we need to scale this value as the updateHorizontalBar accepts a value between 0-SLH_WIDTH
    horizontalSlideValue= ((aux*SLH_WIDTH)/1023);
    //send value to horizontal Slider
    updateHorizontalBar(horizontalSlideValue);
    delay(500); //update value each 500ms   
    //Perform something else with horizontalSlideValue      
    
    //read second pot value
     aux = analogRead(A1);
    //aux has a value from 0 to 1023, we need to scale this value as the updateVerticalBar accepts a value between 0-SLV_HEIGHT
    verticalSlideValue= ((aux*SLV_HEIGHT)/1023);     
    //send value to vertical Slider
    updateVerticalBar(verticalSlideValue);    
    delay(500); //update value each 500ms    
    //Perform something else with verticalSlideValue  
        
  }  
}

