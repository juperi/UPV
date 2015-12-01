/*
	Práctica 06 SGI.
	Interacción.
*/

#include <sstream>
#include <Utilidades.h>		// Libreria completa

// Globales
static float velocidad = 0.0, alfa = 32.0;	// Velocidad / grado del giro
static float posX = 0, posZ = 0.0;		// posición de la cámara en X/Z
static GLuint circuito;

#define tasaFPS 60 // Para que mantenga 60 fps

void init()
{
	// Visibilidad
	glEnable(GL_DEPTH_TEST);

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

	// Camara inicialmente en 0,1,0 (a 1m sobre eje y)
	// X, Z varian, se mantiene a 1 sobre y
	gluLookAt(posX,1,posZ, posX+sin(rad(alfa)),1,posZ+cos(rad(alfa)), 0,1,0);

	ejes(); // ver ejes

	/** Circuito carreras **/
	glPushMatrix();
	glColor3f(0.0,0.0,0.0); // Negro
	GLfloat v0[3], v1[3], v2[3], v3[3]; // Vertices
	float z = floor(posZ)-5;
	float x = 5*sin(z*PI/25);

	v0[0] = x-2;
	v0[1] = 0.0;
	v0[2] = z;
			
	v1[0] = x+2;
	v1[1] = 0.0;
	v1[2] = z;

	
	glBegin(GL_QUAD_STRIP);
		for(int i = 0; i<50; i++){ // 50 quads			
			
			z+=1;
			x = 5*sin(z*PI/25);
			
			v2[0] = x+2;
			v2[1] = 0.0;
			v2[2] = z;
			
			v3[0] = x-2;
			v3[1] = 0.0;
			v3[2] = z;
			quad(v0,v1,v2,v3);
			
			// Actualizamos vertices
			v0[0]=v3[0];
            v0[1]=v3[1];
            v0[2]=v3[2];

            v1[0]=v2[0];
            v1[1]=v2[1];
            v1[2]=v2[2];
		}
	glEnd();
	glPopMatrix();
	
	//Intercambio los buffers
	glutSwapBuffers();
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

	gluPerspective(120, razon, 0.5, 50); // 120 grados fov, razon aspecto == razon marco, distancias, empieza en 0.5 y termina en 50

}

void onKey(int tecla, int x, int y)
{
	// Funcion de atencion al evento de teclado
	stringstream titulo;
	switch(tecla){
		case GLUT_KEY_LEFT: alfa += 2;
			break;
		case GLUT_KEY_RIGHT: alfa -= 2;
			break;
		case GLUT_KEY_UP:
			if(velocidad < 12){ 
				velocidad += 0.1;
				titulo << "Velocidad = " << velocidad << "m/s";
				glutSetWindowTitle(titulo.str().c_str());
			}
			break;
		case GLUT_KEY_DOWN:
			if(velocidad >= 0.01){ 
				velocidad -= 0.1;
				titulo << "Velocidad = " << velocidad << "m/s";
				glutSetWindowTitle(titulo.str().c_str());
			}
			else{
			titulo << "Velocidad = 0 m/s";
			glutSetWindowTitle(titulo.str().c_str());
			}
			break;
		case 27: exit(0); // Boton Esc
	}
	glutPostRedisplay(); // Necesario en callback
}

void update()
{
	// Funcion de atencion al evento "estoy ocioso"

	/* Control de tiempo */
	// Modificar el angulo a velocidad angular cte. p.e. omega = 60º/sg
	static const float omega = 100;
	// Pido la hora por primera vez
	static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);

	int hora_actual = glutGet(GLUT_ELAPSED_TIME);

	//Tiempo transcurrido en msg
	float tiempo_transcurrido = hora_actual-hora_anterior;

	float distancia = velocidad * tiempo_transcurrido/1000;
	posX += sin(rad(alfa))*distancia;
	posZ += cos(rad(alfa))*distancia;

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
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH); 

	// Inicializar la ventana
	glutInitWindowSize(600,400);
	glutCreateWindow("Interfaz de conducción");

	// Inicializacion propia
	init();

	std::cout << "Ejecutando interfaz de conducción..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);
	glutSpecialFunc(onKey); // Captura teclado

	glutTimerFunc(1000/tasaFPS, onTimer, 1000/tasaFPS);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();

}