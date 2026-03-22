#ifndef FUNCOESREQUISICOES_H_INCLUDED
#define FUNCOESREQUISICOES_H_INCLUDED

#include "constantes.h"
#include "estruturas.h"
#include "funcoesLeitura.h"



tipoRequisicao *registarRequisicao(tipoRequisicao *vRequisicoes,tipoAvaria *vAvarias,int quantAvarias, int *totalRequisicoes,int *quantRequisicoesAtivas,
                                   tipoPortatil vPortateis[MAX_PORTATEIS], int quantPortateis,int *quantPortateisDisp);

int procuraRequisicao(tipoRequisicao *vRequisicoes, int totalRequisicoes, char codigoRequisicao[]);

int procuraRequisicao_Por_Numero(tipoRequisicao *vRequisicoes, int totalRequisicoes,int numero);

void mostraRequisicoes(tipoRequisicao *vRequisicoes, int totalRequisicoes);

void registarDevolucao(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int totalRequisicoes,int quantPortateis,
                       int *quantPortateisDisp,int *quantRequisicoesAtivas);

void gravaFicheiroLog(tipoRequisicao devolucao,tipoPortatil vPortatil);

void renovaRequisicao(tipoRequisicao *vRequisicoes,int totalRequisicoes);



void informacaoRequisicao (tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int totalRequisicoes,int quantPortateis);




#endif // FUNCOESREQUISICOES_H_INCLUDED
