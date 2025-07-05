#ifndef LABIRINTO_H_INCLUDED
#define LABIRINTO_H_INCLUDED

#include "constantes.h" // Inclui as macros de constantes
#include "personagem.h" // Inclui a estrutura para Personagem
#include "desafio.h"    // Inclui a estrutura para ListaDesafios
#include "tela_desafio.h" // Inclui as funções para as telas e desafios
#include "salvar.h" // Inclui as funções para os saves

// Funções de Labirinto
void esculpir_passagens(int cx, int cy);
void gerar_mapa_aleatorio();
void monta_mapa(Personagem *personagemAtual);
int my_getch(void);
int exibirLabirinto(ListaDesafios *listaDeTodosDesafios,Personagem *personagemAtual,
					SalvarJogo *saveCarregado);

// Funções do minigame
void desafiarJogador(ListaDesafios *listaDeTodosDesafios,Personagem *personagemAtual);
void desenhaBarraStatus(const char *atributo, Personagem *personagemAtual);

#endif