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
//  Shape Descriptions
//        ROW        SQUARE     TEE        ESS        _2        QUE         PEE
//
//       [0](1)[2][3]  [0](1)   [0](1)[2]    (1)[0]  [3][2]     [2](1)[0]    [0](1)[2]
//                     [2][3]      [3]    [3][2]        (1)[0]  [3]                [3]
/********************************************************
 IMPORTANT : This Example is created for the Arduino 1.0 Software IDE
********************************************************/

#include <MINISMARTGPU.h>     //include the MINISMARTGPU library!

MINISMARTGPU lcd;             //create our object called LCD

SG_AXIS LCD_WIDTH, LCD_HEIGHT; //Variables to handle the screen resolution

#define WIDTHPORTRAIT  LCD_HEIGHT
#define HEIGHTPORTRAIT LCD_WIDTH

//Connect 5 push buttons to play, buttons must be connected to pin 8,9,10,11must reach GND when prESSed.
//defines for controls
#define ROTATE 8         //control input pin
#define DOWN   9         //control input pin
#define ENTER 10         //control input pin
#define LEFT  11         //control input pin
#define RIGHT 12         //control input pin

//General Defines
#define LEVEL_INCREMENTOR    3      //number of lines that pass before the level increments
#define MAX_LEVELS          10      //maximum number of levels in this game
//Define number of blocks in the screen
#define  TET_MAX_BLOCKS_X   10
#define  TET_MAX_BLOCKS_Y   18
#define  TET_MAX_BLOCKS    180

//Background colour
#define  BACKGROUND       0x841F

//type
#define SQUARE 1
#define ROW    2
#define TEE    3
#define ESS    4
#define TWO    5
#define QUE    6
#define PEE    7

//orient
#define  DEG0   0
#define  DEG90  90
#define  DEG180 180
#define  DEG270 270
#define  PASS    1
#define  FAIL    0

// This is a matrix that contains info about which blocks are filled
int screen[TET_MAX_BLOCKS]={0}; // colors
int lines[4]={0};

//global variables
int tetCompleteLines=0,length=0,tetGameOn=0,tetHighScore=0, currentShapeIndex=0,diffLevel=1;
int type[3]={0}; // t_type
int clr[3]={0}; // t_colors
int origin_x[3]={0};
int origin_y[3]={0};
int offsets_x[12]={0}; // 3 * 4
int offsets_y[12]={0};
int orient[3]={0}; // t_orient

//For game time settings
int overFlowFlag=0;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Timer Overflow interrupt routine
////////////////////////////////////////////////////////////////////////////////////////////////////
ISR(TIMER1_OVF_vect){       
  overFlowFlag=1;
}

// Update the offsets of a given shape
/**************************************************************************************************/
void update_offsets (int s){//t_shape *s
    int block,x,y,i,co,si;
    float aux;
    
    i = s * 4;
    
    switch(type[s]){
      case SQUARE:     
        offsets_x[i] = -1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] = -1;   offsets_y[i++] = -1;
        offsets_x[i] =  0;   offsets_y[i++] = -1;
      break;
      case ROW:
        offsets_x[i] = -1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] =  1;   offsets_y[i++] =  0;
        offsets_x[i] =  2;   offsets_y[i++] =  0;
      break;
      case TEE:
        offsets_x[i] = -1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] =  1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] = -1;
      break;
      case ESS:
        offsets_x[i] =  1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] = -1;
        offsets_x[i] = -1;   offsets_y[i++] = -1;
      break;
      case TWO:
        offsets_x[i] =  1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  1;
        offsets_x[i] = -1;   offsets_y[i++] =  1;
      break;
      case QUE:
        offsets_x[i] =  1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] = -1;   offsets_y[i++] =  0;
        offsets_x[i] = -1;   offsets_y[i++] = -1;
      break;
      case PEE:
        offsets_x[i] = -1;   offsets_y[i++] =  0;
        offsets_x[i] =  0;   offsets_y[i++] =  0;
        offsets_x[i] =  1;   offsets_y[i++] =  0;
        offsets_x[i] =  1;   offsets_y[i++] = -1;
      break;
      default:
      break;
    }

    block = 0;
    while (block < 4){
        x = offsets_x[(s * 4) + block];
        y = offsets_y[(s * 4) + block];
      
        aux= orient[s];
        aux= (aux*3.14159)/180;
        co  = cos(aux)*127.0;
        si  = sin(aux)*127.0;  

        offsets_x[(s * 4) + block] = ((x * co) - (y * si)) / 127;
        offsets_y[(s * 4) + block] = ((x * si) + (y * co)) / 127;        

        block++;
    }
}

