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
		int end   = M_H - i - 1;
		add_line_range(line, start, end, 0, 255, 0);
		usleep(100000);
	}
		
	clear();
	
}
