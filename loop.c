#include "fx.c"

void setup () {

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