// move_to_next_shape - increments currentShapeIndex
/**************************************************************************************************/
void move_to_next_shape(){
    if (currentShapeIndex < 2){
        currentShapeIndex++;
    }else{
        currentShapeIndex = 0;
    }
}

// get previous shape 
/**************************************************************************************************/
int get_previous_shape_index(){
    int result;
    if (currentShapeIndex < 1){
        result = 2;
    }else{
        result = currentShapeIndex - 1;
    }
    return result;
}

// get next shape
/**************************************************************************************************/
int get_next_shape_index(){
    int result;
    if (currentShapeIndex > 1){
        result = 0;
    }else{
        result = currentShapeIndex + 1;
    }
    return result;
}

// shape_can_move - Checks whether a shape can move in a given direction - If so the origin is updated and the result returned as PASS - If not origin is left al_1 and FAIL is returned
/**************************************************************************************************/
int shape_can_move (int s, int dir_x, int dir_y){
    int original_origin_x, original_origin_y, shape_pos_x, shape_pos_y, pass, block, p;//t_co_ord
    int result;
    
    pass=1;
    original_origin_x = origin_x[s];
    original_origin_y = origin_y[s];
    
    origin_x[s] = origin_x[s] + dir_x;
    origin_y[s] = origin_y[s] + dir_y;
    block = 0;
    
    while(block<4){
        shape_pos_x = origin_x[s] + offsets_x[(s * 4) + block];
        shape_pos_y = origin_y[s] + offsets_y[(s * 4) + block];
        // check for out of X/Y locations
        if ((shape_pos_x < 0) || (shape_pos_x >= TET_MAX_BLOCKS_X)  || (shape_pos_y < 0) || (shape_pos_y >= TET_MAX_BLOCKS_Y)){
            pass = 0;
        // check if something is there already
        }else if (screen[shape_pos_x + (TET_MAX_BLOCKS_X * shape_pos_y)] != SG_BLACK){
            pass = 0;
        }
        if (pass == 0){
            break;
        }
        block++;
    }

    if (pass == 1){
        // update the value of previous shape if we can move
        p = get_previous_shape_index();
        type[p]         = type[s];
        clr[p]          = clr[s];
        block = 0;
        while (block < 4){
            offsets_x[(p * 4) + block] = offsets_x[(s * 4) + block];
            offsets_y[(p * 4) + block] = offsets_y[(s * 4) + block];
            block++;
        }
        orient[p]            = orient[s];
        origin_x[p] = original_origin_x;
        origin_y[p] = original_origin_y;
        result = PASS;
    }else{
        // restore origin & return
        origin_x[s] = original_origin_x;
        origin_y[s] = original_origin_y;
        result = FAIL;
    }
    return result;
}

// rotate_shape -  Changes orientation by +90 degrees
/**************************************************************************************************/
void rotate_shape(int s){
    if (orient[s] == DEG270){
        orient[s] = DEG0;
    }else{
        orient[s] = orient[s] + DEG90;
    }
}

// shape_can_rotate - Checks whether a shape be rotated - If so the orientation is updated and the result returned as PASS - If not orientation is left al_1 and FAIL is returned
/**************************************************************************************************/
int shape_can_rotate (int s){
    int shape_pos_x, shape_pos_y, pass, result, block, p,original_orient;
    
    pass = 1;
    // save the orientation
    original_orient = orient[s];
    // rotate the shape
    rotate_shape (s);
    update_offsets (s);//t_shape *s

    block = 0;
    while (block<4){
        shape_pos_x = origin_x[s] + offsets_x[(s * 4) + block];
        shape_pos_y = origin_y[s] + offsets_y[(s * 4) + block];
        // check for out of X/Y locations
        if ((shape_pos_x < 0) || (shape_pos_x >= TET_MAX_BLOCKS_X -1) ||
            (shape_pos_y < 0) || (shape_pos_y >= TET_MAX_BLOCKS_Y)){
                pass = 0;
        // check if something is there already
        }else if (screen[shape_pos_x + (TET_MAX_BLOCKS_X * shape_pos_y)] != SG_BLACK){
                pass = 0;
        }
        if (pass == 0){
            break;
        }
        block++;
    }

    if (pass == 1){
        // update the value of previous shape if we can move
        p = get_previous_shape_index();
        type[p]       = type[s];
        clr[p]        = clr[s];
        origin_x[p]   = origin_x[s];
        origin_y[p]   = origin_y[s];
        orient[p]     = original_orient;
        update_offsets (p);
        result = PASS;
    }else{
        // restore orientation and return
        orient[s] = original_orient;
        update_offsets (s);//t_shape *s
        result = FAIL;
    }
    return result;
}

