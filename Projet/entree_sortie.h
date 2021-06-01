#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/* Cette librairie permet de lire facilement un fichier texte en le lisant "mot a mot" o\'u un mot est un ensemble de caracteres sans blanc ou retour chariot.
Si vous connaissez a l'avance la structure du fichier, vous pouvez lire ces mots directement dans un format approprié: si ce mot est un entier, utiliser GetEntier; si ce mot est une chaine, utiliser GetChaine; etc.

Remarque: si vous utilisez l'un derriere l'autre les commandes GetChaine, GetEntier ou GetReel, la commande lit le mot suivant (pas besoin d'utiliser Skip) */



/*LIT LE PROCHAIN CARACTERE NON VIDE */
char ReadChar(FILE *f);
 
/* SAUT DES ESPACES LORS DE LA LECTURE DANS UN FICHIER POINTE PAR f */
void Skip(FILE *f); 

/* LECTURE DES CARACTERES JUSQU'A LA FIN DU FICHIER OU DE LA LIGNE */
void SkipLine(FILE *f); 
 
/* LECTURE DE CHAINE DE CARACTERES JUSQU'A UN ESPACE, LA FIN DE LA LIGNE 
OU LA FIN DE FICHIER ; taille_max est la taille maximale de la chaine
de caractere contenu dans le fichier */
void GetChaine(FILE *f,int taille_max, char *ch);

/* LECTURE ET TRANSFORMATION DE CARACTERES EN ENTIERS */
int GetEntier(FILE *f);

/* LECTURE ET TRANSFORMATION DE CARACTERES EN ENTIERS REELS */
double GetReel(FILE *f);

