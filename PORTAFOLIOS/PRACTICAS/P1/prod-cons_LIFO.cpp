// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio del productor-consumidor con
// buffer intermedio.
//
// *****************************************************************************
// David Gil Bautista

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // necesario para {\ttbf usleep()}
#include <stdlib.h> // necesario para {\ttbf random()}, {\ttbf srandom()}
#include <time.h>   // necesario para {\ttbf time()}

// Colores consola
#define BLACK    "\033[0m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define BLUE     "\033[34m"
#define MAGENTA  "\033[35m"
#define CYAN     "\033[36m"
#define WHITE    "\033[37m"

using namespace std ;

// ---------------------------------------------------------------------
// constantes configurables:

const unsigned
  num_items  = 40 ,    // numero total de items que se producen o consumen
  tam_vector = 10 ;    // tamaño del vector, debe ser menor que el número de items

sem_t producir;
sem_t consumir;
sem_t mutex;

unsigned buffer[tam_vector];
int indice = 0;

// ---------------------------------------------------------------------
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)

void retraso_aleatorio( const float smin, const float smax )
{
  static bool primera = true ;
  if ( primera )        // si es la primera vez:
  {  srand(time(NULL)); //   inicializar la semilla del generador
     primera = false ;  //   no repetir la inicialización
  }
  // calcular un número de segundos aleatorio, entre {\ttbf smin} y {\ttbf smax}
  const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  // dormir la hebra (los segundos se pasan a microsegundos, multiplicándos por 1 millón)
  usleep( (useconds_t) (tsec*1000000.0)  );
}

// ---------------------------------------------------------------------
// función que simula la producción de un dato

unsigned producir_dato()
{
	sem_wait(&mutex);

  static int contador = 0 ;
  contador = contador + 1 ;
  retraso_aleatorio( 0.1, 0.5 );

  cout << CYAN << "Dato producido: " << contador << CYAN <<endl << flush ;

  sem_post(&mutex);

  return contador ;
}
// ---------------------------------------------------------------------
// función que simula la consumición de un dato

void consumir_dato( int dato )
{

	sem_wait(&mutex);

  retraso_aleatorio( 0.1, 1.5 );
  cout << YELLOW <<"Dato consumido: " << dato << YELLOW << endl << flush ;
  
  sem_post(&mutex);

}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
  	sem_wait(&producir);

    int dato = producir_dato() ;

    buffer[indice] = dato;
    indice++;
    
    //cout << "Productor : dato insertado: " << dato << endl << flush ;

    sem_post(&consumir);
  }
  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * )
{
  for( unsigned i = 0 ; i < num_items ; i++ )
  {
  	sem_wait(&consumir);

		indice--;
    int dato = buffer[indice];
    
    //cout << "Consumidor:                              dato extraído : " << dato << endl << flush ;
    consumir_dato( dato ) ;

    sem_post(&producir);
  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
	sem_init(&producir, 0, tam_vector);
	sem_init(&consumir, 0, 0);
	sem_init(&mutex, 0, 1);

	pthread_t productor, consumidor;

	pthread_create(&productor,NULL,funcion_productor,NULL);
	pthread_create(&consumidor,NULL,funcion_consumidor,NULL);

	pthread_join(productor,NULL);
	pthread_join(consumidor,NULL);

	sem_destroy(&producir);
	sem_destroy(&consumir);
	sem_destroy(&mutex);

   return 0 ;
}