#include <stdio.h>
#include "Initialisation_Disque.c"
#include "Vidage.c"
#include "Défragmentation_Blocs.c"
#include "Update.c"
#include "Gestion_blocs_libres.c"

void afficherMenu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Initialiser le disque\n");
    printf("2. Vidage de la mémoire secondaire\n");
    printf("3. Défragmenter les blocs\n");
    printf("4. Mettre à jour les métadonnées\n");
    printf("5. Gérer les blocs libres (vérification)\n");
    printf("6. Quitter\n");
    printf("Votre choix : ");
}

int main() {
    Disk myDisk; // Déclarer le disque principal
    int choix;

    do {
        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                // Initialiser le disque
                initializeDisk(&myDisk);
                printf("Disque initialisé avec succès.\n");
                break;

            case 2:
                // Vidage de la mémoire secondaire
                clearDisk(&myDisk);
                printf("Mémoire secondaire vidée avec succès.\n");
                break;

            case 3:
                // Défragmenter les blocs
                defragmentBlocks(&myDisk);
                printf("Blocs défragmentés avec succès.\n");
                break;

            case 4:
                // Mettre à jour les métadonnées
                // stillSTILLstill

            case 5:
                // Gérer les blocs libres
                // stillSTILLstill

            case 6:
                printf("Au revoir !\n");
                break;

            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 6);

    return 0;
}
