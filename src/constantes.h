#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

// Constantes relacionadas ao ranking
#define TAMANHO_RANKING 5    // Número máximo de entradas no ranking

// Constantes relacionadas a personagens e atributos
#define TAMANHO_MAX_NOME 15   // Tamanho máximo permitido para nomes de personagens (sem contar o '\0')
#define PONTUACAO_PER 10 // Pontuação inicial base para a "nota" do personagem
#define PONTUACAO_ATR 100 // Total de pontos para distribuir nos atributos (estamina, reputacao, etc.)

// Constantes relacionadas ao labirinto
#define vazio 0 // vazio
#define parede 1 // parede
#define saida 2 // saida
#define comida 3 // comida
#define LARGURA 13
#define ALTURA 13
#define LABIRINTOS_PARA_FORMACAO 2 // Quantidade de labirintos para se formar

// Constantes relacionadas ao desafio
#define MAX_DESAFIO_NOME 50 // Tamanho máximo para o nome do desafio
#define MAX_ARQUIVO_NOME 100 // Tamanho máximo para o nome do arquivo da tela
#define MAX_DESAFIOS 20 // Tamanho máximo de desafios do games
#define MAX_DESCRICAO_DESAFIO 256 // Tamanho máximo da escriçao do desafio
#define MAX_INTERACOES 3 // Para as opções A, B, C

// Constantes de cores do texto
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#endif