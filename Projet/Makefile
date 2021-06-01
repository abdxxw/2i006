C_FLAGS = -g -Wall -lm

all: VisuNetlist VisuGraphe tests get_plot_files

entree_sortie.o : entree_sortie.c
	gcc $(C_FLAGS) -c entree_sortie.c

netlist.o : netlist.c
	gcc $(C_FLAGS) -c netlist.c

netlistG.o : netlistG.c
	gcc $(C_FLAGS) -c netlistG.c

SVGwriter.o : SVGwriter.c
	gcc $(C_FLAGS) -c SVGwriter.c

VisuNetlist.o : VisuNetlist.c
	gcc $(C_FLAGS) -c VisuNetlist.c

VisuGraphe.o : VisuGraphe.c
	gcc $(C_FLAGS) -c VisuGraphe.c

tests.o : tests.c
	gcc $(C_FLAGS) -c tests.c

get_plot_files.o : get_plot_files.c
	gcc $(C_FLAGS) -c get_plot_files.c

VisuNetlist	:  netlist.o entree_sortie.o VisuNetlist.o SVGwriter.o 
	gcc $(C_FLAGS) -o VisuNetlist netlist.o entree_sortie.o VisuNetlist.o SVGwriter.o 

VisuGraphe	:  netlist.o entree_sortie.o VisuGraphe.o SVGwriter.o netlistG.o
	gcc $(C_FLAGS) -o VisuGraphe netlist.o entree_sortie.o VisuGraphe.o SVGwriter.o netlistG.o

tests	:  netlist.o entree_sortie.o SVGwriter.o netlistG.o tests.o
	gcc $(C_FLAGS) -o tests netlist.o entree_sortie.o SVGwriter.o netlistG.o tests.o


get_plot_files	:  netlist.o entree_sortie.o SVGwriter.o netlistG.o get_plot_files.o
	gcc $(C_FLAGS) -o get_plot_files netlist.o entree_sortie.o SVGwriter.o netlistG.o get_plot_files.o

clean	:
	rm -f *.o VisuNetlist VisuGraphe tests get_plot_files
