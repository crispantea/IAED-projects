/*
 * File:  project1.c
 * Author:  Cristiano Pantea ist1106324
 * Description: Sistema de gestão de carreiras de transporte público.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define OFFSET 1

/* Estados auxiliares */
#define DENTRO 1
#define FORA 0
#define TRUE 1
#define FALSE 0

/* Máximo de argumentos que um comando pode receber */
#define MAXARGUMENTOS 5
/* Número máximo de caracteres que um argumento pode ter */
#define MAXPORARGUMENTO 50
/* Valor default para os nomes das paragens nas carreiras sem ligacoes */
#define SEMPARAGEM "Sem Paragens Ainda"
/* Começo do comando se o nome nao tiver aspas */
#define COMECOCOMANDO 2

/* Máximo de caracteres que uma carreira pode conter no nome */
#define NOME_CARREIRA_MAX 20
/* Máximo de caracteres que uma paragem pode conter no nome */
#define NOME_PARAGEM_MAX 50

/* Máximo de carreiras que podem existir */
#define MAX_CARREIRAS 300
/* Máximo de paragens que podem existir */
#define MAX_PARAGENS 10000
/* Máximo de ligações que podem existir */
#define MAX_LIGACOES 30000

/* Máximo de caracteres que é possível receber de input pelo utilizador */
#define MAX_TAMANHO_INPUT 8192

/* Erros paragens */
#define PARAGEM_EXISTE "stop already exists.\n"
#define PARAGEM_N_EXISTE "no such stop.\n"

/* Erros carreiras */
#define CARR_N_EXISTE "no such line.\n"
#define PARAG_N_EXISTE "no such stop.\n"
#define LIG_N_ASSOC "link cannot be associated with bus line.\n"
#define CUSTO_VALOR_NEG "negative cost or duration.\n"
#define ORDEM_ERRADA "incorrect sort option.\n"

/* Estruturas necessárias */
typedef struct {
    char nome[NOME_PARAGEM_MAX + OFFSET];
    double latitude;
    double longitude;
} Paragem;

typedef struct {
    char nome[NOME_CARREIRA_MAX + OFFSET];
    Paragem paragemOrigem;
    Paragem paragemDestino;
} Carreira;

typedef struct {
    Carreira carreira;
    Paragem paragemOrigem;
    Paragem paragemDestino;
    double custo;
    double duracao;
} Ligacao;

/* Variáveis globais para manter o tracking do indice das listas */
int _posParagem = 0;
int _posCarreira = 0;
int _posLigacoes = 0;

/* Listas para guardar as diferentes estruturas */
Carreira listaCarreiras[MAX_CARREIRAS];
Paragem listaParagens[MAX_PARAGENS];
Ligacao listaLigacoes[MAX_LIGACOES];

/* Função recebeInput: recebe o input tirando os espaços e tabs a mais */
void recebeInput(char input[]) {

    int i = 0;
    char c;
    int ult_carater_espaco = FALSE;
    int estado = FORA;

    /* Lê o input linha a linha */
    while ((c = getchar()) != '\n') {
        /* Se for um espaço/tab verificar se deve ou nao tirá-lo */
        if ((c == ' ' || c == '\t') && estado == FORA) {
            if (ult_carater_espaco == FALSE) {
                input[i] = ' ';
                i++;
            }
            ult_carater_espaco = TRUE;
        }
        /* Caso não seja um espaço ou um tab guardar o caracter */ 
        else {
            input[i] = c;
            i++;
            ult_carater_espaco = FALSE;
        }
        /* Altera o estado para dentro se estiver a entrar numa palavra
        limitada por aspas e vice-versa */
        if (c == '"') {
            if (estado == FORA) 
                estado = DENTRO;
            else 
                estado = FORA;
        }
    }
    input[i] = '\0';
}

