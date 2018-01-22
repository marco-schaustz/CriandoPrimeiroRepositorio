#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#define SIZE 100 /*Quantidade de clientes/produtos/pedidos*/
#define Clear system("cls") /*Auxílio de portabilidade*/

/*Funções ----------------------------------------------*/
void clientes(void);
void cadastroClient(char arq[], int size);
void capturaClient(char arq[], int size);
void consultaClient(char arq[], int size);
void imprimiClient(char arq[], int size);
void produtos(void);
void cadastroProduct(char arq[], int size);
void capturaProduct(char arq[], int size);
void consultaProduct(char arq[], int size);
void imprimiProduct(char arq[], int size);
void pedidos(void);
void cadastroRequest(char arq[], int size);
void capturaRequest (void);
void consultaRequest(char arq[], int size);
void imprimiRequest(char arq[], int size);
//-------------------------------------------------------

typedef struct{
	int cod; /*Chave do registro*/
	char cpf[15];
	char nome[50];
	char nasc[9];
	char tel[16];
	char end[60];
	char data[9];
}Client;

typedef struct{
	int cod;
	char nome[50];
	int qtd;
	float pcompra;
	float pvenda;
	char produtora[50];
	char data[9]; /*Data da atualização*/
}Product;

typedef struct{
	int cod;
	int productcod;
	char status[14];
	char nome[50];
	int qtd;
	float pcompra;
	char produtora[50];
	char tel[16];
	char entrega[9];
	char data[9]; /*Data da atualização*/
}Request;

/*Variáveis globais -----------------------------------------------------------------*/
char txt[14]; /*Varíavel para armazenar destinos de arquivos*/
int cod; /*Código identificador*/
char dataTemp[9]; /*char para troca da data americana pela data padrão brasileiro*/
int i; /*Contador do for de troca dos padrões de data*/
//------------------------------------------------------------------------------------

/*Declaração de variáveis globais struct --------------------------------------------*/
Client cliente = {0, "", "", "", "", "", ""};
Product produto = {0, "", 0, 0, 0, "", ""};
Request pedido = {0, 0, "", "", 0, 0, "", "", "", ""};
//------------------------------------------------------------------------------------

/*Ponteiros de arquivo globais-------------------------------------------------------*/
FILE *cPtr = 0;
FILE *pPtr = 0;
FILE *rPtr = 0;
//------------------------------------------------------------------------------------

main(){
	setlocale(LC_ALL, "");
	
	char opcao = '0';
	
	do{
		Clear;

		printf("Selecione o módulo desejado: (Digite o número correspondente)\n"
				"1 - Clientes\n"
				"2 - Produtos (Filmes)\n"
				"3 - Pedidos\n"
				"0 - Sair do programa\n");
		fflush(stdin);
		scanf("%c", &opcao);
		
		switch(opcao){
			case '1':
				clientes();
				break;
			case '2':
				produtos();
				break;
			case '3':
				pedidos();
				break;
			case '0':
				printf("\nEncerrando operações.\n");
				break;
			default:
				printf("\nOpção Inválida.\n");
				fflush(stdin);
				scanf("%*c");
				break;
		} /*Fim do switch*/
	} /*Fim do Do*/
	
	while(opcao != '0');
	
	fflush(stdin);
	scanf("%*c");
}

/*----------------------------CLIENTES---------------------------------*/

void clientes (void){
	Clear;
	
	strcpy( txt, "client.txt"); /*colocando o arquivo destino na variável*/
	
	char escolha = '0';
	
	do{
		printf("Selecione a ação desejada: (Digite o número correspondente)\n"
				"1 - Cadastrar/Atualizar Cliente.\n"
				"2 - Consultar Cliente.\n"
				"3 - Lista de Clientes.\n"
				"0 - Voltar ao menu principal.\n");
		fflush(stdin);
		scanf("%c", &escolha);
	
		switch(escolha){
			case '1':
				cadastroClient(txt, SIZE);
				break;
			case '2':
				consultaClient(txt, SIZE);
				break;
			case '3':
				imprimiClient(txt, SIZE);
				break;
			case '0':
				printf("\nEncerrando operações.\n");
				break;
			default:
				printf("\nOpção Inválida.\n");
				fflush(stdin);
				scanf("%*c");
				break;
		}/*Fim do switch*/
	}/*Fim do Do*/
	while (escolha != '0');
}

