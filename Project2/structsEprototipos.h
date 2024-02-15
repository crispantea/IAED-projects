#ifndef _STRUCTSEPROTOTIPOS_H_
#define _STRUCTSEPROTOTIPOS_H_

/* Estados auxiliares */
#define VERDADE 1
#define FALSO 0
#define ENCONTRADO 1
#define NAO_ENCONTRADO 0
/* Tamanho máximo de uma instrução */
#define buffer 65535

struct Paragem;
struct Ligacao;
struct Carreira;
struct CarreirasNomes;

/* Estruturas necessárias */
typedef struct CarreirasNomes {

    char* nomeCarreira;
    struct CarreirasNomes *next;
} CarreirasNomes;

typedef struct Paragem {

    char* nome;
    double latitude, longitude;
    int numCarreiras;
    struct Paragem* next;
    struct CarreirasNomes *headNomes;
} Paragem;

typedef struct Ligacao {

    struct Ligacao *anterior, *prox;
    char *paragOrig, *paragDest;
    double custo, duracao;
} Ligacao;

typedef struct Carreira {

    char *nome;
    double custoTotal;
    double duracaoTotal;
    struct Ligacao *ligHead, *ligTail;
    int numLigacoes;
    struct Carreira *next;
} Carreira;


void apagaTudo(Carreira **ptrHeadCar, Paragem **ptrHeadParag);

void erroMemoria(void* memoria, Carreira **ptrHeadCar, Paragem **ptrHeadPar);

int leEspacos();

char* leNome(Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void leAteFinalLinha();

void criaParagem(char* nomeParagem, double latitude, double longitude,
Paragem** ptrHeadPar, Paragem** ptrTailPar, Carreira **ptrHeadCar);

void criaCarreira(char* nomeParagem, Carreira** ptrHeadCar, 
Carreira** ptrTailCar, Paragem **ptrHeadPar);

Paragem* encontraParagem(Paragem **ptrHeadParagem, char* nomeParagem);

Paragem* encontraParagemPrev(Paragem **ptrHeadParagem, char* nomeParagem);

Carreira* encontraCarreira(Carreira **ptrHeadCarreira, char* nomeCarreira);

Carreira* encontraCarreiraPrev(Carreira **ptrHeadCarreira, char* nomeCarreira);

int verificaInversoOk(char *s);

void adicionaPrimeiraLigacao(Carreira **ptrCarreira, char *nomeOrig,
char* nomeDest, double custo, double duracao, Carreira **ptrHeadCar,
Paragem **ptrHeadPar);

void adicionaLigacaoAnterior(Carreira **ptrCar, char *nomeOrig, char *nomeDest,
double custo, double duracao, Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void adicionaLigacaoFim(Carreira **ptrCar, char *nomeOrig, char *nomeDest,
double custo, double duracao, Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void adicionaCarreira(Paragem **ptrParagem, char *nomeCarreira, 
Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void verificaAdicionaCar(Paragem **ptrParagem, Carreira **ptrCarreira,
Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void adicionaLigacao(Carreira **ptrCarreira, Paragem **ptrParagOrig,
Paragem **ptrParagDest, double custo, double duracao, Carreira **ptrHeadCar,
Paragem **ptrHeadPar);

void mostraLigacoesCarreira(Carreira **ptrCarreira);

void mostraLigacoesCarreiraInv(Carreira **ptrCarreira);

void listaParagens(Paragem **ptrHeadParagem);

void listaCarreiras(Carreira **ptrHead);

void paragens(Carreira **ptrHeadCar, Paragem **ptrHeadPar, Paragem **ptrTailPar);

void carreiras(Paragem **ptrHeadPar, Carreira** ptrHeadCar, Carreira** ptrTailCar);

void ligacoes(Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void freeIntersecoes(char **arrayOrganiza, int nCarreiras);

void ordenaCarreiras(Paragem **ptrParagem, Carreira **ptrHeadCar, 
Paragem **ptrHeadPar);

void organizaEverificaI(Paragem **ptrParagem, Carreira **ptrHeadCar, 
Paragem **ptrHeadPar);

void resetaCarreira(Carreira **ptrCarreira);

void intersecoes(Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void freeDasParag(Paragem **ptrHeadPar, char *nomeCarreira);

void freeLigacoesA(Carreira **ptrCarreira);

int encontraParagemLig(Carreira **carreira, char *nomeParagem);

void criaLigacao(Ligacao **exclui1, Ligacao **exclui2, Carreira **ptrCarreira, 
double custo, double duracao, Carreira **ptrHeadCar, Paragem **ptrHeadPar);

Ligacao* encontraExcluiFim(Ligacao **excluiInicio, char *nomeParag, 
Carreira **ptrCarreira, double *custo, double *duracao);

void tiraLigacoesMeio(Carreira **carreira, char *nomeParagem, 
Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void eliminaNomesCarreiras(Paragem **paragemApagar);

void eliminaParagem(Paragem **headParagens, Paragem **paragemApagar,
char *nomeParagem);

void removeCarListaParag(char *nomeParagem, char *nomeCarreira, 
Paragem **ptrHeadPar);

void funcaoTiraParagem(Carreira **carreira, char *nomeParagem, 
Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void removePar(Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void removeCar(Carreira **ptrHeadCar, Paragem **ptrHeadPar);

void freeNomesCarreiras(CarreirasNomes **headNomes);

#endif