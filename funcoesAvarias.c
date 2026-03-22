#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "funcoesPortateis.h"
#include "funcoesRequisicoes.h"
#include "funcoesAvarias.h"


// Função que regista uma avaria, quer o portátil se encontre requisitado, quer se encontre no Instituto
tipoAvaria *registarAvaria(tipoAvaria *vAvarias,tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int totalRequisicoes,
                           int *quantPortateisDisp,int quantPortateis,int *quantAvarias, int *quantRequisicoesAtivas,int *quantPortateisAvariados)
{
    tipoAvaria *vAvariasBackup;
    int posicao,posicaoPortatil,posicaoAvaria,diasExcesso;      // diasA = dias entre a Aquisição do portátil e a avaria
    int numero,certeza,diasA=0,diasD=0,diasR=0;                 // diasD = dias entre a última devolução do portátil (a haver) e a avaria
    vAvariasBackup=vAvarias;                                    // diasR = dias entre a última Reparação (a haver) e a avaria

    numero=lerInteiro("\n Indique o numero identificativo do computador portatil que avariou, 0 para retroceder",0,MAX_ID_PORTATIL);
    posicaoPortatil=procuraPortatil(vPortateis,quantPortateis,numero);

    if(posicaoPortatil==-1){
        if(numero!=0){
            printf("\n Computador portatil nao existente!\n");
        }
    }
    else{
        if(strcmp(vPortateis[posicaoPortatil].estado,"avariado")==0){ // Verifica se o portátil já se encontra registado como avariado
            printf("\n Este computador portatil ja foi registado como avariado\n");
        }
        else{
            posicao = procuraRequisicao_Por_Numero(vRequisicoes, totalRequisicoes,numero); // procura ultima requisicao do portatil independentemente do estado
            if (posicao!=-1&&strcmp(vRequisicoes[posicao].estadoRequisicao,"ativa") == 0){ // verifica o estado da requisicao se esta existir

                vAvarias=realloc(vAvarias,(*quantAvarias+1)*sizeof(tipoAvaria));

                if (vAvarias==NULL){
                    printf("\n Erro ao criar vetor de avarias!\n");
                    vAvarias = vAvariasBackup;
                }
                else{
                    strcpy(vRequisicoes[posicao].estadoRequisicao,"concluida");
                    do{
                        lerString("\n Indique o local onde o Portatil foi devolvido por avaria (Residencias, Campus1, Campus2 ou Campus5): ",vRequisicoes[posicao].localDevolucao,MAX_LOC);
                        if(strcmp (vRequisicoes[posicao].localDevolucao,"Residencias")!=0 && strcmp (vRequisicoes[posicao].localDevolucao,"Campus1")!=0
                           && strcmp (vRequisicoes[posicao].localDevolucao,"Campus2")!=0 && strcmp(vRequisicoes[posicao].localDevolucao,"Campus5")!=0){
                            printf("\n Localizacao invalida!\n");
                        }
                    }while(strcmp (vRequisicoes[posicao].localDevolucao,"Residencias")!=0 && strcmp (vRequisicoes[posicao].localDevolucao,"Campus1")!=0
                           && strcmp (vRequisicoes[posicao].localDevolucao,"Campus2")!=0 && strcmp(vRequisicoes[posicao].localDevolucao,"Campus5")!=0);

                    strcpy(vPortateis[posicaoPortatil].localizacao,vRequisicoes[posicao].localDevolucao); // Update da localização do portátil

                    do{
                        printf("\n Indique a data da devolucao por avaria\n\n");
                        vRequisicoes[posicao].dataDevolucao=lerData();
                        vRequisicoes[posicao].totalDias=contaDias_Entre_Datas(vRequisicoes[posicao].dataRequisicao,vRequisicoes[posicao].dataDevolucao);
                        if(vRequisicoes[posicao].totalDias<0){
                            printf("\n A data de devolucao por avaria tem que ser igual ou posterior a data de Requisicao (%d-%d-%d)! \n",vRequisicoes[posicao].dataRequisicao.dia,
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
                    strcpy(vPortateis[posicaoPortatil].estado,"avariado");
                    vAvarias[*quantAvarias].numeroAvaria = (*quantAvarias)+1;
                    vAvarias[*quantAvarias].numeroPortatil = numero;
                    vAvarias[*quantAvarias].totalDiasAvaria=-1;
                    vAvarias[*quantAvarias].dataAvaria=vRequisicoes[posicao].dataDevolucao;
                    vAvarias[*quantAvarias].dataReparacao.dia=0;
                    vAvarias[*quantAvarias].dataReparacao.mes=0;
                    vAvarias[*quantAvarias].dataReparacao.ano=0;
                    do{
                        lerString("\n Indique o tipo de avaria (temporaria ou permanente): ",vAvarias[*quantAvarias].tipoDeAvaria,MAX_AVARIA);
                        if(strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"temporaria")!=0&&strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"permanente")!=0){
                            printf("\n Tipo de avaria invalido!\n");
                        }
                    }while(strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"temporaria")!=0&&strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"permanente")!=0);

                    (*quantRequisicoesAtivas)--;
                    vPortateis[posicaoPortatil].quantAvarias++;
                    (*quantAvarias)++;
                    (*quantPortateisAvariados)++;

                    printf("\n Avaria registada com sucesso\n");
                    gravaFicheiroLog(vRequisicoes[posicao],vPortateis[posicaoPortatil]);
                }
            }
            else{ // Estes são os casos em que um portátil que não se encontra requisitado, avariou
                vAvarias=realloc(vAvarias,((*quantAvarias)+1)*sizeof(tipoAvaria));

                if (vAvarias==NULL){
                    printf("\n Erro ao criar vetor de avarias!\n");
                    vAvarias = vAvariasBackup;
                }
                else{
                    certeza=lerInteiro("\n Computador portatil nao requisitado, tem a certeza que pretende declara-lo como avariado? (1-Sim,0-Nao):",0,1);
                    if(certeza==1){
                        strcpy(vPortateis[posicaoPortatil].estado,"avariado");
                        vAvarias[*quantAvarias].numeroAvaria = (*quantAvarias)+1;
                        vAvarias[*quantAvarias].numeroPortatil = numero;
                        vAvarias[*quantAvarias].totalDiasAvaria=-1;
                        vAvarias[*quantAvarias].dataReparacao.dia=0;
                        vAvarias[*quantAvarias].dataReparacao.mes=0;
                        vAvarias[*quantAvarias].dataReparacao.ano=0;
                        do{
                            do{
                                do{
                                    printf("\n Indique a data da avaria\n\n");
                                    vAvarias[*quantAvarias].dataAvaria=lerData();
                                    diasA=contaDias_Entre_Datas(vPortateis[posicaoPortatil].dataAquisicao,vAvarias[*quantAvarias].dataAvaria);
                                    if(diasA<0){
                                        printf("\n Data da avaria tem que ser igual ou posterior a data de aquisicao do computador portatil (%d-%d-%d)!\n",vPortateis[posicaoPortatil].dataAquisicao.dia,
                                               vPortateis[posicaoPortatil].dataAquisicao.mes,vPortateis[posicaoPortatil].dataAquisicao.ano);
                                    }
                                }while(diasA<0);
                                if(vPortateis[posicaoPortatil].quantRequisicoes!=0){ // Caso o portátil já tenha sido requisitado
                                    diasD=contaDias_Entre_Datas(vRequisicoes[posicao].dataDevolucao,vAvarias[*quantAvarias].dataAvaria);
                                    if(diasD<0){
                                        printf("\n Data da avaria tem que ser igual ou posterior a data da ultima devolucao do computador portatil (%d-%d-%d)!\n",vRequisicoes[posicao].dataDevolucao.dia,
                                               vRequisicoes[posicao].dataDevolucao.mes,vRequisicoes[posicao].dataDevolucao.ano);
                                    }
                                }
                            }while(diasD<0);
                            if(vPortateis[posicaoPortatil].quantAvarias!=0){ // Se o portátil já esteve avariado e já não se encontra avariado então foi reparado!
                                                                             // Logo a data da avaria terá que ser posterior à data da última reparação
                                posicaoAvaria=procuraAvaria(vAvarias,*quantAvarias,numero);
                                diasR=contaDias_Entre_Datas(vAvarias[posicaoAvaria].dataReparacao,vAvarias[*quantAvarias].dataAvaria);
                                if(diasR<0){
                                    printf("\n Data de avaria tem que ser igual ou posterior a data da ultima reparacao deste computador portatil (%d-%d-%d)!\n", vAvarias[posicaoAvaria].dataReparacao.dia,
                                    vAvarias[posicaoAvaria].dataReparacao.mes,vAvarias[posicaoAvaria].dataReparacao.ano);
                                }
                            }
                        }while(diasR<0);
                        do{
                            lerString("\n Indique o tipo de avaria (temporaria ou permanente): ",vAvarias[*quantAvarias].tipoDeAvaria,MAX_AVARIA);
                            if(strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"temporaria")!=0&&strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"permanente")!=0){
                                printf("\n Tipo de avaria invalido!\n");
                            }
                        }while(strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"temporaria")!=0&&strcmp(vAvarias[*quantAvarias].tipoDeAvaria,"permanente")!=0);

                        (*quantPortateisDisp)--;
                        vPortateis[posicaoPortatil].quantAvarias++;
                        (*quantAvarias)++;
                        (*quantPortateisAvariados)++;

                        printf("\n Avaria registada com sucesso\n");
                        gravaFicheiroLog(vRequisicoes[posicao],vPortateis[posicaoPortatil]);
                    }
                }
            }
        }
    }
    return vAvarias;
}

