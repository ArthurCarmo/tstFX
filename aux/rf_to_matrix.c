#include <stdio.h>

int main() {

	FILE *f;
	
	f = fopen("rf", "r");
	
	int i, j;
	char c;
	
	for(i = 0; i < 39; i++) {
		printf("{");
		for(j = 0; j < 36; j++) {
			switch(fgetc(f)) {
				case ' ' : printf("vazio");
					break;
				case 'G' : printf("green");
					break;
				case 'B' : printf("blue");
					break;
				case 'K' : printf("gold");
					break;
				case 'g' : printf("weak_green");
					break;
				case 'b' : printf("weak_blue");
					break;
				case 'k' : printf("weak_gold");
					break;
				case 'W' : printf("wall");
					break;
				default : printf("vazio");
					break;
			}
			if(j != 35) printf(",");
		}
		fgetc(f);
		printf("}");
		if(i != 38) printf(",");
	} printf("\n");

	return 0;
}
