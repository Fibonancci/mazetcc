#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para isdigit, se precisar para parsing de ID

#include "desafio.h"      // Inclui o cabeçalho do desafio
#include "tela_desafio.h" // Inclui o cabeçalho para as telas de resultado
#include "constantes.h"   // Para macros globais, se necessário

// Funções utilitárias
extern void limparTela();
extern void exibirLogo();
extern int verificarInteiro(int *valor, int min_val, int max_val);

// Função para inicializar a lista de desafios
void inicializarListaDesafios(ListaDesafios *lista){
    if(lista == NULL){return;}
    lista->numDesafios = 0;
}

// Função para carregar todos os desafios de um arquivo .txt
/*int carregarTodosDesafios(ListaDesafios *lista, const char *lista_geral_desafios){
    char caminho_completo[256]; // Tamanho suficiente para "data/desafios/" + lista_geral_desafios
    snprintf(caminho_completo, sizeof(caminho_completo), "data/desafios/%s", lista_geral_desafios);

    FILE *arquivo = fopen(caminho_completo, "r");
    if (arquivo == NULL) {
        fprintf(stderr,"ERRO CRITICO: Erro ao abrir o arquivo de tela de desafio");
        exit(EXIT_FAILURE);
    }

    char linha[256];
    lista->numDesafios = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL && lista->numDesafios < MAX_DESAFIOS) {

        // Assumindo o formato do arquivo
        int id, est_bf, est_df, rep_bf, rep_df, motiv_bf, motiv_df, sabe_bf, sabe_df, nota_bf, nota_df;
        char nome[MAX_DESAFIO_NOME], arquivoTela[MAX_ARQUIVO_NOME], descricao[MAX_DESCRICAO_DESAFIO];

        // Analisar linha com sscanf para checar formato do arquivo
        int escaneado = sscanf(linha, "%d;%[^;];%[^;];%[^;];%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
                             &id, nome, arquivoTela, descricao,
                             &est_bf, &est_df, &rep_bf, &rep_df, &motiv_bf, 
                             &motiv_df, &sabe_bf, &sabe_df, &nota_bf, &nota_df);

        if (escaneado == 14) { // Se todos os 14 campos foram lidos com sucesso
            lista->desafios[lista->numDesafios].id = id;
            strncpy(lista->desafios[lista->numDesafios].nome, nome, MAX_DESAFIO_NOME - 1);
            lista->desafios[lista->numDesafios].nome[MAX_DESAFIO_NOME - 1] = '\0';
            
            strncpy(lista->desafios[lista->numDesafios].arquivoTela, arquivoTela, MAX_ARQUIVO_NOME - 1);
            lista->desafios[lista->numDesafios].arquivoTela[MAX_ARQUIVO_NOME - 1] = '\0';
            
            strncpy(lista->desafios[lista->numDesafios].descricao, descricao, MAX_DESCRICAO_DESAFIO - 1);
            lista->desafios[lista->numDesafios].descricao[MAX_DESCRICAO_DESAFIO - 1] = '\0';
            
            lista->desafios[lista->numDesafios].estamina_buff = est_bf;
            lista->desafios[lista->numDesafios].estamina_debuff = est_df;
            lista->desafios[lista->numDesafios].reputacao_buff = rep_bf;
            lista->desafios[lista->numDesafios].reputacao_debuff = rep_df;
            lista->desafios[lista->numDesafios].motivacao_buff = motiv_bf;
            lista->desafios[lista->numDesafios].motivacao_debuff = motiv_df;
            lista->desafios[lista->numDesafios].sabedoria_buff = sabe_bf;
            lista->desafios[lista->numDesafios].sabedoria_debuff = sabe_df;
            lista->desafios[lista->numDesafios].nota_buff = nota_bf;
            lista->desafios[lista->numDesafios].nota_debuff = nota_df;

            lista->numDesafios++;
        } else {
            fprintf(stderr, "Aviso: Linha inválida no arquivo de desafios: %s\n", linha);
        }
    }

    fclose(arquivo);
    return 0;
}*/

