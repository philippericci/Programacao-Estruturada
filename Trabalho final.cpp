#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct reg
{
	char produto[30];
	int quantidade;
	float preco;
	char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);
void comparar(FILE *); //este item e novo, vai comparar dois produtos e vai indicar qual e mais barato 

int main()
{
	setlocale(LC_ALL, "");
	int op;
	FILE *arq;

	if ((arq = fopen("dados1.dat", "rb+")) == NULL)
	{
		if ((arq = fopen("dados1.dat", "wb+")) == NULL)
		{
			printf("Falha ao abrir o arquivo!\n");
			system("pause");
		}
	}

	//existentes no arquivo agenda1.dat
	do
	{
		system("CLS");
		printf("\n======= ESTOQUE ======= \n");
		printf("1.Cadastrar\n");
		printf("2.Consultar por codigo\n");
		printf("3.Gerar arquivo\n");
		printf("4.Comparar precos\n");
		printf("5.Excluir registro\n");
		printf("6.Sair\n");
		printf("===========Produtos: %d\n", tamanho(arq));
		printf("Opcao:");
		scanf("%d", &op);
		switch (op)
		{
		case 1://cadastrar novo contato
			cadastrar(arq);
			break;
		case 2://consultar por código
			consultar(arq);
			break;
		case 3://	gera arquivo de texto com todos os produtos ordenados alfabeticamente
		     geraarqtxt(arq);
			break;
			
		case 4: //compara dois itens por preço
			comparar(arq); 
			break;
			
		case 5: //exclui um registro do arq
		excluir(arq); 
			break;

		case 6: fclose(arq);
		}
	} while (op != 5);
}

void cadastrar(FILE *arq)
{
	setlocale(LC_ALL, "");
	reg item;
	char confirma;
	item.status = ' ';
	fflush(stdin);

	printf("--- Cadastrando novo produto --- \n");
	printf("\nIndentificacao do produto:%d\n", tamanho(arq) + 1);
	printf("Nome do Produto ..........:"); //getchar();
	gets(item.produto);
	printf("Quantidade de Produto......:");
	scanf("%i",&item.quantidade);
	printf("Preço........:");
	scanf("%f",&item.preco);
	printf("\nConfirma <s/n>:");
	fflush(stdin);
	scanf("%c", &confirma);

	if (toupper(confirma) == 'S')
	{
		printf("\ngravando...\n\n");
		fseek(arq, 0, SEEK_END);
		fwrite(&item, sizeof(reg), 1, arq);

	}
	system("pause");
}

void consultar(FILE *arq)
{
	setlocale(LC_ALL, "");
	reg item;
	int nr;
	printf("\nConsulta pelo codigo\n");
	printf("\nInforme o Codigo...:");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&item, sizeof(reg), 1, arq);
		if (item.status == ' ')
		{
			printf("\nProduto......:%s", item.produto);
			printf("\nQuantidade..:%i", item.quantidade);
			printf("\nPreco....:%.2f\n\n", item.preco);
		}

		else
			printf("\nRegistro excluido! \n");

	}
	else
	{
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

void geraarqtxt(FILE *arq)
{
	setlocale(LC_ALL, "");
	char nomearq[20];
	fflush(stdin);     				//Estava faltando limpar o buffer do teclado...
	printf("Nome do arquivo texto:");
	scanf("%s", &nomearq);			//Estava faltando o "&" antes da variavel
	strcat(nomearq, ".txt");
	FILE *arqtxt = fopen(nomearq, "w");
	if (!arqtxt)
	{
		printf("Nao foi possivel criar esse arquivo!\n");
		system("pause");
		//return;
	}
	else				//Estava faltando o else...
	{
		fprintf(arqtxt, "Nome                Quantidade    Preço                   Status\n");
		fprintf(arqtxt, "================================================================\n");
		
		int nr;
		reg item;
		for (nr = 0; nr<tamanho(arq); nr++)
		{
			fseek(arq, nr * sizeof(reg), SEEK_SET);
			fread(&item, sizeof(reg), 1, arq);
			fprintf(arqtxt, "%-30s %-5d %-10.2f %-5c\n", item.produto, item.quantidade, item.preco, item.status);//Estavam todas com %s porém como o código de vocês usam variavel int e float estava fechando o programa quando chamavam as variaveis
		}
		fprintf(arqtxt, "================================================================\n");
		fclose(arqtxt);
		system("pause");
	}
}

void excluir(FILE *arq)
{
	setlocale(LC_ALL, "");
	reg item;
	char confirma;
	int nr;

	printf("\nInforme o codigo do registro para excluir\n");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&item, sizeof(reg), 1, arq);
		if (item.status == ' ')
		{
			printf("\nProduto......:%s", item.produto);
			printf("\nQuantidade..:%i", item.quantidade);
			printf("\nPreço....:%.2f\n", item.preco);
			printf("\nConfirma a exclusao: <s/n>\n");
			getchar();
			scanf("%c", &confirma);

			if (toupper(confirma) == 'S')
			{
				printf("\nexcluindo...\n\n");
				fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
				item.status = '*';
				fwrite(&item, sizeof(reg), 1, arq);
			}
		}
	else
		printf("Registro inexistente! \n");
	}
	else
	{
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

int tamanho(FILE *arq)
{
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(reg);
}


void comparar(FILE *arq) // este void e o novo, tenta comparar os produtos pelo codigo, tipo chama os codigo e compare apenas os precos 
{
	setlocale(LC_ALL, "");
	reg item1,item2;
	float preco1,preco2;
	int nr1,nr2;
	
	printf("\nComparacao pelos codigos\n");
	printf("\nInforme o primeiro codigo...:");
	scanf("%d", &nr1);
	if ((nr1 <= tamanho(arq)) && (nr1>0))
	{
		fseek(arq, (nr1 - 1) * sizeof(reg), SEEK_SET);
		fread(&item1, sizeof(reg), 1, arq);
		if (item1.status == ' ')
		{
			preco1 = item1.preco;			
			
			printf ("\nInforme o segundo codigo...:");
			scanf ("%d",&nr2);
			if ((nr2 <= tamanho(arq)) && (nr2>0))
				{
				fseek(arq, (nr2 - 1) * sizeof(reg), SEEK_SET);
				fread(&item2, sizeof(reg), 1, arq);
				if (item2.status == ' ')
				{
					preco2 = item2.preco;					
					printf ("Item 1: R$ %.2f \tItem2: R$ %.2f\n", preco1,preco2);
					
					if (preco1>preco2){
						printf ("Item 1 e mais caro que o Item 2.\n");
					}
					else if (preco1==preco2){
						printf ("Os dois itens possuem o mesmo preço.\n");
					}
					else if (preco1<preco2){
						printf ("Item 1 é mais barato que o Item 2.\n");
					}
				}

				else
				printf("\nRegistro excluido! \n");
				}
				else
				{
				printf("\nNumero de registro invalido!\n");
				}
			}
			else
			printf("\nRegistro excluido! \n");
			}
			else
			{
			printf("\nNumero de registro invalido!\n");
			}
	
	
	system ("pause");
}
