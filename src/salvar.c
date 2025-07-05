#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Habilita isdigit

// --- DEFINIÇÕES DO MAPA ---
#include "constantes.h" // Inclui as macros para as denifições do labirinto
#include "salvar.h" //
#include "personagem.h" //

// Funções utilitárias definidas em outro arquivo
extern void aguardarVoltar(int opcaoSaida, const char *msgOpcao); //
extern void limparTela(); // Adicione esta linha
extern void exibirLogo(); // Adicione esta linha

// Retorna 1 se o Salvar for vazio, 0) caso contrário.
// ESTA É A ÚNICA E CORRETA FUNÇÃO checarSave
int checarSave(SalvarJogo s) { //
    // Um save é considerado vazio se o nome do personagem dentro dele é "Vazio" ou uma string vazia
    if (strcmp(s.perfil.nome, "Vazio") == 0 || strlen(s.perfil.nome) == 0) {
        return 1; // É vazio (verdadeiro)
    }
    return 0; // Não é vazio (falso)
}

// Função para salvar um personagem em um slot
int salvarSave(SalvarJogo *s) { //
    SalvarJogo espacos[TAMANHO_RANKING]; // Array para armazenar os slots de salvamento
    FILE *file; //
    int slot; //
    
    limparTela(); //
    exibirLogo(); //

    //Abre o arquivo para leitura e escrita binária (rb+)
    file = fopen("data/salvar.bin", "rb+"); //

    //Se o arquivo não existir, ele será criado vazio
    if (file == NULL) { //
        file = fopen("data/salvar.bin", "wb"); //
        if (file == NULL) { //
            perror("Erro ao criar o arquivo para salvar"); //
            return 0; // Retorna 0 para indicar erro
        }
        for (int i = 0; i < TAMANHO_RANKING; i++) { //
            strcpy(espacos[i].perfil.nome, "Vazio"); //
            espacos[i].perfil.estamina = 0; //
            espacos[i].perfil.reputacao = 0; //
            espacos[i].perfil.motivacao = 0; //
            espacos[i].perfil.sabedoria = 0; //
            espacos[i].perfil.nota = 0; //
        }
        fwrite(espacos, sizeof(SalvarJogo), TAMANHO_RANKING, file); //
        fclose(file); //

        file = fopen("data/salvar.bin", "rb+"); //
        if (file == NULL) { //
            perror("\nErro ao reabrir o arquivo para salvar"); //
            return 0; // Retorna 0 para indicar erro
        }
    } else {
        // Lê os dados existentes do arquivo para o array 'espacos'
        fread(espacos, sizeof(SalvarJogo), TAMANHO_RANKING, file); //
    }

    printf("\nEm qual slot deseja salvar? (1-%d):\n", TAMANHO_RANKING); //
    for (int i = 0; i < TAMANHO_RANKING; i++) { //
        printf("%d. %s\n", i+1, espacos[i].perfil.nome); //
    }
    printf("\nEscolha o slot: "); //
    scanf("%d", &slot); //
    while (getchar() != '\n'); // Limpa o buffer do teclado

    if (slot < 1 || slot > TAMANHO_RANKING) { //
        printf("Slot inválido. Escolha um número entre 1 e %d.\n", TAMANHO_RANKING); //
        fclose(file); //
        return 0; // Retorna 0 para indicar erro
    }
    slot-=1; // Ajusta o slot para o índice do array (0-4)

    espacos[slot] = *s; // Atualiza o slot selecionado com os dados do personagem
    fseek(file, 0, SEEK_SET); // Move o ponteiro do arquivo para o início
    fwrite(espacos, sizeof(SalvarJogo), TAMANHO_RANKING, file); //

    fclose(file); //
    printf("Personagem salvo com sucesso no slot %d!\n", slot + 1); //
    return 1; // Retorna 1 para indicar sucesso
}

// ESTA É A FUNÇÃO PRINCIPAL DE CARREGAMENTO (sem argumentos, retorna SalvarJogo)
SalvarJogo carregarSave() { //
    SalvarJogo espacos[TAMANHO_RANKING]; //
    FILE *file; //
    int slot; //
    SalvarJogo personagem_carregado; //
    
    limparTela(); //
    exibirLogo(); //

    // Inicializa o personagem carregado como vazio
    strcpy(personagem_carregado.perfil.nome, ""); //
   
    // Abre o arquivo para leitura binária
    file = fopen("data/salvar.bin", "rb"); //
    if (file == NULL) { //
        perror("Erro ao abrir o arquivo para carregar. Nenhum save encontrado."); //
        return personagem_carregado; // Retorna um personagem "vazio"
    }

    fread(espacos, sizeof(SalvarJogo), TAMANHO_RANKING, file); //

    printf("\nDe qual slot deseja carregar? (1-%d):\n", TAMANHO_RANKING); //
    for (int i = 0; i < TAMANHO_RANKING; i++) { //
        printf("%d. %s\n", i + 1, espacos[i].perfil.nome); //
    }
    printf("Escolha o slot: "); //
    scanf("%d", &slot); //
    while (getchar() != '\n'); // Limpa o buffer do teclado

    if (slot < 1 || slot > TAMANHO_RANKING) { //
        printf("Slot inválido.\n"); //
        fclose(file); //
        return personagem_carregado; // Retorna um personagem "vazio"
    }
    slot-=1; //

    // CORREÇÃO AQUI: Usa a função 'checarSave' (que existe e está correta)
    if (checarSave(espacos[slot]) == 1) { // Se retornar 1, significa que o slot é vazio
        printf("Slot vazio. Nenhum personagem para carregar.\n"); //
        fclose(file); //
        return personagem_carregado; // Retorna um personagem "vazio"
    }

    fclose(file); //
    printf("Personagem carregado com sucesso do slot %d!\n", slot + 1); //
    return espacos[slot]; // Retorna o personagem carregado do slot selecionado
}