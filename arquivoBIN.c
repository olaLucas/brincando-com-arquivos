#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1
#define ERROR -1

#define NOMETAM 150
#define NOMETEMPLATE "NOME: "
#define IDADETEMPLATE "IDADE: "
#define ALTURATEMPLATE "ALTURA: "

#define READMODE "rb"
#define WRITEMODE "wb"
#define APPENDMODE "ab"
#define APPEND_READ "ab+"

#define ALTERARFILENAME "tempAlterar.bin"
#define EXCLUIRFILENAME "tempExcluir.bin"
#define FILENAME "arquivo.bin"

typedef struct individuo 
{
    char nome[NOMETAM];
    int idade;
    float altura;
} ind;

// main functions
void menu();
void exitMENU();
void cleanBuffer();
void pause();

// ind functions
ind initIND();
void exibirIndividuo(ind temp);
void excluirIND();
void alterarIND();
int menuAlterar();

// file functions
void initFILE();
void inserirDados(ind temp);
void exibirDados();
int excluirDados(const char nomeExcluir[]);
int overwrite(const char nomeArquivoOriginal[], const char nomeArquivoTemp[]);


// file
void initFILE()
{
    FILE * temp;
    if ((temp = fopen(FILENAME, READMODE)) == NULL) 
    {
        puts("Criando arquivo..."); 
        
        if ((temp = fopen(FILENAME, APPEND_READ)) == NULL)
        {
            puts("Não foi possível criar o arquivo.");
        }
        else
        {
            puts("Arquivo criado.");
            fclose(temp);
        }

        return;
    }
    else
    {
        puts("Arquivo já existente.");
        return;
    }
}

