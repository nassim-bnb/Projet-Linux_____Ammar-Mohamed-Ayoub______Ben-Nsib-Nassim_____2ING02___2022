#ifndef TYPE_DEFINE_HEADER
#define TYPE_DEFINE_HEADER

#include "operations_math.h"



enum operation
{
    ADDITION,
    SOUSSTRACTION,
    DIVISION,
    MULTIPLICATION,
};

typedef struct info
{
    int nombre_question;
    int pid_source;
    int numero_client;
} info;

typedef struct question
{
    enum operation operation;
    int pid_client;
    int numero_client;
    int operande_1;
    int operande_2;
    int est_dernier_question;
} question;

typedef struct reponse
{
    int pid_serveur;
    int resultat;
} reponse;

typedef int (*fonction)(int x, int y);

fonction operations[] = {
    &addition,
    &soustraction,
    &division,
    &multiplication,
};
char *operateurs[] = {"+", "-", "/", "*"};

#endif
