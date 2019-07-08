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

//Connect 2 push buttons to play, buttons must be connected to pin 8 and 9, and must reach GND when pressed.
//defines for controls
#define LEFT  8         //control input pin
#define RIGHT 9         //control input pin

//game definitions
#define MAXLEVEL 15
#define NUMCOLUMNSX 22

int uSD_initialised;
int tile_size, NUM_TILES_X, NUM_TILES_Y;
int NUM_LINES, NUM_COLUMNS, NUM_SPRITES;
int Sprite_x[42], Sprite_y[42], Sprite_type[42], Sprite_count;
int screen[40];
int ball_v_x, ball_v_y;
int ball_x_offset, ball_y_offset;
int platform_x_offset;
int level;

const char  arkanoidMap[]={
1, 2, 3, 4, 5, 6, 3, 4, 5, 6, 3, 7, 8, 9, 10, 7,
8, 9, 10, 7,
11, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 16, 17, 18, 19, 16,
17, 18, 19, 16,
20, 21, 22, 23, 24, 25, 22, 23, 24, 25, 22, 26, 27, 28, 29, 26,
27, 28, 29, 26,
30, 31, 32, 33, 34, 35, 32, 33, 34, 35, 32, 36, 37, 38, 39, 36,
37, 38, 39, 36,
40, 41, 42, 43, 44, 45, 42, 43, 44, 45, 42, 46, 47, 48, 49, 46,
47, 48, 49, 46,
51, 52, 53, 50, 54, 52, 53, 50, 54, 52, 53, 50, 55, 56, 57, 50,
55, 56, 57, 50,
58, 59, 24, 25, 22, 23, 24, 25, 22, 23, 24, 28, 29, 26, 27, 28,
29, 26, 27, 28,
60, 61, 34, 35, 32, 33, 34, 35, 32, 33, 34, 38, 39, 36, 37, 38,
39, 36, 37, 38,
62, 63, 44, 45, 42, 43, 44, 45, 42, 43, 44, 48, 49, 46, 47, 48,
49, 46, 47, 48,
64, 50, 54, 52, 53, 50, 54, 52, 53, 50, 54, 56, 57, 50, 55, 56,
57, 50, 55, 56,
65, 21, 66, 67, 68, 25, 66, 67, 68, 25, 66, 69, 70, 28, 71, 69,
70, 28, 71, 69,
72, 73, 74, 75, 76, 77, 74, 75, 76, 77, 74, 78, 79, 80, 81, 78,
79, 80, 81, 78,
82, 83, 84, 85, 86, 87, 84, 85, 86, 87, 84, 88, 89, 90, 91, 88,
89, 90, 91, 88,
93, 94, 95, 92, 96, 94, 95, 92, 96, 94, 95, 92, 97, 98, 99, 92,
97, 98, 99, 92,
100, 101, 102, 103, 104, 105, 102, 103, 104, 105, 102, 106, 107, 108, 109, 106,
107, 108, 109, 106,
110, 111, 76, 77, 74, 75, 76, 77, 74, 75, 76, 80, 81, 78, 79, 80,
81, 78, 79, 80
};


