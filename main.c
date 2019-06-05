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
#define MAX_RESERVED 3
#define NUM_OPTIONS 12

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

typedef struct data
{
	int dia;
	int mes;
	int ano;
} Data;

typedef struct historicoVendas
{
	int reghist;
	int regcarro;
	char modelo[20];
	char cor[10];
	float valor;
	int regloja;
	char nome[30];
	char cnpj[19];
	Data dataVenda;
} HistoricoVendas;

///Funções principais
void setUp();
void menu();
void redirect(int input);
void maiuscula(char *p);
void printMenuPrincipal();
void loop();

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
void mostraLojasTabela();
Loja* buscaLojaPorCNPJ(char *cnpj);
void mostraLojaPorCNPJ();
void alteraLoja(Loja *pLoja);
void consultaConcessionaria();

///Funções para Montadora
void alocaMontadora(Montadora **p, int tam);
void cadastroMontadora();
int verificaMontadora();
void gravaMontadora(Montadora *pMontadora);
void mostraMontadora(Montadora* pMontadora);
Montadora *buscaMontadora(int reg);
void buscaMontadoraPorStatus(char status);
void mostraMontadoraPorStatus();
void buscaMontadoraPorModelo(char *modelo);
void mostraMontadoraPorModelo();
void mostraMontadoras();
void alteraMontadora(Montadora *pMontadora);
void mostraMontadorasTabela();

///Funcões de lógica de negócio
void reservaCarro();
void finalizarReserva();
HistoricoVendas* gerarHistorico(Montadora *pMontadora, Loja *pLoja, Data *pData);
HistoricoVendas *buscaHistorico(int reg);
void debugRedirect();
void mostraHistorico();
void editaLoja();
void editaMontadora();
void alocaData(Data **p, int tam);
void gravaHistorico(HistoricoVendas *pHistorico);
void alocaHistorico(HistoricoVendas** p, int tam);
int verificaHistorico();
bool validaData(Data *data);

///TODO tentar fazer função genérica de gravação
void grava(void *p, int size, const char *arquivo);

//Função principal
int main()
{
	setUp();

	loop();
	return 0;
}


//Loop do menu principal
void loop()
{
	char input;
	int option = 0;
	do
	{
		printMenuPrincipal(option);
		fflush(stdin);
		input = getch();
		if (input == 'w')
		{
			if (option == 0)
				option = NUM_OPTIONS;
			option--;
		}
		else if (input == 's')
		{
			if (option == NUM_OPTIONS - 1)
				option = -1;
			option++;
		}
		else if (input == 32 || input == 13 || input == 'd')
		{
			if (input == 'd') {
				debugRedirect();
			}
			else if (option == NUM_OPTIONS - 1)
			{
				break;
			}
			else
				redirect(option);
		}
		else if (input == 27)
			input = 'q';
	} while (input != 'q');
}

//Função que muda o tamanho da janela, coloca o nome e muda output do console para UTF-8
void setUp()
{
	system("color 02");
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleTitle("Concessionaria de Veiculos");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	SetConsoleScreenBufferSize(hOut, NewSBSize);
	DisplayArea.Top = 0;
	DisplayArea.Left = 0;
	DisplayArea.Right = 149;
	DisplayArea.Bottom = 34;
	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
}

//Exibe na tela o menu principal recebendo como argumento a posição atual da seta
void printMenuPrincipal(int option)
{
	//Array que controla a seta que aparece na frente da opção a ser selecionada
	char arrows[NUM_OPTIONS];

	for (int i = 0; i < NUM_OPTIONS; i++)
	{
		arrows[i] = ' ';
	}
	arrows[option] = '>';

	system("cls");
	printf("======================================================================================================================================================\n");
	for (int i = 0; i < 10; i++)
	{
		printf("=\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     =\n");
	}

	printf("=                                                              %c Cadastro Loja                                                                       =\n", arrows[0]);
	printf("=                                                              %c Mostrar Lojas                                                                       =\n", arrows[1]);
	printf("=                                                              %c Cadastro Montadora                                                                  =\n", arrows[2]);
	printf("=                                                              %c Mostrar Montadoras                                                                  =\n", arrows[3]);
	printf("=                                                              %c Mostrar Montadoras Por Status                                                       =\n", arrows[4]);
	printf("=                                                              %c Mostrar Montadoras Por Modelo                                                       =\n", arrows[5]);
	printf("=                                                              %c Reserva                                                                             =\n", arrows[6]);
	printf("=                                                              %c Finalizar Reserva                                                                   =\n", arrows[7]);
	printf("=                                                              %c Mostrar Histórico                                                                   =\n", arrows[8]);
	printf("=                                                              %c Editar Loja                                                                         =\n", arrows[9]);
	printf("=                                                              %c Editar Montadora                                                                    =\n", arrows[10]);
	printf("=                                                              %c Sair                                                                                =\n", arrows[11]);

	for (int i = 0; i < 9; i++)
	{
		printf("=\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t     =\n");
	}
	printf("=     Utilize as teclas 'w' e 's' para mover o cursor e as teclas espaco ou enter para confirmar. Voce pode usar os atalhos 'q' ou esc para sair     =\n");
	printf("======================================================================================================================================================\n");
}

