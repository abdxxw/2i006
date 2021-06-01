#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "netlist.h"
#include "entree_sortie.h"
#include "SVGwriter.h"

/*  
    input : nombre de reseaux
    output : netlist allouer avec des champs initialisé
            le tableau T_Res est allouer avec calloc ( tous les champs a NULL)
*/
netlist * allouer_netlist(int nombreRes)
{ 
    netlist * nL = malloc(sizeof(netlist));
    nL->NbRes = nombreRes;
    nL->T_Res = calloc(nombreRes,nombreRes*sizeof(Reseau *));
    return nL;
}

/*  
    input : Numero de reseau, nombre de point du reseau, nombre de segement du reseau 
    output : reseau allouer avec des champs initialisé
            le tableau T_Pt est allouer avec calloc ( tous les champs a NULL)
*/
Reseau * allouer_reseau(int indiceRes,int nombrePts,int nombreSeg)
{
    Reseau * res = malloc(sizeof(Reseau));
    res->NumRes = indiceRes;
    res->NbPt = nombrePts;
    res->NbSeg = nombreSeg;
    res->T_Pt = calloc(nombrePts,nombrePts*sizeof(Point *));
    return res;
}


/*  
    input : Numero de point, Numero de reseau du point, coordonnees de point
    output : point allouer avec des champs initialisé
            la list des segements incidents est vide pour le moment
*/
Point * allouer_point(int indicePt,int indiceRes, int x,int y)
{
    Point * Pt = malloc(sizeof(Point));
    Pt->x = x;
    Pt->y = y;
    Pt->NumRes = indiceRes;
    Pt->NumPt= indicePt;
    Pt->Lincid = NULL;
    return Pt;
}

/*  
    input :  Numero de reseau du segement, les numeros de point de ses 2 extrimité et hv (horizontal ou vertical)
    output : point allouer avec des champs initialisé
            la list des segements en intersection est vide pour le moment
*/

Segment * allouer_segment(int numRes, int p1,int p2,int hv)
{

    Segment * Sg = malloc(sizeof(Segment));
    Sg->NumRes = numRes;
    Sg->p1 = p1;
    Sg->p2 = p2;
    Sg->HouV = hv;
    Sg->Lintersec = NULL;
    Sg->added = 0;
    return Sg;
}

/*  
    input :  void
    output : Cellule allouer avec segement NULL et suivant NULL
*/

Cell_segment * allouer_cellule()
{
    Cell_segment * cell = malloc(sizeof(Cell_segment));
    cell->seg = NULL;
    cell->suiv = NULL;
    return cell;
}

/*  
    input :  l'ordonne, type, seggement et numero de point de l'extrimité
    output : extrimité allouer et initialisé 
*/
Extremite * allouer_extremite(int x, int VGD,Segment * seg,int nPT)
{
    Extremite * ext = malloc(sizeof(Extremite));
    ext->x = x;
    ext->VouGouD = VGD;
    ext->PtrSeg = seg;
    ext->NumPt = nPT;
    return ext;
}
/*  
    input :  extrimité
    output : Echeancier allouer et initialisé  avec deux fils NULL
*/
Echeancier * allouer_echeancier(Extremite * x)
{
    Echeancier * E = malloc(sizeof(Echeancier));
    E->ext = x;
    E->g = E->d = NULL;
    return E;
}

/*  
    input :  Ordonné et segement du point
    output : AVL allouer et initialisé  avec deux fils NULL
*/
AVL * allouer_AVL(int x, Segment * s)
{
    AVL * a = malloc(sizeof(AVL));
    a->x = x;
    a->PtrSeg = s;
    a->fg = a->fd = NULL;
    return a;
}


/*  
    input :  Cellule segement
    output : void
	liberer liste de cellules 
*/
void liberer_cell(Cell_segment * cell)
{
    if(cell){
        liberer_cell(cell->suiv); // appels recursifs jusqu'a la fin de la liste
        free(cell);		// liberer la cellule sans toucher au segement

    }
}

/*  
    input :  Segement
    output : void
	liberer un segement
*/
void liberer_segment(Segment * Seg)
{
    if(Seg){
        liberer_cell(Seg->Lintersec); // liberer la liste des cellule de segement en intersection
        free(Seg); // liberer le segment
    }
}


/*  
    input :  Point
    output : void
	liberer un point
*/
void liberer_point(Point * Pt)
{
    if(Pt){
        liberer_cell(Pt->Lincid); // liberer la liste des cellule de segement incidents
        free(Pt); // liberer le point
    }
}


/*  
    input :  Reseau
    output : void
	liberer un reseau
*/
void liberer_reseau(Reseau * r)
{
    int j;
    if(r){

        for(j=0;j<r->NbPt;j++){
            liberer_point(r->T_Pt[j]); // parcourire le tableau et liberer chaque point
        }
        free(r->T_Pt); // liberer le tableau
        free(r);	// liberer le reseau
    }
}


