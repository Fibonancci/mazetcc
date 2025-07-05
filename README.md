# Projeto MazeTCC

Este README.md contém as instruções essenciais para a organização do projeto, compilação e execução do jogo MazeTCC. Por favor, leia atentamente para garantir a consistência no desenvolvimento.

---

## 1. Estrutura de Pastas do Projeto

Para manter o projeto organizado e garantir que os caminhos dos arquivos funcionem corretamente, adotamos a seguinte estrutura de diretórios:

**Ponto Chave:** O executável compilado (`mazeTcc_x.x.x`) deve ser gerado no diretório **raiz do projeto** (`projeto/`), ao lado da pasta `data/`. Os caminhos dos arquivos de dados (como `data/salvar.bin` ou `data/desafios/desafio1_tela.txt`) são definidos internamente no código, facilitando o gerenciamento.

---

## 2. Instruções de Compilação

Para compilar o projeto, siga estes passos:

1.  **Abra o Terminal:** Navegue até o diretório **raiz do projeto** (`projeto/`).
    ```bash
    cd /caminho/para/projeto/
    ```
    (Substitua `/caminho/para/projeto/` pelo caminho real do seu diretório.)

2.  **Comando de Compilação:** Use o `gcc` para compilar os arquivos-fonte.

    * **Opção A: Compilar TODOS os arquivos `.c` no `src/` (Recomendado para versões estáveis):**
        ```bash
        gcc src/*.c -o mazeTcc
        ```
        Este comando instrui o `gcc` a compilar todos os arquivos com a extensão `.c` encontrados dentro da pasta `src/` e gerar um executável chamado `mazeTcc_0.5` no diretório atual.

    * **Opção B: Compilar APENAS alguns arquivos `.c` (Para desenvolvimento/exclusão temporária):**
        Se você estiver trabalhando em arquivos específicos (por exemplo, `desafio.c` e `desafio.h`) e eles estiverem incompletos ou causando erros, você pode excluí-los da compilação temporariamente.
        **Exemplo:** Se `desafio.c` e `desafio.h` estão em edição e você não quer compilá-los:
        ```bash
        gcc src/mazetcc_0.5.c src/personagem.c src/ranking.c src/salvar.c src/labirinto.c -o mazeTcc_0.5
        ```
        **Atenção:** Ao usar esta opção, certifique-se de que os arquivos que você **está** compilando **não dependam** das funções ou estruturas dos arquivos que você **não está** compilando. Se houver dependências (como `#include "desafio.h"` ou chamadas a funções de `desafio.c`), você precisará:
        * **Comentar temporariamente** a linha `#include "desafio.h"` em `mazetcc_0.5.c`.
        * **Comentar temporariamente** as chamadas de função (`carregarTodosDesafios`, `buscarDesafioPorId`, `exibirTelaDesafio`, etc.) no `mazetcc_0.5.c` (especialmente a seção de exemplo dentro do `main` que usa desafios).

---

## 3. Instruções de Execução

Após a compilação bem-sucedida, siga estes passos para executar o jogo:

1.  **Crie a Pasta `data/` (se não existir):**
    No terminal, no diretório **raiz do seu projeto**:
    ```bash
    mkdir -p data
    ```
    Isso garante que a pasta `data/` exista. O `mkdir -p` cria as pastas pai se elas não existirem, então é seguro usar.

2.  **Crie a Subpasta `data/desafios/` (se não existir):**
    ```bash
    mkdir -p data/desafios
    ```
    Esta pasta é essencial para armazenar os arquivos de tela dos desafios.

3.  **Posicione os Arquivos de Dados:**
    * Coloque `desafios.txt`, `ranking.txt` e `salvar.bin` (se já existirem ou forem gerados) dentro da pasta `data/`.
    * Coloque `desafio1_tela.txt`, `desafio2_tela.txt`, etc., dentro da pasta `data/desafios/`.

4.  **Execute o Programa:** No terminal, no diretório **raiz do seu projeto**:
    ```bash
    ./mazeTcc_x.x.x
    ```

---

## 4. Gerenciamento de Arquivos de Dados

Os caminhos para os arquivos de dados são gerenciados internamente no código:

* **`desafios.txt`**: Contém a lista principal de todos os desafios, seus IDs, nomes dos arquivos de tela e modificadores de atributos. É carregado de `data/desafios.txt`.
    * **Formato de exemplo:**
        ```
        id;nome_desafio;descricao_desafio;est_bf;est_df;rep_bf;rep_df;motiv_bf;motiv_df;sabe_bf;sabe_df;nota_bf;nota_df

        1;A Floresta Sombria;desafio1_tela.txt;Uma floresta misteriosa que suga sua energia e desmotiva.;0;10;0;0;0;5;0;0;0;0
        2;O Oraculo Sabio;desafio2_tela.txt;Um antigo oraculo oferece sabedoria em troca de um bom esforco.;0;0;5;0;0;0;10;0;0;0
        3;A Feira de Reputacao;desafio3_tela.txt;Uma feira local onde voce pode ganhar ou perder reputacao rapidamente.;0;0;15;5;0;0;0;0;0;0
        ```
* **Arquivos de Tela de Desafio (`.txt` em `data/desafios/`)**: Contêm o texto detalhado que será exibido na tela para cada desafio específico. A função `exibirTelaDesafio` constrói o caminho completo `data/desafios/NOME_DO_ARQUIVO.txt`.
* **`ranking.txt`**: Armazena os dados do ranking. As funções `salvarRanking` e `carregarRanking` esperam encontrá-lo ou criá-lo em `data/ranking.txt`.
* **`salvar.bin`**: Armazena os dados dos jogos salvos. As funções `salvarSave` e `carregarSave` esperam encontrá-lo ou criá-lo em `data/salvar.bin`.

---

## 5. Estruturas de Dados Chave

### `Personagem` (em `personagem.h`)
Define os atributos de um personagem:
```c
typedef struct {
    char nome[TAMANHO_MAX_NOME +1];
    int estamina;
    int reputacao;
    int motivacao;
    int sabedoria;
    int nota;
} Personagem;