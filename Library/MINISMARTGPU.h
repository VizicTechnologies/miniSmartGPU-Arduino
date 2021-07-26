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

#ifndef MINISMARTGPU_h
#define MINISMARTGPU_h

#include <inttypes.h>
#include <SoftwareSerial.h>

/*******************USER MODIFABLE**********************
 ONLY MODIFY THE LINES THAT CONTAIN:"//----------------------------------------------------------------------------------------------"
*******************************************************/

//SOFTWARE OR HARDWARE SERIAL DEFINITIONS
//Uncomment next line to allow miniSmartGPU communicate via hardware serial ports, If this line is commented, then Software Serial will be used
#define SG_USE_HARDWARESERIAL        //----------------------------------------------------------------------------------------------


//IFs
#ifdef SG_USE_HARDWARESERIAL         //If the above line SG_USE_HARDWARESERIAL is uncommented:
	//Arduino hardware serial port configuration: modify to Serial, Serial2, Serial3 if needed
	#define SG_SERIALPORT (Serial)     //----------------------------------------------------------------------------------------------
#else                                //Use software serial, the next defined TX_PIN and RX_PIN will be used for miniSmartGPU communication
	#define SG_SERIALPORT softSerial   //Use software serial(don't modify this line)
	//TX and RX pin definitions: those are valid only if SG_USE_HARDWARESERIAL is commented!(Only if using software serial)
	//Software serial UART Transmit pin - Connect this pin to miniSmartGPU RX pin
	#define SG_TX_PIN     5            //----------------------------------------------------------------------------------------------
	//Software serial UART Receive pin - Connect this pin to miniSmartGPU TX pin
	#define SG_RX_PIN     2            //----------------------------------------------------------------------------------------------
#endif

//miniSmartGPU RESET PIN DEFINITION
//-SmartSHIELD uses default Arduino pin p4 for Reset, if another pin is soldered in the SmartSHIELD(pin 7, 8 or 13), please modify here:
//Define for the pin of arduino that will reset miniSmartGPU, this arduino pin is connected via the SmartSHIELD board(Default connected pin 4) to miniSmartGPU RESET pin
#define SG_RESET 4                   //----------------------------------------------------------------------------------------------         

//If DUE (SAM3X8E) is being used and miniSmartGPU is mounted as shield...
#if defined(__SAM3X8E__) //If arduino DUE is being used...
	#ifdef SG_USE_HARDWARESERIAL
	//Arduino DUE can't use hardware Serial or Serial1 for communication(only for debug via terminal), this because a bad design of arduino engineers, so redefine to Serial2 or Serial3
	#define SG_SERIALPORT   (Serial2)  //----------------------------------------------------------------------------------------------
	#endif
#endif
/****************END OF USER MODIFABLE******************/


/**************DON'T MODIFY UP FROM HERE****************/
//General definitions
#define SG_OFF 0
#define SG_ON  1
#define SG_GND 0
#define SG_VCC 1
#define SG_SCROLLBUTTONSIZE 25

//basic colours definitions
#define SG_BLACK   0x0000
#define SG_WHITE   0xFFFF
#define SG_RED     0xF800
#define SG_GREEN   0x07E0
#define SG_BLUE    0x001F
#define SG_YELLOW  0xFFE0
#define SG_CYAN    0x07FF
#define SG_MAGENTA 0xF81F

//File access definitions
#define SG_BEGINNING    0
#define SG_ALLCONTENTS  0

//General typedefs
typedef unsigned int  SG_AXIS;
typedef unsigned int  SG_COLOUR;
typedef unsigned int  SG_RADIUS;
typedef unsigned int  SG_NUMBEROFBYTES;
typedef unsigned int  SG_ITEMNUMBER;
typedef unsigned int  SG_ADDRESS;
typedef unsigned long SG_POINTERPOSITION;
typedef char SG_FILENAME[];

/*******ENUMS*********/
typedef enum {
	SG_DISABLE, //0
	SG_ENABLE   //1
} SG_STATE; //to set Enable/Disable States