/*  
    input :  Netlist
    output : void
	liberer une netlist
*/
void liberer_netlist(netlist * N)
{ 
    int i;
    Cell_segment * tempListeSeg = NULL,*temp = NULL;
    if(N){
        for(i=0;i<N->NbRes;i++){
            temp = tempListeSeg = liste_seg_res(N->T_Res[i]); // recuperer la liste de tous les segements de chaque reseau
			
            while(tempListeSeg){
                liberer_segment(tempListeSeg->seg); // liberer chaque segement
                tempListeSeg = tempListeSeg->suiv;
            }
			
            liberer_cell(temp); // liberer la liste des cellule cree precedemment
            liberer_reseau(N->T_Res[i]); // liberer le reseau
        }
		
        free(N->T_Res); // liberer le tableau de reseau
        free(N); // liberer la netlist
    }
}


/*  
    input :  extremite
    output : void
	liberer une extremite
*/
void liberer_extremiter(Extremite * ext)
{
    if(ext){
        free(ext);
    }
}


/*  
    input :  echeancier
    output : void
	liberer l'arbre
*/
void liberer_echeancier(Echeancier * E)
{
    if(E){
        liberer_echeancier(E->g); // appel recursif sur le fils gauche
        liberer_echeancier(E->d); // appel recursif sur le fils droit
        liberer_extremiter(E->ext); // liberer l'extrimité
		free(E); // liberer le noeud
    }
}

/*  
    input :  AVL
    output : void
	liberer l'arbre
*/
void liberer_AVL(AVL * a)
{
    if(a){
        liberer_AVL(a->fg); // appel recursif sur le fils gauche
        liberer_AVL(a->fd); // appel recursif sur le fils droit
        free(a); // liberer le noeud
    }
}



/*  
    input :  nom de fichier
    output : netlist initialisé
*/
netlist * init_netlist_fic (char * nomFic)
{
    assert(nomFic);
    FILE *f=fopen(nomFic,"r"); // ouvrir le fichier en mode lecture 
	
    if(!f){

        printf("Erreur ouverture de fichier\n");
        return NULL;
    }
	
    netlist * nL=allouer_netlist(GetEntier(f)); // recuperer le nombre de reseau du fichier
    Reseau * r = NULL;
    Point * p = NULL;
    Segment * s = NULL;
    Cell_segment * tempCell = NULL;
    int i,j,p1,p2,x,y,z;
    double d1,d2;

    for(i=0;i<nL->NbRes;i++){
        //lire le contenu de chaque reseau 
        x=GetEntier(f);
        y=GetEntier(f);
        z=GetEntier(f);

        r=allouer_reseau(x,y,z); // allouer le reseau 
        nL->T_Res[r->NumRes] = r;
		
        for(j=0;j<r->NbPt;j++){
			//lire le contenu de chaque point
            x=GetEntier(f);
            d1=GetReel(f);
            d2=GetReel(f);
            p = allouer_point(x,r->NumRes,d1,d2); // allouer point
            r->T_Pt[j]= p; 
        }
		
        for(j=0;j<r->NbSeg;j++){
			// relier les point avec les segements 
            p1 = GetEntier(f);
            p2 = GetEntier(f);
            s = allouer_segment(r->NumRes,p1,p2,(r->T_Pt[p1]->x==r->T_Pt[p2]->x)); // allouer segement 
			
			// alouer cellule pour le premier point du segement et l ajouter
            tempCell = allouer_cellule();
            tempCell->seg = s;
            tempCell->suiv = r->T_Pt[p1]->Lincid;
            r->T_Pt[p1]->Lincid = tempCell;
			
			// alouer cellule pour le deuxieme point du segement et l ajouter
            tempCell = allouer_cellule();
            tempCell->seg = s;
            tempCell->suiv = r->T_Pt[p2]->Lincid;
            r->T_Pt[p2]->Lincid = tempCell;

        }
    }
	
    fclose(f); // fermer le fichier
    return nL;

}



