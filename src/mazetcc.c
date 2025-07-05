#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Habilita isdigit
#include <unistd.h>
#include <time.h>

#include "constantes.h"
#include "ranking.h"
#include "personagem.h"
#include "salvar.h"
#include "desafio.h"
#include "tela_desafio.h"
#include "labirinto.h"

// Funções Utilitárias
void limparBufferEntrada();
int verificarInteiro(int *valor, int min_val, int max_val);
void exibirInstrucaoJogo();
void exibirIntruducaoDoJogo();

// Funções de Menu/Telas
void aguardarVoltar(int opcaoSaida, const char *msgOpcao);
int exibirCreditos();
void exibirLogo();
void limparTela();
void exibirMenu(Ranking *r, Personagem *personagemAtual, SalvarJogo *saveCarregado, ListaDesafios *listaDeTodosDesafios); // Declaração da função principal do menu

// --- Definição das Funções Utilitárias ---

// função auxiliar para limpar o buffer de entrada (após scanf)
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printLento(const char *texto, int tempo) {
    for (int i = 0; texto[i] != '\0'; i++) {
        printf("%c", texto[i]);
        fflush(stdout); // Garante que o caractere seja exibido imediatamente

        // Pausa o programa pelo tempo especificado
        #ifdef _WIN32
            Sleep(tempo); // Sleep em milissegundos
        #else
            usleep(tempo * 1000); // usleep em microssegundos
        #endif
    }
}

void exibirInstrucaoJogo(){
    limparTela();
    printf("\n------ Movimentacao ------\n");
    printf(" W - Mover para Cima\n");
    printf(" A - Mover para a Esquerda\n");
    printf(" S - Mover para Baixo\n");
    printf(" D - Mover para a Direita\n");
    printf("\n------- Labirinto -------\n");
    printf(" :) - Personagem\n");
    printf(ANSI_COLOR_YELLOW" ※ "ANSI_COLOR_GREEN);
    printf("- Desafio\n");
    printf(" ▓▓ - Saida\n");
    printf("\n-------------------------\n");
    aguardarVoltar(1, "Menu Principal");
}

void pausa(int tempo) {
    #ifdef _WIN32
        Sleep(tempo);
    #else
        usleep(tempo * 1000); // usleep em microssegundos
    #endif
}

void exibirIntruducaoDoJogo(){
    printf(ANSI_COLOR_RESET);

    printf("                                                                \n");
    pausa(100);
    printf("          ███╗   ███╗ █████╗ ███████╗███████╗                 \n");
    pausa(100);
    printf("          ████╗ ████║██╔══██╗╚══███╔╝██╔════╝                 \n");
    pausa(100);
    printf("          ██╔████╔██║███████║  ███╔╝ █████╗                   \n");
    pausa(100);
    printf("          ██║╚██╔╝██║██╔══██║ ███╔╝  ██╔══╝                   \n");
    pausa(100);
    printf("          ██║ ╚═╝ ██║██║  ██║███████╗███████╗                 \n");
    pausa(100);
    printf("          ╚═╝     ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝                 \n");
    pausa(100);
    printf("                                                              \n");
    pausa(100);
    printf("               ████████╗ ██████╗ ██████╗                     \n");
    pausa(100);
    printf("               ╚══██╔══╝██╔════╝██╔════╝                     \n");
    pausa(100);
    printf("                  ██║   ██║     ██║                         \n");
    pausa(100);
    printf("                  ██║   ██║     ██║                         \n");
    pausa(100);
    printf("                  ██║   ╚██████╗╚██████╗                     \n");
    pausa(100);
    printf("                  ╚═╝    ╚═════╝ ╚═════╝                     \n");

    const char *paragrafo1 = "--------------------------------------------------------\n";
    const char *paragrafo2 = "   Voce sobreviveu ao Ensino Medio. Acha que ja zerou a vida?\n";
    const char *paragrafo3 = "   Pense de novo. Você acaba de ser convocado para a arena mais\n   desafiadora, imprevisivel e gloriosa de todas:\n";
    const char *paragrafo4 = "   FACULDADE!\n\n";
    const char *paragrafo5 = "   Embarque em uma aventura unica! Desafios acadêmicos testarao\n   sua resiliencia. \n\n";
    const char *paragrafo6 = "   Um calouro de faculdade descobrira as aventuras do mundo\n   acadêmico, com desafios diários como:\n";
    const char *paragrafo7 = "   Comer em um restaurante universitario meio suspeito, interagindo\n   com os colegas e passando pelos obstaculos criados pelos \n";
    const char *paragrafo8 = "   seus professores.\n\n";
    const char *paragrafo9 = "   Crie seu personagem, defina sua trajetoria e supere obstáculos\n   com astucia e bravura.\n\n";
    const char *paragrafo10 = "   O sucesso do seu TCC está em suas maos.\n";
    const char *paragrafo11 = "   Prepare-se para marcar sua historia na graduação!\n";
    const char *paragrafo12 = "--------------------------------------------------------\n";

    printLento(paragrafo1, 5);
    printLento(paragrafo2, 10);
    printLento(paragrafo3, 10);
    printLento(paragrafo4, 10);
    printLento(paragrafo5, 10);
    printLento(paragrafo6, 10);
    printLento(paragrafo7, 10);
    printLento(paragrafo8, 10);
    printLento(paragrafo9, 10);
    printLento(paragrafo10, 10);
    printLento(paragrafo11, 10);
    printLento(paragrafo12, 5);

    aguardarVoltar(1," Menu do Jogo");
    printf(ANSI_COLOR_GREEN);
}   

