#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "funcoesPortateis.h"
#include "funcoesRequisicoes.h"
#include "funcoesAvarias.h"

// Funcão inserePortateis pergunta quantos portáteis se pretende inserir e insere essa quantidade com auxílio da funcão leDadosPortatil
int inserePortateis(tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis,int *quantPortateisDisp)
{
    int quantInserir,i,diferenca;

    do{
        // Como nesta função, ao longo do programa, a maioria das opcões dos menus permitem retroceder após a escolha desta, inserindo 0
        quantInserir=lerInteiro("\n Indique quantos computadores portateis pretende inserir, 0 para retroceder",0,MAX_PORTATEIS);

        diferenca=MAX_PORTATEIS-quantPortateis; // Calcula a quantidade de portáteis que ainda se podem inserir

        if((quantInserir+quantPortateis)>MAX_PORTATEIS){ // Verifica se a quantidade de portáteis a inserir + os que já existem excede o máximo possível
            printf(" Existem %d computadores portateis no sistema. Somente pode inserir mais %d! \n",quantPortateis,diferenca);
        }
    }while((quantInserir+quantPortateis)>MAX_PORTATEIS);

    for(i=0;i<quantInserir;i++){ // Insere os dados para a quantidade de portatéis pretendida, quantInserir=0 força a sair da função, efectivamente retrocedendo
        vPortateis[quantPortateis]=leDadosPortatil(vPortateis,quantPortateis);
        printf("\n Computador portatil %d inserido com sucesso\n",vPortateis[quantPortateis].numero);
        quantPortateis++;
        (*quantPortateisDisp)++;
    }
    return quantPortateis;
}

// Funcão leDadosPortatil pergunta ao utilizador as informacões de um portátil e associa-as ao mesmo
tipoPortatil leDadosPortatil(tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis)
{
    tipoPortatil portatil;
    int posicao=-1;

        do{
            portatil.numero=lerInteiro("\n Indique o numero identificativo do novo computador portatil",1,MAX_ID_PORTATIL);
            if(quantPortateis!=0){
                posicao=procuraPortatil(vPortateis,quantPortateis,portatil.numero);
                if(posicao!=-1){
                    printf("  Computador portatil ja inserido!\n");
                }
            }
        }while(posicao!=-1);

        printf("\n Computador portatil numero: %d\n",portatil.numero);
        lerString(">Indique a designacao do computador portatil: ",portatil.designacao,MAX_DESIGN);
        do{
            portatil.processador=lerProcessador_Ou_Memoria(">Indique o processador (I3, I5, I7): I"); // Optámos por ler o processador como um numero int, Ix
            if(portatil.processador!=3 && portatil.processador!=5 && portatil.processador!=7){
                printf("  Numero invalido!\n");
            }
        }while(portatil.processador!=3 && portatil.processador!=5 && portatil.processador!=7);
        do{
            portatil.memoria=lerProcessador_Ou_Memoria(">Indique a quantidade de GigaBytes da memoria RAM (4, 8, 16, 32): "); // optamos por ler a memória como um numero int, xx GB
            if(portatil.memoria!=4 && portatil.memoria!=8 && portatil.memoria!=16 && portatil.memoria!=32){
                printf("  Numero invalido!\n");
            }
        }while(portatil.memoria!=4 && portatil.memoria!=8 && portatil.memoria!=16 && portatil.memoria!=32);
        strcpy(portatil.estado,"disponivel");
        do{
            lerString(">Indique a localizacao (Residencias, Campus1, Campus2 ou Campus5): ",portatil.localizacao,MAX_LOC);

            // Optámos por apenas aceitar as opções escritas exactamente como foram apresentadas no enunciado do projecto

            if(strcmp (portatil.localizacao,"Residencias")!=0 && strcmp (portatil.localizacao,"Campus1")!=0
                && strcmp (portatil.localizacao,"Campus2")!=0 && strcmp(portatil.localizacao,"Campus5")!=0){
                printf("  Localizacao invalida!\n");
            }
        }while(strcmp (portatil.localizacao,"Residencias")!=0 && strcmp (portatil.localizacao,"Campus1")!=0
                && strcmp (portatil.localizacao,"Campus2")!=0 && strcmp(portatil.localizacao,"Campus5")!=0);

        printf(" >Indique a data de aquisicao:\n");
        portatil.dataAquisicao=lerData();
        portatil.valorEquip=lerFloat(">Indique o valor do equipamento em euros",VALOR_EQ_MIN,VALOR_EQ_MAX);
        portatil.quantAvarias=0;
        portatil.quantRequisicoes=0;
        portatil.quantDiasReq=0;

    return portatil;
}