// initialise the constants
void initGame(void){
    int i;
    // data
    NUM_SPRITES = 42;
    tile_size = 10;
    NUM_LINES = (LCDHEIGHT+1) / tile_size;      //+1
    NUM_COLUMNS = (LCDWIDTH+1) / tile_size-1; //+1
    ball_x_offset = 0;
    ball_y_offset = 0;
    platform_x_offset = 0;
    level = 1;

    // platform
    Sprite_x[0] = 0;
    Sprite_y[0] = (NUM_LINES - 2);
    Sprite_type[0] = 1;

    // ball
    Sprite_x[1] = 2 + abs(random(0,65535) % NUM_COLUMNS);
    if (Sprite_x[1] >= NUM_COLUMNS) Sprite_x[1] = 2;
    Sprite_y[1] = NUM_LINES - 2;//(NUM_LINES - 3);
    Sprite_type[1] = 1;
    ball_v_x = 2;
    ball_v_y = -2;

    // rectangles
    i = 2;
    while (i < 12){
        Sprite_type[i] = 1;
        Sprite_x[i] = 2 * (i-2);
        Sprite_y[i] = 1;
        if (i > 10) Sprite_type[i] = -1;
        screen[i - 2] = i;
        i++;
    }
    while (i < 22){
        Sprite_type[i] = 1;
        Sprite_x[i] = 2 * (i-12);
        Sprite_y[i] = 2;
        if (i > 20) Sprite_type[i] = -1;
        screen[i - 2] = i;
        i++;
    }
    while (i < 32){
        Sprite_type[i] = 1;
        Sprite_x[i] = 2 * (i-22);
        Sprite_y[i] = 3;
        if (i > 30) Sprite_type[i] = -1;
        screen[i - 2] = i;
        i++;
    }
    while (i < 42){
        Sprite_type[i] = 1;
        Sprite_x[i] = 2 * (i-32);
        Sprite_y[i] = 4;
        if (i > 40) Sprite_type[i] = -1;
        screen[i - 2] = i;
        i++;
    }
    Sprite_count = 8 * 4;

    // screen
    NUM_TILES_X = (LCDWIDTH) / tile_size;
    NUM_TILES_Y = (LCDHEIGHT) / tile_size;
    lcd.erase();
}

// display a single tile
void display_tile(int tile, int x, int y){
    lcd.drawRectangle(x, y, x + tile_size-1, y + tile_size-1, BLACK, FILL);
}

// display the background tiles
void display_background(){
    int i, j, tile;

    j = 0;
    while (j < NUM_TILES_Y){
        i = 0;
        while (i < NUM_TILES_X){
            tile = arkanoidMap[(i + (NUMCOLUMNSX * j))];
            display_tile(255&tile, i * tile_size, j * tile_size);
            i++;
        }
        j++;
    }
}

void display_sprite(int i){
    int colour;
	
    if (i < 0 || i >= NUM_SPRITES) return;
    if (i == 0){// platform
        lcd.drawRectangle(Sprite_x[i] * tile_size + (tile_size / 4), Sprite_y[i] * tile_size, (Sprite_x[i] + 2) * tile_size - (tile_size / 4), (Sprite_y[i] + 1) * tile_size - (tile_size / 2), WHITE,FILL);
        lcd.drawCircle(Sprite_x[i]*tile_size+tile_size/4, Sprite_y[i]*tile_size+tile_size/4,tile_size/4,WHITE,FILL);
        lcd.drawCircle((Sprite_x[i] + 2) * tile_size - (tile_size / 4), Sprite_y[i]*tile_size+tile_size/4,tile_size/4,WHITE,FILL);
    }else if (i == 1){//ball
        lcd.drawCircle(Sprite_x[i]*tile_size+tile_size/2+ball_x_offset, Sprite_y[i]*tile_size+tile_size/2+ball_y_offset,tile_size/2 - 1,WHITE,FILL);
    }else if (Sprite_type[i] >= 0 && i > 1 && i < 42){//rectangles
        if (i < 12){
            colour = YELLOW;
        }else if (i < 22){
            colour = RED;
        }else if (i < 32){
            colour = GREEN;
        }else if (i < 42){
            colour = MAGENTA;
        }
        if (i < 42){
            lcd.drawRectangle(Sprite_x[i] * tile_size, Sprite_y[i] * tile_size, (Sprite_x[i] + 2) * tile_size - 1, (Sprite_y[i] + 1) * tile_size - 1, BLACK,UNFILL);
            lcd.drawRectangle(Sprite_x[i] * tile_size, Sprite_y[i] * tile_size, (Sprite_x[i] + 2) * tile_size - 2, (Sprite_y[i] + 1) * tile_size - 2, WHITE,UNFILL);
            lcd.drawRectangle(Sprite_x[i] * tile_size + 1, Sprite_y[i] * tile_size + 1, (Sprite_x[i] + 2) * tile_size - 2, (Sprite_y[i] + 1) * tile_size - 2, colour,FILL);
        }
    }else if (i >= 0 && Sprite_type[i] < 0){
        display_tile(arkanoidMap[Sprite_x[i] + Sprite_y[i] * NUMCOLUMNSX], Sprite_x[i] * tile_size, Sprite_y[i] * tile_size);
        display_tile(arkanoidMap[(Sprite_x[i] + 1) + Sprite_y[i] * NUMCOLUMNSX], (Sprite_x[i] + 1) * tile_size, Sprite_y[i] * tile_size);
    }
}

