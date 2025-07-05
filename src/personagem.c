#include <stdio.h>
#include <stdlib.h> // Para system(), atoi
#include <string.h> // Para strcspn, strncpy
#include <ctype.h>  // Para isdigit
#include <string.h>

#include "labirinto.h"
#include "personagem.h" // Inclui o cabeçalho que define Personagem e suas funções
#include "constantes.h" // Inclui as macros para PONTUACAO_PER, PONTUACAO_ATR, TAMANHO_MAX_NOME

// Funções utilitárias definidas em outro arquivo
extern void limparTela();
extern void exibirLogo();
extern void aguardarVoltar(int opcaoSaida, const char *msgOpcao);
extern int verificarInteiro(int *valor, int min_val, int max_val);

// exibe todos os detalhes de um personagem
void exibirPersonagem(const Personagem *personagemAtual) {
    limparTela();
    exibirLogo();
    printf("\n--- Dados do Personagem ---\n");
    printf(" Nome: %s\n", personagemAtual->nome);
    printf(" Estamina: %d\n", personagemAtual->estamina);
    printf(" Reputacao: %d\n", personagemAtual->reputacao);
    printf(" Motivacao: %d\n", personagemAtual->motivacao);
    printf(" Sabedoria: %d\n", personagemAtual->sabedoria);
    printf(" Nota: %d\n", personagemAtual->nota);
    //printf(" Matricula: %d\n", personagemAtual->estado_atual);
    printf(" Labirintos Zerados: %d\n", personagemAtual->labirintos_zerados);
    printf("---------------------------\n");
    aguardarVoltar(1, "Jogar");
}

int verificarMortePersonagem(const Personagem *p) {

    if (p->estamina <= 0 ||
        p->reputacao <= 0 ||
        p->motivacao <= 0 ||
        p->sabedoria <= 0 ||
        p->nota <= 0) {
        return 1; // Personagem está morto
    }
    return 0; // Personagem está vivo
}

int criarPersonagem(Personagem *p){
    //Nota inicial
    p->nota = PONTUACAO_PER;
    p->labirintos_zerados=0;
    p->estado_atual=0; // Define personagem matriculado

    limparTela();
    exibirLogo();

    printf("\nDigite o nome do personagem (max %d caracteres): ", TAMANHO_MAX_NOME);
    
    if (fgets(p->nome, TAMANHO_MAX_NOME + 1, stdin) == NULL) {
        printf("Erro ao ler o nome.\n");
        return 0;
    }

    // Remove o \n
    p->nome[strcspn(p->nome, "\n")] = '\0';
   
    while (1) {
        int estamina, reputacao, motivacao, sabedoria;
        int pontosDisponiveis = PONTUACAO_ATR;

        // printf("\nVocê tem %d pontos restantes para distribuir entre Estamina, Reputação, Motivação e Sabedoria.\n", pontosDisponiveis);

        printf("\nVocê tem %d pontos restantes para distribuir.",pontosDisponiveis);
        printf("\nDigite a Estamina: ");
        if (!verificarInteiro(&estamina, 1, 99)) {
            continue;
        }
        if (estamina > pontosDisponiveis) {
            printf("Você não tem pontos suficientes. Pontos restantes: %d\n", pontosDisponiveis);
            continue;
        }
        pontosDisponiveis -= estamina;

        limparTela();
        exibirLogo();
        printf("\nVocê tem %d pontos restantes para distribuir.",pontosDisponiveis);
        printf("\nDigite a Reputação: ");
        if (!verificarInteiro(&reputacao, 1, 99)) {
            continue;
        }
        if (reputacao > pontosDisponiveis) {
            printf("Você não tem pontos suficientes. Pontos restantes: %d\n", pontosDisponiveis);
            continue;
        }
        pontosDisponiveis -= reputacao;

        limparTela();
        exibirLogo();
        printf("\nVocê tem %d pontos restantes para distribuir.",pontosDisponiveis);
        printf("\nDigite a Motivação: ");
        if (!verificarInteiro(&motivacao, 1, 99)) {
             continue;
        }
        if (motivacao > pontosDisponiveis) {
             printf("\nVocê não tem pontos suficientes. Pontos restantes: %d\n", pontosDisponiveis);
             continue;
        }
        pontosDisponiveis -= motivacao;

        limparTela();
        exibirLogo();
        printf("\nVocê tem %d pontos restantes para distribuir.",pontosDisponiveis);
        printf("\nDigite a Sabedoria: ");
        if (!verificarInteiro(&sabedoria, 1, 99)) {
            continue;
        }
        if (sabedoria > pontosDisponiveis) {
            printf("\nVocê não tem pontos suficientes. Pontos restantes: %d\n", pontosDisponiveis);
            continue;
        }
        pontosDisponiveis -= sabedoria;

        int totalDistribuido = estamina + reputacao + motivacao + sabedoria;
        if (totalDistribuido != PONTUACAO_ATR) {
            printf("\nA soma dos pontos deve ser exatamente %d. Pontos distribuídos: %d\n", PONTUACAO_ATR, totalDistribuido);
            continue;
        } else {
            p->estamina = estamina;
            p->reputacao = reputacao;
            p->motivacao = motivacao;
            p->sabedoria = sabedoria;
            printf("\nAtributos do personagem distribuídos com sucesso!\n");
            break; // Sai do loop se tudo estiver correto
        }
    }
    return 1; // Sucesso
}