// Função que devolve a posição do portáril pretendido através do seu número
int procuraPortatil(tipoPortatil vPortateis[MAX_PORTATEIS], int quantPortateis, int numero)
{
    int i, posicao=-1;

    for(i=0; i<quantPortateis;i++){
       if (numero == vPortateis[i].numero){
            posicao = i;
            i = quantPortateis;
       }
    }
    return posicao;
}

// Função que mostra os dados dos portáteis bem como, se houverem, as suas requisições
void mostraDadosPortateis(tipoRequisicao *vRequisicoes,tipoPortatil vPortateis[MAX_PORTATEIS],int totalRequisicoes,int quantPortateis)
{
    int i,u;

    for(i=0; i<quantPortateis; i++){
        printf("\n Computador portatil numero: %d\n",vPortateis[i].numero);
        printf(" ->   Designacao: %s\n", vPortateis[i].designacao);
        printf(" ->   Processador: I%d\n", vPortateis[i].processador);
        printf(" ->   RAM: %d GB\n", vPortateis[i].memoria);
        printf(" ->   Estado: %s\n", vPortateis[i].estado);
        printf(" ->   Localizacao: %s\n", vPortateis[i].localizacao);
        printf(" ->   Data de aquisicao (dd-mm-aaaa): %d - %d - %d\n", vPortateis[i].dataAquisicao.dia,vPortateis[i].dataAquisicao.mes,vPortateis[i].dataAquisicao.ano);
        printf(" ->   Valor do equipamento: %.2f euros\n", vPortateis[i].valorEquip);
        printf(" ->   Quantidade de avarias: %d\n", vPortateis[i].quantAvarias);
        if (vPortateis[i].quantRequisicoes==0){
            printf(" ->   Sem requisicoes\n\n");
        }
        else{
            printf(" ->   Quantidade de requisicoes: %d\n", vPortateis[i].quantRequisicoes);
            for (u=0; u<totalRequisicoes; u++){
                if(vPortateis[i].numero==vRequisicoes[u].numeroPortatil){
                    printf(" Requisicao %s:\n",vRequisicoes[u].codigo);
                    printf("   Tipo de utente: %s \n",vRequisicoes[u].tipoUtente);
                    printf("   Prazo da requisicao: %d + %d dias\n",vRequisicoes[u].prazo,MAX_PRAZO_RENOV*vRequisicoes[u].numRenov);
                }
            }
        }
    }
}

// Função que altera a localização de um portátil existente se este não estiver requisitado
void alteraLocalizacao(tipoPortatil vPortateis[MAX_PORTATEIS],int quantPortateis)
{
    int posicao=-1,numero;
    char localizacaoAux[MAX_LOC];

// O código comentado em baixo permitia relembrar ao utilizador o numero identificativo dos portateis existentes. Poderia ser implementado em diversas funcoes.
// Bastante útil quando há poucos portáteis mas desagradável quando há muitos portáteis. Decidimos comentar em vez de apagar ou implementar.
//    int i;
//    printf("  Portateis existentes: ");
//    for(i=0;i<quantPortateis;i++){
//        printf("%d ",vPortateis[i].numero);
//    }
//    printf("\n");

    numero=lerInteiro("\n Indique o numero identificativo do computador portatil ao qual pretende alterar a localizacao, 0 para retroceder ",0,MAX_ID_PORTATIL);
    posicao=procuraPortatil(vPortateis,quantPortateis,numero);

    if(posicao==-1){   // Verifica se um portátil com este número existe
        if(numero!=0){ // Se Não foi introduzido 0, apresenta esta mensagem, caso contrário apenas retrocede
            printf("\n Computador portatil nao existente!\n\n");
        }
    }
    else{
        if(strcmp(vPortateis[posicao].estado,"requisitado")==0){ // Achámos que não faria sentido alterar a localização de um portátil requisitado
            printf("\n Impossivel alterar localizacao de um computador portatil requisitado!\n");
        }
        else{
            strcpy(localizacaoAux,vPortateis[posicao].localizacao); // localizacaoAux guarda a localização do portátil antes da alteração
            do{
                lerString("\n Indique a nova localizacao (Residencias, Campus1, Campus2 ou Campus5): ",vPortateis[posicao].localizacao,MAX_LOC);
                if(strcmp (vPortateis[posicao].localizacao,"Residencias")!=0 && strcmp (vPortateis[posicao].localizacao,"Campus1")!=0
                   && strcmp (vPortateis[posicao].localizacao,"Campus2")!=0 && strcmp(vPortateis[posicao].localizacao,"Campus5")!=0){
                    printf(" Localizacao invalida!\n");
                }
            }while(strcmp (vPortateis[posicao].localizacao,"Residencias")!=0 && strcmp (vPortateis[posicao].localizacao,"Campus1")!=0
                   && strcmp (vPortateis[posicao].localizacao,"Campus2")!=0 && strcmp(vPortateis[posicao].localizacao,"Campus5")!=0);

            if(strcmp(localizacaoAux,vPortateis[posicao].localizacao)==0){ // Verifica se a nova localização é igual à anterior
                printf("\n O computador portatil ja se encontra nesta localizacao!\n");
            }
            else{

                printf("\n Alteracao da localizacao realizada com sucesso\n");
            }
        }
    }
}