// Adds the shape to the screen matrix
/**************************************************************************************************/
void add_to_screen (int s){
    int shape_pos_x, shape_pos_y, pass, block;//t_co_ord

    pass = 1;
    block = 0;
    while (block<4){
        shape_pos_x = origin_x[s] + offsets_x[(s * 4) + block];
        shape_pos_y = origin_y[s] + offsets_y[(s * 4) + block];
        screen[shape_pos_x + (TET_MAX_BLOCKS_X * shape_pos_y)] = clr[s];
        block++;
    }
}

// removes the shape from the screen matrix
/**************************************************************************************************/
void remove_from_screen (int s){
    int shape_pos_x, shape_pos_y, pass, block;//t_co_ord

    pass = 1;
    block = 0;
    while (block<4){
        shape_pos_x = origin_x[s] + offsets_x[(s * 4) + block];
        shape_pos_y = origin_y[s] + offsets_y[(s * 4) + block];
        screen[shape_pos_x + (TET_MAX_BLOCKS_X * shape_pos_y)] = SG_BLACK;
        block++;
    }
}

// find_full_lines - Finds up to 4 full lines ... returns the number of full lines found.  The array of lines is - updated with the lines found in the lines array passed to this function
/**************************************************************************************************/
int find_full_lines (int *lines){
  int found_lines, x, line;

    found_lines = 0;
    line = TET_MAX_BLOCKS_Y - 1;
    
    while (line >= 0){
        x = 0;
        while (x<TET_MAX_BLOCKS_X){
            if (screen[x + (TET_MAX_BLOCKS_X * line)] == SG_BLACK){
                break;  // found empty cell, go to next line
            }
            x++;
        }
        if (x == TET_MAX_BLOCKS_X){
            lines[found_lines] = line;
            found_lines ++;
            if (found_lines == 4){
                break;  // find a max of 4 lines
            }
        }
        line--;
    }
    tetCompleteLines = tetCompleteLines + found_lines;
    return found_lines;
}

// remove_line_from_screen - Removes a given line from the screen
/**************************************************************************************************/
void remove_line_from_screen(int rmline){
    int found_lines, x, line;

    found_lines = 0;
    line = rmline - 1;
    
    while (line >= 0){
        x = 0;
        while (x < TET_MAX_BLOCKS_X){
            screen[x + (TET_MAX_BLOCKS_X * (line + 1))] = screen[x + (TET_MAX_BLOCKS_X * line)];
            x++;
        }
        line--;
    }
    x = 0;
    while (x < TET_MAX_BLOCKS_X){
        screen[x + (TET_MAX_BLOCKS_X * 0)] = SG_BLACK;
        x++;
    }
}

// get_new_shape - Generates the next shape
/**************************************************************************************************/
void get_new_shape(int s){
  int rnum;
  static char previousShape=0;

  origin_x[s] = 5;  
  orient[s] = DEG0;
  origin_y[s] = 1;  
  
  do{
    rnum = random(1,8); //from shape 0 to 7  
  }while(rnum==previousShape); //this will avoid shape repetition
 
  previousShape=rnum; //update value for next cycle
  type[s] =  rnum;
  
  switch(rnum){ //get shape colour
    case SQUARE:  
      clr[s] = SG_WHITE;      
    break;
    case ROW:  
      clr[s] = SG_CYAN;      
    break;
    case TEE:  
      clr[s] = SG_RED;      
    break;
    case ESS:  
      clr[s] = SG_BLUE;      
    break;
    case TWO:  
      clr[s] = SG_YELLOW;      
    break;
    case QUE:  
      clr[s] = SG_GREEN;      
    break;
    case PEE:  
      clr[s] = SG_MAGENTA;      
    break;
    default:  
    break;      
  }
  update_offsets (s);
}

