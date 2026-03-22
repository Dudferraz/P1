#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Instituto Politécnico de Leiria - Engenharia Informática (EI)
// Unidade Curricular: Programação I, 1ºS, 2022/2023

// Programa de gestão de requisições de computadores portáteis

// Docente Responsável pela UC: Vítor Távora
// Docente Orientador: Eunice Oliveira

// Autoria do programa:
// PL7 - Diurno, Grupo 7
// Duarte Ferraz, nº 2221447
// Pedro Silva, nº 2222062


#include "funcoesPortateis.h"
#include "funcoesRequisicoes.h"
#include "funcoesAvarias.h"

char menu(int quantPortateis,int quantPortateisDisp,int quantRequisicoesAtivas, int totalRequisicoes,int quantPortateisAvariados);
char submenuDadosRequisicoes(void);
char submenuAvaria_Reparacao(void);
char submenuGravar_LerFicheiroBinario(void);

int main(void)
{
    tipoPortatil vPortateis[MAX_PORTATEIS];

    tipoRequisicao *vRequisicoes;
    vRequisicoes = NULL;

    tipoAvaria *vAvarias;
    vAvarias = NULL;

    int quantPortateis=0, quantPortateisDisp=0;
    int quantRequisicoesAtivas=0, totalRequisicoes=0;
    int quantPortateisAvariados=0, quantAvarias=0;
    char opcao;      // opcao Menu Principal
    char subOpcaoTU; // opcao Submenu listar dados de Todas ou Uma requisicao
    char subOpcaoAR; // opcao Submenu Avaria, Reparacao e dados avarias
    char subOpcaoGL; // opcao Submenu Gravar Ler ficheiro binario

    printf("\n Programacao I - Projeto - 1S - 22/23\n");
    printf(" Programa de gestao de requisicoes de computadores portateis\n");
    printf(" Autores: Duarte Ferraz e Pedro Silva\n");
    do {
        opcao = menu(quantPortateis,quantPortateisDisp,quantRequisicoesAtivas,totalRequisicoes,quantPortateisAvariados);

        switch (opcao){
            case 'A': // Inserir Computadores Portateis
                if(quantPortateis==MAX_PORTATEIS){
                    printf("\n Numero limite de computadores portateis atingido (30)! \n");
                }
                else{
                    quantPortateis=inserePortateis(vPortateis,quantPortateis,&quantPortateisDisp);
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin(); // Ao longo dos switches dos menus a funcao limpaBufferStdin é usada para não ocupar a tela de
                                    // novo com o menu principal/submenus sem o utilizador clicar Enter
                break;

            case 'B': // Listar computadores portateis
                if(quantPortateis==0){
                    printf("\n Sem computadores portateis no sistema! \n");
                }
                else{
                    mostraDadosPortateis(vRequisicoes,vPortateis,totalRequisicoes,quantPortateis);
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin();
                break;

            case 'C': // Registar alteracao da Localizacao de um computador portatil
                if(quantPortateis==0){
                    printf("\n Sem computadores portateis no sistema! \n");
                }
                else{
                    alteraLocalizacao(vPortateis,quantPortateis);
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin();
                break;

            case 'D': // Registar Requisicao de computador portatil
                if(quantPortateis==0){
                    printf("\n Sem computadores portateis no sistema! \n");
                }
                else{
                    if(quantPortateisDisp==0){
                        printf("\n Nao existem computadores portateis disponiveis!\n");
                    }
                    else{
                        vRequisicoes=registarRequisicao(vRequisicoes,vAvarias,quantAvarias,&totalRequisicoes,&quantRequisicoesAtivas,vPortateis,
                                                        quantPortateis,&quantPortateisDisp);
                    }
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin();
                break;

            case 'E': // Listar dados de requisicoes
                if(totalRequisicoes==0){
                    printf("\n Sem requisicoes! \n");
                    printf("\n *Pressione Enter para voltar ao menu principal* \n");
                    limpaBufferStdin();
                }
                else{
                    do{
                        subOpcaoTU=submenuDadosRequisicoes();
                        switch (subOpcaoTU){
                            case 'T': // Listar dados de Todas as requisicoes
                                mostraRequisicoes(vRequisicoes,totalRequisicoes);
                                printf("\n *Pressione Enter para voltar ao submenu* \n");
                                limpaBufferStdin();
                                break;

                            case 'U': // Listar dados de Uma requisicao
                                informacaoRequisicao(vPortateis,vRequisicoes,totalRequisicoes,quantPortateis);
                                printf("\n *Pressione Enter para voltar ao submenu* \n");
                                limpaBufferStdin();
                        }
                    }while(subOpcaoTU!='M');
                }
                break;

            case 'F': // Registar Devolucao de computador portatil
                if(quantPortateis==0){
                    printf("\n Sem computadores portateis no sistema! \n");
                }
                else{
                    if(totalRequisicoes==0){
                        printf("\n Ainda nenhum computador portatil requisitado! \n");
                    }
                    else{
                        if(quantRequisicoesAtivas==0){
                            printf("\n Nao existem requisicoes ativas!\n");
                        }
                        else{
                            registarDevolucao(vPortateis,vRequisicoes,totalRequisicoes,quantPortateis,
                                              &quantPortateisDisp,&quantRequisicoesAtivas);
                        }
                    }
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin();
                break;

            case 'G': // Renovar requisicao de computador portatil
                if(quantPortateis==0){
                    printf("\n Sem computadores portateis no sistema! \n");
                }
                else{
                    if(totalRequisicoes==0){
                        printf("\n Nao existem requisicoes! \n");
                    }
                    else{
                        if(quantRequisicoesAtivas==0){
                            printf("\n Nao existem requisicoes ativas!\n"); // só requisicoes ativas podem ser renovadas
                        }
                        else{
                            renovaRequisicao(vRequisicoes,totalRequisicoes);
                        }
                    }
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin();
                break;

            case 'H': // Avarias/Reparacoes de computadores portateis e dados de avarias
                if(quantPortateis==0){
                    printf("\n Sem computadores portateis no sistema! \n");
                    printf("\n *Pressione Enter para voltar ao menu principal* \n");
                    limpaBufferStdin();
                }
                else{
                    do{
                        subOpcaoAR=submenuAvaria_Reparacao();
                        switch (subOpcaoAR){
                            case 'A': // Registar Avaria de um computador portatil
                                vAvarias=registarAvaria(vAvarias,vPortateis,vRequisicoes,totalRequisicoes,&quantPortateisDisp,
                                                        quantPortateis,&quantAvarias,&quantRequisicoesAtivas,&quantPortateisAvariados);
                                printf("\n *Pressione Enter para voltar ao submenu* \n");
                                limpaBufferStdin();
                                break;

                            case 'R': // Registar Reparacao de um computador portatil
                                if(quantPortateisAvariados==0){
                                    printf("\n Nao existem computadores portateis avariados!\n");
                                }
                                else{
                                    registarReparacao(vAvarias,vPortateis,quantPortateis,quantAvarias,&quantPortateisDisp,&quantPortateisAvariados);
                                }
                                printf("\n *Pressione Enter para voltar ao submenu* \n");
                                limpaBufferStdin();
                                break;

                            case 'L': // Listagem de todas as avarias ocorridas
                                if(quantAvarias==0){
                                    printf("\n Sem avarias registadas!\n");
                                }
                                else{
                                    listaAvarias(vAvarias,vPortateis,quantAvarias,quantPortateis);
                                }
                                printf("\n *Pressione Enter para voltar ao submenu* \n");
                                limpaBufferStdin();
                        }
                    }while(subOpcaoAR!='M');
                }
                break;

            case 'I': // Gravar/Ler dados de computadores portateis e requisicoes
                do{
                    subOpcaoGL=submenuGravar_LerFicheiroBinario();
                    switch (subOpcaoGL){
                        case 'G': // Gravar informacoes dos computadores portateis e requisicoes
                            if(quantPortateis==0){
                                printf("\n Sem informacoes para gravar! \n");
                            }
                            else{
                                gravaFicheiroBinarioPR(vPortateis,vRequisicoes,quantPortateis,quantPortateisDisp,
                                                     quantRequisicoesAtivas,totalRequisicoes,quantPortateisAvariados);
                                gravaFicheiroBinarioA(vAvarias,quantAvarias);
                            }
                            break;

                        case 'L': // Ler informacoes dos computadores portateis e requisicoes
                            vRequisicoes=leFicheiroBinarioPR(vPortateis,vRequisicoes,&quantPortateis,&quantPortateisDisp,
                                                           &quantRequisicoesAtivas,&totalRequisicoes,&quantPortateisAvariados);
                            vAvarias=leFicheiroBinarioA(vAvarias,&quantAvarias);
                    }
                }while(subOpcaoGL!='M');
                break;

            case 'J': // Dados Estatisticos
                if(quantPortateis==0){
                    printf("\n Sem dados estatisticos, sem computadores portateis no sistema!\n");
                }
                else{
                    apresentaDadosEstatisticos(vPortateis,vRequisicoes,quantPortateis,totalRequisicoes,quantRequisicoesAtivas);
                }
                printf("\n *Pressione Enter para voltar ao menu principal* \n");
                limpaBufferStdin();
        }
    }while(opcao != 'S');

    free(vRequisicoes);
    free(vAvarias);

    return 0;
}

// funcao do Menu Principal
char menu(int quantPortateis,int quantPortateisDisp,int quantRequisicoesAtivas,int totalRequisicoes,int quantPortateisAvariados)
{
    // Para o menu principal não foi possível selecionar Letras sugestivas pelo que se optou por Letras em ordem alfabetica
    // Contudo existem 3 submenus que já apresentam Letras sugestivas

    char opcao;
    do{
        printf("\n");
        printf(" ************************ Menu Principal ************************\n\n");
        printf("> Computadores portateis existentes: %d\n", quantPortateis);
        printf("> Computadores portateis disponiveis: %d\n", quantPortateisDisp);
        printf("> Computadores portateis avariados: %d\n", quantPortateisAvariados); // Adicionámos esta informação por acharmos relevante
        printf("> Total de requisicoes efetuadas: %d\n", totalRequisicoes);
        printf("> Requisicoes ativas: %d\n\n", quantRequisicoesAtivas);
        printf(" A - Inserir computadores portateis\n");
        printf(" B - Listar computadores portateis\n");
        printf(" C - Registar alteracao da Localizacao de um computador portatil\n");
        printf(" D - Registar Requisicao de computador portatil\n");
        printf(" E - Listar dados de requisicoes\n");
        printf(" F - Registar Devolucao de computador portatil\n");
        printf(" G - Renovar requisicao de computador portatil\n");
        printf(" H - Avarias/Reparacoes de computadores portateis e dados de avarias\n");
        printf(" I - Gravar/Ler dados de computadores portateis e requisicoes em ficheiro binario\n");
        printf(" J - Dados Estatisticos\n");
        printf(" S - Sair\n\n");
        printf(" Opcao -> ");

        opcao = getchar();
        opcao = toupper(opcao);

        printf("\n");
        limpaBufferStdin();

        if (opcao!='A' && opcao!='B' && opcao!='C' && opcao!='D' && opcao!='E' && opcao!='F' && opcao!='G' && opcao!='H'
            && opcao!='I' && opcao!='J' && opcao!='S'){
            printf(" Opcao invalida!\n");
        }
    }while(opcao!='A' && opcao!='B' && opcao!='C' && opcao!='D' && opcao!='E' && opcao!='F' && opcao!='G' && opcao!='H'
            && opcao!='I' && opcao!='J' && opcao!='S');

    return opcao;
}

// funcao do Submenu Listar dados de Todas ou Uma requisicao
char submenuDadosRequisicoes(void)
{
    char subOpcao;
    do{
        printf("\n");
        printf(" *************** SubMenu Dados de Requisicoes ***************\n\n");
        printf(" T - Listar dados de Todas as requisicoes\n");
        printf(" U - Listar dados de Uma requisicao\n");
        printf(" M - Sair para o Menu principal\n\n");
        printf(" Opcao -> ");

        subOpcao = getchar();
        subOpcao = toupper(subOpcao);

        printf("\n");
        limpaBufferStdin();

        if(subOpcao!='T'&&subOpcao!='U'&&subOpcao!='M'){
            printf(" Opcao invalida!\n");
        }
    }while(subOpcao!='T'&&subOpcao!='U'&&subOpcao!='M');

    return subOpcao;
}

// funcao do Submenu regista Avarias, Reparacoes e Listagem de avarias
char submenuAvaria_Reparacao(void)
{
    char subOpcao;
    do{
        printf("\n");
        printf(" ***************** SubMenu Avaria/Reparacao *****************\n\n");
        printf(" A - Registar Avaria de um computador portatil\n");
        printf(" R - Registar Reparacao de um computador portatil\n");
        printf(" L - Listagem de todas as avarias ocorridas\n");
        printf(" M - Sair para o Menu principal\n\n");
        printf(" Opcao -> ");

        subOpcao = getchar();
        subOpcao = toupper(subOpcao);

        printf("\n");
        limpaBufferStdin();

        if(subOpcao!='A'&&subOpcao!='R'&&subOpcao!='M'&&subOpcao!='L'){
            printf(" Opcao invalida!\n");
        }
    }while(subOpcao!='A'&&subOpcao!='R'&&subOpcao!='M'&&subOpcao!='L');

    return subOpcao;
}

// funcao do Submenu Gravar e Ler Ficheiros Binarios
char submenuGravar_LerFicheiroBinario(void)
{
    char subOpcao;
    do{
        printf("\n");
        printf(" *********** SubMenu Gravar/Ler Ficheiro Binario ***********\n\n");
        printf(" G - Gravar informacoes dos computadores portateis e requisicoes em ficheiro binario\n");
        printf(" L - Ler informacoes dos computadores portateis e requisicoes em ficheiro binario\n");
        printf(" M - Sair para o Menu principal\n\n");
        printf(" Opcao -> ");

        subOpcao = getchar();
        subOpcao = toupper(subOpcao);

        printf("\n");
        limpaBufferStdin();

        if(subOpcao!='G'&&subOpcao!='L'&&subOpcao!='M'){
            printf(" Opcao invalida!\n");
        }
    }while(subOpcao!='G'&&subOpcao!='L'&&subOpcao!='M');

    return subOpcao;
}

