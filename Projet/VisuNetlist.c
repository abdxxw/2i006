#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "netlist.h"
#include "entree_sortie.h"
#include <time.h>

int main(int argc, char ** argv){

    
    netlist * nL= init_netlist_fic (argv[1]); // fichier a lire
    ecrire_netlist_fic (argv[2],nL); // fichier a ecrire
    Sauvegarde_intersection(argv[3],nL,0); // sauvgarder inter
    Dessiner_Netlist(nL,"./Jeu_essai/a");
    liberer_netlist(nL);
/*
    char * Tfic [] = {"InstancesA/alea0030_030_10_088.net","InstancesA/alea0030_030_90_007.net","InstancesA/alea0100_050_10_097.net","InstancesA/alea0100_080_90_024.net","InstancesA/alea0300_300_10_044.net","InstancesA/alea0300_300_90_099.net","InstancesA/alea0500_500_10_054.net","InstancesA/alea0500_500_90_033.net","InstancesA/c1.net","InstancesA/c4.net","InstancesA/c5.net","InstancesA/c5_2.net","InstancesA/ibm01-1000_2.net","InstancesA/ibm01-5000_2.net","InstancesA/ibm01-10000_2.net","InstancesA/ibm01_2.net"};
    FILE * fictmp=NULL,* f1,* f2,* f3;
    f1=fopen("naif.txt","w");
    f2=fopen("balayage.txt","w");
    f3=fopen("avl.txt","w");
    clock_t  temps_initial;
    clock_t  temps_final;
    double temps_cpu1,temps_cpu2,temps_cpu3;
     for (i=12;i<16;i++) {
        fictmp=fopen(Tfic[i],"r");
        nL= init_netlist_fic (Tfic[i]); // fichier a lire
        printf("%s\n",Tfic[i]);
        fscanf(fictmp,"%d",&j);
        temps_initial = clock();
        intersect_naif(nL); 
        temps_final = clock();
        temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

        nL= init_netlist_fic (Tfic[i]); // fichier a lire
        
        temps_initial = clock();
        intersect_balayage(nL);  
        temps_final = clock();
        temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

	    nL= init_netlist_fic (Tfic[i]);
        temps_initial = clock();
        intersect_balayage_AVL(nL);
        temps_final = clock();
        temps_cpu3 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

        //printf("temps d'execution pour naif : %f , balayage : %f , avl : %f \n",temps_cpu1,temps_cpu2,temps_cpu3);

        fprintf(f1,"%d %f\n",j,temps_cpu1);
        fprintf(f2,"%d %f\n",j,temps_cpu2);
	    fprintf(f3,"%d %f\n",j,temps_cpu3);
        liberer_netlist(nL);

        fclose(fictmp);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
*/

    return 0;
}