/* Funcao contaArgumentos: conta o número de argumentos recebido num input*/
int contaArgumentos(char input[],int tamanhoInput) {

    int i = 0, estado = FORA, nArgumentos = 0;

    /* Conta o número de argumentos em função do némero de espaços
    pois a função recebeInput já retira os espacos em excesso */
    while (i < tamanhoInput) {
        if (estado == FORA) {
            while (input[i] != '"' && i < tamanhoInput) {
                if (input[i] == ' ')
                    nArgumentos += 1;
                i++;
            }
            /* Nao deve contar os espaços dentro como um novo argumento */
            if (input[i] == '"')
                estado = DENTRO;
        }
        else if (estado == DENTRO) {
            i++;
            while (input[i] != '"' && i < tamanhoInput)
                i++;
            if (input[i] == '"')
                estado = FORA;
        }
        i++;
    }
    return nArgumentos;
}

/* Funcao argumentosVetor: guarda os argumentos recebidos no input num vetor,
separando cada argumento numa posição diferente do vetor */
void argumentosVetor(char guardaArgumentos[][MAXPORARGUMENTO + OFFSET],
                    int tamanhoInput, char input[]) {
    
    int i;
    /* indices */
    int i_palavra = 0, i_carater = 0;
    int estado = FORA;

    /* Comeca a ler apartir do primeiro caracter não branco */
    for (i = COMECOCOMANDO; i < tamanhoInput; i++) {
        /* Caso encontre um espaco estando FORA, significando o fim de um
        argumento, guarda-o e atualiza os indices */
        if (input[i] == ' ' && estado == FORA) {
            guardaArgumentos[i_palavra][i_carater] = '\0';
            i_palavra++;
            i_carater = 0;
        }
        /* Atualiza os estados */
        else if (input[i] == '"' && estado == FORA) {
            estado = DENTRO;
        }
        else if (input[i] == '"' && estado == DENTRO) {
            estado = FORA;
        }
        /* Guarda os caracteres correspondeste aos caracteres de cada palavra*/
        else {
            guardaArgumentos[i_palavra][i_carater] = input[i];
            i_carater++;
        }
    }
    guardaArgumentos[i_palavra][i_carater] = '\0';
}

/* Função escreveCarreiras: printa as carreiras com os respetivos dados
do comando c quando recebe 0 argumentos */
void escreveCarreiras() {

    double custo = 0, duracao = 0;
    /* Número de paragens é igual ao número de ligações da carreira mais 
    um por causa da ligação inicial que adiciona 2 paragens novas */
    int nParagens = 1; 
    int i, j;

    for (j = 0; j < _posCarreira; j++) {
        for (i = 0; i < _posLigacoes; i++) {
            /* Estando na mesma carreira adiciona os dados da ligação */
            if (strcmp(listaCarreiras[j].nome, 
            listaLigacoes[i].carreira.nome) == 0) {
                nParagens++;
                custo += listaLigacoes[i].custo;
                duracao += listaLigacoes[i].duracao;
            }
        }
        /* Caso nParagens seja igual a um, nao foi incrementado nada desde o
        inicio, ou seja, nao existe nenhuma ligacao associada */
        if (nParagens == 1) {
            nParagens = 0;
            printf("%s %d %.2f %.2f\n", 
            listaCarreiras[j].nome, nParagens, custo, duracao);
            nParagens = 1;
        }
        else {
            printf("%s %s %s %d %.2f %.2f\n",
            listaCarreiras[j].nome, 
            listaCarreiras[j].paragemOrigem.nome,
            listaCarreiras[j].paragemDestino.nome, 
            nParagens, custo, duracao);
            /* reseta as variaveis */
            custo = 0;
            duracao = 0;
            nParagens = 1;
        }
    }
}

