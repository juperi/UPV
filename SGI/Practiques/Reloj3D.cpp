/*
	Practica 5 SGI
	Reloj Anal�gico 3D Animado
*/
#include <sstream>
#include <Utilidades.h>		// Libreria completa
#include <time.h>			// Libreria tiempo

struct tm *newtime;
time_t ltime;

static float alfa,beta,gamma;
static bool crece = true;
static GLuint triangulo1, triangulo2, puntos;

#define tasaFPS 60 // Para que mantenga 60 fps

void FPS()
{
	// Muestra en la barra de titulo los FPS
	int ahora, tiempo_transcurrido;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	static int fotogramas = 0;

	// Cada vez que se llame a esta funcion se incrementa fotogramas
	fotogramas ++;

	// Tiempo transcurrido
	ahora = glutGet(GLUT_ELAPSED_TIME);
	tiempo_transcurrido = ahora - antes;

	// Si ha transcurrido mas de 1 sg, muestro los FPS y reinicio el reloj
	if (tiempo_transcurrido > 1000){
		// modifico el texto de la barra de titulo
		stringstream titulo;
		titulo << "FPS estrellas = " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		// Reinicio el reloj y la cuenta de fotogramas
		antes = ahora;
		fotogramas = 0;
	}

}

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
			glVertex3f(r2*sin(i*2*PI/3),r2*cos(i*2*PI/3),0.0); // vertices tirangulo peque�o
		}
	glEnd(); // Cierra glBegin
	glEndList();
	
	/*	Triangulo 2 (inverso) */
	triangulo2 = glGenLists(1);
	glNewList(triangulo2,GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);
		float r3 = -1.0;
		float r4 = -0.7;
		for (int i = 0; i<5; i++){
			glVertex3f(r3*sin(i*2*PI/3),r3*cos(i*2*PI/3),0.0);
			glVertex3f(r4*sin(i*2*PI/3),r4*cos(i*2*PI/3),0.0);
		}
	glEnd();
	glEndList();

	/**	Puntos **/
	puntos = glGenLists(1);
	glNewList(puntos,GL_COMPILE);

	glBegin(GL_POINTS);
		float r = 0.9;
		for (int i = 0; i<60; i++){
			glVertex3f(r*sin(i*2*PI/60),r*cos(i*2*PI/60),0.0);
		}
	glEnd();
	glEndList();

	// Activo tama�o puntos
	glEnable(GL_POINT_SIZE);
	glPointSize(10);

	/* Activo test de visibilidad */
	glEnable(GL_DEPTH_TEST);
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

	gluLookAt(0,3,6, 0,0,0, 0,1,0); // subimos la c�mara
	//gluLookAt(0,0,6.5, 0,0,0, 0,1,0); // C�mara normal

	/** Esfera exterior **/
	glLineWidth(1.0f);
	glColor3f(0.3,0.6,0.6);
	glutWireSphere(1.0,10,10);

	/** Puntos reloj **/;
	glPushMatrix();
	glColor3f(0.0,0.0,0.0);
	glCallList(puntos);
	glPopMatrix();

	/** Esfera horas reloj **/
	int rot = 0; // Inicialmente 0�
	for (int i = 0; i<12; i++){
		glPushMatrix();
		glColor3f(0,0.6,0.4); // Color Verde
		glRotatef(rot,0,0,1); // Rotacion sobre eje z
		glTranslatef(0,0.9,0);
		glutSolidSphere(0.05,10,10);
		glPopMatrix();
		rot += 30;
	}

	/** Estrellas rotatorias interiores **/
	glPushMatrix();
	glScalef(0.25,0.25,0.25); // Diametro 1/10
	glRotatef(beta,0,1,0); // Rotacion constante sobre eje y
	FPS(); // Muertra FPS a los que giran las estrellas

	int pos = 0; // Inicialmente 0�
	for (int i=0; i<4; i++){ // Dibuja 4 estrellas
		glPushMatrix();
		glRotatef(pos,0,1,0); // Rotacion sobre eje y
		glColor3f(0.2 + 0.2/4 *i, 0.9 - 0.5/4 *i, 0.9 - 0.9/4 *i); // Color degradado
		glCallList(triangulo1);
		glCallList(triangulo2);
		glPopMatrix();
		pos += 40;
	}
	glPopMatrix();
	
	/** Linea marca segundos **/
	glColor3f(0.4,0,1);
    glLineWidth(6.0);
    glPushMatrix();
    glRotatef((360/60) * newtime->tm_sec,0.0,0.0,-1.0);
    glBegin(GL_LINES);
    glVertex2f(0,0.1);
    glVertex2f(0,0.85);
    glEnd();
	glPopMatrix();

	/** Cono marca minutos **/
    glColor3f(0,0,0);
	glLineWidth(2.0);
    glPushMatrix();
	glRotatef(90,-1,0,0);
    glRotatef((360/60) * newtime->tm_min,0,1,0);
	glTranslatef(0,0,0.25); // Lo trasladamos hacia arriba (eje y)
    glutWireCone(0.075,0.5,10,10);
    glPopMatrix();

	/** Triangulo marca horas **/
	glPushMatrix();
	glColor3f(0.7,0,0); // Color Rojo
	glScalef(0.1,0.1,0.1); // Diametro 1/10
	glRotatef((360/12) * newtime->tm_hour, 0.0, 0.0, -1.0);
	glTranslatef(0,6,0); // Lo trasladamos hacia arriba (eje y)
	glScalef(gamma,gamma,gamma); // Rotacion sobre eje y
	glCallList(triangulo1);
	//FPS(); // Muertra FPS a los que cambia de tama�o
	glPopMatrix();

	// Intercambiar el buffer back por el front
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

void update()
{
	// Funcion de atencion al evento "estoy ocioso"
	alfa += 0.01; 
	if (alfa > 2*PI) alfa = 0; // Si completa la vuelta, empieza de nuevo

	/* Control de tiempo */
	// Modificar el angulo a velocidad angular constante p.e. omega = 60�/sg
	static const float omega = 100;
	// Pido la hora por primera vez
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);

	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	//Tiempo transcurrido en msg
	float tiempo_transcurrido = hora_actual-hora_anterior;

	// Angulo estrellas centrales
	beta += omega * tiempo_transcurrido/1000.0;

	// Angulo aguja marca horas
	if (crece == true){
		gamma += omega * tiempo_transcurrido/100000.0;
		if (gamma >= 2) crece = false;
	}else{
		if (gamma <= 0.75) crece = true;
		gamma -= omega * tiempo_transcurrido/100000.0;
	}

	// Actualizar hora_anterior a la hora_actual
	hora_anterior = hora_actual;

	glutPostRedisplay(); // Encola evento de display
}

void onTimer(int tiempo)
{
	// Funcion de atencion a la cuenta atras

	// Encolar un nuevo timer
	glutTimerFunc(tiempo, onTimer, tiempo);

	// Actualizar la escena
	update();	
}

void main(int argc, char** argv)
{
	// Inicializar la GL a traves de la root
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	// Inicializar la ventana
	glutInitWindowSize(600,600);
	glutCreateWindow("Reloj Anal�gico 3D");

	// Inicializacion propia
	init();

	std::cout << "Running Analogic 3D Clock..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);

	glutTimerFunc(1000/tasaFPS, onTimer, 1000/tasaFPS);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();
}