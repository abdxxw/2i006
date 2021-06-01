#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int * allouetableau(int n)
{
    int * tab = malloc(n*sizeof(int));
    return tab;
}
void allouetableau2(int **T, int n)
{
    *T = malloc(n*sizeof(int));
}

void remplir_tableau(int *T, int n,int V)
{
    int i;
    for(i=0;i<n;i++)
        T[i]= rand()%V;
}


void afficher_tableau(int *T, int n)
{
    int i;
    for(i=0;i<n;i++)
        printf("%d ",T[i]);
}


int ** allouematrice(int n)
{
    int ** tab = malloc(n*sizeof(int*));
    int i;
    for(i=0;i<n;i++)
        tab[i]=malloc(n*sizeof(int));
    return tab;
}

void remplir_matrice(int **T, int n,int V)
{
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            T[i][j]= rand()%V;
}


void afficher_matrice(int **T, int n)
{
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            printf("%d ",T[i][j]);
}


int somme_tableau(int *T,int n)
{
    int s=0,i,j;

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){

            s+=(T[i]-T[j])*(T[i]-T[j]);
        }
            
    }

    return s;
}


int somme_tableau2(int *T,int n)
{
    int s=0,s1=0,i;

    for(i=0;i<n;i++){

      s+=(T[i])*(T[i]);
      s1+=T[i];
    }

    return 2*n*s-2*s1*s1;
}
/*
int verif(int ** T,int n){
    int i,j,k,l;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            for(k=0;k<n;k++)
                for(l=0;l<n;l++)
                    if(T[i][j]==T[k][l])
                        return 1;


    return 0;

    
}


int verif2(int ** T,int n,int V){

    int * TAB = calloc(sizeof(int)*V);

    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            TAB[T[i][j]]++;
            if(TAB[T[i][j]]>1)
                return 1;
        }
    }
    return 0;
        

    
}*/
int main()
{   
    int tt[]={50,100,150,200,250,300,350,400,450,500};

    clock_t  temps_initial;
    clock_t  temps_final;
    double temps_cpu1,temps_cpu2;
    int i,j;
    FILE *f=fopen("01_sortie_vitesse.txt","w");
    FILE *f1=fopen("02_sortie_vitesse.txt","w");
    for(i=0;i<10;i++){

        int *tab = allouetableau(tt[i]);
        remplir_tableau(tab,tt[i],50);
        temps_initial = clock();
        int s1 = somme_tableau(tab,tt[i]);

        temps_final = clock();

        temps_cpu1 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;


        temps_initial = clock();
            for(j=0;j<100;j++){
                remplir_tableau(tab,tt[i],50);
                int s2 = somme_tableau2(tab,tt[i]);
        }

        temps_final = clock();

        temps_cpu2 =   (( double)(temps_final  - temps_initial))/CLOCKS_PER_SEC;
        fprintf(f,"%d %f\n",tt[i],temps_cpu1);
        fprintf(f1,"%d %f\n",tt[i],temps_cpu2);
        printf("\n%d %f %f\n",tt[i],temps_cpu1,temps_cpu2);
    }
    fclose(f);
    fclose(f1);

    return 0;
}