void inserirDados(ind temp) 
{
    FILE * arquivo;
    if((arquivo = fopen(FILENAME, APPENDMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo.");
    }
    else
    {
        size_t retorno = fwrite(&temp, sizeof(ind), 1, arquivo);
        fclose(arquivo);
    }
}

void exibirDados() 
{
    ind temp;
    size_t retorno = 0;
    FILE * arquivo;

    if ((arquivo = fopen(FILENAME, READMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo.");   
    }
    else
    {
        while ((retorno = fread(&temp, sizeof(temp), 1, arquivo)) != FALSE) 
        {
            exibirIndividuo(temp);
            printf("\n");
        }

        pause();
        fclose(arquivo);
    }
}

int buscarDados(const char nome[])
{
    ind temp;
    FILE * arquivo;
    size_t retorno;

    if((arquivo = fopen(FILENAME, READMODE)) == NULL)
    {
        puts("Ocorreu um erro ao abrir o arquivo.");
        return ERROR;
    }

    while ((retorno = fread(&temp, sizeof(temp), 1, arquivo)) != FALSE)
    {
        if(strcmp(temp.nome, nome) == 0)
        {
            puts("Encontrado.");
            exibirIndividuo(temp);
            pause();
            return TRUE;
        }
        else
        {
            puts("Procurando...");
        }
    }
    
    return FALSE;
}

int excluirDados(const char nomeExcluir[])
{
    FILE * arquivoNovo;
    FILE * arquivoAntigo;
    ind temp;
    int found = FALSE;
    size_t retorno = 0;
    int retornoString;

    if((arquivoNovo = fopen(EXCLUIRFILENAME, WRITEMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo novo.");
        return ERROR;
    }

    if((arquivoAntigo = fopen(FILENAME, READMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo antigo.");
        return ERROR;
    }

    while ((retorno = fread(&temp, sizeof(temp), 1, arquivoAntigo)) != 0)
    {
        if((retornoString = strcmp(temp.nome, nomeExcluir)) == 0)  // "apagando" o individuo alvo
        {
            found = TRUE;
            puts("Não registrando individuo alvo...");
        }
        else if(retornoString != 0) // "salvando" os demais individuos
        {
            puts("Registrando...");
            fwrite(&temp, sizeof(struct individuo), 1, arquivoNovo);
        }
    }

    //overwritting
    return overwrite(FILENAME, EXCLUIRFILENAME);
}

int alterarDados(const char nomeAlterar[])
{
    FILE * arquivoTemp;
    FILE * arquivoOriginal;
    size_t retorno = 0;
    int found = FALSE;
    int retornoString;
    ind temp;

    if((arquivoTemp = fopen(ALTERARFILENAME, WRITEMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo novo.");
        return ERROR;
    }

    if((arquivoOriginal = fopen(FILENAME, READMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo antigo.");
        return ERROR;
    }

    while ((retorno = fread(&temp, sizeof(temp), 1, arquivoOriginal)) != 0)
    {
        if((retornoString = strcmp(temp.nome, nomeAlterar)) == 0)  // selecionando o individuo alvo para alteração
        {
            puts("Alterando...");
            alterarIND(&temp);
            fwrite(&temp, sizeof(temp), 1, arquivoTemp);
        }
        else if(retornoString != 0) // copiando os demais individuos
        {
            puts("Copiando...");
            fwrite(&temp, sizeof(temp), 1, arquivoTemp);
        }
    }

    fclose(arquivoTemp);
    fclose(arquivoOriginal);

    return overwrite(FILENAME, ALTERARFILENAME);
}

int overwrite(const char nomeArquivoOriginal[], const char nomeArquivoTemp[])
{
    int retorno;
    ind temp;
    FILE * arquivoOriginal;
    FILE * arquivoTemp;

    if ((arquivoOriginal = fopen(FILENAME, WRITEMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo antigo (overwrite).");
        return ERROR;
    }

    if ((arquivoTemp = fopen(nomeArquivoTemp, READMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo novo (overwrite).");
        return ERROR;
    }

    while ((retorno = fread(&temp, sizeof(temp), 1, arquivoTemp)))
    {
        if((retorno = fwrite(&temp, sizeof(temp), 1, arquivoOriginal)) == FALSE) // escrevendo os dados do temp no arquivo antigo
        {
            puts("Ocorreu um erro ao registrar o individuo: \n\n");
            exibirIndividuo(temp);
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    remove(nomeArquivoTemp);
}


// ind
ind initIND() 
{ 
    cleanBuffer();
    
    ind temp;
    printf(NOMETEMPLATE); fgets(temp.nome, NOMETAM, stdin);
    printf(IDADETEMPLATE); scanf("%d", &temp.idade);

    cleanBuffer();

    printf(ALTURATEMPLATE); scanf("%f", &temp.altura);

    temp.nome[strcspn(temp.nome, "\n")] = '\0';

    return temp;
}

void exibirIndividuo(ind temp)
{
    printf("%s%s", NOMETEMPLATE, temp.nome);
    printf("\n%s%d", IDADETEMPLATE, temp.idade);
    printf("\n%s%2f", ALTURATEMPLATE, temp.altura);
}

void excluirIND()
{
    cleanBuffer();

    int retorno;
    char nome[NOMETAM];
    printf("Insira o nome: "); fgets(nome, NOMETAM, stdin);

    nome[strcspn(nome, "\n")] = '\0';

    if((retorno = excluirDados(nome)) == ERROR)
    {
        puts("Ocorreu um erro ao realizar a operação.");
        pause();
    }
    else
    {
        puts("Individuo apagado com sucesso.");
        pause();
    }
}

void alterarIND(ind * pessoa)
{
    cleanBuffer();
    
    int retorno;
    char nome[NOMETAM];
    if(pessoa == NULL)
    {
        printf("Insira o nome: "); fgets(nome, NOMETAM, stdin);

        nome[strcspn(nome, "\n")] = '\0';

        if ((retorno = alterarDados(nome)) == FALSE)
        {
            puts("Não foi possível encontrar o individuo.");
            pause();
        }
        else if(retorno == ERROR)
        {
            puts("Ocorreu um erro ao realizar a operação.");
            pause();
        }
        else
        {
            puts("Individuo apagado com sucesso.");
            pause();
        }
    }
    else if(pessoa != NULL)
    {
        exibirIndividuo(*pessoa);
        retorno = menuAlterar(); 

        switch (retorno)
        {
        case 1:
            cleanBuffer();

            printf("Insira o novo nome: ");

            fgets(nome, NOMETAM, stdin);
            nome[strspn(nome, "\n")] = '\0';
            strcpy(pessoa->nome, nome);

            return;
            break;

        case 2:
            scanf("%d", &pessoa->idade);
            return;
            break;

        case 3:
            scanf("%f", &pessoa->altura);
            return;
            break;
        
        default:
            return;
            break;
        }
    }
}

void buscarIND()
{
    cleanBuffer();
    int retorno;
    char nome[NOMETAM];

    printf("Insira o nome: "); fgets(nome, NOMETAM, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    if(buscarDados(nome))
    {
        return;
    }
    else
    {
        puts("Individuo não encontrado.");
        return;
    }
}

// menu
void cleanBuffer() 
{
    int c;
    while ((c = getchar()) != '\n');
}

void pause() 
{
    cleanBuffer();
    getchar();
}

void exitMENU()
{
    puts("Encerrando programa.");
    exit(0);
}

int menuAlterar()
{
    int seletor = -1;
    while (TRUE)
    {
        puts("\n\nO que deseja alterar?\n");
        puts("0. Sair");
        puts("1. Nome");
        puts("2. Idade");
        puts("3. Altura");

        printf("\n >>> ");
        scanf("%d", &seletor);

        if (seletor >= 0 && seletor <= 3)
        {
            return seletor;
        }
        else
        {
            puts("Opção inválida! Tente novamente. \n");
            cleanBuffer();
            getchar();
        }
    }
}

void menu()
{
    int seletor = -1;
    while (seletor != 0) 
    {
        system("clear");
        
        puts("Selecione uma opção: \n");
        puts("0. Sair");
        puts("1. Inserir dados.");
        puts("2. Imprimir dados.");
        puts("3. Buscar individuo.");
        puts("4. Alterar dados.");
        puts("5. Excluir dados.");
        puts("6. Apagar todos os dados.");
    
        printf("\n >>> ");
        scanf("%d", &seletor);

        switch (seletor) 
        { 
            case 0:
                exitMENU();
                break;
            
            case 1:
                inserirDados(initIND());
                break;

            case 2:
                exibirDados();
                break;

            case 3:
                buscarIND();
                break;

            case 4:
                alterarIND(NULL);
                break;

            case 5:
                excluirIND();
                break;

            case 6:
                remove(FILENAME);
                break;
            
            default:
                puts("Opção inválida! Tente novamente. \n");
                cleanBuffer();
                getchar();
                break;
        }
    }
}

int main(void) 
{
    initFILE();
    menu();

    return 0;
}