//Função que redireciona para a opção selecionada no menu principal
void redirect(int input)
{
	switch (input)
	{
	case 0:
		cadastroLoja();
		break;
	case 1:
		mostraLojasTabela();
		break;
	case 2:
		cadastroMontadora();
		break;
	case 3:
		mostraMontadorasTabela();
		break;
	case 4:
		mostraMontadoraPorStatus();
		break;
	case 5:
		mostraMontadoraPorModelo();
		break;
	case 6:
		reservaCarro();
		break;
	case 7:
		finalizarReserva();
		break;
	case 8:
		mostraHistorico();
		break;
	case 9:
		editaLoja();
		break;
	case 10:
		editaMontadora();
		break;
	default:
		break;
	}
}

//Menu secundário que pode ser acessado com a tecla 'd', utilizado para debuggar o código
void menu()
{
	system("cls");
	printf("1 - Cadastro Loja");
	printf("\n2 - Mostrar Lojas");
	printf("\n3 - Mostrar Lojas Tabela");
	printf("\n4 - Consulta Loja Tabela");
	printf("\n5 - Cadastro Montadora");
	printf("\n6 - Mostrar Montadoras");
	printf("\n7 - Mostrar Montadoras");
	printf("\n8 - Mostrar Montadoras Tabela");
	printf("\n9 - Buscar Montadoras Por CNPJ");
	printf("\na - Buscar Montadoras Por Status");
	printf("\nb - Buscar Montadoras Por Modelos");
	printf("\nc - Reserva");
	printf("\nd - Finalizar Reserva");
	printf("\ne - Alterar Concessionária");
	printf("\nf - Alterar Montadora");
	printf("\nq - Sair\n\n\n");
}

//Redirecionamento do menu de debug
void debugRedirect()
{
	char input;

	do
	{
		menu();
		input = getch();
		switch (input)
		{
		case '1':
			cadastroLoja();
			break;
		case '2':
			mostraLojas();
			break;
		case '3':
			mostraLojasTabela();
			break;
		case '4':
			consultaConcessionaria();
			break;
		case '5':
			cadastroMontadora();
			break;
		case '6':
			mostraMontadoras();
			break;
		case '7':
			mostraMontadoras();
			break;
		case '8':
			mostraMontadorasTabela();
			break;
		case '9':
			mostraLojaPorCNPJ();
			break;
		case 'a':
			mostraMontadoraPorStatus();
			break;
		case 'b':
			mostraMontadoraPorModelo();
			break;
		case 'c':
			reservaCarro();
			break;
		case 'd':
			finalizarReserva();
			break;
		case 'e':
			editaLoja();
			break;
		default:
			editaMontadora();
			break;
		}
	} while (input != 'q');

}

//Aloca memória para struct Loja
void alocaLoja(Loja ** p, int tam)
{
	if ((*p = (Loja *)realloc(*p, tam * sizeof(Loja))) == NULL)
	{
		printf("Erro ao alocar memória");
		exit(1);
	}
}

//Aloca memória para struct Endereço
void alocaEndereco(Endereco **p, int tam)
{
	if ((*p = (Endereco *)realloc(*p, tam * sizeof(Endereco))) == NULL)
	{
		printf("Erro ao alocar memória");
		exit(1);
	}
}

