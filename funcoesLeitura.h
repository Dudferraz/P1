#ifndef FUNCOESLEITURA_H_INCLUDED
#define FUNCOESLEITURA_H_INCLUDED

#include "constantes.h"
#include "estruturas.h"



int lerInteiro(char mensagem[MAX_STRING], int minimo, int maximo);

float lerFloat(char mensagem[MAX_STRING], float minimo, float maximo);

void lerString(char mensagem[MAX_STRING], char vetorCaracteres[], int maximoCaracteres);

tipoData lerData(void);

int lerProcessador_Ou_Memoria(char mensagem[MAX_STRING]);

void limpaBufferStdin(void);

int contaDias_Entre_Datas(tipoData dataA,tipoData dataB);

int converteDataDias(tipoData data);


#endif // FUNCOESLEITURA_H_INCLUDED

