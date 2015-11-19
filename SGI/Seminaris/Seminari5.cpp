/*
	Seminarios SGI. Ejemplo 05
	Animacion de un objeto
*/

//#include <GL/freeglut.h> // Con glut.h los timers no funcionan bien
#include <sstream> // Para streams
#include <Utilidades.h>		// Libreria completa
static float alfa = 0; // Variable a animar (angulo)

#define tasaFPS 60

void init()
{
	// Activo test de visibilidad
	glEnable(GL_DEPTH_TEST);
}

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
		titulo << "FPS= " << fotogramas;
		glutSetWindowTitle(titulo.str().c_str());
		// Reinicio el reloj y la cuenta de fotogramas
		antes = ahora;
		fotogramas = 0;
	}

}

void display()
{
	// Funcion que atiende al evento de display (render)

	// Color de borrado
	glClearColor(1.0,1.0,1.0,1.0);
	// Borra la pantalla y buffer de profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Inicializa matriz compuesta de la MV
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situo y oriento la camara
	gluLookAt(0,2,5, 0,0,0, 0,1,0);

	ejes(); // ver ejes

	glPushMatrix();
	glRotatef(alfa, 0,1,0); // Rota alrededor de eje y
	glColor3f(1,0,0);
	glutSolidTeapot(0.5);
	glColor3f(1,1,0);
	glutWireTeapot(0.51);
	glPopMatrix();

	// Intercambiar el buffer back por el front (evitamos flickering)
	glutSwapBuffers();

	FPS(); // Muestra frames
}

void reshape(GLint w, GLint h) // Anchura y altura
{
	// Ajusto el marco a la totalidad del area de dibujo
	glViewport(0,0,w,h);

	//Activar la transformacion PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // Meto en la pila projection la identidad

	// Ajuste de la razon de aspecto de la foto a la razon del marco (para evitar deformacion)
	float razon = (float) w/h;

	gluPerspective(20, razon, 1, 10); // angulo (a mas pequeño mas grande el objeto), razon aspecto == razon marco, distancias

}

void update()
{
	// Funcion de atencion al evento "estoy ocioso"
	
	/* Sin control de tiempo
	alfa += 0.1;
	*/

	/* Con control de tiempo */
	// Modificar el angulo a velocidad angular cte. p.e. omega = 60º/sg
	static const float omega = 100;
	// Pido la hora por primera vez
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);

	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	//Tiempo transcurrido en msg
	float tiempo_transcurrido = hora_actual-hora_anterior;

	alfa += omega * tiempo_transcurrido/1000.0;

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
	// Inicia 2 buffers de color (evitamos flickering) y 1 profundidad
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

	// Inicializar la ventana
	glutInitWindowSize(600,600);
	glutCreateWindow("Animacion");

	// Inicializacion propia
	init();

	std::cout << "Running Tea Pot..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);
	//glutIdleFunc(idle); lo usaremos en onTimer (pasa a ser update)
	glutTimerFunc(1000/tasaFPS, onTimer, 1000/tasaFPS);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();

}