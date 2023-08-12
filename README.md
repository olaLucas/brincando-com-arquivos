# brincando-com-arquivos

# Introdução

Este repositório se resume a um simples CRUD (create, recovery, update and delete) de uma `struct` de indivíduos com nome, idade e altura que são salvos em um arquivo binário utilizando a biblioteca padrão `<stdio.h>` da linguagem C.

# Funções

Para realizar as operações sobre os dados, o programa conta com dois conjuntos de funções. Esta divisão foi feita para separar quem é responsável pela entrada dos dados (IND) e pelo processamento deles dentro dos arquivos (FILE) e para as funções não ficarem demasiadamente extensas (não tenho certeza se foi uma boa escolha, caso não tenha sido, iria agradecer imensamente por um feedback).

## IND Functions

Estas possuem a responsabilidade de receber os dados do usuário (para cadastro, alterações ou exclusão) e exibir mensagens de erro, caso ocorram. 

- **`ind initIND();` :**
    - responsável por perguntar os dados do indivíduo e retornar uma struct com eles;
- **`void exibirIND(ind temp);` :**
    - recebe uma struct como parâmetro e exibe os dados;
- **`void excluirIND();` :**
    - perguntar os nome do indivíduo e repassá-lo para a `excluirFILE()`;
    - exibir mensagens de erro, falha ou sucesso ao realizar a operação;
- **`void alterarIND(ind * pessoa);` :**
    - responsável por receber o nome do indivíduo e do dado que necessita ser alterado;
        - Ela conta com dois “modos” (perguntar o nome ou o atributo à ser alterado), que são acionados dependendo da endereço da struct passado como parâmetro;
    - exibir mensagens de erro, falha ou sucesso;
- **`void buscarIND();` :**
    - pergunta o nome do indivíduo;
    - chama e passa, os atributos para `buscarDados()`;
    - exibe mensagens de erro;
- **`int menuAlterarIND();` :**
    - pergunta o atributo à ser alterado, e os retorna para `buscarIND()`;

## FILE Functions

Estas tem como objetivo acessar diretamente o arquivo para realizar o registro, leituras, buscas e a exclusão dos indivíduos.

- **`void initFILE();`**
    - Verifica se o arquivo já existe. Se não, o cria e exibe uma mensagem com o resultado da operação (sucesso, ou falha).
- **`void inserirFILE(ind temp);`**
    - Registra o indivíduo recebido como parâmetro.
- **`void exibirFILE();`**
    - Percorre o arquivo em busca de registros;
    - Envia os registros encontrados para `exibirIND()`;
- **`int buscarFILE(const char nome[]);`**
    - Percorre o arquivo em busca de um indivíduo com o nome informado no parâmetro;
    - Envia o registro para `exibirIND()`;
    - Ao final retorna como `int`o resultado das operações;
- **`int excluirFILE(const char nomeExcluir[]);`:**
    - Percorre o arquivo em busca de um indivíduo com o nome informado no parâmetro;
    - Cria um arquivo temporário, aonde armazena os registros do arquivo principal com exceção do indivíduo selecionado para exclusão;
    - Verifica se o indivíduo se encontra registrado, se sim, fecha os arquivos e chama a função `overwriteFILE()`;
    - Ao final retorna como `int`o resultado das operações;
- **`int alterarFILE(const char nomeAlterar[]);`:**
    - Percorre o arquivo em busca de um indivíduo com o nome informado no parâmetro;
    - Cria um arquivo temporário aonde armazena os dados do arquivo principal;
    - Ao encontrar o indivíduo alvo, chama a função `alterarIND()` passando o endereço da `struct indivíduo temp`;
    - Fecha os arquivos e chama a função `overwrite()`;
- **`int overwriteFILE(const char nomeArquivoOriginal[], const char nomeArquivoTemp[]);`**
    - Recebe o nome do arquivo original e do arquivo temporário;
    - Sobrescreve os dados do arquivo temporário no arquivo original;
    - Por fim exclui a pasta temporária e retorna como `int` o resultado das operações;

# Conclusão

No geral, foi um projeto bom (na minha visão). Aprendi o básico de como lidar com arquivos em C, futuramente pretendo refazer esse mesmo projeto tabela hash (carregando os registros para RAM ao invés de manipula-los em memória secundária) e em C++ utilizando as bibliotecas da linguagem (talvez até com GUI usando GTK ou Qt 😏).
