#include "fx.cpp"

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

#define MAZE_SIZE 36
#define MAZE_SIZE 36
const uint32_t maze[MAZE_SIZE][MAZE_SIZE] = 
{ 
	// 1	   2      3      4      5      6      7	     8      9     10     11     12     13     14     15     16     17     18     19     20     21     22     23     24     25     26     27     28     29     30     31     32     33     34     35     36
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio,  wall, vazio, vazio, vazio,  blue, vazio, vazio, vazio,  wall, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio,  wall, vazio, vazio, vazio, vazio, vazio, vazio, vazio,  wall, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio,  wall,  blue,  blue,  blue,  blue,  blue,  blue,  blue,  blue,  blue,  wall, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	{ vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio, vazio },
	
};

int 
player_local_l,
player_local_c,
player_global_l,
player_global_c,

bottom_left_l,
bottom_left_c;

uint32_t view[ROWS][COLS] = { 0 };

void update_view() {
	for(int i = 0; i < 12; i++) {
		for(int j = 0; j < 12; j++) {
			view[i][j] = maze[i+bottom_left_l][j+bottom_left_c];
		}
	}
	
	for(int i = 0; i < 12; i++) {
		for(int j = 0; j < 12; j++) {
			strip.setPixelColor(map_px(i, j), view[i][j]);
		}
	}
}

void setup () {

	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(RIGHT, INPUT);
	strip.begin();

/*	for(int i = 0; i < 7; i++) {
		for(int j = 0; j < 6; j++) {
			strip.setPixelColor(map_px(i, j), pernalonga[i][j]);
		}
	}

	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			strip.setPixelColor(map_px(i+7, j), jake[i][j]);
		}
	}
	
	draw_tnmt('r', 0, 6); */
	
	
	player_local_l = 0;
	player_local_c = 5;
	
	player_global_l = 0;
	player_global_c = 19;
	
	bottom_left_l = 0;
	bottom_left_c = 13;
	
	update_view();
	strip.setPixelColor(map_px(player_local_l, player_local_c), player);
	
	strip.show ();	
}

void reset_game() {
	
	
}

void loop () {
	
//	strip.show();
}
