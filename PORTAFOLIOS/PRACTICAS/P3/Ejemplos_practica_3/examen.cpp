// Colores consola
#define BLACK    "\033[0m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define BLUE     "\033[34m"
#define MAGENTA  "\033[35m"
#define CYAN     "\033[36m"
#define WHITE    "\033[37m"

// ------------------------------------------------------------------------------------------------

#include <iostream>
#include <cmath>
#include "mpi.h"

using namespace std;  // --------------------------------------------------------------------------

#define Productor  1 
#define Consumidor 2
#define Buffer     5 // Numero del proceso

#define ITERS 12
#define TAM 3

int producido=0; 
int consumido = 0;
int array[3];

// ------------------------------------------------------------------------------------------------

// Metodos
void productor(int numproductor)
{
    // Cada productor produce 6 valores: 12 elementos producidos (2 productores)
    for(unsigned int i = 0; i < 6; i++)
    {
        // Producimos un valor
        cout << CYAN << "Productor " << numproductor << " produce el valor: " << i << endl << BLACK << flush;
        MPI_Ssend(&i, 1, MPI_INT, Buffer, Productor, MPI_COMM_WORLD);   // La etiqueta del productor es 1
    }
}

void consumidor(int numconsumidor)
{
    int valor, peticion=1;
    //float raiz;
    MPI_Status status;

    // Cada consumidor consume 4 valores, que por 3 consumidores serán 12 elementos consumidos
    for(unsigned int i = 0; i < 4; i++)
    {
        // De entrada el consumidor envia peticion para recibir datos
        MPI_Ssend(&peticion, 1, MPI_INT, Buffer, Consumidor, MPI_COMM_WORLD);        // La etiqueta de envio a consumidor es 2   ------ es como un wait

        // Una vez aceptada la petición por el buffer le pedimos al mismo que nos mande un valor producido
        MPI_Recv (&valor, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD, &status);  // La etiquete de envio de buffer a consumidor es 0

        cout << RED << "    Consumidor " << numconsumidor%3 << " recibe valor: " << valor << " de Buffer " << endl << BLACK << flush;
        //raiz = sqrt(valor);
        //cout << raiz << endl;
    }
}

// ------------------------------------------------------------------------------------------------

void buffer()
{
    int value[TAM], peticion, rama; // POS=0 porque en la primera iteración no hay producto que consumir
    MPI_Status status;
    int elementos= 0;

    for(unsigned int i = 0; i < 24; i++)
    {

        if (elementos==0)         // el consumidor no puede consumir, se debe producir  -BUFFER VACIO-
            rama=0;
        else if (elementos==TAM)  // el productor no puede producir, se debe consumir      -BUFFER COMPLETO-
            rama=1;
        else                // en caso contrario...
        {
            // ...se puede consumir o producir
            MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            //if (status.MPI_SOURCE < Buffer) // Se produce
            if (status.MPI_TAG == Productor) //segun la etiqeta que tenga sera un rama o otra.
                rama = 0;
            else
                rama = 1;
        }

        switch(rama)
        {
            case 0 : // Recibe del productor
            {
                // Recibe de cualquiera de los productores (etiqueta 1)

                MPI_Recv(&value[producido], 1, MPI_INT, MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status);

                cout << WHITE << "    Buffer recibe " << value[producido] << " de Productor " << status.MPI_SOURCE << endl << BLACK << flush;
                
                array[producido]++;

                if (array[producido]>1){
                    producido++;     // incrementamos pos ya que en este momento si hay producto para consumir
                }

                elementos++;

                producido = producido%TAM;

                break;
            }
            case 1 : // Envia al consumidor
            {
                // Recibe de cualquiera de los consumidores (etiqueta 2)
                MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, Consumidor, MPI_COMM_WORLD, &status);


                //if(array[consumido] >1 ){
                    // Devuelve al consumidor que le envió la peticion con etiquete 0
                    MPI_Ssend(&value[consumido], 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

                    cout << GREEN << "    Buffer envia " << value[consumido] << " a Consumidor " << status.MPI_SOURCE%3 << endl << BLACK << flush;                

                    elementos--;

                    consumido++;
                    consumido = consumido%TAM;
                    array[consumido]-=2;
                //}
                break;
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------

// Punto de entrada al programa
int main(int argc, char *argv[])
{
    // Inicialización de open-mpi
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    // Controlamos que estemos llamando correctamente al programa
    if(size!=6)
    {
        if (rank == 0) // Solo mostramos el mensaje 1 vez
            cout << YELLOW <<"Uso: mpirun -np 6 " << argv[0] << endl << BLACK;
    }
    else
    {
        // Llamamos a los metodos dependiendo de cual sea su numero "rank"
        if (rank < 2)
            productor(rank);
        else if (rank == Buffer)
            buffer();
        else
            consumidor(rank);
    }

    cout << YELLOW <<" ---------------------------------------------- Finalizado el proceso " << rank << endl << BLACK << flush;
    cout << endl;

    // Finalización de open-mpi
    MPI_Finalize();

    // Fin del programa
    return 0;

}
