#include "netlistG.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "SVGwriter.h"




/*  
    input : Sommet
    output : liste allouer avec des champs initialisé
*/
Liste * allouer_liste(Sommet * a)
{ 
    Liste * L = malloc(sizeof(Liste));
    L->a = a;
    L->suiv = NULL;
    return L;
}


/*  
    input : coordonnées, type , extrimite ou segement , et indice du sommet
    output : sommet allouer avec des champs initialisé
*/
Sommet * allouer_sommet(double x, double y,enum TypeSommet type,Extremite * E, Segment * S,int ind)
{ 
    Sommet * som = malloc(sizeof(Sommet));
    som->ind = ind;
    som->x = x;
    som->y = y;
    som->type = type;
    som->L_adj = NULL;
    som->E = E;
    som->S = S;
    som->nb = 0;
    som->status = 0; 
    return som;
}



/*  
    input : nombre de sommet de graphe
    output : graphe allouer avec des champs initialisé
*/
Graphe * allouer_graphe(int n)
{ 
    Graphe * g = malloc(sizeof(Graphe));
    g->n = n;
    g->tab = malloc(n*sizeof(Sommet *)); // on utilise calloc pour avoir des champs initialisé a NULL 
	int i;
	for(i=0;i<n;i++)
		g->tab[i] = NULL;
    return g;
}


/*  
    input : liste 
    output : void
*/
void liberer_liste(Liste * l)
{
    if(l){
        liberer_liste(l->suiv); // appel recursif jusqu'a la fin de la liste 
        free(l); // on libere en retournant en arriere
    }
}



/*  
    input : Sommet 
    output : void
*/
void liberer_sommet(Sommet * s)
{
    if(s){
    liberer_extremiter(s->E); // on libere l extrimité ( la fonction verifie si null avant de supprimer)
    liberer_segment(s->S);  // on libere le segement ( la fonction verifie si null avant de supprimer)
    liberer_liste(s->L_adj); // libere la liste
    }
}


/*  
    input : Graphe 
    output : void
*/
void liberer_graphe(Graphe * g)
{
    int i;
    int tab_temp[g->n]; // tableau de marquage pour eviter la double liberation 
    for(i=0;i<g->n;i++){ 
        if(g->tab[i]->status == 0){// si le sommet n'est pas encore marquer 
		tab_temp[i] = 1;
		g->tab[i]->status = 1;
	}else
		tab_temp[i] = 0;
    }

    for(i=0;i<g->n;i++){ // pour tous les sommets
        if(g->tab[i] && tab_temp[i] == 1){
            liberer_sommet(g->tab[i]); //on libere le sommet si non null et premiere occurance du sommet 
		free(g->tab[i]);
	}
    }

    free(g->tab); // on libere le tableau

    free(g); // puis le graphe
}



