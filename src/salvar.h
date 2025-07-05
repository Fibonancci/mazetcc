#ifndef SALVAR_H_INCLUDED
#define SALVAR_H_INCLUDED

#include "constantes.h" // Inclui as macros de constantes
#include "personagem.h" // Inclui o cabeçalho que define Personagem e suas funções

// ageu
typedef struct SalvarJogo{
    Personagem perfil;
} SalvarJogo;

// Funções de Salvar
SalvarJogo carregarSave(); // Declaração da função de carregar personagem
int salvarSave(SalvarJogo *s); // ageu
int checarSave(SalvarJogo s);

#endif