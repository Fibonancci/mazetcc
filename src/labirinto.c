#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // --- NOVO --- Necessário para usar 'bool'
#include <time.h>    // --- NOVO --- Necessário para srand()
#include <termios.h>
#include <unistd.h>
#include <string.h>

// --- DEFINIÇÕES DO MAPA ---
#include "constantes.h" // Inclui as macros para as denifições do labirinto
#include "personagem.h" // Inclui a estrutura para Personagem
#include "desafio.h"    // Inclui a estrutura para ListaDesafios
#include "tela_desafio.h" // Inclui as funções para as telas e desafios
#include "salvar.h" // Inclui as funções para os saves

// Funções externas e uteis
extern void limparTela();
extern void exibirLogo();
extern void exibirInstrucaoJogo();
extern void aguardarVoltar(int opcaoSaida, const char *msgOpcao);

// --- VARIÁVEIS GLOBAIS ---
// Mantemos suas variáveis globais
int pontos = 0, linha = 1, coluna = 1;

// O mapa agora começa "em branco", ele será preenchido pela nova função.
int mapa[ALTURA][LARGURA];

// --- NOVO: FUNÇÃO RECURSIVA PARA "ESCULPIR" O LABIRINTO ---
// Esta função trabalha diretamente na variável global 'mapa'.
void esculpir_passagens(int cx, int cy) {
    int direcoes[4][2] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};
    int direcoes_parede[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    // Embaralha as direções para garantir aleatoriedade
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp[2];
        temp[0] = direcoes[i][0]; temp[1] = direcoes[i][1];
        direcoes[i][0] = direcoes[r][0]; direcoes[i][1] = direcoes[r][1];
        direcoes[r][0] = temp[0]; direcoes[r][1] = temp[1];

        temp[0] = direcoes_parede[i][0]; temp[1] = direcoes_parede[i][1];
        direcoes_parede[i][0] = direcoes_parede[r][0]; direcoes_parede[i][1] = direcoes_parede[r][1];
        direcoes_parede[r][0] = temp[0]; direcoes_parede[r][1] = temp[1];
    }

    // Tenta ir para cada direção
    for (int i = 0; i < 4; i++) {
        int nx = cx + direcoes[i][0];
        int ny = cy + direcoes[i][1];

        // Verifica se a nova posição é válida
        if (nx > 0 && nx < LARGURA - 1 && ny > 0 && ny < ALTURA - 1 && mapa[ny][nx] == parede) {
            mapa[cy + direcoes_parede[i][1]][cx + direcoes_parede[i][0]] = vazio;
            mapa[ny][nx] = vazio;
            esculpir_passagens(nx, ny);
        }
    }
}

// --- NOVO: FUNÇÃO PARA PREPARAR O MAPA ALEATÓRIO ---
void gerar_mapa_aleatorio() {
    // 1. Preenche o mapa inteiro com paredes
    for (int l = 0; l < ALTURA; l++) {
        for (int c = 0; c < LARGURA; c++) {
            mapa[l][c] = parede;
        }
    }

    // 2. Inicia o processo de cavar os caminhos a partir de (1,1)
    mapa[1][1] = vazio;
    esculpir_passagens(1, 1);

    // 3. Adiciona a saída em um local válido perto do fim
    for (int l = ALTURA - 2; l >= 1; l--) {
        if (mapa[l][LARGURA - 2] == vazio) {
            mapa[l][LARGURA - 2] = saida;
            break;
        }
    }

    // 4. Adiciona 5 comidas em posições aleatórias e válidas
    int comidas_geradas = 0;
    while (comidas_geradas < 5) {
        int l_rand = rand() % (ALTURA - 2) + 1;
        int c_rand = rand() % (LARGURA - 2) + 1;
        // Garante que não coloca comida no início, na saída ou onde já tem comida
        if (mapa[l_rand][c_rand] == vazio) {
            mapa[l_rand][c_rand] = comida;
            comidas_geradas++;
        }
    }
}

void desenhaBarraStatus(const char *atributo, Personagem *personagemAtual){
    int tamStatus = 20, statusMaximo = PONTUACAO_ATR; /* -- Pontuação máxima */
    int statusAtual = 0;
    int statusPersonagem;

    if (strcmp(atributo, "estamina") == 0) {
        statusAtual = personagemAtual->estamina;
    }
    if (strcmp(atributo, "reputacao") == 0) {
        statusAtual = personagemAtual->reputacao;
    }
    if (strcmp(atributo, "motivacao") == 0) {
        statusAtual = personagemAtual->motivacao;
    }
    if (strcmp(atributo, "sabedoria") == 0) {
        statusAtual = personagemAtual->sabedoria;
    }
    if (strcmp(atributo, "nota") == 0) {
        statusAtual = personagemAtual->nota;
    }
    
    statusPersonagem = (int)(((float)statusAtual/statusMaximo)*tamStatus);

    printf("[");
    for(int i = 0; i < statusPersonagem; i++){
        printf("=");    
    }
    for(int i = statusPersonagem; i<tamStatus; i++){
        printf(" ");
    }
    printf("] %d/%d\n", statusAtual,statusMaximo);

}