int carregarTodosDesafios(ListaDesafios *lista, const char *lista_geral_desafios){
    char caminho_completo[256];
    snprintf(caminho_completo, sizeof(caminho_completo), "data/desafios/%s", lista_geral_desafios);

    FILE *arquivo = fopen(caminho_completo, "r");
    if (arquivo == NULL) {
        fprintf(stderr,"ERRO CRITICO: Erro ao abrir o arquivo de desafios: %s\n", caminho_completo);
        exit(EXIT_FAILURE);
    }

    char linha[1024]; // Aumente o buffer da linha para acomodar todos os campos
    lista->numDesafios = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL && lista->numDesafios < MAX_DESAFIOS) {
        linha[strcspn(linha, "\n")] = '\0'; // Remover a quebra de linha se houver

        // Variáveis para os campos padrão
        int id;
        char nome[MAX_DESAFIO_NOME], arquivoTela[MAX_ARQUIVO_NOME], descricao[MAX_DESCRICAO_DESAFIO];
        int est_bf, est_df, rep_bf, rep_df, motiv_bf, motiv_df, sabe_bf, sabe_df, nota_bf, nota_df;

        // Variáveis temporárias para ler os 3 conjuntos de 10 números para as opções
        int op0_val[10], op1_val[10], op2_val[10];

        // Analisar linha com sscanf
        // Total: 4 (id, nome, arquivoTela, descricao) + 10 (padrão) + 3*10 (opções) = 44 campos
        int escaneado = sscanf(linha, "%d;%[^;];%[^;];%[^;];%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;%d,%d,%d,%d,%d,%d,%d,%d,%d,%d;%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                             &id, nome, arquivoTela, descricao,
                             &est_bf, &est_df, &rep_bf, &rep_df, &motiv_bf, &motiv_df, &sabe_bf, &sabe_df, &nota_bf, &nota_df, // 14 campos (id + nome + arquivoTela + descricao + padrão)
                             &op0_val[0], &op0_val[1], &op0_val[2], &op0_val[3], &op0_val[4], &op0_val[5], &op0_val[6], &op0_val[7], &op0_val[8], &op0_val[9], // 10 campos (Opção A)
                             &op1_val[0], &op1_val[1], &op1_val[2], &op1_val[3], &op1_val[4], &op1_val[5], &op1_val[6], &op1_val[7], &op1_val[8], &op1_val[9], // 10 campos (Opção B)
                             &op2_val[0], &op2_val[1], &op2_val[2], &op2_val[3], &op2_val[4], &op2_val[5], &op2_val[6], &op2_val[7], &op2_val[8], &op2_val[9]); // 10 campos (Opção C)

        if (escaneado == 44) { // Se todos os 44 campos foram lidos com sucesso
            Desafio *desafioAtual = &(lista->desafios[lista->numDesafios]);

            desafioAtual->id = id;
            strncpy(desafioAtual->nome, nome, MAX_DESAFIO_NOME - 1);
            desafioAtual->nome[MAX_DESAFIO_NOME - 1] = '\0';
            
            strncpy(desafioAtual->arquivoTela, arquivoTela, MAX_ARQUIVO_NOME - 1);
            desafioAtual->arquivoTela[MAX_ARQUIVO_NOME - 1] = '\0';
            
            strncpy(desafioAtual->descricao, descricao, MAX_DESCRICAO_DESAFIO - 1);
            desafioAtual->descricao[MAX_DESCRICAO_DESAFIO - 1] = '\0';
            
            // Atribuição dos buffs/debuffs padrão
            desafioAtual->estamina_buff = est_bf;
            desafioAtual->estamina_debuff = est_df;
            desafioAtual->reputacao_buff = rep_bf;
            desafioAtual->reputacao_debuff = rep_df;
            desafioAtual->motivacao_buff = motiv_bf;
            desafioAtual->motivacao_debuff = motiv_df;
            desafioAtual->sabedoria_buff = sabe_bf;
            desafioAtual->sabedoria_debuff = sabe_df;
            desafioAtual->nota_buff = nota_bf;
            desafioAtual->nota_debuff = nota_df;

            // Atribuição dos valores para a matriz 'opcoes'
            for (int i = 0; i < 10; i++) {
                desafioAtual->opcoes[0][i] = op0_val[i]; // Opção A
                desafioAtual->opcoes[1][i] = op1_val[i]; // Opção B
                desafioAtual->opcoes[2][i] = op2_val[i]; // Opção C
            }
            
            lista->numDesafios++;
        } else {
            fprintf(stderr, "Aviso: Linha invalida no arquivo de desafios (campos lidos: %d): %s\n", escaneado, linha);
        }
    }

    fclose(arquivo);
    return 0;
}


// Função para buscar um desafio pelo ID na lista carregada
Desafio *buscarDesafioPorId(ListaDesafios *lista, int id){
    if(lista == NULL){return NULL;}

    for(int i = 0; i < lista->numDesafios; i++){
        if(lista->desafios[i].id == id){
            return &(lista->desafios[i]);
        }
    }
    return NULL;
}

// Exibir o conteúdo de um arquivo .txt de tela desafio
void exibirTelaDesafio(const char *tela_desafio) {
    int escolha;

    char caminho_completo[256]; // Tamanho suficiente para "data/" + tela_desafio
    snprintf(caminho_completo, sizeof(caminho_completo), "data/desafios/%s", tela_desafio);

    FILE *arquivo = fopen(caminho_completo, "r");
    if (arquivo == NULL) {
        fprintf(stderr,"ERRO CRITICO: Erro ao abrir o arquivo de tela de desafio.");
        exit(EXIT_FAILURE);
    }

    limparTela();

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }
    fclose(arquivo);
    do{
        scanf(" %d", &escolha);
        switch(escolha){

        }
        limparTela();
    } while(escolha !=1);
}