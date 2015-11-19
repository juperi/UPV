/*
	Seminarios SGI. Ejemplo 06
	Interaccion con teclado y raton.
*/

#include <Utilidades.h>		// Libreria completa

// Globales
static enum {ALAMBRICO,SOLIDO,DOBLE} modo;
static float girox = 0.0, giroy = 0.0;
static int xanterior, yanterior; // No hace falta inicializar (al pulsar boton raton le da valor)

void onMenu(int valor)
{
	// Funcion de atencion al menu
	if(valor == 0) modo = ALAMBRICO;
	else if (valor == 1) modo = SOLIDO;
	else modo = DOBLE;

	glutPostRedisplay();
}

void init()
{
	// Activo test de visibilidad
	glEnable(GL_DEPTH_TEST);

	// Crear un menu de popup
	glutCreateMenu(onMenu);
	glutAddMenuEntry("ALAMBRICO", 0);
	glutAddMenuEntry("SOLIDO", 1);
	glutAddMenuEntry("DOBLE", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);// Boton al que asocio el menu (boton der raton)

}

void display()
{
	// Funcion que atiende al evento de display (render)

	// Color de borrado (1er paso para pintar)
	glClearColor(1.0,1.0,1.0,1.0); //RGBA, 30% azul
	// Borra la pantalla y buffer de profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Inicializa matriz compuesta de la MV
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situo y oriento la camara
	gluLookAt(1,3,5, 0,0,0, 0,1,0);// miramos al origen de coordenadas con la vertical eje y

	ejes(); // ver ejes

	glRotatef(girox, 1, 0, 0);
	glRotatef(giroy, 0, 1, 0);

	if(modo == ALAMBRICO){
		glColor3f(1,1,0);
		glutWireTeapot(0.51);
	}
	else if (modo == SOLIDO){
		glColor3f(1,0,0);
		glutSolidTeapot(0.5);
	}
	else{
		glColor3f(1,0,0);
		glutSolidTeapot(0.5);
		glColor3f(1,1,0);
		glutWireTeapot(0.51);
	}

	// Intercambio los buffers
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

	gluPerspective(20, razon, 1, 10); // 20 grados en vertical, razon aspecto == razon marco, distancias, empieza en 1 y termina en 10

}

void onKey(unsigned char tecla, int x, int y)
{
	// Funcion de atencion al evento de teclado
	switch(tecla){
	case 'a': modo = ALAMBRICO;
		break;
	case 's': modo = SOLIDO;
		break;
	case 'b': modo = DOBLE; // Alambrico y solido
		break;
	case 27: exit(0); // Boton Esc
	}
	glutPostRedisplay(); // Necesario en callback
}

void onClick(int boton, int estado, int x, int y) // Area de dibujo origen abajo
{
	// Funcion de atencion al pulsado del boton del raton

	// Almacenar el pixel donde se hizo click
	if(boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN){
		xanterior = x;
		yanterior = y;
	}
}

void onDrag(int x, int y)
{
	// Funcion de atencion al arrastre del raton
	// x, y coordenadas del cursor segun sist referencia de la ventana de dibujo

	static const float pixel2grados = 1.0; // Relaciona pixeles que me muevo con los grados (1 pixe 1 grado)

	// Al mover el raton a la derecha aumenta la x y el giro es alrededor 
	// del eje y positivo
	giroy += (x-xanterior) * pixel2grados;

	// Al mover el raton hacia abajo la y aumenta y el giro es alrededor
	// del eje x positivo (sentido contrario agujas reloj desde x)
	girox += (y-yanterior) * pixel2grados;

	// Actualizar
	xanterior = x;
	yanterior = y;

	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	// Inicializar la GL a traves de la root
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH); 

	// Inicializar la ventana
	glutInitWindowSize(600,600);
	glutCreateWindow("Interacción");

	// Inicializacion propia
	init();

	std::cout << "Running Interaction..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey); // Captura teclado
	glutMouseFunc(onClick); // Pulsar boton raton
	glutMotionFunc(onDrag); // Arrastre raton

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();

}