/* Função existeParagem: verifica se a paragem existe ou nao */
int existeParagem(char nomeParagem[]) {

    int i = 0, existeOuN = FALSE;

    if (_posParagem == 0) {
        return existeOuN;
    }

    while (i < _posParagem && existeOuN != TRUE) {
        if ((strcmp(nomeParagem, listaParagens[i].nome)) == 0) {
                existeOuN = TRUE;
                i++;
        }
        else
            i++;
    }
    return existeOuN;
}

/* Função existeCarreira: verifica se a carreira existe ou nao */
int existeCarreira(char nomeCarreira[]) {

    int i = 0, existeOuN = FALSE;

    if (_posCarreira == 0) {
        return existeOuN;
    }
    
    while (i < _posCarreira && existeOuN != TRUE) {
        if ((strcmp(nomeCarreira, listaCarreiras[i].nome)) == 0) {
                existeOuN = TRUE;
                i++;
        }
        else
            i++;
    }
    return existeOuN;
}

/* Função escreveParagens: Escreve as paragens pela ordem normal
da carreira dada */
void escreveParagens(char paragemComparacao[], char fimCarreira[], 
                    char nomeCarreira[]) {

    int i = 0;

    while (strcmp(paragemComparacao, fimCarreira) != 0) {
        /* Somente printa as paragens correspondentes à carreira dada */
        if (strcmp(nomeCarreira, listaLigacoes[i].carreira.nome) == 0) {
            /* Se a paragem comparacao for igual à paragem origem da ligacao
            atualiza a paragemComparacao e printa a paragem */
            if (strcmp(listaLigacoes[i].paragemOrigem.nome,
            paragemComparacao) == 0) {
                printf("%s, ", paragemComparacao);
                strcpy(paragemComparacao, listaLigacoes[i].paragemDestino.nome);
                i = -1; /* É colocada -1 pois logo a seguir é lhe incrementada
                +1 */
            }
        }
        i++;
    }
}

/* Função escreveParagensInv: Escreve as paragens pela ordem inversa da carreira
dada, o mesmo raciocinio da função anterior, no entanto invertendo o sentido */
void escreveParagensInv(char paragemComparacao[], char InicioCarreira[], 
                        char nomeCarreira[]) {

    int i = 0;

    while (strcmp(paragemComparacao, InicioCarreira) != 0) {
        if (strcmp(nomeCarreira, listaLigacoes[i].carreira.nome) == 0) {
            if (strcmp(listaLigacoes[i].paragemDestino.nome,
            paragemComparacao) == 0) {
                printf("%s, ", paragemComparacao);
                strcpy(paragemComparacao, listaLigacoes[i].paragemOrigem.nome);
                i = -1; /* É colocada -1 pois logo a seguir é lhe incrementada
                +1 */
            }
        }
        i++;
    }
}

/* Função validaInverso: verifica se o argumento "inverso" é dado corretamente*/
int validaInverso(char inverso[]) {
    
    if (strcmp("inv", inverso) == 0 || strcmp("inve", inverso) == 0 ||
    strcmp("inver", inverso) == 0 || strcmp("invers", inverso) == 0 ||
    strcmp("inverso", inverso) == 0)
        return 1;
    else
        return 0;
}

/* Função contaCarreiras: conta o número de carreiras de uma certa paragem */
int contaCarreiras(char nomeParagem[NOME_PARAGEM_MAX + OFFSET]) {

    int posGuardaCar = 0, e, i = 0;
    int existeCar = FALSE;
    char guardaCarreiras[MAX_CARREIRAS][NOME_CARREIRA_MAX + OFFSET];

    while (i <= _posLigacoes) {
        /* Encontrou uma ligação onde a paragem existe */
        if (strcmp(listaLigacoes[i].paragemOrigem.nome, nomeParagem) == 0 ||
        strcmp(listaLigacoes[i].paragemDestino.nome, nomeParagem) == 0) {
            /* Verifica se a carreira já está no vetor de carreiras */
            for (e = 0; e < posGuardaCar; e++) {
                if (strcmp(listaLigacoes[i].carreira.nome,
                guardaCarreiras[e]) == 0) {
                    existeCar = TRUE;
                    break;
                }
            }
            /* Se não estiver, adiciona-a */
            if (existeCar == FALSE) {
                strcpy(guardaCarreiras[posGuardaCar],
                listaLigacoes[i].carreira.nome);
                posGuardaCar++;
                i = -1;
            }
        }
        existeCar = FALSE;
        i++;
    }
    return posGuardaCar;
}