/*  
    input :  reseau
    output : liste de segement de tout le reseau 
*/
Cell_segment * liste_seg_res(Reseau * r)
{
    Cell_segment * tempListeSeg = NULL,* tempCell= NULL,*temp = NULL;
    int i;
	
    temp = r->T_Pt[0]->Lincid; // recuperer la liste des segements incidents en premier point 
	
    while(temp){
		// faire une copie de la liste pour faciliter la liberations a la fin d 'execution du programme
        tempCell = allouer_cellule();
        tempCell->seg=temp->seg;
		temp->seg->added = 1; // mettre added a 1 pour eviter les repetions des segements 
        tempCell->suiv = tempListeSeg;
        tempListeSeg=tempCell;
        temp=temp->suiv;
    }
	
    for(i=1;i<r->NbPt;i++){
		//parcourire tous les points et ajouter tous le segement incidents.
        temp = r->T_Pt[i]->Lincid;
		
        while(temp){
           if (!temp->seg->added){ // verifier si added == 0 avant de l'ajouter 
                tempCell = allouer_cellule();
                tempCell->seg=temp->seg;
				temp->seg->added = 1;
                tempCell->suiv = tempListeSeg;
                tempListeSeg=tempCell;
            }
            temp=temp->suiv;
        }
    }
	
	//remettre le tous a 0 pour eviter des problemes lors d'utilisation multiple de la fonction
	temp=tempListeSeg;
	while(temp){
		temp->seg->added=0;
		temp=temp->suiv;	
	}
    return tempListeSeg;

}




/*  
    input :  nom de fichier et netlist
    output : void
	sauvegarder une netlist dans un fichier.
*/
void ecrire_netlist_fic (char * nomFic,netlist * nL)
{
    Cell_segment * tempListeSeg= NULL,*temp =NULL;
    assert(nomFic);
	
    FILE *f=fopen(nomFic,"w"); // ouverture du fichier en mode ecriture 
    if(!f){
        printf("Erreur ouverture de fichier\n");
        return;
    }
    int i,j;
    fprintf(f,"%d\n",nL->NbRes); // ecrire le nombre du reseau 

    for(i=0;i<nL->NbRes;i++){
		//pour chaque reseau on ecrit ses informations
        fprintf(f,"%d %d %d\n",nL->T_Res[i]->NumRes,nL->T_Res[i]->NbPt,nL->T_Res[i]->NbSeg);

        for(j=0;j<nL->T_Res[i]->NbPt;j++){
			//pour chaque point on ecrit ses informations
            fprintf(f,"  %d %0.f %0.f\n",j,nL->T_Res[i]->T_Pt[j]->x,nL->T_Res[i]->T_Pt[j]->y);

            
        }
		
        temp = tempListeSeg = liste_seg_res(nL->T_Res[i]); // recuperer la liste des segement du reseau courant.
		
        while(tempListeSeg){ 
			// ecrire les numero des point extrimité de chaque segement.
            fprintf(f,"  %d %d\n",tempListeSeg->seg->p1,tempListeSeg->seg->p2);
            tempListeSeg=tempListeSeg->suiv;
        }
       liberer_cell(temp); // liberer la liste cree precedemment
    }

	
    fclose(f); // fermer le fichier
}




/*  
    input :  netlist et deux segemnt
    output : 1 si intersection 0 sinon
*/
int intersection(netlist *N, Segment *s1, Segment *s2)
{

    Segment * H,*V;
    if(s1->HouV == s2->HouV) // pas d'intersection si les deux segemnt sont les deux horizontaux en mm temps ou verticaux en mm temps
        return 0;
    else{
		
		// V pour vertical et H pour horizontal
        if(s1->HouV == 1){
            V=s1; H=s2;
        }
        else{
            V=s2; H=s1;
        }

		
	// on recupere l'abscisse du vertical et les deux ordonnés  
    double xV = N->T_Res[V->NumRes]->T_Pt[V->p1]->x;
    double yV1 = N->T_Res[V->NumRes]->T_Pt[V->p1]->y;
    double yV2 = N->T_Res[V->NumRes]->T_Pt[V->p2]->y;
	
	// on recupere l'ordonné d'horizontal et les deux abscisses
    double xH1 = N->T_Res[H->NumRes]->T_Pt[H->p1]->x;
    double xH2 = N->T_Res[H->NumRes]->T_Pt[H->p2]->x;
    double yH = N->T_Res[H->NumRes]->T_Pt[H->p1]->y;

        return((yH < yV1 && yH >yV2)||(yH < yV2 && yH >yV1)) && ((xV < xH1 && xV >xH2)||(xV < xH2 && xV >xH1));
		// vrai si l'ordonne d'horizontal est entre les deux ordonne du vertical et l'abscisse du vertical est entre les deux abscisses d'horizontal
    }

}


/*  
    input :  netlist 
    output : nombre de segement de la netlist
*/
int nb_segment(netlist *N)
{
    int i,nbSeg=0;
    for(i=0;i<N->NbRes;i++){
        nbSeg+=N->T_Res[i]->NbSeg;
    }
    return nbSeg;
}




/*  
    input :  netlist 
    output : un tableau de tous les segements
*/
Segment ** get_Segements(netlist *N)
{

    int i,j=0,taille = nb_segment(N); // recuperer le nombre de segements
    Segment ** out = malloc(taille*sizeof(Segment *)); // allouer le tableau
    Cell_segment * tempSeg =NULL,*temp=NULL;
	
    for(i=0;i<N->NbRes;i++){
        tempSeg = liste_seg_res(N->T_Res[i]); // pour chaque reseau on recupere la liste des segeemnt
        while(tempSeg){ // on ajoute chaque segement au tableau
            out[j]= tempSeg->seg;
            j++;
            tempSeg=tempSeg->suiv;
        }
        liberer_cell(temp); // liberer la liste cree avant
    }

    return out;

}




