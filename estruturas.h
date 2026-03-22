#ifndef ESTRUTURAS_H_INCLUDED
#define ESTRUTURAS_H_INCLUDED

typedef struct
{
    int dia;
    int mes;
    int ano;

}tipoData;

typedef struct
{
    int numero;
    char designacao[MAX_DESIGN];
    int processador;   // optamos por ler o processador como um numero int, Ix
    int memoria;       // optamos por ler a memória como um numero int, xx GB
    char estado[MAX_ESTADO];
    char localizacao[MAX_LOC];
    tipoData dataAquisicao;
    float valorEquip;  // optamos por ler o valor do equipamento como um float por uma questão de rigor
    int quantAvarias;
    int quantRequisicoes;
    int quantDiasReq;

}tipoPortatil;

typedef struct
{
    char codigo[MAX_CODIGO];
    int numeroPortatil;
    char nome[MAX_NOME];
    char tipoUtente[MAX_TIPO_UTENTE];
    tipoData dataRequisicao;
    int prazo;
    int numRenov;  // Em vez de alterar o prazo original, criámos um contador de renovações, isto porque mostramos ambas as informações nas listagens
    int totalDias;
    char estadoRequisicao[MAX_ESTADO_REQ];
    tipoData dataDevolucao;
    char localDevolucao[MAX_LOCAL_DEVOLUCAO];
    int valorMulta;  // optamos por o valorMulta ser um inteiro uma vez que esta será um multiplo de 10€

}tipoRequisicao;

typedef struct
{
    int numeroAvaria;
    int numeroPortatil;
    char tipoDeAvaria[MAX_AVARIA];
    tipoData dataAvaria;
    tipoData dataReparacao;
    int totalDiasAvaria;

}tipoAvaria;

#endif // ESTRUTURAS_H_INCLUDED
