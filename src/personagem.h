#ifndef PERSONAGEM_H_INCLUDED
#define PERSONAGEM_H_INCLUDED

#include "constantes.h"

// Definição da estrutura personagem
typedef struct Personagem{
    char nome[TAMANHO_MAX_NOME +1];

    int estamina;
    int reputacao;
    int motivacao;
    int sabedoria;
    int nota;

    int labirintos_zerados;
    int estado_atual; // Opcoes: 0 == matriculado; 1==jubilado; 2==formado;
} Personagem;

// Funções de personagem
void exibirPersonagem(const Personagem *p);
int criarPersonagem(Personagem *p); // Declaração da função de criação de personagem
int verificarMortePersonagem(const Personagem *p);

#endif