//Função responsável pelo cadastro da loja
void cadastroLoja()
{
	system("cls");

	int qtd = verificaLoja();
	if (qtd >= MAX_LOJAS)
	{
		printf("Número máximo de lojas atingido!\n\n");
		system("pause");
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
	(pLoja->tabela)->sigla = 'L';
	(pLoja->tabela + 1)->sigla = 'L';
	(pLoja->tabela + 2)->sigla = 'L';
	cadastroEndereco(&(pLoja->end));
	system("cls");
	mostraLoja(pLoja);
	printf("\n\n\n");
	gravaLoja(pLoja);
	system("pause");
}


//Função equivalente ao toUpperCase
void maiuscula(char *p)
{
	for (int i = 0; i < strlen(p); i++)
	{
		if (*(p + i) >= 'a' && *(p + i) <= 'z')
		{
			*(p + i) -= 32;
		}
	}
}

//Função que recebe os valores do endereço de uma concessionária
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

//Exibe todos os elementos de uma struct Loja
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
	if ((pLoja->tabela)->sigla == 'L')
		printf("\nTabela 0: %c", (pLoja->tabela)->sigla);
	else
		printf("\nTabela 0: %c - Registro: %i", (pLoja->tabela)->reservado.sigla, (pLoja->tabela)->reservado.regcarro);
	if ((pLoja->tabela + 1)->sigla == 'L')
		printf("\nTabela 1: %c", (pLoja->tabela + 1)->sigla);
	else
		printf("\nTabela 1: %c - Registro: %i", (pLoja->tabela + 1)->reservado.sigla, (pLoja->tabela + 1)->reservado.regcarro);
	if ((pLoja->tabela + 2)->sigla == 'L')
		printf("\nTabela 2: %c", (pLoja->tabela + 2)->sigla);
	else
		printf("\nTabela 2: %c - Registro: %i", (pLoja->tabela + 2)->reservado.sigla, (pLoja->tabela + 2)->reservado.regcarro);

}

//Exibe alguns campos da struct Loja em forma de tabela
void mostraLojasTabela()
{
	system("cls");
	int qtd = verificaLoja();

	printf("|%-9s|%-30s|%-19s|%-30s|%-11s|%-12s|%-10s|%-10s|%-10s|", "Registro", "Nome", "CNPJ", "Endereco", "Vendidos", "Reservados", "Tabela 0", "Tabela 1", "Tabela 2");

	for (int i = 0; i <= qtd; i++)
	{
		Loja *pLoja = buscaLoja(i);
		if (pLoja != NULL)
		{
			printf("\n");
			printf("|%-9d", pLoja->regloja);
			printf("|%-30s", pLoja->nome);
			printf("|%-19s", pLoja->CNPJ);
			printf("|%-30s", pLoja->end.logradouro);
			printf("|%-11i", pLoja->sold);
			printf("|%-12i", pLoja->reserved);
			if ((pLoja->tabela)->sigla == 'L')
				printf("|%-10c", (pLoja->tabela)->sigla);
			else
				printf("|%-3c - %-4i", (pLoja->tabela)->reservado.sigla, (pLoja->tabela)->reservado.regcarro);
			if ((pLoja->tabela + 1)->sigla == 'L')
				printf("|%-10c", (pLoja->tabela)->sigla);
			else
				printf("|%-3c - %-4i", (pLoja->tabela + 1)->reservado.sigla, (pLoja->tabela + 1)->reservado.regcarro);
			if ((pLoja->tabela + 2)->sigla == 'L')
				printf("|%-10c", (pLoja->tabela + 2)->sigla);
			else
				printf("|%-3c - %-4i", (pLoja->tabela + 2)->reservado.sigla, (pLoja->tabela + 2)->reservado.regcarro);
		}
	}
	printf("\n\n\n");
	system("pause");
}

//Exibe algumas informações de uma conscessionária selecionada pelo CNPJ
void consultaConcessionaria()
{
	system("cls");
	char aux[19];
	fflush(stdin);
	printf("Digite o CNPJ da Concessionária: ");
	gets(aux);
	Loja *pLoja = buscaLojaPorCNPJ(aux);
	if (pLoja != NULL)
	{
		printf("|%-30s|%-11s|%-12s", "Nome", "Vendidos", "Reservados");
		for (int i = 0; i < MAX_RESERVED; i++)
		{
			if ((*(pLoja->tabela + i)).sigla == 'R')
			{
				printf("|Tabela %i", i);
			}
		}
		printf("|\n");
		printf("|%-30s", pLoja->nome);
		printf("|%-11i", pLoja->sold);
		printf("|%-12i", pLoja->reserved);
		for (int i = 0; i < MAX_RESERVED; i++)
		{
			if ((*(pLoja->tabela + i)).sigla == 'R')
			{
				printf("|%-2c - %-3i", (*(pLoja->tabela + i)).sigla, (*(pLoja->tabela + i)).reservado.regcarro);
			}
		}
		printf("|\n\n");
	}
	else
	{
		printf("\nConcessionária não encontrada!");
	}
	system("pause");
}

