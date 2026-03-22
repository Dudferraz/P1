#include <stdio.h>
#include <string.h>



#include "funcoesLeitura.h"

// Função lerInteiro fornecida
int lerInteiro(char mensagem[MAX_STRING], int minimo, int maximo)
{
    int numero, controlo;
    do{
        printf(" %s (%d a %d): ", mensagem, minimo, maximo);
        controlo = scanf ("%d", &numero);
        limpaBufferStdin();

        if (controlo == 0){
            printf(" Devera inserir um numero inteiro! \n");
        }
        else{

            if(numero<minimo || numero>maximo){
                printf(" Numero invalido!\n");
            }
        }
    }while(numero<minimo || numero>maximo || controlo ==0);

    return numero;
}

// Função lerFloat fornecida
float lerFloat(char mensagem[MAX_STRING], float minimo, float maximo)
{
    float numero;
    int controlo;
    do{
        printf(" %s (%.2f a %.2f): ", mensagem, minimo, maximo);
        controlo = scanf ("%f", &numero);
        limpaBufferStdin();

        if (controlo == 0){
            printf(" Devera inserir um numero decimal (float)! \n");
        }
        else{

            if(numero<minimo || numero>maximo){
                printf(" Numero invalido!\n");
            }
        }
    }while(numero<minimo || numero>maximo || controlo ==0);

    return numero;
}

// Função lerString fornecida
void lerString(char mensagem[MAX_STRING], char vetorCaracteres[], int maximoCaracteres)
{
    int tamanhoString;
    do{
        printf(" %s", mensagem);
        fgets(vetorCaracteres, maximoCaracteres, stdin);
        tamanhoString = strlen(vetorCaracteres);

        if (tamanhoString == 1){
            printf(" Nao foram introduzidos caracteres! Apenas carregou no ENTER!\n");
        }
    }while (tamanhoString == 1);

    if(vetorCaracteres[tamanhoString-1] != '\n'){
        limpaBufferStdin();
    }
    else{
        vetorCaracteres[tamanhoString-1] = '\0';
    }
}

// Função para ler uma data (Ficha 6)
tipoData lerData()
{
    tipoData data;
    int max_dias;

    data.ano = lerInteiro("\tAno -> ",MENOR_ANO,MAIOR_ANO);
    data.mes = lerInteiro("\tMes -> ",1,12);

    switch (data.mes){
    case 2:
        if ((data.ano%400==0) || (data.ano%4==0 && data.ano%100!=0)){ //ano é bissexto
            max_dias = 29;
        }
        else{
            max_dias = 28;
        }

        break;
    case 4:
    case 6:
    case 9:
    case 11:
        max_dias = 30;
        break;
    default:
        max_dias = 31;
    }

    data.dia = lerInteiro("\tDia -> ",1,max_dias);

    return data;
}

// Função limpaBufferStdin fornecida
void limpaBufferStdin(void)
{
    char chr;
    do{
        chr = getchar();
    }while (chr != '\n' && chr != EOF);
}

// Função para ler nº de processador ou memoria
int lerProcessador_Ou_Memoria(char mensagem[MAX_STRING]) // lerInteiro mas sem parametros Min e Max
{
    int numero, controlo;
    do{
        printf(" %s", mensagem);
        controlo = scanf ("%d", &numero);
        limpaBufferStdin();

        if (controlo == 0){
            printf(" Devera inserir um numero inteiro! \n");
        }
    }while(controlo ==0);

    return numero;
}

// Função que conta o número de dias entre duas datas
int contaDias_Entre_Datas(tipoData dataA,tipoData dataB)
{
    int totalDiasAB,totalDiasA,totalDiasB;

    totalDiasA=converteDataDias(dataA);
    totalDiasB=converteDataDias(dataB);
    totalDiasAB=(totalDiasB - totalDiasA);

    return totalDiasAB;
}

// Função que converte uma data para número de dias (powerpoint 12 dos apontamentos teóricos)
int converteDataDias(tipoData data)
{
    int totalDias, anosDias, mesesDias, i;
    totalDias = 0;
    anosDias = (data.ano-MENOR_ANO)*365; /* Converte em dias os anos anteriores */
    for (i=MENOR_ANO; i<data.ano; i++) {
        if((i%400==0) || (i%4==0 && i%100!=0)){
            anosDias++; /* Ano bissexto */
        }
    }
    mesesDias = 0; /* Converte em dias os meses decorridos(do ano da data) */
    for (i=1; i<data.mes; i++) {
        switch (i) {
            case 2: mesesDias += 28;
                    if((data.ano%400==0) || (data.ano%4==0 && data.ano%100!=0)){
                        mesesDias++; /* Ano bissexto */
                    }
                break;
            case 4:
            case 6:
            case 9:
            case 11: mesesDias += 30;
                break;
            default: mesesDias += 31;
        }
    }
    totalDias = anosDias + mesesDias + data.dia;
    return totalDias;
}
