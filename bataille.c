//
// SAIK17119305
// INF3135

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include<time.h>
void init_nb_aleatoire();
void affichage_plateau();
void creer_plateauvide();
void proposition_joueur();
void initialisation_plateau();
void incrementer_ToucheNav();
void proposition_joueur();
void affichage_plateau();

typedef struct une_case {
    int x; /* position de la case en x*/
    int y; /* position de la case en y*/
} Case;

typedef struct navire {
    int sens; /* 0 haut 1 droite 2 bas 3 gauche */
    Case premiere_case;
    int taille;
} Navire;

/* Initialiser le générateur au début du main par l’instruction suivante*/
void init_nb_aleatoire() {
    srand(time(NULL));
}

/* Renvoie un nombre, tiré au hasard, compris entre 1 et max*/
int nb_aleatoire(int max) {
    return (rand() % max);
}

/**
 * Modelise un navire
 * @param taille Taille du navire
 * @param taille_plateau Taille de la grille du jeu
 * @return n, le navire
 */
Navire creer_navire(int taille, int taille_plateau) {
    Navire n;
    n.taille = taille;
    n.sens = nb_aleatoire(4);
    n.premiere_case.x = nb_aleatoire(taille_plateau);
    n.premiere_case.y = nb_aleatoire(taille_plateau);
    return n;
}
/**
 * Retourne 1 si le navire est valide dans la grille de plateau. Sinon, retourne 0.
 * @param plateau la grille du plateau
 * @param taille_plateau taille max du plateau
 * @param nav le navire
 * @return 1 si navire valide, sinon 0;
 */
int est_valide(int **plateau, int taille_plateau, struct navire *nav) {
    int x = nav->premiere_case.x;
    int y = nav->premiere_case.y;
    int estValide = 1;
    int i;
    for (i = 0; estValide == 1 && i < nav->taille; i++) {
        if (nav->sens == 1 || nav->sens == 3) {
            if ((x + i) >= taille_plateau || plateau[x + i][y] == 1) {
                estValide = 0;
            }
        } else if (nav->sens == 0 || nav->sens == 2) {
            if ((y + i) >= taille_plateau || plateau[x][y + i] == 1) {
                estValide = 0;
            }
        }
    }

    return estValide;
}

/**
 * Cree un plateau vide.
 * @param plateau la grille de plateau.
 * @param taille_plateau La taille du plateau.
 */
void creer_plateauvide(int **plateau, int taille_plateau) {
    int erreur = 0;
    if (!plateau) {
        printf("Erreur d'allocation de mémoire: %s\n", strerror(errno));
        exit(1);
    }
    for (int i = 0; i < taille_plateau; i++) {
        plateau[i] = calloc(taille_plateau, sizeof(int));
        if (!plateau[i]) {
            erreur = 1;
            break;
        }
    }

    if (erreur == 1) {
        printf("Erreur d'allocation de mémoire: %s\n", strerror(errno));
    }


}
/**
 * Mets le navire dans le plateau.
 * @param plateau Le plateau
 * @param taille_plateau Taille maximale du plateau
 * @param nav Le navire a inserer dans le plateau,
 */
void mettre_navire(int **plateau, int taille_plateau, struct navire *nav) {
    Navire navire = *nav;
    int isFalse = 0;
    while (isFalse != 1) {
        navire = creer_navire(nav->taille, taille_plateau);
        isFalse = est_valide(plateau, taille_plateau, &navire);
    }
    int x = navire.premiere_case.x;
    int y = navire.premiere_case.y;

    if (navire.sens == 0 || navire.sens == 2) {
        for (int j = y; j <= (y + navire.taille - 1); j++) {
            plateau[x][j] = navire.taille;
        }
    } else if (navire.sens == 1 || navire.sens == 3) {

        for (int j = x; j <= x + (navire.taille - 1); j++) {
            plateau[j][y] = navire.taille;
        }

    }
}

/**
 * Initialise 6 navires dans le plateau.
 * @param plateau La grille du plateau
 * @param taille_plateau La taille maximale du plateau.
 */
void initialisation_plateau(int **plateau, int taille_plateau) {
    Navire navire = creer_navire(2, taille_plateau);
    Navire navire2 = creer_navire(2, taille_plateau);
    Navire navire3 = creer_navire(3, taille_plateau);
    Navire navire4 = creer_navire(4, taille_plateau);
    Navire navire5 = creer_navire(5, taille_plateau);
    Navire navire6 = creer_navire(6, taille_plateau);

    mettre_navire(plateau, taille_plateau, &navire);
    mettre_navire(plateau, taille_plateau, &navire2);
    mettre_navire(plateau, taille_plateau, &navire3);
    mettre_navire(plateau, taille_plateau, &navire4);
    mettre_navire(plateau, taille_plateau, &navire5);
    mettre_navire(plateau, taille_plateau, &navire6);
}

