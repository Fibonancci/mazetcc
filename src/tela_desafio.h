#ifndef DESAFIO_TELA_H_INCLUDED
#define DESAFIO_TELA_H_INCLUDED

#include "personagem.h" // Se as telas precisarem exibir info do personagem
#include "constantes.h" // Para macros globais como limparTela, exibirLogo

// Declaração da função de exibir telas de sucesso/falha
// Agora inclui parâmetros para modificadores de atributos
void exibirTelaResultadoDesafio(int sucesso, const char *mensagemAdicional,
                                int estamina_mod, int reputacao_mod,
                                int motivacao_mod, int sabedoria_mod, int nota_mod);
void exibirTelaDesafioAceitarRecusar(int id_desafio, const char *nome_desafio, const char *descricao_desafio);
int exibirTelaInteracaoDesafio(const char *nome_arquivo_tela);
void exibirTelaGameOver();

#endif // DESAFIO_TELA_H_INCLUDED