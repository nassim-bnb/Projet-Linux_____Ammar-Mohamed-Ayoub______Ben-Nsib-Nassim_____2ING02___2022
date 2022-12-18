#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "./../../lib/types.h"
#include "./../../lib/operations.h"
#include "./../../lib/constantes.h"


int main(int argc, char const *argv[])
{

    int descripteur_socket_client, resultat_operation, numero_client = atoi(argv[1]),port_serveur = atoi(argv[2]);
    struct sockaddr_in adresse_socket_serveur;
    int taille_adresse_serveur = sizeof(adresse_socket_serveur);
    info info;
    question question;
    reponse reponse;
    srand(getpid());

    // Création du socket de client
    printf("\n[CLIENT-%d-%d] : Création du socket de client ...\n\n", getpid(), numero_client);
    descripteur_socket_client = resultat_operation = socket(AF_INET, SOCK_DGRAM, 0);
    traiter_erreur(resultat_operation, "SOCKET", "Erreur lors de création du socket de client");

    // Configuration d'adresse de serveur
    printf("[CLIENT-%d-¨%d] : Configuration de l'adresse du serveur ...\n\n", getpid(), numero_client);
    bzero((char *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    adresse_socket_serveur.sin_family = AF_INET;
    adresse_socket_serveur.sin_port = htons(port_serveur);

    printf("=======================================================\n");
    printf("[CLIENT-%d-%d] : Le client est prêt ...\n", getpid(), numero_client);
    printf("=======================================================\n\n");

    info = generer_nombre_question(getpid(), 0);

    for (int numero = 1; numero <= info.nombre_question; numero++)
    {
        // Génération du question
        question = generer_question(getpid(), numero_client, numero == info.nombre_question);
        printf("[CLIENT-%d-%d] : %d %s %d = ", getpid(), numero_client, question.operande_1, operateurs[question.operation], question.operande_2);

        // Envoie du question
        resultat_operation = sendto(descripteur_socket_client, &question, sizeof(question), MSG_CONFIRM, (const struct sockaddr *)&adresse_socket_serveur, taille_adresse_serveur);
        traiter_erreur(resultat_operation, "SENDTO", "Erreur lors de l'envoie du question");

        // Lecture de la réponse
        resultat_operation = recvfrom(descripteur_socket_client, &reponse, sizeof(reponse), MSG_WAITALL, (struct sockaddr *)&adresse_socket_serveur, &taille_adresse_serveur);
        traiter_erreur(resultat_operation, "RECVFROM", "Erreur lors de la lecture de la réponse");
        printf("%d\n", reponse.resultat);

        fflush(stdout);
    }

    // Fermeture du socket de client
    printf("\n[CLIENT-%d-%d] : Fermeture du socket de client ...\n\n", getpid(), numero_client);
    resultat_operation = close(descripteur_socket_client);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du socket de client");
    printf("=======================================================\n");
    printf("[CLIENT-%d-%d] : Le client est bien fermé\n", getpid(), numero_client);
    printf("=======================================================\n\n");
    return 0;
}