void exibirLogo(){
    printf("                                                                \n");
    printf("          ███╗   ███╗ █████╗ ███████╗███████╗                 \n");
    printf("          ████╗ ████║██╔══██╗╚══███╔╝██╔════╝                 \n");
    printf("          ██╔████╔██║███████║  ███╔╝ █████╗                   \n");
    printf("          ██║╚██╔╝██║██╔══██║ ███╔╝  ██╔══╝                   \n");
    printf("          ██║ ╚═╝ ██║██║  ██║███████╗███████╗                 \n");
    printf("          ╚═╝     ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝                 \n");
    printf("                                                              \n");
    printf("               ████████╗ ██████╗ ██████╗                     \n");
    printf("               ╚══██╔══╝██╔════╝██╔════╝                     \n");
    printf("                  ██║   ██║     ██║                         \n");
    printf("                  ██║   ██║     ██║                         \n");
    printf("                  ██║   ╚██████╗╚██████╗                     \n");
    printf("                  ╚═╝    ╚═════╝ ╚═════╝                     \n");
    printf("                                                             \n");

}

void aguardarVoltar(int opcaoSaida, const char *msgOpcao) {
    int escolha;
    int leitura_sucesso;

    printf("\n%d. %s\n",opcaoSaida,msgOpcao);

    do {
        leitura_sucesso = scanf(" %d", &escolha);
        limparBufferEntrada();

        if (leitura_sucesso != 1) {
            escolha = 0; // Garante que o loop continue
        }
    } while (escolha != opcaoSaida);
    limparTela();
}

int exibirCreditos(){
    limparTela();
    char line[256];
    FILE *file = fopen("data/creditos.txt", "r");

    // Verifica se o arquivo foi aberto com sucesso
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    // Fecha o arquivo
    fclose(file);

    aguardarVoltar(1, "Voltar");
    return 0;
}

void limparTela() {
    #ifdef _WIN32 // Para sistemas Windows
        system("cls");
    #else // Para sistemas Unix (Linux, macOS)
        system("clear");
    #endif
}