// Função que procura a última avaria pelo número do portátil avariado
int procuraAvaria(tipoAvaria *vAvarias, int quantAvarias,int numero)
{
    int i, posicao=-1;

    for (i=(quantAvarias-1); i>=0; i--){ // Do fim do vector para o inicio
        if(numero==vAvarias[i].numeroPortatil){
            posicao = i;
            i = -1;
        }
    }
    return posicao;
}

// Função que regista uma reparação
void registarReparacao(tipoAvaria *vAvarias,tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis,int quantAvarias,
                       int *quantPortateisDisp,int *quantPortateisAvariados)
{
    int numero,posicaoPortatil,posicaoAvaria;

    numero=lerInteiro("\n Indique o numero identificativo do Computador Portatil reparado, 0 para retroceder",0,MAX_ID_PORTATIL);
    posicaoPortatil=procuraPortatil(vPortateis,quantPortateis,numero);

    if(posicaoPortatil==-1){
        if(numero!=0){
            printf("\n Computador portatil inexistente!\n");
        }
    }
    else{
        if(strcmp(vPortateis[posicaoPortatil].estado,"avariado")!=0){ // Portátil tem que estar avariado
            printf("\n Este portatil nao se encontra avariado!\n");
        }
        else{
            posicaoAvaria=procuraAvaria(vAvarias,quantAvarias,numero);
            if(strcmp(vAvarias[posicaoAvaria].tipoDeAvaria,"permanente")==0){ // Portátil não pode estar avariado permanentemente
                printf("\n Impossivel registar a reparacao, portatil com avaria permanente!\n");
            }
            else{
                do{
                    printf("\n Indique a data da Reparacao\n\n");
                    vAvarias[posicaoAvaria].dataReparacao=lerData();
                    vAvarias[posicaoAvaria].totalDiasAvaria=contaDias_Entre_Datas(vAvarias[posicaoAvaria].dataAvaria,vAvarias[posicaoAvaria].dataReparacao);
                    if(vAvarias[posicaoAvaria].totalDiasAvaria<0){
                        printf("\n A data de reparacao tem que ser igual ou posterior a data de avaria (%d-%d-%d)!\n",vAvarias[posicaoAvaria].dataAvaria.dia,
                               vAvarias[posicaoAvaria].dataAvaria.mes,vAvarias[posicaoAvaria].dataAvaria.ano);
                    }
                }while(vAvarias[posicaoAvaria].totalDiasAvaria<0);

                (*quantPortateisDisp)++;
                (*quantPortateisAvariados)--;

                strcpy(vPortateis[posicaoPortatil].estado,"disponivel");
                printf("\n Reparacao registada com sucesso\n");
            }
        }
    }
}