// Função que grava as informações dos portáteis e das requisições existentes num ficheiro binário
void gravaFicheiroBinarioPR(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int quantPortateis,
                          int quantPortateisDisp,int quantRequisicoesAtivas,int totalRequisicoes,int quantPortateisAvariados)
{

    FILE *ficheiroBin;

    ficheiroBin = fopen("InfoPortateisRequisicoes.dat", "wb");

    if (ficheiroBin==NULL){
        printf("\n Erro ao abrir o ficheiro dos computadores portateis e das requisicoes!\n");
    }
    else{
        fwrite(&quantPortateis, sizeof(int), 1, ficheiroBin);
        fwrite(&quantPortateisDisp, sizeof(int), 1, ficheiroBin);
        fwrite(&quantPortateisAvariados, sizeof(int), 1, ficheiroBin);
        fwrite(&quantRequisicoesAtivas, sizeof(int), 1, ficheiroBin);
        fwrite(&totalRequisicoes, sizeof(int), 1, ficheiroBin);
        fwrite(vPortateis, sizeof(tipoPortatil), quantPortateis, ficheiroBin);
        fwrite(vRequisicoes, sizeof(tipoRequisicao), totalRequisicoes, ficheiroBin);
        fclose(ficheiroBin);
    }
}

// Função que lê as informações dos portáteis e das requisições existentes dum ficheiro binário
tipoRequisicao *leFicheiroBinarioPR(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int *quantPortateis,
                          int *quantPortateisDisp,int *quantRequisicoesAtivas,int *totalRequisicoes,int *quantPortateisAvariados)
{

    FILE *ficheiroBin;

    tipoRequisicao *vRequisicoesBackup;

    vRequisicoesBackup = vRequisicoes;

    ficheiroBin = fopen("InfoPortateisRequisicoes.dat", "rb");

    if (ficheiroBin==NULL){
        printf("\n Erro ao abrir o ficheiro dos computadores portateis e das requisicoes!\n");
    }
    else{
        fread(quantPortateis, sizeof(int), 1, ficheiroBin);
        fread(quantPortateisDisp, sizeof(int), 1, ficheiroBin);
        fread(quantPortateisAvariados, sizeof(int), 1, ficheiroBin);
        fread(quantRequisicoesAtivas, sizeof(int), 1, ficheiroBin);
        fread(totalRequisicoes, sizeof(int), 1, ficheiroBin);
        fread(vPortateis, sizeof(tipoPortatil), *quantPortateis, ficheiroBin);

        vRequisicoes=realloc(vRequisicoes,(*totalRequisicoes)*sizeof(tipoRequisicao)); // Aloca a dimensão necessária para transferir as requisições

        if (vRequisicoes==NULL){
            printf(" Erro! Foi impossivel ler a informacao das requisicoes!\n");
            vRequisicoes = vRequisicoesBackup;
        }
        else{
            fread(vRequisicoes, sizeof(tipoRequisicao),*totalRequisicoes,ficheiroBin);
        }
        fclose(ficheiroBin);
    }
    return vRequisicoes;
}