// Sua função de impressão permanece a mesma
void monta_mapa(Personagem *personagemAtual) {
    int l, c;
    for (l = 0; l < ALTURA; l++) {
        for (c = 0; c < LARGURA; c++) {
            if (l == linha && c == coluna) {
                printf(":)");
            } else {
                if (mapa[l][c] == vazio) printf("  ");
                // printf(ANSI_COLOR_YELLOW);
                if (mapa[l][c] == saida) printf("▓▓");
                if (mapa[l][c] == parede) printf("\u2588\u2588");
                if (mapa[l][c] == comida) printf(ANSI_COLOR_YELLOW"※ "ANSI_COLOR_GREEN);
            }
        }
        printf("\n");
    }
    printf("\nEstamina\n");
    desenhaBarraStatus("estamina",personagemAtual);

    printf("\nReputação\n");
    desenhaBarraStatus("reputacao",personagemAtual);

    printf("\nMotivação\n");
    desenhaBarraStatus("motivacao",personagemAtual);

    printf("\nSabedoria\n");
    desenhaBarraStatus("sabedoria",personagemAtual);
    // printf("\nPontos: %d\n\n", pontos);
    printf("\n1. Trancar Matricula\n");
    printf("2. Instrucoes de Jogo\n");
    //aguardarVoltar(1,"Trancar Matricula");
    // Tela do labirinto, adicionar o que for de informação do personagem
}

// Sua função de captura de tecla permanece a mesma
int my_getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void desafiarJogador(ListaDesafios *listaDeTodosDesafios, Personagem *personagemAtual){
    int idDesafioAleatorio = rand() % listaDeTodosDesafios->numDesafios;
    Desafio *desafioSelecionado = &(listaDeTodosDesafios->desafios[idDesafioAleatorio]);

    // Exibir a tela de aceitar/recusar
    exibirTelaDesafioAceitarRecusar(desafioSelecionado->id, desafioSelecionado->nome, desafioSelecionado->descricao);

    // A função lê o arquivo do desafio e retorna o índice da opção escolhida (0 para A, 1 para B, 2 para C)
    int opEscolha = exibirTelaInteracaoDesafio(desafioSelecionado->arquivoTela);

    if (opEscolha == -1) { // Jogador escolheu sair ou houve erro na interação
        printf("Voce decidiu nao interagir com o desafio ou a escolha foi invalida.\n");
        // Poderia aplicar um pequeno debuff por fugir, ou simplesmente retornar.
        return;
    }

    // Acessa a linha correta na matriz 'opcoes' usando o índice retornado
    int *modificadoresOp = desafioSelecionado->opcoes[opEscolha];

    // Aplica os buffs e debuffs da opção escolhida
    personagemAtual->estamina += modificadoresOp[0] - modificadoresOp[1];
    personagemAtual->reputacao += modificadoresOp[2] - modificadoresOp[3];
    personagemAtual->motivacao += modificadoresOp[4] - modificadoresOp[5];
    personagemAtual->sabedoria += modificadoresOp[6] - modificadoresOp[7];
    personagemAtual->nota += modificadoresOp[8] - modificadoresOp[9];

    // Remove notas negativas setando como 0
    if (personagemAtual->estamina < 0) personagemAtual->estamina = 0;
    if (personagemAtual->reputacao < 0) personagemAtual->reputacao = 0;
    if (personagemAtual->motivacao < 0) personagemAtual->motivacao = 0;
    if (personagemAtual->sabedoria < 0) personagemAtual->sabedoria = 0;
    if (personagemAtual->nota < 0) personagemAtual->nota = 0;

    // Exibir o resultado do desafio com os modificadores reais aplicados
    exibirTelaResultadoDesafio(1, "Sua escolha gerou os seguintes efeitos:", // Mensagem genérica
                                modificadoresOp[0] - modificadoresOp[1],
                                modificadoresOp[2] - modificadoresOp[3],
                                modificadoresOp[4] - modificadoresOp[5],
                                modificadoresOp[6] - modificadoresOp[7],
                                modificadoresOp[8] - modificadoresOp[9]);
}