// converts joystick input to screen motion
void process_input(){
    if (digitalRead(LEFT)==0){ //if LEFT button is pressed
        display_tile(arkanoidMap[Sprite_x[0] + Sprite_y[0] * NUMCOLUMNSX], Sprite_x[0] * tile_size, Sprite_y[0] * tile_size);
        display_tile(arkanoidMap[(Sprite_x[0] + 1) + Sprite_y[0] * NUMCOLUMNSX], (Sprite_x[0] + 1) * tile_size, Sprite_y[0] * tile_size);
        display_tile(arkanoidMap[(Sprite_x[0] + 2) + Sprite_y[0] * NUMCOLUMNSX], (Sprite_x[0] + 2) * tile_size, Sprite_y[0] * tile_size);
        Sprite_x[0] = Sprite_x[0] + (platform_x_offset - abs(ball_v_x*2)) / tile_size;
        platform_x_offset = (platform_x_offset - abs(ball_v_x*2)) % tile_size;
        if (Sprite_x[0] < 0) Sprite_x[0] = 0;
        display_sprite(0);
    }else if (digitalRead(RIGHT)==0){ //if RIGHT button is pressed
        display_tile(arkanoidMap[Sprite_x[0] - 1 + Sprite_y[0] * NUMCOLUMNSX], (Sprite_x[0] - 1) * tile_size, Sprite_y[0] * tile_size);
        display_tile(arkanoidMap[Sprite_x[0] + Sprite_y[0] * NUMCOLUMNSX], Sprite_x[0] * tile_size, Sprite_y[0] * tile_size);
        display_tile(arkanoidMap[(Sprite_x[0] + 1) + Sprite_y[0] * NUMCOLUMNSX], (Sprite_x[0] + 1) * tile_size, Sprite_y[0] * tile_size);
        Sprite_x[0] = Sprite_x[0] + (platform_x_offset + abs(ball_v_x*2)) / tile_size;
        platform_x_offset = (platform_x_offset + abs(ball_v_x*2)) % tile_size;
        if (Sprite_x[0] >= (NUM_TILES_X - 2)) Sprite_x[0] = (NUM_TILES_X - 2);
        display_sprite(0);
    }
}

void display_sprites(){
    int i;
    display_sprite(0);
    display_sprite(1);
    i = 2;
    while (i < 42){
        display_sprite(i++);
    }
}