//Persiste uma structv Loja no arquivo concessionário.bin
void gravaLoja(Loja *pLoja)
{
	FILE *fptr;

	if ((fptr = fopen("concessionaria.bin", "ab")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		fwrite(pLoja, sizeof(Loja), 1, fptr);
	}
	fclose(fptr);
}

//Verifica quantos elementos do tipo Loja estão armazenados no arquivo
int verificaLoja()
{
	FILE *fptr;
	long int pos;
	int qtd = 0;

	if ((fptr = fopen("concessionaria.bin", "rb")) != NULL)
	{
		fseek(fptr, 0, SEEK_END);
		pos = ftell(fptr);
		qtd = pos / sizeof(Loja);
		fclose(fptr);
	}
	return qtd;
}

//Resgada uma informação de uma loja do arquivo a partir do número de registro
Loja* buscaLoja(int reg)
{
	FILE *fptr;
	Loja *pLoja = NULL;
	int i, qtd = verificaLoja();
	alocaLoja(&pLoja, 1);

	if ((fptr = fopen("concessionaria.bin", "rb")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(pLoja, sizeof(Loja), 1, fptr);
			if (pLoja->regloja == reg)
				return pLoja;
		}
	}

	return NULL;
}

//Exibe todas as estrutuas Loja
void mostraLojas()
{
	system("cls");
	int qtd = verificaLoja();

	printf("========================================================================================\n");
	for (int i = 1; i <= qtd; i++)
	{
		Loja *pLoja = buscaLoja(i);
		if (pLoja != NULL)
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

	if ((fptr = fopen("concessionaria.bin", "rb")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(pLoja, sizeof(Loja), 1, fptr);
			if (strcmp(cnpj, pLoja->CNPJ) == 0)
				return pLoja;
		}
	}

	return NULL;
}

//Exibe uma estrutura Loja filtrada pelo CNPJ
void mostraLojaPorCNPJ()
{
	char aux[19];
	system("cls");
	fflush(stdin);
	printf("Digite o CNPJ: ");
	gets(aux);
	Loja *pLoja = buscaLojaPorCNPJ(aux);
	if (pLoja == NULL)
		printf("\nLoja não encontrada");
	else
		mostraLoja(pLoja);
	printf("\n\n\n");
	system("pause");

}

//Aloca memória para uma struct Montadora
void alocaMontadora(Montadora **p, int tam)
{
	if ((*p = (Montadora*)realloc(*p, tam * sizeof(Montadora))) == NULL)
	{
		printf("Erro ao alocar memória");
		exit(1);
	}
}

//Cadastra uma montadora
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

//Verifica a quantidade de estruturas Montadora estão no arquivo
int verificaMontadora()
{
	FILE *fptr;
	long int pos;
	int qtd = 0;

	if ((fptr = fopen("carro.bin", "rb")) != NULL)
	{
		fseek(fptr, 0, SEEK_END);
		pos = ftell(fptr);
		qtd = pos / sizeof(Montadora);
		fclose(fptr);
	}
	return qtd;
}

//Mostra todos os campos de uma struct Montadora
void mostraMontadora(Montadora* pMontadora)
{
	printf("Registro: %i", pMontadora->regcarro);
	printf("\nModelo: %s", pMontadora->modelo);
	printf("\nCor: %s", pMontadora->cor);
	printf("\nValor: %.2f", pMontadora->valor);
	if (pMontadora->status.sigla == 'L')
		printf("\nStatus: %c", pMontadora->status.sigla);
	else
		printf("\nStatus: %c - CNPJ: %s", pMontadora->status.reserva.sigla, pMontadora->status.reserva.CNPJ);
}

//Mostra a uma mentadora em formato de tabela
void mostraMontadoraTabela(Montadora* pMontadora)
{
	if (pMontadora->status.sigla == 'L')
		printf("|%-10i|%-20s|%-10s|%-10.2f|%-23c|", pMontadora->regcarro, pMontadora->modelo, pMontadora->cor, pMontadora->valor, pMontadora->status.sigla);
	else if (pMontadora->status.sigla == 'R')
		printf("|%-10i|%-20s|%-10s|%-10.2f|%c - %-19s|", pMontadora->regcarro, pMontadora->modelo, pMontadora->cor, pMontadora->valor, pMontadora->status.reserva.sigla, pMontadora->status.reserva.CNPJ);
}

//Persiste uma struct Montadora no arquivo carro.bin
void gravaMontadora(Montadora *pMontadora)
{
	FILE *fptr;

	if ((fptr = fopen("carro.bin", "ab")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		fwrite(pMontadora, sizeof(Montadora), 1, fptr);
	}
	fclose(fptr);
}

//Busca uma Montadora no arquivo a partir do registro
Montadora *buscaMontadora(int reg)
{
	FILE *fptr;
	Montadora *pMontadora = NULL;
	int i, qtd = verificaMontadora();
	alocaMontadora(&pMontadora, 1);

	if ((fptr = fopen("carro.bin", "rb")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(pMontadora, sizeof(Montadora), 1, fptr);
			if (pMontadora->regcarro == reg)
				return pMontadora;
		}
		fclose(fptr);
	}

	return NULL;
}

//Exibe todas as montadoras
void mostraMontadoras()
{
	system("cls");
	int qtd = verificaMontadora();
	printf("========================================================================================\n");
	for (int i = 1; i <= qtd; i++)
	{
		Montadora *pMontadora = buscaMontadora(i);
		if (pMontadora != NULL)
		{
			mostraMontadora(pMontadora);
			printf("\n========================================================================================\n");
		}
	}
	printf("\n\n\n");
	system("pause");
}

//Exibe todas as montadoras em formato de tabela
void mostraMontadorasTabela()
{
	system("cls");
	int qtd = verificaMontadora();

	if (qtd == 0)
	{
		printf("Nenhum registro encontrado!");
		system("pause");
		return;
	}
	printf("|%-10s|%-20s|%-10s|%-10s|%-23s|\n", "Registro", "Modelo", "Cor", "Valor", "Status");
	for (int i = 1; i <= qtd; i++)
	{
		Montadora *pMontadora = buscaMontadora(i);
		if (pMontadora != NULL && pMontadora->status.sigla != 'E')
		{
			mostraMontadoraTabela(pMontadora);
			printf("\n");
		}
	}
	printf("\n\n\n");
	system("pause");
}

//Busca e exibe as montadores de um determinado status
void buscaMontadoraPorStatus(char status)
{
	FILE *fptr;
	Montadora *pMontadora = NULL;
	int i, qtd = verificaMontadora();
	alocaMontadora(&pMontadora, 1);
	maiuscula(&status);

	if ((fptr = fopen("carro.bin", "rb")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(pMontadora, sizeof(Montadora), 1, fptr);
			if (pMontadora->status.sigla == status)
			{
				mostraMontadoraTabela(pMontadora);
				printf("\n");
			}
		}
		fclose(fptr);
	}
}

//Exibe as montadoras de um determinado estado
void mostraMontadoraPorStatus()
{
	char status;
	system("cls");
	fflush(stdin);
	printf("Digite o status: ");
	scanf("%c", &status);
	printf("|%-10s|%-20s|%-10s|%-10s|%-23s|\n", "Registro", "Modelo", "Cor", "Valor", "Status");
	buscaMontadoraPorStatus(status);
	printf("\n\n\n");
	system("pause");
}

//Busca e exibe montadoras com um certo modelo passado
void buscaMontadoraPorModelo(char *modelo)
{
	FILE *fptr;
	Montadora *pMontadora = NULL;
	int i, qtd = verificaMontadora();
	alocaMontadora(&pMontadora, 1);
	maiuscula(modelo);
	if ((fptr = fopen("carro.bin", "rb")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(pMontadora, sizeof(Montadora), 1, fptr);
			if (strcmp(modelo, pMontadora->modelo) == 0)
			{
				mostraMontadoraTabela(pMontadora);
				printf("\n");
			}
		}
		fclose(fptr);
	}
}

//Mostra todas as montadoras pertencentes a um dado modelo
void mostraMontadoraPorModelo()
{
	char aux[20];
	system("cls");
	fflush(stdin);
	printf("Digite o modelo: ");
	gets(aux);
	printf("|%-10s|%-20s|%-10s|%-10s|%-23s|\n", "Registro", "Modelo", "Cor", "Valor", "Status");
	buscaMontadoraPorModelo(aux);
	printf("\n\n\n");
	system("pause");
}

//Função responsável pela lógica da reserva de um carro
void reservaCarro()
{
	char aux[19];
	int regCar;
	Montadora *pMontadora;
	system("cls");
	printf("Qual o CNPJ da concessionária: ");
	gets(aux);
	Loja *pLoja = buscaLojaPorCNPJ(aux);
	if (pLoja == NULL)
		printf("\nConcessionária não encontrada!\n\n");
	else if (pLoja->reserved >= MAX_RESERVED)
		printf("\nLimite de reservas atingido!\n\n");
	else
	{
		mostraLoja(pLoja);
		printf("\n\n");
		printf("\nQual o registro do carro que deseja reservar: ");
		scanf("%i", &regCar);
		pMontadora = buscaMontadora(regCar);
		if (pMontadora == NULL)
		{
			printf("\nCarro não encontrado!\n\n");
			system("pause");
			return;
		}
		if (pMontadora->status.sigla != 'L')
		{
			printf("\nO carro não está disponível!\n\n");
			system("pause");
			return;
		}
		mostraMontadora(pMontadora);
		printf("\n\n");
		if (pMontadora->status.sigla == 'L')
		{
			for (int i = 0; i < MAX_RESERVED; i++)
			{
				if ((*(pLoja->tabela + i)).sigla == 'L')
				{
					(pLoja->tabela + i)->reservado.sigla = 'R';
					(pLoja->tabela + i)->reservado.regcarro = regCar;
					pLoja->reserved = pLoja->reserved + 1;
					pMontadora->status.reserva.sigla = 'R';
					strcpy(pMontadora->status.reserva.CNPJ, aux);
					alteraMontadora(pMontadora);
					alteraLoja(pLoja);
					break;
				}
			}
		}
		else
		{
			printf("\nEsse carro não está disponível!\n\n");
		}
	}
	system("pause");
}

//Altera uma struct Montadora no arquivo a partir do registro
void alteraMontadora(Montadora *pMontadora)
{
	FILE *fptr;
	Montadora *aux = NULL;
	int i, qtd = verificaMontadora();
	alocaMontadora(&aux, 1);
	if ((fptr = fopen("carro.bin", "rb+")) == NULL)
		printf("Erro ao abrir o arquivo");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(aux, sizeof(Montadora), 1, fptr);
			if (pMontadora->regcarro == aux->regcarro)
				break;
		}

		fseek(fptr, i * sizeof(Montadora), SEEK_SET);
		fwrite(pMontadora, sizeof(Montadora), 1, fptr);
		fclose(fptr);
	}
}

//Altera uma struct Loja no arquivo a partir do registro
void alteraLoja(Loja *pLoja)
{
	FILE *fptr;
	Loja *aux = NULL;
	int i, qtd = verificaLoja();
	alocaLoja(&aux, 1);
	if ((fptr = fopen("concessionaria.bin", "rb+")) == NULL)
		printf("Erro ao abrir o arquivo");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(aux, sizeof(Loja), 1, fptr);
			if (pLoja->regloja == aux->regloja)
				break;
		}

		fseek(fptr, i * sizeof(Loja), SEEK_SET);
		fwrite(pLoja, sizeof(Loja), 1, fptr);
		fclose(fptr);
	}
}

