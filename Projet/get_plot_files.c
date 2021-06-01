#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "netlistG.h"
#include "entree_sortie.h"
#include <time.h>

int main(int argc, char ** argv){

	//tableau des fichiers IBM a utilisé
	char * Tfic [] = {"InstancesA/ibm01-1000_2.net","InstancesA/ibm01-5000_2.net","InstancesA/ibm01-10000_2.net","InstancesA/ibm01_2.net"}; 
	//char * Tfic [] = {"InstancesA/ibm01-250_2.net","InstancesA/ibm01-500_2.net","InstancesA/ibm01-1000_2.net","InstancesA/ibm01-2500_2.net","InstancesA/ibm01-5000_2.net"}; 

    FILE * f1,* f2,* f3,* f4,* f5,* f6,* f7;
    /*f1=fopen("./Courbes_PartieA/naif.txt","w");
    f2=fopen("./Courbes_PartieA/balayage.txt","w");
    f3=fopen("./Courbes_PartieA/avl.txt","w");
    f4=fopen("./Courbes_PartieB/getvia.txt","w");
    f5=fopen("./Courbes_PartieB/bicolore.txt","w");
    f6=fopen("./Courbes_PartieB/getvia_nbvia.txt","w");
    f7=fopen("./Courbes_PartieB/bicolore_nbvia.txt","w");*/

    netlist * nL = NULL;
    Graphe * G = NULL;

    clock_t  temps_initial;
    clock_t  temps_final;


    int * via,nbvia=0;
    double temps_cpu1,temps_cpu2,temps_cpu3,temps_cpu4,temps_cpu5;
    int i,j;



     for (i=0;i<1;i++) {

	
        printf("calcule temps pour : %s\n",Tfic[i]);


    /*************** PARTIE A ******************/


        nL= init_netlist_fic (Tfic[i]); // fichier a lire
        temps_initial = clock();
        //intersect_naif(nL); 
        temps_final = clock();
        temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

	liberer_netlist(nL);
        nL= init_netlist_fic (Tfic[i]); // reset
        
	
        temps_initial = clock();
        //intersect_balayage(nL);  
        temps_final = clock();
        temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;


	liberer_netlist(nL);
        nL= init_netlist_fic (Tfic[i]); // reset


        temps_initial = clock();
        //intersect_balayage_AVL(nL);
        temps_final = clock();
        temps_cpu3 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;




       /* fprintf(f1,"%d %f\n",nL->NbRes,temps_cpu1);
        fprintf(f2,"%d %f\n",nL->NbRes,temps_cpu2);
	fprintf(f3,"%d %f\n",nL->NbRes,temps_cpu3);*/


    /*************** PARTIE B ******************/

	liberer_netlist(nL);
        nL= init_netlist_fic (Tfic[i]); // reset
        Sauvegarde_intersection("temp",nL,2);
	G = cree_graphe(nL,"temp"); 

        temps_initial = clock();
        via = getVia(G);  
        temps_final = clock();
        temps_cpu4 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;
        
	for(j=0;j<G->n;j++)
		if(via[j]==0) nbvia++;

	//fprintf(f6,"%d %d\n",nL->NbRes,nbvia);
	nbvia=0;


	liberer_graphe(G);
	G = cree_graphe(nL,"temp"); 
	system("rm -f temp");

        temps_initial = clock();
        via = bicolore(G);  
        temps_final = clock();
        temps_cpu5 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

	for(j=0;j<G->n;j++)
		if(via[j]==0) nbvia++;
	
	//fprintf(f7,"%d %d\n",nL->NbRes,nbvia);
	nbvia=0;

        //fprintf(f4,"%d %f\n",nL->NbRes,temps_cpu4);
	//fprintf(f5,"%d %f\n",nL->NbRes,temps_cpu5);

        printf("\n==============================\ntemps d'execution :\n naif : %f \n balayage : %f \n avl : %f \n getVia : %f \n bicolore : %f  \n==============================\n",nbvia,temps_cpu2,temps_cpu3,temps_cpu4,temps_cpu5);


	liberer_netlist(nL);

	liberer_graphe(G);
    }




    /*fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
    fclose(f5);
    fclose(f6);
    fclose(f7);*/


    return 0;
}
