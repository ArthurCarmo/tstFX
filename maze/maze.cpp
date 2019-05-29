#include "../fx.cpp"

char dir = 's';
char last_dir = 's';

#define LED_PIN 1
#define ROWS 12
#define COLS 12
#define LED_COUNT 144

#define UP	7
#define DOWN	6
#define LEFT	5
#define RIGHT	4
#define CENTER  3
#define INTERRUPT 2

int map_px ( int l, int c ) {
	return l *  COLS + ((l % 2)?COLS-c-1:c);
}

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int apple_x;
int apple_y;
int new_seed = 0;

const uint32_t
wall	 		= 0xFF0000,
portal_in 		= 0x00FF00,
portal_out	 	= 0x0000FF,
player			= 0xFFFFFF,
vazio			= 0x000000,
red			= 0xFF0000,
green			= 0x00FF00,
blue			= 0x0000FF,
gold			= 0xFFDF00,
weak_blue		= 0x000055,
weak_green		= 0x005500,
weak_gold		= 0xc6a600,


pl_orelha		= 0x8F8F8F,
pl_cabeca		= 0xADADAD,
pl_olho			= 0x000000,
pl_bigode		= 0xE3E3E3,
pl_cenoura_cabo		= 0x098500,
pl_cenoura		= 0xFF8103,
pl_vazio		= 0xFFFFFF;

const uint32_t pernalonga[7][6] = 
{
	{pl_vazio, pl_orelha, pl_vazio, pl_orelha, pl_vazio, pl_vazio },
	{pl_vazio, pl_orelha, pl_vazio, pl_orelha, pl_vazio, pl_vazio },
	{pl_vazio, pl_orelha, pl_vazio, pl_orelha, pl_vazio, pl_vazio },
	{pl_vazio, pl_cabeca, pl_cabeca, pl_cabeca, pl_vazio, pl_cenoura_cabo },
	{pl_vazio, pl_olho, pl_cabeca, pl_olho, pl_vazio, pl_cenoura },
	{pl_bigode, pl_bigode, pl_bigode, pl_bigode, pl_bigode, pl_cenoura },
	{pl_bigode, pl_bigode, pl_bigode, pl_bigode, pl_bigode, pl_cenoura }
};

const uint32_t
jake_cabeca	= 0xFFD21C,
jake_olho	= 0xFFFFFF,
jake_bigode	= 0xD1AC17,
jake_focinho	= 0x000000;

const uint32_t jake[5][5] =
{
	{ jake_cabeca, jake_cabeca, jake_cabeca, jake_cabeca, jake_cabeca },
	{ jake_cabeca, jake_olho, jake_cabeca, jake_olho, jake_cabeca }, 
	{ jake_cabeca, jake_bigode, jake_focinho, jake_bigode, jake_cabeca },
	{ jake_cabeca, jake_bigode, jake_cabeca, jake_bigode, jake_cabeca },
	{ jake_cabeca, jake_cabeca, jake_cabeca, jake_cabeca, jake_cabeca }
};

const uint32_t
tnmt_cabeca		= 0x196606,
tnmt_olho		= 0xFFFFFF,
tnmt_leonardo		= 0x2E00E8,
tnmt_rafael		= 0xE80505,
tnmt_donatello		= 0x82127B,
tnmt_michelangelo	= 0xE8890E,
tnmt_vazio		= 0x000000;

void draw_tnmt(char turtle, int x, int y) {
	uint32_t t_cl;
	switch(turtle) {
		case 'l': t_cl = tnmt_leonardo; break;
		case 'r': t_cl = tnmt_rafael; break;
		case 'd': t_cl = tnmt_donatello; break;
		case 'm': t_cl = tnmt_michelangelo; break;
		default : return;
	}
	
	
	for(int i = x; i < 2; i++) {
		strip.setPixelColor(map_px(i, y), tnmt_vazio); 
	} 
	strip.setPixelColor(map_px(x+2, y), t_cl);
	
	for(int j = y+1; j <= y+5; j++) {
		strip.setPixelColor(map_px(x, j), tnmt_cabeca); 
		strip.setPixelColor(map_px(x+2, j), tnmt_cabeca); 		
	}
	
	
	strip.setPixelColor(map_px(x+1, y+1), t_cl);
	strip.setPixelColor(map_px(x+1, y+2), tnmt_olho);
	strip.setPixelColor(map_px(x+1, y+3), t_cl);
	strip.setPixelColor(map_px(x+1, y+4), tnmt_olho);
	strip.setPixelColor(map_px(x+1, y+5), t_cl);
}

