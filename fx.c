#include <GL/glut.h>

int screenW = 600;
int screenH = 480;
float M[1000][1000][3] = { 0 };

#define M_W 12
#define M_H 12

#define orthoX 120
#define orthoY 120

int lineL = orthoX / M_W;
int colL  = orthoY / M_H;
int rr = 255, gg = 0, bb = 0;

void drawline (float x1, float y1, float x2, float y2)
{
    glBegin (GL_LINES);
    glVertex2f (x1, y1);
    glVertex2f (x2, y2);
    glEnd();
}

void draw_grid(int w, int h) {
	int i;
	
	glColor3f(0.0, 0.0, 0.0);
	for(i = 0; i <= orthoX; i += lineL) {
		glBegin(GL_LINES);
		glVertex2f (0.0, i);
		glVertex2f (orthoY, i);
		glEnd();
	}
	
	for(i = 0; i <= orthoY; i += colL) {
		glBegin(GL_LINES);
		glVertex2f (i, 0.0);
		glVertex2f (i, orthoX);
		glEnd();	
	}

}

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

	for(i = 0; i < M_W; i++)
		for(j = 0; j < M_H; j++)
			paint(i, j, M[i][j][0], M[i][j][1], M[i][j][2]);

}

void set_M( int i, int j, float _r, float _g, float _b) {

	if(_r == M[i][j][0] && _g == M[i][j][1] && _b == M[i][j][2]) return;
	
	M[i][j][0] = _r;
	M[i][j][1] = _g;
	M[i][j][2] = _b;
	
	glutPostRedisplay();
}

void refresh_screen(void) {

	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT);
	
	draw_matrix();
	
	set_M(10, 1, 1, 0, 0);

	glColor3f(0.0f,0.0f,0.0f);
	glutSwapBuffers();
	
}

int main (int argc, char *argv[]) {
	
	glutInit(&argc, argv);

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
