#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <GL/glut.h>

#define M_MAX 1000

int screenW = 600;
int screenH = 480;
float M[M_MAX][M_MAX][3] = { 0 };

#define M_W 12
#define M_H 12

#define orthoX 120
#define orthoY 120

int lineL = orthoX / M_H;
int colL  = orthoY / M_W;

void paint( int i, int j, float _r, float _g, float _b) {

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

	for(i = 0; i < M_H; i++)
		for(j = 0; j < M_W; j++)
			paint(j, i, M[i][j][0], M[i][j][1], M[i][j][2]);

}


void refresh_screen(void) {

	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	
	draw_matrix();
	
	glColor3f(0.0f,0.0f,0.0f);
	glutSwapBuffers();
	
}

void inv_M( int i, int j ) {
	M[i][j][0] = 1 - M[i][j][0];
	M[i][j][1] = 1 - M[i][j][1];
	M[i][j][2] = 1 - M[i][j][2];
}

void set_M( int i, int j, float _r, float _g, float _b) {
	
	M[i][j][0] = _r;
	M[i][j][1] = _g;
	M[i][j][2] = _b;
	
}

void set_line_M( int i, float _r, float _g, float _b ) {
	int j;
	for(j = 0; j < M_W; j++) {
		M[i][j][0] = _r;
		M[i][j][1] = _g;
		M[i][j][2] = _b;
	}
}

void set_col_M( int j, float _r, float _g, float _b ) {
	int i;
	for(i = 0; i < M_H; i++) {
		M[i][j][0] = _r;
		M[i][j][1] = _g;
		M[i][j][2] = _b;
	}		
}

void show () { glutPostRedisplay(); }

int min(int a, int b) { return a<b?a:b; }
float minf(float a, float b) { return a<b?a:b; }

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

void * iohandler( void *user_data ) {
	
	char buff[8192];
	char cmd[2048];
	char p1[256];
	char p2[256];
	char p3[256];
	char p4[256];
	char p5[256];
	int _x, _y;
	float __r;
	float __g;
	float __b;
	
	printf("-> ");
	while(fgets(buff, 8192, stdin)) {
		printf("-> ");
		sscanf(buff, "%s %s %s %s %s %s", cmd, p1, p2, p3, p4, p5);
		if(strcmp(cmd, "show") == 0) { show(); }
		else if(strcmp(cmd, "pixel") == 0 || strcmp(cmd, "p") == 0) {
			_x = toint(p1);
			_y = toint(p2);
			if(_x < 0 || _y < 0) continue;
			if(strcmp(p3, "red") == 0) 	  set_M(_x, _y, 1, 0, 0);
			else if(strcmp(p3, "green") == 0) set_M(_x, _y, 0, 1, 0);
			else if(strcmp(p3, "blue") == 0)  set_M(_x, _y, 0, 0, 1);
			else {
				__r = minf(1.0, (float) toint(p3) / 255.0);
				__g = minf(1.0, (float) toint(p4) / 255.0);
				__b = minf(1.0, (float) toint(p5) / 255.0);
				
				if(__r < 0 || __g < 0 || __b < 0) continue;
				set_M(_x, _y, __r, __g, __b);
			}
			
		} else if(strcmp(cmd, "column") == 0 || strcmp(cmd, "col") == 0 || strcmp(cmd, "c") == 0) { 
			_y = toint(p1);
			
			if(_y < 0) continue;
			if(strcmp(p2, "red") == 0) 	  set_col_M(_y, 1, 0, 0);
			else if(strcmp(p2, "green") == 0) set_col_M(_y, 0, 1, 0);
			else if(strcmp(p2, "blue") == 0)  set_col_M(_y, 0, 0, 1);
			else {
				__r = minf(1.0, (float) toint(p2) / 255.0);
				__g = minf(1.0, (float) toint(p3) / 255.0);
				__b = minf(1.0, (float) toint(p4) / 255.0);
				
				if(__r < 0 || __g < 0 || __b < 0) continue;
				
				set_col_M(_y, __r, __g, __b);
			}
		} else if(strcmp(cmd, "line") == 0 || strcmp(cmd, "l") == 0) {
			_x = toint(p1);
		
			if(_x < 0) continue;
			if(strcmp(p2, "red") == 0) 	  set_line_M(_x, 1, 0, 0);
			else if(strcmp(p2, "green") == 0) set_line_M(_x, 0, 1, 0);
			else if(strcmp(p2, "blue") == 0)  set_line_M(_x, 0, 0, 1);
			else {
				__r = minf(1.0, (float) toint(p3) / 255.0);
				__g = minf(1.0, (float) toint(p4) / 255.0);
				__b = minf(1.0, (float) toint(p5) / 255.0);
				
				if(__r < 0 || __g < 0 || __b < 0) continue;
				set_line_M(_x,__r, __g, __b);
			}
		} else if(strcmp(cmd, "invert") == 0 || strcmp(cmd, "inv") == 0 || strcmp(cmd, "i") == 0) { 
			if(strcmp(p1, "all") == 0 || strcmp(p1, "a") == 0) { 
				for(_x = 0; _x < M_H; _x++) 
					for(_y = 0; _y < M_W; _y++)
						inv_M(_x, _y);
			} else if(strcmp(p1, "pixel") == 0 || strcmp(p1, "p") == 0) {
				_x = toint(p2);
				_y = toint(p3);
				
				if(_x < 0 || _y < 0) continue;
				inv_M(_x, _y);
			} else if(strcmp(p1, "column") == 0 || strcmp(p1, "col") == 0 || strcmp(p1, "c") == 0) {
				_y = toint(p2);
				if(_y < 0) continue;
				for(_x = 0; _x < M_H; _x++) inv_M(_x, _y);
			} else if(strcmp(p1, "line") == 0 || strcmp(p1, "l") == 0) {
				_x = toint(p2);
				if(_x < 0) continue;
				for(_y = 0; _y < M_W; _y++) inv_M(_x, _y);
			}
		} else if(strcmp(cmd, "diagonal") == 0 || strcmp(cmd, "diag") == 0 || strcmp(cmd, "d") == 0) {
		} else { }
	}
	
	printf("\nBye!\n");
	exit (0);

}

int main (int argc, char *argv[]) {
	
	glutInit(&argc, argv);

	pthread_t io_thread;
	pthread_create(&io_thread, NULL, iohandler, NULL);

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

	return 0;
	
}
