#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "netlistG.h"
#include "entree_sortie.h"
#include <time.h>

int main(int argc, char ** argv){



    netlist * nL= init_netlist_fic (argv[1]); // fichier a lire
    Sauvegarde_intersection(argv[2],nL,2); // sauvgarder inter
    Graphe * G = cree_graphe(nL,argv[2]);
    Dessiner_Graphe(nL,"./Jeu_essai/a",G);
    Dessiner_Graphe_Via(nL,"./Jeu_essai/b",G,getVia(G));
    //afficher_graphe(*G);
    liberer_netlist(nL);
    liberer_graphe(G);

    return 0;
}
