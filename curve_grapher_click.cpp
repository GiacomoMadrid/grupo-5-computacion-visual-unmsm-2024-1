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

// FunciÃ³n de callback para manejar eventos de clic del mouse
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
			
			x_2 = x_0;
			y_2 = y_0;
			
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

float getDistancePointToRect(int x_0, int y_0, int x_1, int y_1, int x_2, int y_2) {
	// y - y_0 = m (x - x_0)
	// 0 = m*x - y + (y_0 - m*x_0) 
	
	// 0 = A*x + B*y + C
	
	// A = m
	// B = -1
	// C = y_0 - m*x_0
	// m =  (y_1 - y_0) / (x_1 - x_0)
	float maxX = x_1 > x_0 ? x_1 : x_0;
	float minX = x_1 == maxX ? x_0 : x_1;
	bool divZero = x_1 != x_0 ? true : false; 
	float m  =  divZero ? (y_1 - y_0) / (x_1 - x_0) : 1;
	float a = m;
	float b = -1;
	float c = y_0 - m*x_0;
	
	// distance = |a*x_2 + b*y_2 + c| / sqrt(a*a + b*b)
	double aux = a*x_2 + b*y_2 + c >= 0 ? a*x_2 + b*y_2 + c : -1*(a*x_2 + b*y_2 + c);
	double distance =  divZero ? aux / pow(a*a + b*b, 0.5) : maxX - minX;
	
	return (float)distance;
}

void fillConcavityPoints(int x_0, int y_0, int x_1, int y_1, int xa, int ya, vector<Point> &concavityPoints) {
	int x_0a = ((x_0 + xa) % 2) == 0 ? (x_0 + xa) / 2 : ((int)((x_0 + xa) / 2)) + 1;
	int y_0a = ((y_0 + ya) % 2) == 0 ? (y_0 + ya) / 2 : ((int)((y_0 + ya) / 2)) + 1;
	int x_1a = ((x_1 + xa) % 2) == 0 ? (x_1 + xa) / 2 : ((int)((x_1 + xa) / 2)) + 1;
	int y_1a = ((y_1 + ya) % 2) == 0 ? (y_1 + ya) / 2 : ((int)((y_1 + ya) / 2)) + 1;
	
	int x_n = ((x_0a + x_1a) % 2) == 0 ? (x_0a + x_1a) / 2 : ((int)((x_0a + x_1a) / 2)) + 1;
	int y_n = ((y_0a + y_1a) % 2) == 0 ? (y_0a + y_1a) / 2 : ((int)((y_0a + y_1a) / 2)) + 1;
	
	if (x_con_main == -1 && y_con_main == -1) {
		x_con_main = x_n;
		y_con_main = y_n;
	}
	
	int index;
	
	for (size_t i = 0; i < concavityPoints.size(); i++) {
		if (concavityPoints[i].x == x_0 && concavityPoints[i].y == y_0){
			index = i;
		}
	}
	
	Point p = {x_n, y_n};
	
	concavityPoints.insert(concavityPoints.begin() + index, p);
	
	if (getDistancePointToRect(x_0, y_0, x_1, y_1, x_n, y_n) < 1) {
		return;
	}
	//Point p = {x_n, y_n};
	//concavityPoints.push_back(p);
	fillConcavityPoints(x_0, y_0, x_n, y_n, x_0a, y_0a, concavityPoints);
	fillConcavityPoints(x_n, y_n, x_1, y_1, x_1a, y_1a, concavityPoints);
}

// comparador para ordenar por coordenada x
bool compareX(const Point &a, const Point &b) {
	return a.x < b.x;
}

// comparador para ordenar por coordenada y
bool compareY(const Point &a, const Point &b) {
	return a.y < b.y;
}

vector<Point> getAllPointsOfCurve(int x_0, int y_0, int x_1, int y_1, int xa, int ya) {
	vector<Point> concavityPoints;
	
	Point p0 = {x_0, y_0};
	Point p1 = {x_1, y_1};
	
	concavityPoints.push_back(p0);
	concavityPoints.push_back(p1);
	
	fillConcavityPoints(x_0, y_0, x_1, y_1, xa, ya, concavityPoints);
	
	printf("\nConcavity points: ");
	for (const auto& point : concavityPoints) {
		printf("(%d, %d) ", point.x, point.y);
	}
	concavityPoints.push_back(p0);
	concavityPoints.push_back(p1);
	
	// ordenar el vector por coordenada x
	//sort(concavityPoints.begin(), concavityPoints.end(), compareY);
	
	vector<Point> allPointsOfCurve;
	
	for (size_t i = 0; i < concavityPoints.size() - 1; i++) {
		int x_0 = concavityPoints[i].x;
		int y_0 = concavityPoints[i].y;
		int x_1 = concavityPoints[i+1].x;
		int y_1 = concavityPoints[i+1].y;
		vector<Point> missingPoints = getMissingPoints(x_0, y_0, x_1, y_1, allPointsOfCurve);
		allPointsOfCurve.insert(allPointsOfCurve.end(), missingPoints.begin(), missingPoints.end());
	}
	return allPointsOfCurve;
}


void drawCurve() {
	
	for (size_t i = 0; i < allPointsOfCurve.size(); i++) {
		int x = allPointsOfCurve[i].x;
		int y = allPointsOfCurve[i].y;
		drawPixel(x, y);
	}
}

// funcion de inicializacion de OpenGL
void init() {
	glClearColor(0, 0, 0, 0); // establecemos el color de fondo a negro
}

// funcion de dibujado principal
void display() {
	// limpiamos la pantalla
	glClear(GL_COLOR_BUFFER_BIT);
	// si las coordenadas son validas
	if (x_0 != -1 && y_0 != -1 && x_1 != -1 && y_1 != -1 && x_2 != -1 && y_2 != -1) {
		drawPixel(x_con_main, y_con_main);
		printf("\ndistance: %f", getDistancePointToRect(x_0, y_0, x_1, y_1, x_con_main, y_con_main));
		allPointsOfCurve = getAllPointsOfCurve(x_0, y_0, x_1, y_1, x_2, y_2);
		// graficamos la curva
		drawCurve();
	}
	glEnd();
	// forzamos el dibujado de los comandos en el buffer
	glFlush();
}
