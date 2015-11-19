/*
	Practica 3 SGI
	Construir mosaico de una figura
*/

#include <Utilidades.h>		// Libreria completa

static GLuint triangulo1, triangulo2, estrellaG, estrellaP;

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
}

void estrellaGyestrellaF()
{
	glPushMatrix(); // Salva la modelview

	// Dibujo Estrella grande
	glPushMatrix();
	glScalef(0.5,0.5,0.5); // Quiero diametro 1, multiplico x 0.5
	glCallList(triangulo1);
	glCallList(triangulo2);
	glPopMatrix();

	// Dibujo Estrella pequeña
	glPushMatrix();
	glScalef(0.2,0.2,0.2);
	glRotatef(30,0,0,1); // Rotacion de 30 grados
	glCallList(triangulo1);
	glCallList(triangulo2);
	glPopMatrix();

	glPopMatrix(); // Restaura la modelview
}

void display()
{
	// Funcion que atiende al evento de display (render)

	// Color de borrado Blanco
	glClearColor(1.0,1.0,1.0,1.0); 
	
	// Borra la pantalla
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0,0.0,0.8); // Azul
	glMatrixMode(GL_MODELVIEW); // Selecciona la modelview
	glLoadIdentity(); // Carga la matriz identidad

	// Dibujo del conjunto de la derecha (superior)
	glPushMatrix();
	glRotatef(-15,0,0,1); // Giro 15 sobre eje z
	glTranslatef(0.35,0.6,0);
	estrellaGyestrellaF();
	glPopMatrix();
	
	
	// Dibujo del conjunto de la derecha (inferior)
	glPushMatrix();
	glRotatef(15,0,0,1); // Giro 15
	glTranslatef(0.35,-0.6,0);
	estrellaGyestrellaF();
	glPopMatrix();
	
	// Dibujo del conjunto de la izquierda (superior)
	glPushMatrix();
	glRotatef(15,0,0,1); // Giro 15
	glTranslatef(-0.35,0.6,0);
	estrellaGyestrellaF();
	glPopMatrix();
	
	// Dibujo del conjunto de la izquierda (inferior)
	glPushMatrix();
	glRotatef(-15,0,0,1); // Giro 15
	glTranslatef(-0.35,-0.6,0);
	estrellaGyestrellaF();
	glPopMatrix();
	
	glFlush(); // Finaliza dibujo
}

void reshape(GLint w, GLint h) 
{
	// Funcion de atencional evento de reshape
}

void main(int argc, char** argv)
{
	// Inicializar la GL a traves de la root
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	// Inicializar la ventana
	glutInitWindowSize(400,400);
	glutCreateWindow("Mosaico");

	// Inicializacion propia
	init();

	std::cout << "Running mosaic..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();
}