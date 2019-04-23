#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#define M_MAX 1000

int screenW = 600;
int screenH = 480;
float M[M_MAX][M_MAX][3] = { 0 };
float RM[M_MAX][M_MAX][3] = { 0 };

int MAT_COLS = 12;
int MAT_LINES = 12;
int LED_COUNT;

int orthoX;
int orthoY;

int lineL = 10;
int colL  = 10;

enum menu_opcoes { NONE = 0, PIXEL, LINE, COLUMN, INV, DIAGONAL, CLEAR, ADD, SUB, BYE, FX } main_opt;
enum menu_opcoes sec_opt;

int min(int a, int b) { return a<b?a:b; }
int max(int a, int b) { return a>b?a:b; }
float minf(float a, float b) { return a<b?a:b; }
float maxf(float a, float b) { return a>b?a:b; }

void show() { int i, j, k; for(i = 0; i < MAT_LINES; i++) for(j = 0; j < MAT_COLS; j++) for(k = 0; k < 3; k++) RM[i][j][k] = M[i][j][k]; }

void delay(int t) { usleep(t * 1000); }
void begin() { }

void paint( int i, int j, float _r, float _g, float _b ) {

	int geo_i = i * lineL;
	int geo_j = j * colL;
	
	glColor3f(_r, _g, _b);
	glBegin(GL_TRIANGLES);	
		glVertex2d(geo_i, geo_j);
		glVertex2d(geo_i, geo_j + colL);
		glVertex2d(geo_i + lineL, geo_j + colL);
	glEnd();

	glBegin(GL_TRIANGLES);
		glVertex2d(geo_i + lineL, geo_j + colL);
		glVertex2d(geo_i + lineL, geo_j);
		glVertex2d(geo_i, geo_j);
	glEnd();
	
	glLineWidth(1);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex2d(geo_i, geo_j);
		glVertex2d(geo_i, geo_j + colL);
		glVertex2d(geo_i + lineL, geo_j + colL);
		glVertex2d(geo_i + lineL, geo_j);
	glEnd();
	
	glFlush();
}

void draw_matrix ()  { 

	int i, j;

	for(i = 0; i < MAT_LINES; i++)
		for(j = 0; j < MAT_COLS; j++)
			paint(j, i, RM[i][j][0], RM[i][j][1], RM[i][j][2]);

}


void refresh_screen(void) {

	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	
	draw_matrix();
	
	glColor3f(0.0f,0.0f,0.0f);
	glutSwapBuffers();
	
	glutPostRedisplay();
}

void add_M (int _r, int _g, int _b) {
	for(int i = 0; i < MAT_LINES; i++) {
		for(int j = 0; j < MAT_COLS; j++) {
			M[i][j][0] = minf(1.0, M[i][j][0] + (float) _r / 255.0);
			M[i][j][1] = minf(1.0, M[i][j][1] + (float) _g / 255.0);
			M[i][j][2] = minf(1.0, M[i][j][2] + (float) _b / 255.0);
		}	
	}
}

void set_M (int _r, int _g, int _b) {
	for(int i = 0; i < MAT_LINES; i++) {
		for(int j = 0; j < MAT_COLS; j++) {
			M[i][j][0] = (float) _r / 255.0;
			M[i][j][1] = (float) _g / 255.0;
			M[i][j][2] = (float) _b / 255.0;
		}	
	}
}

void inv_M () {
	for(int i = 0; i < MAT_LINES; i++) {
		for(int j = 0; j < MAT_COLS; j++) {
			M[i][j][0] = 1.0 - M[i][j][0];
			M[i][j][1] = 1.0 - M[i][j][1];
			M[i][j][2] = 1.0 - M[i][j][2];
		}	
	}

}

void inv_pixel_M( int i, int j ) {
	M[i][j][0] = 1.0 - M[i][j][0];
	M[i][j][1] = 1.0 - M[i][j][1];
	M[i][j][2] = 1.0 - M[i][j][2];
}

void inv_line_M ( int i ) {
	int j;
	for(j = 0; j < MAT_COLS; j++) {
		M[i][j][0] = 1.0 - M[i][j][0];
		M[i][j][1] = 1.0 - M[i][j][1];
		M[i][j][2] = 1.0 - M[i][j][2];
	}
}

void inv_col_M (int j ) {
	int i;
	for(i = 0; i < MAT_LINES; i++) {
		M[i][j][0] = 1.0 - M[i][j][0];
		M[i][j][1] = 1.0 - M[i][j][1];
		M[i][j][2] = 1.0 - M[i][j][2];
	}
}


