#ifndef DESAFIO_H_INCLUDED
#define DESAFIO_H_INCLUDED

#include "constantes.h"

// Definição estrutura desafio
typedef struct Desafio{
	int id;
	char nome[MAX_DESAFIO_NOME];
	char arquivoTela[MAX_ARQUIVO_NOME];
	char descricao[MAX_DESCRICAO_DESAFIO];

	// Modificadores para os 5 atributos (buffs e debuffs)
	int estamina_buff;
    int estamina_debuff;
    int reputacao_buff;
    int reputacao_debuff;
    int motivacao_buff;
    int motivacao_debuff;
    int sabedoria_buff;
    int sabedoria_debuff;
    int nota_buff;
    int nota_debuff;

    // Matriz de opcoes
    int opcoes[3][10]; // Ex: opcoes[0] para A, opcoes[1] para B, opcoes[2] para C
} Desafio;

typedef struct ListaDesafios{
	Desafio desafios[MAX_DESAFIOS];
    int numDesafios;
} ListaDesafios;

// Função para inicializar a lista de desafios
void inicializarListaDesafios(ListaDesafios *lista);

// Função para carregar todos os desafios de um arquivo .txt
int carregarTodosDesafios(ListaDesafios *lista, const char *nome_arquivo_desafios_geral);

// Função para buscar um desafio pelo ID na lista carregada
// Retorna um ponteiro para o Desafio encontrado ou NULL se não encontrar
Desafio *buscarDesafioPorId(ListaDesafios *lista, int id);

// Função para exibir o conteúdo de um arquivo .txt de tela de desafio
void exibirTelaDesafio(const char *nome_arquivo);

#endif