/*  
    input :  netlist 
    output : void
	mise a jour la liste des intersection 
*/
void intersect_naif (netlist * N)
{

    Segment ** All_Seg =  get_Segements(N); // recuperer tout les segement
    int i,j,taille = nb_segment(N);
    Cell_segment * tempCell=NULL;

    for(i=0;i<taille-1;i++){
        for(j=i;j<taille;j++){
			// boucle imbriqué si intersection on l ajoute 
            if(intersection(N,All_Seg[i],All_Seg[j])){
                tempCell = allouer_cellule();
                tempCell->seg = All_Seg[j];
                tempCell->suiv = All_Seg[i]->Lintersec;
                All_Seg[i]->Lintersec = tempCell;

                tempCell = allouer_cellule();
                tempCell->seg = All_Seg[i];
                tempCell->suiv = All_Seg[j]->Lintersec;
                All_Seg[j]->Lintersec = tempCell;
            }
        }
    }
    free(All_Seg); // liberer le tableau ( sans toucher aux segements)
}



/*  
    input :  nom de fichier et netlist et la methode
    output : void
	sauvgarder les intersection avec 0 : naif , 1: balyage , 2: AVL
*/
void Sauvegarde_intersection(char * nomfic, netlist * N,int methode)
{
    FILE *f = fopen(nomfic,"w"); // ouverture du fichier en mode ecriture
    if(!f){
        printf("Erreur ouverture fichier\n");
        return;
    }

    Cell_segment * tempListeSeg1= NULL,* tempListeSeg2= NULL,* temp=NULL;
    Segment * Seg = NULL;
    int i;
	//mise a jour des intersection 
    switch(methode){

        case 0: intersect_naif(N); break;
        case 1: intersect_balayage(N); break;
        case 2: intersect_balayage_AVL(N); break;
        default: intersect_balayage_AVL(N);
    }
	
    for(i=0;i<N->NbRes;i++){
		// pour chaque reseau recuperer la liste des segements 
        temp = tempListeSeg1 = liste_seg_res(N->T_Res[i]);
        while(tempListeSeg1){
			
            Seg = tempListeSeg1->seg;
            tempListeSeg2 = Seg->Lintersec;
			
			//pour chaque segement seg on parcourt la liste des intersection
            while(tempListeSeg2){
                if((Seg->NumRes < tempListeSeg2->seg->NumRes)) // on definie l ordre pour eviter les redondances
                    fprintf(f,"%d %d %d %d %d %d \n",Seg->NumRes,Seg->p1,Seg->p2,
                    tempListeSeg2->seg->NumRes,tempListeSeg2->seg->p1,tempListeSeg2->seg->p2);
                tempListeSeg2=tempListeSeg2->suiv;
            }
            tempListeSeg1=tempListeSeg1->suiv;
        }
        liberer_cell(temp); // liberer la liste cree avant 
    }
    fclose(f); // fermer le fichier

}




/*  
    input :  netlist 
    output : cordonnée max
*/
double getMaxCord(netlist * N)
{
    int i,j,max=0;
    for(i=0;i<N->NbRes;i++){
        for(j=0;j<N->T_Res[i]->NbPt;j++){
            if(N->T_Res[i]->T_Pt[j]->x > max)
                max = N->T_Res[i]->T_Pt[j]->x;
            if(N->T_Res[i]->T_Pt[j]->y > max)
                max = N->T_Res[i]->T_Pt[j]->y;
        }
    }
    return max;
}



/*  
    input :  netlist ,echeancier et extrimité
    output : echeancier modifier
*/
Echeancier * inserer_extremite(netlist *n, Echeancier * E, Extremite * ext)
{
    if (E==NULL){ // cas de base 
        return allouer_echeancier(ext);
    }
    int x1,x2; 
    
	// x1 numero de point d'extrimité courante
    if (E->ext->VouGouD == 0){ 
        x1= E->ext->PtrSeg->p1;
    }else{
        x1=E->ext->NumPt;
    }
	
	// x2 numero de point d'extrimité qu'on veut ajouter
    if (ext->VouGouD == 0){
        x2= ext->PtrSeg->p1;
    }else{
        x2=ext->NumPt;
    }
    
	
    if(n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[x1]->x < n->T_Res[ext->PtrSeg->NumRes]->T_Pt[x2]->x) // si l'ordonnée du noeud courant est inferieure a l'ordonnée du point qu'on veut ajouté
        E->d = inserer_extremite(n,E->d,ext); // appel recursif sur fils droit
    else if(n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[x1]->x > n->T_Res[ext->PtrSeg->NumRes]->T_Pt[x2]->x)  // si l'ordonnée du noeud courant est superieure a l'ordonnée du point qu'on veut ajouté
        E->g = inserer_extremite(n,E->g,ext); // appel recursif sur fils gauche
    else{
		//dans le cas ou ils sont egaux on suit l'algorithme decrit dans l'enoncé : extrimité gauche < segement Vertical < extrimité droit
        if (E->ext->VouGouD == 1)
            E->d = inserer_extremite(n,E->d,ext);
        else if (E->ext->VouGouD == 2)
            E->g = inserer_extremite(n,E->g,ext);
        else if (ext->VouGouD == 1)
            E->g = inserer_extremite(n,E->g,ext);
        else 
            E->d = inserer_extremite(n,E->d,ext);
    }

    return E;
   


}



