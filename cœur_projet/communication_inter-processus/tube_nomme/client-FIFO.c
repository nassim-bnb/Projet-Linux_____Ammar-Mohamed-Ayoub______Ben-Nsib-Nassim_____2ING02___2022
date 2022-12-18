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

    int descripteur_tube_question, descripteur_tube_reponse, resultat_operation, numero_client = atoi(argv[1]);
    question question;
    reponse reponse;
    info info_client, info_serveur;
    srand(getpid());

    // Installation des handlers
    printf("[CLIENT] : Installation des handlers...\n\n");
    signal(SIGUSR1, handler);

    // Ouverture de tube (question & reponse)
    printf("[CLIENT] : Ouverture de tube (question & réponse) ...\n\n");
    descripteur_tube_question = resultat_operation = open(argv[2], O_WRONLY);
    traiter_erreur(resultat_operation, "OPEN", "Erreur lors de l'ouverture du tube de questions");
    descripteur_tube_reponse = resultat_operation = open(argv[3], O_RDONLY);
    traiter_erreur(resultat_operation, "OPEN", "Erreur lors de l'ouverture du tube de réponses");

    printf("=======================================================\n");
    printf("[CLIENT] : Le client est prêt\n");
    printf("=======================================================\n\n");

    // Envoie des informations du client
    info_client = generer_nombre_question(getpid(), numero_client);
    resultat_operation = write(descripteur_tube_question, &info_client, sizeof(info_client));
    traiter_erreur(resultat_operation, "WRITE", "Erreur lors de l'envoie des informations du client");

    // Attente du signal de serveur
    pause();

    // Lecture des informations du serveur
    resultat_operation = read(descripteur_tube_reponse, &info_serveur, sizeof(info_serveur));
    traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture des informations du serveur");

    while (info_client.nombre_question--)
    {

        //  Envoie du question
        question = generer_question(getpid(), numero_client, 0);
        resultat_operation = write(descripteur_tube_question, &question, sizeof(question));
        traiter_erreur(resultat_operation, "WRITE", "Erreur lors de l'écriture du question");
        printf("[CLIENT-%d-%d] : %d %s %d = ", question.pid_client, question.numero_client, question.operande_1, operateurs[question.operation], question.operande_2);
        fflush(stdout);

        // Envoie signal au serveur
        kill(info_serveur.pid_source, SIGUSR1);

        // Attente du signal de serveur
        pause();

        // Lecture de réponse
        resultat_operation = read(descripteur_tube_reponse, &reponse, sizeof(reponse));
        traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture de la réponse");
        printf("%d\n", reponse.resultat);
        fflush(stdout);

        // Envoie signal au serveur
        kill(info_serveur.pid_source, SIGUSR1);
    }

    // Fermeture des tubes
    printf("\n[CLIENT] : Fermeture des tubes ...\n\n");
    resultat_operation = close(descripteur_tube_question);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du tube de questions");
    resultat_operation = close(descripteur_tube_reponse);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du tube de réponses");
    printf("=======================================================\n");
    printf("[CLIENT] : Le client est bien fermé \n");
    printf("=======================================================\n");

    return 0;
}
