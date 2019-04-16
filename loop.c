#include "fx.c"

void setup () {
	
	M_W = 12; // width
	M_H = 12; // height

	clear();
	
}

void loop () {
	
	int i, j;
	int x = 2;

	for(j = 0; j < M_W; j++) {
		add_col_M(j, 255, 0, 0);
		usleep(100000);
	}
		
	clear();
	
}
