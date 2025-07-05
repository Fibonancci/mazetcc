#include <stdio.h>
#include <stdlib.h> // Para system(), atoi
#include <string.h> // Para strcpy, strncpy, strcmp

#include "ranking.h"    // Inclui o cabeçalho que define Ranking e suas funções
#include "personagem.h" // Precisa de Personagem para a função adicionarAoRanking
#include "constantes.h" // Inclui as macros de constantes

// Funções utilitárias definidas em outro arquivo
extern void limparTela();
extern void exibirLogo();
extern void aguardarVoltar(int opcaoSaida, const char *msgOpcao);

// inicializa o ranking com valores padrão "Vazio" e pontuação 0
void inicializarRanking(Ranking *r){
    for(int i=0;i<TAMANHO_RANKING;i++){
        strcpy(r->nomes[i],"Vazio");
        r->pontuacao[i]=0;
    }
}

// adiciona ou atualiza um personagem no ranking, mantendo-o ordenado por pontuação
int adicionarAoRanking(Ranking *r, const Personagem *p) {
    int totalPontuacao = 0;
    int posExistente = -1; // -1 se não encontrado, ou o índice se encontrado
    int adicionado = 0;    // 0 = não adicionado/atualizado, 1 = adicionado/atualizado

    // Calcula a pontuacao total do personagem
    totalPontuacao = p->estamina + p->motivacao + p->reputacao +
                     p->sabedoria + p->nota;

    // 1. Procurar pelo personagem no ranking existente
    for (int i = 0; i<TAMANHO_RANKING && posExistente==-1; i++) {
        if (strcmp(r->nomes[i], p->nome) == 0) { // Nome encontrado
            posExistente = i;
        }
    }

    // 2. Lógica de atualização/inserção
    if (posExistente != -1) { // Personagem já existe no ranking
        if (totalPontuacao > r->pontuacao[posExistente]) {
            // Nova pontuação é maior, remover a entrada antiga e inserir a nova
            // Mover as entradas ABAIXO da posição atual para cima para "remover" a antiga
            for (int i = posExistente; i < TAMANHO_RANKING - 1; i++) {
                strncpy(r->nomes[i], r->nomes[i + 1], TAMANHO_MAX_NOME);
                r->nomes[i][TAMANHO_MAX_NOME] = '\0'; // Garante terminação nula
                r->pontuacao[i] = r->pontuacao[i + 1];
            }
            // A última posição do ranking agora contém a antiga última entrada (ou Vazio/0)
            // ou uma cópia do que estava na penúltima, precisamos limpar a última
            // para que a próxima inserção não duplique (se for uma inserção no final)
            strncpy(r->nomes[TAMANHO_RANKING - 1], "Vazio", TAMANHO_MAX_NOME);
            r->nomes[TAMANHO_RANKING - 1][TAMANHO_MAX_NOME] = '\0';
            r->pontuacao[TAMANHO_RANKING - 1] = 0;

            // Agora, a entrada antiga foi "removida".
            // Procedemos para inserir a nova pontuação como se fosse uma nova entrada.
            // (Isso será tratado pelo código abaixo, onde a lógica de inserção ordenada já existe)
            // Definimos posExistente para -1 para que o próximo 'if' (totalPontuacao > r->pontuacao[TAMANHO_RANKING-1])
            // e o loop de inserção sejam acionados corretamente.
            posExistente = -1; // Força a reavaliação para inserção como novo.
            adicionado = 1; // Já marcamos como adicionado, pois houve uma atualização/movimentação.
        } else {
            // Nova pontuação é menor ou igual, não faz nada
            return 0;
        }
    }

    // 3. Inserir a pontuação (se for nova ou se a antiga foi atualizada e "removida")
    if ((totalPontuacao > r->pontuacao[TAMANHO_RANKING - 1]) || (posExistente == -1 && adicionado == 1)) { // Só entra se for pontuação melhor que a última ou se já foi um update
        for (int i = 0; i < TAMANHO_RANKING; i++) {
            if (totalPontuacao > r->pontuacao[i]) {
                // Mover nomes/pontuações para baixo para abrir espaço
                for (int j = TAMANHO_RANKING - 1; j > i; j--) {
                    strncpy(r->nomes[j], r->nomes[j - 1], TAMANHO_MAX_NOME);
                    r->nomes[j][TAMANHO_MAX_NOME] = '\0'; // Garante terminação nula
                    r->pontuacao[j] = r->pontuacao[j - 1];
                }
                // Adicionar novo nome/pontuação ao ranking
                strncpy(r->nomes[i], p->nome, TAMANHO_MAX_NOME);
                r->nomes[i][TAMANHO_MAX_NOME] = '\0'; // Garante terminação nula
                r->pontuacao[i] = totalPontuacao;
                adicionado = 1; // Marca que foi adicionado/atualizado
                break;          // Sair do loop de inserção, pois já inserimos
            }
        }
    }
    return adicionado; // Retorna 1 se adicionado/atualizado, 0 caso contrário
}