/* Função comandoRecebe_p: recebe o comando p e executa os comandos em função
do número de argumentos dados */
int comandoRecebe_p(char input[], int tamanhoInput) {

    Paragem paragem;
    char guardaArgumentos[MAXARGUMENTOS][MAXPORARGUMENTO + OFFSET];
    int nArgumentos = 0;
    int i;
    int nCarreiras = 0;

    nArgumentos = contaArgumentos(input, tamanhoInput);
    argumentosVetor(guardaArgumentos, tamanhoInput, input);

    /*
    Indíce de cada argumento no vetor guardaArgumentos

        [0] - Paragem 
        [1] - Latitude
        [2] - Longitude
    */

    /* Verifica se a paragem existe ou não e printa o erro se necessário */
    if (nArgumentos == 1) {
        if (existeParagem(guardaArgumentos[0]) == FALSE) {
            printf("%s: %s", guardaArgumentos[0], PARAG_N_EXISTE);
            return 1;
        }
        /* Caso exista, printa a latitude e a latitude */
        else {
            for (i = 0; i <= _posParagem; i++) {
                if ((strcmp(guardaArgumentos[0], listaParagens[i].nome)) == 0) {
                    printf("%16.12f %16.12f\n", listaParagens[i].latitude,
                    listaParagens[i].longitude);
                    break;
                }    
            }
        }
    }

    /* Verifica se a paragem já existe, caso exista printa o erro */
    if (nArgumentos == 3) {
        if (existeParagem(guardaArgumentos[0]) == TRUE) {
            printf("%s: %s", guardaArgumentos[0], PARAGEM_EXISTE);
            return 1;
        }
        /* Caso não exista, cria a paragem e adiciona à lista de paragens */
        else {
            strcpy(paragem.nome,guardaArgumentos[0]);
            paragem.latitude = atof(guardaArgumentos[1]);
            paragem.longitude = atof(guardaArgumentos[2]);
            listaParagens[_posParagem] = paragem;
            _posParagem += 1;
        }
    }
    
    /* Lista todas as paragens com as respetivas informações */
    if (nArgumentos == 0) {
        for (i = 0; i < _posParagem; i++) {
            nCarreiras = contaCarreiras(listaParagens[i].nome);
            printf("%s: %16.12f %16.12f %d\n", listaParagens[i].nome,
            listaParagens[i].latitude, listaParagens[i].longitude, nCarreiras);
            nCarreiras = 0;
        }
    }   
    return 1;
}

