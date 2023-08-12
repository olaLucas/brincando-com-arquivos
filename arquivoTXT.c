#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define READMODE "r"
#define WRITEMODE "w"
#define APPENDMODE "a"
#define APPEND_READ "a+"
#define NOMEARQ "arquivoTXT.txt"
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
FILE * initFILE();
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
    printf(NOMETEMPLATE); fgets(temp.nome, NOMETAM, stdin);
    printf(IDADETEMPLATE); scanf("%d", &temp.idade);

    cleanBuffer();

    printf(GENEROTEMPLATE); fgets(temp.genero, GENEROTAM, stdin);
    return temp;
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

FILE * initFILE() {
    
    if ((fopen(ENDERECOARQ, READMODE)) == NULL) {

        puts("Não foi possivel acessar o arquivo txt (inexistente, corrompido ou nome incorreto). \n");

        if (menuInitFILE() == 2) {

            puts("Sem a criação de um arquivo para armazenar os dados não é possivel prosseguir.");
            puts("Finalizando o programa. \n");
            exit(0);
        }
    }

    return fopen(ENDERECOARQ, APPEND_READ);
}

void exitFILE(FILE * arquivo) {

    puts("Fechando arquivo... ");
    fclose(arquivo);
    
    puts("Encerrando programa...");
    exit(0);
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
        while (TRUE) {

            if ((c = fgetc(arquivo)) != EOF) {
                
                printf("%c", c);
            
            } else {

                break;
            }
        }
    }

    pause();
}

void excluirDados(FILE * arquivo) {
    puts("puts");
}

void menu(FILE * arquivo) {

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
                exitFILE(arquivo);
                break;
            
            case 1:
                inserirDados(arquivo);
                break;

            case 2:
                exibirDados(arquivo);
                break;

            case 3:
                excluirDados(arquivo);
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
    
    FILE * arquivo = initFILE();
    menu(arquivo);

    return 0;
}