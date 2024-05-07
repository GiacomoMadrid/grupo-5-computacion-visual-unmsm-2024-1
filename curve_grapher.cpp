#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

using namespace std;

// variables globales
int x0 = -1, y0 = -1, x1 = -1, y1 = -1, x2 = -1, y2 = -1;

struct Point {
	int x;
	int y;
};

vector<Point> allPointsOfCurve;

// prototipos de funciones
void reshape(int width, int height);
void obtainData();
void drawPixel(int x, int y);
vector<Point> getMissingPoints(int x0, int y0, int x1, int y1, vector<Point> allPointsOfCurve);
void fillConcavityPoints(int x0, int y0, int x1, int y1, int xa, int ya, vector<Point> &concavityPoints);
bool compareX(const Point &a, const Point &b);
vector<Point> getAllPointsOfCurve(int x0, int y0, int x1, int y1, int xa, int ya);
void drawCurve();
void display();
void init();

// funcion principal
int main(int argc, char** argv) {
	// pedimos datos
	obtainData();
	
	allPointsOfCurve = getAllPointsOfCurve(x0, y0, x1, y1, x2, y2);
	
	// inicializamos el GLUT y configuramos la ventana
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(250, 50);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Hello OpenGL");
	
	// registramos las funciones callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
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

// funcion para pedir datos
void obtainData() {
	printf("--- Ingrese los datos ---\n");
	printf("x0: ");
	scanf("%d", &x0);
	printf("y0: ");
	scanf("%d", &y0);
	printf("x1: ");
	scanf("%d", &x1);
	printf("y1: ");
	scanf("%d", &y1);
	printf("x2: ");
	scanf("%d", &x2);
	printf("y2: ");
	scanf("%d", &y2);
}

// funion para dibujar un píxel
void drawPixel(int x, int y) {
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex3f(x, y, 0);
}

// funcion para obtener un vector de todos los puntos de una linea entre dos puntos
vector<Point> getMissingPoints(int x0, int y0, int x1, int y1, vector<Point> allPointsOfCurve) {
	// algoritmo de Bresenham
	int dx = abs(x0 - x1);
	int dy = abs(y0 - y1);
	int e = dx - dy;
	int ix = x1 > x0 ? 1 : -1;
	int iy = y1 > y0 ? 1 : -1;
	int x = x0;
	int y = y0;
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
	return missingPoints;
}

void fillConcavityPoints(int x0, int y0, int x1, int y1, int xa, int ya, vector<Point> &concavityPoints) {
	int x_0a = ((x0 + xa) % 2) == 0 ? (x0 + xa) / 2 : ((int)((x0 + xa) / 2)) + 1;
	int y_0a = ((y0 + ya) % 2) == 0 ? (y0 + ya) / 2 : ((int)((y0 + ya) / 2)) + 1;
	int x_1a = ((x1 + xa) % 2) == 0 ? (x1 + xa) / 2 : ((int)((x1 + xa) / 2)) + 1;
	int y_1a = ((y1 + ya) % 2) == 0 ? (y1 + ya) / 2 : ((int)((y1 + ya) / 2)) + 1;
	int x_n = ((x_0a + x_1a) % 2) == 0 ? (x_0a + x_1a) / 2 : ((int)((x_0a + x_1a) / 2)) + 1;
	int y_n = ((y_0a + y_1a) % 2) == 0 ? (y_0a + y_1a) / 2 : ((int)((y_0a + y_1a) / 2)) + 1;
	if (((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0)) <= 3*3) {
		return;
	}
	Point p = {x_n, y_n};
	concavityPoints.push_back(p);
	fillConcavityPoints(x0, y0, x_n, y_n, x_0a, y_0a, concavityPoints);
	fillConcavityPoints(x_n, y_n, x1, y1, x_1a, y_1a, concavityPoints);
}

// comparador para ordenar por coordenada x
bool compareX(const Point &a, const Point &b) {
	return a.x < b.x;
}

vector<Point> getAllPointsOfCurve(int x0, int y0, int x1, int y1, int xa, int ya) {
	vector<Point> concavityPoints;
	fillConcavityPoints(x0, y0, x1, y1, xa, ya, concavityPoints);
	Point p0 = {x0, y0};
	Point p1 = {x1, y1};
	for (const auto& point : concavityPoints) {
		printf("(%d, %d) ", point.x, point.y);
	}
	concavityPoints.push_back(p0);
	concavityPoints.push_back(p1);
	// ordenar el vector por coordenada x
	sort(concavityPoints.begin(), concavityPoints.end(), compareX);
	vector<Point> allPointsOfCurve;
	for (size_t i = 0; i < concavityPoints.size() - 1; i++) {
		int x0 = concavityPoints[i].x;
		int y0 = concavityPoints[i].y;
		int x1 = concavityPoints[i+1].x;
		int y1 = concavityPoints[i+1].y;
		vector<Point> missingPoints = getMissingPoints(x0, y0, x1, y1, allPointsOfCurve);
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
	// si las coordenadas son validas
	if (x0 != -1 && y0 != -1 && x1 != -1 && y1 != -1 && x2 != -1 && y2 != -1) {
		// graficamos la curva
		drawCurve();
	}
	glEnd();
	// forzamos el dibujado de los comandos en el buffer
	glFlush();
}