/*  
    input :  netlist 
    output : echeancier allouer et initialisé 
*/
Echeancier * cree_echeancier(netlist *n)
{
    assert(n);
    Segment ** Tseg = get_Segements(n); // recuperer tous les segement
    int i, taille = nb_segment(n);
    Echeancier * E = NULL;
    Point * p1,*p2;
    for(i=0;i<taille;i++){
			//pour chaque segement on recupere les deux points extrimité
            p1 = n->T_Res[Tseg[i]->NumRes]->T_Pt[Tseg[i]->p1];
            p2 = n->T_Res[Tseg[i]->NumRes]->T_Pt[Tseg[i]->p2];
			
        if(Tseg[i]->HouV == 0){
			//si le segement est horizontal on insere les deux extrimité
			
            if(p1->x < p2->x){ // verifie l'ordre ( gauche ou droite )
                E = inserer_extremite(n,E,allouer_extremite(p1->y,1,Tseg[i],p1->NumPt));
                E = inserer_extremite(n,E,allouer_extremite(p2->y,2,Tseg[i],p2->NumPt));
            }else{
                E = inserer_extremite(n,E,allouer_extremite(p1->y,2,Tseg[i],p1->NumPt));
                E = inserer_extremite(n,E,allouer_extremite(p2->y,1,Tseg[i],p2->NumPt));
            }
        }else{
			//sinon une des deux suffit
            E = inserer_extremite(n,E,allouer_extremite(p1->y,0,Tseg[i],-1));
        }
    }

    return E;

}


/*  
    input :  liste des cellule segement, une ordoneé et netlist 
    output : pointeur sur la cellule qui est avant celle qui a une ordonnée superieure a y ( l emplacement d'ajout )
*/
Cell_segment * indice_recherche (Cell_segment * lseg, double y,netlist *n )
{
    if(lseg){
     while (lseg->suiv)
     {
         if (n->T_Res[lseg->suiv->seg->NumRes]->T_Pt[lseg->suiv->seg->p1]->y >= y) return lseg;
         lseg=lseg->suiv;
     }
    }
     return lseg;
}



/*  
    input :  liste des cellule segement deja triée, segement et netlist 
    output : une liste de cellule segement modifier avec insertion de seg ( reste triée)
*/

Cell_segment * inserer_segement (Cell_segment * lseg, Segment *seg,netlist * n)
{
    Cell_segment * cell = allouer_cellule();
    cell->seg = seg;
    if(lseg == NULL){
        return cell;
    }
    
    if((n->T_Res[lseg->seg->NumRes]->T_Pt[lseg->seg->p1]->y >= n->T_Res[seg->NumRes]->T_Pt[seg->p1]->y )){ // cas d'insertion dans la tete
        cell->suiv = lseg;
        lseg = cell;
        return lseg;
    }

    Cell_segment * tmp = indice_recherche(lseg,n->T_Res[seg->NumRes]->T_Pt[seg->p1]->y,n); // sinon on recherche l'emplacement correcte et on l insere
    cell->suiv = tmp->suiv;
    tmp->suiv = cell;
    return lseg;
} 



/*  
    input :  liste des cellule segement triée, segement
    output : une liste de cellule segement modifiée avec suppression de seg 
*/
Cell_segment * supprimer_segement (Cell_segment * lseg, Segment *seg)
{
    assert(lseg);
    Cell_segment * x;
    if(lseg->seg == seg){ // le cas de tete de liste
        x=lseg;
        lseg = lseg->suiv;
        x->suiv = NULL;
        liberer_cell(x);
        return lseg;
    }
    Cell_segment * tmp = lseg;

    while(tmp->suiv){
		// on parcours la liste jusq'a l element avant seg 
        if(tmp->suiv->seg == seg){
			// supprimer seg 
            x = tmp->suiv;
            tmp->suiv=tmp->suiv->suiv;
            x->suiv = NULL;
            liberer_cell(x);
            break;
        }else
            tmp=tmp->suiv;
    }
    return lseg;
} 



