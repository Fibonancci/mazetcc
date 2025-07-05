#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "personagem.h"
#include "constantes.h"

// Definição da estrutura ranking
typedef struct Ranking{
    char nomes[TAMANHO_RANKING][TAMANHO_MAX_NOME +1];
    int pontuacao[TAMANHO_RANKING];
} Ranking;

// Funções de Ranking
void inicializarRanking(Ranking *r);
int adicionarAoRanking(Ranking *r, const Personagem *p);
int carregarRanking(Ranking *r, const char *nomeArquivo);
void exibirRanking(const Ranking *r);
int salvarRanking(Ranking *r, const char *nomeArquivo);

#endif