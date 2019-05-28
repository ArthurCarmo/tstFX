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
wall_color 		= 0xFF0000,
portal_in_color 	= 0x00FF00,
portal_out_color 	= 0x0000FF,
player_color		= 0xFFFFFF,
red			= 0xFF0000,
green			= 0x00FF00,
blue			= 0x0000FF,
pl_orelha		= 0x8F8F8F,
pl_cabeca		= 0xADADAD,
pl_olho			= 0x000000,
pl_bigode		= 0xE3E3E3,
pl_cenoura_cabo		= 0x098500,
pl_cenoura		= 0xFF8103,
vazio			= 0xFFFFFF;

const uint32_t pernalonga[7][6] = 
{
	{vazio, pl_orelha, vazio, pl_orelha, vazio, vazio },
	{vazio, pl_orelha, vazio, pl_orelha, vazio, vazio },
	{vazio, pl_orelha, vazio, pl_orelha, vazio, vazio },
	{vazio, pl_cabeca, pl_cabeca, pl_cabeca, vazio, pl_cenoura_cabo },
	{vazio, pl_olho, pl_cabeca, pl_olho, vazio, pl_cenoura },
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

const uint32_t maze[36][36] = 
{ 
	0
};

void setup () {

	pinMode(UP, INPUT);
	pinMode(DOWN, INPUT);
	pinMode(LEFT, INPUT);
	pinMode(RIGHT, INPUT);
	strip.begin();

	for(int i = 0; i < 7; i++) {
		for(int j = 0; j < 6; j++) {
			strip.setPixelColor(map_px(i, j), pernalonga[i][j]);
		}
	}

	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			strip.setPixelColor(map_px(i+7, j), jake[i][j]);
		}
	}
	
	draw_tnmt('r', 0, 6);

	strip.show ();	
}

void reset_game() {
	
	
}

void loop () {
	
	strip.show();
}
