#include "fx.cpp"

#define LED_PIN 0
#define MAT_LINES 12
#define MAT_COLS 12
#define LED_COUNT 144

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup () {

	// Específico do simulador para desenhar o número correto de colunas
	strip.fx_num_cols(MAT_COLS);
	
	strip.begin();	
	strip.show ();
	
}

void loop () {
	
	for(int j = 0; j < MAT_COLS; j++) {
		set_col_range(j, 0, j+1, 255, 0, 0);
		strip.show();
		delay(100);
	}
	
	for(int i = 0; i < MAT_LINES; i++) {
		int line  = MAT_LINES - i - 1;
		int start = 0;
		int end   = MAT_LINES - i;
		add_line_range(line, start, end, 0, 255, 0);
		strip.show();
		delay(100);
	}
		
	strip.clear();
	strip.show();
	delay(100);
	
	bool turn = true;
	bool black = false;
	for(int k = 0; k < LED_COUNT; k += 4) {
		int n = LED_COUNT / 2 - k - 3;
		if(k >= LED_COUNT / 4 && turn) { turn = false; black = !black; inv_M(); strip.show(); }
		black = !black;
		for(int i = 0, j = LED_COUNT-1; i < n; i++, j--) {
			if(i > 0) strip.setPixelColor(i-1, 255 * black, 255 * black, 255 * black);
			if(j < LED_COUNT-1) set_strip(j+1, 255 * black, 255 * black, 255 * black);
			strip.setPixelColor(i, 0, 0, 255);
			strip.setPixelColor(j, 255, 0, 0);
			strip.setPixelColor(i+1, 0, 0, 255);
			strip.setPixelColor(j-1, 255, 0, 0);
			strip.setPixelColor(i+2, 0, 0, 255);
			strip.setPixelColor(j-2, 255, 0, 0);
			strip.setPixelColor(i+3, 0, 0, 255);
			strip.setPixelColor(j-3, 255, 0, 0);
			strip.show();
			delay(20);
		}
	}
	
	delay(500);
	set_M(0, 0, 0);
	
}
