
#include <GL/glut.h> 

GLfloat anguloCuboX = 0.0f;  
GLfloat anguloCuboY = 0.0f; 
GLfloat anguloEsfera = 0.0f; 
GLfloat desp = 0.0f; 

GLint ancho, alto; 
int hazPerspectiva = 0; 

void reshape(int width, int height) 
{ 
	glViewport(0, 0, width, height); 
	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	//Defines perpective proyection
	if(hazPerspectiva) 
		gluPerspective(90.0f, (GLfloat)width/(GLfloat)height, 1.0f, 20.0f); 
	//Defines orthographic projection
	else 
		glOrtho(-4, 4, -4, 4, 1, 10); 
	
	glMatrixMode(GL_MODELVIEW); 
	
	ancho = width; 
	alto = height; 
} 

void drawCube(void) 
{ 
	glColor3f(1.0f, 0.0f, 0.0f); 
	glBegin(GL_QUADS);  //cara frontal 
	glVertex3f(-1.0f, -1.0f,  1.0f); 
	glVertex3f( 1.0f, -1.0f,  1.0f); 
	glVertex3f( 1.0f,  1.0f,  1.0f); 
	glVertex3f(-1.0f,  1.0f,  1.0f); 
	glEnd(); 
	
	glColor3f(0.0f, 1.0f, 0.0f); 
	glBegin(GL_QUADS);  //cara trasera 
	glVertex3f( 1.0f, -1.0f, -1.0f); 
	glVertex3f(-1.0f, -1.0f, -1.0f); 
	glVertex3f(-1.0f,  1.0f, -1.0f); 
	glVertex3f( 1.0f,  1.0f, -1.0f); 
	glEnd(); 
	
	glColor3f(0.0f, 0.0f, 1.0f); 
	glBegin(GL_QUADS);  //cara lateral izq 
	glVertex3f(-1.0f, -1.0f, -1.0f); 
	glVertex3f(-1.0f, -1.0f,  1.0f); 
	glVertex3f(-1.0f,  1.0f,  1.0f); 
	glVertex3f(-1.0f,  1.0f, -1.0f); 
	glEnd();
	glColor3f(1.0f, 1.0f, 0.0f); 
	glBegin(GL_QUADS);  //cara lateral dcha 
	glVertex3f( 1.0f, -1.0f,  1.0f); 
	glVertex3f( 1.0f, -1.0f, -1.0f); 
	glVertex3f( 1.0f,  1.0f, -1.0f); 
	glVertex3f( 1.0f,  1.0f,  1.0f); 
	glEnd(); 
	
	glColor3f(0.0f, 1.0f, 1.0f); 
	glBegin(GL_QUADS);  //cara arriba 
	glVertex3f(-1.0f,  1.0f,  1.0f); 
	glVertex3f( 1.0f,  1.0f,  1.0f); 
	glVertex3f( 1.0f,  1.0f, -1.0f); 
	glVertex3f(-1.0f,  1.0f, -1.0f); 
	glEnd(); 
	
	glColor3f(1.0f, 0.0f, 1.0f); 
	glBegin(GL_QUADS);  //cara abajo 
	glVertex3f( 1.0f, -1.0f, -1.0f); 
	glVertex3f( 1.0f, -1.0f,  1.0f); 
	glVertex3f(-1.0f, -1.0f,  1.0f); 
	glVertex3f(-1.0f, -1.0f, -1.0f); 
	glEnd(); 
	
} 

void display() 
{ 
	//Clears color and depth/Z buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	glLoadIdentity(); 
	
	//Transforms and draws cube
	glTranslatef(0.0f, 0.0f, -5.0f); 
	glRotatef(anguloCuboX, 1.0f, 0.0f, 0.0f); 
	glRotatef(anguloCuboY, 0.0f, 1.0f, 0.0f); 
	glTranslatef(desp, 0.0f, 0.0f);
	drawCube(); 
	
	glFlush(); 
	
	//Swaps the front and back buffers, displaying the rendered image
	glutSwapBuffers(); 
} 

void init() 
{ 
	glClearColor(0,0,0,0); 
	
	//Allow to render objects in the correct order based on their depth
	glEnable(GL_DEPTH_TEST); 
	
	ancho = 400; 
	alto = 400; 
} 

void idle() 
{ 
	display(); 
} 

void keyboard(unsigned char key, int x, int y) 
{ 
	switch(key) 
	{ 
	case 'r': 
		anguloCuboY+=10.0f;
		hazPerspectiva=1; 
		
		//Sets new type of projection
		reshape(ancho,alto); 
		break; 
	case 'l': 
		anguloCuboY-=10.0f; 
		hazPerspectiva=1; 
		
		//Sets new type of projection
		reshape(ancho,alto); 
		break; 
	case 'u': 
		anguloCuboX-=10.0f; 
		hazPerspectiva=1; 
		reshape(ancho,alto); 
		break; 
	case 'd': 
		anguloCuboX+=10.0f; 
		hazPerspectiva=1; 
		
		//Sets new type of projection
		reshape(ancho,alto); 
		break; 
	case 'f': 
		hazPerspectiva=0; 
		desp +=0.5f; 
		
		//Sets new type of projection
		reshape(ancho,alto); 
		break; 
	case 'b': 
		hazPerspectiva=0; 
		desp -=0.5f; 
		
		//Sets new type of projection
		reshape(ancho,alto); 
		break; 
	case 27:   // escape 
		exit(0); 
		break; 
		
	} 
} 

int main(int argc, char **argv) 
{ 
	glutInit(&argc, argv); 
	//Sets 2 bufferes for 3D: a back buffer that gets swapped with the front buffer when rendering is complete 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(ancho, alto); 
	glutCreateWindow("Cubo 1"); 
	
	init(); 
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	//Sets the idle callback function: called whenever there are no other events pending
	glutIdleFunc(idle); 
	//Sets the keyboard callback function: called whenever a keyboard key is pressed
	glutKeyboardFunc(keyboard); 
	glutMainLoop(); 
	
	return 0; 
}
