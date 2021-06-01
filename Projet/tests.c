#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "netlistG.h"
#include "entree_sortie.h"
#include <time.h>

int main(int argc, char ** argv){

    // pour tester la validité de nos fonctions on propose les deux fichier simple test.net et testG.net 

    /*************** PARTIE A ******************/

    netlist * nL=NULL;

    nL= init_netlist_fic("./Jeu_essai/test.net"); // tester la fonction d initialisation de netlist
    ecrire_netlist_fic("./Jeu_essai/test_ecrire.net",nL); // // tester la fonction d'ecriture de netlist

    /* pour verifier le bon fonctionnement de ces deux fonctions on compare le contenus des deux fichiers */
    int x = system("cmp -s ./Jeu_essai/test.net ./Jeu_essai/test_ecrire.net"); // utiliser la commande bash pour comparer le contenu de deux fichiers
    if(!x) printf("\nles deux fichiers sont identiques\n\n"); // si le resultat est 0 ( valeur de retour vrai en bash est 0)
    else   printf("\nNon identiques\n\n");

    //test dessinage du netlist 
    Dessiner_Netlist(nL,"./Jeu_essai/rep_netlist");	//out : rep_netlist.svg



    liberer_netlist(nL);
    nL= init_netlist_fic("./Jeu_essai/test.net");
    //mise a jour des intersections methode naif
        Sauvegarde_intersection("./Jeu_essai/test_naif.int",nL,0);	//out : test_naif.int


    liberer_netlist(nL);
    nL= init_netlist_fic("./Jeu_essai/test.net");
    //mise a jour des intersections methode balayage
        Sauvegarde_intersection("./Jeu_essai/test_bal.int",nL,1);	//out : test_bal.int


    liberer_netlist(nL);
    nL= init_netlist_fic("./Jeu_essai/test.net");
    //mise a jour des intersections methode AVL
        Sauvegarde_intersection("./Jeu_essai/test_AVL.int",nL,2);	//out : test_AVL.int


    /* pour verifier le bon fonctionnement de ces deux fonctions on compare le contenus des deux fichiers */
    x = system("cmp -s ./Jeu_essai/test_naif.int ./Jeu_essai/test_bal.int && cmp -s ./Jeu_essai/test_bal.int ./Jeu_essai/test_AVL.int"); // utiliser la commande bash pour comparer le contenu de deux fichiers
    if(!x) printf("les 3 fichiers sont identiques\n\n"); // si le resultat est 0 ( valeur de retour vrai en bash est 0)
    else   printf("Non identiques\n\n");


    liberer_netlist(nL); 



	/*************** PARTIE B ******************/

    Graphe * G = NULL;
    Graphe * G2 = NULL;
    netlist * nL2=NULL;

    nL = init_netlist_fic("./Jeu_essai/test.net"); // cree la netlist pour le graphe 
    nL2 = init_netlist_fic("./Jeu_essai/testG.net"); // cree la netlist pour le graphe 2

    Sauvegarde_intersection("./Jeu_essai/graphe.int",nL,2); // sauvegarder les intersections
    Sauvegarde_intersection("./Jeu_essai/graphe2.int",nL2,2); // sauvegarder les intersections

    // out : graphe.int
    G = cree_graphe(nL,"./Jeu_essai/graphe.int");  // cree le graphe a partir du netlist et les intersections
    G2 = cree_graphe(nL2,"./Jeu_essai/graphe2.int");  // cree le graphe a partir du netlist et les intersections

	/* dessiner le graphe */
    Dessiner_Graphe(nL,"./Jeu_essai/rep_graphe",G); // out : rep_graphe.svg
    Dessiner_Graphe(nL2,"./Jeu_essai/rep_graphe2",G2); // out : rep_graphe2.svg

	/* dessiner le graphe avec via methode 1*/
	printf("\n=========================\nmethode 1 getVia :\n\n");
    Dessiner_Graphe_Via(nL,"./Jeu_essai/rep_graphe_via1",G,getVia(G)); // out : rep_graphe_via1.svg
    Dessiner_Graphe_Via(nL2,"./Jeu_essai/rep_graphe2_via1",G2,getVia(G2)); // out : rep_graphe2_via1.svg

	/* dessiner le graphe avec via methode 2*/
	printf("\n=========================\nmethode 2 bicolore :\n\n");
    Dessiner_Graphe_Via(nL,"./Jeu_essai/rep_graphe_via2",G,bicolore(G)); // out : rep_graphe_via2.svg
    Dessiner_Graphe_Via(nL2,"./Jeu_essai/rep_graphe2_via2",G2,bicolore(G2)); // out : rep_graphe2_via2.svg

    liberer_netlist(nL);
    liberer_netlist(nL2);
    liberer_graphe(G);
    liberer_graphe(G2);
    return 0;
}
