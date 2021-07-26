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
 ARDUINO MINI SMARTGPU LIBRARY VERSION V4.0
 - Library supports MINISMARTGPU LCD160x128 connected via SmartSHIELD board
 IMPORTANT : This library is created for the Arduino 1.8.13 Software IDE or newer
********************************************************/

#include <Arduino.h> 

#include "MINISMARTGPU.h"

//Object creation to be used when software serial is needed (not used if SG_USE_HARDWARESERIAL is defined)
#ifndef SG_USE_HARDWARESERIAL	
	SoftwareSerial SG_SERIALPORT(SG_RX_PIN, SG_TX_PIN);
#endif	

/****************************************************************/
//Communication Functions(PLATFORM DEPENDENT) - MODIFY THE NEXT 
//FUNCTIONS TO FIT YOUR PLATFORM IF DIFFERENT THAN ARDUINO
//Note: MINISMARTGPU board default baud rate is 9600bps
/****************************************************************/
//Sends a single character through the serial port(USART)
static void putcharTX(char data){
	SG_SERIALPORT.write(data);             //Sends a single character through the serial port
}

//Returns a single character obtained from the serial port(USART)
static char getcharRX(){
	while(SG_SERIALPORT.available() == 0); //waits for a single character received by the serial port
	return SG_SERIALPORT.read();           //Returns the single received characted of the serial port
}

//Changes/Sets a new Baudrate to the Host processor
static void setBaudSmartGPU(unsigned long newBaud){
	SG_SERIALPORT.begin(newBaud);
}

//Performs a Hardware Reset on MINISMARTGPU RESET pin
static void resetSmartGPU(){         //Reset the MINISMARTGPU board
	digitalWrite(SG_RESET, LOW);       //Set the pin to GND to reset 
	delay(500);
	digitalWrite(SG_RESET, HIGH);      //Set the pin to 5v to end reset
	delay(500);	
}

//Arduino Exclusive function - Configures the Hardware
static void initSmartGPU(){          //configure the arduino board for MINISMARTGPU board support
	setBaudSmartGPU(9600);             //Default Arduino-MINISMARTGPU communication Baudrate is 9600
	pinMode(SG_RESET, OUTPUT);         //Configure pin as output
	digitalWrite(SG_RESET, HIGH);      //Set the pin to 5v to exit reset	
}


/****************************************************************/
/****************************************************************/ 
/****************************************************************/
//MiniSmartGPU Arduino Functions - DO NOT MODIFY DOWN FROM HERE
/****************************************************************/
/****************************************************************/
/****************************************************************/
MINISMARTGPU::MINISMARTGPU(){
}

SG_REPLY MINISMARTGPU::init(){        //Initialize serial port and reset pin
	initSmartGPU();
	return SG_OK;
}

SG_REPLY MINISMARTGPU::reset(){       //Physically Reset the MINISMARTGPU board
  resetSmartGPU();
  return SG_OK;
}

SG_REPLY MINISMARTGPU::start(){       //Init the MINISMARTGPU
  delay(500); 
  putcharTX('U');  
  delay(500);
	wid=160; hei=128;
  return (SG_REPLY)getcharRX();
}

