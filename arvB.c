/* Mudanças a serem feitas a Arvore B:
	- 1) Guardar inteiros;
	- 2) Verificar a redundancia no teste da funcao "insereChave"
	- 3) Mudar a estrategia p/ quando insere uma folha, entanto a descidas para 
	nulo
*/

#include <stdio.h>
#include <stdlib.h>

#define T 2	//Constante T que determina o tamanho da Arv.B
#define MAX_CHAVES ( 2 * T - 1 )	//MÃ¡ximo de registros
#define MAX_FILHOS ( 2 * T )		//MÃ¡ximo de filhos
#define MIN_CHAVES ( T - 1 )		//MÃ­nimo de registros


//DefiniÃ§Ã£o da estrutura 'tipoArvB'
struct estruturaArvB
{
	int contChaves;
	char chaves[MAX_CHAVES];
	struct estruturaArvB *filhos[MAX_FILHOS];
	int folha;
}; 
typedef struct estruturaArvB tipoArvB;


//ProtÃ³tipo das funÃ§Ãµes
tipoArvB  *alocaNovoNo ();
int buscaBinaria (tipoArvB *raiz, char valor); 
void insereChave (tipoArvB *raiz, char valor, tipoArvB *filhoDir);
tipoArvB *insere (tipoArvB *raiz, char valor, int *flag, char *valorRetorno);
tipoArvB *insereArvB (tipoArvB *raiz, char valor);
void imprime_arvore(tipoArvB *arv);
void imprime_no(tipoArvB* arv);


//FunÃ§Ã£o para alocaÃ§Ã£o de um novo nÃ³ (inicializando os campos)
tipoArvB  *alocaNovoNo() {
	tipoArvB *novoNo;
	int i;
	
	novoNo = (tipoArvB *) malloc (sizeof(tipoArvB));
	novoNo->contChaves = 0;
	novoNo->folha = 1;
	
	for (i = 0; i < MAX_CHAVES; i++)
		novoNo->chaves[i] = 0;
	
	for (i = 0; i < MAX_FILHOS; i++)
		novoNo->filhos[i] = NULL;

	return novoNo;
}


//FuncÃ£o que executa a busca binÃ¡ria no nÃ³
int buscaBinaria (tipoArvB *raiz, char valor) {
	int inicio, meio, fim;

	inicio = 0;
	fim = raiz->contChaves;
		
	while (inicio < fim) {
		if (inicio == fim) {
			meio = inicio; //ou fim
			return (meio);
		} else {
			meio = (inicio + (fim - inicio) / 2 );
		}
		
		if ( raiz->chaves[meio] == valor ) {
			return meio;
		} else {
			if (valor < raiz->chaves[meio])
				fim = (meio-1);
			else
				inicio = (meio+1);
		}
	}
	return (meio);
}

//FunÃ§Ã£o para inserÃ§Ã£o de uma chave e o ponteiro para o filho direito em um nÃ³ nÃ£o cheio
void insereChave (tipoArvB *no, char valor, tipoArvB *filhoDir) {
	int i, pos;
	
	//Busca a posiÃ§Ã£o correta para inserÃ§Ã£o da nova chave
	pos = buscaBinaria (no, valor);
	i = no->contChaves;

	//Executa o remanejamento dos valores para manter a ordenaÃ§Ã£o
	while ( (i > pos) && (valor < no->chaves[i-1]) ) {
		no->chaves[i] = no->chaves[i-1];
		no->filhos[i+1] = no->filhos[i];
		i--;
	}

	//Executa a inserÃ§Ã£o da nova chave
	no->chaves[pos] = valor;
	no->filhos[pos+1] = filhoDir;
	no->contChaves++;
}

