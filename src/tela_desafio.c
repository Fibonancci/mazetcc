#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "tela_desafio.h"
#include "constantes.h" // Para acesso as constantes

// Funções utilitárias
extern void limparBufferEntrada();
extern void limparTela();
extern void exibirLogo();
extern void aguardarVoltar(int opcaoSaida, const char *msgOpcao);
extern int verificarInteiro(int *valor, int min_val, int max_val); // Se necessário

// Implementação da função para exibir tela de resultado (sucesso/falha)
void exibirTelaResultadoDesafio(int sucesso, const char *mensagemAdicional,
                                int estamina_mod, int reputacao_mod,
                                int motivacao_mod, int sabedoria_mod, int nota_mod) {
    limparTela();

    if (sucesso) {
        printf("\n====================================\n");
        printf("   DESAFIO CONCLUÍDO COM SUCESSO!\n");
        printf("====================================\n");
    } else {
        printf(ANSI_COLOR_RED);
        printf("\n====================================\n");
        printf("         DESAFIO FALHOU!\n");
        printf("====================================\n");
    }

    if (mensagemAdicional != NULL && strlen(mensagemAdicional) > 0) {
        printf("\n%s\n", mensagemAdicional);
    }

    printf("\n--- Impacto nos Atributos ---\n");
    if (estamina_mod != 0) {
        printf("    Estamina:  %s%d\n", (estamina_mod > 0 ? "+" : ""), estamina_mod);
    }
    if (reputacao_mod != 0) {
        printf("    Reputação: %s%d\n", (reputacao_mod > 0 ? "+" : ""), reputacao_mod);
    }
    if (motivacao_mod != 0) {
        printf("    Motivação: %s%d\n", (motivacao_mod > 0 ? "+" : ""), motivacao_mod);
    }
    if (sabedoria_mod != 0) {
        printf("    Sabedoria: %s%d\n", (sabedoria_mod > 0 ? "+" : ""), sabedoria_mod);
    }
    if (nota_mod != 0) {
        printf("   Nota: %s%d\n", (nota_mod > 0 ? "+" : ""), nota_mod);
    }
    printf("-------------------------------------\n");
    
    aguardarVoltar(1,"Retornar ao labirinto.");
    printf(ANSI_COLOR_GREEN);
}

// Exibir a tela de aceitar/recusar desafio
void exibirTelaDesafioAceitarRecusar(int id_desafio, const char *nome_desafio, const char *descricao_desafio) {
    int escolha,prosseguir=0;
    limparTela();

    
        printf("\n====================================\n");
        printf("         NOVO DESAFIO!\n");
        printf("====================================\n");
        printf("ID: %d\n", id_desafio);
        printf("Nome: %s\n", nome_desafio);
        printf("Descrição: %s\n", descricao_desafio);
        printf("------------------------------------\n");
        printf("1. Enfrentar o Desafio\n");
        printf("------------------------------------\n");
    do{
        scanf(" %d", &escolha);
        limparBufferEntrada();
        if(escolha==1){
            prosseguir=1;
        }
    } while(prosseguir!=1);
    limparTela();
}

int exibirTelaInteracaoDesafio(const char *nome_arquivo_tela) {
    char caminho_completo[256];
    snprintf(caminho_completo, sizeof(caminho_completo), "data/desafios/%s", nome_arquivo_tela);

    FILE *arquivo = fopen(caminho_completo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "ERRO CRITICO: Nao foi possivel abrir o arquivo de tela de desafio: %s\n", caminho_completo);
        exit(EXIT_FAILURE);
    }

    limparTela();

    char linha[256];
    // Exibir o conteúdo do arquivo de tela (descrição do desafio e suas opções)
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }
    fclose(arquivo);

    char escolha;
    int indice_escolhido = -1; // -1 para erro/saída
    int escolha_valida = 0;

    do {
        scanf(" %c", &escolha);
        limparBufferEntrada();
        escolha = toupper(escolha);

        if (escolha == 'A') {
            indice_escolhido = 0; // Corresponde ao indice 0 da matriz opcoes[][]
            escolha_valida = 1;
        } else if (escolha == 'B') {
            indice_escolhido = 1; // Corresponde ao indice 1
            escolha_valida = 1;
        } else if (escolha == 'C') {
            indice_escolhido = 2; // Corresponde ao indice 2
            escolha_valida = 1;
        } else {
            printf("Opcao invalida. ");
        }
    } while (!escolha_valida);

    return indice_escolhido;
}

void exibirTelaGameOver() {
    limparTela();

    printf(ANSI_COLOR_RED);
    printf("\n====================================\n");
    printf("           GAME OVER\n");
    printf("====================================\n");
    printf(ANSI_COLOR_RESET);
    printf("\nSeus atributos cairam a zero ou abaixo.\n");
    printf("Voce nao conseguiu se formar e trancou a matricula para sempre!\n");
    printf("Tente novamente em uma nova vida academica.\n");
    printf(ANSI_COLOR_GREEN);

}