typedef enum {
	SG_OK   = 'O',  //Command successfully executed
	SG_FAIL = 'F'   //Command Fail
} SG_REPLY;       //to get SmartGPU2 command responses

//Graphics functions definitions
typedef enum {
	SG_UNFILL, //0
	SG_FILL    //1
} SG_FILLGEOM;  //to set fill or unfill colour geometry

typedef enum {
	SG_HORIZONTAL, //0
	SG_VERTICAL    //1
} SG_ORIENTATIONPARAMETER;//to set gradient colour fade orientation and objects: scrollBar and Slider orientations

typedef enum {
	SG_LANDSCAPE_LEFT,     //0 left
	SG_PORTRAIT_LOW,      //1 low
	SG_LANDSCAPE_RIGHT,     //2 right
	SG_PORTRAIT_TOP       //3 top
} SG_LCDORIENTATION;  //to set LCD orientations

//fonts definitions
typedef enum {
	SG_FONT0 = 0,
	SG_FONT1,
	SG_FONT2,
	SG_FONT3,
	SG_FONT4,
	SG_FONT5,
	SG_FONT6,
	SG_FONT7,
	SG_FONT8,
	SG_FONT9,
	SG_FONT10,
	SG_FONT11,
	SG_FONT12,
	SG_FONT13
} SG_FONTSIZE; //to set text font sizes

typedef enum {
	SG_TRANS = 0,
	SG_FILLED
} SG_TEXTBACKGROUND; //to set text background colour to transparent or filled


//File access definitions
typedef enum {
	SG_READONLY = 1, //1
	SG_WRITEONLY,    //2
	SG_READWRITE     //3
} SG_OPENMODE;       //to set the file access open mode

//SMARTGPU2 Command Execution responses definitions
typedef enum {
	SG_F_OK = 0,				/* (0) Succeeded */
	SG_F_DISK_ERR,				/* (1) A hard error occurred in the low level disk I/O layer */
	SG_F_INT_ERR,				/* (2) Assertion failed */
	SG_F_NOT_READY,			/* (3) The physical drive cannot work */
	SG_F_NO_FILE,				/* (4) Could not find the file */
	SG_F_NO_PATH,				/* (5) Could not find the path */
	SG_F_INVALID_NAME,			/* (6) The path name format is invalid */
	SG_F_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	SG_F_EXIST,				/* (8) Access denied due to prohibited access */
	SG_F_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	SG_F_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	SG_F_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	SG_F_NOT_ENABLED,			/* (12) The volume has no work area */
	SG_F_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	SG_F_MKFS_ABORTED,			/* (14) The f_mkfs() aborted due to any parameter error */
	SG_F_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	SG_F_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	SG_F_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	SG_F_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
	SG_F_INVALID_PARAMETER		/* (19) Given parameter is invalid */
} SG_FILERESULT;               //Gets all FAT functions related responses


//Recommended(but not limited to) Arduino-SmartGPU Baud rate definitions
typedef enum{
	SG_BAUD0 = 9600,
	SG_BAUD1 = 19200,
	SG_BAUD2 = 57600,
	SG_BAUD3 = 115200,
	SG_BAUD4 = 256000,
	SG_BAUD5 = 500000,
	SG_BAUD6 = 1000000,
} SG_BAUDRATE;

/*******STRUCTS*********/
//Point
typedef struct {
	SG_AXIS x; //X axis
	SG_AXIS y; //Y axis
} SG_POINT;    //to create a point with point.x and point.y variables


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
class MINISMARTGPU{
	
public:
/****************************************************************/
//Arduino exclusive Functions
/****************************************************************/
  MINISMARTGPU();
	
	SG_REPLY init();

	SG_REPLY reset();
	
	SG_REPLY start();  

/****************************************************************/
//Master Functions
/****************************************************************/		
	SG_REPLY erase();
	
	SG_REPLY sleep(SG_STATE);
	
	SG_REPLY orientation(SG_LCDORIENTATION);
	
	SG_REPLY bright(unsigned char);
	
	SG_REPLY baudChange(SG_BAUDRATE);
	
