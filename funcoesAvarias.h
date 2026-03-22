#ifndef FUNCOESAVARIAS_H_INCLUDED
#define FUNCOESAVARIAS_H_INCLUDED

#include "constantes.h"
#include "estruturas.h"
#include "funcoesLeitura.h"

tipoAvaria *registarAvaria(tipoAvaria *vAvarias,tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int totalRequisicoes,
                           int *quantPortateisDisp,int quantPortateis,int *quantAvarias,int *quantRequisicoesAtivas, int *quantPortateisAvariados);

int procuraAvaria(tipoAvaria *vAvarias, int quantAvarias,int numero);

void registarReparacao(tipoAvaria *vAvarias,tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis,int quantAvarias,
                       int *quantPortateisDisp,int *quantPortateisAvariados);

void listaAvarias(tipoAvaria *vAvarias,tipoPortatil vPortateis[MAX_PORTATEIS],int quantAvarias,int quantPortateis);

void gravaFicheiroBinarioA(tipoAvaria *vAvarias,int quantAvarias);

tipoAvaria *leFicheiroBinarioA(tipoAvaria *vAvarias,int *quantAvarias);

#endif // FUNCOESAVARIAS_H_INCLUDED
