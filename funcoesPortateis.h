#ifndef FUNCOESPORTATEIS_H_INCLUDED
#define FUNCOESPORTATEIS_H_INCLUDED

#include "constantes.h"
#include "estruturas.h"
#include "funcoesLeitura.h"



int inserePortateis(tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis, int *quantPortateisDisp);

tipoPortatil leDadosPortatil(tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis);

int procuraPortatil(tipoPortatil vPortateis[MAX_PORTATEIS], int quantPortateis, int numero);

void mostraDadosPortateis(tipoRequisicao *vRequisicoes,tipoPortatil vPortateis[MAX_PORTATEIS],int totalRequisicoes,int quantPortateis);

void alteraLocalizacao(tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis);

void gravaFicheiroBinarioPR(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int quantPortateis,
                          int quantPortateisDisp,int quantRequisicoesAtivas,int totalRequisicoes,int quantPortateisAvariados);

tipoRequisicao *leFicheiroBinarioPR(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int *quantPortateis,
                          int *quantPortateisDisp,int *quantRequisicoesAtivas,int *totalRequisicoes,int *quantPortateisAvariados);

void apresentaDadosEstatisticos(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int quantPortateis,
                                int totalRequisicoes,int quantRequisicoesAtivas);




#endif // FUNCOESPORTATEIS_H_INCLUDED