void set_pixel_Mf( int i, int j, float _r, float _g, float _b) {
	
	M[i][j][0] = _r;
	M[i][j][1] = _g;
	M[i][j][2] = _b;
	
}

void set_line_Mf( int i, float _r, float _g, float _b ) {
	int j;
	for(j = 0; j < MAT_COLS; j++) {
		M[i][j][0] = _r;
		M[i][j][1] = _g;
		M[i][j][2] = _b;
	}
}

void set_col_Mf( int j, float _r, float _g, float _b ) {
	int i;
	for(i = 0; i < MAT_LINES; i++) {
		M[i][j][0] = _r;
		M[i][j][1] = _g;
		M[i][j][2] = _b;
	}		
}

void add_pixel_Mf( int i, int j, float _r, float _g, float _b ) {
	M[i][j][0] = minf(1.0, M[i][j][0] + _r);
	M[i][j][1] = minf(1.0, M[i][j][1] + _g);
	M[i][j][2] = minf(1.0, M[i][j][2] + _b);
}

void add_line_Mf( int i, float _r, float _g, float _b ) {
	int j = 0;
	for(j = 0; j < MAT_COLS; j++) {
		M[i][j][0] = minf(1.0, M[i][j][0] + _r);
		M[i][j][1] = minf(1.0, M[i][j][1] + _g);
		M[i][j][2] = minf(1.0, M[i][j][2] + _b);
	}
}

void add_col_Mf( int j, float _r, float _g, float _b ) {
	int i = 0;
	for(i = 0; i < MAT_LINES; i++) {
		M[i][j][0] = minf(1.0, M[i][j][0] + _r);
		M[i][j][1] = minf(1.0, M[i][j][1] + _g);
		M[i][j][2] = minf(1.0, M[i][j][2] + _b);
	}
}

void clear () { int i; for (i = 0; i < MAT_LINES; i++) set_line_Mf(i, 0, 0, 0); }

void set_pixel_M( int i, int j, int _r, int _g, int _b ) {
	
	M[i][j][0] = (float) _r / 255.0;
	M[i][j][1] = (float) _g / 255.0;
	M[i][j][2] = (float) _b / 255.0;
	
}

void set_line_M( int i, int _r, int _g, int _b ) {
	int j;
	for(j = 0; j < MAT_COLS; j++) {
		M[i][j][0] = (float) _r / 255.0;
		M[i][j][1] = (float) _g / 255.0;
		M[i][j][2] = (float) _b / 255.0;
	}
}

void set_col_M( int j, int _r, int _g, int _b ) {
	int i;
	for(i = 0; i < MAT_LINES; i++) {
		M[i][j][0] = (float) _r / 255.0;
		M[i][j][1] = (float) _g / 255.0;
		M[i][j][2] = (float) _b / 255.0;
	}		
}

void add_pixel_M( int i, int j, int _r, int _g, int _b ) {
	M[i][j][0] = minf(1.0, M[i][j][0] + (float) _r / 255.0);
	M[i][j][1] = minf(1.0, M[i][j][1] + (float) _g / 255.0);
	M[i][j][2] = minf(1.0, M[i][j][2] + (float) _b / 255.0);
}

void add_line_M( int i, int _r, int _g, int _b ) {
	int j = 0;
	for(j = 0; j < MAT_COLS; j++) {
		M[i][j][0] = minf(1.0, M[i][j][0] + (float) _r / 255.0);
		M[i][j][1] = minf(1.0, M[i][j][1] + (float) _g / 255.0);
		M[i][j][2] = minf(1.0, M[i][j][2] + (float) _b / 255.0);
	}
}

void add_col_M( int j, int _r, int _g, int _b ) {
	int i = 0;
	for(i = 0; i < MAT_LINES; i++) {
		M[i][j][0] = minf(1.0, M[i][j][0] + (float) _r / 255.0);
		M[i][j][1] = minf(1.0, M[i][j][1] + (float) _g / 255.0);
		M[i][j][2] = minf(1.0, M[i][j][2] + (float) _b / 255.0);
	}
}

int toint(char *s) {
	int i = 0;
	int r = 0;
	
	for(i = 0; s[i] != '\0'; i++) {
		if(s[i] < '0' || s[i] > '9') return -1;
		r *= 10;
		r += s[i] - '0';
	}
	
	return min(M_MAX-1, r);
}

enum menu_opcoes cmd_val(char *cmd) {

