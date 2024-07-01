#include <GL/glut.h> 
GLint ancho, alto; 
int hazPerspectiva = 1; 

// Perspective parameters
GLfloat fov = 175.0f;
GLfloat nearClipPersp = 0.1f;
GLfloat farClipPersp = 1.5f;

// Orthographic parameters
GLfloat leftOrtho = -4.0f, rightOrtho = 4.0f;
GLfloat bottomOrtho = -4.0f, topOrtho = 4.0f;
GLfloat nearClipOrtho = 0.1f, farClipOrtho = 1.5f;

void reshape(int width, int height) 
{ 
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	
	if(hazPerspectiva) 
		gluPerspective(fov, (GLfloat)width/(GLfloat)height, nearClipPersp, farClipPersp); 
	else 
		glOrtho(leftOrtho, rightOrtho, bottomOrtho, topOrtho, nearClipOrtho, farClipOrtho); 
	
	glMatrixMode(GL_MODELVIEW); 
	
	ancho = width; 
	alto = height; 
} 


void display() 
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity(); 
	
	glutWireTeapot(1.5f);
	
	glColor3f(1.0f, 1.0f, 1.0f); 
	
	glFlush(); 
	glutSwapBuffers(); 
} 

void init() 
{ 
	glClearColor(0,0,0,0); 
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
		//Change proyection
		case 'O':
		case 'o':
			hazPerspectiva=0; 
			reshape(ancho,alto); 
			break; 
		case 'P':
		case 'p':
			hazPerspectiva=1; 
			reshape(ancho,alto); 
			break; 
			
		//Ortogonal
		case 'i':  
			leftOrtho-=0.1;
			reshape(ancho,alto); 
			break; 
		case 'd':  
			rightOrtho+=0.1;
			reshape(ancho,alto); 
			break; 
		case 'a': 
			bottomOrtho-=0.1;
			reshape(ancho,alto); 
			break; 
		case 'u': 
			topOrtho+=0.1;
			reshape(ancho,alto); 
			break; 
			
		//Perspectiva
		case 'b': 
			fov-=0.1;
			reshape(ancho,alto); 
			break; 
		case 'B': 
			fov+=0.1;
			reshape(ancho,alto); 
			break; 
			
		//Near and far clip
		case 'n': 
			if(hazPerspectiva){
				nearClipPersp-=0.1;
			}
			else{
				nearClipOrtho-=0.1;
			}
			reshape(ancho,alto); 
			break; 
		case 'N': 
			if(hazPerspectiva){
				nearClipPersp+=0.1;
			}
			else{
				nearClipOrtho+=0.1;
			}
			reshape(ancho,alto); 
			break; 
		case 'f': 
			if(hazPerspectiva){
				farClipPersp-=0.1;
			}
			else{
				farClipOrtho-=0.1;
			}
			reshape(ancho,alto); 
			break; 
		case 'F': 
			if(hazPerspectiva){
				farClipPersp+=0.1;
			}
			else{
				farClipOrtho+=0.1;
			}
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowPosition(100, 100); 
	glutInitWindowSize(ancho, alto); 
	
	glutCreateWindow("Teapot"); 
	init(); 
	
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape); 
	glutIdleFunc(idle); 
	glutKeyboardFunc(keyboard); 
	
	glutMainLoop(); 
	return 0; 
}

