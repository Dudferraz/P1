#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcoesRequisicoes.h"
#include "funcoesPortateis.h"
#include "funcoesAvarias.h"

// Função que regista a requisição de um portátil existente e disponível
tipoRequisicao *registarRequisicao(tipoRequisicao *vRequisicoes,tipoAvaria *vAvarias,int quantAvarias, int *totalRequisicoes,int *quantRequisicoesAtivas,
                tipoPortatil vPortateis[MAX_PORTATEIS], int quantPortateis,int *quantPortateisDisp){

    int numPortatil, posicaoPortatil, posicaoRequisicao, posicaoAvaria;
    int diasA=0,diasD=0,diasR=0;                                                   // diasA = dias entre a Aquisição do portátil e a requisição
    tipoRequisicao *vRequisicoesBackup;                                            // diasD = dias entre a última Devolução (a haver) e a requisição
                                                                                   // diasR = dias entre a última Reparação (a haver) e a requisição

    vRequisicoesBackup = vRequisicoes; // Backup para não perder o endereço do vector caso ele se perca ao realocar memória para este

    numPortatil = lerInteiro("\n Indique o numero identificativo do computador portatil a requisitar, 0 para retroceder",0,MAX_ID_PORTATIL);
    posicaoPortatil = procuraPortatil(vPortateis, quantPortateis, numPortatil);

    if (posicaoPortatil == -1){
        if(numPortatil!=0){
            printf("\n Numero de computador portatil inexistente! Impossivel fazer a requisicao!\n");
        }
    }
    else{
        if(strcmp(vPortateis[posicaoPortatil].estado,"disponivel")!=0){
            printf("\n Computador portatil pretendido nao se encontra disponivel!\n");
        }
        else{
            vRequisicoes=realloc(vRequisicoes,(*totalRequisicoes+1)*sizeof(tipoRequisicao));
            if (vRequisicoes==NULL){
                printf("\n Erro! Impossivel fazer a requisicao!\n");
                vRequisicoes = vRequisicoesBackup;
            }
            else{
                vRequisicoes[*totalRequisicoes].numeroPortatil = numPortatil;
                do{
                    lerString("\n Indique o codigo (unico) que pretende associar a esta requisicao: ",vRequisicoes[*totalRequisicoes].codigo,MAX_CODIGO);
                    posicaoRequisicao = procuraRequisicao(vRequisicoes, *totalRequisicoes,vRequisicoes[*totalRequisicoes].codigo);
                    if (posicaoRequisicao != -1){ // Encontrou um código igual ao introduzido
                        printf(" Codigo ja existente! Insira um codigo diferente!\n");
                    }
                }while(posicaoRequisicao != -1);
                lerString("\n Indique o nome do utente que pretende requisitar o computador portatil: ",vRequisicoes[*totalRequisicoes].nome,MAX_NOME);
                do{
                    lerString("\n Indique o tipo de utente (estudante, docente ou tecnico administrativo): ",vRequisicoes[*totalRequisicoes].tipoUtente,MAX_TIPO_UTENTE);
                    if(strcmp (vRequisicoes[*totalRequisicoes].tipoUtente,"estudante")!=0 && strcmp (vRequisicoes[*totalRequisicoes].tipoUtente,"docente")!=0 &&
                       strcmp (vRequisicoes[*totalRequisicoes].tipoUtente,"tecnico administrativo")!=0){
                        printf("\n Tipo de utente invalido!\n");
                    }
                }while(strcmp (vRequisicoes[*totalRequisicoes].tipoUtente,"estudante")!=0 && strcmp (vRequisicoes[*totalRequisicoes].tipoUtente,"docente")!=0 &&
                       strcmp (vRequisicoes[*totalRequisicoes].tipoUtente,"tecnico administrativo")!=0); // Só aceita estes tipos de utentes
                do{
                    do{
                        do{
                            printf("\n Indique a data da requisicao\n\n");
                            vRequisicoes[*totalRequisicoes].dataRequisicao=lerData();
                            diasA=contaDias_Entre_Datas(vPortateis[posicaoPortatil].dataAquisicao,vRequisicoes[*totalRequisicoes].dataRequisicao);
                            if(diasA<0){ // Avalia se a data da requisição é anterior à data de aquisição do portátil
                                printf("\n Data da requisicao tem que ser igual ou posterior a data de aquisicao do computador portatil (%d-%d-%d)!\n",vPortateis[posicaoPortatil].dataAquisicao.dia,
                                       vPortateis[posicaoPortatil].dataAquisicao.mes,vPortateis[posicaoPortatil].dataAquisicao.ano);
                            }
                        }while(diasA<0);
                        if(vPortateis[posicaoPortatil].quantRequisicoes!=0){
                            posicaoRequisicao=procuraRequisicao_Por_Numero(vRequisicoes,*totalRequisicoes,numPortatil);
                            diasD=contaDias_Entre_Datas(vRequisicoes[posicaoRequisicao].dataDevolucao,vRequisicoes[*totalRequisicoes].dataRequisicao);

                            // procuraRequisicao_Por_Numero devolve a posicao da ultima requisicao com aquele numero, independentemente do estado
                            // no entanto este portátil a ter uma requisicao anterior, esta estará obrigatoriamente concluída uma vez que se assegurou
                            // que o portatil está disponivel, portanto se teve uma requisicao anterior, terá uma data de devolucão

                            if(diasD<0){ // Avalia se a data da requisição é anterior à data da última devolução (a haver)
                                printf("\n Data de requisicao tem que ser igual ou posterior a data da ultima devolucao do computador portatil (%d-%d-%d)!\n",vRequisicoes[posicaoRequisicao].dataDevolucao.dia,
                                       vRequisicoes[posicaoRequisicao].dataDevolucao.mes,vRequisicoes[posicaoRequisicao].dataDevolucao.ano);
                            }
                        }
                    }while(diasD<0);
                    if(vPortateis[posicaoPortatil].quantAvarias!=0){ // Se o portátil já esteve avariado e está disponível então foi reparado! Logo a data da requisição terá que ser posterior
                                                                     // à data da última reparação. É necessário fazer esta protecção pois um portátil pode avariar sem ter sido requisitado, não passando pelo processo da devolução.
                        posicaoAvaria=procuraAvaria(vAvarias,quantAvarias,numPortatil);
                        diasR=contaDias_Entre_Datas(vAvarias[posicaoAvaria].dataReparacao,vRequisicoes[*totalRequisicoes].dataRequisicao);
                        if(diasR<0){
                            printf("\n Data de requisicao tem que ser igual ou posterior a data da ultima reparacao deste computador portatil (%d-%d-%d)!\n", vAvarias[posicaoAvaria].dataReparacao.dia,
                                   vAvarias[posicaoAvaria].dataReparacao.mes,vAvarias[posicaoAvaria].dataReparacao.ano);
                        }
                    }
                }while(diasR<0);

                vRequisicoes[*totalRequisicoes].prazo=lerInteiro("\n Indique o prazo de requisicao: ",1,MAX_PRAZO);
                strcpy(vRequisicoes[*totalRequisicoes].estadoRequisicao,"ativa");
                vRequisicoes[*totalRequisicoes].dataDevolucao.dia=0; // Inicializacao dos restantes campos
                vRequisicoes[*totalRequisicoes].dataDevolucao.mes=0; // por uma questao de boas praticas
                vRequisicoes[*totalRequisicoes].dataDevolucao.ano=0;
                vRequisicoes[*totalRequisicoes].totalDias=0;
                vRequisicoes[*totalRequisicoes].numRenov=0;
                strcpy(vRequisicoes[*totalRequisicoes].localDevolucao,"Nao devolvido");
                vRequisicoes[*totalRequisicoes].valorMulta=0;
                vRequisicoes[*totalRequisicoes].numRenov=0;

                (*totalRequisicoes)++;
                (*quantRequisicoesAtivas)++;
                (*quantPortateisDisp)--;

                vPortateis[posicaoPortatil].quantRequisicoes++;
                strcpy(vPortateis[posicaoPortatil].estado,"requisitado");

                printf("\n Requisicao registada com sucesso\n");
            }
        }
    }
    return vRequisicoes;
}