/*  
    input : netlist et fichier d'intersections 
    output : Graphe
*/
Graphe * cree_graphe(netlist *n,char * nomFic)
{
    assert(n);
    Segment ** Tseg = get_Segements(n); // recuperer tous les segements
    int i, taille,nbSeg=0,nbPoints=0;

    int tab_res[n->NbRes+1]; // tableau pour stocker dans i la somme des nombre de segments des reseaux 0,1,...i-1
    int tab_res_pt[n->NbRes+1]; // tableau pour stocker dans i la somme nombre de points des reseaux 0,1,...i-1
    Point * p1,*p2;
    
    for(i=0;i<n->NbRes;i++){ // calculer le nombre totale des segments et points et remplir tab_res et tab_res_pt
        tab_res[i]=nbSeg;
        tab_res_pt[i]=nbPoints;
        nbSeg+=n->T_Res[i]->NbSeg;
	nbPoints+=n->T_Res[i]->NbPt;
    }


    tab_res[i]=nbSeg;//la dernier case
    tab_res_pt[i]=nbPoints;//la dernier case 
    taille=nbSeg;


    int tab_pt[nbPoints]; // tableau de marquague des point pour eviter la redondonce des extrimité
	for(i=0;i<nbPoints;i++)
		tab_pt[i]=0;

    Graphe * G = allouer_graphe(3*taille); // allouer le graphe avec un nombre de sommet 3 fois le nombre des segements

    int j=0,k;
    Liste * l = NULL;
    for(i=0;i<taille;i++){
			// parcourire le tableau des segements et recuperer les deux extrimités de chaqu'un
            p1 = n->T_Res[Tseg[i]->NumRes]->T_Pt[Tseg[i]->p1]; 
            p2 = n->T_Res[Tseg[i]->NumRes]->T_Pt[Tseg[i]->p2];
            
			//on ajoute un sommet segement
            G->tab[j]=allouer_sommet((p1->x+p2->x)/2,(p1->y+p2->y)/2,S,NULL,allouer_segment(Tseg[i]->NumRes,p1->NumPt,p2->NumPt,(p1->x==p2->x)),j);


	/* consernant les deux extrimité on a deux cas :
		soit c'est la premiere occurance de lexrtimité : dans ce cas on l alloue et l'ajoute en sauvgardant l'indice dans le tableau du marque
		et a laide du tableau qui contient le nombre des points des reseau avant la case courante on peut retrouver la bonne position

		dans le deuxieme cas on doit pas re allouer la meme extrimité mais faire pointé la case du tableau vers la premiere occurance
		en utilisant le tableau de marquage pour recuperer son indice et le tab_res_pt pour trouver la bonne position
	*/

	if(tab_pt[tab_res_pt[p1->NumRes]+p1->NumPt] == 0){// si c'est la permiere occurance de l'extrimité on l alloue et l'ajoute
            G->tab[j+1] = allouer_sommet(p1->x,p1->y,E,allouer_extremite(p1->y,1,Tseg[i],p1->NumPt),NULL,j+1); // sommet d'extrimité 1
	    tab_pt[tab_res_pt[p1->NumRes]+p1->NumPt]=j+1;// et on marque la case par l indice du premiere occurance
	}else
	    G->tab[j+1] =  G->tab[tab_pt[tab_res_pt[p1->NumRes]+p1->NumPt]]; // sinon la case point vers la premiere occurance de l'exrimité


	if(tab_pt[tab_res_pt[p2->NumRes]+p2->NumPt] == 0){// si c'est la permiere occurance de l'extrimité on l alloue et l'ajoute
            G->tab[j+2] = allouer_sommet(p2->x,p2->y,E,allouer_extremite(p2->y,1,Tseg[i],p2->NumPt),NULL,j+2); // sommet d'extrimité 2
	    tab_pt[tab_res_pt[p2->NumRes]+p2->NumPt]=j+2;// et on marque la case par l indice du premiere occurance
	}else
	    G->tab[j+2] =  G->tab[tab_pt[tab_res_pt[p2->NumRes]+p2->NumPt]];// sinon la case point vers la premiere occurance de l'exrimité


			// on ajoute le sommet segement dans la liste d'adjacense du sommet extrimite
                    l = allouer_liste(G->tab[j+1]);
                    l->suiv = G->tab[j]->L_adj;
                    G->tab[j]->L_adj = l;
                    G->tab[j]->nb++;

			// on ajoute le sommet extrimite dans la liste d'adjacense du sommet segement
                    l = allouer_liste(G->tab[j]);
                    l->suiv = G->tab[j+1]->L_adj;
                    G->tab[j+1]->L_adj = l;
                    G->tab[j+1]->nb++;


			// meme traitement pour le deuxieme sommet extrimite du segement 
                    l = allouer_liste(G->tab[j+2]);
                    l->suiv = G->tab[j]->L_adj;
                    G->tab[j]->L_adj = l;
                    G->tab[j]->nb++;

                    l = allouer_liste(G->tab[j]);
                    l->suiv = G->tab[j+2]->L_adj;
                    G->tab[j+2]->L_adj = l;
                    G->tab[j+2]->nb++;
                   
            j+=3; // avancer par 3 case ( segement + 2 extrimite )
    }

	
    FILE * f = fopen(nomFic,"r"); // on ouvre de fichier des intersections
    int r1,r2,p11,p12,p21,p22;
    Sommet * s1 =NULL,*s2=NULL;

    if(!f){
        printf("Erreur ouverture fichier\n");
        return NULL;
    }else{
        
        while(fscanf(f,"%d %d %d %d %d %d \n",&r1 ,&p11 ,&p12, &r2, &p21, &p22)==6){ // tantque pas fin de fichier 
		
			//pour chaque intersection on parcours le tableau des sommets dans l intervalle du reseau correspondant
			// a l aide du tableau tab_res qui nous aide a diminuer la complixité 
			//les segement sont repertie par reseau ( voir fonction get_segements)
			
            for(k=tab_res[r1]*3;k<tab_res[r1+1]*3;k+=3){
                if((G->tab[k]->S->p1 == p11 && G->tab[k]->S->p2 == p12 )||(G->tab[k]->S->p1 == p12 && G->tab[k]->S->p2 == p11 ))
                    s1 = G->tab[k];
                    
            }
            for(k=tab_res[r2]*3;k<tab_res[r2+1]*3;k+=3){
                if((G->tab[k]->S->p1 == p21 && G->tab[k]->S->p2 == p22 )||(G->tab[k]->S->p1 == p22 && G->tab[k]->S->p2 == p21 )) 
                    s2 = G->tab[k];
            }
			
            if(s1 && s2){ // si les deux sommets segement existe ( intersection )
				
				// on ajoute les sommets dans la liste d'adjacense de l'autre
                Liste * l = allouer_liste(s2);

                l->suiv = s1->L_adj;
                s1->L_adj = l;
                s1->nb++;

                l = allouer_liste(s1);

                l->suiv = s2->L_adj;
                s2->L_adj = l;
                s2->nb++;

            }

        }


    }
    fclose(f); // fermer le fichier
    return G;

}



