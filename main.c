#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define READMODE "r"
#define WRITEMODE "w"
#define APPENDMODE "a"
#define NOMEARQ "arquivo.txt"
#define NOMETEMPLATE "NOME: "
#define IDADETEMPLATE "IDADE: "
#define GENEROTEMPLATE "GÊNERO: "

#define NOMETAM 150
#define GENEROTAM 20

typedef struct individuo {

    char nome[NOMETAM];
    int idade;
    char genero[GENEROTAM];
} ind;

ind initIND();
void initFILE();
void exibirDados();
void excluirDados();
void inserirDados();
void cleanBuffer();
void inserirDados();
void exitFILE();
int menuInitFILE();

void cleanBuffer() {

    int c = 0;
    while ((c = getchar()) != '\n');
}

void pause() {

    cleanBuffer();
    getchar();
}

ind initIND() { 

    cleanBuffer();
    
    ind temp;
    printf("NOME: "); fgets(temp.nome, NOMETAM, stdin);
    printf("\nIDADE: "); scanf("%d", &temp.idade);

    cleanBuffer();

    printf("\nGênero: "); fgets(temp.genero, GENEROTAM, stdin);
    return temp;
}

void exitFILE(FILE ** arquivoREAD, FILE ** arquivoWRITE, FILE ** arquivoAPPEND) {

    fclose(*arquivoREAD);
    fclose(*arquivoWRITE);
    fclose(*arquivoAPPEND);
}

int menuInitFILE() {
    int seletor = 0;

    while (TRUE) {
        puts("Deseja criar um novo arquivo? "); puts("1. Sim"); puts("2. Não");
        printf("\n >>> ");
        scanf("%d", &seletor);

        if (seletor == 1 || seletor == 2) {
            break;
        } else {
            puts("Opção inválida, tente novamente.");
            pause();
        }
    }

    return seletor;
}

void initFILE(FILE ** arquivoREAD, FILE ** arquivoWRITE, FILE ** arquivoAPPEND) {
    
    FILE * temp;
    if ((temp = fopen(NOMEARQ, READMODE)) == NULL) {

        puts("Não foi possivel acessar o arquivo txt (inexistente, corrompido ou nome incorreto). \n");

        if (menuInitFILE() == 2) {

            puts("Sem a criação de um arquivo para armazenar os dados não é possivel prosseguir.");
            puts("Finalizando o programa. \n");
            exit(0);
        }
    }

    *arquivoWRITE = fopen(NOMEARQ, WRITEMODE);
    *arquivoAPPEND = fopen(NOMEARQ, APPENDMODE);
    *arquivoREAD = fopen(NOMEARQ, READMODE);
}

void inserirDados(FILE * arquivo) {
    
    ind temp = initIND();
    if (temp.idade == -1) {
        return;

    } else {

        fputs(NOMETEMPLATE, arquivo); fputs(temp.nome, arquivo); 
        fputs(IDADETEMPLATE, arquivo); fprintf(arquivo, "%d\n", temp.idade);
        fputs(GENEROTEMPLATE, arquivo); fputs(temp.genero, arquivo);
    }
}

void exibirDados(FILE * arquivo) {

    if (arquivo == NULL) {

        puts("Arquivo vazio.");

    } else {

        char c = 0;
        while (c != EOF) {

            c = fgetc(arquivo);
            printf("%c", c);
        }
    }

    pause();
}

void excluirDados(FILE * arquivo) {
    puts("puts");
}

void menu() {

    FILE * arquivoREAD;
    FILE * arquivoWRITE;
    FILE * arquivoAPPEND;

    initFILE(&arquivoREAD, &arquivoWRITE, &arquivoAPPEND);

    int seletor = -1;
    while (seletor != 0) {
        
        system("clear");
        puts("--------------| ELEMETOS |--------------\n");
        
        puts("Selecione uma opção: \n");
        puts("0. Sair");
        puts("1. Inserir dados.");
        puts("2. Imprimir dados.");
        puts("3. Excluir dados.");
        
        printf("\n >>> ");
        scanf("%d", &seletor);

        switch (seletor) {
        
            case 0:
                exitFILE(&arquivoREAD, &arquivoWRITE, &arquivoAPPEND);
                exit(0);
                break;
            
            case 1:
                inserirDados(arquivoAPPEND);
                break;

            case 2:
                exibirDados(arquivoREAD);
                break;

            case 3:
                excluirDados(arquivoWRITE);
                break;
            
            default:
                puts("Opção inválida! Tente novamente. \n");
                cleanBuffer();
                getchar();
                break;
        }
    }
}

int main(void) {
    
    menu();

    return 0;
}