/* Função comandoRecebe_l: recebe o comando l e executa os comandos em função
do número de argumentos dados */
int comandoRecebe_l(char input[], int tamanhoInput) {

    Ligacao ligacao1;
    double custo, duracao;
    char guardaArgumentosL[MAXARGUMENTOS][MAXPORARGUMENTO + OFFSET];
    int i, mudanca = 0;
    
    /*
    Indíce de cada argumento no vetor guardaArgumentos

        [0] - NomeCarreira 
        [1] - ParagemOrigem
        [2] - ParagemDestino
        [3] - Custo
        [4] - Duracao
    */
    
    argumentosVetor(guardaArgumentosL, tamanhoInput, input);

    /* Verifica os erros */
    if (existeCarreira(guardaArgumentosL[0]) == 0) {
        printf("%s: %s", guardaArgumentosL[0], CARR_N_EXISTE);
        return 1;
    }
    if (existeParagem(guardaArgumentosL[1]) == 0) {
        printf("%s: %s", guardaArgumentosL[1], PARAG_N_EXISTE);
        return 1;
    }
    if (existeParagem(guardaArgumentosL[2]) == 0) {
        printf("%s: %s", guardaArgumentosL[2], PARAG_N_EXISTE);
        return 1;
    }
    custo = atof(guardaArgumentosL[3]);
    duracao = atof(guardaArgumentosL[4]);
    if (custo < 0 || duracao < 0) {
        printf("%s", CUSTO_VALOR_NEG);
        return 1;
    }

    /* Faz as mudanças nas paragens origem e destino da carreira caso seja
    necessário */
    for (i = 0; i < _posCarreira; i++) {
        if (strcmp(listaCarreiras[i].nome, guardaArgumentosL[0]) == 0) {
            /* Se esta não tiver paragem origem nem paragem destino adiciona */
            if (strcmp(listaCarreiras[i].paragemOrigem.nome, 
            SEMPARAGEM) == 0) {
                strcpy(listaCarreiras[i].paragemOrigem.nome, 
                guardaArgumentosL[1]);
                strcpy(listaCarreiras[i].paragemDestino.nome, 
                guardaArgumentosL[2]);
                mudanca = TRUE;
            }
            /* Caso tenha faz as mudanças em função do que foi recebido */
            else if (strcmp(listaCarreiras[i].paragemDestino.nome, 
            guardaArgumentosL[1]) == 0) {
                strcpy(listaCarreiras[i].paragemDestino.nome, 
                guardaArgumentosL[2]);
                mudanca = TRUE;
            }
            else if (strcmp(listaCarreiras[i].paragemOrigem.nome, 
            guardaArgumentosL[2]) == 0) {
                strcpy(listaCarreiras[i].paragemOrigem.nome, 
                guardaArgumentosL[1]);
                mudanca =  TRUE;
            }
        }
    }

    /* Caso não tenha ocorrido mudança, ou seja, não verificava as condições
    para tal, printa o erro */
    if (mudanca == FALSE) {
        printf("%s", LIG_N_ASSOC);
        return 1;
    }

    /* Adiciona a ligação à lista de ligações */
    mudanca = FALSE;
    strcpy(ligacao1.carreira.nome, guardaArgumentosL[0]);
    strcpy(ligacao1.paragemOrigem.nome, guardaArgumentosL[1]);
    strcpy(ligacao1.paragemDestino.nome, guardaArgumentosL[2]);
    ligacao1.custo = custo;
    ligacao1.duracao = duracao;

    listaLigacoes[_posLigacoes] = ligacao1;
    _posLigacoes+=1;

    return 1;
}

