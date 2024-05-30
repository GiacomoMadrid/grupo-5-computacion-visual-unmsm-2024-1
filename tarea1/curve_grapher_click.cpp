#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// variables globales
int x_0 = -1, y_0 = -1, x_1 = -1, y_1 = -1, x_2 = -1, y_2 = -1;
int counter = 0;
int clickedX = -1, clickedY = -1;
int x_con_main = -1, y_con_main = -1;

struct Point {
	int x;
	int y;
};

vector<Point> allPointsOfCurve;

// prototipos de funciones
void mouseCallback(int button, int state, int x, int y);
void reshape(int width, int height);
void obtainData();
void drawPixel(int x, int y);
vector<Point> getMissingPoints(int x_0, int y_0, int x_1, int y_1, vector<Point> allPointsOfCurve);
void fillConcavityPoints(int x_0, int y_0, int x_1, int y_1, int xa, int ya, vector<Point> &concavityPoints);
bool compareX(const Point &a, const Point &b);
vector<Point> getAllPointsOfCurve(int x_0, int y_0, int x_1, int y_1, int xa, int ya);
void drawCurve();
void display();
void init();

// funcion principal
int main(int argc, char** argv) {
	// pedimos datos
	//obtainData();
	
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

// funcion de callback para manejar eventos de clic del mouse
void mouseCallback(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		clickedX = x;
		clickedY = glutGet(GLUT_WINDOW_HEIGHT) - y; // invertimos la coordenada 'y'
		glutPostRedisplay(); //  redibujamos la ventana
		
		if (counter == 0) {
			printf("\nx0: %i, y0: %i", clickedX, clickedY);
			
			glClear(GL_COLOR_BUFFER_BIT);
			
			x_1 = -1;
			y_1 = -1;
			
			x_2 = -1;
			y_2= -1;
			
			x_0 = clickedX;
			y_0 = clickedY;
			
			drawPixel(x_0, y_0);
			counter += 1; // incrementamos el contador
		} else if (counter == 1) {
			printf("\nx1: %i, y1: %i", clickedX, clickedY);
			x_1 = clickedX;
			y_1 = clickedY;
			
			drawPixel(x_1, y_1);
			
			counter = 0; // reiniciamos el contador
		}
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		clickedX = x;
		clickedY = glutGet(GLUT_WINDOW_HEIGHT) - y; // invertimos la coordenada 'y'
		glutPostRedisplay(); //  redibujamos la ventana
		
		x_con_main = -1;
		y_con_main = -1;
		
		printf("\nx2: %i, y2: %i\n", clickedX, clickedY);
		x_2 = clickedX;
		y_2 = clickedY;
	}
}

// funcion callback para manejar eventos de redimensionamiento de la ventana
void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

// funcion para pedir datos
void obtainData() {
	printf("--- Ingrese los datos ---\n");
	printf("x_0: ");
	scanf("%d", &x_0);
	printf("y_0: ");
	scanf("%d", &y_0);
	printf("x_1: ");
	scanf("%d", &x_1);
	printf("y_1: ");
	scanf("%d", &y_1);
	printf("x_2: ");
	scanf("%d", &x_2);
	printf("y_2: ");
	scanf("%d", &y_2);
}

// funion para dibujar un pÃ­xel
void drawPixel(int x, int y) {
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0);
}

// funcion para obtener un vector de todos los puntos de una linea entre dos puntos
vector<Point> getMissingPoints(int x_0, int y_0, int x_1, int y_1, vector<Point> allPointsOfCurve) {
	// algoritmo de Bresenham
	int dx = abs(x_0 - x_1);
	int dy = abs(y_0 - y_1);
	int e = dx - dy;
	int ix = x_1 > x_0 ? 1 : -1;
	int iy = y_1 > y_0 ? 1 : -1;
	int x = x_0;
	int y = y_0;
	int de;
	vector<Point> missingPoints;
	while (1) {
		Point p = {x, y};
		bool found = false;
		for (const auto& point : allPointsOfCurve) {
			if (point.x == p.x && point.y == p.y) {
				found = true;
				break;
			}
		}
		if (!found) {
			missingPoints.push_back(p);
		}
		if ((x == x_1) && (y == y_1)) {
			break;
		}
		de = 2 * e;
		if (de > -dy) {
			e -= dy;
			x += ix;
		}
		if (de < dx) {
			e += dx;
			y += iy;
		}
	}
	return missingPoints;
}

// funcion para dibujar una linea
void drawLine(int x0, int y0, int x1, int y1) {
	// algoritmo de Bresenham
	int dx = abs(x0 - x1);
	int dy = abs(y0 - y1);
	int e = dx - dy;
	int ix = x1 > x0 ? 1 : -1;
	int iy = y1 > y0 ? 1 : -1;
	int x = x0;
	int y = y0;
	int de;
	while (1) {
		drawPixel(x, y);
		//printf("(%i, %i) ", x, y);
		if ((x == x1) && (y == y1)) {
			break;
		}
		de = 2 * e;
		if (de > -dy) {
			e -= dy;
			x += ix;
		}
		if (de < dx) {
			e += dx;
			y += iy;
		}
	}
}

