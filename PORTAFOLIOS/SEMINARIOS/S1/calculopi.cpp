// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

// David Gil Bautista 
// Grupo D3 
// DNI: 45925324M

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include "fun_tiempo.h"

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)

const unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
const unsigned n      = 4 ;                      // número de hebras

int nhebras;

double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( )
{  
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ) 
{  
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ;

   // calcular suma parcial en "sumap"
   for (double i = ih; i < m; i+=nhebras ){
		sumap += f((i+0.5)/m);
   }

   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   return NULL ;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente( )
{  
   // crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
 
   double sumaTotal = 0.0;
   pthread_t hebras[nhebras];

   for (int i = 0; i < nhebras; ++i)
      pthread_create(&(hebras[i]), NULL, funcion_hebra, (void *)i);
   
   // esperar (join) a que termine cada hebra, sumar su resultado
   
   for (int i = 0; i < nhebras; ++i)
      pthread_join(hebras[i],NULL);
   

   // devolver resultado completo
 
   for (int i = 0; i < nhebras; ++i)
   		sumaTotal+= resultado_parcial[i];

   return sumaTotal/m ;
}

// ---------------------------------------------------------------------

int main(int argc, char *argv[])
{

   	cout << endl <<"Introduce numero de procesos nhebras: ";
   	cin >> nhebras;

	cout << endl <<"Ejemplo 4 (cálculo de PI)" << endl ;
   	double pi_sec = 0.0, pi_conc = 0.0 ;


    //------------------------------------------------------------------------------------------------------------   

	struct timespec inicio_s = ahora(); // inicio = inicio del tiempo a medir

    // Calculo secuencial
	pi_sec  = calcular_integral_secuencial() ;

    struct timespec fin_s = ahora(); // fin = fin del tiempo a medir


    struct timespec inicio_c = ahora(); // inicio = inicio del tiempo a medir

    // Calculo concurrente
	pi_conc = calcular_integral_concurrente() ;

    struct timespec fin_c = ahora(); // fin = fin del tiempo a medir

    //------------------------------------------------------------------------------------------------------------

  	cout <<"\nvalor de PI (calculado secuencialmente)  == " << pi_sec  << endl 
 		 << "valor de PI (calculado concurrentemente) == " << pi_conc << endl ;

    cout << endl << "Duracion secuencialmente: " << duracion(&inicio_s, &fin_s) << endl; 
    cout << "Duracion concurrentemente para " << nhebras << " procesos: "<< duracion(&inicio_c, &fin_c) << endl;
   	cout << endl;

   	return 0 ;
}