/*  
    input : Graphe
    output : un tableau qui nous indique les vias 
*/
int * getVia(Graphe * g)
{
    int * tabS = malloc(g->n*sizeof(int)); // allouer le tableau des vias 
    int i;
    Liste * l = NULL;
    for(i=0;i<g->n;i++){
		//pour chaque sommet 
        if(g->tab[i]->type == S){ // si c'est un segement
            if(g->tab[i]->S->HouV == 0) // si horizontal ====> face 1 
                tabS[i]=1;
            if(g->tab[i]->S->HouV == 1) // si vertical ====> face 2
                tabS[i]=2;
        }else{
            tabS[i]=3; // sinon si une extrimité on initialise a 3 ( non vias )
			
            if(g->tab[i]->L_adj->a->S->HouV == 0){ // si le segement est horizontal
                l=g->tab[i]->L_adj;// on parcours la liste d'adjacense 
                while(l){
                    if(l->a->S->HouV == 1){ // si un segement vertical existe alors le point est un via 
                        tabS[i]=0;
                        break;
                    }
                    l=l->suiv;
                }
            }else{ // si le segement est vertical
                l=g->tab[i]->L_adj; // on parcours la liste d'adjacense 
                while(l){

                    if(l->a->S->HouV == 0){ // si un segement horizontal existe alors le point est un via 
                        tabS[i]=0;
                        break;
                    }
                    l=l->suiv;
                }
            }
        }
        
    }
    
    return tabS;
}



/*  
    input : Graphe, netlist et le nom du fichier html qu'on veut retourner 
    output : void 
*/
void Dessiner_Graphe(netlist * nL,char * nomFic,Graphe * G)
{


    int i;
    Liste * tempList= NULL;
    SVGwriter * svg = malloc(sizeof(SVGwriter)); // allouer le SVG
    double MaxCord = getMaxCord(nL); // recuperer la max coordonnee
    SVGinit(svg,nomFic,MaxCord,MaxCord); // initialiter le SVG
    

    for(i=0;i<G->n;i++){
		// on parcours la liste des sommets 
        tempList=G->tab[i]->L_adj; // on recuperer la liste des adjacense
        
        while(tempList){
            SVGlineRandColor(svg); // on choisit une couleur aleatoire et dessine une ligne 
            SVGline(svg,G->tab[i]->x,G->tab[i]->y,tempList->a->x,tempList->a->y);
            tempList=tempList->suiv;
        }
     }

    for(i=0;i<G->n;i++){ // on dessine les point a la fin pour garder un bon affichage du graphe
        SVGpoint(svg,G->tab[i]->x,G->tab[i]->y);
     }

    SVGfinalize(svg); //finaliser SVG 
}