void update_ball(){
    int i, j, k;

    // set previous ball background
    i = -1;
    while (i <= 1){
        if ((ball_v_x < 0 && i > 0) || (ball_v_x > 0 && i < 0) || Sprite_x[1] + i < 0 || Sprite_x[1] > NUM_COLUMNS){
            i++;
            continue;
        }
        j = -1;
        while (j <= 1){
            if ((ball_v_y < 0 && j > 0) || (ball_v_y > 0 && j < 0) || Sprite_y[1] + j < 0 || Sprite_y[1] >= NUM_LINES){
                j++;
                continue;
            }
            display_tile(arkanoidMap[Sprite_x[1] + i + (Sprite_y[1] + j) * NUMCOLUMNSX],
                (Sprite_x[1] + i) * tile_size,
                (Sprite_y[1] + j) * tile_size);
            j++;
        }
        i++;
    }
    display_sprite(0);

    // check intersection with sprites
    if (ball_x_offset == 0 && ball_y_offset == 0){
        i = NUM_SPRITES - 1;
        while (i > 1){
            if ((Sprite_type[i] >= 0 && (Sprite_x[i] == Sprite_x[1] || Sprite_x[i] + 1 == Sprite_x[1]) && Sprite_y[i] + 1 == Sprite_y[1])){
                // remove sprite from display
                Sprite_type[i] = -1;
                Sprite_count--;
                ball_v_y = -ball_v_y;
                display_sprite(i);
                if (ball_v_x < 0) display_sprite(i - 1);
                if (ball_v_x > 0) display_sprite(i + 1);
                display_sprite(i);
                break;
            }
            i--;
        }
    }

    // change direction if necessary
    if (ball_x_offset == 0 && (Sprite_x[1] <= 0 || Sprite_x[1] >= (NUM_COLUMNS))){
        ball_v_x = -ball_v_x;
    }
    if (ball_y_offset == 0 && (Sprite_y[1] <= 0)){
        ball_v_y = -ball_v_y;
    }

    // check intersection with platform
    if (ball_v_x < 0){
        if (ball_x_offset == 0 && ball_y_offset == 0 && (Sprite_y[1] + 1) == Sprite_y[0] &&
            (Sprite_x[1] == Sprite_x[0] || Sprite_x[1] == (Sprite_x[0] + 1) || Sprite_x[1] == (Sprite_x[0] + 2))){
            ball_v_y = -ball_v_y;
        }
    }else if (ball_v_x > 0){
        if (ball_x_offset == 0 && ball_y_offset == 0 && (Sprite_y[1] + 1) == Sprite_y[0] &&
            (Sprite_x[1] + 1 == Sprite_x[0] || Sprite_x[1] + 1 == (Sprite_x[0] + 1) || Sprite_x[1] + 1 == (Sprite_x[0] + 2))){
            ball_v_y = -ball_v_y;
        }
    }else{
        ball_v_y = -ball_v_y;
    }

    // update position
    Sprite_x[1] = Sprite_x[1] + (ball_x_offset + ball_v_x) / tile_size;
    ball_x_offset = (ball_x_offset + ball_v_x) % tile_size;
    Sprite_y[1] = Sprite_y[1] + (ball_y_offset + ball_v_y) / tile_size;
    ball_y_offset = (ball_y_offset + ball_v_y) % tile_size;
    display_sprite(1);
}

void welcome_screen(){
  lcd.setTextColour(YELLOW);
  lcd.setTextSize(FONT2);
  lcd.string(0,0,LCDWIDTH-1,LCDHEIGHT-1,"***********************",0);
  lcd.string(10,20,LCDWIDTH-1,LCDHEIGHT-1,"Vizic Technologies",0);  
  lcd.string(22,50,LCDWIDTH-1,LCDHEIGHT-1,"--ARKANOID--",0);     
  lcd.string(23,80,LCDWIDTH-1,LCDHEIGHT-1,"mini SmartGPU",0);     
  lcd.string(0,112,LCDWIDTH-1,LCDHEIGHT-1,"***********************",0);  
  while(digitalRead(LEFT)!=0 && digitalRead(RIGHT)!=0); //while no button is pressed
}


/**************************************************************************************************/
/**************************************************************************************************/
void setup() { //initial setup
  //Those TWO functions must always be called for MINISMARTGPU support
  lcd.init();  //configure the serial and pinout of arduino board for MINISMARTGPU support
  lcd.start(); //initialize the MINISMARTGPU procESSor

  //configure input controls
  pinMode(LEFT,  INPUT_PULLUP);          // set pin to input (buttons)
  pinMode(RIGHT, INPUT_PULLUP);          // set pin to input (buttons)     
}

/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
/**************************************************************************************************/
void loop() { //main loop    

    lcd.baudChange(2000000); //set a fast baud! is recommended to use fast baud rates

    welcome_screen();
    
    while(1){
        initGame();
        // main input & display loop
        display_background();
        display_sprites();
        
        while (Sprite_count){
            process_input();
            update_ball();
            if (Sprite_y[1] >= NUM_LINES) break;
            delay((MAXLEVEL-level));
        }
        // game over
        lcd.drawRectangle(0,115,LCDWIDTH-1,LCDHEIGHT-1,BLACK,FILL);
        if (Sprite_count <= 0){
            level++;
            lcd.string(45,80,LCDWIDTH-1,LCDHEIGHT-1,"You Win!",0);            
        }else{
            lcd.string(43,80,LCDWIDTH-1,LCDHEIGHT-1,"You Lose!",0);            
        }
        while(digitalRead(LEFT)!=0 && digitalRead(RIGHT)!=0); //while no button is pressed
    }
}

