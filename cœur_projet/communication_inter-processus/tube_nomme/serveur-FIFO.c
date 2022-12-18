#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include "./../lib/types.h"
#include "./../lib/operations.h"
#include "./../lib/constantes.h"

int main(int argc, char const *argv[])
{

    int descripteur_tube_question, descripteur_tube_reponse, resultat_operation, nombre_client = atoi(argv[1]);
    question question;
    reponse reponse;
    info info_client, info_serveur;

    // Installation des handlers
    printf("[SERVEUR] : Installation des handlers...\n\n");
    signal(SIGUSR1, handler);

    // Ouverture de tube (question & reponse)
    printf("[SERVEUR] : Ouverture de tube (question & réponse) ...\n\n");
    descripteur_tube_question = resultat_operation = open(argv[2], O_RDONLY);
    traiter_erreur(resultat_operation, "OPEN", "Erreur lors de l'ouverture du tube de questions");
    descripteur_tube_reponse = resultat_operation = open(argv[3], O_WRONLY);
    traiter_erreur(resultat_operation, "OPEN", "Erreur lors de l'ouverture du tube de réponses");

    printf("=======================================================\n");
    printf("[SERVEUR] : Le serveur est prêt\n");
    printf("=======================================================\n\n");

    while (nombre_client)
    {
        // Lecture des informations du client
        resultat_operation = read(descripteur_tube_question, &info_client, sizeof(info));
        traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture des informations du client");

        if (resultat_operation != 0)
        {

            // Envoie des informations du serveur
            info_serveur.pid_source = getpid();
            resultat_operation = write(descripteur_tube_reponse, &info_serveur, sizeof(info));
            traiter_erreur(resultat_operation, "WRITE", "Erreur lors de l'envoie des informations du serveur");

            // Envoie signal au client
            kill(info_client.pid_source, SIGUSR1);

            while (info_client.nombre_question--)
            {

                // Attente du signal de client
                pause();

                //  Lecture du question
                resultat_operation = read(descripteur_tube_question, &question, sizeof(question));
                traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture du question");
                printf("[SERVEUR-CLIENT-%d-%d] : %d %s %d = ?\n", question.pid_client, question.numero_client, question.operande_1, operateurs[question.operation], question.operande_2);
                fflush(stdout);

                // Génération de réponse
                reponse = generer_reponse(question, 0);

                // Envoie de réponse
                resultat_operation = write(descripteur_tube_reponse, &reponse, sizeof(reponse));
                traiter_erreur(resultat_operation, "WRITE", "Erreur lors de l'envoie de la réponse");

                // Envoie signal au client
                kill(info_client.pid_source, SIGUSR1);
            }
            nombre_client--;
            printf("=======================================================\n\n");
        }
    }

    // Fermeture des tubes
    printf("[SERVEUR] : Fermeture des tubes ...\n\n");
    resultat_operation = close(descripteur_tube_question);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du tube de questions");
    resultat_operation = close(descripteur_tube_reponse);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du tube de réponses");
    printf("=======================================================\n");
    printf("[SERVEUR] : Le serveur est bien fermé \n");
    printf("=======================================================\n");

    return 0;
}
