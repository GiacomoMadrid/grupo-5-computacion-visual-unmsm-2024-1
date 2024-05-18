#include <GL/glut.h> 
#include <stdio.h> 


int x1,x2,y1,y2;

void reshape(int width, int height) 
{ 
	//Viewport covers the entire window
	glViewport(0, 0, width, height); 
	
	//Sets up the projection matrix as orthographic projection
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	glOrtho(-100, 100, -100, 100, -100, 100); 
	
	//Switch back to the modelview matrix for transformations
	glMatrixMode(GL_MODELVIEW); 
} 

void write_pixel(int x, int y)
{
	glBegin(GL_POINTS); 
	glVertex3f(x,y,0); 
	glEnd(); 
}

void inc_line(int x1, int y1, int x2,int y2)
{
	int dx, dy, incP, incN, cond, x, y,t;
	float pend=0;
	
	
	//Línea vertical
	if(x2-x1==0){
		//Restricción para que y se incremente
		if (y1 > y2) {
			t = y1;
			y1 = y2;
			y2 = t;
		}
		
		for (y = y1; y <= y2; y++) {
			write_pixel(x1, y);
		}
	}
	
	
	//Linea horizontal y diagonal
	else{
		//Restricción para que x se incremente
		if(x1>x2){
			t=x1;
			x1=x2;
			x2=t;
			
			t=y1;
			y1=y2;
			y2=t;
		}
		
		pend=(float)(y2-y1)/(x2-x1) ;
		
		//Inicializa variables
		dx = abs(x2-x1);
		dy = abs(y2-y1);
		cond = 2*dy-dx;
		x = x1;
		y = y1;

		write_pixel(x,y);
		
		
		while(x < x2)
		{
			//Actualiza x
			x = x + 1;
			
			incP = 2*dy;//Incremental cuando cond es +
			incN = 2*(dy-dx);//IncrementaL cuando cond es -

			//Actualiza y
			if(cond<=0)
			{
				cond = cond+incP;
			}
			else
			{
				cond = cond + incN;
				
				if(pend>0){
					y = y + 1;
				}
				else{
					y = y - 1;
				}
				
			}
			
			write_pixel(x,y);
			
			dx = abs(x2-x);
			dy = abs(y2-y);
		}
	}
}

void display() 
{ 
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT); 
	//Define initial color of pixels to be drawn
	glColor3f(0,1,0); 
	//Reset matrixes
	glLoadIdentity(); 
	//Draw line
	inc_line(x1, y1, x2, y2);
	//Ensures that rendering commands are processed immediately
	glFlush(); 
} 

void init() 
{ 
	//Sets the color used to clear the color buffer to black
	glClearColor(0,0,0,0); 
} 

int main(int argc, char **argv) 
{ 
	printf("Ingresar x1, y1, x2, y2 \n");
	scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
	
	//Parses the command-line arguments and initializes the GLUT state
	glutInit(&argc, argv); 
	//Sets the display mode: RGB color mode and a single buffer for 2D rendering
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	//Sets the window initial position
	glutInitWindowPosition(50, 50); 
	//Sets the window initial size
	glutInitWindowSize(500, 500); 
	//Creates a window with the specified title
	glutCreateWindow("Hello OpenGL"); 

	//Function to initialize states
	init(); 
	//Sets the display callback function: called whenever the contents of the window need to be drawn
	glutDisplayFunc(display); 
	//Sets the reshape callback function: called when the thw windows is reshaped allowing viewport and projection matrix adjustment 
	glutReshapeFunc(reshape); 
	
	//Enters the GLUT event processing loops, checking for and processing events
	glutMainLoop(); 
	
	return 0; 
} 
