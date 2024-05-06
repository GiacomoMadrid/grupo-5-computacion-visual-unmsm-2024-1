
#include <stdio.h>

// variables globales
int clickedX = -1, clickedY = -1;
int x0, y0;
int counter = 0;

// prototipos de funciones
void reshape(int width, int height);
void drawPixel(int x, int y);
void drawLine(int x0, int y0, int x1, int y1);
void display();
void init();
void mouseCallback(int button, int state, int x, int y);

// funcion principal
int main(int argc, char** argv) {
	// inicializamos el GLUT y configuramos la ventana
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(250, 50);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Hello OpenGL");
	
	// registramos las funciones callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseCallback);
	
	// inicializamos OpenGL
	init();
	
	// incluimos el bucle principal de eventos GLUT
	glutMainLoop();
	
	return 0;
}

// funcion callback para manejar eventos de redimensionamiento de la ventana
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

// funion para dibujar un píxel
void drawPixel(int x, int y) {
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0);
}

// funcion para dibujar una linea
void drawLine(int x0, int y0, int x1, int y1) {
	// algoritmo de Bresenham
	int dx = abs(x0 - x1);
	int dy = -abs(y0 - y1);
	int e = dx + dy;
	int ix = x1 > x0 ? 1:-1;
	int iy = y1 > y0 ? 1:-1;
	int x = x0;
	int y = y0;
	int de = 2*e;
	printf("points: ");
	while (1){
		drawPixel(x, y);
		printf("(%i, %i) ", x, y);
		if ((x == x1 && x0 != x1) || (y == y1 && y0 != y1)) {
			break;
		}
		if (de >= dy) {
			de += dy;
			x += ix;
		}
		if (de <= dx) {
			de += dx;
			y += iy;
		}
	};
	printf("\n-----------------------------------------\n");
}

// Función de callback para manejar eventos de clic del mouse
void mouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		clickedX = x;
		clickedY = glutGet(GLUT_WINDOW_HEIGHT) - y; // invertimos la coordenada 'y'
		glutPostRedisplay(); //  redibujamos la ventana
		
		if (counter == 0) {
			printf("x0: %i, y0: %i\n", clickedX, clickedY);
			x0 = clickedX;
			y0 = clickedY;
		} else if (counter == 1) {
			printf("x1: %i, y1: %i\n", clickedX, clickedY);
		}
		counter += 1; // incrementamos el contador
	}
}

// funcion de inicializacion de OpenGL
void init() {
	glClearColor(0, 0, 0, 0); // establecemos el color de fondo a negro
}

// funcion de dibujado principal
void display() {
	// si las coordenadas son validas
	if (clickedX != -1 && clickedY != -1) {
		// dibujamos el pixel
		drawPixel(clickedX, clickedY);
		// si el click es de orden impar
		if (counter == 1) {
			x0 = clickedX;
			y0 = clickedY;
		};
		// si el clicl es de orden par
		if (counter == 2) {
			// graficamos la linea
			drawLine(x0, y0, clickedX, clickedY);
			// reiniciamos el contador
			counter = 0;
		}
	}
	glEnd();
	// forzamos el dibujado de los comandos en el buffer
	glFlush();
}
