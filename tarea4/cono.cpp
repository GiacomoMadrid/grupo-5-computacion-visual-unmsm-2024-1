#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

float dameX(float R, int N, int n);
float dameZ(float R, int N, int n);
void anillo(float R, float y, int N);
float fRadioCono();
void conoAlambre();
void formaAlambre(float H, int N, float(*f)(float y));

float nAnillos=100;
float alturaFigura=100;
float contadorRadio=50;//radioMax

//#define M_PI 3.141516
	
/*--------------------------------------------------------------------------
Devuelve el valor de x del punto que gira.
R: el radio de giro.
N: el número total de tramos en el anillo.
n: el tramo actual.
--------------------------------------------------------------------------*/
float dameX(float R, int N, int n) 
{
	float x = (float) R * cos(n * (2 * M_PI) / N);
	return x;
}

/*--------------------------------------------------------------------------
Devuelve el valor de z del punto que gira.
R: el radio de giro.
N: el número total de tramos en el anillo.
n: el tramo actual.
--------------------------------------------------------------------------*/
float dameZ(float R, int N, int n) 
{
	float z = (float) R * sin(n * (2 * M_PI) / N);
	return z;
}

/*--------------------------------------------------------------------------
Dibuja un anillo.
R: el radio del anillo.
y: la altura a la que se dibuja el anillo.
N: el número de tramos del anillo.
--------------------------------------------------------------------------*/
void anillo(float R, float y, int N) 
{
	int i;
	float x, z;
	
	glBegin(GL_LINE_LOOP);
	for(i = 0; i < N; i++) 
	{
		x = dameX(R, N, i);
		z = dameZ(R, N, i);
		glVertex3f(x, y, z);
	}
	glEnd();
}


/*--------------------------------------------------------------------------
Devuelve un valor de radio seg�n las proporciones de un cono.
--------------------------------------------------------------------------*/
float fRadioCono() 
{
	int radioAnillo=contadorRadio;
	contadorRadio=contadorRadio-0.5;
	return(radioAnillo);
}

/*--------------------------------------------------------------------------
Algoritmo de dibujo para una figura de revolución.
H: la altura de la figura.
N: el número de anillos y divisiones en cada anillo.
f(): puntero a la función que nos devuelve el valor de radio 
--------------------------------------------------------------------------*/
void formaAlambre(float H, int N, float(*f)()) 
{
	int i;
	float y, r;	
	for(i = 0; i < N; i++) 
	{ 
		y = i * H / N - (H / 2);	// Para cada nivel de altura
		r = f();           // obtenemos el radio
		anillo(r, y, N);    // y dibujamos un anillo
	}
}

/*--------------------------------------------------------------------------
Dibuja un cono.
--------------------------------------------------------------------------*/
void conoAlambre() 
{
	formaAlambre(alturaFigura, nAnillos, fRadioCono);
}


/*--------------------------------------------------------------------------
Borra la ventana y establece el color de dibujo y el ancho de las lineas
--------------------------------------------------------------------------*/
void display(void) 
{
	glClearColor (1.0,1.0,1.0, 1.0);
	glClear (GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glLoadIdentity ();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -4.0f);
	glRotatef(15.0f, 1.0f, 0.0f, 0.0f);	
	conoAlambre();
	
	glPopMatrix();
	glFlush ();
}


/*--------------------------------------------------------------------------
Especifica las caracteristicas de la vista.
--------------------------------------------------------------------------*/
void  inicializaVista(void) 
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/*--------------------------------------------------------------------------
Esta función se llama al cambiar el tamaño de la ventana.
width, height: ancho y alto de la zona de dibujo.
--------------------------------------------------------------------------*/
void reshape(int width, int height) 
{
	
	glViewport (0, 0, width, height);
	inicializaVista ();
}


/*--------------------------------------------------------------------------
Crea la ventana para dibujar con OpenGL.
argc: número de argumentos pasados al iniciar el programa.
**argv: array con cada uno de los argumentos.
--------------------------------------------------------------------------*/
void  inicializaVentana(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (700, 700);
	glutInitWindowPosition (200, 30);
	glutCreateWindow (argv[0]);
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
}

/*--------------------------------------------------------------------------
Crea la venta, inica OpenGL y crea las listas de visualización
argc: número de argumentos pasados al iniciar el programa.
**argv: array con cada uno de los argumentos.
--------------------------------------------------------------------------*/
void iniciaOpenGL(int argc, char **argv) 
{
	inicializaVentana (argc, argv);
	inicializaVista ();
	glutMainLoop();
}


/*--------------------------------------------------------------------------
Función main.
argc: número de argumentos pasados al iniciar el programa.
**argv: array con cada uno de los argumentos.
--------------------------------------------------------------------------*/
int main (int argc, char** argv) 
{
	iniciaOpenGL(argc, argv);
	return (1);
}