// carrega dados do ranking antigo de um arquivo (load save)
int carregarRanking(Ranking *r, const char *nomeArquivo){
    char tempNome[TAMANHO_MAX_NOME +1];
    int tempPontuacao;
    int contLido=0;

    char caminho_completo[256]; // Tamanho suficiente para "data/" + nomeArquivo
    snprintf(caminho_completo, sizeof(caminho_completo), "data/%s", nomeArquivo);

    FILE *arquivo = fopen(caminho_completo, "r");
    inicializarRanking(r);
    
    if(arquivo == NULL){printf("Arquivo de ranking nao localizado!\n");return 1;}// retorna erro}

    while (contLido < TAMANHO_RANKING && fscanf(arquivo, "%15[^;];%d\n", tempNome, &tempPontuacao) == 2) {
        strncpy(r->nomes[contLido], tempNome, TAMANHO_MAX_NOME);
        r->nomes[contLido][TAMANHO_MAX_NOME] = '\0'; // garante que o nome termine com \0
        r->pontuacao[contLido] = tempPontuacao;

        contLido++;
    }

    fclose(arquivo);
    printf("Ranking carregado com sucesso, contendo %d entradas\n",contLido);

    return 0; // retorna sucesso
}

// exibe o ranking formatado na tela
void exibirRanking(const Ranking *r){
    limparTela();
    exibirLogo();

    printf("\n--- TOP %d RANKING ---\n",TAMANHO_RANKING);
    printf("Posicao | Personagem           | Pontuacao\n");
    printf("-----------------------------------------\n");
    for(int i=0;i<TAMANHO_RANKING;i++){
        if(strcmp(r->nomes[i],"Vazio")!=0 || r->pontuacao[i]!=0){
            printf("%7d | %-20s | %d\n",i+1,r->nomes[i],r->pontuacao[i]);
        } else {printf("%7d | %-20s |  %d\n",i+1,"------------",0);}
    }
    printf("-----------------------------------------\n");
    aguardarVoltar(1, "Voltar");
}

// salvar o ranking atual no arquivo
int salvarRanking(Ranking *r, const char *nomeArquivo){
    char caminho_completo[256]; // Tamanho suficiente para "data/" + nomeArquivo
    snprintf(caminho_completo, sizeof(caminho_completo), "data/%s", nomeArquivo);

    FILE *arquivo = fopen(caminho_completo,"w");
    
    if(arquivo == NULL){printf("Erro ao abrir arquivo de ranking!\n");return 1;}

    for(int i=0;i<TAMANHO_RANKING;i++){
        //printf("Escrevendo no ranking [%d]\n",i);
        //if(strcmp(r->nomes[i],"Vazio")!=0 || r->pontuacao[i]!=0){
            fprintf(arquivo, "%s;%d\n",r->nomes[i],r->pontuacao[i]);
        //}
    }
    fclose(arquivo);

    return 0;
}