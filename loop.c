#include "fx.c"

void setup () {

}

void loop () {
	
	int i, j;
	int x = 2;
	clear();

	for(i = 0; i < M_H; i++) {
		add_col_M(i, 255, 0, 0);
		usleep(100000);
	}
		
	clear();
	
}