/*  
    input :  liste des cellule segement triée, une ordonnée
    output : le premier segement avant l'ordonnée
*/
Segment * Prem_segment_apres(double y,Cell_segment * lseg,netlist * n)
{
	//on parcours la liste tantque l'ordonnée des segement sont inferieure a y et la liste n'est pas encore finie
    while((lseg) && (n->T_Res[lseg->seg->NumRes]->T_Pt[lseg->seg->p1]->y <= y)){
        lseg=lseg->suiv;
    }
	
    if(lseg) 
        return lseg->seg;
    else // si on sort de la boucle a cause de fin de liste on retourne NULL 
        return NULL;
}


/*  
    input :  liste des cellule segement triée, une segement
    output : le premier segement avant l'ordonnée du segement seg
*/
Segment * Au_Dessus(Segment * seg,Cell_segment * lseg,netlist * n)
{

    return Prem_segment_apres(n->T_Res[seg->NumRes]->T_Pt[seg->p1]->y,lseg,n); 

}



/*  
    input :  netlist, Echeancier, liste des segement
    output : void
    parcours infixe  Gauche Noeud Droite pour garder le tri et mise a jour des intersection
*/
void intersect_balayage_rec(netlist * n,Echeancier* E,Cell_segment ** T)
{ 

    if(E){

        intersect_balayage_rec(n,E->g,T); // appel fils gauche 

/*============== TRAITEMENT ========================*/
        if(E->ext->VouGouD == 1){
            *T = inserer_segement(*T,E->ext->PtrSeg,n); // si extremité gauche on l'insere 
        }
        else if(E->ext->VouGouD == 2){
            *T = supprimer_segement(*T,E->ext->PtrSeg); // si extremité droite on la supprime 
        }
        else{
            // si segement vertical
            double y1 = n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[E->ext->PtrSeg->p1]->y;
            double y2 = n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[E->ext->PtrSeg->p2]->y;
            if(y1 > y2) { // y1 est le min eutre les deux extremité du segement vertical 
                double tmp = y1;
                y1 = y2;
                y2 = tmp;
            }
                // execution de l agorithme de l'enoncé

            Segment * h = Prem_segment_apres(y1,*T,n); // recuperer le premier segement apres y1

            while((h)&&(n->T_Res[h->NumRes]->T_Pt[h->p1]->y < y2)){
                if (h->NumRes != E->ext->PtrSeg->NumRes){

                    // ajout des intersections 
                    Cell_segment * tempCell = allouer_cellule();
                    tempCell->seg = h;
                    tempCell->suiv = E->ext->PtrSeg->Lintersec;
                    E->ext->PtrSeg->Lintersec = tempCell;

                    tempCell = allouer_cellule();
                    tempCell->seg = E->ext->PtrSeg;
                    tempCell->suiv = h->Lintersec;
                    h->Lintersec = tempCell;
                }
                h = Au_Dessus(h,*T,n);
            }

        }
        intersect_balayage_rec(n,E->d,T); // appel fils droit 
    }
}


/*  
    input :  netlist
    output : void
    fonction pour appeller la fonction recursifs intersect_balayage_rec
*/

void intersect_balayage(netlist * n)
{

    Echeancier * E = cree_echeancier(n);
    Cell_segment * T = NULL;
    intersect_balayage_rec(n,E,&T);
    liberer_cell(T);

}



/*  
    input :  AVL
    output : hauteur du noeud
*/
int AVL_hauteur(AVL * a)
{
    if(!a)
        return -1;
    else
     return a->h;
    
}



/*  
    input :  AVL
    output : void
    mise a jour de toute les hauteurs 
*/
void majHauteur(AVL * a)
{
    if(a){
        majHauteur(a->fg);
        majHauteur(a->fd);
       a->h = (AVL_hauteur(a->fg) < AVL_hauteur(a->fd) ? AVL_hauteur(a->fd) : AVL_hauteur(a->fg))+1;
    }
    
}


/*  
    input :  AVL, x et segement a inserer 
    output : void
    insertion simple dans un arbre binaire de recharche 
*/

void inserer_AVL(AVL ** a,int x,Segment * Seg)
{
  
    if(!*a){
        *a=allouer_AVL(x,Seg);
        return;
    }
    if((*a)->x < x)
        inserer_AVL(&((*a)->fd),x,Seg);
    else 
        inserer_AVL(&((*a)->fg),x,Seg);
    

}


/*  
    input :  AVL, et pere pour le retourner par adresse 
    output : max d'un AVL 
*/

