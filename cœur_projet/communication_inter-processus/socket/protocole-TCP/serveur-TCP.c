#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./../../lib/types.h"
#include "./../../lib/operations.h"
#include "./../../lib/constantes.h"


void lancer_service_client(int descripteur_socket_client)
{
    int resultat_operation;
    reponse reponse;
    question question;
    info info;

    // Lecture des informations du client
    resultat_operation = read(descripteur_socket_client, &info, sizeof(info));
    traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture des informations du client");
    printf("========================================================================\n");
    printf("[SERVEUR-SERVICE-%d-%d] : Service est prêt pour le client ...\n", info.pid_source, info.numero_client);
    printf("========================================================================\n\n");

    for (int numero = 1; numero <= info.nombre_question; numero++)
    {
        // Lecture du question
        resultat_operation = read(descripteur_socket_client, &question, sizeof(question));
        traiter_erreur(resultat_operation, "READ", "Erreur lors de la lecture du question");
        printf("[SERVEUR-SERVICE-%d-%d] : %d %s %d = ?\n", info.pid_source, info.numero_client, question.operande_1, operateurs[question.operation], question.operande_2);

        // Génération de réponse
        reponse = generer_reponse(question, 0);

        // Envoie de réponse
        resultat_operation = write(descripteur_socket_client, &reponse, sizeof(reponse));
        traiter_erreur(resultat_operation, "READ", "Erreur lors de l'envoie de la réponse");

        fflush(stdout);
    }

    // Fermeture du socket de client
    printf("\n[SERVEUR-SERVICE-%d-%d] : Fermeture du socket de service ...\n\n", info.pid_source, info.numero_client);
    resultat_operation = close(descripteur_socket_client);
    traiter_erreur(descripteur_socket_client, "CLOSE", "Erreur lors de la fermeture du socket de service");
    printf("===========================================================\n");
    printf("[SERVEUR-SERVICE-%d-%d] : Le service est bien fermé\n", info.pid_source, info.numero_client);
    printf("===========================================================\n\n");
    exit(0);
    fflush(stdout);
}

int main(int argc, char const *argv[])
{

    int descripteur_socket_serveur, descripteur_socket_client, resultat_operation, nombre_client = atoi(argv[1]),port_serveur = atoi(argv[2]);
    struct sockaddr_in adresse_socket_serveur, adresse_socket_client;
    socklen_t taille_adresse_client = sizeof(adresse_socket_client);

    // Création du socket de serveur
    printf("[SERVEUR] : Création du socket de serveur ...\n\n");
    descripteur_socket_serveur = resultat_operation = socket(AF_INET, SOCK_STREAM, 0);
    traiter_erreur(resultat_operation, "SOCKET", "Erreur lors de création du socket de serveur");

    // Configuration d'adresse de serveur
    printf("[SERVEUR] : Configuration de l'adresse du serveur ...\n\n");
    bzero((char *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    adresse_socket_serveur.sin_family = AF_INET;
    adresse_socket_serveur.sin_addr.s_addr = INADDR_ANY;
    adresse_socket_serveur.sin_port = htons(port_serveur = atoi(argv[2]));

    // Attachment le socket de serveur au port et à l'adresse
    printf("[SERVEUR] : Attachment le socket de serveur au port et à l'adresse ...\n\n");
    resultat_operation = bind(descripteur_socket_serveur, (struct sockaddr *)&adresse_socket_serveur, sizeof(adresse_socket_serveur));
    traiter_erreur(resultat_operation, "BIND", "Erreur lors d'attachment le socket de serveur au port et à l'adresse");

    // Mettre le serveur à l'écoute des nouvelles connexions de clients
    printf("[SERVEUR] : Mettre le serveur à l'écoute des nouvelles connexions de clients ...\n\n");
    resultat_operation = listen(descripteur_socket_serveur, NOMBRE_MAX_CONNEXION_CLIENT);
    traiter_erreur(resultat_operation, "LISTEN", "Erreur lors de mettre le serveur à l'écoute des nouvelles connexions de clients");

    printf("=======================================================\n");
    printf("[SERVEUR] : Le serveur est prêt\n");
    printf("=======================================================\n\n");

    while (nombre_client--)
    {
        // Attente de nouvelle connexion
        printf("\n[SERVEUR] : Attente de nouvelles connexions ...\n\n");
        descripteur_socket_client = resultat_operation = accept(descripteur_socket_serveur, (struct sockaddr *)&adresse_socket_client, &taille_adresse_client);
        traiter_erreur(resultat_operation, "ACCEPT", "Erreur lors de l'acceptation de nouvelle connexion");

        // Création de nouveau processus fils, pour s'occuper de nouveau client
        printf("[SERVEUR] : Création de nouveau processus fils, pour s'occuper de nouveau client ...\n\n");
        resultat_operation = fork();
        traiter_erreur(resultat_operation, "FORK", "Erreur lors de la création nouveau processus fils");

        // Lancement de logique de service
        if (!resultat_operation)
        {
            lancer_service_client(descripteur_socket_client);
        }
    }

    // Fermeture du socket de serveur
    printf("[SERVEUR] : Fermeture du socket de serveur ...\n\n");
    resultat_operation = shutdown(descripteur_socket_serveur, SHUT_RDWR);
    traiter_erreur(resultat_operation, "SHUTDOWN", "Erreur lors de la fermeture du socket de client");
    resultat_operation = close(descripteur_socket_serveur);
    traiter_erreur(resultat_operation, "CLOSE", "Erreur lors de la fermeture du socket de client");
    printf("=======================================================\n");
    printf("[SERVEUR] : Le serveur est bien fermé \n");
    printf("=======================================================\n");

    return 0;
}