/****************************************************************/
//Master Functions
/****************************************************************/
SG_REPLY MINISMARTGPU::erase(){       //Erase the MINISMARTGPU screen
  putcharTX('M');             //Master function
  putcharTX('E');  
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::sleep(SG_STATE state){       //Send MINISMARTGPU to sleep mode
  putcharTX('M');             //Master function
  putcharTX('Z'); 
  putcharTX(state);  
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::orientation(SG_LCDORIENTATION side){       //Change display orientation
  putcharTX('M');             //Master function
  putcharTX('O'); 
  putcharTX(side); 
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::bright(unsigned char val){       //Change display brightness
  putcharTX('M');             //Master function
  putcharTX('B'); 
  putcharTX(val);  
  return (SG_REPLY)getcharRX();
}

// mini SMART GPU DEFAULT/INITIAL BAUD RATE: 9600bps
SG_REPLY MINISMARTGPU::baudChange(SG_BAUDRATE val){       //Change baud rate of arduino and MINISMARTGPU board
  unsigned char aux;
 
  switch(val){
	case SG_BAUD0:
	  aux=0;
	break;
	case SG_BAUD1:
	  aux=1;
	break;
	case SG_BAUD2:
	  aux=2;
	break;
	case SG_BAUD3:
	  aux=3;
	break;
	case SG_BAUD4:
	  aux=4;
	break;	
	case SG_BAUD5:
	  aux=5;
	break;
	case SG_BAUD6:
	  aux=6;
	break;
	default:
	  return (SG_REPLY)'F';
	break;
  } 
  putcharTX('M');             //Master function    
  putcharTX('X');
  putcharTX(aux); 
  aux=getcharRX();
  if(aux=='O'){
	delay(150);
	setBaudSmartGPU((unsigned long)val); //Change arduino serial port speed
	delay(200); 
	return (SG_REPLY)getcharRX();
  }else{
	return (SG_REPLY)aux;
  }
}

SG_REPLY MINISMARTGPU::setEraseBackColour(SG_COLOUR colour){       //Change the default screen background colour for erase function
  putcharTX('M');             //Master function
  putcharTX('C');             //Background Colour
  putcharTX(colour>>8);
  putcharTX(colour); 
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::getWidth(SG_AXIS *w){
	*w = wid;
	return SG_OK;
}

SG_REPLY MINISMARTGPU::getHeight(SG_AXIS *h){
	*h = hei;
	return SG_OK;
}

/****************************************************************/
//Geometric Functions
/****************************************************************/
SG_REPLY MINISMARTGPU::putPixel(SG_AXIS x, SG_AXIS y, SG_COLOUR colour){       //Draw a pixel on the screen
  putcharTX('G');             //Geometric function  
  putcharTX('P'); 
  putcharTX(x);
  putcharTX(y);
  putcharTX(colour>>8);
  putcharTX(colour); 
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::drawLine(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, SG_COLOUR colour){       //Draw a line on the screen
  putcharTX('G');             //Geometric function 
  putcharTX('L'); 
  putcharTX(x1);
  putcharTX(y1);
  putcharTX(x2);
  putcharTX(y2);  
  putcharTX(colour>>8);
  putcharTX(colour);
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::drawRectangle(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, SG_COLOUR colour, SG_FILLGEOM fill){       //Draw a rectangle on the screen
  putcharTX('G');             //Geometric function 
  putcharTX('R'); 
  putcharTX(x1);
  putcharTX(y1);
  putcharTX(x2);
  putcharTX(y2);  
  putcharTX(colour>>8);
  putcharTX(colour);
  putcharTX(fill);    
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::drawTriangle(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, SG_AXIS x3, SG_AXIS y3, SG_COLOUR colour, SG_FILLGEOM fill){       //Draw a triangle on the screen
  putcharTX('G');             //Geometric function 
  putcharTX('T'); 
  putcharTX(x1);
  putcharTX(y1);
  putcharTX(x2);
  putcharTX(y2);  
  putcharTX(x3);
  putcharTX(y3);    
  putcharTX(colour>>8);
  putcharTX(colour);
  putcharTX(fill);  
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::drawCircle(SG_AXIS x, SG_AXIS y, SG_RADIUS radius, SG_COLOUR colour, SG_FILLGEOM fill){       //Draw a circle on the screen
  putcharTX('G');             //Geometric function 
  putcharTX('C');  
  putcharTX(x);
  putcharTX(y);
  putcharTX(radius);
  putcharTX(colour>>8);
  putcharTX(colour);
  putcharTX(fill);  
  return (SG_REPLY)getcharRX();
}


/****************************************************************/
//String Functions
/****************************************************************/
SG_REPLY MINISMARTGPU::putLetter(SG_AXIS x, SG_AXIS y, char letter){  //Draw a letter on the screen on X,Y coords
  putcharTX('S');             //String Function 
  putcharTX('L');             //Letter - a simple letter 
  putcharTX(x);
  putcharTX(y);
  putcharTX(letter); 
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::string(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, char text[], SG_NUMBEROFBYTES *SPB){    //Draw a string on the screen on defined Text Box coords, and stores the successfully printed bytes on SPB
  unsigned int counter=0, sp=0;  
  putcharTX('S');             //String Function 
  putcharTX('S');             //String  
  putcharTX(x1);
  putcharTX(y1); 
  putcharTX(x2);
  putcharTX(y2);   
  while(1){
	putcharTX(text[counter]);
    if(text[counter]==0x00){
      break;
	}	
	counter++;
  }    
  ((unsigned char*) &sp)[1]=getcharRX();
  ((unsigned char*) &sp)[0]=getcharRX(); 

  *SPB = sp;      
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::stringSD(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, SG_NUMBEROFBYTES BS, SG_NUMBEROFBYTES BR, SG_FILENAME name, SG_NUMBEROFBYTES *SPB){       //Draw a String from a text file stored on the micro SD card
  unsigned char counter=0, sp=0;
  
  putcharTX('S');             //String function 
  putcharTX('F');             //text File from SD 
  putcharTX(x1);
  putcharTX(y1);
  putcharTX(x2);
  putcharTX(y2);  
  putcharTX(BS>>8);
  putcharTX(BS);  
  putcharTX(BR>>8);
  putcharTX(BR);  
  while(1){
	putcharTX(name[counter]);
    if(name[counter]==0x00){
      break;
	}	
	counter++;
  }  
  ((unsigned char*) &sp)[1]=getcharRX();
  ((unsigned char*) &sp)[0]=getcharRX(); 

  *SPB = sp; 
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::setTextColour(SG_COLOUR colour){        //Set the default text colour for letters and strings
  putcharTX('S');             //String Function 
  putcharTX('C');             //Config  
  putcharTX('T');             //Text
  putcharTX(colour>>8);
  putcharTX(colour);  
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::setTextBackColour(SG_COLOUR colour){    //Set the default text background colour for letters and strings
  putcharTX('S');             //String Function 
  putcharTX('C');             //Config  
  putcharTX('B');             //Background
  putcharTX(colour>>8);
  putcharTX(colour);  
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::setTextSize(SG_FONTSIZE size){           //Set the default text size for letters and strings
  putcharTX('S');             //String Function 
  putcharTX('C');             //Config  
  putcharTX('S');             //Size
  putcharTX(size);  
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::setTextBackFill(SG_TEXTBACKGROUND fillState){  //Set the default text FILL or UNFILL background letters and strings
  putcharTX('S');             //String Function 
  putcharTX('C');             //Config  
  putcharTX('F');             //Fill
  putcharTX(fillState);   
  return (SG_REPLY)getcharRX();
}


/****************************************************************/
//Image Functions
/****************************************************************/
SG_REPLY MINISMARTGPU::drawIcon(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, char icon[]){            //Send and image or icon pixel by pixel to SMARTGPU, 16bit(2 bytes) each pixel RGB565
  unsigned int i,j,k=0; 
  
  putcharTX('I');             //Image function 
  putcharTX('I');             //Icon image received pixel by pixel
  putcharTX(x1);
  putcharTX(y1);
  putcharTX(x2);
  putcharTX(y2);
  
  //Send icon buffer pixel by pixel
  for(j=0;j<=(y2-y1);j++){
	for(i=0;i<=(x2-x1);i++){
		putcharTX(icon[k++]); //16bit per pixel - Upper 8bits part
		putcharTX(icon[k++]); //16bit per pixel - Lower 8bits part
	}
  }  
  return (SG_REPLY)getcharRX();  
}

SG_REPLY MINISMARTGPU::imageBMPSD(SG_AXIS x, SG_AXIS y, SG_FILENAME name){        //Draw an Image stored on the micro SD card on the screen, at X,Y top right corner coordinates
  unsigned char counter=0;
  
  putcharTX('I');             //Image function 
  putcharTX('B');             //BMP from SD card
  putcharTX(x);
  putcharTX(y);
  while(1){
	putcharTX(name[counter]);
    if(name[counter]==0x00){
      break;
	}	
	counter++;
  }
  return (SG_REPLY)getcharRX();
}

SG_REPLY MINISMARTGPU::getImageFromMemory(SG_AXIS x1, SG_AXIS y1, SG_AXIS x2, SG_AXIS y2, char buffer[]){ //Read the internal memory of the SMARTGPU, This command returns 24bit pixels (3 bytes)
  unsigned int i,j,k=0;
  
  putcharTX('I');             //Image function
  putcharTX('M');             //from SmartGPU internal Display Memory
  putcharTX(x1);
  putcharTX(y1);
  putcharTX(x2);
  putcharTX(y2); 

  //receive all the pixels
  for(j=0;j<=(y2-y1);j++){
	for(i=0;i<=(x2-x1);i++){
		buffer[k++]=getcharRX(); //Red
		buffer[k++]=getcharRX(); //Green		
		buffer[k++]=getcharRX(); //Blue
	}	
  }
  return (SG_REPLY)getcharRX();  
}


/****************************************************************/
//SD FAT management Functions
//Those next SDF - SD Functions return file execution status instead of ACK 'O' or NAK 'F'
/****************************************************************/
SG_FILERESULT MINISMARTGPU::SDFgetList(unsigned int *numOfFiles){ //get number of files
  SG_FILERESULT  res=SG_F_OK;
  unsigned int files=0;

  //delay(1);      
  putcharTX('F');           //File function - memory card file management
  putcharTX('L');           //List/count files
  ((unsigned char *) &files)[1]= getcharRX();      //Get Upper part
  ((unsigned char *) &files)[0]= getcharRX();      //Get Lower part  

  *numOfFiles=files;       
  
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status
}

SG_FILERESULT MINISMARTGPU::SDFgetFileName(SG_ITEMNUMBER itemNumber, SG_FILENAME name){ //searches for the "itemNumber" on the SD current folder and updates the buffer with the File name ended with NULL character
  SG_FILERESULT  res=SG_F_OK;
	unsigned int   i=0;

  putcharTX('F');           //File function - memory card file management
  putcharTX('G');           //Get name of given item file number
  putcharTX(itemNumber>>8); //Send Upper part of itemNumber
  putcharTX(itemNumber);    //Send Lower part of itemNumber
  
	
  while(1){
	name[i]=getcharRX(); 
	if(name[i]==0x00){         //if we find NULL character, means end of name
		break;   
	}
	i++;
  }
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status
 }
 
SG_FILERESULT MINISMARTGPU::SDFnewFile(SG_FILENAME name){ //create a new File, fails if already exist
  SG_FILERESULT res = SG_F_OK;
  unsigned int counter=0;  
 
  putcharTX('F');           //File function - memory card file management
  putcharTX('N');           //New

  while(1){
  	putcharTX(name[counter]);
  	if(name[counter]==0x00){
  	break;
  	}	
  	counter++;
  }
  
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status
}

SG_FILERESULT MINISMARTGPU::SDFopenFile(SG_FILENAME name, SG_OPENMODE mode){ //opens an existing file in READONLY, WRITEONLY or READWRITE mode
  SG_FILERESULT res = SG_F_OK;
  unsigned int counter=0;  
  
	putcharTX('F');           //File function - memory card file management
	putcharTX('O');           //Open file
	putcharTX(mode);          //Mode - READONLY,WRITEONLY,READWRITE
	while(1){
		putcharTX(name[counter]);
		if(name[counter]==0x00){
		break;
		}	
		counter++;
	}
 
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
	getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status
  
}

SG_FILERESULT MINISMARTGPU::SDFcloseFile(){ //close and save file object
  SG_FILERESULT res = SG_F_OK;

  //delay(50);
  putcharTX('F');           //File function - memory card file management
  putcharTX('C');           //Close File
 
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status
}

SG_FILERESULT MINISMARTGPU::SDFsaveFile(){ //sync/save file object
  SG_FILERESULT res = SG_F_OK;
  
  putcharTX('F');           //File function - memory card file management
  putcharTX('S');           //Save/Sync file - Save changes on file
 
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status 
}

SG_FILERESULT MINISMARTGPU::SDFsetFilePointer(SG_POINTERPOSITION pointerPosition){ // set/move file pointer of file object
  SG_FILERESULT res =SG_F_OK;
  
  putcharTX('F');           //File function - memory card file management
  putcharTX('P');           //Pointer position
  putcharTX('S');           //Set  
  putcharTX(pointerPosition>>24);
  putcharTX(pointerPosition>>16);
  putcharTX(pointerPosition>>8);
  putcharTX(pointerPosition);

  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status 
}	

SG_FILERESULT MINISMARTGPU::SDFgetFilePointer(SG_POINTERPOSITION *pointerPosition){ // get file pointer of file object
  SG_FILERESULT res = SG_F_OK;
  unsigned long pos = 0;
  
  putcharTX('F');           //File function - memory card file management
  putcharTX('P');           //Pointer position 
  putcharTX('G');           //Get
 
  ((unsigned char *) &pos)[3]=getcharRX();
  ((unsigned char *) &pos)[2]=getcharRX();
  ((unsigned char *) &pos)[1]=getcharRX();  
  ((unsigned char *) &pos)[0]=getcharRX(); 

  *pointerPosition =pos;
  
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status 
}	

SG_FILERESULT MINISMARTGPU::SDFreadFile(char buffer[], SG_NUMBEROFBYTES BTR, SG_NUMBEROFBYTES*SRB){ //Buffer, Bytes to Read, Succesfully Read Bytes
  SG_FILERESULT res = SG_F_OK;
  unsigned int x=0, sr=0;

  putcharTX('F');           //File function - memory card file management
  putcharTX('R');           //Read file
  putcharTX(BTR>>8);
  putcharTX(BTR);  

  for(x=0;x<BTR;x++){
		buffer[x]=getcharRX();
  }	
	
  ((unsigned char*) &sr)[1]=getcharRX();
  ((unsigned char*) &sr)[0]=getcharRX(); 

  *SRB = sr;                   //store succesfully read bytes
  
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status 
}	

SG_FILERESULT MINISMARTGPU::SDFwriteFile(char buffer[], SG_NUMBEROFBYTES BTW, SG_NUMBEROFBYTES *SWB){ //Buffer, Bytes to Write, Succesfully Written Bytes
  SG_FILERESULT res = SG_F_OK;
  unsigned int x=0, sw=0;

  if(BTW>512){
	*SWB=0;                   //Return 0 successfully written bytes
	return SG_F_INVALID_PARAMETER; //512 bytes is the max to write in a single call
  }
  
  //delay(10);  
  putcharTX('F');           //File function - memory card file management
  putcharTX('W');           //Write file
  putcharTX(BTW>>8);
  putcharTX(BTW);    
  for(x=0;x<BTW;x++){
	putcharTX(buffer[x]);
  }
  
  ((unsigned char*) &sw)[1]=getcharRX();
  ((unsigned char*) &sw)[0]=getcharRX();  

  *SWB = sw;                   //store succesfully written bytes   
  
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status 
}

SG_FILERESULT MINISMARTGPU::SDFtruncateFile(){  //truncates the file size to the current file read/write pointer
  SG_FILERESULT res=SG_F_OK;
  
  putcharTX('F');           //File function - memory card file management
  putcharTX('V');           //Truncate

  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status  
}

SG_FILERESULT MINISMARTGPU::SDFeraseFile(SG_FILENAME name){ //Erases an existing File
  SG_FILERESULT res = SG_F_OK;
  unsigned int counter=0;  
  
	putcharTX('F');           //File function - memory card file management
	putcharTX('E');           //Erase Dir File	
	while(1){
		putcharTX(name[counter]);
		if(name[counter]==0x00){
		break;
		}	
		counter++;
	}
 
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
	getcharRX();               //Discard 'O' or 'F'
    return res;                  //Return SD file execution status  
}

SG_FILERESULT MINISMARTGPU::SDFgetFileSize(SG_FILENAME name, unsigned long *fileSize){ //Get Size of an existing File
  SG_FILERESULT res = SG_F_OK;
  unsigned int counter=0;  
  unsigned long size=0;
  
	putcharTX('F');           //File function - memory card file management
	putcharTX('I');           //Info
	while(1){
		putcharTX(name[counter]);
		if(name[counter]==0x00){
		break;
		}	
		counter++;
	}	

  ((unsigned char *) &size)[3]=getcharRX();
  ((unsigned char *) &size)[2]=getcharRX();
  ((unsigned char *) &size)[1]=getcharRX();  
  ((unsigned char *) &size)[0]=getcharRX(); 
	
  *fileSize=size;
  
  res=(SG_FILERESULT)getcharRX(); //Get SD file execution status
  getcharRX();               //Discard 'O' or 'F'
  return res;                  //Return SD file execution status  
}
