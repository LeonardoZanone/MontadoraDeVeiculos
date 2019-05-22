///Leonardo Zanone 160795 - Fausto Jurado 140761
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

#define MAX_LOJAS 5
#define MAX_CARROS 50

typedef struct endereco
{
    char logradouro[80];
    char bairro[15];
    char CEP[10];
    char cidade[15];
    char estado[3];
    char fone[12];
    char email[40];
} Endereco;

typedef struct infocarro
{
    char sigla;
    int regcarro;
} InfoCarro;

typedef union dadoscarro
{
    char sigla;
    InfoCarro reservado;
} DadosCarro;

typedef struct loja
{
    int regloja; //gerar automático
    char nome[30];
    char CNPJ[19];
    Endereco end;
    int sold;
    int reserved;
    DadosCarro tabela[3];
} Loja;

typedef struct infoloja
{
    char sigla;
    char CNPJ[19];
} InfoLoja;

typedef union dadosloja
{
    char sigla;
    InfoLoja reserva;
} DadosLoja;

typedef struct montadora
{
    int regcarro; //gerar automático
    char modelo[20];
    char cor[10];
    float valor;
    DadosLoja status;
} Montadora;

///Funções principais
void setUp();
void menu();
void redirect(char input);
void maiuscula(char *p);

///Funções para Loja
void alocaLoja(Loja ** p, int tam);
void alocaEndereco(Endereco **p, int tam);
void cadastroLoja();
void cadastroEndereco(Endereco *pEndereco);
void mostraLoja(Loja *pLoja);
void gravaLoja(Loja *pLoja);
int verificaLoja();
Loja* buscaLoja(int reg);
void mostraLojas();
Loja* buscaLojaPorCNPJ(char *cnpj);
void mostraLojaPorCNPJ();

///Funções para Montadora
void alocaMontadora(Montadora **p, int tam);
void cadastroMontadora();
int verificaMontadora();
void gravaMontadora(Loja *pMontadors);
void mostraMontadora(Montadora* pMontadora);
Montadora *buscaMontadora(int reg);
void buscaMontadoraPorStatus(char status);
void mostraMontadoraPorStatus();
void buscaMontadoraPorModelo(char *modelo);
void mostraMontadoraPorModelo();

int main()
{
    char input;

    setUp();

    do
    {
        menu();
        input = getch();
        redirect(input);
    }
    while(input != '8');

    return 0;
}

void setUp()
{
    setlocale(LC_ALL, "Portuguese");

    HWND hWnd;
    SetConsoleTitle("Concessionaria de Veiculos");
    hWnd = FindWindow(NULL, "Concessionaria de Veiculos");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
    SMALL_RECT DisplayArea = {0, 0, 0, 0};
    SetConsoleScreenBufferSize(hOut, NewSBSize);
    DisplayArea.Top = 0;
    DisplayArea.Left = 0;
    DisplayArea.Right = 149;
    DisplayArea.Bottom = 34;
    SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
    ShowWindow(hWnd, SW_MAXIMIZE);
}

void menu()
{
    system("cls");
    printf("1 - Cadastro Loja");
    printf("\n2 - Mostrar Lojas");
    printf("\n3 - Cadastro Montadora");
    printf("\n4 - Busca Montadora");
    printf("\n5 - Mostrar Montadoras");
    printf("\n6 - Mostrar Montadoras Por Status");
    printf("\n7 - Mostrar Montadoras Por Modelos");
    printf("\n8 - Sair\n\n\n");
}

void redirect(char input)
{
    switch(input)
    {
    case '1':
        cadastroLoja();
        break;
    case '2':
        mostraLojas();
        break;
    case '3':
        cadastroMontadora();
        break;
    case '4':
        mostraMontadoras();
        break;
    case '5':
        mostraLojaPorCNPJ();
        break;
    case '6':
        mostraMontadoraPorStatus();
        break;
    case '7':
        mostraMontadoraPorModelo();
        break;
    default:
        break;
    }
}

void alocaLoja(Loja ** p, int tam)
{
    if((*p = (Loja *) realloc(*p, tam * sizeof(Loja))) == NULL)
    {
        printf("Erro ao alocar memória");
        exit(1);
    }
}

void alocaEndereco(Endereco **p, int tam)
{
    if((*p = (Endereco *) realloc(*p, tam * sizeof(Endereco))) == NULL)
    {
        printf("Erro ao alocar memória");
        exit(1);
    }
}

