/*
 * File:  project1.c
 * Author:  Cristiano Pantea ist1106324
 * Description: Sistema de gestão de carreiras de transporte público.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structsEprototipos.h"

/* Função erroMemoria: verifica se o alocamento da memória foi bem sucedido */
void erroMemoria(void* memoria, Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    if (memoria == NULL) {
        printf("No memory.\n");
        apagaTudo(ptrHeadCar, ptrHeadPar);
        exit(1);
    }
}

/* Função leEspacos: lê os espaços e devolve 0 se chegou ao final da linha ou 1 
caso contrário. */
int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Função leNome: lê um nome. */
char* leNome(Carreira **ptrHeadCar, Paragem **ptrHeadPar) {
    char s[buffer];
    char* string;
    int i = 0, c, len;
    s[0] = getchar();
    /* Se o nome não tiver aspas */
    if (s[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            s[i++] = c;
        ungetc(c, stdin);
    }
    /* Caso contenha aspas */
    else {
        while((c = getchar()) != '"')
            s[i++] = c;
    }
    s[i++] = '\0';

    len = strlen(s);
    string = (char*) malloc(sizeof(char) * len + 1);
    erroMemoria(&string, ptrHeadCar, ptrHeadPar);
    strcpy(string, s);

    return string;
}

/* Função leAteFinalLinha: lê até ao final da linha. */
void leAteFinalLinha() {
    char c;
    while ((c = getchar()) != '\n');
}

/* Função criaParagem: cria uma paragem */
void criaParagem(char* nomeParagem, double latitude, double longitude,
            Paragem** ptrHeadPar, Paragem** ptrTailPar, Carreira **ptrHeadCar) {

    Paragem* novoElem = malloc(sizeof(Paragem));
    novoElem->nome = (char*) malloc(sizeof(char) * (strlen(nomeParagem) + 1));
    erroMemoria(novoElem, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->nome, ptrHeadCar, ptrHeadPar);
    strcpy(novoElem->nome, nomeParagem);
    novoElem->latitude = latitude;
    novoElem->longitude = longitude;
    novoElem->numCarreiras = 0;
    novoElem->next = NULL;
    novoElem->headNomes = NULL;
    if (*ptrHeadPar == NULL) {
        (*ptrHeadPar) = novoElem;
        (*ptrTailPar) = novoElem;
    }
    else {
        (*ptrTailPar)->next = novoElem;
        (*ptrTailPar) = novoElem; 
    }
}

/* Função criCarreira: cria uma carreira. */
void criaCarreira(char* nomeParagem, Carreira** ptrHeadCar, 
                  Carreira** ptrTailCar, Paragem **ptrHeadPar) {

    Carreira* novoElem = malloc(sizeof(Carreira));
    novoElem->nome = malloc(sizeof(char) * (strlen(nomeParagem) + 1));
    erroMemoria(novoElem, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->nome, ptrHeadCar, ptrHeadPar);
    strcpy(novoElem->nome, nomeParagem);
    novoElem->ligHead = NULL;
    novoElem->ligTail = NULL;
    novoElem->next = NULL;
    novoElem->custoTotal = 0;
    novoElem->duracaoTotal = 0;
    novoElem->numLigacoes = 0;
    if (*ptrHeadCar == NULL) {
        (*ptrHeadCar) = novoElem;
        (*ptrTailCar) = novoElem;
    }
    else {
        (*ptrTailCar)->next = novoElem;
        (*ptrTailCar) = novoElem; 
    }
}

/* Função encontraParagem: verifica se a paragem existe, se esta existir,
devolve um ponteiro para a mesma, caso contrário devolve NULL. */
Paragem* encontraParagem(Paragem **ptrHeadParagem, char* nomeParagem) {

    Paragem* aux = *ptrHeadParagem;

    while (aux) {
        if (strcmp(aux->nome, nomeParagem) == 0) 
            break;
        aux = aux->next;
    }
    return aux;
}

/* Função encontraParagemPrev: verifica se a paragem existe, se esta existir,
devolve um ponteiro para a paragem anterior à pedida, caso contrário devolve
NULL. */
Paragem* encontraParagemPrev(Paragem **ptrHeadParagem, char* nomeParagem) {

    Paragem* aux = *ptrHeadParagem;

    while (aux) {
        if (strcmp(aux->next->nome, nomeParagem) == 0) 
            break;
        aux = aux->next;
    }
    return aux;
}

/* Função encontraCarreira: verifica se a carreira já existe, se esta existir,
devolve um ponteiro para a mesma, caso contrário devolve NULL. */
Carreira* encontraCarreira(Carreira **ptrHeadCarreira, char* nomeCarreira) {

    Carreira *aux = *ptrHeadCarreira;

    while (aux) {
        if (strcmp(aux->nome, nomeCarreira) == 0) 
            break;
        aux = aux->next;
    }
    return aux;
}

/* Função encontraCarreiraPrev: verifica se a carreira já existe, se esta 
existir, devolve um ponteiro para a carreira anterior à pedida, caso contrário
devolve NULL. */
Carreira* encontraCarreiraPrev(Carreira **ptrHeadCarreira, char* nomeCarreira) {

    Carreira* aux = *ptrHeadCarreira;

    while (aux) {
        if (strcmp(aux->next->nome, nomeCarreira) == 0) 
            break;
        aux = aux->next;
    }
    return aux;
}

/* Função veerificaInversoOk: verifica que o argumento inverso foi dado
corretamente. */
int verificaInversoOk(char *s) {
    char inv[] = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADE;
}

/* Função adicionaPrimeiraLigacao: adiciona a primeira ligação de uma 
carreira. */
void adicionaPrimeiraLigacao(Carreira **ptrCarreira, char *nomeOrig,
                             char* nomeDest, double custo, double duracao,
                             Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    Ligacao *novoElem = malloc(sizeof(Ligacao));
    novoElem->paragOrig = malloc(sizeof(char) * (strlen(nomeOrig) + 1));
    novoElem->paragDest = malloc(sizeof(char) * (strlen(nomeDest) + 1));
    erroMemoria(novoElem, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->paragOrig, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->paragDest, ptrHeadCar, ptrHeadPar);
    strcpy(novoElem->paragOrig, nomeOrig);
    strcpy(novoElem->paragDest, nomeDest);
    novoElem->custo = custo;
    novoElem->duracao = duracao;
    novoElem->anterior = NULL;
    novoElem->prox = NULL;
    (*ptrCarreira)->ligHead = novoElem;
    (*ptrCarreira)->ligTail = novoElem;
    (*ptrCarreira)->custoTotal = custo;
    (*ptrCarreira)->duracaoTotal = duracao;
    (*ptrCarreira)->numLigacoes = 1;
}

/* Função adicionaLigacaoAnterior: adiciona uma ligação ao inicio da
carreira. */
void adicionaLigacaoAnterior(Carreira **ptrCar, char *nomeOrig, char *nomeDest,
                            double custo, double duracao, Carreira **ptrHeadCar,
                            Paragem **ptrHeadPar) {
    
    Ligacao *novoElem = malloc(sizeof(Ligacao));
    novoElem->paragOrig = malloc(sizeof(char) * (strlen(nomeOrig) + 1));
    novoElem->paragDest = malloc(sizeof(char) * (strlen(nomeDest) + 1));
    erroMemoria(novoElem, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->paragOrig, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->paragDest, ptrHeadCar, ptrHeadPar);
    strcpy(novoElem->paragOrig, nomeOrig);
    strcpy(novoElem->paragDest, nomeDest);
    novoElem->custo = custo;
    novoElem->duracao = duracao;
    (*ptrCar)->ligHead->anterior = novoElem;
    novoElem->anterior = NULL;
    novoElem->prox = (*ptrCar)->ligHead;
    (*ptrCar)->ligHead = novoElem;
    (*ptrCar)->custoTotal += custo;
    (*ptrCar)->duracaoTotal += duracao;
    (*ptrCar)->numLigacoes += 1;
}

/* Função adicionaLigacaoFim: adiciona uma ligação ao fim da carreira. */
void adicionaLigacaoFim(Carreira **ptrCar, char *nomeOrig, char *nomeDest,
                        double custo, double duracao, Carreira **ptrHeadCar,
                        Paragem **ptrHeadPar) {
    
    Ligacao *novoElem = malloc(sizeof(Ligacao));
    novoElem->paragOrig = malloc(sizeof(char) * (strlen(nomeOrig) + 1));
    novoElem->paragDest = malloc(sizeof(char) * (strlen(nomeDest) + 1));
    erroMemoria(novoElem, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->paragOrig, ptrHeadCar, ptrHeadPar);
    erroMemoria(novoElem->paragDest, ptrHeadCar, ptrHeadPar);
    strcpy(novoElem->paragOrig, nomeOrig);
    strcpy(novoElem->paragDest, nomeDest);
    novoElem->custo = custo;
    novoElem->duracao = duracao;
    (*ptrCar)->ligTail->prox = novoElem;
    novoElem->anterior = (*ptrCar)->ligTail;
    novoElem->prox = NULL;
    (*ptrCar)->ligTail = novoElem;
    (*ptrCar)->custoTotal += custo;
    (*ptrCar)->duracaoTotal += duracao;
    (*ptrCar)->numLigacoes += 1;
}

/* Função adicionaCarreira: adiciona o nome da carreira à lista de nomes
de carreiras de uma determinada paragem. */
void adicionaCarreira(Paragem **ptrParagem, char *nomeCarreira, 
                     Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    CarreirasNomes *aux = malloc(sizeof(CarreirasNomes));
    aux->nomeCarreira = (char*) malloc(sizeof(char) * (strlen(nomeCarreira) + 1));
    erroMemoria(aux, ptrHeadCar, ptrHeadPar);
    erroMemoria(aux->nomeCarreira, ptrHeadCar, ptrHeadPar);
    strcpy(aux->nomeCarreira, nomeCarreira);
    aux->next = (*ptrParagem)->headNomes;
    (*ptrParagem)->headNomes = aux;
}

/* Função verificaAdicionaCar: verifica se é necessário adicionar o nome da
carreira à lista de nomes de cada paragem. */
void verificaAdicionaCar(Paragem **ptrParagem, Carreira **ptrCarreira,
                         Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    CarreirasNomes *aux = (*ptrParagem)->headNomes;

    while (aux) {
        /* Se o nome já estiver na lista, não adiciona. */
        if (strcmp(aux->nomeCarreira, (*ptrCarreira)->nome) == 0)
            break;
        aux = aux->next;
    }
    /* Se não encontrou adiciona. */
    if (aux == NULL) {
        adicionaCarreira(ptrParagem, (*ptrCarreira)->nome, ptrHeadCar, ptrHeadPar);
        (*ptrParagem)->numCarreiras += 1;
    }
}

/* Função adicionaLigacao: adiciona a ligação.*/
void adicionaLigacao(Carreira **ptrCarreira, Paragem **ptrParagOrig,
                     Paragem **ptrParagDest, double custo, double duracao,
                     Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    int numLigacoesCarreira = (*ptrCarreira)->numLigacoes;
    int estado = FALSO;

    /* Adiciona as ligações de acordo com o que foi inserido pelo utilizador. */
    if (numLigacoesCarreira == 0) {
        adicionaPrimeiraLigacao(ptrCarreira, (*ptrParagOrig)->nome, 
        (*ptrParagDest)->nome, custo, duracao, ptrHeadCar, ptrHeadPar);
        estado = VERDADE;
    }        
    else {
        if (strcmp(((*ptrCarreira)->ligTail->paragDest), (*ptrParagOrig)->nome) == 0) {
            adicionaLigacaoFim(ptrCarreira, (*ptrParagOrig)->nome,
                               (*ptrParagDest)->nome, custo, duracao,
                                ptrHeadCar, ptrHeadPar);
            estado = VERDADE;
        }
        else if (strcmp((*ptrCarreira)->ligHead->paragOrig, (*ptrParagDest)->nome) == 0) {
            adicionaLigacaoAnterior(ptrCarreira, (*ptrParagOrig)->nome,
                                    (*ptrParagDest)->nome, custo, duracao,
                                     ptrHeadCar, ptrHeadPar);
            estado = VERDADE;
        }
        /* Caso não tenha entrado em nenhum dos casos anteriores, esta não
        pode ser adicionada. */
        else
            printf("link cannot be associated with bus line.\n");
    }
    if (estado == VERDADE) {
        verificaAdicionaCar(ptrParagOrig, ptrCarreira, ptrHeadCar, ptrHeadPar);
        verificaAdicionaCar(ptrParagDest, ptrCarreira, ptrHeadCar, ptrHeadPar);
    }
}

/* Função mostraLigacoesCarreira: printa as ligações da carreira pela ordem 
normal. */
void mostraLigacoesCarreira(Carreira **ptrCarreira) {

    Ligacao *aux = (*ptrCarreira)->ligHead;

    if ((*ptrCarreira)->ligHead == NULL || (*ptrCarreira)->ligTail == NULL)
        return;
    
    printf("%s, ", aux->paragOrig);
    while (aux->prox) {
        printf("%s, ", aux->paragDest);
        aux = aux->prox;
    }
    printf("%s\n", aux->paragDest);
}

/* Função mostraLigacoesCarreiraInv: printa as ligações da carreira por ordem
inversa. */
void mostraLigacoesCarreiraInv(Carreira **ptrCarreira) {

    Ligacao *aux = (*ptrCarreira)->ligTail;
    printf("%s, ", aux->paragDest);
    while (aux->anterior) {
        printf("%s, ", aux->paragOrig);
        aux = aux->anterior;
    }
    printf("%s\n", aux->paragOrig);
}

/* Função listaParagens: lista as paragens e as suas informações. */
void listaParagens(Paragem **ptrHeadParagem) {
    
    Paragem *aux = *ptrHeadParagem;

    if ((*ptrHeadParagem) == NULL)
        return;

    while (aux->next) {
        printf("%s: %16.12f %16.12f %d\n", aux->nome, aux->latitude,
        aux->longitude, aux->numCarreiras);
        aux = aux->next;
    }
    printf("%s: %16.12f %16.12f %d\n", aux->nome, aux->latitude,
        aux->longitude, aux->numCarreiras);
}

/* Função listaCarreiras: lista as carreiras e as suas informações. */
void listaCarreiras(Carreira **ptrHead) {

    Carreira *aux = *ptrHead;
    if ((*ptrHead) == NULL) {
        return;
    }

    while (aux->next) {
        printf("%s ", aux->nome);
        if (aux->numLigacoes > 0) {
            printf("%s %s %d %.2f %.2f\n", aux->ligHead->paragOrig,
            aux->ligTail->paragDest, aux->numLigacoes+1, aux->custoTotal,
            aux->duracaoTotal);
        }
        else    
            printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
        aux = aux->next;
    }
    printf("%s ", aux->nome);
    if (aux->numLigacoes > 0) {
        printf("%s %s %d %.2f %.2f\n", aux->ligHead->paragOrig,
            aux->ligTail->paragDest, aux->numLigacoes+1, aux->custoTotal,
            aux->duracaoTotal);
    }
    else
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
    
}

/* Função paragens: recebe o comando p. */
void paragens(Carreira **ptrHeadCar, Paragem **ptrHeadPar, 
              Paragem **ptrTailPar) {
    char *nomeParagem;
    int fimLinha = leEspacos();
    Paragem* aux;

    if (!fimLinha) { /* 0 argumentos. */
        listaParagens(ptrHeadPar);
        return;
    }
    nomeParagem = leNome(ptrHeadCar, ptrHeadPar);
    fimLinha = leEspacos();
    if (!fimLinha) { /* 1 argumento. */
        aux = encontraParagem(ptrHeadPar, nomeParagem);
        if (aux == NULL)
            printf("%s: no such stop.\n", nomeParagem);
        else   
            printf("%16.12f %16.12f\n", aux->latitude, aux->longitude);
    }
    else { /* 3 argumentos. */
        double latitude, longitude;
        if(scanf("%lf%lf", &latitude, &longitude)) {
            aux = encontraParagem(ptrHeadPar, nomeParagem);
            if (aux == NULL) {
                criaParagem(nomeParagem, latitude, longitude, ptrHeadPar,
                ptrTailPar, ptrHeadCar);
            }
            else
                printf("%s: stop already exists.\n", nomeParagem);
            leAteFinalLinha();
        }
    }
    free(nomeParagem);
}

/* Função carreiras: recebe o comando c. */
void carreiras(Paragem **ptrHeadPar, Carreira** ptrHeadCar,
               Carreira** ptrTailCar) {
    char *nomeCarreira, *verInv;
    int fimLinha = leEspacos();
    Carreira* aux;

    if (!fimLinha) { /* 0 argumentos. */
        listaCarreiras(ptrHeadCar);
        return;
    }

    nomeCarreira = leNome(ptrHeadCar, ptrHeadPar);
    fimLinha = leEspacos();
    if (!fimLinha) { /* 1 argumento. */
        aux = encontraCarreira(ptrHeadCar, nomeCarreira);
        if (aux == NULL) {
            criaCarreira(nomeCarreira, ptrHeadCar, ptrTailCar, ptrHeadPar);
        }
        else {
            mostraLigacoesCarreira(&aux);
        }
    }
    else { /* 2 argumentos. */
        verInv = leNome(ptrHeadCar, ptrHeadPar);
        aux = encontraCarreira(ptrHeadCar, nomeCarreira);
        if (verificaInversoOk(verInv)) {
            mostraLigacoesCarreiraInv(&aux);
            free(verInv);
        }
        else {
            printf("incorrect sort option.\n");
            free(verInv);
        }
        leAteFinalLinha();
    }
    free(nomeCarreira);
}

/* Função ligacoes: recebe o comando l. */
void ligacoes(Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    char *nomeCarreira, *nomeOrigem, *nomeDestino;
    double custo, duracao;
    Carreira *carAux;
    Paragem *parAuxOrig, *parAuxDest;

    leEspacos();
    nomeCarreira = leNome(ptrHeadCar, ptrHeadPar);
    leEspacos();
    nomeOrigem = leNome(ptrHeadCar, ptrHeadPar);
    leEspacos();
    nomeDestino = leNome(ptrHeadCar, ptrHeadPar);

    if(scanf("%lf%lf", &custo, &duracao))
    leAteFinalLinha();

    carAux = encontraCarreira(ptrHeadCar, nomeCarreira);

    /* Verifica os erros. */
    if (carAux == NULL)
        printf("%s: no such line.\n", nomeCarreira);
    else {
        parAuxOrig = encontraParagem(ptrHeadPar, nomeOrigem);
        if (parAuxOrig == NULL) 
	    printf("%s: no such stop.\n", nomeOrigem);
	else {
	    parAuxDest = encontraParagem(ptrHeadPar, nomeDestino);
	    if (parAuxDest == NULL)
	        printf("%s: no such stop.\n", nomeDestino);
	    else if (custo < 0.0 || duracao < 0.0)
	            printf("negative cost or duration.\n");

        /* Adiciona a ligação caso não tenha sido encontrado nenhum erro.*/
		else 
		    adicionaLigacao(&carAux, &parAuxOrig, &parAuxDest, custo, duracao,
            ptrHeadCar, ptrHeadPar);
	}
    }
    free(nomeCarreira);
    free(nomeOrigem);
    free(nomeDestino);
}

/* Função freeIntersecoes: liberta a memória alocada para ordenar as carreiras
no comando i. */
void freeIntersecoes(char **arrayOrganiza, int nCarreiras) {

    int i;
    for (i = 0; i < nCarreiras; i++) {
        free(arrayOrganiza[i]);
    }
}

/* Função ordenaCarreiras: ordena os nomes das carreiras de uma determinada
paragem. */
void ordenaCarreiras(Paragem **ptrParagem, Carreira **ptrHeadCar,
                     Paragem **ptrHeadPar) {

    CarreirasNomes *aux = (*ptrParagem)->headNomes;
    int nCarreiras = (*ptrParagem)->numCarreiras;
    int i, j;
    char **arrayOrganiza = malloc(sizeof(char*) * nCarreiras);
    erroMemoria(arrayOrganiza, ptrHeadCar, ptrHeadPar);

    /* Aloca a memória para os nomes das carreiras e copia os nomes da lista
    para o array que vai organizar. */
    for (i = 0; i < nCarreiras; i++) {
        arrayOrganiza[i] = (char*) malloc(sizeof(char) * (strlen(aux->nomeCarreira) + 1));
        erroMemoria(arrayOrganiza[i], ptrHeadCar, ptrHeadPar);
        strcpy(arrayOrganiza[i], aux->nomeCarreira);
        aux = aux->next;
    }

    /* Faz a ordenação. */
    for (i = 0; i < nCarreiras - 1; i++) {
        for (j = 0; j < nCarreiras - i - 1; j++) {
            if (strcmp(arrayOrganiza[j], arrayOrganiza[j+1]) > 0) {
                char *fazTroca = malloc(sizeof(char) * (strlen(arrayOrganiza[j]) + 1));
                strcpy(fazTroca, arrayOrganiza[j]);
                strcpy(arrayOrganiza[j], arrayOrganiza[j+1]);
                strcpy(arrayOrganiza[j+1], fazTroca);
                free(fazTroca);
            }
        }
    }
    
    printf("%s %d: ", (*ptrParagem)->nome, nCarreiras);
    for (i = 0; i < nCarreiras - 1; i++) {
        printf("%s ", arrayOrganiza[i]);
    }
    printf("%s\n", arrayOrganiza[i]);
    freeIntersecoes(arrayOrganiza, nCarreiras);
    free(arrayOrganiza);
}

/* Função organizaEverificaI: vai verificar se é necessário ordernar as
carreiras, caso seja necessário organiza. */
void organizaEverificaI(Paragem **ptrParagem, Carreira **ptrHeadCar, 
                        Paragem **ptrHeadPar) {

    if ((*ptrParagem)->numCarreiras > 1) {
        ordenaCarreiras(ptrParagem, ptrHeadCar, ptrHeadPar);
    }
}

/* Função resetaCarreira: reseta uma determinada carreira. */
void resetaCarreira(Carreira **ptrCarreira) {

    Carreira *temp = (*ptrCarreira);
    free(temp->ligHead->paragOrig);
    free(temp->ligHead->paragDest);
    free(temp->ligHead);
    temp->numLigacoes = 0;
    temp->duracaoTotal = 0;
    temp->custoTotal = 0;
    (*ptrCarreira)->ligHead = NULL;
}

/* Função intersecoes: recebe o compando i. */
void intersecoes(Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    Paragem *aux = *ptrHeadPar;

    while (aux) {
        organizaEverificaI(&aux, ptrHeadCar, ptrHeadPar);
        aux = aux->next;
    }
}

/* Elimina o nome de uma determinada carreira de todas as listas de todas 
as paragens. */
void freeDasParag(Paragem **ptrHeadPar, char *nomeCarreira) {

    CarreirasNomes *auxNomesCar, *nextNomeCar, *prevNomeCar;
    Paragem *aux = (*ptrHeadPar);

    while (aux) {
        auxNomesCar = aux->headNomes;
        while (auxNomesCar) {
            nextNomeCar = auxNomesCar->next;
            /* Se o nome estiver na head */
            if ((strcmp(auxNomesCar->nomeCarreira, nomeCarreira) == 0) &&
                 auxNomesCar == aux->headNomes) {
                aux->headNomes = nextNomeCar;
                aux->numCarreiras -= 1;
                free(auxNomesCar->nomeCarreira);
                free(auxNomesCar);
                break;
            }
            /* Se o nome do próximo da lista corresponder, mas não for o da 
            head. */
            else if (auxNomesCar->next == NULL) {
                auxNomesCar = nextNomeCar;
                break;
            }
            /* Caso contrário. */
            else if (strcmp(auxNomesCar->next->nomeCarreira, nomeCarreira) == 0) {
                prevNomeCar = auxNomesCar;
                auxNomesCar = auxNomesCar->next;
                nextNomeCar = auxNomesCar->next;
                prevNomeCar->next = nextNomeCar;
                aux->numCarreiras -= 1;
                free(auxNomesCar->nomeCarreira);
                free(auxNomesCar);
                break;
            }
            auxNomesCar = nextNomeCar;
        }   
        aux = aux->next;
    }
}

/* Função freeLigacoesA: elimina todas as ligações de uma determinada 
carreira. */
void freeLigacoesA(Carreira **ptrCarreira) {

    int i = 0;
    Ligacao *next;
    Ligacao *current = (*ptrCarreira)->ligHead;

    while (i < (*ptrCarreira)->numLigacoes) {
        next = current->prox;
        free(current->paragDest);
        free(current->paragOrig);
        free(current);
        current = next;
        i++;
    }
    free(current);
    
}

/* Função encontraParagemLig: devolve FALSO se uma determinada paragem não
existir nas ligações de uma carreira e devolve VERDADEIRO caso contrário. */
int encontraParagemLig(Carreira **carreira, char *nomeParagem) {

    Ligacao *aux = (*carreira)->ligHead;

    while (aux) {
        if (strcmp(aux->paragOrig, nomeParagem) == 0 ||
            strcmp(aux->paragDest, nomeParagem) == 0)
            return VERDADE;
        aux = aux->prox;
    }
    return FALSO;
}

/* Função criaLigacao: cria uma nova ligação tendo em conta a origem de uma
ligação recebida e o destino de outra ligação recebida. */
void criaLigacao(Ligacao **exclui1, Ligacao **exclui2, Carreira **ptrCarreira, 
    double custo, double duracao, Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    Ligacao *junta = malloc(sizeof(Ligacao));
    erroMemoria(junta, ptrHeadCar, ptrHeadPar);
    (*ptrCarreira)->numLigacoes -= 1;
    junta->custo = custo;
    junta->duracao = duracao;
    junta->custo += (*exclui1)->custo + (*exclui2)->custo;
    junta->duracao += (*exclui1)->duracao + (*exclui2)->duracao;
    junta->anterior = (*exclui1)->anterior;
    junta->prox = (*exclui2)->prox;

    if ((*exclui1)->anterior != NULL)
        (*exclui1)->anterior->prox = junta;
    else
        (*ptrCarreira)->ligHead = junta;  

    if ((*exclui2)->prox != NULL)
        (*exclui2)->prox->anterior = junta;
    else
        (*ptrCarreira)->ligTail = junta;


    junta->paragOrig = (char*) malloc(sizeof(char) * (strlen((*exclui1)->paragOrig) + 1));
    junta->paragDest = (char*) malloc(sizeof(char) * (strlen((*exclui2)->paragOrig) + 1));
    erroMemoria(junta->paragOrig, ptrHeadCar, ptrHeadPar);
    erroMemoria(junta->paragDest, ptrHeadCar, ptrHeadPar);
    strcpy(junta->paragOrig, (*exclui1)->paragOrig);
    strcpy(junta->paragDest, (*exclui2)->paragDest);
    
    free((*exclui1)->paragOrig);
    free((*exclui1)->paragDest);
    free((*exclui1));
    free((*exclui2)->paragOrig);
    free((*exclui2)->paragDest);
    free((*exclui2));
}

/* Função encontraExcluiFim: devolve um ponteiro para a ligação cuja paragem 
destino é diferente da paragem que é necessário excluir. */
Ligacao* encontraExcluiFim(Ligacao **excluiInicio, char *nomeParag, 
                           Carreira **ptrCarreira, double *custo, 
                           double *duracao) {

    Ligacao *central = (*excluiInicio);
    Ligacao *exclui, *antes, *proximo;
    (*custo) = 0;
    (*duracao) = 0;

    /* Quando a ligação seguinte à primeira que é necessário excluir já
    tem o destino diferente da paragem que é necessário retirar. */
    if (strcmp(central->prox->paragDest, nomeParag) != 0) {
        return central->prox;
    }
    /* Caso contrário vai procurar a ligação e vai excluindo as que estão no
    meio. */
    else {
        while (strcmp(central->prox->paragDest, nomeParag) == 0) {
            antes = central;
            central = central->prox;
            exclui = central;
            (*custo) += central->custo;
            (*duracao) += central->duracao;
            (*ptrCarreira)->numLigacoes -= 1;
            if (exclui->prox) {
                proximo = exclui->prox;
                antes->prox = proximo;
                proximo->anterior = antes;
                free(central->paragOrig);
                free(central->paragDest);
                free(central);
            }
            central = antes;
        }
        return central->prox;
    }
}

/* Função tiraLigacoesMeio: elimina as ligações que contenham uma determinada 
paragem e que se situam no centro de uma carreira. */
void tiraLigacoesMeio(Carreira **carreira, char *nomeParagem, 
                      Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    Ligacao *aux = (*carreira)->ligHead;
    Ligacao *exclui1, *exclui2;
    double custo, duracao;

    while (aux) {
        if (strcmp(aux->paragDest, nomeParagem) == 0) {
            exclui1 = aux;
            break;
        }
        aux = aux->prox;
    }
    exclui2 = encontraExcluiFim(&exclui1, nomeParagem, carreira, &custo, &duracao);
    criaLigacao(&exclui1, &exclui2, carreira, custo, duracao, ptrHeadCar, ptrHeadPar);
}

/* Função eliminaNomesCarreiras: elimina todos os nomes de carreiras presentes
na lista de nomes de uma determinada paragem. */
void eliminaNomesCarreiras(Paragem **paragemApagar) {

    CarreirasNomes *current, *next;

    current = (*paragemApagar)->headNomes;
    
    while (current) {
        next = current->next;
        free(current->nomeCarreira);
        free(current);
        current = next;
    }
}

/* Função eliminaParagem: elimina uma determinada paragem do sistema. */
void eliminaParagem(Paragem **headParagens, Paragem **paragemApagar,
                    char *nomeParagem) {

    Paragem *prev, *next, *current;

    /* Se for a head */
    if (strcmp((*headParagens)->nome, nomeParagem) == 0) {
        current = (*headParagens);
        next = (*headParagens)->next;
        /* Se for a head e tiver alguma a seguir */
        if (next) {
            (*headParagens) = next;
            eliminaNomesCarreiras(paragemApagar);
            free(current->nome);
            free(current);
        }
        /* Se for a head e não tiver nada a seguir */
        else {
            (*headParagens) = NULL;
            eliminaNomesCarreiras(paragemApagar);
            free(current->nome);
            free(current);
        }
    }
    /* Caso não seja a head */
    else {
        prev = encontraParagemPrev(headParagens, nomeParagem);
        current = prev->next;
        next = current->next;
        /* Se existir a uma paragem depois da que eu quero eliminar */
        if (next) {
            eliminaNomesCarreiras(paragemApagar);
            prev->next = next;
            free(current->nome);
            free(current);
        }
        /* Se não existir nenhuma paragem depois da que eu quero eliminar */
        else {
            eliminaNomesCarreiras(paragemApagar);
            prev->next = NULL;
            free(current->nome);
            free(current);
        }
    }
}

/* Função removeCarListaParag: remove o nome de uma determinada carreira da
lista de uma determinada paragem. */
void removeCarListaParag(char *nomeParagem, char *nomeCarreira, 
                         Paragem **ptrHeadPar) {

    Paragem *paragemRemove;
    CarreirasNomes *aux, *remover;

    paragemRemove = encontraParagem(ptrHeadPar, nomeParagem);

    /* Se o nome da carreira que for pra retirar é o da cabeça */
    if (strcmp(paragemRemove->headNomes->nomeCarreira, nomeCarreira) == 0) {
        aux = paragemRemove->headNomes;
        paragemRemove->headNomes = paragemRemove->headNomes->next;
        free(aux->nomeCarreira);
        free(aux);
        paragemRemove->numCarreiras -= 1;
        return;
    }
    /* Caso haja mais, encontrar o nome */
    else {
        aux = paragemRemove->headNomes;
        while (aux->next) {
            if (strcmp(aux->next->nomeCarreira, nomeCarreira) == 0) {
                break;
            }
            aux = aux->next;
        }
        remover = aux->next;

        if (remover == NULL) {
            return;
        }
        else {
        aux->next = remover->next;
        free(remover->nomeCarreira);
        free(remover);
        paragemRemove->numCarreiras -= 1;
        }
    }
}

/* Função funcaoTiraParagem: elimina as ligações de uma determinada carreira
que contenham uma determinada paragem. */
void funcaoTiraParagem(Carreira **carreira, char *nomeParagem, 
                       Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    Ligacao *current;
    char *nomeOrig, *nomeDest;

    while (encontraParagemLig(carreira, nomeParagem)) {
    
        /* Se o número de ligações for um */
        if ((*carreira)->numLigacoes == 1) {
            nomeOrig = (*carreira)->ligHead->paragOrig;
            nomeDest = (*carreira)->ligTail->paragDest;
            /* Se a origem e o destino forem a paragem a eliminar, resetar a 
            paragem */
            if (strcmp(nomeParagem, nomeOrig) == 0 && 
            strcmp(nomeParagem, nomeDest) == 0)
                resetaCarreira(carreira);
            /* Se a paragem a tirar for a origem, tirar a carreira da lista das 
            carreiras da paragem destino */
            else if (strcmp(nomeParagem, nomeOrig) == 0) {
                removeCarListaParag(nomeDest, (*carreira)->nome, ptrHeadPar);
                resetaCarreira(carreira);
            }
            /* Se a paragem a tirar for a destino, tirar a carreira da lista das 
            carreiras da paragem origem */
            else if (strcmp(nomeParagem, nomeDest) == 0) {
                removeCarListaParag(nomeOrig, (*carreira)->nome, ptrHeadPar);
                resetaCarreira(carreira);
            }
        }
        else {
            /* Se a paragem a eliminar for a origem */
            if (strcmp((*carreira)->ligHead->paragOrig, nomeParagem) == 0) {
                current = (*carreira)->ligHead;
                (*carreira)->ligHead = current->prox;
                (*carreira)->ligHead->anterior = NULL;
                (*carreira)->numLigacoes -= 1;
                (*carreira)->custoTotal -= current->custo;
                (*carreira)->duracaoTotal -= current->duracao;
                free(current->paragOrig);
                free(current->paragDest);
                free(current);
                }
            /* Se a paragem a eliminar for a destino */
            else if (strcmp((*carreira)->ligTail->paragDest, nomeParagem) == 0) {
                current = (*carreira)->ligTail;
                (*carreira)->ligTail = current->anterior;
                (*carreira)->ligTail->prox = NULL;
                (*carreira)->numLigacoes -= 1;
                (*carreira)->custoTotal -= current->custo;
                (*carreira)->duracaoTotal -= current->duracao;
                free(current->paragOrig);
                free(current->paragDest);
                free(current);
            }
            /* Se tiver no meio */
            else {
                tiraLigacoesMeio(carreira, nomeParagem, ptrHeadCar, ptrHeadPar);
            }
        }
    }
}

/* Função removePar: elimina uma determinada paragem do sistema. */
void removePar(Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    char *nomeParagem;
    Paragem *paragemApagar;
    CarreirasNomes *tiraDasLig;
    Carreira *tiraLigacoes;

    leEspacos();
    nomeParagem = leNome(ptrHeadCar, ptrHeadPar);
    leAteFinalLinha();

    paragemApagar = encontraParagem(ptrHeadPar, nomeParagem);
    
    if (paragemApagar == NULL) {
        printf("%s: no such stop.\n", nomeParagem);
        return;
    }
    tiraDasLig = paragemApagar->headNomes;
    /* Elimina a paragem das ligações de cada carreira dessa paragem */
    while (tiraDasLig) {
        tiraLigacoes = encontraCarreira(ptrHeadCar, tiraDasLig->nomeCarreira);
        funcaoTiraParagem(&tiraLigacoes, nomeParagem, ptrHeadCar, ptrHeadPar);
        tiraDasLig = tiraDasLig->next;
    }
    eliminaParagem(ptrHeadPar, &paragemApagar, nomeParagem);
    free(nomeParagem);
}

/* Função removeCar: elimina uma determinada carreira do sistema. */
void removeCar(Carreira **ptrHeadCar, Paragem **ptrHeadPar) {

    Carreira *prev, *carCerta;
    char* nomeCarreira;

    leEspacos();
    nomeCarreira = leNome(ptrHeadCar, ptrHeadPar);
    carCerta = encontraCarreira(ptrHeadCar, nomeCarreira);

    if (carCerta == NULL) {
        printf("%s: no such line.\n", nomeCarreira);
        return;
    }
    else {
        if (carCerta == (*ptrHeadCar)) {
            if (carCerta->numLigacoes != 0)
                freeLigacoesA(&carCerta);
            freeDasParag(ptrHeadPar, nomeCarreira);
            (*ptrHeadCar) = (*ptrHeadCar)->next;
            free(carCerta->nome);
            free(carCerta);
        }
        else {
            if (carCerta->numLigacoes != 0)
                freeLigacoesA(&carCerta);
            freeDasParag(ptrHeadPar, nomeCarreira);
            prev = encontraCarreiraPrev(ptrHeadCar, nomeCarreira);
            carCerta = prev->next;
            prev->next = carCerta->next;
            free(carCerta->nome);
            free(carCerta);
        }
    }
    free(nomeCarreira);
}

/* Função freeNomesCarreiras: elimina todos os nomes de carreiras de uma 
determinada lista de carreiras. */
void freeNomesCarreiras(CarreirasNomes **headNomes) {

    CarreirasNomes *auxNext;
    CarreirasNomes *auxCurrent = (*headNomes);

    while (auxCurrent) {
        auxNext = auxCurrent->next;
        free(auxCurrent->nomeCarreira);
        free(auxCurrent);
        auxCurrent = auxNext;
    }
}

/* Função apagaTudo: elimina todos os dados do sistema. */
void apagaTudo(Carreira **ptrHeadCar, Paragem **ptrHeadParag) {

    Paragem *auxParNext;
    Carreira *auxCarNext;
    Carreira *auxCarCurrent = (*ptrHeadCar);
    Paragem *auxParCurrent = (*ptrHeadParag);

    /* Elimina as carreiras e tudo o que estas contenham, incluindo as suas
    ligações. */
    while (auxCarCurrent) {
        auxCarNext = auxCarCurrent->next;
        freeLigacoesA(&auxCarCurrent);
        free(auxCarCurrent->nome);
        free(auxCarCurrent);
        auxCarCurrent = auxCarNext;
    }
    (*ptrHeadCar) = NULL;

    /* Elimina as paragens e tudo o que estas contenham, incluindo os nomes
    de cada carreira que por essa paragem passa. */
    while (auxParCurrent) {
        auxParNext = auxParCurrent->next;
        free(auxParCurrent->nome);
        freeNomesCarreiras(&auxParCurrent->headNomes);
        free(auxParCurrent);
        auxParCurrent = auxParNext;
    }
    (*ptrHeadParag) = NULL;
}

int main() {

    int c;
    Paragem *ptrHeadParag = NULL, *ptrTailParag = NULL;
    Carreira *ptrHeadCar = NULL, *ptrTailCar = NULL;

    do {
        c = getchar();
        switch(c) {
            case 'c':
                carreiras(&ptrHeadParag, &ptrHeadCar, &ptrTailCar);
                break;
            case 'p':
                paragens(&ptrHeadCar, &ptrHeadParag, &ptrTailParag);
                break;
            case 'l':
                ligacoes(&ptrHeadCar, &ptrHeadParag);
                break;
            case 'i':
                intersecoes(&ptrHeadCar, &ptrHeadParag);
                break;
            case 'a':
                apagaTudo(&ptrHeadCar, &ptrHeadParag);
                break;
            case 'r':
                removeCar(&ptrHeadCar, &ptrHeadParag);
                break;
            case 'e':
                removePar(&ptrHeadCar, &ptrHeadParag);
                break;
	        case 'q':
                apagaTudo(&ptrHeadCar, &ptrHeadParag);
	            break;
            default:
	        /* Ignorar linhas em branco */
	        if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q');
    apagaTudo(&ptrHeadCar, &ptrHeadParag);
    return 0;
}