// update_level
/**************************************************************************************************/
int update_level(){
   int next_level, result; 
 
    next_level = tetCompleteLines / LEVEL_INCREMENTOR;
    // only update if we are incrementing the level ... user may have started at level 5 or so
    if ((next_level > diffLevel) && (next_level <= MAX_LEVELS)){
        diffLevel = next_level;
        result = 1;
    }
    return result;
}

// new_game - Initializes things for a new game
/**************************************************************************************************/
void new_game (int level){
    int x=0, y;// int
    
    tetGameOn = 0;
    diffLevel = 1;
    tetHighScore = 0;
    tetCompleteLines = 0;
    tetGameOn = 1;

    // clear the screen
    lcd.drawRectangle(0, 0, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, BACKGROUND, SG_FILL);
    while (x < TET_MAX_BLOCKS){
        screen[x] = SG_BLACK;
        x++;
    }
    // set up the level & timeouts
    diffLevel = level;
    tetCompleteLines = 0;
}

// game_over
/**************************************************************************************************/
void game_over(){
    int  x, y, x1, x2, y1, y2, xs, ys;

    tetGameOn = 0;
    lcd.setTextColour(SG_WHITE);    
    lcd.setTextSize(SG_FONT2);
    lcd.setTextBackColour(SG_BLACK);
    lcd.setTextBackFill(SG_FILLED);    
    lcd.string(5, 70, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "GAME OVER!",0);    
}

// get input - check if any button is pressed, if so return value, if not returns 0
/**************************************************************************************************/
int get_input(){

    if (digitalRead(ENTER)==0){
        return ENTER; 
    }else if (digitalRead(ROTATE)==0){
        return ROTATE;
    }else if (digitalRead(DOWN)==0){
        return DOWN;
    }else if (digitalRead(LEFT)==0){
        return LEFT;
    }else if (digitalRead(RIGHT)==0){
        return RIGHT;
    }else{
        return 0; 
    }
}

// draw_block - draws a block onto the screen
/**************************************************************************************************/
void draw_block(int x, int y, int clr){
    lcd.drawRectangle(1 + (x * length), y * length, (x + 1) * length, (y + 1) * length, clr, SG_FILL);
}

// draw_shape - AC-draws a shape onto the next portion of the screen
/**************************************************************************************************/
void draw_shape(int s){
    int x, y, max_i, max_j, i, j, block;
    
    x = (TET_MAX_BLOCKS_X + 2) * length;
    y = (TET_MAX_BLOCKS_Y - 6) * length;
    max_i = WIDTHPORTRAIT/length;
    max_j = TET_MAX_BLOCKS_Y;
    i = max_i - 6;
    
    while (i< max_i){
        j = max_j - 6;
        while (j < max_j){
            draw_block(i, j, SG_BLACK);
            j++;
        }
    i++;
    }
    block = 0;
    while (block < 4){
        draw_block(max_i - 4 + offsets_x[(s * 4) + block], max_j - 4 + offsets_y[(s * 4) + block], clr[s]);
        block++;
    }
}

// draw_screen - draws the screen
/**************************************************************************************************/
void draw_screen(){
    int i, j;
    char score[4]={0};    

    i = 0;
    while (i < TET_MAX_BLOCKS_X){
        j = 0;
        while (j < TET_MAX_BLOCKS_Y){
            draw_block(i, j, screen[i + (TET_MAX_BLOCKS_X * j)]);
            j++;
        }
        i++;
    }

    lcd.setTextColour(SG_BLACK);    
    lcd.setTextSize(SG_FONT0);
    lcd.setTextBackColour(SG_BLACK);
    lcd.setTextBackFill(SG_FILLED);  
    lcd.setTextBackColour(BACKGROUND);//txt_Set(HIGHLIGHT_COLOUR, background);    
    lcd.string(85, 10, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1,"SCORE",0);
    itoa(tetCompleteLines,score,10); //convert points to ascii
    lcd.string(85, 20, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, score,0);  
    
    lcd.string(85, 40, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1,"LEVEL",0);
    itoa(diffLevel,score,10); //convert difficult to ascii
    lcd.string(85, 50, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, score,0);    
    lcd.string(85, 84, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1,"NEXT",0);      
}