AVL * max_AVL(AVL *a,AVL ** pere)
{
    assert(a);
    if(!a->fd){ // si pas de fils droit alors la racine est le max et pere NULL
        *pere = NULL;
        return a;
    }
    else { // sinon on deplace jusqu'au fils le plus a droite et on le retourne 
        *pere=a;
        while(a->fd->fd){
            *pere=(*pere)->fd;
            a=a->fd;
        }
         return a->fd;
    }

}



/*  
    input :  AVL, et l abscisse du noeud a supprimé 
    output : void
    avec passage  d AVL par adresse pour le modifier 
*/

void supprimer_AVL (AVL **a,int x)
{
    assert(*a);
    if ((*a)->x==x){ // cas de base le noeud a supprimé 

        AVL *tmp=*a,*pere,*max;
        if ((!(*a)->fg) && (!(*a)->fd)) // si pas de fils 
            *a=NULL;
        else if (!(*a)->fg){ // si fils droit only on avanace et supprime
            *a = (*a)->fd;
            tmp->fd = NULL;
        }
        else if (!(*a)->fd){ // si fils gauche only on avanace et supprime
            *a = (*a)->fg;
            tmp->fg = NULL;
        }
        else{   // sinon on recherche le max de son fils gauche , on l echange avec le noeud lui meme on predre consideration de ses fils 
            max = max_AVL((*a)->fg,&pere);
            *a = max;
            if(pere){
            pere->fd = max->fg;
            max->fg = tmp->fg;
            }
            max->fd = tmp->fd;
            tmp->fg = tmp->fd = NULL;
           
        }
        liberer_AVL(tmp); // liberer le noeud 
        return;
    }

    if((*a)->x < x)
        supprimer_AVL(&((*a)->fd),x); // appel recursif sur fils droit 
    if((*a)->x > x)
        supprimer_AVL(&((*a)->fg),x); // appel recursif sur fils gauche  
}



/*  
    input :  AVL
    output : void
    avec passage  d AVL par adresse pour le modifier suivant la rotation vu en cours 
*/

void rotationD(AVL ** a){
    AVL * tmp = *a;
    *a = (*a)->fg;
    tmp->fg = (*a)->fd;
    (*a)->fd = tmp;
}



/*  
    input :  AVL
    output : void
    avec passage  d AVL par adresse pour le modifier suivant la rotation vu en cours 
*/

void rotationG(AVL ** a){
    AVL * tmp = *a;
    *a = (*a)->fd;
    tmp->fd = (*a)->fg;
    (*a)->fg = tmp;
}


/*  
    input :  AVL
    output : void
    avec passage  d AVL par adresse pour le modifier
*/

void equilibrer_AVL(AVL ** a){

    
    if (!(*a))
        return;
    int def;
    if((!(*a)->fg)&&(!(*a)->fd)) // pas de fils
        return;
    else if (!(*a)->fg) // si que un fils droit 
        if ((*a)->fd->h > 1) // et plus d'un fils droit rotation droite
            def=-2;
        else
            return;
    else if (!(*a)->fd) // si que un fils gauche
        if((*a)->fg->h > 1) // et plus d'un fils gauche rotation gauche
            def=2;
        else
            return;
    else // sinon dans le cas de deux fils existants
        def=(*a)->fg->h-(*a)->fd->h; // differance entre la hauteur des deux fils 

    if(def == 2){ // rotation droite
        if(((*a)->fg->fd)&&((!(*a)->fg->fg)||((*a)->fg->fg->h < (*a)->fg->fd->h))) // si le fils gauche et droit du fils gauche existent et l hauteur du gauche est inferieure de celle du droit
            rotationG(&((*a)->fg)); // on fait un rotation gauche du fils gauche 
        rotationD(a); 
    }
     if(def == -2){ // rotation gauche
        if(((*a)->fd->fg)&&((!(*a)->fd->fd)||((*a)->fd->fd->h < (*a)->fd->fg->h)))// si le fils gauche et droit du fils droit existent et l hauteur du droit est inferieure de celle du gauche
            rotationD(&((*a)->fd));// on fait un rotation droit du fils droit 
        rotationG(a);
    }

    majHauteur(*a); // on met a jour les hauteur
}


/*  
    input :  AVL, ordonnée et un segement
    output : void
    avec passage  de segement par adresse pour le retourner
*/

void Prem_segment_apres_AVL(double y,AVL * a,Segment ** s){ 

    if(a){
        if(a->x > y){ // si l ordonnée du noeud est superieure a y on met a jour le segement
            *s=a->PtrSeg;
            Prem_segment_apres_AVL(y,a->fg,s); // appel recursif sur le fils gauche 
        }else
            Prem_segment_apres_AVL(y,a->fd,s); // sinon appel recursif sur le fils droit 
       
    }

}



/*  
    input :  AVL, segement et netlist 
    output : Segement
*/