	if(strcmp(cmd, "clean") == 0 
	|| strcmp(cmd, "clear") == 0
	|| strcmp(cmd, "reset") == 0)	return main_opt = CLEAR;
	
	if(strcmp(cmd, "pixel") == 0
	|| strcmp(cmd, "p") == 0)	return main_opt = PIXEL;
	
	if(strcmp(cmd, "column") == 0
	|| strcmp(cmd, "col") == 0
	|| strcmp(cmd, "c") == 0)	return main_opt = COLUMN;
	
	if(strcmp(cmd, "line") == 0
	|| strcmp(cmd, "l") == 0)	return main_opt = LINE;

	if(strcmp(cmd, "invert") == 0
	|| strcmp(cmd, "inv") == 0
	|| strcmp(cmd, "i") == 0)	return main_opt = INV;

	if(strcmp(cmd, "add") == 0)	return main_opt = ADD;
	if(strcmp(cmd, "sub") == 0)	return main_opt = SUB;

	if(strcmp(cmd, "diagonal") == 0
	|| strcmp(cmd, "diag") == 0
	|| strcmp(cmd, "d") == 0)	return main_opt = DIAGONAL;
	
	if(strcmp(cmd, "bye") == 0
	|| strcmp(cmd, "exit") == 0
	|| strcmp(cmd, "quit") == 0)	return main_opt = BYE;
	
	if(strcmp(cmd, "fx") == 0
	|| strcmp(cmd, "effect") == 0)	return main_opt = FX;
	
	return main_opt = NONE;
	
}

void get_colors(char *p1, char *p2, char *p3, float *r, float *g, float *b) {
	if(strcmp(p1, "red") == 0) {
		*r = 1.0;
		*g = 0.0;
		*b = 0.0;
		return;
	}
	
	if(strcmp(p1, "green") == 0) {
		*r = 0.0;
		*g = 1.0;
		*b = 0.0;
		return;
	}
	else if(strcmp(p1, "blue") == 0) {
		*r = 0.0;
		*g = 0.0;
		*b = 1.0;
		return;
	}
	else if(strcmp(p1, "black") == 0 || strcmp(p1, "off") == 0) {
		*r = 0.0;
		*g = 0.0;
		*b = 0.0;
		return ;
	}

	*r = minf(1.0, (float) toint(p1) / 255.0);
	*g = minf(1.0, (float) toint(p2) / 255.0);
	*b = minf(1.0, (float) toint(p3) / 255.0);
	
}

void setup();
void loop ();
void * fx_loop( void *user_data ) {
	int *playing = (int *) user_data;
	sleep(1);
	setup();
	while( *playing ) loop();
	
	return NULL;
}