//Função resposável pela lógica de finalizar uma reserva
void finalizarReserva()
{
	system("cls");

	int reg, dia, mes, ano;
	char option;
	bool condition;
	printf("Digite o número de registro do carro: ");
	scanf("%i", &reg);

	Montadora *pMontadora = buscaMontadora(reg);
	if (pMontadora == NULL)
	{
		printf("\nCarro não encontrado\n\n");
		system("pause");
		return;
	}
	mostraMontadora(pMontadora);
	if (pMontadora->status.sigla == 'L')
	{
		printf("\nO carro está livre para reserva!\n\n");
		system("pause");
		return;
	}
	Loja *pLoja = buscaLojaPorCNPJ(pMontadora->status.reserva.CNPJ);
	fflush(stdin);
	printf("\nO que deseja fazer?");
	printf("\n1 - Comprar");
	printf("\n2 - Liberar\n\n");
	scanf("%c", &option);
	if (option == '1')
	{
		//O status E de 'Ereased' significa que a informação foi 'deletada', porém permanece no arquivo
		pMontadora->status.sigla = 'E';
		for (int i = 0; i < MAX_RESERVED; i++)
		{
			if ((*(pLoja->tabela + i)).sigla == 'R' && (*(pLoja->tabela + i)).reservado.regcarro == reg)
			{
				pLoja->reserved = pLoja->reserved - 1;
				(pLoja->tabela + i)->sigla = 'L';
				pLoja->sold = pLoja->sold + 1;
				break;
			}
		}
		Data *pData = NULL;
		do
		{
			fflush(stdin);
			printf("\nDigite a data da venda no formato dd/mm/yyyy: ");
			scanf("%i/%i/%i", &dia, &mes, &ano);
			alocaData(&pData, 1);
			pData->dia = dia;
			pData->mes = mes;
			pData->ano = ano;
			condition = validaData(pData);
			if (!condition)
				printf("Data inválida! Tente novamente!");
		} while (!condition);
		HistoricoVendas *pHistorico = gerarHistorico(pMontadora, pLoja, pData);
		gravaHistorico(pHistorico);
	}
	else if (option == '2')
	{
		for (int i = 0; i < MAX_RESERVED; i++)
		{
			if ((*(pLoja->tabela + i)).sigla == 'R' && (*(pLoja->tabela + i)).reservado.regcarro == reg)
			{
				pMontadora->status.sigla = 'L';
				pLoja->reserved = pLoja->reserved - 1;
				(pLoja->tabela + i)->sigla = 'L';
				break;
			}
		}
	}

	alteraLoja(pLoja);
	alteraMontadora(pMontadora);
	return;
}

