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

/********************************************************
 This simple sketch does the next:
 1.- list the files
 2.- print the number of files
 3.- print the names while they fit
********************************************************/

#include <MINISMARTGPU.h>     //include the MINISMARTGPU library!

MINISMARTGPU lcd;             //create our object called lcd

SG_AXIS LCD_WIDTH, LCD_HEIGHT; //Variables to handle the screen resolution

SG_FILERESULT res = SG_F_OK;      //create the variable that will store all MINISMARTGPU commands responses

unsigned int row=2;

//function that loops forever on error
void testError(){ //if the response is different than OK, print and loop forever
  if(res!=SG_F_OK){
    lcd.string(10,row,LCD_WIDTH-1,LCD_HEIGHT-1,"Error on microSD... forever loop@",0);
    while(1);  
  }
}


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
    char buffer[100]={0}; 
    unsigned int files=0, i=0;        
    
    //strings config
    lcd.setTextColour(SG_GREEN);    
    
    lcd.string(0,row,LCD_WIDTH-1,LCD_HEIGHT-1,"List Files-print names demo!",0); row+=10;      
    res=lcd.SDFgetList(&files);    //obtain files
    testError();
    lcd.string(10,row,LCD_WIDTH-1,LCD_HEIGHT-1,"-Files:",0);
    itoa(files,buffer,10);        //convert number to ascii
    lcd.string(50,row,LCD_WIDTH-1,LCD_HEIGHT-1,buffer,0);//print files number
    row+=10;   
    
    //print file names
    lcd.string(10,row,LCD_WIDTH-1,LCD_HEIGHT-1,"--------------------",0);             row+=10;
    for(i=0;i<files;i++){
      res=lcd.SDFgetFileName(i,buffer); //get Dir number i name in buffer
      testError();
      lcd.string(10,row,LCD_WIDTH-1,LCD_HEIGHT-1,buffer,0); row+=10;      //print the name if fit
    }    
    
    while(1); //loop forever    
}
