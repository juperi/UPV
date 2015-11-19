/*
	Seminarios SGI. Ejemplo 04
	Dibujo de una tetera con marco, cámara Ortogonal, Perspectiva
*/

#include <Utilidades.h>		// Libreria completa

void init()
{
	// Activo test de visibilidad
	glEnable(GL_DEPTH_TEST);
}

void display()
{
	// Funcion que atiende al evento de display (render)

	// Color de borrado (1er paso para pintar)
	glClearColor(1.0,1.0,1.0,1.0); //RGBA, 30% azul
	// Borra la pantalla y buffer de profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Variar el marco
	//glViewport(300,300,300,300); // Empieza en el 300,300 y mide 300 tanto de ancho/alto
	// al reescalar la ventanta, el dibujo se quedará en el mismo sitio--->se tiene que poner en reshape

	// Inicializa matriz compuesta de la MV
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Situo y oriento la camara (se tiene que devolver al origen glTranslatef(0,0,0);)
	//gluLookAt(0,0,5, 0,0,0, 0,1,0); // Camara a 5 unidades del origen desde el punto z, eje vertical que voy a utilizar sera el y (se ve igual)
	//gluLookAt(1,4,5, 0,0,0, 0,1,0); // subimos la cámara
	gluLookAt(0,5,0, 0,0,0, -1,0,0);// miramos desde arriba (en eje y) idem rotates
	//gluLookAt(0,0,5, 0,0,0, 0,1,0);// miramos al origen con la vertical eje y en verde x en rojo

	ejes(); // ver ejes

	glTranslatef(0,0,0); // si cambiamos a -2 se acerca el objeto (1a forma de hacerlo grande)
	//glRotatef(-90,0,1,0);
	//glRotatef(90,0,0,-1);
	glColor3f(1,0,0);
	glutSolidTeapot(0.5);
	glColor3f(1,1,0);
	glutWireTeapot(0.51);

	// segunda tetera mas atras
	glTranslatef(0.2,0,-1); // como no hemos activado visibilidad se dibujaran en orden (roja mas despues) por eso no se tapan
	glColor3f(0,0,1);
	glutSolidTeapot(0.5);
	glColor3f(1,1,0);
	glutWireTeapot(0.51);

	// Forzar el envio de todas las ordenes
	glFlush();
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

	/*
	if(w<h) // Ajustamos la ventana en x, la y vendra ampliada
		glOrtho(-2,2, -2/razon,2/razon, -2,2);// Volumen de la vista (De -2 a 2 en cada eje)
	else // Ajustamos en y, la x vendra ampliada
		glOrtho(-2*razon,2*razon, -2,2, -2,2);
	*/

	gluPerspective(20, razon, 1, 10); // angulo (a mas pequeño mas grande el objeto), razon aspecto == razon marco, distancias

}

void main(int argc, char** argv)
{
	// Inicializar la GL a traves de la root
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); 

	// Inicializar la ventana
	glutInitWindowSize(600,600);
	glutCreateWindow("Cámara");

	// Inicializacion propia
	init();

	std::cout << "Running Tea Pot..." << std::endl; // Mensaje por consola

	// Registro las callbacks
	glutDisplayFunc(display); // Funcion atendida por la de display de arriba
	glutReshapeFunc(reshape);

	// Pongo en marcha el bucle de atencion a evento
	glutMainLoop();

	// Per a la practica:
	// Repetir estrella a lo largo del eje y y de colores diferentes
	// cerrada en esfera en alambrico de radio 1
}