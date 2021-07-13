//	Program developed by
//	
//	Informatika Fakultatea
//	Euskal Herriko Unibertsitatea
//	http://www.ehu.eus/if
//
// to compile it: gcc dibujar-puntos.c -lGL -lGLU -lglut
//
// This example shows :the use of GL_POINTS
//


#include <GL/glut.h>
#include <stdio.h>
#include "triangulo.h"

extern void cargar_triangulos(int *hkopptr, hiruki **hptrptr);
// Indica en que triangulo estamos dibujando.
int indice = 0;

// puntero hacia los triangulos cargados
hiruki *triangulosptr;

//numero de triangulos hayados en el .TXT
int num_triangles;


// texturaren informazioa
extern void load_ppm(char *file, unsigned char **bufferptr, int *dimxptr, int * dimyptr);
unsigned char *bufferra;
int dimx,dimy;

//consigue los datos de la textura adaptados al tamaño de la ventana
unsigned char * color_textura(float u, float v) {
	int col = u *dimx;
	int fila = dimy -v*dimy;
	return(bufferra + (3*dimx*fila +3*col));

}


// Dada una altura y dos puntos, dibujar la linea que los une mediante interpolacion
void dibujar_segmento(punto *pc1PTR, punto *pc2PTR, float alt) {
	unsigned char* colorv;
	unsigned char r,g,b;

	// variables auxiliares para no modificar las originales
	float pU,pV;

	pU = pc1PTR->u;
	pV = pc1PTR->v;

	// Variacion respecto de X
	float varU = (pc2PTR->u - pc1PTR->u)/ ( pc2PTR->x - pc1PTR->x);
	float varV = (pc2PTR->v - pc1PTR->v)/ ( pc2PTR->x - pc1PTR->x);

	
	// bucle de interpolacion
	for( float i = pc1PTR->x; i < pc2PTR->x; ++i){

		if (pU + varU<=1 && pV + varV<= 1) {
			pU = pU + varU;
			pV = pV + varV;	
		} else {
			printf("%s\n", "Se han sobrepasado los limites de U o V ( >1 )");
		}
		
		//funcion de dibujado.
		colorv = color_textura(pU,pV);
		r= colorv[0];
		g =colorv[1];
		b = colorv[2]; 
		glBegin( GL_POINTS);
		glColor3ub(r,g,b);
		glVertex3f(i,alt,0.);
		glEnd();

	}

}

