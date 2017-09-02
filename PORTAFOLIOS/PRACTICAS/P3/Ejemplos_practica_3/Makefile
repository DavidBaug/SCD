.SUFFIXES:
.PHONY:    msg_uso,ej,ej1,ej2,ej3,clean

# -----------------------------------------------

msg_uso:
	@echo ""
	@echo "SCD: Implementacion de algoritmos distribuidos con MPI."
	@echo "usar con:"
	@echo "   make ej1	: compilar y ejecutar Productor-Consumidor"	
	@echo "   make ej2	: compilar y ejecutar Cena de los filosofos"
	@echo "   make ej3	: compilar y ejecutar Cena de los filosofos con camarero"	
	@echo ""
	@echo "   make clean	: elimina todos los archivos .o"
	@echo ""

# -----------------------------------------------

ej: *.cpp
	mpicxx -g -Wall ej1_pc.cpp -o ej1.o
	mpicxx -g -Wall ej2_filo.cpp -o ej2.o
	mpicxx -g -Wall ej3_cam.cpp -o ej3.o
	g++ -Wall pinta.cpp -o pinta.o

ej1: 	ej1_pc.cpp
	mpicxx -g -Wall ej1_pc.cpp -o ej1.o
	mpirun -np 10 ./ej1.o

ej2: ej2_filo.cpp
	g++ -Wall pinta.cpp -o pinta.o
	mpicxx -g -Wall ej2_filo.cpp -o ej2.o
	./pinta.o
	mpirun -np 10 ./ej2.o

ej3: ej3_cam.cpp pinta.cpp
	g++ -Wall pinta.cpp -o pinta.o
	mpicxx -g -Wall ej3_cam.cpp -o ej3.o
	./pinta.o
	mpirun -np 11 ./ej3.o

# -----------------------------------------------

clean:
	rm -f *.o
	
