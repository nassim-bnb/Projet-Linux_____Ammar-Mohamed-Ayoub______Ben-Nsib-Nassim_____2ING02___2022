#ifndef OPERATIONS_DEFINE_HEADER
#define OPERATIONS_DEFINE_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "types.h"
#include "constantes.h"

reponse generer_reponse(question question, int pid_serveur)
{
    reponse reponse;

    reponse.resultat = operations[question.operation](question.operande_1, question.operande_2);
    reponse.pid_serveur = pid_serveur;

    return reponse;
}

info generer_nombre_question(int pid_client, int numero_client)
{
    info info;

    info.nombre_question = rand() % NOMBRE_MAX_QUESTION;
    info.nombre_question += NOMBRE_MIN_QUESTION;
    info.pid_source = pid_client;
    info.numero_client = numero_client;

    return info;
}

question generer_question(int pid_client, int numero_client, int est_dernier_question)
{
    question question;

    question.pid_client = pid_client;
    question.operation = rand() % NOMBRE_OPERATIONS;
    question.operande_1 = rand() % VALEUR_MAX_OPERANDE;
    question.operande_1 += VALEUR_MIN_OPERANDE;
    question.operande_2 = rand() % VALEUR_MAX_OPERANDE;
    question.operande_2 += VALEUR_MIN_OPERANDE;
    question.numero_client = numero_client;
    question.est_dernier_question = est_dernier_question;

    return question;
}

void traiter_erreur(int code, char *operation, char *message_erreur)
{
    if (code < 0)
    {
        printf("\033[91m");
        perror(operation);
        printf("\n%s\n\n", message_erreur);
        exit(EXIT_FAILURE);
    }
}

void handler(int code)
{
}

#endif