int exibirLabirinto(ListaDesafios *listaDeTodosDesafios,Personagem *personagemAtual,SalvarJogo *saveCarregado) {
    int key, saiu_do_labirinto = 0;
    int venceu_jogo = 0; // Flag para indicar se o jogador se formou
    int personagem_morreu = 0;

    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    gerar_mapa_aleatorio(); // Gera o labirinto aleatório no nosso mapa global
    monta_mapa(personagemAtual);

    while (saiu_do_labirinto == 0) {
        key = my_getch();

        if (key == 'w' || key == 'W') {
            linha--;
            if (mapa[linha][coluna] == parede) linha++;
            if (mapa[linha][coluna] == comida) {
                pontos++;
                desafiarJogador(listaDeTodosDesafios,personagemAtual);
                if (verificarMortePersonagem(personagemAtual)) {
                    personagem_morreu = 1;
                    personagemAtual->estado_atual=1;
                    break; 
                }
                mapa[linha][coluna] = vazio;
            }
        }
        if (key == 'a' || key == 'A') {
            coluna--;
            if (mapa[linha][coluna] == parede) coluna++;
            if (mapa[linha][coluna] == comida) {
                pontos++;
                desafiarJogador(listaDeTodosDesafios,personagemAtual);
                if (verificarMortePersonagem(personagemAtual)) {
                    personagem_morreu = 1;
                    personagemAtual->estado_atual=1;
                    break; 
                }
                mapa[linha][coluna] = vazio;
            }
        }
        if (key == 's' || key == 'S') {
            linha++;
            if (mapa[linha][coluna] == parede) linha--;
            if (mapa[linha][coluna] == comida) {
                pontos++;
                desafiarJogador(listaDeTodosDesafios,personagemAtual);
                if (verificarMortePersonagem(personagemAtual)) {
                    personagem_morreu = 1;
                    personagemAtual->estado_atual=1;
                    break; 
                }
                mapa[linha][coluna] = vazio;
            }
        }
        if (key == 'd' || key == 'D') {
            coluna++;
            if (mapa[linha][coluna] == parede) coluna--;
            if (mapa[linha][coluna] == comida) {
                pontos++;
                desafiarJogador(listaDeTodosDesafios,personagemAtual);
                if (verificarMortePersonagem(personagemAtual)) {
                    personagem_morreu = 1;
                    personagemAtual->estado_atual=1;
                    break; 
                }
                mapa[linha][coluna] = vazio;
            }
        }
        if (key == '1') { // Opção de trancar matrícula (sair do labirinto)
            saiu_do_labirinto = 1;
            linha = 1; // Posição inicial
            coluna = 1;
            pontos = 0; // Zera pontos do labirinto atual
            gerar_mapa_aleatorio(); // Gera um NOVO labirinto
        }
        if (key == '2') { // Opção de acessar as intrucoes do jogo (sair/retornar do labirinto)
            exibirInstrucaoJogo();
        }

        // --- Lógica de Saída do Labirinto ---
        if (mapa[linha][coluna] == saida) {
            saiu_do_labirinto = 1; // Marca que o labirinto foi concluído

            linha = 1; // Posição inicial
            coluna = 1;
            pontos = 0; // Zera pontos do labirinto atual
            gerar_mapa_aleatorio(); // Gera um NOVO labirinto

            // Incrementa o contador de labirintos zerados do personagem
            personagemAtual->labirintos_zerados++;
            limparTela();
            exibirLogo();
            printf("\nParabens! Voce concluiu o labirinto #%d!\n", personagemAtual->labirintos_zerados);
            aguardarVoltar(1, "Continuar sua jornada...");

            // Verifica se o jogador se formou
            if (personagemAtual->labirintos_zerados >= LABIRINTOS_PARA_FORMACAO) {
                venceu_jogo = 1; // Marca que o jogador venceu o jogo
            } else {
                // Se não se formou, reinicia a posição no labirinto e gera um novo mapa
                saiu_do_labirinto = 0; // Permite que o loop continue para o próximo labirinto
            }
        }

        // Limpa a tela e redesenha o mapa apenas se o jogo não terminou
        if (saiu_do_labirinto == 0 && venceu_jogo == 0) {
            limparTela();
            monta_mapa(personagemAtual);
        }
    }

    if (personagem_morreu) {
        exibirTelaGameOver();
        aguardarVoltar(1, "Salvar Jogo");
    } else if (venceu_jogo) {
        exibirLogo();
        printf(ANSI_COLOR_YELLOW); // Cor amarela para o destaque
        printf("\n===============================================================\n");
        printf("     PARABÉNS! VOCÊ CONSEGUIU SE FORMAR NA UNIVERSIDADE!\n");
        printf("===============================================================\n");
        printf(ANSI_COLOR_RESET); // Volta para a cor padrão
        printf(" Você concluiu %d labirintos e é oficialmente um GRADUADO!\n", LABIRINTOS_PARA_FORMACAO);
        printf(" Sua pontuacao final: %d\n", personagemAtual->nota + personagemAtual->estamina + 
                personagemAtual->reputacao + personagemAtual->sabedoria + personagemAtual->motivacao); // Pontuação total do ranking
        printf(ANSI_COLOR_GREEN);
	aguardarVoltar(1, "Salvar Jogo");
    } else { // Se saiu porque trancou matrícula
        limparTela();
        printf("\n------- Matricula Trancada -------\n");
        printf("Voce trancou sua matricula!\n");
        printf("Volte para casa e estude mais um pouco.\n");
        printf("----------------------------------\n");

        aguardarVoltar(1, "Salvar Jogo");
    }

    saveCarregado->perfil = *personagemAtual; // Atribui o personagem criado ao save
    salvarSave(saveCarregado);

    printf(ANSI_COLOR_GREEN);
    aguardarVoltar(1, "Menu Principal");

    return 0; // Retorna para o menu principal
}
