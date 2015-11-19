/*
	Practica 4 SGI
	Estrella 3D
*/

#include <Utilidades.h>		// Libreria completa

static GLuint triangulo1, triangulo2;

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

	/* Activo test de visibilidad */
	glEnable(GL_DEPTH_TEST);
}

void display()
{
	// Funcion que atiende al evento de display (render)

	// Color de borrado Blanco
	glClearColor(1.0,1.0,1.0,1.0); 
	
	// Borra la pantalla y buffer de profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Inicializa matriz compuesta de la MV
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(1,4,5, 0,0,0, 0,1,0); // subimos la cámara

	glColor3f(0.3,0.6,0.6);
	glutWireSphere(1.0,20,20); // Dibuja esfera de radio 1

	float r,g,b = 0.0;
	int rotate = 0; // Inicialmente 0º
	for (int i=0; i<6; i++){ // Dibuja 6 estrellas

		glPushMatrix();
		glRotatef(rotate,0,1,0); // Rotacion sobre eje y
		glColor3f(r,g,b); // Color aleatorio
		glCallList(triangulo1);
		glCallList(triangulo2);
		glPopMatrix();
		
		// Actualizamos rotacion y color
		rotate += 30;
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
	}

	glFlush(); // Finaliza dibujo
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

void main(int argc, char** argv)
{
	// Inicializar la GL a traves de la root
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	// Inicializar la ventana
	glutInitWindowSize(600,600);
	glutCreateWindow("Estrella 3D");

	// Inicializacion propia
	init();

	std::cout << "Running 3D Star..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();
}