// Função que lista as informações de todas as requisições, incluindo as informações da devolução caso já tenha occorido
void mostraRequisicoes(tipoRequisicao *vRequisicoes, int totalRequisicoes)
{
    int i;

    for(i=0; i<totalRequisicoes; i++){
        printf("\n Codigo da requisicao: %s\n",vRequisicoes[i].codigo);
        printf(" ->   Numero do computador portatil: %d\n", vRequisicoes[i].numeroPortatil);
        printf(" ->   Nome do utente: %s\n", vRequisicoes[i].nome);
        printf(" ->   Tipo de utente: %s\n", vRequisicoes[i].tipoUtente);
        printf(" ->   Data de requisicao (dd-mm-aaaa): %d - %d - %d\n", vRequisicoes[i].dataRequisicao.dia,vRequisicoes[i].dataRequisicao.mes,vRequisicoes[i].dataRequisicao.ano);
        printf(" ->   Prazo da requisicao: %d + %d dias\n",vRequisicoes[i].prazo,MAX_PRAZO_RENOV*vRequisicoes[i].numRenov);
        printf(" ->   Quantidade de renovacoes: %d\n",vRequisicoes[i].numRenov); // Mostramos também a quantidade de renovações que foram feitas
        printf(" ->   Estado: %s\n", vRequisicoes[i].estadoRequisicao);
        if (strcmp(vRequisicoes[i].estadoRequisicao,"concluida")==0){
            printf(" ->   Data de devolucao (dd-mm-aaaa): %d - %d - %d\n", vRequisicoes[i].dataDevolucao.dia,vRequisicoes[i].dataDevolucao.mes,
                   vRequisicoes[i].dataDevolucao.ano);
            printf(" ->   Duracao da requisicao: %d dias\n",vRequisicoes[i].totalDias);
            printf(" ->   Local de devolucao: %s\n",vRequisicoes[i].localDevolucao);
            if(vRequisicoes[i].valorMulta>0){
                printf(" ->   Valor da multa: %d euros\n",vRequisicoes[i].valorMulta);
            }
            else{
                printf(" Sem multa\n");
            }
        }
    }
}