	SG_REPLY setEraseBackColour(SG_COLOUR);
	
	SG_REPLY getWidth(SG_AXIS*);

  SG_REPLY getHeight(SG_AXIS*);
	
/****************************************************************/
//Geometric Functions
/****************************************************************/		
	SG_REPLY putPixel(SG_AXIS, SG_AXIS, SG_COLOUR);
	
	SG_REPLY drawLine(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, SG_COLOUR);
	
	SG_REPLY drawRectangle(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, SG_COLOUR, SG_FILLGEOM);
	
	SG_REPLY drawTriangle(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, SG_COLOUR, SG_FILLGEOM);
	
	SG_REPLY drawCircle(SG_AXIS, SG_AXIS, SG_RADIUS, SG_COLOUR, SG_FILLGEOM);
	
/****************************************************************/
//String Functions
/****************************************************************/		
	SG_REPLY putLetter(SG_AXIS, SG_AXIS, char);

	SG_REPLY string(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, char[], SG_NUMBEROFBYTES*); //returns in SG_NUMBEROFBYTES the successfully printed chars or letters
	
	SG_REPLY stringSD(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, SG_NUMBEROFBYTES, SG_NUMBEROFBYTES, SG_FILENAME, SG_NUMBEROFBYTES*); //returns in SG_NUMBEROFBYTES the successfully printed chars or letters
	
	SG_REPLY setTextColour(SG_COLOUR);
	
	SG_REPLY setTextBackColour(SG_COLOUR);

	SG_REPLY setTextSize(SG_FONTSIZE);

	SG_REPLY setTextBackFill(SG_TEXTBACKGROUND);
	
/****************************************************************/
//Image Functions
/****************************************************************/		
	SG_REPLY drawIcon(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, char[]);
	
	SG_REPLY imageBMPSD(SG_AXIS, SG_AXIS, SG_FILENAME);

	SG_REPLY getImageFromMemory(SG_AXIS, SG_AXIS, SG_AXIS, SG_AXIS, char[]); //Read the internal memory of the SMARTGPU2, This command returns 24bit pixels (3 bytes)
	

/****************************************************************/
//SD FAT management Functions
//Those next SDF - SD Functions return file execution status(SG_FILERESULT) instead of ACK 'O' or NAK 'F'(SG_REPLY)
/***************************************************************/
	SG_FILERESULT SDFgetList(unsigned int*); //get number of dirs and files

	SG_FILERESULT SDFgetFileName(SG_ITEMNUMBER, SG_FILENAME); //searches for the "itemNumber" on the SD current folder and updates the buffer with the File name ended with NULL character

	SG_FILERESULT SDFnewFile(SG_FILENAME); //create a new File, fails if already exist

	SG_FILERESULT SDFopenFile(SG_FILENAME, SG_OPENMODE); //opens an existing file in READONLY, WRITEONLY or READWRITE mode

	SG_FILERESULT SDFcloseFile(); //close and save file object
 
	SG_FILERESULT SDFsaveFile(); //sync/save file object

	SG_FILERESULT SDFsetFilePointer(SG_POINTERPOSITION); // set/move file pointer of file object

	SG_FILERESULT SDFgetFilePointer(SG_POINTERPOSITION*); // get file pointer of file object

	SG_FILERESULT SDFreadFile(char[], SG_NUMBEROFBYTES, SG_NUMBEROFBYTES*); //Bytes to Read, Succesfully Read Bytes

	SG_FILERESULT SDFwriteFile(char[], SG_NUMBEROFBYTES, SG_NUMBEROFBYTES*); //Bytes to Write, Succesfully Written Bytes

	SG_FILERESULT SDFtruncateFile();  //truncates the file size to the current file read/write pointer of the file

	SG_FILERESULT SDFeraseFile(SG_FILENAME); //Erases an existing File

	SG_FILERESULT SDFgetFileSize(SG_FILENAME, unsigned long *); //Get Size of an existing File

private:

	SG_AXIS wid;
	SG_AXIS hei;

};

#endif