/* Função comandoRecebe_c: recebe o comando c e executa os comandos em função
do número de argumentos dados */
int comandoRecebe_c(char input[], int tamanhoInput) {

    Carreira carreira;
    int nArgumentos;
    char guardaArgumentosC[MAXARGUMENTOS][MAXPORARGUMENTO + OFFSET];
    int i, i_car = 0; /* i_car = indice carreira */
    char paragemComparacao[NOME_PARAGEM_MAX + OFFSET];
    char fimCarreira[NOME_CARREIRA_MAX + OFFSET];

    /*
    Indíce de cada argumento no vetor guardaArgumentos

        [0] - NomeCarreira
        [1] - Inverso
    */

    nArgumentos = contaArgumentos(input, tamanhoInput);
    argumentosVetor(guardaArgumentosC, tamanhoInput, input);


    if (nArgumentos == 1 || nArgumentos == 2) {

        /* Obtem o indice da carreira recebida na lista de carreiras */
        for (i_car = 0; i_car < _posCarreira; i_car++) {
            if (strcmp(guardaArgumentosC[0], listaCarreiras[i_car].nome) == 0)
                break;
        }
    }
    
    /* Caso onde tem de printar as paragens da carreira recebida pela ordem
    inversa */
    if (nArgumentos == 2) {

        char inicioCarreira[NOME_PARAGEM_MAX + OFFSET];

        if (validaInverso(guardaArgumentosC[1]) == FALSE) {
            printf("%s", ORDEM_ERRADA);
            return 1;
        }
        
        /* Se a paragem for circular */
        if (strcmp(listaCarreiras[i_car].paragemOrigem.nome,
        listaCarreiras[i_car].paragemDestino.nome) == 0) {

            /* Faz com que a carreira "deixe" de ser circular, aumentando
            o caso inicial em um, ou seja, acabando uma paragem à frente */
            strcpy(paragemComparacao,listaCarreiras[i_car].paragemDestino.nome);
            for (i = 0; i < _posLigacoes; i++) {
                if (strcmp(listaLigacoes[i].paragemOrigem.nome, 
                listaCarreiras[i_car].paragemOrigem.nome) == 0) {
                    if (strcmp(listaLigacoes[i].carreira.nome,
                    listaCarreiras[i_car].nome) == 0) {
                        strcpy(inicioCarreira, 
                        listaLigacoes[i].paragemDestino.nome);
                    }
                }
            }
            escreveParagensInv(paragemComparacao, inicioCarreira,
            guardaArgumentosC[0]);
            printf("%s, %s\n", paragemComparacao,
            listaCarreiras[i_car].paragemOrigem.nome);
        }
        /* Se a carreira não for circular */
        else {
            strcpy(inicioCarreira, listaCarreiras[i_car].paragemOrigem.nome);
            strcpy(paragemComparacao, 
            listaCarreiras[i_car].paragemDestino.nome);

            escreveParagensInv(paragemComparacao, inicioCarreira, 
            guardaArgumentosC[0]);
            printf("%s\n", paragemComparacao);
        }
    }

    /* Se a carreira não existir cria a carreira, se esta existir printa
    as paragens pela ordem normal da carreira */
    if (nArgumentos == 1) {

        /* Cria a carreira se ela não existir */
        if (existeCarreira(guardaArgumentosC[0]) == FALSE) {
            strcpy(carreira.nome, guardaArgumentosC[0]);
            strcpy(carreira.paragemOrigem.nome, SEMPARAGEM);
            listaCarreiras[_posCarreira] = carreira;
            _posCarreira++;
            return 1;
        }

        /* Se a paragem existir mas não tiver ligacões não dá print a nada */
        else if (existeCarreira(guardaArgumentosC[0]) == 1 &&
        (strcmp(listaCarreiras[i_car].paragemOrigem.nome, SEMPARAGEM) == 0)) {
            return 1;
        }

        /* Se for circular */
        else if (strcmp(listaCarreiras[i_car].paragemOrigem.nome,
        listaCarreiras[i_car].paragemDestino.nome) == 0) {
            strcpy(paragemComparacao, listaCarreiras[i_car].paragemOrigem.nome);

            /* Reduz o problema para fazer até à origem da ligação final e 
            depois printa a paragem final no fim */
            for (i = 0; i < _posLigacoes; i++) {
                if (strcmp(listaLigacoes[i].paragemDestino.nome, 
                listaCarreiras[i_car].paragemDestino.nome) == 0) {
                    if (strcmp(listaLigacoes[i].carreira.nome, 
                    listaCarreiras[i_car].nome) == 0) {
                        strcpy(fimCarreira, 
                        listaLigacoes[i].paragemOrigem.nome);
                    }
                }
            }
            escreveParagens(paragemComparacao, 
            fimCarreira, guardaArgumentosC[0]);
            printf("%s, %s\n", paragemComparacao, 
            listaCarreiras[i_car].paragemDestino.nome);
        }

        /* Se não for circular */
        else {
            strcpy(fimCarreira, listaCarreiras[i_car].paragemDestino.nome);
            strcpy(paragemComparacao, listaCarreiras[i_car].paragemOrigem.nome);

            escreveParagens(paragemComparacao, fimCarreira, 
            guardaArgumentosC[0]);
            printf("%s\n", paragemComparacao);
       }
    }
    /* Printa as informações de todas as carreiras */
    if (nArgumentos == 0) 
        escreveCarreiras();
    
    return 1;
}

