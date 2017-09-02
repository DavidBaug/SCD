## ---------------------------------------------------------------------
## SCD. Makefile para la práctica 1.
## ---------------------------------------------------------------------
##
##	David Gil Bautista D3
##

.PHONY:  pcFIFO,pcLIFO, fu, msg, clean, todo
.SUFFIXES:

msg:
	@echo "Practica 1 SCD: "
	@echo "	usa 'make pcFIFO' para compilar y ejecutar el programa de los productores - consumidores versión FIFO."
	@echo "	usa 'make pcLIFO' para compilar y ejecutar el programa de los productores - consumidores versión LIFO."
	@echo "	usa 'make pcNL' para compilar y ejecutar el programa de los productores - consumidores versión N-productores-consumidores LIFO."
	@echo "	usa 'make pcNF' para compilar y ejecutar el programa de los productores - consumidores versión N-productores-consumidores FIFO."
	@echo ""
	@echo "	usa 'make fu' para compilar y ejecutar el programa de los fumadores y el estanquero."
	@echo ""
	@echo "	usa 'make clean' para borrar todos los archivos creados al compilar."
	@echo "	usa 'make todo' para recompilar (si es necesario) los dos programas."

## ---------------------------------------------------------------------

pcFIFO: pcF	
	./prod-consF

prod-consF: prod-cons_FIFO.cpp
	g++ -g -c prod-cons_FIFO.cpp

pcF: prod-consF
	g++ -o prod-consF prod-cons_FIFO.o -lrt -lpthread

## ---------------------------------------------------------------------

pcLIFO: pcL
	./prod-consL

prod-consL: prod-cons_LIFO.cpp
	g++ -g -c prod-cons_LIFO.cpp

pcL: prod-consL
	g++ -o prod-consL prod-cons_LIFO.o -lrt -lpthread

## ---------------------------------------------------------------------

pcNL: pcNpcL
	./prod-consNL

prod-consNL: prod-cons_NLIFO.cpp
	g++ -g -c prod-cons_NLIFO.cpp

pcNpcL: prod-consNL
	g++ -o prod-consNL prod-cons_NLIFO.o -lrt -lpthread

## ---------------------------------------------------------------------

pcNF: pcNpcF
	./prod-consNF

prod-consNF: prod-cons_NFIFO.cpp
	g++ -g -c prod-cons_NFIFO.cpp

pcNpcF: prod-consNF
	g++ -o prod-consNF prod-cons_NFIFO.o -lrt -lpthread

## ---------------------------------------------------------------------


fu: fuma	
	./fumadores

fumadores: fumadores.cpp 
	g++ -g -c fumadores.cpp

fuma: fumadores
	g++ -o fumadores fumadores.o -lrt -lpthread

	## ---------------------------------------------------------------------

fu2: fuma2	
	./estancos

fumadores2: estancos2.cpp 
	g++ -g -c estancos2.cpp

fuma2: fumadores
	g++ -o estancos estancos2.o -lrt -lpthread

	## ---------------------------------------------------------------------

todo:
	make fumadores
	make prod-consF
	make prod-consL
	make prod-consNF
	make prod-consNL

## ---------------------------------------------------------------------

clean:
	rm -rf *.o fumadores prod-consL prod-consF prod-consNF prod-consNL

## ---------------------------------------------------------------------










