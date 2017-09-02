// *****************************************************************************
//
// Prácticas de SCD. Práctica 1.
// Plantilla de código para el ejercicio de los fumadores
//
// *****************************************************************************

#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

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
// introduce un retraso aleatorio de duración comprendida entre
// 'smin' y 'smax' (dados en segundos)



sem_t sem_estanquero[2]; // En la primera iteración se puede producir
sem_t sem_fumador[6];
sem_t mutex;

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

// ----------------------------------------------------------------------------
// función que simula la acción de fumar, como un retardo aleatorio de la hebra.
// recibe como parámetro el numero de fumador
// el tiempo que tarda en fumar está entre dos y ocho décimas de segundo.

void fumar( int num_fumador )
{
   cout << YELLOW <<"Fumador número " << num_fumador << ": comienza a fumar." << YELLOW << endl << flush ;
   retraso_aleatorio( 0.2, 0.8 );
   cout << MAGENTA <<"Fumador número " << num_fumador << ": termina de fumar." << MAGENTA << endl << flush ;
}
// ----------------------------------------------------------------------------

// falta: resto de funciones
// ..............

void * estanquero(void * ih_void)
{

    unsigned long ih = (unsigned long) ih_void;

    while (true)
    {
        sem_wait(&sem_estanquero[ih]);

        // calcular un numero aleatorio {0,1,2}
        unsigned int ingrediente = (rand() % 3) ;

        sem_wait(&mutex);
        cout << CYAN << "Estanquero "<< ih <<" produce el ingrediente: " << ingrediente << CYAN << endl << flush;
        sem_post(&mutex);

        //Depende del dependiente se lo ofrezco a un fumador u otro

        if (ih == 0)
          sem_post(&sem_fumador[ingrediente]);
        else
          sem_post(&sem_fumador[ingrediente+3]);
    }
}

void * fumadores(void * ih_void)
{

  unsigned long ih = (unsigned long) ih_void ;

  while(true)
  {
    sem_wait(&sem_fumador[ih]); //Fumador espera a que se produzca ingrediente


    sem_wait(&mutex);
    cout  <<WHITE <<"Fumador " << ih << " recoge ingrediente." << WHITE << endl << flush;
    sem_post(&mutex);

    //Si el fumador es mayor que 3 [0,1,2], llamo al dependiente 2    

    if( ih > 2)
      sem_post(&sem_estanquero[0]); //Como ya se ha recogido el ingrediente y se está fumando se puede colocar otro
    else
      sem_post(&sem_estanquero[1]);

    fumar(ih); //Comienza a fumar fumador[ih]
    
  }
}

// ----------------------------------------------------------------------------

int main()
{
  srand( time(NULL) ); // inicializa semilla aleatoria para selección aleatoria de fumador

  //cout << "Introduce numero de fumadores: ";
  	const int nFuma = 6;


    //Inicializamos los semáforos
    sem_init(&mutex, 0, 1);

    sem_init(&sem_estanquero[0], 0, 1); // inicialmente se puede producir
    sem_init(&sem_estanquero[1], 0, 1); // inicialmente se puede producir
   
    for (int i = 0; i < nFuma; ++i){
      sem_init(&sem_fumador[i], 0, 0); // inicialmente no se puede fumar
    }

    // Declaramos las hebras
    pthread_t hebra_estanquero[2], hebra_fumador[nFuma];


    // Inicializamos las hebras
    pthread_create(&hebra_estanquero[0],NULL,estanquero,(void *)0); //Ejecuta estanquero
    pthread_create(&hebra_estanquero[1],NULL,estanquero,(void *)1); //Ejecuta estanquero

    for (unsigned long i = 0; i < nFuma; ++i)
    {
      pthread_create(&hebra_fumador[i],NULL,fumadores,(void * )i);   //Cada hebra fumadora ejecuta fumadores
    }

    // Lanza las hebras
    pthread_join(hebra_estanquero[0], NULL);
    pthread_join(hebra_estanquero[1], NULL);

    for (int i = 0; i < nFuma; ++i){
      pthread_join(hebra_fumador[i],NULL);
    }
    


    // Destruimos los semaforos, no tiene mucho sentido, ya que saldremos con Ctrl+C
    sem_destroy(&sem_estanquero[0]);
    sem_destroy(&sem_estanquero[1]);
    
    for (int i = 0; i < nFuma; ++i){
      sem_destroy(&sem_fumador[i]);
    }


  return 0 ;
}