/*  
    input : Graphe, netlist et le nom du fichier html qu'on veut retourner  et le tableau des vias 
    output : void 
*/
void Dessiner_Graphe_Via(netlist * nL,char * nomFic,Graphe * G,int * tabS)
{

    int i,nb=0;
    SVGwriter * svg = malloc(sizeof(SVGwriter)); // allouer le SV
    double MaxCord = getMaxCord(nL);  // recuperer la max coordonnee
    SVGinit(svg,nomFic,MaxCord,MaxCord);  // initialiter le SVG
 

    for(i=0;i<G->n;i++){
		// on parcours la liste des sommets 
        if(G->tab[i]->type == 1 && tabS[i]==1){ // si c'est un segement de face 1
            SVGlineColor(svg, "#33ffe6"); // on choisit une couleur 
            SVGline(svg,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p1]->x,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p1]->y,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p2]->x,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p2]->y);
        }else if(G->tab[i]->type == 1 && tabS[i]==2){ // si c'est un segement de face 2
            SVGlineColor(svg, "#e42d04");  // on choisit une autre couleur 
            SVGline(svg,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p1]->x,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p1]->y,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p2]->x,nL->T_Res[G->tab[i]->S->NumRes]->T_Pt[G->tab[i]->S->p2]->y);
        }
    }

       
    for(i=0;i<G->n;i++){ // on dessine les vias a la fin pour garder un bon affichage du graphe
        if(tabS[i]==0 && G->tab[i]->status == 0){ // si c'est une extrimité via on l affiche 
            nb++;
            SVGpoint(svg,G->tab[i]->x,G->tab[i]->y);
            G->tab[i]->status = 1;
        } 
    }
       
    for(i=0;i<G->n;i++){ // on dessine les vias a la fin pour garder un bon affichage du graphe
            G->tab[i]->status = 0;
         
    }

    SVGfinalize(svg);
	
    printf("nombre de vias est %d \n\n",nb);
}


/*  
    input : Graphe
    output : void 
	fonction d'affichage d'un graphe
*/
void afficher_graphe(Graphe g)
{
    int i;
    Liste * l= NULL;
    for (i=0;i<g.n;i++)
    {
        if (g.tab[i]->type == E){
            printf("Le point de coordonnee : x=%f  y=%f\n",g.tab[i]->x,g.tab[i]->y);
            l = g.tab[i]->L_adj;
            while (l)
            {
                printf("Est un point du segement qui relie les points p1=%d p2=%d il est HouV=%d\n",l->a->S->p1,l->a->S->p2,l->a->S->HouV);
                l=l->suiv;
            }
    }
}
}



/*  
    input : Graphe, indice du pere , tableau de marquage, et la liste des sommet du cycle impair passée par adresse qu'on veut retourner
    output : void 
	dans cette fonction on suit l'algorithme decrit dans l'enoncé en utilisant un parcours en profondeur 
*/
int detecte_cycle_impair_rec(Graphe *G,int r,int* M,Liste ** l)
{
Liste *cour;
Sommet *s;
int detect =0;
cour=G->tab[r]->L_adj;
while ((cour!=NULL)&& !detect){ 
    s=cour->a;
    if (M[s->ind]==-1){ 
        if(M[r]==1)
            M[s->ind]=2;
        if(M[r]==2)
            M[s->ind]=1;
        
        detect = detecte_cycle_impair_rec(G,s->ind,M,l); // appel recursifs sans ajout des sommet a la liste
	
    }else if (M[s->ind]==1){
        if(M[r]==1)
            detect=1;
    }else if (M[s->ind]==2){
        if(M[r]==2)
            detect=1;
    } 
    cour=cour->suiv;
    }
	if(detect){ // l'ajout se fait dans le cas de detection d'un cycle en retournant en arriere dans la pile d'execution
	Liste * tmp = allouer_liste(s);
        tmp->suiv = *l;
        *l =tmp;
	}
    return detect;
}



