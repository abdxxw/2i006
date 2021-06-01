#include <stdio.h>
#include <stdlib.h>
#include "biblioHash.h"
#include <assert.h>
#include <string.h>

// how to use : ./mainHash <file_name> <number_of_books>
int main (int argc , char ** argv)
{
    cell_t * l = NULL;
    tableHachage_t * b;
    int x,ch;
    char * name,s1[50],s2[50];
    b=initTableHachage(tM);
    
    if(argc !=3){
        printf("Erreur Format\n");
        return 1;
    }
    
    name = strdup(argv[1]);
    x = atoi(argv[2]);

    assert(x<=100000);

    printf("Lecture : \n");
    Hlecture_n_entree(name ,x ,b);
    do{

        Hmenu();
        if(!scanf("%d",&ch)){
            break;
        }
        
        switch(ch){
            case 1: 
                Hafficher(*b); break;
            case 2: 
                printf("Donnez le numero de livre , titre du livre , nom d'auteur : \n");
                scanf("%d %s %s",&x,s1,s2);
                Hinserer_livre(b,intialise_cell(x,s1,s2)); 
                printf ("Le livre a ete inserer avec succes\n");
                break;
            case 3:
                printf ("Entrer le titre du livre a supprimer\n");
                scanf ("%s",s1);
                Hsupprimer_livre(b,Hrecherche_titre(*b,s1));
                break;
            case 4:
                printf ("Donner le titre du livre voulu\n");
                scanf("%s",s1);
                l = Hrecherche_titre(*b,s1);
                if(l)
                    printf("Livre num :%d %s par %s\n",l->num,l->titre,l->auteur);
                else 
                    printf("livre n'existe pas\n");
                break;
            case 5:
                printf ("Donner l'auteur des livres voulus\n");
                scanf("%s",s1);
                l = Hrecherche_auteur(*b,s1);
                Hafficher_livres(l);
                liberer_cell(l);
                break;
            case 6:
                printf ("Donner le numero du livre voulu\n");
                scanf("%d",&x);
                l = Hrecherche_num(*b,x);
                if(l)
                    printf("Livre num :%d %s par %s\n",l->num,l->titre,l->auteur);
                else 
                    printf("livre n'existe pas\n");
                break;
            case 7:
                Hafficher_livres(Hrecherche_double(*b));
                break;
            default:
                Hliberer_biblio(b); 
                ch = 0; 
                break;
        }
        
    }while(ch!=0);

    return 0;
}