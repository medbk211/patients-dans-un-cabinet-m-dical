#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    char nom[50];
    char prenom[50];
    int rdv; // 0 pour sans rendez-vous, 1 pour avec rendez-vous
} Patient;

// Structure pour représenter une cellule de la liste chaînée
typedef struct Cellule {
    Patient patient;
    struct Cellule *suivant;
} Cellule;

// Type liste (pointeur vers une cellule)
typedef Cellule *liste;


void saisirPatient(Patient *p) {
    printf("Entrez le nom du patient : ");
    scanf("%s", p->nom);
    printf("Entrez le prénom du patient : ");
    scanf("%s", p->prenom);
    printf("Le patient a-t-il un rendez-vous ? (0 pour sans rendez-vous, 1 pour avec rendez-vous) : ");
    scanf("%d", &p->rdv);
}


void afficherPatient(Patient p) {
    printf("Nom : %s | ", p.nom);
    printf("Prénom : %s | ", p.prenom);
    printf("Rendez-vous : %s | \n", p.rdv ? "Avec" : "Sans");
}


liste AjoutPatient(liste tete, Patient P) {
    Cellule* nouvelleCellule = (Cellule*)malloc(sizeof(Cellule));
    nouvelleCellule->patient = P;
    nouvelleCellule->suivant = NULL;

    if (tete == NULL) {
        return nouvelleCellule;
    } else {
        Cellule* courant = tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouvelleCellule;
        return tete;
    }
}


void RendezVous(liste tete, int* rdv, int* sansRdv) {
    *rdv = 0;
    *sansRdv = 0;

    Cellule *courant = tete;
    while (courant != NULL) {
        if (courant->patient.rdv == 1) {
            (*rdv)++;
        } else {
            (*sansRdv)++;
        }
        courant = courant->suivant;
    }
}

liste SupprimePatient(liste tete) {
    if (tete == NULL) {
        printf("La liste est vide. Aucun patient à supprimer.\n");
        return tete;
    }

    Cellule *courant = tete;
    Cellule *prec = NULL;

    while(courant != NULL && courant->patient.rdv == 1){
        prec = courant;
        courant = courant->suivant;
    }

    if (courant == NULL) {
        printf("Aucun patient sans rendez-vous trouvé.\n");
        return tete;
    }

    if (prec == NULL) {
        // Le premier patient n'a pas de rendez-vous
        tete = tete->suivant;
    } else {
        prec->suivant = courant->suivant;
    }
    free(courant);
    printf("Le premier patient sans rendez-vous a été supprimé.\n");

    return tete;
}

void ConsulterSalleAttente(liste tete) {
    if (tete == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    printf("Patients avec rendez-vous :\n");
    Cellule *courant = tete;
    while (courant != NULL) {
        if (courant->patient.rdv == 1) {
            afficherPatient(courant->patient);
        }
        courant = courant->suivant;
    }

    printf("Patients sans rendez-vous :\n");
    courant = tete;
    while (courant != NULL) {
        if (courant->patient.rdv == 0) {
            afficherPatient(courant->patient);
        }
        courant = courant->suivant;
    }

    printf("Opération effectuée avec succès.\n");
}

void sauvegarderPatients(liste tete,char *chemin){
 FILE *consultation;

 consultation = fopen(chemin,"w");

 if(consultation == NULL){
    printf("Erreur lors de l'ouverture du fichier.\n");
    return;
 }

 Cellule *courant = tete;
  while (courant != NULL){
        fprintf(consultation, "%s %s %d\n", courant->patient.nom, courant->patient.prenom, courant->patient.rdv);
        courant = courant->suivant;
        courant= courant->suivant;
  }

   fclose(consultation);

    printf("Les patients ont été sauvegardés avec succès dans le fichier : %s\n", chemin);
}

liste recupererListePatients(liste tete, char *chemin) {
    FILE *consultation;
    consultation = fopen(chemin, "r");
    if (consultation == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return tete;
    }


    char ligne[100];
    while (fgets(ligne, sizeof(ligne), consultation) != NULL) {
        Patient p;
        if (sscanf(ligne, "%s %s %d", p.nom, p.prenom, &p.rdv) == 3) {
            tete = AjoutPatient(tete, p);
        }
    }
    fclose(consultation);
    return tete;
}




int main() {
    liste tete = NULL;

    int choix;
    do {
        printf("\nMenu :\n");
        printf("1. Ajouter un patient\n");
        printf("2. Afficher la liste des patients\n");
        printf("3. Compter les rendez-vous\n");
        printf("4. Supprime Patient\n");
        printf("5. affiche tout d’abord les patients avec rendez-vous, ensuite les patients sans rendez-vous.\n");
        printf("6. Sauvegarder les patientdans un fichier accessible selon un chemin\n");
        printf("7. Retourner dans une liste chaînée les données d un fichier accessible selon un chemin.\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                {
                    Patient nouveauPatient;
                    saisirPatient(&nouveauPatient);
                    tete = AjoutPatient(tete, nouveauPatient);
                    printf("Patient ajouté à la liste.\n");
                }
                break;
            case 2:
                {
                   {
                    Cellule* courant = tete;
                    if(tete == NULL){
                        printf("La liste est vide ");
                    }
                    while (courant != NULL) {
                        afficherPatient(courant->patient);
                        courant = courant->suivant;
                    }
                }
                }
                break;
            case 3:
                {
                    int rdv, sansRdv;
                    RendezVous(tete, &rdv, &sansRdv);
                    printf("Nombre de patients avec rendez-vous : %d\n", rdv);
                    printf("Nombre de patients sans rendez-vous : %d\n", sansRdv);
                }
                break;
            case 4:
                {
                    SupprimePatient(tete);
                }
                break;
            case 5:
                {
                    ConsulterSalleAttente (tete);
                }
                break;
            case 6:
                {
                 sauvegarderPatients(tete,"patient.txt");

                }
                break;
             case 7:
                {

               tete =  recupererListePatients(tete,"patient.txt");
                }
                break;
            case 0:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);



    return 0;
}