float getDistancePointToRect(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2) {
	printf("\nCalculando distancia");
	float maxX = x_1 > x_0 ? x_1 : x_0;
	float minX = x_1 == maxX ? x_0 : x_1;
	bool divZero = x_1 != x_0 ? true : false; 
	float m  =  divZero ? (y_1 - y_0) / (x_1 - x_0) : 1;
	float a = m;
	float b = -1;
	float c = y_0 - m*x_0;
	
	printf("\nmaxX: ", maxX);
	printf("\nminX: ", minX);
	printf("\ndivZero: ", divZero);
	printf("\nm: ", m);
	printf("\na: ", a);
	printf("\nb: ", b);
	printf("\nc: ", c);
	
	// distance = |a*x_2 + b*y_2 + c| / sqrt(a*a + b*b)
	double aux = a*x_2 + b*y_2 + c >= 0 ? a*x_2 + b*y_2 + c : -1*(a*x_2 + b*y_2 + c);
	double distance =  divZero ? aux / pow(a*a + b*b, 0.5) : maxX - minX;
	printf("\naux: ", aux);
	printf("\ndistance: ", distance);
	
	return (float)distance;
}

void fillConcavityPoints(int x_0, int y_0, int x_1, int y_1, int xa, int ya, vector<Point> &concavityPoints) {
	int x_0a = ((x_0 + xa) % 2) == 0 ? (x_0 + xa) / 2 : ((int)((x_0 + xa) / 2)) + 1;
	int y_0a = ((y_0 + ya) % 2) == 0 ? (y_0 + ya) / 2 : ((int)((y_0 + ya) / 2)) + 1;
	int x_1a = ((x_1 + xa) % 2) == 0 ? (x_1 + xa) / 2 : ((int)((x_1 + xa) / 2)) + 1;
	int y_1a = ((y_1 + ya) % 2) == 0 ? (y_1 + ya) / 2 : ((int)((y_1 + ya) / 2)) + 1;
	
	int x_n = ((x_0a + x_1a) % 2) == 0 ? (x_0a + x_1a) / 2 : ((int)((x_0a + x_1a) / 2)) + 1;
	int y_n = ((y_0a + y_1a) % 2) == 0 ? (y_0a + y_1a) / 2 : ((int)((y_0a + y_1a) / 2)) + 1;
	
	int index;
	
	if (x_con_main == -1 && y_con_main == -1) {
		x_con_main = x_n;
		y_con_main = y_n;
	}
	
	for (size_t i = 0; i < concavityPoints.size(); i++) {
		if (concavityPoints[i].x == x_1 && concavityPoints[i].y == y_1){
			index = i;
		}
	}
	
	Point p = {x_n, y_n};
	
	concavityPoints.insert(concavityPoints.begin() + index, p);
	
	if (getDistancePointToRect(x_0, y_0, x_1, y_1, xa, ya) < M_PI) {
		return;
	}
	
	fillConcavityPoints(x_0, y_0, x_n, y_n, x_0a, y_0a, concavityPoints);
	fillConcavityPoints(x_n, y_n, x_1, y_1, x_1a, y_1a, concavityPoints);
}

vector<Point> getAllPointsOfCurve(int x_0, int y_0, int x_1, int y_1, int xa, int ya) {
	//printf("\nCreando vector concavityPoints");
	vector<Point> concavityPoints;
	
	Point p0 = {x_0, y_0};
	Point p1 = {x_1, y_1};
	
	//printf("\nAgregamos los puntos extremos al vector");
	concavityPoints.push_back(p0);
	concavityPoints.push_back(p1);
	
	if (x_2 != -1 && y_2 != -1) {
		//printf("\nLlenamos el vector con puntos de concavidad");
		fillConcavityPoints(x_0, y_0, x_1, y_1, xa, ya, concavityPoints);
	}
	
	//printf("\nConcavity points: ");
	//for (const auto& point : concavityPoints) {
	//	printf("(%d, %d) ", point.x, point.y);
	//}
	
	return concavityPoints;
}


void drawCurve() {
	for (size_t i = 0; i < allPointsOfCurve.size() - 1; i++) {		
		int x_0 = allPointsOfCurve[i].x;
		int y_0 = allPointsOfCurve[i].y;
		int x_1 = allPointsOfCurve[i+1].x;
		int y_1 = allPointsOfCurve[i+1].y;
		drawLine(x_0, y_0, x_1, y_1);
	}
}

// funcion de inicializacion de OpenGL
void init() {
	glClearColor(0, 0, 0, 0);
}

// funcion de dibujado principal
void display() {
	// limpiamos la pantalla
	glClear(GL_COLOR_BUFFER_BIT);
	// si las coordenadas son validas
	if (x_0 != -1 && y_0 != -1 && x_1 != -1 && y_1 != -1) {
		allPointsOfCurve = getAllPointsOfCurve(x_0, y_0, x_1, y_1, x_2, y_2);
		// graficamos la curva
		drawCurve();
	}
	glEnd();
	// forzamos el dibujado de los comandos en el buffer
	glFlush();
}