// Is a block touching the top of the screen?
/**************************************************************************************************/
int touching_top(){
    int i=0, j=0;
    
    while (i < TET_MAX_BLOCKS_X){ //*2
        if (screen[i + (TET_MAX_BLOCKS_X * j)] != SG_BLACK){
            return 1;
        }
        i++;
    }
    return 0;
}

// Draw the welcome screen
/**************************************************************************************************/
void welcome_screen_160(){
    int title_offset;

    // clear the screen
    lcd.erase(); 
    lcd.setTextColour(SG_WHITE);    
    lcd.setTextSize(SG_FONT0);
    lcd.setTextBackColour(SG_BLACK);
    lcd.setTextBackFill(SG_FILLED);     
    lcd.string(0, 0, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "*************************",0);        
    lcd.string(10,20, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "Vizic Technologies",0);    
    lcd.setTextSize(SG_FONT3);    
    lcd.string(30, 55, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "TETRIS",0);  
    lcd.setTextSize(SG_FONT1);    
    lcd.string(15,100, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "Mini SmartGPU",0);    
    lcd.string(50,120, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "2013",0);    
    lcd.setTextSize(SG_FONT0);        
    lcd.string(0, 147, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "*************************",0);           
    while(get_input()==0);                      //Wait for button to be prESS
}

// Input loop to adjust game settings
/**************************************************************************************************/
void settings_loop(){
    int dirx1=0, diry1=0, rotate1=0, inputControl=0;
    
    lcd.erase();                 //erase the screen
    lcd.setTextColour(SG_WHITE);    
    lcd.setTextSize(SG_FONT2);
    lcd.setTextBackColour(SG_BLACK);
    lcd.setTextBackFill(SG_FILLED);     
    lcd.string(30,20,WIDTHPORTRAIT-1,HEIGHTPORTRAIT-1,"LEVEL:",0);
    lcd.setTextSize(SG_FONT0);    
    lcd.string(13,50,  WIDTHPORTRAIT-1,HEIGHTPORTRAIT-1,"Press LEFT/RIGHT",0);
    lcd.string(12, 95, WIDTHPORTRAIT-1,HEIGHTPORTRAIT-1,"HELP:Press ENTER",0);    
    lcd.string( 8,120, WIDTHPORTRAIT-1,HEIGHTPORTRAIT-1,"EXIT:ROTATE/DOWN",0);    

    lcd.setTextSize(SG_FONT2);    
    while(1){
        // LEVEL
        lcd.drawRectangle(95,20,120,40,SG_BLACK,SG_FILL); //erase previous number
        lcd.putLetter(95, 20, diffLevel + 0x30);    //print diff level converted to ascii by adding 0x30

        delay(200);        
        while((inputControl=get_input()) == 0); //wait for a button prESS
        
        switch(inputControl){
          case LEFT:
           diffLevel--;
           if(diffLevel<1) diffLevel=1;          
          break;
        
          case RIGHT:
           diffLevel++;
           if(diffLevel>9) diffLevel=9;          
          break;
        
          default:        
          break;            
        }
        if(inputControl==ENTER) help(); //help
        if(inputControl==ROTATE || inputControl==DOWN) break; //Exit
    }
}

// Draw the help screen
/**************************************************************************************************/
void help(){   
    // clear the screen
    lcd.erase();    
    lcd.setTextColour(SG_WHITE);    
    lcd.setTextSize(SG_FONT0);
    lcd.setTextBackColour(SG_BLACK);
    lcd.setTextBackFill(SG_FILLED);      
    lcd.string(0, 0, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1,  "HELP:",0);
    lcd.string(0, 20, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "ROTATE - rotate",0);
    lcd.string(0, 40, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "DOWN   - move down",0);    
    lcd.string(0, 60, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "LEFT    - move left",0);
    lcd.string(0, 80, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, "RIGHT   - move right",0);
    lcd.string(0, 100, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1,"ENTER   - help",0);    

    delay(1000);  
    while(get_input() == 0); //wait for a button press to exit   
    settings_loop();         //return to settings loop      
}



