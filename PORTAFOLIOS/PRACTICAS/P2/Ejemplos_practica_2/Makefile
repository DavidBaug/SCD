
.SUFFIXES:
.PHONY: msg_uso, msg_clases, comp, recomp, clean, pc, le, cd, ej, ej1, ej2, ej3

JAVASRCS:=$(wildcard *.java)
CLASSES_BASE:=$(shell grep class *.java | cut -d' ' -f2)
CLASSES:=$(addsuffix .class,$(CLASSES_BASE))

msg_uso:
	@echo ""
	@echo "SCD: Ejemplos de monitores en Java."
	@echo "Usar con:"
	@echo "   make comp  : compilar todos los archivos java si es necesario"
	@echo "   make recomp: compilar todo incondicionalmente"
	@echo ""
	@echo "   make pc    : compilar todo y ejecutar ejemplo del productor-consumidor ( 5 4 4 5 10)"
	@echo "   make le    : idem para el ejemplo de lectores-escritores (5 20 15 25)"
	@echo "   make cd    : idem para el ejemplo del contador de días"
	@echo "   make clean : elimina todos los archivos .class"
	@echo ""
	@echo "   make ej	 : Compila ejercicios. "
	@echo "   make ej1l	 : ejecuta Productor-consumidor version LIFO. "
	@echo "   make ej1f	 : ejecuta Productor-consumidor version FIFO. "
	@echo "   make ej2	 : ejecuta Fumadores. "
	@echo "   make ej3	 : ejecuta Barbero Durmiente. "
	@echo ""
	@echo "   make zip   : Comprime todos los archivos en un zip."
	@echo ""
	@echo ""
	
comp: msg_clases monitor_classes $(CLASSES)

recomp:
	make clean	
	make comp

$(CLASSES): $(JAVASRCS)
	javac -g $(JAVASRCS)

monitor_classes: $(wildcard monitor/*.class)
	javac -g monitor/*.java -d .
#--------------------------------------------------------------------------------------------------

clean:
	rm -rf *.class monitor/*.class SCD_P2.zip
	
#--------------------------------------------------------------------------------------------------

ej: ej1_pcl.java ej1_pcf.java ej2_fum.java ej3_barb.java
	javac ej1_pcl.java && javac ej1_pcf.java && javac ej2_fum.java && javac ej3_barb.java

ej1l: ej1_pcl.class
	java ej1_pcl 5 4 4 5 10

ej1f: ej1_pcf.class
	java ej1_pcf 5 4 4 5 10

ej2: ej2_fum.class
	java ej2_fum

ej3: ej3_barb.class
	java ej3_barb

#--------------------------------------------------------------------------------------------------

pc: MainProductorConsumidor.class
	java MainProductorConsumidor 5 4 4 5 10
	
le: MainLectorEscritor.class
	java MainLectorEscritor 5 20 15 25

cd: MainContadorDias.class
	java MainContadorDias

#--------------------------------------------------------------------------------------------------
	
msg_clases:
	@echo "fuentes java = " $(JAVASRCS)
	@echo "clases = " $(CLASSES_BASE)

#--------------------------------------------------------------------------------------------------

zip:	SCD_P2.zip

SCD_P2.zip:
	make clean
	zip SCD_P2.zip *.java *.txt Makefile monitor/*