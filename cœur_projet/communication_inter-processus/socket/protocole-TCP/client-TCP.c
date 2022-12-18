#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./../../lib/types.h"
#include "./../../lib/operations.h"
#include "./../../lib/constantes.h"


int main(int argc, char const *argv[])
{
    int descripteur_socket_client, resultat_operation, numero_client = atoi(argv[1]),port_serveur = atoi(argv[2]);
    struct sockaddr_in adresse_socket_serveur;
    info info;
    question question;
    reponse reponse;
    srand(getpid());

    // Création du socket de client
    printf("\n[CLIENT-%d-%d] : Création du socket de client ...\n\n", getpid(), numero_client);
    descripteur_socket_client = resultat_operation = socket(AF_INET, SOCK_STREAM, 0);
    traiter_erreur(resultat_operation, "SOCKET", "Erreur lors de création du socket de client");

    // Configuration d'adresse de serveur
    printf("[CLIENT-%d-¨%d] : Configuration de l'adresse du serveur ...\n\n", getpid(), numero_client);
    bzero((char *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    adresse_socket_serveur.sin_family = AF_INET;
    adresse_socket_serveur.sin_port = htons(port_serveur);

    // Conversion de l'adresse IPV4 en forme binaire
    resultat_operation = inet_pton(AF_INET, "127.0.0.1", (struct sockaddr *)&adresse_socket_serveur.sin_addr);
    traiter_erreur(resultat_operation, "INET_PTON", "Erreur lors de la conversion de l'adresse IPV4 en forme binaire");

    // Etablissment de connexion
    printf("[CLIENT-%d-%d] : Etablissment de connexion ...\n\n", getpid(), numero_client);
    resultat_operation = connect(descripteur_socket_client, (struct sockaddr *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    traiter_erreur(resultat_operation, "CONNECT", "Erreur lors d'etablissment de connexion");

    printf("=======================================================\n");
    printf("[CLIENT-%d-%d] : Le client est prêt ...\n", getpid(), numero_client);
    printf("=======================================================\n\n");

    info = generer_nombre_question(getpid(), numero_client);
    resultat_operation = write(descripteur_socket_client, &info, sizeof(info));
    traiter_erreur(resultat_operation, "WRITE", "Erreur lors de l'envoie des informations du client");

    for (int numero = 1; numero <= info.nombre_question; numero++)
    {
        // Génération du question
        question = generer_question(getpid(), 0, 0);
        printf("[CLIENT-%d-%d] : %d %s %d = ", getpid(), numero_client, question.operande_1, operateurs[question.operation], question.operande_2);

        // Envoie du question
        resultat_operation = write(descripteur_socket_client, &question, sizeof(question));
        traiter_erreur(resultat_operation, "WRITE", "Erreur lors de l'envoie du question");

        // Lecture de la réponse
        resultat_operation = read(descripteur_socket_client, &reponse, sizeof(reponse));
        traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture de la réponse");
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
