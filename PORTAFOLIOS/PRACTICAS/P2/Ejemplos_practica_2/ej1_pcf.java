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

import monitor.*;	// Importar carpeta monitores

class ej1f_Buffer extends AbstractMonitor
{
    private int numSlots = 0;							// Tamanio del array buffer
    private int cont = 0;
    private int contP = 0;
    private int contC = 0;								// Contador para comparar el numero de posiciones en el array en las hebras
    private double[] buffer = null;

    public int consumidos=0;
    public int producidos=0;

    private Condition produciendo = makeCondition();	// Hebra "semaforo" produciendo
    private Condition consumiendo = makeCondition();	// Hebra "semaforo" consumiendo

    public ej1f_Buffer(int p_numSlots){		// Constructor del Buffer (Inicializamos el array)
        numSlots = p_numSlots;
        buffer = new double[numSlots];
    }

    public int getContP(){ return this.contP;}

    public int getContC(){ return this.contC;}


    public void depositar(double valor) throws InterruptedException
    {
        enter();
            if ( cont == numSlots){
                produciendo.await();
            }
            buffer[contP%numSlots] = valor;
            contP++;
            producidos++;

            cont++;

            consumiendo.signal();
        leave();
    }

    public double consumir() throws InterruptedException
    {
        enter();
            double valor;
            if ( cont == 0)
                consumiendo.await();
            contC++;
            consumidos++;

            cont--;

            valor = buffer[contC%numSlots];

            produciendo.signal();
        leave();
        return valor;
    }
}

class ej1f_Productor implements Runnable
{
    private ej1f_Buffer bb;			// Creamos objeto de la clase Buffer 
    int veces;						// Contador veces que se producen las cosas
    int numP;						// Si nos encontramos con n Productores este es el identificador de uno de estos
    Thread thr;						// Creamos hebra

    public ej1f_Productor(ej1f_Buffer pbb, int pveces, int pnumP){	// Constructor de la clase
        bb    = pbb;
        veces = pveces;
        numP  = pnumP;
        thr   = new Thread(this,"\033[35m"+"Productor "+numP);				// Inicializar hebra, el this significa que estamos haciendo 
    }																		// teferencia al objeto Productor con esta hebra

    public void run()									
    {																// Segun Alberto esto es lo que ejecuta la hebra en esta clase
        try
        {
            double item = this.numP;
            for (int i=0; i<veces; i++)
            {
                System.out.println("\033[37m"+thr.getName() + ", produciendo " + item + "\033[37m");
                bb.depositar(item++);
            }
        }
        catch(Exception e)
        {
            System.err.println("\033[36m"+"Excepcion en main: " + e +"\033[36m");
        }
    }
}

class ej1f_Consumidor implements Runnable
{
    private ej1f_Buffer bb;
    int veces;
    int numC;
    Thread thr;
    public ej1f_Consumidor(ej1f_Buffer pbb, int pveces, int pnumC)
    {
        bb    = pbb;
        veces = pveces;
        numC  = pnumC;
        thr   = new Thread(this,"\033[37m"+"  Consumidor "+numC);
    }
    public void run()
    {
        try
        {
            for (int i=0; i<veces; i++)
            {
                double item = bb.consumir ();
                System.out.println("\033[31m"+thr.getName() + ", consumiendo " + item + "\033[0m");
            }
        }
        catch(Exception e)
        {
            System.err.println("Excepcion en main: " + e);
        }
    }
}

class ej1_pcf
{
    public static void main(String[] args)
    {
        if ( args.length != 5 )
        {
          System.err.println("Necesita 5 argumentos: (1) núm.prod.  (2) núm.iter.prod.  (3) núm.cons.  (4) núm.iter.cons.  (5) tam.buf.");
          return ;
        }

        // leer parametros, crear vectores y buffer intermedio
        ej1f_Productor[]  prod = new ej1f_Productor[Integer.parseInt(args[0])];       // Numero productores
        int iter_prod = Integer.parseInt(args[1]);                                  // Iteraciones productor

        ej1f_Consumidor[] cons = new ej1f_Consumidor[Integer.parseInt(args[2])];		// Numero consumidores  
        int iter_cons = Integer.parseInt(args[3]);                                  // Iteraciones consumidor
        
        ej1f_Buffer buffer = new ej1f_Buffer(Integer.parseInt(args[4]));				// Tamaño buffer

        if (cons.length*iter_cons != prod.length*iter_prod)
        {
            System.err.println("No coinciden numero de items a producir con a consumir");
            return;
        }

        // crear hebras
        for (int i = 0; i < cons.length; i++)
            cons[i] = new ej1f_Consumidor(buffer,iter_cons,i);

        for (int i = 0; i < prod.length; i++)
            prod[i] = new ej1f_Productor(buffer,iter_prod,i);

        // poner en marcha las hebras
        for (int i = 0; i < prod.length; i++)
            prod[i].thr.start();

        for (int i = 0; i < cons.length; i++)
            cons[i].thr.start();

         if ((buffer.consumidos == buffer.producidos) && (buffer.producidos == cons.length*iter_cons)) {
            System.out.println("\033[36m"+"Se han consumido todos los elementos");
        }
    }
}