//Gera o histórico de uma venda
HistoricoVendas* gerarHistorico(Montadora *pMontadora, Loja *pLoja, Data *pData)
{
	HistoricoVendas *pHistorico = NULL;
	alocaHistorico(&pHistorico, 1);

	pHistorico->reghist = verificaHistorico() + 1;
	pHistorico->regcarro = pMontadora->regcarro;
	strcpy(pHistorico->modelo, pMontadora->modelo);
	strcpy(pHistorico->cor, pMontadora->cor);
	pHistorico->valor = pMontadora->valor;
	pHistorico->regloja = pLoja->regloja;
	strcpy(pHistorico->nome, pLoja->nome);
	strcpy(pHistorico->cnpj, pLoja->CNPJ);
	pHistorico->dataVenda.ano = pData->ano;
	pHistorico->dataVenda.mes = pData->mes;
	pHistorico->dataVenda.dia = pData->dia;

	return pHistorico;
}

//Persiste uma struct HistoricoVendas no arquivo historico.bin
void gravaHistorico(HistoricoVendas *pHistorico)
{
	FILE *fptr;

	if ((fptr = fopen("historico.bin", "ab")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		fwrite(pHistorico, sizeof(HistoricoVendas), 1, fptr);
	}
	fclose(fptr);
}

//Aloca memória para struct HitoricoVendas
void alocaHistorico(HistoricoVendas** p, int tam)
{
	if ((*p = (HistoricoVendas*)realloc(*p, tam * sizeof(HistoricoVendas))) == NULL)
	{
		printf("Erro ao alocar memória");
		exit(1);
	}
}

//Aloca memória para a struct Data
void alocaData(Data **p, int tam)
{
	if ((*p = (Data *)realloc(*p, tam * sizeof(Data))) == NULL)
	{
		printf("Erro ao alocar memória");
		exit(1);
	}
}

//Verifica quantos elementos HistoricoVendas existem no arquivo
int verificaHistorico()
{
	FILE *fptr;
	long int pos;
	int qtd = 0;

	if ((fptr = fopen("historico.bin", "rb")) != NULL)
	{
		fseek(fptr, 0, SEEK_END);
		pos = ftell(fptr);
		qtd = pos / sizeof(HistoricoVendas);
		fclose(fptr);
	}
	return qtd;
}

//Exibe um histórico de duas maneiras dependendo da escolha do usuário
void mostraHistorico()
{
	system("cls");

	int qtd = verificaHistorico();
	char option, cnpj[19], modelo[20];
	if (qtd == 0)
	{
		printf("Nenhum registro encontrado");
		system("pause");
		return;
	}

	do
	{
		system("cls");
		printf("Qual o tipo de busca?");
		printf("\n1 - Por Concessionária");
		printf("\n2 - Por Modelo");
		printf("\n3 - Total\n\n");
		option = getch();
	} while (option != '1' && option != '2' && option != '3');

	switch (option)
	{
	case '1':
		printf("Digite o CNPJ da concessionária: ");
		gets(cnpj);
		break;
	case '2':
		printf("Digite o modelo do carro: ");
		gets(modelo);
		maiuscula(modelo);
		break;
	}

	printf("\n\n|%-8s|%-8s|%-19s|%-20s|%-8s|%-20s|%-10s|%-10s|%-10s|\n", "RegHist", "RegLoja", "CNPJ", "Nome", "RegCar", "Modelo", "Cor", "Valor", "Data");
	for (int i = 0; i <= qtd; i++)
	{
		HistoricoVendas* pHistorico = buscaHistorico(i);
		if (pHistorico != NULL)
		{
			if ((option == '1' && strcmp(cnpj, pHistorico->cnpj) == 0) || (option == '2' && strcmp(modelo, pHistorico->modelo) == 0) || option == '3')
				printf("|%-8i|%-8i|%-19s|%-20s|%-8i|%-20s|%-10s|%-10.2f|%02i/%02i/%04i|\n", pHistorico->reghist, pHistorico->regloja, pHistorico->cnpj, pHistorico->nome, pHistorico->regcarro, pHistorico->modelo, pHistorico->cor, pHistorico->valor, pHistorico->dataVenda.dia, pHistorico->dataVenda.mes, pHistorico->dataVenda.ano);
		}
	}

	printf("\n\n");
	system("pause");
}

//Resgata uma struct HistoricoVendas do arquivo a partir do registro
HistoricoVendas *buscaHistorico(int reg)
{
	FILE *fptr;
	HistoricoVendas *pHistorico = NULL;
	int i, qtd = verificaHistorico();
	alocaHistorico(&pHistorico, 1);

	if ((fptr = fopen("historico.bin", "rb")) == NULL)
		printf("Erro ao abrir o arquivo!");
	else
	{
		for (i = 0; i < qtd; i++)
		{
			fread(pHistorico, sizeof(HistoricoVendas), 1, fptr);
			if (pHistorico->reghist == reg)
				return pHistorico;
		}
	}

	return NULL;
}

//Edita o Nome e/ou CNPJ de uma Loja
void editaLoja()
{
	system("cls");
	char aux[19], nome[30], option;
	printf("Digite o CNPJ da Loja: ");
	gets(aux);

	Loja *pLoja = buscaLojaPorCNPJ(aux);
	if (pLoja == NULL)
	{
		printf("\nConcessionária não encontrada!");
		system("pause");
		return;
	}
	printf("\n\n");

	do
	{
		system("cls");
		mostraLoja(pLoja);
		printf("\n\n");
		printf("Qual(is) campos deseja alterar?");
		printf("\n1 - Nome");
		printf("\n2 - CNPJ");
		printf("\n3 - Ambos");
		printf("\n4 - Cancelar\n\n");
		option = getch();
	} while (option != '1' && option != '2' && option != '3' && option != '4');

	switch (option)
	{
	case '1':
		printf("\nDigite o novo nome: ");
		gets(nome);
		maiuscula(nome);
		strcpy(pLoja->nome, nome);
		alteraLoja(pLoja);
		printf("\n\nAlterado com sucesso!\n");
		break;
	case '2':
		printf("\nDigite o novo CNPJ: ");
		gets(aux);
		strcpy(pLoja->CNPJ, aux);
		alteraLoja(pLoja);
		printf("\n\nAlterado com sucesso!\n");
		break;
	case '3':
		printf("\nDigite o novo nome: ");
		gets(nome);
		maiuscula(nome);
		strcpy(pLoja->nome, nome);
		fflush(stdin);
		printf("\nDigite o novo CNPJ: ");
		gets(aux);
		strcpy(pLoja->CNPJ, aux);
		alteraLoja(pLoja);
		printf("\n\nAlterado com sucesso!\n");
		break;
	default:
		break;
	}

	system("pause");
}

//Edita o Modelo, Cor ou Valor de um carro
void editaMontadora()
{
	system("cls");
	int reg;
	char modelo[20], cor[10], option;
	float valor;
	printf("Digite o registro do carro: ");
	scanf("%i", &reg);

	Montadora *pMontadora = buscaMontadora(reg);
	if (pMontadora == NULL)
	{
		printf("\nCarro não encontrado!");
		system("pause");
		return;
	}
	printf("\n\n");

	do
	{
		system("cls");
		mostraMontadora(pMontadora);
		printf("\n\n");
		printf("Qual campos deseja alterar?");
		printf("\n1 - Modelo");
		printf("\n2 - Cor");
		printf("\n3 - Valor");
		printf("\n4 - Cancelar\n\n");
		option = getch();
	} while (option != '1' && option != '2' && option != '3' && option != '4');

	fflush(stdin);
	switch (option)
	{
	case '1':
		printf("\nDigite o novo modelo: ");
		gets(modelo);
		maiuscula(modelo);
		strcpy(pMontadora->modelo, modelo);
		alteraMontadora(pMontadora);
		printf("\n\nAlterado com sucesso!\n");
		break;
	case '2':
		printf("\nDigite a nova cor: ");
		gets(cor);
		maiuscula(cor);
		strcpy(pMontadora->cor, cor);
		alteraMontadora(pMontadora);
		printf("\n\nAlterado com sucesso!\n");
		break;
	case '3':
		printf("\nDigite o novo valor: ");
		scanf("%f", &valor);
		pMontadora->valor = valor;
		alteraMontadora(pMontadora);
		printf("\n\nAlterado com sucesso!\n");
		break;
	default:
		break;
	}

	system("pause");
}

//Validação da data digitada
bool validaData(Data *data)
{
	if (data->dia < 1 || data->dia >(31 - (((data->mes - 1) % 7) % 2)) || data->mes < 1 || data->mes > 12)
	{
		return false;
	}//Verificação de dia/mes/ano negativo

	if (data->mes == 2)
	{
		if (data->ano % 4 == 0 && data->dia > 29)
		{
			return false;
		}
		if (data->ano % 4 != 0 && data->dia > 28)
		{
			return false;
		}
	}
	return true;
}