// Função que procura a requisição pretendida através do código da mesma
int procuraRequisicao(tipoRequisicao *vRequisicoes, int totalRequisicoes, char codigoRequisicao[])
{
    int i, posicao=-1;

    for (i=0; i<totalRequisicoes; i++){
        if(strcmp(codigoRequisicao,vRequisicoes[i].codigo)==0){
            posicao = i;
            i = totalRequisicoes;
        }
    }
    return posicao;
}

// Função que procura a última requisição de um portátil através do número deste independentemente do estado da requisição
int procuraRequisicao_Por_Numero(tipoRequisicao *vRequisicoes, int totalRequisicoes,int numero)
{
    int i, posicao=-1;

    for (i=(totalRequisicoes-1); i>=0; i--){ // Do final para o inicio do vector
        if(numero==vRequisicoes[i].numeroPortatil){
            posicao = i;
            i = -1;
        }
    }
    return posicao;
}

// Função que regista uma devolução sem avaria
void registarDevolucao(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int totalRequisicoes,int quantPortateis,
                       int *quantPortateisDisp,int *quantRequisicoesAtivas)
{
    int posicao,posicaoPortatil;
    int numero,diasExcesso;

    numero=lerInteiro("\n Indique o numero do computador portatil que se pretende devolver, 0 para retroceder",0,MAX_ID_PORTATIL);
    posicaoPortatil=procuraPortatil(vPortateis,quantPortateis,numero);

    if(posicaoPortatil==-1){
        if(numero!=0){
            printf("\n Computador portatil nao existente!\n");
        }
    }
    else{
        posicao = procuraRequisicao_Por_Numero(vRequisicoes, totalRequisicoes,numero); // procura ultima requisicao do portatil independentemente do estado
        if(strcmp(vRequisicoes[posicao].estadoRequisicao,"ativa") != 0){               // verifica o estado da requisicao
            printf("\n Nao existe uma requisicao ativa associada a este computador portatil!\n");
        }
        else{
            strcpy(vRequisicoes[posicao].estadoRequisicao,"concluida"); // Altera o estado da requisição
            do{
                lerString("\n Indique o local onde o computador portatil foi devolvido (Residencias, Campus1, Campus2 ou Campus5): ",vRequisicoes[posicao].localDevolucao,MAX_LOC);
                if(strcmp (vRequisicoes[posicao].localDevolucao,"Residencias")!=0 && strcmp (vRequisicoes[posicao].localDevolucao,"Campus1")!=0
                    && strcmp (vRequisicoes[posicao].localDevolucao,"Campus2")!=0 && strcmp(vRequisicoes[posicao].localDevolucao,"Campus5")!=0){
                    printf("\n Localizacao invalida!\n");
                }
            }while(strcmp (vRequisicoes[posicao].localDevolucao,"Residencias")!=0 && strcmp (vRequisicoes[posicao].localDevolucao,"Campus1")!=0
                && strcmp (vRequisicoes[posicao].localDevolucao,"Campus2")!=0 && strcmp(vRequisicoes[posicao].localDevolucao,"Campus5")!=0);

            strcpy(vPortateis[posicaoPortatil].localizacao,vRequisicoes[posicao].localDevolucao); // Update da localização do portátil
            do{
                printf("\n Indique a data da devolucao\n\n");
                vRequisicoes[posicao].dataDevolucao=lerData();
                vRequisicoes[posicao].totalDias=contaDias_Entre_Datas(vRequisicoes[posicao].dataRequisicao,vRequisicoes[posicao].dataDevolucao);
                if(vRequisicoes[posicao].totalDias<0){ // Avalia se a data de devolução é anterior à de requisição
                    printf("\n A data de devolucao tem que ser igual ou posterior a data de requisicao (%d-%d-%d)!\n",vRequisicoes[posicao].dataRequisicao.dia,
                           vRequisicoes[posicao].dataRequisicao.mes,vRequisicoes[posicao].dataRequisicao.ano);
                }
            }while(vRequisicoes[posicao].totalDias<0);

            vPortateis[posicaoPortatil].quantDiasReq+=vRequisicoes[posicao].totalDias;

            diasExcesso=vRequisicoes[posicao].totalDias-(vRequisicoes[posicao].prazo+vRequisicoes[posicao].numRenov*MAX_PRAZO_RENOV);

            if(diasExcesso>0){ // Informa da multa
                vRequisicoes[posicao].valorMulta=MULTA_BASE*diasExcesso;
                printf("\n Prazo previsto de devolucao excedido em %d (dias)! Multa de %d euros\n",diasExcesso,vRequisicoes[posicao].valorMulta);
            }
            else{
                vRequisicoes[posicao].valorMulta=0;
            }
            strcpy(vPortateis[posicaoPortatil].estado,"disponivel");
            printf("\n Devolucao registada com sucesso\n");
            (*quantPortateisDisp)++;
            (*quantRequisicoesAtivas)--;

            gravaFicheiroLog(vRequisicoes[posicao],vPortateis[posicaoPortatil]);
        }
    }
}