// Função que lista as informações das avarias
void listaAvarias(tipoAvaria *vAvarias,tipoPortatil vPortateis[MAX_PORTATEIS],int quantAvarias,int quantPortateis)
{
    int i,posicaoPortatil;

    for(i=0;i<quantAvarias;i++){
        printf(" Avaria %d:\n",vAvarias[i].numeroAvaria);
        printf(" ->  Numero do Portatil: %d\n",vAvarias[i].numeroPortatil);
        posicaoPortatil=procuraPortatil(vPortateis,quantPortateis,vAvarias[i].numeroPortatil); // Procura a posição do portátil em questão para extrair as suas informações
        printf(" ->  Designacao: %s\n", vPortateis[posicaoPortatil].designacao);
        printf(" ->  Processador: I%d\n", vPortateis[posicaoPortatil].processador);
        printf(" ->  RAM: %d GB\n", vPortateis[posicaoPortatil].memoria);
        printf(" ->  Estado: %s\n", vPortateis[posicaoPortatil].estado);
        printf(" ->  Localizacao: %s\n", vPortateis[posicaoPortatil].localizacao);
        printf(" ->  Data de Aquisicao(dd-mm-aaaa): %d - %d - %d\n", vPortateis[posicaoPortatil].dataAquisicao.dia,
                                    vPortateis[posicaoPortatil].dataAquisicao.mes,vPortateis[posicaoPortatil].dataAquisicao.ano);
        printf(" ->  Valor do Equipamento: %.2f euros\n", vPortateis[posicaoPortatil].valorEquip);
        printf(" ->  Tipo de avaria: %s\n",vAvarias[i].tipoDeAvaria);
        printf(" ->  Data da Avaria(dd-mm-aaaa): %d - %d - %d\n", vAvarias[i].dataAvaria.dia,vAvarias[i].dataAvaria.mes,vAvarias[i].dataAvaria.ano);
        if(vAvarias[i].totalDiasAvaria>=0){ // Se foi reparado
            printf(" ->  Data da reparacao(dd-mm-aaaa): %d - %d - %d\n", vAvarias[i].dataReparacao.dia,vAvarias[i].dataReparacao.mes,vAvarias[i].dataReparacao.ano);
            printf(" ->  Total de dias da avaria: %d\n",vAvarias[i].totalDiasAvaria);
        }
    }
}

