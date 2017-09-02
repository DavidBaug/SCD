/* COLORES ANSI PARA LA CONSOLA
 *    System.out.println("\033[0m BLACK");
 *    System.out.println("\033[31m RED");

 *    System.out.println("\033[32m GREEN");
 *    System.out.println("\033[33m YELLOW");
 *    System.out.println("\033[34m BLUE");
 *    System.out.println("\033[35m MAGENTA");
 *    System.out.println("\033[36m CYAN");
 *    System.out.println("\033[37m WHITE");
*/

import java.util.Random;
import monitor.*;

class Barberia extends AbstractMonitor
{
    private Condition cliente = makeCondition();
    private Condition barbero = makeCondition();
    private Condition silla = makeCondition();

    private Boolean dormido;
    private Boolean primeraVez = true;

    private static int counter=0;

    //-----------------------------------------------------------------------------------------

    public static void finalizar(int salida){

        counter++;

        if (counter == salida) {
            System.out.println("\033[33m"+"Ya se ha ejecutado "+salida+" veces."+"\033[33m");
            System.exit(0);
        }

    }

    // invocado por los clientes para cortarse el pelo
    public void cortarPelo(){
        enter();

            // Si las silla está ocupada...
            if (!silla.isEmpty()){
                // .. pasamos el cliente a la sala de espera
                System.out.println("\033[37m"+" ::: Silla ocupada ::: " + "\033[37m");
                cliente.await();
            }

            if(dormido){
                System.out.println("\033[36m"+" ´´´ Cliente despierta barbero ´´´ "+"\033[36m");
                dormido = false;
            }

            // Pelamos al cliente (despertamos al barbero y ponemos al cliente en la silla)
            System.out.println("\033[35m"+"     Cliente empieza a cortarse el pelo" + "\033[35m");
            barbero.signal();
            silla.await();

        leave();
    }
    // invocado por el barbero para esperar (si procede) a un nuevo cliente y sentarlo para el corte
    public void siguienteCliente(){
        enter();

            // Si la sala y la silla están vacias...
            if (cliente.isEmpty() && silla.isEmpty())
            {
                // ...ponemos al barbero a dormir
                System.out.println("\033[31m"+" *** Barbero se pone a dormir " + "\033[31m");
                dormido = true;
                barbero.await();

            }

            if(!primeraVez){
                // Sacamos al siguiente cliente de la sala de espera
                System.out.println("\033[32m"+" +++ Barbero coge otro cliente " + "\033[0m");
            }
            
            cliente.signal();
            primeraVez=false;

        leave();
    }

    // invocado por el barbero para indicar que ha terminado de cortar el pelo
    public void finCliente()
    {
        enter();

            // Sacamos al cliente de la silla (estaba en espera la hebra mientras se pelaba)
            System.out.println("\033[34m"+" --- Barbero termina de afeitar " + "\033[34m");
            silla.signal();

        leave();
    }
}

class Cliente implements Runnable
{
    private Barberia barberia;
    public Thread thr;

    public Cliente(Barberia barberia)
    {
        this.barberia = barberia;
        thr   = new Thread(this," - Cliente ");

    }

    public void run()
    {
        while (true)
        {
            try
            {
                barberia.cortarPelo();  // el cliente espera (si procede) y se corta el pelo
                auxBarbero.dormir_max( 2000 ); // el cliente esta fuera de la barberia un tiempo, evita que se solape
                
            }
            catch(Exception e)
            {
                System.err.println("Excepcion en main: " + e);
            }

        }
    }
}

class Barbero implements Runnable
{
    private Barberia barberia;
    public Thread thr;

    public Barbero(Barberia mon)
    {
        barberia = mon;
        thr   = new Thread(this," + Barbero");
    }

    public void run()
    {
        while (true)
        {
            try
            {
                barberia.siguienteCliente();
                auxBarbero.dormir_max( 3500 ); // el barbero esta cortando el pelo
                barberia.finCliente();
                Barberia.finalizar(5);
            }
            catch(Exception e)
            {
                System.err.println("Excepcion en main: " + e);
            }

        }
    }
}

class auxBarbero                                            // Dejamos un tiempo entre cada iteración del barbero
{
    static Random genAlea = new Random();
    static void dormir_max(int milisecsMax)
    {
        try
        {
            Thread.sleep(genAlea.nextInt(milisecsMax));         // Hebra que pausa la ejecución
        }
        catch(InterruptedException e)
        {
            System.err.println("sleep interumpido en 'aux.dormir_max()'");
        }
    }
}

class ej3_barb
{
    public static void main(String[] args)
    {
        final int NUM_CLIENTES = 6;

        // leer parametros, crear vectores y buffer intermedio
        Barberia barberia = new Barberia();

        // crear hebras
        Barbero barbero = new Barbero(barberia);
        Cliente[] clientes = new Cliente[NUM_CLIENTES];
        for (int i=0; i<NUM_CLIENTES; i++)
            clientes[i] = new Cliente(barberia);                // Inicializamos cada hebra del array clientes

        // poner en marcha las hebras
        barbero.thr.start();

        for (int i=0; i<NUM_CLIENTES; i++)
            clientes[i].thr.start();

        // auxBarbero.dormir_max( 6500 ); // el barbero está dormido (Espera)

    }
}

