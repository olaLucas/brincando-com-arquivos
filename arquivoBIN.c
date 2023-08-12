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

// file functions
void initFILE();
void inserirFILE(ind temp);
void exibirFILE();
int buscarDados(const char nome[]);
int excluirFILE(const char nomeExcluir[]);
int overwriteFILE(const char nomeArquivoOriginal[], const char nomeArquivoTemp[]);

// ind functions
ind initIND();
void exibirIndividuoIND(ind temp);
void excluirIND();
void alterarIND(ind * pessoa);
void buscarIND();
int menuAlterarIND();

// main functions
void menu();
void exitMENU();
void cleanBuffer();
void pause();


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
}

void inserirFILE(ind temp) 
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

void exibirFILE() 
{
    unsigned int counter = 0;
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
            counter++;
            exibirIndividuoIND(temp);
            puts("\n");
        }

        if (counter == 0)
        {
            puts("Arquivo vazio.");
        }
        else
        {
            pause();
        }
        
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
            puts("\nEncontrado. \n");
            exibirIndividuoIND(temp);
        
            puts("\nPressione enter para continuar..."); pause();
            return TRUE;
        }
        else
        {
            puts("Procurando...");
        }
    }
    
    return FALSE;
}

int excluirFILE(const char nomeExcluir[])
{
    ind temp;
    FILE * arquivoNovo;
    FILE * arquivoAntigo;
    size_t retornoFunctions = 0;
    int found = FALSE;
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

    while ((retornoFunctions = fread(&temp, sizeof(temp), 1, arquivoAntigo)) != 0)
    {
        if((retornoString = strcmp(temp.nome, nomeExcluir)) == 0)  // "apagando" o individuo alvo
        {
            found = TRUE;
        }
        else if(retornoString != 0) // "salvando" os demais individuos
        {
            fwrite(&temp, sizeof(struct individuo), 1, arquivoNovo);
        }
    }

    // checando o que retornar
    if (found == TRUE)
    {   
        // overwritting
        retornoFunctions = overwriteFILE(FILENAME, EXCLUIRFILENAME);

        if(retornoFunctions == FALSE)
        {
            return TRUE;
        }
        else
        {
            return ERROR;
        }
    }
    else
    {
        return FALSE;
    }
}

int alterarDados(const char nomeAlterar[])
{
    FILE * arquivoTemp;
    FILE * arquivoOriginal;
    size_t retornoFunctions = 0;
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

    while ((retornoFunctions = fread(&temp, sizeof(temp), 1, arquivoOriginal)) != 0)
    {
        if((retornoString = strcmp(temp.nome, nomeAlterar)) == 0)  // selecionando o individuo alvo para alteração
        {
            found = TRUE;
            alterarIND(&temp);
            fwrite(&temp, sizeof(temp), 1, arquivoTemp);
            
        }
        else if(retornoString != 0) // copiando os demais individuos
        {
            fwrite(&temp, sizeof(temp), 1, arquivoTemp);
        }
    }

    fclose(arquivoTemp);
    fclose(arquivoOriginal);

    retornoFunctions = overwriteFILE(FILENAME, ALTERARFILENAME);

    if(found == TRUE && retornoFunctions == FALSE)
    {
        return TRUE;
    }
    else if (found == FALSE && retornoFunctions == FALSE)
    {
        return FALSE;
    }
    else
    {
        return ERROR;
    }

}

int overwriteFILE(const char nomeArquivoOriginal[], const char nomeArquivoTemp[])
{
    int retornoFunctions;
    int error = FALSE;
    ind temp;
    FILE * arquivoOriginal;
    FILE * arquivoTemp;

    if ((arquivoOriginal = fopen(FILENAME, WRITEMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo antigo (overwrite).");
        return (error = TRUE);
    }

    if ((arquivoTemp = fopen(nomeArquivoTemp, READMODE)) == NULL)
    {
        puts("Não foi possível abrir o arquivo novo (overwrite).");
        return (error = TRUE);
    }

    while ((retornoFunctions = fread(&temp, sizeof(temp), 1, arquivoTemp)))
    {
        if((retornoFunctions = fwrite(&temp, sizeof(temp), 1, arquivoOriginal)) == FALSE) // escrevendo os dados do temp no arquivo antigo
        {
            puts("Ocorreu um erro ao registrar o individuo: \n\n");
            exibirIndividuoIND(temp);
            error = TRUE;
        }
    }

    fclose(arquivoOriginal);
    fclose(arquivoTemp);
    remove(nomeArquivoTemp);

    return error;
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

void exibirIndividuoIND(ind temp)
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

    if((retorno = excluirFILE(nome)) == ERROR)
    {
        puts("Ocorreu um erro ao realizar a operação.");
        pause();
    }
    else if(retorno == FALSE)
    {
        puts("Individuo não encontrado.");
    }
    else
    {
        puts("Individuo apagado com sucesso.");
        pause();
    }
}

void alterarIND(ind * pessoa)
{
    
    
    int retorno;
    char nome[NOMETAM];
    if(pessoa == NULL)
    {
        cleanBuffer();

        printf("\n\nInsira o nome: "); fgets(nome, NOMETAM, stdin);
        nome[strcspn(nome, "\n")] = '\0';

        if ((retorno = alterarDados(nome)) == FALSE)
        {
            puts("Não foi possível encontrar o individuo.");
        }
        else if(retorno == ERROR)
        {
            puts("Ocorreu um erro ao realizar a operação.");
        }
        else
        {
            puts("Informações alteradas com sucesso.");
        }
    }
    else if(pessoa != NULL)
    {
        exibirIndividuoIND(*pessoa);
        retorno = menuAlterarIND(); 

        switch (retorno)
        {
        case 1:
            cleanBuffer();
            printf("Insira o novo nome: ");
            fgets(nome, NOMETAM, stdin);
            nome[strcspn(nome, "\n")] = '\0';
            memcpy(pessoa->nome, nome, sizeof(nome));
            break;

        case 2:
            printf("Insira a nova idade: ");
            scanf("%d", &pessoa->idade);
            break;

        case 3:
            printf("Insira a nova altura: ");
            scanf("%f", &pessoa->altura);
            break;
        
        default:
            break;
        }
    }
}

void buscarIND()
{
    cleanBuffer();
    int retorno;
    char nome[NOMETAM];

    printf("\n\nInsira o nome: "); fgets(nome, NOMETAM, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    if(buscarDados(nome))
    {
        return;
    }
    else
    {
        puts("\nIndividuo não encontrado.");
        return;
    }
}

int menuAlterarIND()
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

// main
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

void menu()
{
    int seletor = -1;
    while (seletor != 0) 
    {
        //system("clear");
        
        puts("\nSelecione uma opção: \n");
        puts("0. Sair");
        puts("1. Inserir individuo.");
        puts("2. Imprimir individuo.");
        puts("3. Buscar individuo.");
        puts("4. Alterar individuo.");
        puts("5. Excluir individuo.");
        puts("6. Excluir arquivo.");
    
        printf("\n >>> ");
        scanf("%d", &seletor);

        switch (seletor) 
        { 
            case 0:
                exitMENU();
                break;
            
            case 1:
                puts("\n");
                inserirFILE(initIND());
                break;

            case 2:
                exibirFILE();
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