/* Função ordenaCarreiras: recebe o vetor com as carreiras e ordena-as
alfabeticamente*/
void ordenaCarreiras(char guardaCarreias[][NOME_CARREIRA_MAX + OFFSET],
                     int nCarreiras) {

    int i, j;
    char fazTroca[NOME_CARREIRA_MAX + OFFSET];

    for (i = 0; i < nCarreiras - 1; i++) {
        for (j = 0; j < nCarreiras - i - 1; j++) {
            /* Verifica se estão na ordem correta, se não estiverem trocam */
            if (strcmp(guardaCarreias[j], guardaCarreias[j+1]) > 0) {
                strcpy(fazTroca, guardaCarreias[j]);
                strcpy(guardaCarreias[j], guardaCarreias[j+1]);
                strcpy(guardaCarreias[j+1], fazTroca);
            }
        }
    }
}

/* Função comandoRecebe_i: lista as paragens com interseções, mostrando 
quais as carreiras que se intersetam nessa paragem (estas vêm ordenadas
alfabeticamente) */
int comandoRecebe_i() {

    char guardaCarreiras[MAX_CARREIRAS][NOME_CARREIRA_MAX + OFFSET];
    int i, j, e;
    int posGuardaCarreiras = 0;

    for (i = 0; i < _posParagem; i++) {
        posGuardaCarreiras = 0;
        for (j = 0; j < _posLigacoes; j++) {
            /* Verifica se a paragem é a origem ou destino de alguma ligação */
            if ((strcmp(listaLigacoes[j].paragemOrigem.nome,
            listaParagens[i].nome) == 0) ||
            strcmp(listaLigacoes[j].paragemDestino.nome,
            listaParagens[i].nome) == 0) {
            /* Se for verificar se a carreira já está na lista de Carreiras */
                for (e = 0; e < posGuardaCarreiras; e++) {
                    if (strcmp(guardaCarreiras[e], 
                    listaLigacoes[j].carreira.nome) == 0)
                        break;
                }
                /* Se chegar ao fim do ciclo, ou seja, a carreira não está na 
                lista, guardar na lista */
                if (e == posGuardaCarreiras) {
                    strcpy(guardaCarreiras[posGuardaCarreiras],
                    listaLigacoes[j].carreira.nome);
                    posGuardaCarreiras++;
                }
            }            
        }
        ordenaCarreiras(guardaCarreiras, posGuardaCarreiras);
        /* Se houver interseções */
        if (posGuardaCarreiras > 1) {
            printf("%s %d:", listaParagens[i].nome, posGuardaCarreiras);
            for (e = 0; e < posGuardaCarreiras - 1; e++) {
                printf(" %s", guardaCarreiras[e]);
            }
            printf(" %s\n", guardaCarreiras[e]);
        }
    }
    return 1;
}

/* Recebe o input e redireciona para as funções correspondentes ao comando
recebido */
int main() {

    char input[BUFSIZ + OFFSET];
    char comando;
    int tamanhoInput;

    while (1) {
        recebeInput(input); /* recebe linha a linha */
        tamanhoInput = strlen(input);
        comando = input[0]; /* comando correspondente à primeira letra */

        switch (comando) {

            case 'q':
                return 0;
                break;
            case 'c':
                comandoRecebe_c(input, tamanhoInput);
                break;
            case 'p':
                comandoRecebe_p(input, tamanhoInput);
                break;
            case 'l':
                comandoRecebe_l(input, tamanhoInput);
                break;
            case 'i':
                comandoRecebe_i(input);
                break;
        }
    }
    return 0;
}