// Função que grava um ficheiro log das devoluções, com ou sem avaria
void gravaFicheiroLog(tipoRequisicao vDevolucao,tipoPortatil vPortatil)
{
    FILE *ficheiro;

    ficheiro = fopen("Log.txt", "a");

    if (ficheiro==NULL){
        printf("\n Erro ao abrir o ficheiro!\n");
    }
    else{
        fprintf(ficheiro,"Devolucao");
        fprintf(ficheiro,"\n Codigo da Requisicao: %s\n",vDevolucao.codigo);
        fprintf(ficheiro," ->   Numero do Portatil: %d\n", vDevolucao.numeroPortatil);
        fprintf(ficheiro," ->   Nome do Utente: %s\n", vDevolucao.nome);
        fprintf(ficheiro," ->   Tipo de Utente: %s\n", vDevolucao.tipoUtente);
        fprintf(ficheiro," ->   Data de Requisicao (dd-mm-aaaa): %d - %d - %d\n", vDevolucao.dataRequisicao.dia,vDevolucao.dataRequisicao.mes,vDevolucao.dataRequisicao.ano);
        fprintf(ficheiro," ->   Prazo da Requisicao: %d dias\n", vDevolucao.prazo);
        fprintf(ficheiro," ->   Estado: %s\n", vDevolucao.estadoRequisicao);
        fprintf(ficheiro," ->   Data de Devolucao (dd-mm-aaaa): %d - %d - %d\n", vDevolucao.dataDevolucao.dia,vDevolucao.dataDevolucao.mes,
                                                                            vDevolucao.dataDevolucao.ano);
        fprintf(ficheiro," ->   Quantidade de Renovacoes: %d\n",vDevolucao.numRenov);
        fprintf(ficheiro," ->   Duracao da Requisicao: %d dias\n",vDevolucao.totalDias);
        fprintf(ficheiro," ->   Local de Devolucao: %s\n",vDevolucao.localDevolucao);
        fprintf(ficheiro," ->   Valor da Multa: %d\n",vDevolucao.valorMulta);
        fprintf(ficheiro," ->   Processador do Portatil: I%d\n",vPortatil.processador);
        fprintf(ficheiro," ->   Memoria RAM do Portatil: %d GB\n\n",vPortatil.memoria);

    }
    fclose(ficheiro);
}

