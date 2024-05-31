#include <GL/glut.h> 
#include <stdio.h> 
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int x_1, y_1;
float a, b, c;
float m;
int ancho = 800;
int alto = 800;

struct Punto {
	int x;
	int y;
};

vector<Punto> puntos;

void reshape(int width, int height){ 
	glViewport(0, 0, width, height); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 	
	glOrtho(0, width, 0, height, -0.5, 0.5);
	glMatrixMode(GL_MODELVIEW); 
} 
	
void write_pixel(int x, int y){
	glPointSize(1);
	glVertex3f(x+glutGet(GLUT_WINDOW_WIDTH)/2,y+glutGet(GLUT_WINDOW_HEIGHT)/2,0); 
}
	
int punto_medio(int b, int a1, int a2){
	int da = (a2-a1)/2;
	if( da > 1){
		write_pixel(b, da);
		return punto_medio(b, da, a1);
	}
	write_pixel(b, da);
	return(da);
}
		
void linea_punto_pendiente(int x_1, int y_1, float p){
	int x, y, yp;
	y = y_1;	
	
	for (x = -1*ancho; x <= ancho; x++){		
		yp = y;
		y = (y_1 + p * (x - x_1));	
		write_pixel(x, y);	
		Punto p = {x, y};
		puntos.push_back(p);
	}
	
}
	
void linea_punto_medio(int x0, int y0, int x1, int y1) {
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
		write_pixel(x, y);
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
	
			
void parabola(float a, float b, float c){
	int x, y;
	
	for(x = -1*ancho; x <= ancho; x++){
		y = (a*x*x + b*x + c);
		write_pixel(x, y);		
		Punto p = {x, y};
		puntos.push_back(p);
	}	
}
	
				
void display_linea(){ 		
	glBegin(GL_POINTS);	
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f(1,1,1); 
	glLoadIdentity(); 	
	
	linea_punto_medio(-1*ancho, 0, ancho, 0);	
	linea_punto_medio(0, -1*alto, 0, alto);
	
	glColor3f(0.9,0.8,0.2); 
	linea_punto_pendiente(x_1, y_1, m);	
	
	for(size_t i = 0; i<puntos.size()-1; i++){
		int x_0 = puntos[i].x;
		int y_0 = puntos[i].y;
		int x_1 = puntos[i+1].x;
		int y_1 = puntos[i+1].y;
		
		linea_punto_medio(x_0, y_0, x_1, y_1);
	}
	
	glEnd();
	glFlush(); 
} 
					
					
void display_parabola(){ 	
	glBegin(GL_POINTS);	
	glClear(GL_COLOR_BUFFER_BIT); 
	glColor3f(1,1,1); 
	glLoadIdentity(); 		
	linea_punto_medio(-1*ancho, 0, ancho, 0);	
	linea_punto_medio(0, -1*alto, 0, alto);
	
	glColor3f(0.9,0.8,0.2); 	
	parabola(a, b, c);		
	
	for(size_t i = 0; i<puntos.size()-1; i++){
		int x_0 = puntos[i].x;
		int y_0 = puntos[i].y;
		int x_1 = puntos[i+1].x;
		int y_1 = puntos[i+1].y;
		
		linea_punto_medio(x_0, y_0, x_1, y_1);
	}
		
	glEnd();
	glFlush(); 
} 
		
	
void init(){ 
	glClearColor(0,0,0,0); 
} 
							
							
int main(int argc, char **argv){ 
	int opc;
	
	do{
		printf("Seleccione la figura a graficar\n");
		printf("1) Linea recta\n");
		printf("2) Parabola\n");
		std::cin>>opc;
		
		switch(opc){
		case 1:{
			printf("Ingresar x_1, y_1, m; -> (coordenadas del punto (x_1,y_1) y la pendiente)\n");
			scanf("%d %d %f", &x_1, &y_1, &m);
			
			glutInit(&argc, argv); 
			glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
			glutInitWindowPosition(ancho, alto); 
			glutInitWindowSize(500, 500); 
			glutCreateWindow("Linea Recta");					
			init(); 
			glutDisplayFunc(display_linea); 
			
			break;
		}
			
		case 2:{
				printf("Ingresar a, b, c; para los valores de: ax^2+bx+c\n");
				scanf("%f %f %f", &a, &b, &c);
				
				glutInit(&argc, argv); 
				glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
				glutInitWindowPosition(ancho, alto); 
				glutInitWindowSize(500, 500); 
				glutCreateWindow("Parabola");					
				init(); 
				glutDisplayFunc(display_parabola); 
				
				
				break;
			}
				
		default:{
				printf("Por favor, elija una opcion valida.\n");
				break;
			}
				
		}		
		
	}while(opc < 1 || opc >2);
	
	glutReshapeFunc(reshape); 
	glutMainLoop(); 
	return 0; 
} 
									
