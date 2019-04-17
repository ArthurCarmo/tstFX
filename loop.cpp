#include "fx.c"

void setup () {
	
	M_W = 12; // width
	M_H = 12; // height

	clear();
	
}

void loop () {
	
	for(int j = 0; j < M_W; j++) {
		set_col_range(j, 0, j+1, 255, 0, 0);
		usleep(100000);
	}
	
	for(int i = 0; i < M_H; i++) {
		int line  = M_H - i - 1;
		int start = 0;
		int end   = M_H - i;
		add_line_range(line, start, end, 0, 255, 0);
		usleep(100000);
	}
		
	clear();
	usleep(100000);
	
	for(int k = 0; k < LED_COUNT; k += 4) {
		int n = LED_COUNT / 2 - k - 3;
		for(int i = 0, j = LED_COUNT-1; i < n; i++, j--) {
			if(i > 0) set_strip(i-1, 0, 0, 0);
			if(j < LED_COUNT-1) set_strip(j+1, 0, 0, 0);
			set_strip(i, 0, 0, 255);
			set_strip(j, 255, 0, 0);
			set_strip(i+1, 0, 0, 255);
			set_strip(j-1, 255, 0, 0);
			set_strip(i+2, 0, 0, 255);
			set_strip(j-2, 255, 0, 0);
			set_strip(i+3, 0, 0, 255);
			set_strip(j-3, 255, 0, 0);
			
			usleep(20000);
		}
	}
	
}