void cadastroClient(char arq[], int size){
	Clear;
	
	cPtr = 0; /*Zerando ponteiro de arquivo*/
	char j = '0'; /*Variável de escolha de atualização*/
	
	/*Abre arquivo em modo r+ (leitura e escrita), se não existir o arquivo, cria em modo w -------------------------------------------*/
	if ((cPtr = fopen (arq, "r+")) == NULL){
		if ((cPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de clientes em branco caso aberto em modo w*/
			if ( fwrite(&cliente, sizeof(Client), 1, cPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(cPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} cPtr = 0;
		
		if ((cPtr = fopen (arq, "r+")) == NULL){ /*Finalmente abre em modo r+*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //-----------------------------------------------------------------------------------------------------------
	
	printf("Digite o código de identificação do cliente: (1 a %d) ", size);
	fflush(stdin);
	scanf("%d", &cod); if (cod > size){ printf("\nCódigo inválido (Somente de 1 a %d).\n", size); exit(1); } /*Capturando e verificando o código identificador*/
	
	/*Lendo os dados no arquivo e armazenando na variável cliente --------------------------------------------------------------------*/
	if (fseek( cPtr, (cod - 1) * sizeof (Client), SEEK_SET) != 0){ /*Posicionando ponteiro*/
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&cliente, sizeof(Client), 1, cPtr) != 1){ /*Escrevendo no arquivo*/
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} //-------------------------------------------------------------------------------------------------------------------------------
					
	if (cliente.cod != 0){ /*Mostra o cliente se não nulo ----------------------------------------------------------------------------*/
		printf("%d\nCPF: %s\nNome: %s\tNasc: %s\tTel: %s\nEndereço: %s\tData de atualização: %s\n\n", cliente.cod, cliente.cpf, cliente.nome, cliente.nasc, cliente.tel, cliente.end, cliente.data);
		
		printf("\nAtualizar:\n"
				"1 - Sim\n"
				"0 - Não\n");
		fflush(stdin);
		scanf("%c", &j);
		
		if ( j == '1' ){ /*Capturando os dados na variável cliente da struct Client*/
			capturaClient(arq, size);
			printf("\nCliente atualizado com sucesso!\n\n");
		}/*Fim do if interno - j == 1, Opção Atualizar*/
		
		else /*Opção - Não atualizar*/
			Clear;
	}/*Fim do if externo - cliente.cod != 0*/ //---------------------------------------------------------------------------------------
	
	else{ /*cliente.cod == 0 - cliente não cadastrado -------------------------------------------------------------------------------*/
		printf("\nCliente não cadastrado.\n");
		capturaClient(arq, size);
		printf("\nCliente cadastrado com sucesso!\n\n");
	}/*Fim do else*/ //---------------------------------------------------------------------------------------------------------------
	
	if ( fclose(cPtr) != 0 ){ /*Fechando o arquivo ---------------------------------------------*/
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} cPtr = 0; //-------------------------------------------------------------------------------
}/*Fim da função*/

void capturaClient(char arq[], int size){
	cPtr = 0;
	
	/*Abre arquivo client para atualização/cadastro em modo r+ (leitura e escrita) -------------------------*/
	if ((cPtr = fopen (arq, "r+")) == NULL){
		printf("\nImpossível abrir arquivo %s\n", arq);
		exit(1);
	} //-------------------------------------------------------------------------------------------------------
	
	cliente.cod = cod;
	
	printf("\nDigite o CPF(apenas números): ");
	fflush(stdin);
	for (i = 0; i < 14; i++){ /*Colocar caracteres especiais automaticamente  */
		if (i == 3 || i == 7){
			cliente.cpf[i] = '.';
			printf(".");
		}
		else if (i == 11){
			cliente.cpf[i] = '/';
			printf("/");
		}
		else{
			cliente.cpf[i] = getch();
			printf("%c", cliente.cpf[i]);
		}
	}/*Fim do for*/
	printf("\n");
	
	printf("\nDigite o nome completo: ");
	fflush(stdin);
	gets(cliente.nome);
			
	printf("\nDigite a data de nascimento (exemplo: 10/10/86): ");
	fflush(stdin);
	for (i = 0; i < 8; i++){ /*Colocar caracteres especiais automaticamente  */
		if (i == 2 || i == 5){
			cliente.nasc[i] = '/';
			printf("/");
		}
		else{
			cliente.nasc[i] = getch();
			printf("%c", cliente.nasc[i]);
		}
	}/*Fim do for*/
	printf("\n");
		
	printf("\nDigite o número de telefone para contato: ");
	fflush(stdin);
	for (i = 0; i < 15; i++){ /*Colocar caracteres especiais automaticamente  */
		if (i == 0){
			cliente.tel[i] = '(';
			printf("(");
		}
		else if (i == 3){
			cliente.tel[i] = ')';
			printf(")");
		}
		else if (i == 4){
			cliente.tel[i] = ' ';
			printf(" ");
		}
		else if (i == 10){
			cliente.tel[i] = '-';
			printf("-");
		}
		else{
			cliente.tel[i] = getch();
			printf("%c", cliente.tel[i]);
		}
	}/*Fim do for*/ 
	printf("\n");
		
	printf("\nDigite o endereço seguido do número e complemento: ");
	fflush(stdin);
	gets(cliente.end);
	
	_strdate (cliente.data); /*Grava a data do sistema na variável*/
	/*Trocando a data do sistema para o padrão brasileiro*/
	for (i = 0; i <= 7; i++){
		if (i <= 2)
			dataTemp[i+3] = cliente.data[i];
		if (i >= 3 && i <= 5)
			dataTemp[i-3] = cliente.data[i];
		if (i > 5)
			dataTemp[i] = cliente.data[i];
	} strcpy(cliente.data, dataTemp);
	
	/*Gravando os dados da variável cliente no arquivo --------------------------------------------*/
	if (fseek( cPtr, (cliente.cod - 1) * sizeof (Client), SEEK_SET) != 0){
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fwrite( &cliente, sizeof (Client), 1, cPtr) != 1 ){
		printf("\nErro no arquivo.\n");
		exit(1);
	} //---------------------------------------------------------------------------------------------
	
} /*Fim da função*/

void consultaClient(char arq[], int size){
	Clear;
	
	cPtr = 0; /*Zerando ponteiro de arquivo*/
	
	/*Abre arquivo em modo r (leitura e escrita), se não existir o arquivo, cria em modo w ------------------------------------------*/
	if ((cPtr = fopen (arq, "r")) == NULL){
		if ((cPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de clientes em branco caso aberto em modo w*/
			if ( fwrite(&cliente, sizeof(Client), 1, cPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		if ( fclose(cPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} cPtr = 0;
		
		if ((cPtr = fopen (arq, "r")) == NULL){ /*Finalmente abre em modo r*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //----------------------------------------------------------------------------------------------------------
	
	printf("Digite o código de identificação do cliente: (1 a %d) ", size);
	fflush(stdin);
	scanf("%d", &cliente.cod); if (cliente.cod > size){ printf("\nCódigo inválido (Somente de 1 a %d).\n", size); exit(1); }
	printf("\n");
	
	/*Lendo os dados do arquivo e armazenando na variável cliente -----------------------------------------------------------------------------------*/
	if (fseek( cPtr, (cliente.cod - 1) * sizeof (Client), SEEK_SET) != 0){ /*Busca a posição no arquivo para o registro especificado pelo usuário*/
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&cliente, sizeof(Client), 1, cPtr) != 1){ /*Lê em cliente registros correpondentes a struct Client*/
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} //-----------------------------------------------------------------------------------------------------------------------------------------------
	
	/*Mostra o cliente se não nulo ------------------------------------------------------------------------------------------------------------------*/
	if (cliente.cod != 0)
		printf("%d\nCPF: %s\nNome: %s\tNasc: %s\tTel: %s\nEndereço: %s\tData de atualização: %s\n\n", cliente.cod, cliente.cpf, cliente.nome, cliente.nasc, cliente.tel, cliente.end, cliente.data);
	else
		printf("Cliente não cadastrado.\n\n");
	//------------------------------------------------------------------------------------------------------------------------------------------------
		
	if ( fclose(cPtr) != 0 ){ /*Fechando arquivo ---------------------------------------------------------------------------------------------------*/
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} cPtr = 0; //-----------------------------------------------------------------------------------------------------------------------------------
}

void imprimiClient(char arq[], int size){
	Clear;
	
	cPtr = 0; /*Zerando ponteiro de arquivo*/
	int c = 0; /*Contador da impressão*/
	
	/*Abre arquivo em modo r (leitura e escrita), se não existir o arquivo, cria em modo w --------------------------------------------*/
	if ((cPtr = fopen (arq, "r")) == NULL){
		if ((cPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de clientes em branco caso aberto em modo w*/
			if ( fwrite(&cliente, sizeof(Client), 1, cPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(cPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} cPtr = 0;
		
		if ((cPtr = fopen (arq, "r")) == NULL){ /*Finalmente abre em modo r*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //-------------------------------------------------------------------------------------------------------------
	
	rewind(cPtr); /*Reposiciona o ponteiro no início do arquivo*/
					
	while (!feof(cPtr)) { /*Lê todos os registros do arquivo --------------------------------------------------------------------------------*/
		if ( fread(&cliente, sizeof(Client), 1, cPtr) != 1){
			continue;/*Caso não leia corretamente, sai da função while e não lê por exemplo o último cadastro 2x*/
		}
		if (cliente.cod != 0){ /*Mostra as contas registradas (não nulas)*/
			printf("%d\nCPF: %s\nNome: %s\tNasc: %s\tTel: %s\nEndereço: %s\tData de atualização: %s\n\n", cliente.cod, cliente.cpf, cliente.nome, cliente.nasc, cliente.tel, cliente.end, cliente.data);
			c++; /*Contador de quantidade de clientes cadastros*/
		}
	} /*Fim do while*/ //---------------------------------------------------------------------------------------------------------------------
	
	if (c == 0){ /*Contador de clientes cadastrados zerado - Nenhum cliente cadastrado*/
		Clear;
		printf("Nenhum cliente cadastrado.\n");
		fflush(stdin);
		scanf("%*c");
	}
} /*Fim da função*/

/*----------------------------PRODUTOS---------------------------------*/

void produtos(void){
	Clear;
	
	strcpy( txt, "product.txt"); /*colocando o arquivo destino na variável*/
	
	char escolha = '0';
	
	do{
		printf("Selecione a ação desejada: (Digite o número correspondente)\n"
				"1 - Cadastrar/Atualizar Produto.\n"
				"2 - Consultar Produto.\n"
				"3 - Lista de Produtos.\n"
				"0 - Voltar ao menu principal.\n");
		fflush(stdin);
		scanf("%c", &escolha);
	
		switch(escolha){
			case '1':
				cadastroProduct(txt, SIZE);
				break;
			case '2':
				consultaProduct(txt, SIZE);
				break;
			case '3':
				imprimiProduct(txt, SIZE);
				break;
			case '0':
				printf("\nEncerrando operações.\n");
				break;
			default:
				printf("\nOpção Inválida.\n");
				fflush(stdin);
				scanf("%*c");
				break;
		}/*Fim do switch*/
	}/*Fim do Do*/
	while (escolha != '0');
}

void cadastroProduct(char arq[], int size){
	Clear;
	
	pPtr = 0; /*Zerando ponteiro de arquivo*/
	char j = '0'; /*variável de escolha switch*/
	
	/*Abre arquivo em modo r+ (leitura e escrita), se não existir o arquivo, cria em modo w ----------------------------------------------*/
	if ((pPtr = fopen (arq, "r+")) == NULL){
		if ((pPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de produtos em branco caso aberto em modo w*/
			if ( fwrite(&produto, sizeof(Product), 1, pPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(pPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} pPtr = 0;
		
		if ((pPtr = fopen (arq, "r+")) == NULL){ /*Finalmente abre em modo r+*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //--------------------------------------------------------------------------------------------------------------
	
	printf("Digite o código de identificação do produto: (1 a %d) ", size);
	fflush(stdin);
	scanf("%d", &cod); if (cod > size){ printf("\nCódigo inválido (Somente de 1 a %d).\n", size); exit(1); } /*Capturando e verificando o código identificador*/
	
	/*Lendo os dados do arquivo na variável produto -----------------------------------------------------------------------*/
	if (fseek( pPtr, (cod - 1) * sizeof (Product), SEEK_SET) != 0){ /*Posicionando ponteiro*/
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&produto, sizeof(Product), 1, pPtr) != 1){ /*Escrevendo no arquivo*/
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} //--------------------------------------------------------------------------------------------------------------------
	
	if ( fclose(pPtr) != 0 ){ /*Fechando arquivo --------------------------------------------------------*/
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} pPtr = 0; //----------------------------------------------------------------------------------------
	
	if (produto.cod != 0){ /*Mostra o produto se não nulo*/
		printf("%d\nNome: %s\tQuantidade: %d\nPreço de Compra: R$ %.2f\tValor total da Venda: R$ %.2f (R$ %.2f cada)\nLucro Total Estimado: R$ %.2f\nProdutora: %s\tData de atualização: %s\n\n", produto.cod, produto.nome, produto.qtd, produto.pcompra, (produto.pvenda * (float) produto.qtd), produto.pvenda, ( (float) produto.qtd * (produto.pvenda - produto.pcompra) ), produto.produtora, produto.data);
		
		printf("\nAtualizar:\n"
				"1 - Sim\n"
				"0 - Não\n");
		fflush(stdin);
		scanf("%c", &j);
		
		if ( j == '1' ){ /*Atualiza os dados e sobrescreve*/
			capturaProduct(arq, size);
			printf("\nProduto atualizado com sucesso!\n\n");
		}/*Fim do if interno*/
		
		else /*j == '0' - Não atualizar*/
			Clear;
			
	}/*Fim do if externo (produto.cod != 0)*/
	
	else{ /*produto.cod == 0 - Nenhum produto cadastrado*/
		printf("\nProduto não cadastrado.\n");
		
		capturaProduct(arq, size);
		
		printf("\nProduto cadastrado com sucesso!\n\n");
	}/*Fim do else*/
}/*Fim da função*/

void capturaProduct (char arq[], int size){
	
	/*Abre arquivo Produtos para atualização/cadastro em modo r+ (leitura e escrita) -------------------------*/
	if ((pPtr = fopen (arq, "r+")) == NULL){
		printf("\nImpossível abrir arquivo %s\n", arq);
		exit(1);
	} //-------------------------------------------------------------------------------------------------------
	
	produto.cod = cod;
	
	printf("\nDigite o nome do produto: ");
	fflush(stdin);
	gets(produto.nome);
			
	printf("\nDigite a quantidade: ");
	fflush(stdin);
	scanf("%d", &produto.qtd);
	
	printf("\nDigite o preço estimado de compra: ");
	fflush(stdin);
	scanf("%f", &produto.pcompra);
	
	printf("\nDigite o preço estimado de venda: ");
	fflush(stdin);
	scanf("%f", &produto.pvenda);
	
	printf("\nDigite o nome da Produtora: ");
	fflush(stdin);
	gets(produto.produtora);
	
	_strdate (produto.data); /*Grava a data do sistema na variável*/
	/*Trocando a data do sistema para o padrão brasileiro*/
	for (i = 0; i <= 7; i++){
		if (i <= 2)
			dataTemp[i+3] = produto.data[i];
		if (i >= 3 && i <= 5)
			dataTemp[i-3] = produto.data[i];
		if (i > 5)
			dataTemp[i] = produto.data[i];
	} strcpy(produto.data, dataTemp);
	
	/*Gravando variável produto no arquivo --------------------------------------------------------------*/
	if (fseek( pPtr, (produto.cod - 1) * sizeof (Product), SEEK_SET) != 0){
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fwrite( &produto, sizeof (Product), 1, pPtr) != 1 ){
		printf("\nErro no arquivo.\n");
		exit(1);
	} //--------------------------------------------------------------------------------------------------
	
	if ( fclose(pPtr) != 0 ){ /*Fechando arquivo --------------------------------------------------------*/
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} pPtr = 0; //----------------------------------------------------------------------------------------
}/*Fim da função*/

void consultaProduct(char arq[], int size){
	Clear;
	
	pPtr = 0;
	
	/*Abre arquivo em modo r (leitura e escrita), se não existir o arquivo, cria em modo w -----------------------------------*/
	if ((pPtr = fopen (arq, "r")) == NULL){
		if ((pPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de produtos em branco caso aberto em modo w*/
			if ( fwrite(&produto, sizeof(Product), 1, pPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(pPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} pPtr = 0;
		
		if ((pPtr = fopen (arq, "r")) == NULL){ /*Finalmente abre em modo r*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //---------------------------------------------------------------------------------------------------
	
	printf("Digite o código de identificação do produto: (1 a %d) ", size);
	fflush(stdin);
	scanf("%d", &produto.cod); if (produto.cod > size){ printf("\nCódigo inválido (Somente de 1 a %d).\n", size); exit(1); }
	printf("\n");
	
	/*Lendo o arquivo e gravando os dados na variável produto ---------------------------------------------------------------------------------------*/
	if (fseek( pPtr, (produto.cod - 1) * sizeof (Product), SEEK_SET) != 0){ /*Busca a posição no arquivo para o registro especificado pelo usuário*/
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&produto, sizeof(Product), 1, pPtr) != 1){ /*Lê em produto registros correpondentes a struct Product*/
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} //---------------------------------------------------------------------------------------------------------------------------------------------
	
	/*Mostra o produto se não nulo -----------------------------------------------------------------------------------------------------------------*/
	if (produto.cod != 0)
		printf("%d\nNome: %s\tQuantidade: %d\nPreço de Compra: R$ %.2f\tValor total da Venda: R$ %.2f (R$ %.2f cada)\nLucro Total Estimado: R$ %.2f\nProdutora: %s\tData de atualização: %s\n\n", produto.cod, produto.nome, produto.qtd, produto.pcompra, (produto.pvenda * (float) produto.qtd), produto.pvenda, ( (float) produto.qtd * (produto.pvenda - produto.pcompra) ), produto.produtora, produto.data);
	else
		printf("Produto não cadastrado.\n\n");
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	
	/*Fechando arquivo -----------------------------------------------------------------------------------------------------------------------------*/
	if ( fclose(pPtr) != 0 ){
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} pPtr = 0;
	//-----------------------------------------------------------------------------------------------------------------------------------------------
} /*Fim da função*/

void imprimiProduct(char arq[], int size){
	Clear;
	
	pPtr = 0;
	int p = 0; /*Contador da impressão*/
	
	/*Abre arquivo em modo r (leitura e escrita), se não existir o arquivo, cria em modo w ----------------------------------*/
	if ((pPtr = fopen (arq, "r")) == NULL){
		if ((pPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de produtos em branco caso aberto em modo w*/
			if ( fwrite(&produto, sizeof(Product), 1, pPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(pPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} pPtr = 0;
		
		if ((pPtr = fopen (arq, "r")) == NULL){ /*Finalmente abre em modo r*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //----------------------------------------------------------------------------------------------------
	
	/* ----- !feof estava dando erro com o fread por um motivo que não descobri ainda, então usei uma forma mais manual de apresentação ------ */
	for (i = 0; i < size; i++){
		
		/*Lendo o arquivo e gravando os dados na variável prduto ---------------------------------------------------------------------------------------*/
		if (fseek( pPtr, i * sizeof (Product), SEEK_SET) != 0){ /*Busca a posição no arquivo para o registro especificado pelo usuário*/
			printf("\nErro inesperado.\n");
			exit(1);
		}
		if ( fread(&produto, sizeof(Product), 1, pPtr) != 1){ /*Lê em produto registros correpondentes a struct Product*/
			printf("\nErro na gravação do arquivo.\n");
			exit(1);
		} //---------------------------------------------------------------------------------------------------------------------------------------------
	
		if (produto.cod != 0){ /*Mostra as contas registradas (não nulas)*/
			printf("%d\nNome: %s\tQuantidade: %d\nPreço de Compra: R$ %.2f\tValor total da Venda: R$ %.2f (R$ %.2f cada)\nLucro Total Estimado: R$ %.2f\nProdutora: %s\tData de atualização: %s\n\n", produto.cod, produto.nome, produto.qtd, produto.pcompra, (produto.pvenda * (float) produto.qtd), produto.pvenda, ( (float) produto.qtd * (produto.pvenda - produto.pcompra) ), produto.produtora, produto.data);
			p++; /*Contador de quantidade de produtos existentes*/
		}
	}/*Fim do for*/
		
	if (p == 0){ /*Contador zerado - Nenhum produto com codigo diferente de 0 (nenhum produto cadastrado) ------------------------------*/
		Clear;
		printf("Nenhum produto cadastrado.\n");
		fflush(stdin);
		scanf("%*c");
	} //---------------------------------------------------------------------------------------------------------------------------------
	
	/*Fechando arquivo -----------------------------------------------------------------------------------------------------------------*/
	if ( fclose(pPtr) != 0 ){
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} pPtr = 0;
	//-----------------------------------------------------------------------------------------------------------------------------------
} /*Fim da função*/

/*----------------------------PEDIDOS---------------------------------*/

void pedidos(void){
	Clear;
	
	strcpy( txt, "request.txt"); /*colocando o arquivo destino na variável*/
	
	char escolha = '0';
	
	do{
		printf("Selecione a ação desejada: (Digite o número correspondente)\n"
				"1 - Cadastrar/Atualizar Pedido.\n"
				"2 - Consultar Pedido.\n"
				"3 - Lista de Pedidos.\n"
				"0 - Voltar ao menu principal.\n");
		fflush(stdin);
		scanf("%c", &escolha);
	
		switch(escolha){
			case '1':
				cadastroRequest(txt, SIZE);
				break;
			case '2':
				consultaRequest(txt, SIZE);
				break;
			case '3':
				imprimiRequest(txt, SIZE);
				break;
			case '0':
				printf("\nEncerrando operações.\n");
				break;
			default:
				printf("\nOpção Inválida.\n");
				fflush(stdin);
				scanf("%*c");
				break;
		}/*Fim do switch*/
	}/*Fim do Do*/
	while (escolha != '0');
}

void cadastroRequest(char arq[], int size){
	Clear;
	
	rPtr = 0; /*Zerando Ponteiros*/
	pPtr = 0; /*-----------------*/
	int i; /*Contador do for de escrita em modo w*/
	char j = '0'; /*Escolha de atualização*/
	int status = 0; /*Escolha de status*/
	
	/*Abre arquivo em modo r+ (leitura e escrita), se não existir o arquivo, cria em modo w*/
	if ((rPtr = fopen (arq, "r+")) == NULL){
		if ((rPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de pedidos em branco caso aberto em modo w*/
			if ( fwrite(&pedido, sizeof(Request), 1, rPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(rPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} rPtr = 0;
		
		if ((rPtr = fopen (arq, "r+")) == NULL){ /*Finalmente abre em modo r+*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/
	
	printf("Digite o código de identificação do pedido: (1 a %d) ", size);
	fflush(stdin);
	scanf("%d", &cod); if (cod > size){ printf("\nCódigo inválido (Somente de 1 a %d).\n", size); exit(1); } /*Capturando e verificando o código identificador*/
	
	/*Lendo os dados na variável pedido -----------------------------------------------------------------------*/
	if (fseek( rPtr, (cod - 1) * sizeof (Request), SEEK_SET) != 0){ /*Posicionando ponteiro*/
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&pedido, sizeof(Request), 1, rPtr) != 1){ /*Escrevendo no arquivo*/
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} //---------------------------------------------------------------------------------------------------------
	
	/*Verificando se o pedido existe --------------------------------------------------------------------------*/				
	if (pedido.cod != 0){
		printf("%d\nStatus: %s\nCódigo do produto: %d\nNome: %s\tQuantidade: %d\tEntrega: %s\nPreço total de Compra: R$ %.2f (R$ %.2f cada)\nProdutora: %s\tTelefone: %s\nData de atualização: %s\n\n", pedido.cod, pedido.status, pedido.productcod, pedido.nome, pedido.qtd, pedido.entrega, (pedido.pcompra * (float) pedido.qtd), pedido.pcompra, pedido.produtora, pedido.tel, pedido.data);
		
		printf("\nAtualizar status ou pedido inteiro? (Digite o número correspondente a opção desejada.\n"
				"1 - Status\n"
				"2 - Pedido inteiro\n"
				"0 - Não atualizar\n");
		fflush(stdin);
		scanf("%c", &j);
		
		if ( j == '1' ){ /*Atualizar status*/
			printf("\nDigite o número correspondente ao status:\n"
					"1 - Em andamento\n"
					"2 - Entregue\n"
					"3 - Cancelado\n"
					"4 - Devolução\n");
			fflush(stdin);
			scanf("%c", &status);
			
			/*Abre arquivo Produtos para atualização em modo r+ (leitura e escrita)*/
			if ((pPtr = fopen ("product.txt", "r+")) == NULL){
				printf("\nImpossível abrir arquivo %s\n", "product.txt");
				exit(1);
			}
			
			switch (status){
				case '1':
					strcpy (pedido.status, "Em andamento");
					break;
				case '2':
					strcpy (pedido.status, "Entregue");
					
					/*Lendo product.txt e armazenando em produto e adicionando a quantidade ---------------------------------------*/
					if (fseek( pPtr, (pedido.productcod - 1) * sizeof (Product), SEEK_SET) != 0){ /*Posicionando ponteiro*/
						printf("\nErro inesperado.\n");
						exit(1);
					}
					if ( fread(&produto, sizeof(Product), 1, pPtr) != 1){ /*Escrevendo no arquivo*/
						printf("\nErro na gravação do arquivo.\n");
						exit(1);
					}
			
					if (produto.cod != pedido.productcod){ /*Verificando se os códigos batem*/
						printf("Erro 004: Contate o suporte");
						exit(1);
					}
					
					produto.qtd += pedido.qtd; /*Atualizando a quantidade do produto na struct produto*/
					//-----------------------------------------------------------------------------------------------------------------
					
					/*Gravando produto em product.txt com a qtd atualizada --------------------------------------------------------*/
					if (fseek( pPtr, (pedido.productcod - 1) * sizeof (Product), SEEK_SET) != 0){
						printf("\nErro inesperado.\n");
						exit(1);
					}
					if (fwrite( &produto, sizeof (Product), 1, pPtr) != 1){ /*Sobrescrevendo o produto com qtd atualizada*/
						printf("\nErro no arquivo.\n");
						exit(1);
					}
					printf("Quantidade do produto atualizada com sucesso!\n");
					//-------------------------------------------------------------------------------------------------------------------
					break;
					
				case '3':
					strcpy (pedido.status, "Cancelado");
					break;
					
				case '4':
					/*Se o status anterior era Entregue, necessário remover a qtd de product*/
					if ( strcmp(pedido.status, "Entregue") == 0){ /*Comparando as duas strings*/
						/*Retirar qtd do pedido em produto -----------------------------------------------------------------------------*/
						if (fseek( pPtr, (pedido.productcod - 1) * sizeof (Product), SEEK_SET) != 0){
							printf("\nErro inesperado.\n");
							exit(1);
						}
						if ( fread(&produto, sizeof(Product), 1, pPtr) != 1){
							printf("\nErro na gravação do arquivo.\n");
							exit(1);
						}
					
						if (produto.cod != pedido.productcod){ /*Verificando se os códigos batem*/
							printf("\nProduto não cadastrado, cadastre-o primeiro.\n");
							exit(1);
						}
						
						produto.qtd -= pedido.qtd; /*Atualizando a quantidade do produto na struct produto*/
						//----------------------------------------------------------------------------------------------------------------
					
						/*Gravando produto em product com a qtd atualizada ---------------------------------------------------------*/
						if (fseek( pPtr, (produto.cod - 1) * sizeof (Product), SEEK_SET) != 0){
							printf("\nErro inesperado.\n");
							exit(1);
						}
						if (fwrite( &produto, sizeof (Product), 1, pPtr) != 1){ /*Sobrescrevendo o produto com qtd atualizada*/
							printf("\nErro no arquivo.\n");
							exit(1);
						}
						printf("\nQuantidade do produto atualizada com sucesso!\n");
						//----------------------------------------------------------------------------------------------------------------
					} /*Fim do if que verifica se o status anterior era Entregue*/
					strcpy (pedido.status, "Devolução");
					break;
					
				default:
					printf("\nOpção Inválida.\n");
					fflush(stdin);
					scanf("%*c");
					break;	
			}/*Fim do switch*/
			
			_strdate (pedido.data); /*Grava a data do sistema na variável*/
			/*Trocando a data do sistema para o padrão brasileiro*/
			for (i = 0; i <= 7; i++){
				if (i <= 2)
					dataTemp[i+3] = pedido.data[i];
				if (i >= 3 && i <= 5)
					dataTemp[i-3] = pedido.data[i];
				if (i > 5)
					dataTemp[i] = pedido.data[i];
			} strcpy(pedido.data, dataTemp);
			
			/*Gravando a variável pedido atualizada no arquivo ----------------------------------------------------------------------*/
			if (fseek( rPtr, (pedido.cod - 1) * sizeof (Request), SEEK_SET) != 0){
				printf("\nErro inesperado.\n");
				exit(1);
			}
			if ( fwrite( &pedido, sizeof (Request), 1, rPtr) != 1 ){
				printf("\nErro no arquivo.\n");
				exit(1);
			} //----------------------------------------------------------------------------------------------------------------------
			
			/*Fechando arquivo ------------------------------------------------------------------------------------------------------*/
			if ( fclose(pPtr) != 0 ){
				printf("\nProblemas ao fechar arquivo %s\n", arq);
				exit(1);
			} pPtr = 0; 
			if ( fclose(rPtr) != 0){
				printf("\nProblemas ao fechar arquivo %s\n", arq);
				exit(1);
			} rPtr = 0; //------------------------------------------------------------------------------------------------------------
			
			printf("\nPedido atualizado com sucesso!\n\n");
		}/*Fim do if Atualizar Status*/
		
		else if ( j == '2' ){ /*Atualizar pedido inteiro*/	/*Capturando os dados na variável pedido da struct Request*/
			capturaRequest();
			printf("\nPedido atualizado com sucesso!\n\n");
		}
		else /* 0 digitado ou qualquer outro caractere - Não atualizar status*/
			Clear;
	}/*Fim do if pedido.cod != 0 (Caso o pedido exista)*/
			
	else{ /*pedido.cod == 0 (pedido não existe)*/
		printf("\nPedido não cadastrado.\n");
		capturaRequest();
		printf("\nPedido cadastrado com sucesso!\n\n");
	}/*Fim do else*/
	
}/*Fim da função*/

void capturaRequest (void){
	rPtr = 0;
	pPtr = 0;
		
	/*Mostrará a lista de produtos para a escolha do pedido ----------------------------------------------------------------------*/
	imprimiProduct("product.txt", SIZE);
	
	/*Abre arquivo Produtos para atualização em modo r+ (leitura e escrita)*/
	if ((pPtr = fopen ("product.txt", "r+")) == NULL){
		printf("\nImpossível abrir arquivo %s\n", "product.txt");
		exit(1);
	}
	/*Abre request para atualização*/
	if ((rPtr = fopen ("request.txt", "r+")) == NULL){
		printf("\nImpossível abrir arquivo %s\n", "request.txt");
		exit(1);
	}
		
	printf("\nDigite o código do produto cadastrado: ");
	fflush(stdin);
	scanf("%d", &pedido.productcod); if (pedido.productcod > SIZE){ printf("\nCódigo inválido (Somente de 1 a %d).\n", SIZE); exit(1); } /*Capturando e verificando o código identificador*/
	
	/*Capturando dados do produto na variável-struct produto para manipular junto com os pedidos*/
	if (fseek( pPtr, (pedido.productcod - 1) * sizeof (Product), SEEK_SET) != 0){
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&produto, sizeof(Product), 1, pPtr) != 1){
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} if(produto.cod == 0){ /*Verificando se o produto está cadastrado*/
		printf("\nProduto não cadastrado, cadastre-o primeiro.\n");
		exit(1);
	} /*------------------------------------------------------------------------------------------------*/
	
	pedido.cod = cod;
	
	strcpy(pedido.nome, produto.nome);
			
	printf("\nDigite a quantidade: ");
	fflush(stdin);
	scanf("%d", &pedido.qtd);
		
	printf("\nDigite o preço de cada produto: ");
	fflush(stdin);
	scanf("%f", &pedido.pcompra);
	//Atualizar preço de compra em produto
	produto.pcompra = pedido.pcompra;
			
	printf("\nDigite o nome da Produtora: ");
	fflush(stdin);
	gets(pedido.produtora);
	//Atualizar nome da Produtora em produto
	strcpy(produto.produtora, pedido.produtora);
		
	printf("\nDigite o número de telefone da Produtora para contato:\n");
	fflush(stdin);
	for (i = 0; i < 15; i++){ /*Colocar caracteres especiais automaticamente  */
		if (i == 0){
			pedido.tel[i] = '(';
			printf("(");
		}
		else if (i == 3){
			pedido.tel[i] = ')';
			printf(")");
		}
		else if (i == 4){
			pedido.tel[i] = ' ';
			printf(" ");
		}
		else if (i == 10){
			pedido.tel[i] = '-';
			printf("-");
		}
		else{
			pedido.tel[i] = getch();
			printf("%c", pedido.tel[i]);
		}
	}/*Fim do for*/ 
	printf("\n");
		
	printf("\nDigite a data de entrega (exemplo: 01/01/18): ");
	fflush(stdin);
		for (i = 0; i < 8; i++){ /*Colocar caracteres especiais automaticamente  */
		if (i == 2 || i == 5){
			pedido.entrega[i] = '/';
			printf("/");
		}
		else{
			pedido.entrega[i] = getch();
			printf("%c", pedido.entrega[i]);
		}
	}/*Fim do for*/
	printf("\n");
	
	_strdate (pedido.data); /*Grava a data do sistema na variável*/
	/*Trocando a data do sistema para o padrão brasileiro*/
	for (i = 0; i <= 7; i++){
		if (i <= 2)
			dataTemp[i+3] = pedido.data[i];
		if (i >= 3 && i <= 5)
			dataTemp[i-3] = pedido.data[i];
		if (i > 5)
			dataTemp[i] = pedido.data[i];
	} strcpy(pedido.data, dataTemp);
	
	strcpy(produto.data, pedido.data);
	
	strcpy (pedido.status, "Em andamento"); /*Por padrão quando atualiza o pedido fica Em andamento*/

	/*Escreve os dados no arquivo Pedidos*/
	if (fseek( rPtr, (pedido.cod - 1) * sizeof (Request), SEEK_SET) != 0){
		printf("\nErro inesperado.\n");
		exit(1);
	}
		
	if ( fwrite( &pedido, sizeof (Request), 1, rPtr) != 1 ){
		printf("\nErro no arquivo.\n");
		exit(1);
	} //----------------------------------------------------------------------------------------------------------------
		
	/*Atualiza os dados no arquivo Produto --------------------------------------------------------------------------*/
	if (fseek( pPtr, (pedido.productcod - 1) * sizeof (Product), SEEK_SET) != 0){
		printf("\nErro inesperado.\n");
		exit(1);
	}
			
	if ( fwrite( &produto, sizeof (Product), 1, pPtr) != 1 ){
		printf("\nErro no arquivo.\n");
		exit(1);
	} //--------------------------------------------------------------------------------------------------------------
	
	
	/*Fechamento de arquivos -----------------------------------------------------------------------------------------*/
	if ( fclose(pPtr) != 0 ){
		printf("\nProblemas ao fechar arquivo %s\n", "product.txt");
		exit(1);
	} pPtr = 0;
	
	if ( fclose(rPtr) != 0){
		printf("\nProblemas ao fechar arquivo %s\n", "request.txt");
		exit(1); 
	} rPtr = 0; //------------------------------------------------------------------------------------------------------
}

void consultaRequest(char arq[], int size){
	Clear;
	
	rPtr = 0;
	
	/*Abre arquivo em modo r (leitura e escrita), se não existir o arquivo, cria em modo w -----------------------------------*/
	if ((rPtr = fopen (arq, "r")) == NULL){
		if ((rPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de pedidos em branco caso aberto em modo w*/
			if ( fwrite(&pedido, sizeof(Request), 1, rPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(rPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} rPtr = 0;
		
		if ((rPtr = fopen (arq, "r")) == NULL){ /*Finalmente abre em modo r*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //---------------------------------------------------------------------------------------------------
	
	printf("Digite o código de identificação do pedido: (1 a %d) ", size);
	fflush(stdin);
	scanf("%d", &pedido.cod); if (pedido.cod > size){ printf("\nCódigo inválido (Somente de 1 a %d).\n", size); exit(1); }
	printf("\n");
	
	/*Lendo o arquivo e gravando os dados na variável pedido ---------------------------------------------------------------------------------------*/
	if (fseek( rPtr, (pedido.cod - 1) * sizeof (Request), SEEK_SET) != 0){ /*Busca a posição no arquivo para o registro especificado pelo usuário*/
		printf("\nErro inesperado.\n");
		exit(1);
	}
	if ( fread(&pedido, sizeof(Request), 1, rPtr) != 1){ /*Lê em pedido registros correpondentes a struct Request*/
		printf("\nErro na gravação do arquivo.\n");
		exit(1);
	} //---------------------------------------------------------------------------------------------------------------------------------------------
	
	/*Mostra o pedido se não nulo -----------------------------------------------------------------------------------------------------------------*/
	if (pedido.cod != 0)
		printf("%d\nStatus: %s\nCódigo do produto: %d\nNome: %s\tQuantidade: %d\tEntrega: %s\nPreço total de Compra: R$ %.2f (R$ %.2f cada)\nProdutora: %s\tTelefone: %s\nData de atualização: %s\n\n", pedido.cod, pedido.status, pedido.productcod, pedido.nome, pedido.qtd, pedido.entrega, (pedido.pcompra * (float) pedido.qtd), pedido.pcompra, pedido.produtora, pedido.tel, pedido.data);
	else
		printf("Pedido não cadastrado.\n\n");
	//-----------------------------------------------------------------------------------------------------------------------------------------------
	
	/*Fechando arquivo -----------------------------------------------------------------------------------------------------------------------------*/
	if ( fclose(rPtr) != 0 ){
		printf("\nProblemas ao fechar arquivo %s\n", arq);
		exit(1);
	} rPtr = 0;
	//-----------------------------------------------------------------------------------------------------------------------------------------------
}/*Fim da função*/

void imprimiRequest (char arq[], int size){
	Clear;
	
	rPtr = 0; /*Zerando ponteiro de arquivo*/
	int r = 0; /*Contador da impressão*/
	
	/*Abre arquivo em modo r (leitura e escrita), se não existir o arquivo, cria em modo w --------------------------------------------*/
	if ((rPtr = fopen (arq, "r")) == NULL){
		if ((rPtr = fopen (arq, "w")) == NULL){
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}
		
		for (i = 1; i <= size; i++){ /*Escreve 'size' linhas de dados de pedidos em branco caso aberto em modo w*/
			if ( fwrite(&pedido, sizeof(Request), 1, rPtr) != 1){
				printf("\nErro no arquivo.\n");
				exit(1);
			}/*Fim do if*/
		}/*Fim do for*/
		
		if ( fclose(rPtr) != 0 ){ //Fecha o arquivo de modo w
			printf("\nProblemas ao fechar arquivo %s\n", arq);
			exit(1);
		} rPtr = 0;
		
		if ((rPtr = fopen (arq, "r")) == NULL){ /*Finalmente abre em modo r*/
			printf("\nImpossível abrir arquivo %s\n", arq);
			exit(1);
		}/*Fim do if interno*/
	}/*Fim do if externo*/ //-------------------------------------------------------------------------------------------------------------
	
	/* ----- !feof estava dando erro com o fread por um motivo que não descobri ainda, então usei uma forma mais manual de apresentação ------ */
	for (i = 0; i < size; i++){
		
		/*Lendo o arquivo e gravando os dados na variável pedido ---------------------------------------------------------------------------------------*/
		if (fseek( rPtr, i * sizeof (Request), SEEK_SET) != 0){ /*Busca a posição no arquivo para o registro especificado pelo usuário*/
			printf("\nErro inesperado.\n");
			exit(1);
		}
		if ( fread(&pedido, sizeof(Request), 1, rPtr) != 1){ /*Lê em peddo registros correpondentes a struct Request*/
			printf("\nErro na gravação do arquivo.\n");
			exit(1);
		} //---------------------------------------------------------------------------------------------------------------------------------------------
	
		if (pedido.cod != 0){ /*Mostra as contas registradas (não nulas)*/
			printf("%d\nStatus: %s\nCódigo do produto: %d\nNome: %s\tQuantidade: %d\tEntrega: %s\nPreço total de Compra: R$ %.2f (R$ %.2f cada)\nProdutora: %s\tTelefone: %s\nData de atualização: %s\n\n", pedido.cod, pedido.status, pedido.productcod, pedido.nome, pedido.qtd, pedido.entrega, (pedido.pcompra * (float) pedido.qtd), pedido.pcompra, pedido.produtora, pedido.tel, pedido.data);
			r++; /*Contador de quantidade de pedidos existentes*/
		}
	}/*Fim do for*/
	
	if (r == 0){ /*Contador de pedidos cadastrados zerado - Nenhum pedido cadastrado*/
		Clear;
		printf("Nenhum pedido cadastrado.\n");
		fflush(stdin);
		scanf("%*c");
	}
} /*Fim da função*/