// Função que renova uma requisição ativa por 7 dias
void renovaRequisicao(tipoRequisicao *vRequisicoes,int totalRequisicoes)
{
    int posicao;
    char strCodigo[MAX_CODIGO];

    lerString("\n Indique o codigo da requisicao ativa que se pretende renovar, 0 para retroceder: ",strCodigo,MAX_CODIGO);
    posicao = procuraRequisicao(vRequisicoes, totalRequisicoes,strCodigo);

    if (posicao == -1){
        if(strCodigo[0]!='0'){
            printf("\n Codigo inexistente!\n");
        }
    }
    else{
        if(strcmp(vRequisicoes[posicao].estadoRequisicao,"concluida")==0){ // Verifica se está de facto ativa
            printf("\n Esta Requisicao ja se encontra concluida!\n");
        }
        else{
            vRequisicoes[posicao].numRenov++;
            printf("\n Renovacao registada com sucesso por mais 7 dias\n");
        }
    }
}

// Função que mostra a informação de uma requisição pelo código desta introduzido pelo utilizador
void informacaoRequisicao (tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int totalRequisicoes,int quantPortateis)
{
    int posicao,posicaoPortatil; // posicao -> posição da requisição pretendida
    char strCodigo[MAX_CODIGO];

    lerString("\n Indique o codigo da requisicao que pretende ver, 0 para retroceder: ",strCodigo,MAX_CODIGO);
    posicao = procuraRequisicao(vRequisicoes, totalRequisicoes,strCodigo);

    if (posicao == -1){
        if(strCodigo[0]!='0'){
            printf("\n Codigo inexistente!\n");
        }
    }
    else{
        posicaoPortatil=procuraPortatil(vPortateis,quantPortateis,vRequisicoes[posicao].numeroPortatil);
        printf("\n Codigo da requisicao: %s\n",vRequisicoes[posicao].codigo);
        printf(" ->   Numero do computador portatil: %d\n", vRequisicoes[posicao].numeroPortatil);
        printf(" ->   Designacao: %s\n", vPortateis[posicaoPortatil].designacao);
        printf(" ->   Estado: %s\n", vPortateis[posicaoPortatil].estado);
        printf(" ->   Quantidade de requisicoes deste computador portatil: %d\n", vPortateis[posicaoPortatil].quantRequisicoes);
        printf(" ->   Quantidade total de dias requisitado: %d\n",vPortateis[posicaoPortatil].quantDiasReq);
        printf(" ->   Nome do utente: %s\n", vRequisicoes[posicao].nome);
        printf(" ->   Tipo de utente: %s\n", vRequisicoes[posicao].tipoUtente);
        printf(" ->   Data de requisicao (dd-mm-aaaa): %d - %d - %d\n", vRequisicoes[posicao].dataRequisicao.dia,vRequisicoes[posicao].dataRequisicao.mes,
                                                                      vRequisicoes[posicao].dataRequisicao.ano);
        printf(" ->   Prazo da requisicao: %d + %d dias\n",vRequisicoes[posicao].prazo,MAX_PRAZO_RENOV*vRequisicoes[posicao].numRenov);
        printf(" ->   Quantidade de renovacoes: %d\n",vRequisicoes[posicao].numRenov);
        printf(" ->   Estado: %s\n", vRequisicoes[posicao].estadoRequisicao);
        if (strcmp(vRequisicoes[posicao].estadoRequisicao,"concluida")==0){
            printf(" ->   Data de devolucao (dd-mm-aaaa): %d - %d - %d\n", vRequisicoes[posicao].dataDevolucao.dia,vRequisicoes[posicao].dataDevolucao.mes,
                                                                         vRequisicoes[posicao].dataDevolucao.ano);
            printf(" ->   Duracao da requisicao: %d dias\n",vRequisicoes[posicao].totalDias);
            printf(" ->   Local de devolucao: %s\n",vRequisicoes[posicao].localDevolucao);
            if(vRequisicoes[posicao].valorMulta>0){
                printf(" ->   Valor da multa: %d euros\n",vRequisicoes[posicao].valorMulta);
            }
            else{
                printf(" Sem multa\n");
            }
        }
    }
}