Segment * Au_Dessus_AVL(Segment * seg,AVL * a,netlist * n){

    Segment * s = NULL;
    Prem_segment_apres_AVL(n->T_Res[seg->NumRes]->T_Pt[seg->p1]->y,a,&s); // tout simplement appel a la fonction precedente 
    return s;
    

}



/*  
    input :  AVL, echeancier et netlist 
    output : void
    passage de AVL par adresse pour la modifier 
    parcours infixe  Gauche Noeud Droite pour garder le tri et mise a jour des intercetions 
*/

void intersect_balayage_AVL_rec(netlist * n,Echeancier* E,AVL ** a){ 

    if(E){
        
        intersect_balayage_AVL_rec(n,E->g,a); // appel fils gauche 

    /*============== TRAITEMENT ========================*/
        
        if(E->ext->VouGouD == 1){
            inserer_AVL(a,n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[E->ext->PtrSeg->p1]->y,E->ext->PtrSeg); // si extremité gauche on l'insere 
            equilibrer_AVL(a); // equilibrage apres insertion =============================== important pour complixité
  
        }
        else if(E->ext->VouGouD == 2){
            supprimer_AVL(a,n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[E->ext->PtrSeg->p1]->y); // si extremité droite on la supprime 
            equilibrer_AVL(a); // equilibrage apres suppression  =============================== important pour complixité
        }
        else{

            // si segement vertical
            double y1 = n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[E->ext->PtrSeg->p1]->y;
            double y2 = n->T_Res[E->ext->PtrSeg->NumRes]->T_Pt[E->ext->PtrSeg->p2]->y;

            if(y1 > y2) { // y1 est le min eutre les deux extremité du segement vertical 
                double tmp = y1;
                y1 = y2;
                y2 = tmp;
            }
            Segment * h = NULL;
            Prem_segment_apres_AVL(y1,*a,&h); 

            while((h)&&(n->T_Res[h->NumRes]->T_Pt[h->p1]->y < y2)){
                if (h->NumRes != E->ext->PtrSeg->NumRes){

                    Cell_segment * tempCell = allouer_cellule();
                    tempCell->seg = h;
                    tempCell->suiv = E->ext->PtrSeg->Lintersec;
                    E->ext->PtrSeg->Lintersec = tempCell;

                    tempCell = allouer_cellule();
                    tempCell->seg = E->ext->PtrSeg;
                    tempCell->suiv = h->Lintersec;
                    h->Lintersec = tempCell;
                }
                h = Au_Dessus_AVL(h,*a,n);
            }
    
        }
        intersect_balayage_AVL_rec(n,E->d,a); // appel fils droit 
    }
}



/*  
    input :  netlist
    output : void
    fonction pour appeller la fonction recursifs intersect_balayage_AVL_rec
*/

void intersect_balayage_AVL(netlist * n){

    Echeancier * E = cree_echeancier(n);
    AVL * a = NULL;
    intersect_balayage_AVL_rec(n,E,&a);
    liberer_AVL(a);

}



/*  
    input :  AVL
    output : void
    Affichage d'un AVL dans l ordre infixe 
*/
void afficher_AVL (AVL *a)
{
    if (a)
    {   
        afficher_AVL(a->fg);
        printf("%f\n",a->x);
        afficher_AVL(a->fd);
    }
}


/*  
    input :  Netlist et nom de fichier 
    output : void
    dessiner netlist en utilisant SVG dans le fichier nomFic.html
*/
void Dessiner_Netlist(netlist * nL,char * nomFic){


    int i,j;
    Cell_segment * tempListeSeg= NULL;
    SVGwriter * svg = malloc(sizeof(SVGwriter)); // allouer le SVG
    double MaxCord = getMaxCord(nL); // recuperer la max coordonnee
    SVGinit(svg,nomFic,MaxCord,MaxCord); // initialiter le SVG
    
    for(i=0;i<nL->NbRes;i++){
        //pour chaque reseau 
        tempListeSeg = liste_seg_res(nL->T_Res[i]); // on recupere la liste des segement 
        while(tempListeSeg){
            SVGlineRandColor(svg); // on choisit une couleur aleatoire et dessine une ligne 
            SVGline(svg,nL->T_Res[i]->T_Pt[tempListeSeg->seg->p1]->x,nL->T_Res[i]->T_Pt[tempListeSeg->seg->p1]->y,nL->T_Res[i]->T_Pt[tempListeSeg->seg->p2]->x,nL->T_Res[i]->T_Pt[tempListeSeg->seg->p2]->y);
            tempListeSeg=tempListeSeg->suiv;
        }
        for(j=0;j<nL->T_Res[i]->NbPt;j++){ // on dessine les point a la fin pour garder un bon affichage du netlist
          SVGpoint(svg,nL->T_Res[i]->T_Pt[j]->x,nL->T_Res[i]->T_Pt[j]->y);
        }
        
    }

    SVGfinalize(svg); //finaliser SVG 
}