//Dados los 3 puntos de un triangulo junto con sus valores de textura dibuja de arriba abajo el espacio proporcional de la foto
void dibujar_triangulo( hiruki *tPTR) {
	punto *supPTR,*infPTR,*midPTR;
	//Ordena los 3 puntos en base a su altura
	if(tPTR->p1.y > tPTR->p2.y){
		supPTR = &(tPTR->p1);
		midPTR = &(tPTR->p2);

	} else {
		supPTR = &(tPTR->p2);
		midPTR = &(tPTR->p1);

	}

	if(tPTR->p3.y < midPTR->y) {
		infPTR = &(tPTR->p3);

	} else {
		infPTR = midPTR;
		if(supPTR->y > tPTR->p3.y){
			midPTR = &(tPTR->p3);

		} else {
			midPTR = supPTR;
			supPTR = &(tPTR->p3);

		}
	}
	
	printf("%s", "el mas alto es: " );
	printf("%f\n", supPTR->y );
	printf("%s", "el mediano es: " );
	printf("%f\n", midPTR->y );
	printf("%s", "el mas bajo es: " );
	printf("%f\n", infPTR->y );

	//puntos auxiliares para no modificar los valores originales.
	punto p1,p2;
	p1.x = supPTR->x;
	p1.y = supPTR->y;
	p1.z = supPTR->z;
	p1.u = supPTR->u;
	p1.v = supPTR->v;

	p2.x = supPTR->x;
	p2.y = supPTR->y;
	p2.z = supPTR->z;
	p2.u = supPTR->u;
	p2.v = supPTR->v;

	// Variaciones del resto de las variables respecto a la Y
	float mSM;
	float mSI;
	float mMI;

	float uSM;
	float uSI;
	float uMI;

	float vSM ;
	float vSI;
	float vMI ;

	//todos los puntos estan en la misma altura
	if(midPTR->y - supPTR->y == 0 && infPTR->y - supPTR->y == 0 &&  infPTR->y - midPTR->y == 0 ){
		 printf("%s\n","No es un triangulo" );

	//punto medio y superior en la misma altura
	} else if (midPTR->y - supPTR->y == 0) {
		printf("%s\n", "apunta hacia abajo" );
		mSM = 0;
		uSM = 0;
		vSM = 0;

		p1.x = midPTR->x;
		p1.y = midPTR->y;
		p1.z = midPTR->z;
		p1.u = midPTR->u;
		p1.v = midPTR->v;

		mSI = (infPTR->x - supPTR->x) / (infPTR->y-supPTR->y);
		mMI = (infPTR->x - midPTR->x) / (infPTR->y-midPTR->y );
		 
		uSI = (infPTR->u - supPTR->u) / (infPTR->y-supPTR->y);
		uMI = (infPTR->u - midPTR->u) / (infPTR->y-midPTR->y);

		vSI = (infPTR->v - supPTR->v) / (infPTR->y - supPTR->y);
		vMI = (infPTR->v - midPTR->v) / (infPTR->y - midPTR->y);

	//punto medio e inferior en la misma altura
	} else if (infPTR->y - midPTR->y == 0) {
		printf("%s\n", "apunta hacia arriba");
		mMI = 0;
		uMI = 0;
		vMI = 0;

		mSM = (midPTR->x - supPTR->x) / (midPTR->y-supPTR->y);
		mSI = (infPTR->x - supPTR->x) / (infPTR->y-supPTR->y);
		 
		uSM = (midPTR->u - supPTR->u) / (midPTR->y-supPTR->y);
		uSI = (infPTR->u - supPTR->u) / (infPTR->y-supPTR->y);
		 
		vSM = (midPTR->v - supPTR->v) / (midPTR->y - supPTR->y);
		vSI = (infPTR->v - supPTR->v) / (infPTR->y - supPTR->y);

	//todos los puntos en diferentes alturas	
	} else {
		// Variaciones del resto de las variables respecto a la Y
		mSM = (midPTR->x - supPTR->x) / (midPTR->y-supPTR->y);
		mSI = (infPTR->x - supPTR->x) / (infPTR->y-supPTR->y);
		mMI = (infPTR->x - midPTR->x) / (infPTR->y-midPTR->y );

		uSM = (midPTR->u - supPTR->u) / (midPTR->y-supPTR->y);
		uSI = (infPTR->u - supPTR->u) / (infPTR->y-supPTR->y);
		uMI = (infPTR->u - midPTR->u) / (infPTR->y-midPTR->y);

		vSM = (midPTR->v - supPTR->v) / (midPTR->y - supPTR->y);
		vSI = (infPTR->v - supPTR->v) / (infPTR->y - supPTR->y);
		vMI = (infPTR->v - midPTR->v) / (infPTR->y - midPTR->y);
	}

	//dibujar el primer punto
	unsigned char* colorv;
	unsigned char r,g,b;

	colorv = color_textura(p1.u,p1.v);
	r = colorv[0];
	g =colorv[1];
	b = colorv[2]; 
	glBegin(GL_POINTS);
	glColor3ub(r,g,b);
	glVertex3f(p1.x,p1.y,0.);
	glEnd();


	//Bucle para movernos desde el punto mas alto hasta el medio
	for(float i = supPTR->y; i > midPTR->y; i--){
		
		//Resta de la variacion ya que nos movemos de arriba a abajo	
		p1.x = p1.x - mSM;
		p2.x = p2.x - mSI;

		if (p1.u - uSM >= 0 && p2.u - uSI >= 0) {
			p1.u = p1.u - uSM;
			p2.u = p2.u - uSI;

		} else {
			printf("%s\n","Se han sobrepasado los limites de las coords U (<0)" );

		}
			
		if (p1.v - vSM >= 0 && p2.v - vSI >= 0) {
			p1.v = p1.v - vSM;
			p2.v = p2.v - vSI;

		} else {
			printf("%s\n","Se han sobrepasado los limites de las coords V(<0)" );

		}
			
		if(p1.x >= p2.x){
			dibujar_segmento(&p2,&p1,i);

		} else {
			dibujar_segmento(&p1,&p2,i);

		}
	
	}

	//bucle que hace la segunda mitad del triangulo desde el punto medio al bajo
	for (float j = midPTR->y; j > infPTR->y; j--) {
		p1.x = p1.x - mMI;
		p2.x = p2.x - mSI;

		p1.u = p1.u - uMI;
		p2.u = p2.u - uSI;

		p1.v = p1.v - vMI;
		p2.v = p2.v - vSI;

		if(p1.x >= p2.x) {
			dibujar_segmento(&p2,&p1,j);

		} else {
			dibujar_segmento(&p1,&p2,j);

		}
		
	}

}


