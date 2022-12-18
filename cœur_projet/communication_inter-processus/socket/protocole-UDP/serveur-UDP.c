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

    int descripteur_socket_serveur, descripteur_socket_client, resultat_operation, nombre_client = atoi(argv[1]),port_serveur = atoi(argv[2]);
    struct sockaddr_in adresse_socket_serveur, adresse_socket_client;
    int taille_adresse_client = sizeof(adresse_socket_client);
    question question;
    reponse reponse;

    // Création du socket de serveur
    printf("[SERVEUR] : Création du socket de serveur ...\n\n");
    descripteur_socket_serveur = resultat_operation = socket(AF_INET, SOCK_DGRAM, 0);
    traiter_erreur(resultat_operation, "SOCKET", "Erreur lors de création du socket de serveur");

    // Configuration d'adresse de serveur
    printf("[SERVEUR] : Configuration de l'adresse du serveur ...\n\n");
    bzero((char *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    bzero((char *)&adresse_socket_client, sizeof(adresse_socket_client));
    adresse_socket_serveur.sin_family = AF_INET;
    adresse_socket_serveur.sin_addr.s_addr = INADDR_ANY;
    adresse_socket_serveur.sin_port = htons(port_serveur);

    // Attachment le socket de serveur au port et à l'adresse
    printf("[SERVEUR] : Attachment le socket de serveur au port et à l'adresse ...\n\n");
    resultat_operation = bind(descripteur_socket_serveur, (struct sockaddr *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    traiter_erreur(resultat_operation, "BIND", "Erreur lors d'attachment le socket de serveur au port et à l'adresse");

    printf("=======================================================\n");
    printf("[SERVEUR] : Le serveur est prêt\n");
    printf("=======================================================\n\n");

    while (nombre_client)
    {
        // Lecture du question
        resultat_operation = recvfrom(descripteur_socket_serveur, &question, sizeof(question), MSG_WAITALL, (struct sockaddr *)&adresse_socket_client, &taille_adresse_client);
        traiter_erreur(resultat_operation, "RECVFROM", "Erreur lors de la lecture du question");
        printf("[SERVEUR-CLIENT-%d-%d] : %d %s %d = ?\n", question.pid_client, question.numero_client, question.operande_1, operateurs[question.operation], question.operande_2);
        if (question.est_dernier_question)
        {
            nombre_client--;
        }

        // Génération de réponse
        reponse = generer_reponse(question, 0);

        // Envoie de réponse
        resultat_operation = sendto(descripteur_socket_serveur, &reponse, sizeof(reponse), MSG_CONFIRM, (const struct sockaddr *)&adresse_socket_client, taille_adresse_client);
        traiter_erreur(resultat_operation, "SENDTO", "Erreur lors de l'envoie de la réponse");

        fflush(stdout);
    }

    // Fermeture du socket de serveur
    printf("\n[SERVEUR] : Fermeture du socket de serveur ...\n\n");
    resultat_operation = close(descripteur_socket_serveur);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du socket de client");
    printf("=======================================================\n");
    printf("[SERVEUR] : Le serveur est bien fermé \n");
    printf("=======================================================\n");

    return 0;
}