void exibirMenu(Ranking *rankingDoJogo, Personagem *personagemAtual, 
                SalvarJogo *saveCarregado, ListaDesafios *listaDeTodosDesafios){
    int escolha;

    limparTela();
    exibirIntruducaoDoJogo();

    limparTela();
    do{
        exibirLogo();

        printf("\n--- MENU DO JOGO ---\n");
        printf("1. Iniciar Novo Jogo\n");
        printf("2. Carregar Jogo Salvo\n");
        printf("3. Instrucoes\n");
        printf("4. Ranking\n");
        printf("5. Créditos\n");
        printf("6. Sair\n");
        printf("--------------------\n");
        printf("Digite sua escolha: ");
        scanf(" %d", &escolha);
        limparBufferEntrada();

        switch(escolha){
            case 1:
                printf("Iniciando um novo jogo...\n");
                if (criarPersonagem(personagemAtual)){
                    exibirPersonagem(personagemAtual);
                    
                    if (personagemAtual->labirintos_zerados >= LABIRINTOS_PARA_FORMACAO || personagemAtual->estado_atual ==2) {
                        limparTela();
                        exibirLogo();
                        printf("\nParabens! Voce ja se formou!\n");
                        aguardarVoltar(1, "Menu Principal");
                    } else if (personagemAtual->estado_atual ==1) {
                        limparTela();
                        exibirLogo();
                        printf("\nEssa não, você foi jubilado! Iicie um novo game.\n");
                        aguardarVoltar(1, "Menu Principal");
                    } else {
                        exibirLabirinto(listaDeTodosDesafios,personagemAtual,saveCarregado);
                    }
                } else {
                    printf("Erro ao criar o personagem.\n");
                }
                adicionarAoRanking(rankingDoJogo,personagemAtual);
                salvarRanking(rankingDoJogo,"ranking.txt");
                limparTela();
                break;
            case 2:
                *saveCarregado = carregarSave();
                if (checarSave(*saveCarregado) == 0) { // Se o save não for vazio
                    *personagemAtual = saveCarregado->perfil;
                    exibirPersonagem(personagemAtual);

                    if (personagemAtual->labirintos_zerados >= LABIRINTOS_PARA_FORMACAO || personagemAtual->estado_atual ==2) {
                        limparTela();
                        exibirLogo();
                        printf("\nParabens! Voce ja se formou!\n");
                        aguardarVoltar(1, "Menu Principal");
                    } else if (personagemAtual->estado_atual ==1) {
                        limparTela();
                        exibirLogo();
                        printf("\nEssa não, você foi jubilado! Iicie um novo game.\n");
                        aguardarVoltar(1, "Menu Principal");
                    } else {
                        exibirLabirinto(listaDeTodosDesafios,personagemAtual,saveCarregado);
                    }

                } else {
                    printf("Nenhum personagem encontrado ou o slot estava vazio.\n");
                }
                // Aqui você pode adicionar lógica para continuar o jogo com o personagem carregado
                limparTela();
                break;
            case 3:
                exibirInstrucaoJogo();
                break;
            case 4:
                //salvarRanking(&r,"ranking.txt");
                carregarRanking(rankingDoJogo,"ranking.txt");
                exibirRanking(rankingDoJogo);
                break;
            case 5:
                exibirCreditos();
                break;
            case 6:
                printf("Fianlizando o game... Bye bye o/\n");
                break;
            default: 
                printf("Opção inválida. Por favor, digite um número entre 1 e 5.\n");
                break;
        }
        limparTela();
    } while(escolha != 6);
}

// Função para verificar se a string é um número inteiro válido, 1 para válido, 0 para inválido
// Essa funçaõ recebe uma string, um ponteiro para o valor inteiro a ser preenchido, e os valores mínimo e máximo permitidos
int verificarInteiro(int *valor, int min_val, int max_val){
    char input[20];

    //1. verifica se está vazio
    if (fgets(input, sizeof(input), stdin) == NULL){
        return 0;
    }

    //2. retira residuo
    input[strcspn(input, "\n")] = '\0';

    if (input[0] == '\0'){
        printf("Valor vazio, digite um numero inteiro. \n");
    return 0;
    }

    for (int i = 0; input[i] != '\0'; i++){
        if (!isdigit(input[i])){
            printf("Valor invalido, digite apenas numeros inteiro. \n");
        return 0;
        }
    }

    //Converte string para inteiro
    *valor = atoi(input);
    if (*valor < min_val || *valor > max_val){
        printf("Valor Invalido, o valor deve estar entre %d e %d. Digite novamente. \n", min_val, max_val);
        return 0;
    }
    //sucesso
    return 1; 
}

// --- Função Principal (main) ---

int main(){

    srand(time(NULL));
    // --- 1. Preparar Estruturas de Dados ---
    ListaDesafios listaDeTodosDesafios; // Para carregar os desafios do arquivo
    Ranking rankingDoJogo;             // Para carregar/gerenciar o ranking
    SalvarJogo saveCarregado;              // Para armazenar o save do jogador
    Personagem personagemAtual;        // O personagem que o jogador irá usar

    // Inicializa as estruturas que precisam de inicialização
    inicializarListaDesafios(&listaDeTodosDesafios);
    inicializarRanking(&rankingDoJogo);

    // Carrega as estruturas ja inicializadas
    if (carregarTodosDesafios(&listaDeTodosDesafios, "desafios.txt") != 0) {
    // Se a função retornar diferente de 0 (ou seja, falha), exibe o erro e sai.
        fprintf(stderr, "ERRO CRÍTICO: Não foi possível carregar os desafios. Encerrando o jogo.\\n");
        exit(EXIT_FAILURE);
    }
    if (carregarRanking(&rankingDoJogo,"ranking.txt") != 0) {
        fprintf(stderr, "ERRO CRÍTICO: Não foi possível carregar o ranking. Encerrando o jogo.\\n");
        exit(EXIT_FAILURE);
    }

    // Colore as letras do terminal
    printf(ANSI_COLOR_GREEN);

    exibirMenu(&rankingDoJogo,&personagemAtual,&saveCarregado,&listaDeTodosDesafios);

    // Volta a cor do texto para o padrão
    printf(ANSI_COLOR_RESET);

    return 0;
}