//FunÃ§Ã£o que busca pelo nÃ³ onde deve ser inserido o valor/chave e faz a quebra do nÃ³, quando necessÃ¡rio
tipoArvB *insere (tipoArvB *no, char valor, int *flag, char *valorRetorno) {
	int i, pos;
	char meio;
	tipoArvB *noAux, *filhoDir;
	
	if (no == NULL) {
		//EntÃ£o o nÃ³ pai (anterior) Ã© o nÃ³ onde deve ser feita a inserÃ§Ã£o, pois alcanÃ§ou um nÃ³ folha
		*flag = 1;
		*valorRetorno = valor;
		return (NULL);
	} else {
		//Executa a busca pelo nÃ³ onde o valor deve ser inserido
		pos = buscaBinaria (no, valor);
		
		//Identifica se a chave jÃ¡ estÃ¡ presente na Ã¡rvore
		if ( 	(pos < no->contChaves) && 
			(no->chaves[pos] == valor)
			){
			printf("O valor da chave jÃ¡ estÃ¡ presente na Ãrvore B!\n");
			*flag = 0;
		} else {
			//Desce na Ã¡rvore, buscando pelo nÃ³ folha onde deve ocorrer a inserÃ§Ã£o
			if ( no->chaves[pos] < valor)
				pos++;
			filhoDir = insere(no->filhos[pos], valor, flag, valorRetorno); 	//Executa chamada recursiva
			
			if ( *flag ) { //Se ocorreu a descida como esperado, entÃ£o insere o valor no nÃ³
				if ( no->contChaves < MAX_CHAVES) { //Verifica se hÃ¡ espaÃ§o no nÃ³
					insereChave(no, *valorRetorno, filhoDir);
					*flag = 0;
				} else { //EntÃ£o Ã© preciso dividir o nÃ³ p/ poder inserir o valor
					noAux = alocaNovoNo();
					meio = no->chaves[MIN_CHAVES];
				
					//Insere metade das chaves no novo nÃ³
					noAux->filhos[0] = no->filhos[MIN_CHAVES+1];
					for ( 	i = MIN_CHAVES + 1;
						i < MAX_CHAVES;
						i++				) {
						
						insereChave( noAux, no->chaves[i], no->filhos[i+1] );
					}
				
					//Atualiza o nÃ³ ("apaga" as chaves transferidas
					for (	i = MIN_CHAVES;
						i < MAX_CHAVES;
						i++				) {
						no->chaves[i] = 0;
						no->filhos[i+1] = NULL;
						no->contChaves--;
					}
					//no->contChaves = MIN_CHAVES;
				
					//Verifica em qual dos novos nÃ³s o valor deve ser inserido e executa
					if ( pos <= MIN_CHAVES) {
						insereChave(no, *valorRetorno, filhoDir);
					} else {
						insereChave(noAux, *valorRetorno, filhoDir);
					}
				
					//Retorna o elemento do meio para ser inserido no nÃ³ pai e o filho direito da chave central
					*flag = 1;
					*valorRetorno = meio;
					return (noAux);
				}
			}
		}
	}
}


// FunÃ§Ã£o principal de inserÃ§Ã£o na Ãrvore B (funÃ§Ã£o a ser chamada pelas funÃ§Ãµes externas)
tipoArvB *insereArvB (tipoArvB *raiz, char valor) {
	int flag;
	char valorRetorno, i;
	tipoArvB *filhoDir, *novaRaiz;
	
	filhoDir = insere (raiz, valor, &flag, &valorRetorno);
	
	//Verifica se ocorreu a descida na Ã¡rvore adequadamente, se hÃ¡ a necessidade de um novo nÃ³ como raiz
	if ( flag ) {
		novaRaiz = alocaNovoNo();
		novaRaiz->contChaves = 1;
		novaRaiz->chaves[0] = valorRetorno;
		novaRaiz->filhos[0] = raiz;
		novaRaiz->filhos[1] = filhoDir;
		novaRaiz->folha = 0;
		return (novaRaiz);
	} else {
		return (raiz);
	}
}


/*FunÃ§Ã£o para impressÃ£o da estrutura Ã¡rvore B (em prÃ©-ordem)
void imprimeB(tipoArvB *no) {
	int i;
	if (no != NULL) { 
		//Imprime as chaves
		printf("[");
		for ( i = 0; i < no->contChaves; i++ ) {
			printf("%d,", no->chaves[i]);
		}
		printf("]\n");
		
		//Executa chamada recursiva para os filhos
		for ( i = 0; i < no->contChaves + 1; i++ ) {
			imprimeB( no->filhos[i] );
		}
	}
}
*/


void imprime_arvore(tipoArvB *arv)
{
	int i;
	if (arv != NULL)
	{	
		imprime_no(arv);
		printf("\n");
		
		if (eh_folha(arv) == 0)
		{
			for (i = 0; i < arv->contChaves; i++)
			{
				printf("Filho do %d: ", arv->chaves[i]);
				imprime_arvore(arv->filhos[i]);
			}
			printf("Filho direito do %d: ", arv->chaves[i - 1]);
			imprime_arvore(arv->filhos[i]);
		}
	}
}


void imprime_no(tipoArvB* arv)
{
	int i = 0;
	if (arv == NULL)
		return;
	
	if (arv->contChaves == 1)
		printf("[%d]", arv->chaves[0]);
	
	else if (arv->contChaves == 2)
		printf("[%d, %d]", arv->chaves[0], arv->chaves[1]);
	
	else
	{
		printf("[%d", arv->chaves[0]);
		for (i = 1; i < arv->contChaves - 1; i++)
			printf(", %d", arv->chaves[i]);
		printf(", %d]", arv->chaves[i]);
		
	}
}

int eh_folha(tipoArvB* arv)
{
	int i;
	for (i = 0; i <= arv->contChaves; i++)
	{
		if (arv->filhos[i] != NULL)
			return 0;
	}
	return 1;
}