void * io_handler( void *user_data ) {
	
	char buff[8192];
	char cmd[2048];
	char p1[256];
	char p2[256];
	char p3[256];
	char p4[256];
	char p5[256];
	char p6[256];
	int _x, _y;
	float __r;
	float __g;
	float __b;
	
	pthread_t fx_thread;
	int playing = 0;
	
	printf("-> ");
	while(fgets(buff, 8192, stdin)) {
		printf("-> ");
		sscanf(buff, "%s %s %s %s %s %s %s", cmd, p1, p2, p3, p4, p5, p6);
		switch(cmd_val(cmd)) {
		
			case CLEAR	: clear(); break;
			case PIXEL	:
				_x = toint(p1);
				_y = toint(p2);
				if(_x < 0 || _y < 0) continue;
				
				get_colors(p3, p4, p5, &__r, &__g, &__b);

				if(__r < 0 || __g < 0 || __b < 0) continue;
				set_pixel_Mf(_x, _y, __r, __g, __b);

				break;
			
			case COLUMN	:
				_y = toint(p1);
				
				if(_y < 0) continue;
				
				get_colors(p2, p3, p4, &__r, &__g, &__b);
				
				if(__r < 0 || __g < 0 || __b < 0) continue;
				set_col_Mf(_y, __r, __g, __b);
				
				break;
			
			case LINE	:
				_x = toint(p1);
			
				if(_x < 0) continue;
				
				get_colors(p2, p3, p4, &__r, &__g, &__b);
					
				if(__r < 0 || __g < 0 || __b < 0) continue;
				set_line_Mf(_x,__r, __g, __b);

				break;
				
			case INV	:
				if(strcmp(p1, "all") == 0 || strcmp(p1, "a") == 0) { 
					for(_x = 0; _x < MAT_LINES; _x++) 
						for(_y = 0; _y < MAT_COLS; _y++)
							inv_pixel_M(_x, _y);
				} else if(strcmp(p1, "pixel") == 0 || strcmp(p1, "p") == 0) {
					_x = toint(p2);
					_y = toint(p3);
					
					if(_x < 0 || _y < 0) continue;
					inv_pixel_M(_x, _y);
					
				} else if(strcmp(p1, "column") == 0 || strcmp(p1, "col") == 0 || strcmp(p1, "c") == 0) {
					_y = toint(p2);
					if(_y < 0) continue;
					inv_col_M(_y);
					
				} else if(strcmp(p1, "line") == 0 || strcmp(p1, "l") == 0) {
					_x = toint(p2);
					if(_x < 0) continue;
					inv_line_M(_x);
				}
				break;
				
			case ADD	:
				if(strcmp(p1, "pixel") == 0 || strcmp(p1, "p") == 0) {
					_x = toint(p2);
					_y = toint(p3);
					if(_x < 0 || _y < 0) continue;
					
					get_colors(p4, p5, p6, &__r, &__g, &__b);
					
					if(__r < 0 || __g < 0 || __b < 0) continue;				
					add_pixel_Mf(_x, _y, __r, __g, __b);
										
					
				} else if(strcmp(p1, "column") == 0 || strcmp(p1, "col") == 0 || strcmp(p1, "c") == 0) {
					_y = toint(p2);
					if(_y < 0) continue;
					
					get_colors(p3, p4, p5, &__r, &__g, &__b);
					
					if(__r < 0 || __g < 0 || __b < 0) continue;				
					add_col_Mf(_y, __r, __g, __b);
					
					
				} else if(strcmp(p1, "line") == 0 || strcmp(p1, "l") == 0) {
					_x = toint(p2);
					if(_x < 0) continue;
					
					get_colors(p3, p4, p5, &__r, &__g, &__b);
										
					if(__r < 0 || __g < 0 || __b < 0) continue;				
					add_line_Mf(_x, __r, __g, __b);
					
				}
				
				break;
				
			case FX		:
				if(strcmp(p1, "start") == 0 || strcmp(p1, "begin") == 0) {
					if(!playing) {
						playing = 1;
						pthread_create(&fx_thread, NULL, fx_loop, (void *) &playing);
					}
				} else {
					if(playing) {
						playing = 0;
						pthread_join(fx_thread, NULL);
					}
				}			
				break;

			case DIAGONAL	:
				break;


			case BYE	: goto JMP;			
			
			default : break;
		}
	
	}

	JMP:
	if(playing) {
		playing = 0;
		pthread_join(fx_thread, NULL);
	}
	printf("\nBye!\n");

	glutLeaveMainLoop();
	return NULL;
}


int main (int argc, char *argv[]) {
	
	
	setup();
	
	orthoX = 10 * MAT_COLS;
	orthoY = 10 * MAT_LINES;
	LED_COUNT = MAT_COLS * MAT_LINES;
	
	glutInit(&argc, argv);

	pthread_t io_thread;
	pthread_create(&io_thread, NULL, io_handler, NULL);
	
	main_opt = NONE;

	// Dois buffers, janela 600x450
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenW, screenH);
	glutInitWindowPosition(10,10);
	glutCreateWindow("SpecialFX");
	
	glMatrixMode(GL_PROJECTION);
	glViewport (0, 0, orthoX, orthoY);
	gluOrtho2D (0, orthoX, orthoY, 0 );
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	
	glutDisplayFunc(refresh_screen);
	glutMainLoop();

	pthread_join(io_thread, NULL);

	return 0;
	
}


void set_line_range ( int i, int start, int end, int r, int g, int b ) {

	int j = 0;
	
	if(start > end) {
	
		j = start;
		start = end;
		end = j;
		
	}
	
	end = min(end, MAT_COLS);
	
	for(j = start; j < end; j++) {
		M[i][j][0] = (float) r / 255.0;
		M[i][j][1] = (float) g / 255.0;
		M[i][j][2] = (float) b / 255.0;
	}

}

void set_col_range ( int j, int start, int end, int r, int g, int b ) {

	int i = 0;
	
	if(start > end) {
	
		j = start;
		start = end;
		end = j;
		
	}
	
	end = min(end, MAT_LINES);
	
	for(i = start; i < end; i++) {
		M[i][j][0] = (float) r / 255.0;
		M[i][j][1] = (float) g / 255.0;
		M[i][j][2] = (float) b / 255.0;
	}
	
}