/**************************************************************************************************/
/**************************************************************************************************/
void setup() { //initial setup
  //Those TWO functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU procESSor

  //Obtain screen resolution
  lcd.getWidth(&LCD_WIDTH);
  lcd.getHeight(&LCD_HEIGHT);
  
  //configure input controls
  pinMode(ENTER, INPUT_PULLUP);          // set pin to input (buttons)
  pinMode(ROTATE,INPUT_PULLUP);          // set pin to input (buttons)
  pinMode(LEFT,  INPUT_PULLUP);          // set pin to input (buttons)
  pinMode(RIGHT, INPUT_PULLUP);          // set pin to input (buttons)  
  pinMode(DOWN,  INPUT_PULLUP);          // set pin to input (buttons)    
  
  // Timer/Counter 1 initialization
  // Clock value: 15.625 kHz
  TCCR1A=0x00;
  TCCR1B=0x05;
  // Timer/Counter 1 interrupt(s) initialization
  TIMSK1=0x01;
  sei();  //Enable interrupts (timer)
}

// main method
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
void loop() { //main loop
    int touched_bottom, dirx, diry, rotate; 
    int can_move_hor, can_move_ver, can_rotate, i;
    int  full_lines,refresh_timing, orig_refresh_timing;
    
    lcd.orientation(SG_PORTRAIT_LOW);
    lcd.baudChange(SG_BAUD6); //set a fast baud! is recommended to use fast baud rates
    TCNT1 = 0x0001;
    
    while(1){
        welcome_screen_160();
        settings_loop();

        currentShapeIndex = 0;
        length = HEIGHTPORTRAIT / TET_MAX_BLOCKS_Y;//gfx_Get(Y_RES) / TET_MAX_BLOCKS_Y;
        // set how long between screen refreshes & shape movements
        orig_refresh_timing = 1000 - diffLevel * 100;
        refresh_timing = orig_refresh_timing;

        new_game(diffLevel);
        draw_screen();
        delay(refresh_timing);

        get_new_shape(get_next_shape_index());
        randomSeed(TCNT1); //set a new seed to random generator
       
        do{
            move_to_next_shape();
            get_new_shape (get_next_shape_index());
            draw_shape(get_next_shape_index());         
            i = 0;
            TCNT1 = 0xFFFF - (refresh_timing * 15); overFlowFlag=0;            
            while(1){
                if (i){
                    remove_from_screen (currentShapeIndex);
                }
                can_rotate = 0;
                can_move_hor = 0;                
                can_move_ver = 1;               

                switch(get_input()){
                  case ENTER:
                     settings_loop();
                     lcd.drawRectangle(0, 0, WIDTHPORTRAIT-1, HEIGHTPORTRAIT-1, BACKGROUND, SG_FILL);
                     draw_shape(get_next_shape_index());                                       
                     if (update_level()){
                       refresh_timing = orig_refresh_timing - 100 * diffLevel;
                       if (refresh_timing < 0){
                           refresh_timing = 0;
                       }
                     }
                  break;
                  
                  case DOWN:
                     can_move_ver = shape_can_move(currentShapeIndex, 0, 1);
                  break;
  
                  case ROTATE:
                     delay(150);
                     can_rotate = shape_can_rotate(currentShapeIndex);
                  break;                
                  
                  case LEFT:
                     can_move_hor = shape_can_move(currentShapeIndex, -1, 0);
                  break;
                  
                  case RIGHT:
                     can_move_hor = shape_can_move(currentShapeIndex, 1, 0);
                  break;                  
                  
                  default:
                  break;
                }                
                if (overFlowFlag==1){
                    diry = 1;
                    can_move_ver = shape_can_move(currentShapeIndex, 0, diry);
                    TCNT1 = 0xFFFF - (refresh_timing * 15); overFlowFlag=0;                    
                }
                add_to_screen(currentShapeIndex);
                draw_screen();

                if(!can_move_hor && !can_move_ver && !can_rotate){
                    break;
                }
                i++;
            }
            full_lines = find_full_lines(lines);
            while (full_lines--){
                remove_line_from_screen(lines[full_lines]);
            }
            if (update_level()){
                refresh_timing = orig_refresh_timing - 100 * diffLevel;
                if (refresh_timing < 0){
                    refresh_timing = 0;
                }
            }
        }while(!touching_top());

        game_over();
        while(get_input()==0);
    }
}
