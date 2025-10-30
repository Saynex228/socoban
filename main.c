#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#define TAILLE 12 
#define MUR '#'

typedef char t_Plateau[TAILLE][TAILLE];
void afficherEntete();
int kbhit();
void chargerPartie(t_Plateau plateau, char fichier[]);
void afficherPlateau();

int main() {
    t_Plateau plateau;
    char nom[15];
    printf("Tappez un nom de fichier");
    scanf("%s", nom);
    chargerPartie(plateau, nom);
    afficherEntete();
    afficherPlateau(plateau);
}

void afficherEntete(){
     printf("====================================================\n");
     printf("                       Socoban                      \n");
     printf("           q, z, s, d pour les déplacements         \n");
     printf("   x pour abandonner et r pour recommencer la parti \n");
     printf("====================================================\n");
}
void afficherPlateau(t_Plateau plateau){
    for(int i = 0; i < TAILLE; i++){
        for(int j = 0; j < TAILLE; j++){
            printf("%c", plateau[i][j]);
        }
        printf("\n");
}
}



int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere présent
	int unCaractere=0;
	struct termios oldt, newt;
	int ch;
	int oldf;

	// mettre le terminal en mode non bloquant
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	ch = getchar();

	// restaurer le mode du terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}



void chargerPartie(t_Plateau plateau, char fichier[]){
    FILE * f;
    char finDeLigne;

    f = fopen(fichier, "r");
    if (f==NULL){
        printf("ERREUR SUR FICHIER");
        exit(EXIT_FAILURE);
    } else {
        for (int ligne=0 ; ligne<TAILLE ; ligne++){
            for (int colonne=0 ; colonne<TAILLE ; colonne++){
                fread(&plateau[ligne][colonne], sizeof(char), 1, f);
            }
            fread(&finDeLigne, sizeof(char), 1, f);
        }
        fclose(f);
    }
}

void enregistrerPartie(t_Plateau plateau, char fichier[]){
    FILE * f;
    char finDeLigne='\n';

    f = fopen(fichier, "w");
    for (int ligne=0 ; ligne<TAILLE ; ligne++){
        for (int colonne=0 ; colonne<TAILLE ; colonne++){
            fwrite(&plateau[ligne][colonne], sizeof(char), 1, f);
        }
        fwrite(&finDeLigne, sizeof(char), 1, f);
    }
    fclose(f);
}