void add_line_range ( int i, int start, int end, int r, int g, int b ) {
	
	int j = 0;
	
	if(start > end) {
	
		j = start;
		start = end;
		end = j;
		
	}
	
	end = min(end, MAT_COLS);
	
	for(j = start; j < end; j++) {
		M[i][j][0] = minf(1.0, M[i][j][0] + (float) r / 255.0);
		M[i][j][1] = minf(1.0, M[i][j][1] + (float) g / 255.0);
		M[i][j][2] = minf(1.0, M[i][j][2] + (float) b / 255.0);
	}

}

void add_col_range ( int j, int start, int end, int r, int g, int b ) {

	int i = 0;
	
	if(start > end) {
	
		j = start;
		start = end;
		end = j;
		
	}
	
	end = min(end, MAT_LINES);
	
	for(i = start; i < end; i++) {
		M[i][j][0] = minf(1.0, M[i][j][0] + (float) r / 255.0);
		M[i][j][1] = minf(1.0, M[i][j][1] + (float) g / 255.0);
		M[i][j][2] = minf(1.0, M[i][j][2] + (float) b / 255.0);
	}
	
}

void inv_line_range ( int i, int start, int end ) {
	
	int j;
	if(start > end) {
	
		j = start;
		start = end;
		end = j;
		
	}
	end = min(end, MAT_COLS);
	
	for(j = start; j < end; j++) {
		M[i][j][0] = 1.0 - M[i][j][0];
		M[i][j][1] = 1.0 - M[i][j][1];
		M[i][j][2] = 1.0 - M[i][j][2];
	}

}

void inv_col_range ( int j, int start, int end ) {

	int i = 0;
	if(start > end) {
	
		j = start;
		start = end;
		end = j;
		
	}
	end = min(end, MAT_LINES);
	
	for(i = start; i < end; i++) {
		M[i][j][0] = 1.0 - M[i][j][0];
		M[i][j][1] = 1.0 - M[i][j][1];
		M[i][j][2] = 1.0 - M[i][j][2];
	}

}

void set_strip ( int n, int r, int g, int b ) {
	int i, j;
	
	i = n / MAT_COLS;
	j = (i%2)?(MAT_COLS - (n % MAT_COLS) - 1):(n % MAT_COLS);
	
	M[i][j][0] = (float) r / 255.0;
	M[i][j][1] = (float) g / 255.0;
	M[i][j][2] = (float) b / 255.0;

}

#define LED_PIN 0
#define NEO_KHZ800 0
#define NEO_KHZ400 0
#define NEO_GRB 0
#define NEO_RGB 0
#define NEO_RGBW 0

class Adafruit_NeoPixel { 

	public :
	
		Adafruit_NeoPixel(int, int, int) { }
	
		void begin() { }
		void clear() { int i; for (i = 0; i < MAT_LINES; i++) set_line_Mf(i, 0, 0, 0); }
		void show()  { int i, j, k; for(i = 0; i < MAT_LINES; i++) for(j = 0; j < MAT_COLS; j++) for(k = 0; k < 3; k++) RM[i][j][k] = M[i][j][k]; }
		
		void setPixelColor( int n, int r, int g, int b ) { 
		
			int i, j;
	
			i = n / MAT_COLS;
			j = (i%2)?(MAT_COLS - (n % MAT_COLS) - 1):(n % MAT_COLS);
			
			M[i][j][0] = (float) r / 255.0;
			M[i][j][1] = (float) g / 255.0;
			M[i][j][2] = (float) b / 255.0;
		
		}
		
		void setPixelColor( int n, int color ) { 
		
			int i, j;
			int r = (color & 0xFF0000) >> 16;
			int g = (color & 0x00FF00) >> 8;
			int b = color & 0x0000FF;
	
			i = n / MAT_COLS;
			j = (i%2)?(MAT_COLS - (n % MAT_COLS) - 1):(n % MAT_COLS);
			
			M[i][j][0] = (float) r / 255.0;
			M[i][j][1] = (float) g / 255.0;
			M[i][j][2] = (float) b / 255.0;
		
		}
		
		int getPixelColor( int n ) {
			int i, j;
			
			i = n / MAT_COLS;
			j = (i%2)?(MAT_COLS - (n % MAT_COLS) - 1):(n % MAT_COLS);
			
			return (((int) (M[i][j][0] * 255)) << 16) + (((int) (M[i][j][1] * 255)) << 8) + (int) (M[i][j][2] * 255);
		}
		
		int Color( int r, int g, int b) {
			return (r << 16) | (g << 8) | b;
		}

};