// Função que apresenta os diversos dados estatísticos
void apresentaDadosEstatisticos(tipoPortatil vPortateis[MAX_PORTATEIS],tipoRequisicao *vRequisicoes,int quantPortateis,int totalRequisicoes,int quantRequisicoesAtivas)
{
    float percentagemI3,percentagemI5,percentagemI7,mediaMultas;
    int i,contI3=0,contI5=0,contI7=0,contM=0,totalMultas=0,min,max=0,dias; // contM = nº de multas, totalMultas = soma dos valores de cada multa
    int vCont[3]={0}; // Vector de contadores

    for(i=0;i<quantPortateis;i++){
        if(vPortateis[i].processador==3){
            contI3++;
        }
        else{
            if(vPortateis[i].processador==5){
                contI5++;
            }
            else{
                contI7++;
            }
        }
    }
    percentagemI3=(contI3/(float)quantPortateis)*100;
    percentagemI5=(contI5/(float)quantPortateis)*100;
    percentagemI7=(contI7/(float)quantPortateis)*100;

    for(i=0;i<totalRequisicoes;i++){
        if(vRequisicoes[i].valorMulta>0){  // Como no enunciado diz "o custo médio de cada multa paga", e como ninguém paga uma multa de 0 euros, apenas
            contM++;                       // multas superiores a 0 euros são consideradas para o cálculo da média.
            totalMultas+=vRequisicoes[i].valorMulta;
        }
    }
    if(totalMultas==0){
        mediaMultas=0;
    }
    else{
        mediaMultas=totalMultas/(float)contM;
    }
    for(i=0;i<totalRequisicoes;i++){ // Conta nº de requisicoes de cada tipo de utente
        if(strcmp(vRequisicoes[i].tipoUtente,"estudante")==0){
            vCont[0]++;
        }
        else{
            if(strcmp(vRequisicoes[i].tipoUtente,"docente")==0){
                vCont[1]++;
            }
            else{
                vCont[2]++;
            }
        }
    }
    min=vCont[0]; // Assume um dos contadores como o minimo

    for(i=0;i<3;i++){ // Verifica qual dos contadores é menor
        if(vCont[i]<min){
            min=vCont[i];
        }
    }
    for(i=0;i<totalRequisicoes;i++){ // Obtém o nº de dias da devolução mais recente, que é a que tem mais dias desde o ano MENOR_ANO
        dias=converteDataDias(vRequisicoes[i].dataDevolucao); // Conta dias entre a data de devolução e o ano MENOR_ANO

        if(dias>max){
            max=dias;
        }
    }
    printf("\nDados Estatisticos:\n\n");
    printf(" ->  Percentagem de Portateis com processador I3: %.2f %%\n",percentagemI3);
    printf(" ->  Percentagem de Portateis com processador I5: %.2f %%\n",percentagemI5);
    printf(" ->  Percentagem de Portateis com processador I7: %.2f %%\n",percentagemI7);

    if(contM==0){
        printf(" ->  Sem multas\n");
    }
    else{
        printf(" ->  Custo medio das multas pagas: %.2f euros\n",mediaMultas);
    }
    if(totalRequisicoes!=0){
        printf(" ->  Tipo(s) de utente(s) com a menor quantidade de requisicoes efetuadas:");
        if(vCont[0]==min){
            printf(" Estudante ");
        }
        if(vCont[1]==min){
            printf(" Docente ");
        }
        if(vCont[2]==min){
            printf(" Tecnico Administrativo ");
        }
        printf("\n");
        if((totalRequisicoes-quantRequisicoesAtivas)==0){ // Verifica se as requisições estão todas ativas, ou seja, se há ou não devoluções
            printf(" ->  Sem devolucoes\n");
        }
        else{
            printf(" ->  Devolucao(oes) mais recente(s) (dd-mm-aaaa):\n");
            for(i=0;i<totalRequisicoes;i++){
                dias=converteDataDias(vRequisicoes[i].dataDevolucao);
                if(dias==max){ // A(s) devolução(ões) mais recente(s) é(são) aquela(s) cujo nº de dias desde o ano MENOR_ANO é igual à variável max calculada anteriormente
                    printf(" ----> Codigo: %s ->  %d - %d - %d\n",vRequisicoes[i].codigo,vRequisicoes[i].dataDevolucao.dia,
                           vRequisicoes[i].dataDevolucao.mes,vRequisicoes[i].dataDevolucao.ano);
                }
            }
        }
    }
    else{
        printf(" ->  Sem Requisicoes\n");
    }
}