static void marraztu(void) {
	float u,v;
	float i,j;
	unsigned char* colorv;
	unsigned char r,g,b;

	// borramos lo que haya...
	glClear( GL_COLOR_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500.0, 0.0, 500.0,-250.0, 250.0);
	printf("%s %d \n", "el indice es: ",indice +1);
	dibujar_triangulo(triangulosptr + indice);	
	glFlush();

}


// This function will be called whenever the user pushes one key
static void teklatua (unsigned char key, int x, int y)
{
switch(key)
	{
	case 13: 
		printf ("ENTER: que hay que dibujar el siguiente triángulo.\n");
		/* hacer algo para que se dibuje el siguiente triangulo */
		/*
		indice ++;  // pero si es el último? hay que controlarlo!
		*/
		// printf("%s %d \n", "el indice: ", indice );
		printf("%s %d \n", "num_triangles: ", num_triangles );
		//Avanzar al siguiente triangulo o al primero en caso de que sea el ultimo
			if(indice < num_triangles-1){
				indice++;
			}else{
				indice=0;
			}
			

		break;
	case 27:  // <ESC>
		exit( 0 );
		break;
	default:
		printf("%d %c\n", key, key );
	}

// The screen must be drawn to show the new triangle
glutPostRedisplay();
}

int main(int argc, char** argv)
{
	printf("%s\n","HOLA");

	printf("This program draws points in the viewport \n");
	printf("Press <ESC> to finish\n");
	glutInit(&argc,argv);
	glutInitDisplayMode ( GLUT_RGB );
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition ( 100, 100 );
	glutCreateWindow( "GL_POINTS" );

	glutDisplayFunc( marraztu );
	glutKeyboardFunc( teklatua );
	/* we put the information of the texture in the buffer pointed by bufferra. The dimensions of the texture are loaded into dimx and dimy */ 
        load_ppm("foto.ppm", &bufferra, &dimx, &dimy);
        //lee la información de los triángulos... pero hay que definir las variables!
        cargar_triangulos(&num_triangles, &triangulosptr);
        printf("%s", " tenemos ");
        printf("%d", num_triangles );
        printf("%s\n"," triangulos " );
        printf("%s\n", "Los puntos del triangulo son: " );

        printf("%f", triangulosptr->p1.x );
        printf("%s", " , " );
        printf("%f", triangulosptr->p1.y);
        printf("%s\n",  "");
 
        printf("%f", triangulosptr->p2.x );
        printf("%s", " , " );
        printf("%f", triangulosptr->p2.y);
        printf("%s\n", "" );

        printf("%f", triangulosptr->p3.x );
        printf("%s", " , " );
        printf("%f", triangulosptr->p3.y);
        printf("%s\n", "" );


        
       
	glClearColor( 0.0f, 0.0f, 0.7f, 1.0f );

	glutMainLoop();

	return 0;   
}