void cadastroLoja()
{
    system("cls");

    int qtd = verificaLoja();
    if(qtd >= MAX_LOJAS)
    {
        printf("Número máximo de lojas atingido");
        return;
    }

    Loja *pLoja = NULL;
    alocaLoja(&pLoja, 1);
    char aux[30];

    printf("Digite o nome da Loja: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pLoja->nome, aux);
    printf("Digite o CNPJ da Loja: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pLoja->CNPJ, aux);
    pLoja->regloja = verificaLoja() + 1;
    pLoja->reserved = 0;
    pLoja->sold = 0;
    pLoja->tabela[0].sigla = 'L';
    pLoja->tabela[1].sigla = 'L';
    pLoja->tabela[2].sigla = 'L';
    cadastroEndereco(&(pLoja->end));
    system("cls");
    mostraLoja(pLoja);
    printf("\n\n\n");
    gravaLoja(pLoja);
    system("pause");
}

void maiuscula(char *p)
{
    for(int i = 0; i < strlen(p); i++)
    {
        if(*(p + i) >= 'a' && *(p + i) <= 'z')
        {
            *(p + i) -= 32;
        }
    }
}

void cadastroEndereco(Endereco *pEndereco)
{
    ///TODO Limitar o tamanho de aux para o equivalente ao membro da estrutura
    char aux[80];
    fflush(stdin);
    printf("Digite o logradouro: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->logradouro, aux);
    printf("Digite o bairro: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->bairro, aux);
    printf("Digite o CEP: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->CEP, aux);
    printf("Digite a cidade: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->cidade, aux);
    printf("Digite o estado: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->estado, aux);
    printf("Digite o telefone: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->fone, aux);
    printf("Digite o e-mail: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pEndereco->email, aux);
}

void mostraLoja(Loja *pLoja)
{
    printf("Nome: %s", pLoja->nome);
    printf("\nCNPJ: %s", pLoja->CNPJ);
    printf("\nRegistro: %i", pLoja->regloja);
    printf("\nReservados: %i", pLoja->reserved);
    printf("\nVendidos: %i", pLoja->sold);
    printf("\nLogradouro: %s", pLoja->end.logradouro);
    printf("\nBairo: %s", pLoja->end.bairro);
    printf("\nCEP: %s", pLoja->end.CEP);
    printf("\nCidade: %s", pLoja->end.cidade);
    printf("\nEstado: %s", pLoja->end.estado);
    printf("\nTelefone: %s", pLoja->end.fone);
    printf("\nE-mail: %s", pLoja->end.email);
    if(pLoja->tabela[0].sigla == 'L')
        printf("\nTabela 0: %c", pLoja->tabela[0].sigla);
    else
        printf("\nTabela 0: %c - Registro: %i", pLoja->tabela[0].reservado.sigla, pLoja->tabela[0].reservado.regcarro);
    if(pLoja->tabela[1].sigla == 'L')
        printf("\nTabela 1: %c", pLoja->tabela[1].sigla);
    else
        printf("\nTabela 1: %c - Registro: %i", pLoja->tabela[1].reservado.sigla, pLoja->tabela[1].reservado.regcarro);
    if(pLoja->tabela[2].sigla == 'L')
        printf("\nTabela 2: %c", pLoja->tabela[2].sigla);
    else
        printf("\nTabela 2: %c - Registro: %i", pLoja->tabela[2].reservado.sigla, pLoja->tabela[2].reservado.regcarro);

}

void gravaLoja(Loja *pLoja)
{
    FILE *fptr;

    if((fptr = fopen("concessionaria.bin", "ab")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        fwrite(pLoja, sizeof(Loja), 1, fptr);
    }
    fclose(fptr);
}

int verificaLoja()
{
    FILE *fptr;
    long int pos;
    int qtd = 0;

    if((fptr = fopen("concessionaria.bin", "rb")) != NULL)
    {
        fseek(fptr, 0, SEEK_END);
        pos = ftell(fptr);
        qtd = pos / sizeof(Loja);
        fclose(fptr);
    }
    return qtd;
}

Loja* buscaLoja(int reg)
{
    FILE *fptr;
    Loja *pLoja = NULL;
    int i, qtd = verificaLoja();
    alocaLoja(&pLoja, 1);

    if((fptr = fopen("concessionaria.bin", "rb")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        for(i = 0; i < qtd; i++)
        {
            fread(pLoja, sizeof(Loja), 1, fptr);
            if(pLoja->regloja == reg)
                return pLoja;
        }
    }

    return NULL;
}

void mostraLojas()
{
    system("cls");
    int qtd = verificaLoja();

    printf("========================================================================================\n");
    for(int i = 1; i <= qtd; i++)
    {
        Loja *pLoja = buscaLoja(i);
        if(pLoja != NULL)
        {
            mostraLoja(pLoja);
            printf("\n========================================================================================\n");
        }
    }
    printf("\n\n\n");
    system("pause");
}

Loja* buscaLojaPorCNPJ(char *cnpj)
{
    FILE *fptr;
    Loja *pLoja = NULL;
    int i, qtd = verificaLoja();
    alocaLoja(&pLoja, 1);

    if((fptr = fopen("concessionaria.bin", "rb")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        for(i = 0; i < qtd; i++)
        {
            fread(pLoja, sizeof(Loja), 1, fptr);
            if(strcmp(cnpj, pLoja->CNPJ) == 0)
                return pLoja;
        }
    }

    return NULL;
}

void mostraLojaPorCNPJ()
{
    char aux[19];
    system("cls");
    fflush(stdin);
    printf("Digite o CNPJ: ");
    gets(aux);
    Loja *pLoja = buscaLojaPorCNPJ(aux);
    if(pLoja == NULL)
        printf("\nLoja não encontrada");
    else
        mostraLoja(pLoja);
    printf("\n\n\n");
    system("pause");

}
void alocaMontadora(Montadora **p, int tam)
{
    if((*p = (Montadora*) realloc(*p, tam * sizeof(Montadora))) == NULL)
    {
        printf("Erro ao alocar memória");
        exit(1);
    }
}

void cadastroMontadora()
{
    system("cls");

    Montadora *pMontadora = NULL;
    alocaMontadora(&pMontadora, 1);
    char aux[20];

    pMontadora->regcarro = verificaMontadora() + 1;
    fflush(stdin);
    printf("Digite o modelo do carro: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pMontadora->modelo, aux);
    fflush(stdin);
    printf("Digite a cor do carro: ");
    gets(aux);
    maiuscula(aux);
    strcpy(pMontadora->cor, aux);
    printf("Digite o valor do carro: ");
    scanf("%f", &pMontadora->valor);
    pMontadora->status.sigla = 'L';
    gravaMontadora(pMontadora);

    system("pause");
}

int verificaMontadora()
{
    FILE *fptr;
    long int pos;
    int qtd = 0;

    if((fptr = fopen("carro.bin", "rb")) != NULL)
    {
        fseek(fptr, 0, SEEK_END);
        pos = ftell(fptr);
        qtd = pos / sizeof(Montadora);
        fclose(fptr);
    }
    return qtd;
}

void mostraMontadora(Montadora* pMontadora)
{
    printf("Registro: %i", pMontadora->regcarro);
    printf("\nModelo: %s", pMontadora->modelo);
    printf("\nCor: %s", pMontadora->cor);
    printf("\nValor: %.2f", pMontadora->valor);
    if(pMontadora->status.sigla == 'L')
        printf("\nStatus: %c", pMontadora->status.sigla);
    else
        printf("\nStatus: %c - CNPJ: %s", pMontadora->status.reserva.sigla, pMontadora->status.reserva.CNPJ);
}

void gravaMontadora(Loja *pMontadors)
{
    FILE *fptr;

    if((fptr = fopen("carro.bin", "ab")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        fwrite(pMontadors, sizeof(Montadora), 1, fptr);
    }
    fclose(fptr);
}

Montadora *buscaMontadora(int reg)
{
    FILE *fptr;
    Montadora *pMontadora = NULL;
    int i, qtd = verificaMontadora();
    alocaMontadora(&pMontadora, 1);

    if((fptr = fopen("carro.bin", "rb")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        for(i = 0; i < qtd; i++)
        {
            fread(pMontadora, sizeof(Montadora), 1, fptr);
            if(pMontadora->regcarro == reg)
                return pMontadora;
        }
        fclose(fptr);
    }

    return NULL;
}

void mostraMontadoras()
{
    system("cls");
    int qtd = verificaMontadora();
    printf("========================================================================================\n");
    for(int i = 1; i <= qtd; i++)
    {
        Montadora *pMontadora = buscaMontadora(i);
        if(pMontadora != NULL)
        {
            mostraMontadora(pMontadora);
            printf("\n========================================================================================\n");
        }
    }
    printf("\n\n\n");
    system("pause");
}

void buscaMontadoraPorStatus(char status)
{
    FILE *fptr;
    Montadora *pMontadora = NULL;
    int i, qtd = verificaMontadora();
    alocaMontadora(&pMontadora, 1);
    maiuscula(&status);

    if((fptr = fopen("carro.bin", "rb")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        for(i = 0; i < qtd; i++)
        {
            fread(pMontadora, sizeof(Montadora), 1, fptr);
            if(pMontadora->status.sigla == status)
            {
                printf("========================================================================================\n");
                mostraMontadora(pMontadora);
                printf("\n");
            }
        }
        fclose(fptr);
    }
}

void mostraMontadoraPorStatus()
{
    char status;
    system("cls");
    fflush(stdin);
    printf("Digite o status: ");
    scanf("%c", &status);
    buscaMontadoraPorStatus(status);
    printf("\n\n\n");
    system("pause");
}

void buscaMontadoraPorModelo(char *modelo)
{
    FILE *fptr;
    Montadora *pMontadora = NULL;
    int i, qtd = verificaMontadora();
    alocaMontadora(&pMontadora, 1);
    maiuscula(modelo);
    if((fptr = fopen("carro.bin", "rb")) == NULL)
        printf("Erro ao abrir o arquivo!");
    else
    {
        for(i = 0; i < qtd; i++)
        {
            fread(pMontadora, sizeof(Montadora), 1, fptr);
            if(strcmp(modelo, pMontadora->modelo) == 0)
            {
                printf("========================================================================================\n");
                mostraMontadora(pMontadora);
                printf("\n");
            }
        }
        fclose(fptr);
    }
}

void mostraMontadoraPorModelo()
{
    char aux[20];
    system("cls");
    fflush(stdin);
    printf("Digite o modelo: ");
    gets(aux);
    buscaMontadoraPorModelo(aux);
    printf("\n\n\n");
    system("pause");
}


