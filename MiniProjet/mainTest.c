#include <stdlib.h>
#include <stdio.h>
#include "biblio.h"
#include "biblioHash.h"
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>


// how to use : ./mainTest <file_name> <number_of_lignes> <title_to_search> <author_to_search>  <number_to_search>
int main (int argc , char ** argv){

     assert(atoi(argv[2])<=100000);
    Biblio * b=intialise_biblio();
    lecture_n_entree(strdup(argv[1]) ,atoi(argv[2]) ,b);
    tableHachage_t * Hb=initTableHachage(tM);
    Hlecture_n_entree(strdup(argv[1]) ,atoi(argv[2]) ,Hb);

    clock_t  temps_initial;
    clock_t  temps_final;
    double temps_cpu1,temps_cpu2;
    int i;
    FILE *f1=fopen("liste.txt","w");
    FILE *f2=fopen("tHash.txt","w");

    temps_initial = clock();
    for (i=0;i<1000;i++)
        recherche_titre(*b,argv[3]);   
    temps_final = clock();
    temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;


    temps_initial = clock();
    for (i=0;i<1000;i++)
        Hrecherche_titre(*Hb,argv[3]);   
    temps_final = clock();
    temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

    printf("temps d'execution de listee : %f , table de hachage : %f \n",temps_cpu1,temps_cpu2);



    temps_initial = clock();
    for (i=0;i<1000;i++)
        recherche_auteur(*b,argv[4]);   
    temps_final = clock();
    temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;


    temps_initial = clock();
    for (i=0;i<1000;i++)
        Hrecherche_auteur(*Hb,argv[4]);   
    temps_final = clock();
    temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

    printf("temps d'execution de listee : %f , table de hachage : %f \n",temps_cpu1,temps_cpu2);



    temps_initial = clock();
    for (i=0;i<1000;i++)
        recherche_num(*b,atoi(argv[5]));   
    temps_final = clock();
    temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;


    temps_initial = clock();
    for (i=0;i<1000;i++)
        Hrecherche_num(*Hb,atoi(argv[5]));   
    temps_final = clock();
    temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

    printf("temps d'execution de listee : %f , table de hachage : %f \n",temps_cpu1,temps_cpu2);
/*

    for(i=1000;i<50000;i+=1000){

        b=intialise_biblio();
        lecture_n_entree(strdup(argv[1]) ,i ,b);
        Hb=initTableHachage(tM);
        Hlecture_n_entree(strdup(argv[1]) ,i ,Hb);

        temps_initial = clock();
        recherche_double(*b);  
        temps_final = clock();
        temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;
        

        temps_initial = clock();
        Hrecherche_double(*Hb);  
        temps_final = clock();
        temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;

        fprintf(f1,"%d %f\n",i,temps_cpu1);
        fprintf(f2,"%d %f\n",i,temps_cpu2);

    }
*/
    fclose(f1);
    fclose(f2);

    return 0;
}
