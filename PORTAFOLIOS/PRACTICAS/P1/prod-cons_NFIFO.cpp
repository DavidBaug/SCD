// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio del productor-consumidor con
// buffer intermedio.
//
// *****************************************************************************

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
  num_items  = 20,    // numero total de items que se producen o consumen
  tam_vector = 10 ;    // tamaño del vector, debe ser menor que el número de items

sem_t producir;
sem_t consumir;
sem_t mutex;
sem_t mutex1;

unsigned buffer[tam_vector];
int escritos = num_items;
int leidos = num_items;

int indiceP = 0;
int indiceC = 0;

bool primeraVez = true;

int prod, cons;

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

unsigned producir_dato(unsigned long prod)
{
  static int contador = 0 ;
  contador = contador + 1 ;
  retraso_aleatorio( 0.1, 0.5 );

  cout << RED << "Productor "<< prod <<" produce: " << contador << RED <<endl << flush ;

  return contador ;
}

// ---------------------------------------------------------------------
// función que simula la consumición de un dato

void consumir_dato( unsigned long consumidor, int dato )
{
  retraso_aleatorio( 0.1, 1.5 );
  cout << GREEN <<"Consumidor "<<consumidor<<" consume dato: " << dato << GREEN << endl << flush ;
  
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del productor

void * funcion_productor( void * ih_void)
{
  unsigned long ih = (unsigned long) ih_void ;

  for( unsigned i = 0 ; i < num_items/prod ; i++ )
  {

  	sem_wait(&mutex1);
    int dato = producir_dato(ih) ;
  	sem_post(&mutex1);

  	sem_wait(&producir);
  	sem_wait(&mutex);
	
    buffer[indiceP] = dato;
	indiceP=(indiceP+1)%tam_vector;

    sem_post(&consumir);
  	sem_post(&mutex);

    cout << CYAN <<"\tProductor : "<< ih <<" dato insertado: " << dato << CYAN << endl << flush ;

    escritos--;

  }
  return NULL ;
}
// ---------------------------------------------------------------------
// función que ejecuta la hebra del consumidor

void * funcion_consumidor( void * ih_void)
{

	unsigned long ih = (unsigned long) ih_void ;

  for( unsigned i = 0 ; i < num_items/cons ; i++ )
  {
  	sem_wait(&consumir);
  	
  	sem_wait(&mutex);

    int dato = buffer[indiceC];
  	indiceC=(indiceC+1)%tam_vector;  

  	sem_post(&producir);
	sem_post(&mutex);

	leidos--;
    
    cout << YELLOW <<"\tConsumidor: " << ih << " dato extraído : " << dato << YELLOW << endl << flush ;
    consumir_dato( ih, dato ) ;

  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{
	cout << GREEN <<"El numero de items es 20 por lo que se agradecería que los consumidores y productores fueran múltiplo de tal." << GREEN <<endl;

	cout << "\nintroduce el numero de productores: ";
	cin >> prod;

	cout << "introduce el numero de consumidores: ";
	cin >> cons;

	sem_init(&producir, 0, tam_vector);
	sem_init(&consumir, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&mutex1,0,1);

	pthread_t productor[prod], consumidor[cons];


	// Inicializar
	for (int i = 0; i < prod; ++i)
		pthread_create(&productor[i],NULL,funcion_productor,(void *)i);			
	

	for (int i = 0; i < cons; ++i)
		pthread_create(&consumidor[i],NULL,funcion_consumidor,(void *)i);
	


	// Lanzar
	for (int i = 0; i < prod; ++i)
		pthread_join(productor[i],NULL);
	

	for (int i = 0; i < cons; ++i)
		pthread_join(consumidor[i],NULL);
	
	
	sem_destroy(&producir);
	sem_destroy(&consumir);
	sem_destroy(&mutex);
	sem_destroy(&mutex1);

	if(leidos == escritos)
		cout << "\n--------------Se han consumido todos los elementos--------------" << endl<<endl;

   return 0 ;
}