#define MAZE_LINES 39
#define MAZE_COLUMNS 36
const uint32_t maze[MAZE_LINES][MAZE_COLUMNS] = 
{ 
	#include "parcial_map"
};

int 
start_l = 37,
start_c = 18,

player_l,
player_c,

top_left_l,
top_left_c;

int centered;

uint32_t view[ROWS][COLS] = { 0 };

uint32_t menu[ROWS] = 
{ 	weak_gold, weak_gold, weak_gold,
	pl_vazio, weak_blue, weak_blue,
	weak_blue, pl_vazio, weak_green,
	weak_green, weak_green, pl_vazio
};

int line_center = 8;
int col_center  = 5;
void center_screen() {
	if(player_l > MAZE_LINES - (ROWS - line_center)) { 
		top_left_l = MAZE_LINES - ROWS;
	} else if (player_l < line_center) {
		top_left_l = 0;
	} else {
		top_left_l = player_l - line_center;
	}
	if(player_c > MAZE_COLUMNS - (COLS - col_center)) { 
		top_left_c = MAZE_COLUMNS - COLS;
	} else if (player_c < col_center) {
		top_left_c = 0;
	} else {
		top_left_c = player_c - col_center;
	}
}

void draw_items() {
	for(int i = 0; i < ROWS; i++) {
		strip.setPixelColor(map_px(i, COLS-1), menu[i]);
	}
}

void update_view() {

	if(centered) center_screen();	
	
	for(int i = 0; i < ROWS; i++) {
		for(int j = 0; j < COLS-1; j++) {
			view[i][j] = maze[i+top_left_l][j+top_left_c];
		}
	}
	
	for(int i = 0; i < ROWS; i++) {
		for(int j = 0; j < COLS-1; j++) {
			strip.setPixelColor(map_px(i, j), view[i][j]);
		}
	}

	if(player_l >= top_left_l && player_l < top_left_l + ROWS 
	&& player_c >= top_left_c && player_c < top_left_c + COLS - 1) {
		strip.setPixelColor(map_px(player_l-top_left_l, player_c-top_left_c), player);
	}
	
	strip.show();
}

void setup () {

	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(RIGHT, INPUT);
	strip.begin();

	centered = 1;	
	player_l = start_l;
	player_c = start_c;

	top_left_l = MAZE_LINES - ROWS;
	top_left_c = 13;
	
	draw_items();
	update_view();
	
	strip.show ();	
}

void reset_game() {
	
	
}

void screen_up() {
	if(top_left_l > 0) --top_left_l;
	update_view();
}

void screen_down() {
	if(top_left_l < MAZE_LINES - ROWS) ++top_left_l;
	update_view();
}
void screen_left() {
	if(top_left_c > 0) --top_left_c;
	update_view();
}
void screen_right() {
	if(top_left_c < MAZE_COLUMNS - COLS) ++top_left_c;
	update_view();
}

void player_up() {
	if(player_l > 0) { 
		--player_l;
		update_view();
	}
}
void player_down() {
	if(player_l < MAZE_LINES - 1) { 
		++player_l;
		update_view();
	}
}
void player_left() {
	if(player_c > 0) { 
		--player_c;
		update_view();
	}
}
void player_right() {
	if(player_c < MAZE_COLUMNS - 2) {
		++player_c;
		update_view();
	}
}

void loop () {
	if(digitalRead(UP)) {
//		screen_up();
		player_up();
		delay(100);
	} else if(digitalRead(DOWN)) {
//		screen_down();
		player_down();
		delay(100);
	} else if(digitalRead(LEFT)) {
//		screen_left();
		player_left();
		delay(100);
	} else if(digitalRead(RIGHT)) {
//		screen_right();
		player_right();
		delay(100);
	} else if(digitalRead(CENTER)) {
		centered = 1 - centered;
		update_view();
		delay(100);
	}
//	strip.show();
}
