/*
	Practica 5 SGI
	Reloj 3D Animado
*/

#include <Utilidades.h>		// Libreria completa
#include <time.h>			// Libreria tiempo
#include <math.h>

#define tasaFPS 60 //fps a los que se movera la estrella central

struct tm *newtime;
time_t ltime;

static float rx,ry,rz,alfa;

static GLuint triangulo1, triangulo2, puntos;

void init()
{
	/*	Triangulo 1	*/
	triangulo1 = glGenLists(1); // Obtiene id list
	glNewList(triangulo1,GL_COMPILE); // Abre list

	glBegin(GL_TRIANGLE_STRIP);
		float r1 = 1.0;
		float r2 = 0.7;
		for (int i = 0; i<5; i++){
			glVertex3f(r1*sin(i*2*PI/3),r1*cos(i*2*PI/3),0.0); // vertices triangulo grande
			glVertex3f(r2*sin(i*2*PI/3),r2*cos(i*2*PI/3),0.0); // vertices tirangulo pequeño
		}
	glEnd(); // Cierra glBegin
	glEndList();
	
	/*	Triangulo 2	*/
	triangulo2 = glGenLists(1); // Obtiene id list
	glNewList(triangulo2,GL_COMPILE); // Abre list

	glBegin(GL_TRIANGLE_STRIP);
		float r3 = -1.0;
		float r4 = -0.7;
		for (int i = 0; i<5; i++){
			glVertex3f(r3*sin(i*2*PI/3),r3*cos(i*2*PI/3),0.0); // vertices triangulo grande
			glVertex3f(r4*sin(i*2*PI/3),r4*cos(i*2*PI/3),0.0); // vertices tirangulo pequeño
		}
	glEnd(); // Cierra glBegin
	glEndList();

	/**	Puntos **/
	puntos = glGenLists(1); // Obtiene id list
	glNewList(puntos,GL_COMPILE); // Abre list

	glBegin(GL_POINTS);
		float r = 0.9;
		for (int i = 0; i<60; i++){
			glVertex3f(r*sin(i*2*PI/60),r*cos(i*2*PI/60),0.0);
		}
	glEnd(); // Cierra glBegin
	glEndList();

	/* Activo test de visibilidad */
	glEnable(GL_DEPTH_TEST);

	// Activo puntos
	glEnable(GL_POINT_SIZE);
	glPointSize(10);
}

void display()
{
	// Funcion que atiende al evento de display (render)
	
	// Obtenemos el tiempo actual
	time(&ltime);
	newtime = localtime(&ltime); // Conversion a tiempo local

	// Color de borrado Blanco
	glClearColor(1.0,1.0,1.0,1.0); 
	
	// Borra la pantalla y buffer de profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Inicializa matriz compuesta de la MV
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(0,4,5, 0,0,0, 0,1,0); // subimos la cámara
	gluLookAt(0,0,6.5, 0,0,0, 0,1,0); // Cámara normal

	/** Esfera exterior **/
	glLineWidth(1.0f);
	glColor3f(0.3,0.6,0.6);
	glutWireSphere(1.0,20,20);

	/** Puntos reloj **/;
	glPushMatrix();
	glColor3f(0.0,0.0,0.0);
	glCallList(puntos);
	glPopMatrix();

	/** Esfera horas reloj **/
	int rotar = 0; // Inicialmente 0º
	for (int i = 0; i<12; i++){

		glPushMatrix();
		glColor3f(0,0.8,0); // Color Verde
		glRotatef(rotar,0,0,1); // Rotacion sobre eje z
		glTranslatef(0,0.9,0);
		glutSolidSphere(0.05,10,10);
		glPopMatrix();
		rotar += 30;
	}

	/** Estrellas rotatorias interiores **/
	glPushMatrix();
	glScalef(0.25,0.25,0.25); // Diametro 1/10
	//glRotatef(alfa,0,1,0); // Rotacion sobre eje y

	int pos = 0; // Inicialmente 0º
	for (int i=0; i<4; i++){ // Dibuja 4 estrellas

		glPushMatrix();
		glRotatef(pos,0,1,0); // Rotacion sobre eje y
		glColor3ub(rand()%255,rand()%255,rand()%255); // Color aleatorio
		glCallList(triangulo1);
		glCallList(triangulo2);
		glPopMatrix();
		pos += 40;
	}
	glPopMatrix();
	
	/** Linea marca segundos **/

    glColor3f(0.5,0,1);
    glLineWidth(6.0);
    glPushMatrix();
    glRotatef((360/60) * newtime->tm_sec,0.0,0.0,-1.0);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(0,0.8);
    glEnd();
	glPopMatrix();

	/** Cono marca minutos **/

    glColor3f(0,0,0);
	glLineWidth(2.0);
    glPushMatrix();
	glRotatef(90,-1,0,0);
    glRotatef((360/60) * newtime->tm_min,0,1,0);
	glTranslatef(0,0,0.25); // Lo trasladamos hacia arriba (eje y)
    glutWireCone(0.1,0.5,10,10);
    glPopMatrix();

	/** Triangulo marca horas **/

	glPushMatrix();
	glColor3f(0.7,0,0); // Color Rojo
	glScalef(0.1,0.1,0.1); // Diametro 1/10
	glRotatef((360/12) * newtime->tm_hour, 0.0, 0.0, -1.0);
	glTranslatef(0,7,0); // Lo trasladamos hacia arriba (eje y)
	glCallList(triangulo1);
	glPopMatrix();

	// Intercambiar el buffer back por el front (evitamos flickering)
	glutSwapBuffers();
}

void reshape(GLint w, GLint h) 
{
	// Funcion de atencional evento de reshape

	// Razon de aspecto de la vista
	static const float razon = 1.0; // a/b = w'/h'
	// Razon de aspecto del area de dibujo
	float razonAD= float(w)/h;
	float wp,hp; // w',h'
	if(razonAD<razon){
		wp= float(w);
		hp= wp/razon;
		glViewport(0,int(h/2.0-hp/2.0),w,int(hp));
	}
	else{
		hp= float(h);
		wp= hp*razon;
		glViewport(int(w/2.0-wp/2.0),0,int(wp),h);
	}

	//Activar la transformacion PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* CAMARA PERSPECTIVA */
	gluPerspective(18, razon, 1, 10); // razon aspecto == razon marco, distancias
}

void onTimer(int tiempo)
{
	// Funcion de atencion
	rx = 30 * cos(alfa);
	ry = 30 * sin(alfa);
	rz = 30 * cos(alfa);

	alfa += 0.01;
	if (alfa > 2*PI) alfa = 0;

	glutPostRedisplay();
	glutTimerFunc(100, onTimer, 1);
}

void main(int argc, char** argv)
{
	// Inicializar la GL a traves de la root
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	// Inicializar la ventana
	glutInitWindowSize(600,600);
	glutCreateWindow("Reloj 3D");

	// Inicializacion propia
	init();

	std::cout << "Running 3D Clock..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);

	glutTimerFunc(10, onTimer, 1);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();
}