/*  
    input : Graphe, tableau des vias 
    output : Liste de segement du cycle impair 
*/
Liste * detecte_cycle_impair(Graphe *G,int * tabS)
{

int * M=(int*) malloc(G->n*sizeof(int)); // allouer le tableau de marquage 
int i,r=0;
int detect =0;
Liste * l = NULL;


for(i=0;i<G->n;i++){ 
    if(tabS[i]==0) // si on trouve un via on met le tableau de marquage a 0 sinon a -1
        M[i]=0;
    else    
        M[i]=-1;
}

while((r<G->n)&&(!detect)){
	// tantque on a pas trouver un cycle impair et l indicide est inferieure a la taille 
	M[r]=1; // mettre le pere a 1
    detect=detecte_cycle_impair_rec(G,r,M,&l); // appel a la fonction recursif 
    while((r<G->n)&&(M[r]!=-1)) // ignorer les cases qui n est pas a -1
        r++;
	
}
if(detect)
    return l; // si on a detecté un cycle on le retourne sinon null
else    
    return NULL;
}



/*  
    input : Graphe
    output : un tableau des vias  
*/
int * Ajout_vias_cycle_impair(Graphe *G){

int * tabS=(int*) malloc(G->n*sizeof(int)); // allouer le tableau 
int i;
for(i=0;i<G->n;i++) // initialiser le tableau a -1 
    tabS[i]=-1;
Liste * l = detecte_cycle_impair(G,tabS); // on recupere le cycle impair 

while(l){
	// on parcours cette liste 
    while(l && l->a->type != E) // recherche d'une extrimité
        l=l->suiv;
    if(!l) break; // si la liste est fini on sort 
    tabS[l->a->ind]=0; // on met le premier sommet a 0 ( via )
    l=detecte_cycle_impair(G,tabS); // et on cherche un nouveau cycle 
}

return tabS;
}



/*  
    input : Graphe
    output : un tableau des vias  
*/
int * Ajout_vias_cycle_impair_better(Graphe *G){

int * tabS=(int*) malloc(G->n*sizeof(int)); // allouer le tableau 
int i;
for(i=0;i<G->n;i++) // initialiser le tableau a -1 
    tabS[i]=-1;
Liste * l = detecte_cycle_impair(G,tabS); // on recupere le cycle impair 
int max=0;
Sommet * s_max = NULL;
while(l){
	// on parcours cette liste 
    while(l){ // recherche d'extrimité max
        if(l->a->type == E && max < l->a->nb){
            max = l->a->nb;
            s_max = l->a;
        }
        l=l->suiv;
    }
     
    if(max==0) break; // si la liste est fini on sort 
    
    

    tabS[s_max->ind]=0; // on met le premier sommet a 0 ( via )
    l=detecte_cycle_impair(G,tabS); // et on cherche un nouveau cycle 
}

return tabS;
}




/*  
    input : Graphe
    output : un tableau des vias  
*/
int * bicolore(Graphe * G){
int i;
int * tabS=Ajout_vias_cycle_impair(G); // recupere le tableau des vias 

Liste * tmp;
for(i=0;i<G->n;i++){
	//parcourire les sommets
    if(tabS[i]!=0){
		//si non via 
        tmp = G->tab[i]->L_adj;
        while(tmp){
			//parcourire la liste d'adjacense 
            switch (tabS[i]){ // appliquer l'algorithme decrit dans l'enoncé
                case -1 : 
                    if (tabS[tmp->a->ind]==1) tabS[i]=2;
                    else if (tabS[tmp->a->ind]==2) tabS[i]=1;
                    else if (tabS[tmp->a->ind]==-1) {tabS[i]=1;tabS[tmp->a->ind]=2;}
                    break;
                case 1 : if(tabS[tmp->a->ind]!=0) tabS[tmp->a->ind]=2; break;
                case 2 : if(tabS[tmp->a->ind]!=0) tabS[tmp->a->ind]=1; break;
            
            }
            tmp=tmp->suiv;
        }
        
    }
}
return tabS;
}
