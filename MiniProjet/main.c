#include <stdio.h>
#include <stdlib.h>
#include "entree_sortie.h"
#include "biblio.h"
#include <string.h>
#include <assert.h>


// how to use : ./main <file_name> <number_of_books>
int main (int argc , char ** argv)
{
    s_livre * l = NULL;
    Biblio * b;
    int x,ch;
    char * name,s1[50],s2[50];
    b=intialise_biblio();
    
    if(argc !=3){
        printf("Erreur Format\n");
        return 1;
    }
    
    name = strdup(argv[1]);
    x = atoi(argv[2]);

    assert(x<=100000);

    printf("Lecture : \n");
    lecture_n_entree(name ,x ,b);
    do{

        menu();
        if(!scanf("%d",&ch)){
            break;
        }
        
        switch(ch){
            case 1: 
                afficher(*b); break;
            case 2: 
                printf("Donnez le numero de livre , titre du livre , nom d'auteur : \n");
                scanf("%d %s %s",&x,s1,s2);
                inserer_livre(b,intialise_livre(x,s1,s2)); 
                printf ("Le livre a ete inserer avec succes\n");
                break;
            case 3:
                printf ("Entrer le titre du livre a supprimer\n");
                scanf ("%s",s1);
                supprimer_livre(b,recherche_titre(*b,s1));
                break;
            case 4:
                printf ("Donner le titre du livre voulu\n");
                scanf("%s",s1);
                l = recherche_titre(*b,s1);
                if(l)
                    printf("Livre num :%d %s par %s\n",l->num,l->titre,l->auteur);
                else 
                    printf("livre n'existe pas\n");
                break;
            case 5:
                printf ("Donner l'auteur des livres voulus\n");
                scanf("%s",s1);
                l = recherche_auteur(*b,s1);
                afficher_livres(l);
                liberer_livre(l);
                break;
            case 6:
                printf ("Donner le numero du livre voulu\n");
                scanf("%d",&x);
                l = recherche_num(*b,x);
                if(l)
                    printf("Livre num :%d %s par %s\n",l->num,l->titre,l->auteur);
                else 
                    printf("livre n'existe pas\n");
                break;
            case 7:
                afficher_livres(recherche_double(*b));
                break;
            default:
                liberer_biblio(b); 
                ch = 0; 
                break;
        }
        
    }while(ch!=0);

    return 0;
}