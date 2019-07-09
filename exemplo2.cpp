#include "fx.cpp"

#define LED_PIN 6
#define LED_COUNT 144


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int k = 0;

void setup()
{
	strip.begin();
	strip.show();
}

#define LINES 12
#define COLS  12

#define CL 0x0000FF


int map_px ( int l, int c ) {
	return l *  COLS + ((l % 2)?COLS-c-1:c);
}

void loop()
{
	strip.clear();
	for(int i = 0; i < LINES; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
			strip.setPixelColor(map_px(i, j), (max(0, (CL- (int)200.0*j/COLS))) << (8*(k%3)));
			k++;
		}
	}
	strip.show();
	delay(1000);
}