/**
 * Incremente de nombre de tirs sur les navires selon leur taille
 * @param x Coordonnee X
 * @param y Coordonnee Y
 * @param plateau La grille du plateau
 * @param nbToucheNav Le tableau des tirs de navires selon leur taille.
 */
void incrementer_ToucheNav(int x, int y, int **plateau, int *nbToucheNav){
    for (int i = 2; i <= 6; i++) {
        if (plateau[y][x] == i) {
            nbToucheNav[i]++;
            if (nbToucheNav[i] == i){
                printf("Le navire de taille %i a coule\n", i);
            }
        }
    }
}
/**
 * Propose un tir a effectuer sur une coordonnée et affiche les résultats,
 * @param plateau La grille du plateau
 * @param prop La grille de tableau de proposition du joueur
 * @param nbTouche Le nombre de tirs touchant un navire.
 * @param nbJoue Le nombre de tirs effectues
 * @param nbToucheNav Le tableau des tirs de navires selon leur taille.
 * @param taille_plateau La taille maximale du plateau.
 */
void proposition_joueur(int **plateau, int **prop, int *nbTouche, int
*nbJoue, int *nbToucheNav, int taille_plateau) {
    int x, y;
    printf("Chossissez la coordonnee de X et Y\n");
    scanf("%i", &x);
    scanf("%i", &y);
    while (x < 0 || x >= taille_plateau || y < 0 || y >= taille_plateau) {
        printf("Rechossissez les coordonnées de X et Y\n");
        scanf("%i", &x);
        scanf("%i", &y);
    }
    if (plateau[y][x] != 0) {
        if (prop[y][x] != 1) {
            prop[y][x] = 1;
            *nbJoue = *nbJoue + 1;
            *nbTouche = *nbTouche + 1;
            incrementer_ToucheNav(x ,y ,plateau ,nbToucheNav);
        } else {
            prop[y][x] = -1;
            *nbJoue = *nbJoue + 1;
        }
    } else if (plateau[x][y] == 0) {
        prop[y][x] = 0;
        *nbJoue = *nbJoue + 1;
    }
    affichage_plateau(prop, taille_plateau);
}
/**
 * Affiche le plateau
 * @param plateau La grille du plateau
 * @param taille_plateau La taille maximale du plateau
 */
void affichage_plateau(int **plateau, int taille_plateau) {
    for (int i = 0; i < taille_plateau; i++) {
        for (int j = 0; j < taille_plateau; j++) {
            printf("%d", plateau[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int taille_plateau = 0;
    int **plateau;
    int **prop;
    int nbJoue = 0;
    int nbTouche = 0;
    char quit;
    int nbTouchNav[8];
    for (int i = 0; i <= 6; i++){
       nbTouchNav[i] = 0;
    }

    printf("Chossissez la taille du tableau");
    scanf("%i", &taille_plateau);
    plateau = calloc(taille_plateau, sizeof(int *));
    prop = calloc(taille_plateau, sizeof(int *));
    init_nb_aleatoire();

    creer_plateauvide(plateau, taille_plateau);
    creer_plateauvide(prop, taille_plateau);
    initialisation_plateau(plateau, taille_plateau);
    //affichage_plateau(plateau, taille_plateau);
    while (nbTouche != 22 && quit != 'q') {
        proposition_joueur(plateau, prop, &nbTouche,
                           &nbJoue, nbTouchNav, taille_plateau);
        if (nbJoue > 0) {
            printf("Voulez-vous quitter? Pressez q pour quitter ou "
                   "n'importe quel touche pour continuer.\n");
            scanf("%s", &quit);
        }
    }

    printf("Le nombre de tirs joue: %i\n", nbJoue);
    printf("Le nombre de tirs touche: %i\n", nbTouche);
    printf("Le nombre de tirs par navire:\n");

    for (int i = 2; i <= 6; i++){
        printf("Navire de taille %i: %i\n",i ,nbTouchNav[i]);
    }
    affichage_plateau(prop, taille_plateau);


    for (int k = 0; k < taille_plateau; k++) {
        free(plateau[k]);
        free(prop[k]);
    }
    free(plateau);
    free(prop);

    return 0;
}