// Função que grava as informações das Avarias existentes num ficheiro binário
void gravaFicheiroBinarioA(tipoAvaria *vAvarias,int quantAvarias)
{

    FILE *ficheiroBin;

    ficheiroBin = fopen("InfoAvarias.dat", "wb");

    if (ficheiroBin==NULL){
        printf("\n Erro ao abrir o ficheiro das Avarias!\n");
    }
    else{
        fwrite(&quantAvarias, sizeof(int), 1, ficheiroBin);
        fwrite(vAvarias, sizeof(tipoAvaria), quantAvarias, ficheiroBin);
        fclose(ficheiroBin);
        printf("\n Informacoes gravadas com sucesso\n");
    }
}


// Função que lê as informações das Avarias existentes dum ficheiro binário
tipoAvaria *leFicheiroBinarioA(tipoAvaria *vAvarias,int *quantAvarias)
{

    FILE *ficheiroBin;

    tipoAvaria *vAvariasBackup;

    vAvariasBackup = vAvarias;

    ficheiroBin = fopen("InfoAvarias.dat", "rb");

    if (ficheiroBin==NULL){
        printf("\n Erro ao abrir o ficheiro das Avarias!\n");
    }
    else{
        fread(quantAvarias, sizeof(int), 1, ficheiroBin);

        vAvarias=realloc(vAvarias,(*quantAvarias)*sizeof(tipoAvaria)); // Aloca a dimensão necessária para transferir as requisições

        if (vAvarias==NULL){
            printf(" Erro! Foi impossivel ler a informacao das avarias!\n");
            vAvarias = vAvariasBackup;
        }
        else{
            fread(vAvarias, sizeof(tipoAvaria),*quantAvarias,ficheiroBin);
            printf("\n Informacoes lidas com sucesso\n");
        }
        fclose(ficheiroBin);
    }
    return vAvarias;
}
