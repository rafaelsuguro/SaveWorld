/**************************************
** Implementacão da heurística GRASP **
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include "quicksort.h"

#include <signal.h>
#include <string.h>
#include <sys/time.h>


/* Estrutura que contém dados sobre o resultado parciais ou finais da heurística */
typedef struct Resultado
{
    int *vetorPontosCobertos;          /* Vetor dos pontos cobertos pelas estacões que estão na solucão */
    Estacao *vetorEstacoesResultado;   /* Vetor que contém as estacões que estão na solucão */
    int nEstacoesResultado;            /* Número de estacões da solucão */
    float custo;                       /* Custo calculado até o momento das estacões */
}Resultado;

Estacao *vetorEstacoes;                             /* Vetor de estacões que irá ser ordenado pelo quicksort */
Resultado vetorResultado, vetorResultadoFinal;      /* Variáveis de resultado parcial e final, respectivamente */
int nPontos, nEstacoes;                             /* Variáveis com o número de pontos a serem cobertos e número de estacões existentes, respectivamente */


/* Executa leitura de dados do arquivo de teste */
void leDados()
{
    /* Variá veis locais */
    char flag;
    int lido;
    int contEstacoes, contPontos = 0;
    
    scanf("%c %d\n", &flag, &nPontos); /* Leitura do número de pontos a serem cobertos */ 
    fflush(stdin);
    scanf("%c %d\n", &flag, &nEstacoes); /* Leitura do número de estacões disponíveis */
    fflush(stdin);
    
    vetorEstacoes = (Estacao*)malloc(sizeof(Estacao)*nEstacoes); /* Aloca memória para armazenar o vetor de estacões */
    
    for(contEstacoes = 0; contEstacoes < nEstacoes; contEstacoes++)
    {
        vetorEstacoes[contEstacoes].vPontosEstacao = (int*)malloc(sizeof(int)*nPontos); /* Aloca memória para armazenar o vetor de pontos cobertos pela estacão */
        scanf("%s %f ", vetorEstacoes[contEstacoes].nomeEstacao, &vetorEstacoes[contEstacoes].custoEstacao);  /* Armazena o custo da estacão */
        contPontos = 0;
        do
        {
            scanf("%d%c%c", &lido,&flag,&flag); /* Armazena pontos cobertos pela estacão */
            /* Voltando no stdin, esse eh um metodo de capturar o \n sem avancar o ponteiro do stdin sem necessidade */
            fseek(stdin, -1, SEEK_CUR);
            fflush(stdin);
            vetorEstacoes[contEstacoes].vPontosEstacao[contPontos] = lido;
            contPontos++;
        } while(flag != '\n');
        vetorEstacoes[contEstacoes].nPontosEstacao = contPontos;
    }
}

/* Calcula a RCL do algoritmo guloso aleatório */
int calcula_RCL() {
    int i;
    float cmin,cmax,max,alpha;

    alpha = 0.5; /* porcentagem utilizada do vetor */
    cmin = vetorEstacoes[0].custoEstacao/vetorEstacoes[0].nPontosEstacao;
    cmax = vetorEstacoes[nEstacoes - 1].custoEstacao/vetorEstacoes[nEstacoes - 1].nPontosEstacao;
    /* Verifica se todos as estacões possuem a mesma relação da ordenação, no
       caso, custo da estacão dividido pelo número de pontos cobertos pela estacão */
    if(cmax == cmin) {
        alpha = 0.5; /* porcentagem utilizada do vetor */
        cmin = vetorEstacoes[0].custoEstacao;
        cmax = vetorEstacoes[nEstacoes - 1].custoEstacao;
        max = cmin + (alpha * (cmax - cmin));
        i = 0;
        while(vetorEstacoes[i].custoEstacao < max) {
            i++;
        }
    }
    else if(cmax != cmin) {
        max = cmin + (alpha * (cmax - cmin));
        i = 0;
        while(vetorEstacoes[i].custoEstacao/vetorEstacoes[i].nPontosEstacao < max) {
            i++;
        }
    }
    return i;
}


/* Executa algoritmo guloso aleatório */
void buscaGulosa()
{
    /* Variáveis locais */
    int contEstacoes, contPontos, contPontosEstacao, contResultado = 0;
    int RCL = calcula_RCL();
    struct timeval random;
    int r_RCL;
    
    /* Inicializa variável que conterá o resultado da iteracão do algoritmo guloso aleatório
       e depois será utilizado na busca local */        
    vetorResultado.custo = 0;
    vetorResultado.vetorEstacoesResultado = (Estacao*)malloc(sizeof(Estacao)*nEstacoes);
    vetorResultado.vetorPontosCobertos = (int*)malloc(sizeof(int)*nPontos);
    
    for(contPontos = 0; contPontos < nPontos; contPontos++)
    {
        vetorResultado.vetorPontosCobertos[contPontos] = 0;
    }
    
    gettimeofday(&random,NULL);
    r_RCL = random.tv_usec % RCL; /* sorteia estacão da RCL, num range da RCL sorteado com os microsegundos para uma melhor randomizacão */
    
    /* Insere o primeiro elemento na solucão */
    vetorResultado.vetorEstacoesResultado[contResultado] = vetorEstacoes[r_RCL];
    vetorResultado.custo += vetorEstacoes[r_RCL].custoEstacao;
    for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[r_RCL].nPontosEstacao; contPontosEstacao++)
    {
        vetorResultado.vetorPontosCobertos[vetorEstacoes[r_RCL].vPontosEstacao[contPontosEstacao] - 1]++;
    }
    contResultado++;
    
    /* Insere os demais elementos, verificando as restricões */
    for(contEstacoes = 0; contEstacoes < RCL; contEstacoes++)
    {
        /* Verifica se os pontos cobertos pela estacão existem no vetor de pontos cobertos da solucão */ 
        for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[contEstacoes].nPontosEstacao; contPontosEstacao++)
        {
            /* Se algum ponto não existir na solucão, pára */
            if(vetorResultado.vetorPontosCobertos[vetorEstacoes[contEstacoes].vPontosEstacao[contPontosEstacao] - 1] == 0)
            {
                break;
            }
        }
        /* Se for possível, insere novo elemento na solucão */
        if(contPontosEstacao != vetorEstacoes[contEstacoes].nPontosEstacao)
        {
            vetorResultado.vetorEstacoesResultado[contResultado] = vetorEstacoes[contEstacoes];
            vetorResultado.custo += vetorEstacoes[contEstacoes].custoEstacao;
            for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[contEstacoes].nPontosEstacao; contPontosEstacao++)
            {
                vetorResultado.vetorPontosCobertos[vetorEstacoes[contEstacoes].vPontosEstacao[contPontosEstacao] - 1]++;
            }
            contResultado++;
        }
        /* Verifica se todos os pontos a serem cobertos foram cobertos */
        for(contPontos = 0; contPontos < nPontos; contPontos++)
        {
            /* Caso um dos pontos ainda não tenha sido coberto, pára */
            if(vetorResultado.vetorPontosCobertos[contPontos] == 0)
            {
                break;
            }
        }
        /* Caso todos os pontos tenham sidos cobertos, pára */
        if(contPontos == nPontos)
        {
            break;
        }
        /* Se o algoritmo ainda não encontrou solucão válida e não há mais elementos na RCL
           insere um novo elemento na RCL */
        if(contPontos != nPontos && contEstacoes == RCL - 1 && RCL < nEstacoes)
        {
            RCL = RCL + 1;
        }
    }
    vetorResultado.nEstacoesResultado = contResultado;
}

/* Funcão auxiliar que remove uma estacão*/
void removeEstacao(int i) {
	int nextIndex = i + 1;
    int custoEstacaoRemovida = vetorResultado.vetorEstacoesResultado[i].custoEstacao;
    int j;
    
    
    for(j = 0; j < vetorResultado.vetorEstacoesResultado[i].nPontosEstacao; j++)
    {
        vetorResultado.vetorPontosCobertos[vetorResultado.vetorEstacoesResultado[i].vPontosEstacao[j] - 1]--;
    }
	/* Se tiver out of bounds */
	while (nextIndex < vetorResultado.nEstacoesResultado)
	{
		vetorResultado.vetorEstacoesResultado[i] = vetorResultado.vetorEstacoesResultado[nextIndex];
		i++;
		nextIndex++;
	}
    vetorResultado.custo = vetorResultado.custo - custoEstacaoRemovida;
	vetorResultado.nEstacoesResultado--;
}

/* Executa a busca local */
void buscaLocal() {
	Estacao e;
	int i, j;
	int pontoASerRemovido;
	/* Vamos procurar estacoes que tem todos os pontos com multiplicidade maior que 1
	/  Ou seja, essa estacao esta so adicionando custo porque os pontos ja foram cobertos
	/  Detalhe que se houverem mais de 1 estacao nessa situacao a escolha vai ser gulosa
	/  e deterministica, ja que estamos correndo o vetor em ordem crescente */
	for(i = 0; i < vetorResultado.nEstacoesResultado; i++)
	{
		e = vetorResultado.vetorEstacoesResultado[i];
		for(j = 0; j < e.nPontosEstacao; j++)
		{
			pontoASerRemovido = e.vPontosEstacao[j];
			/* Caso algum ponto seja exclusivamente coberto por esta estacao, ela nao pode ser removida */
			if (vetorResultado.vetorPontosCobertos[pontoASerRemovido - 1]  <= 1)
			{
				break;
			}
		}
		/* Se for true eh porque todos os pontos tem multiplicidade maior que 1 */
		if ( e.nPontosEstacao == j )
		{
			removeEstacao(i);
			/* Como removemos uma estacao, voltamos um passo no contador do for() */
			i--;
		}
	}
}

/* Compara se o resultado obtido na iteracão é melhor que o resultado global, caso isto ocorra
   atualiza a solucão */
void comparaResultado()
{
    int i;
    
    if(vetorResultadoFinal.custo > vetorResultado.custo || vetorResultadoFinal.custo == -1)
    {
        if(vetorResultadoFinal.custo != -1)
        {
            free(vetorResultadoFinal.vetorEstacoesResultado);
            free(vetorResultadoFinal.vetorPontosCobertos);
        }
        vetorResultadoFinal.custo = vetorResultado.custo;
        vetorResultadoFinal.nEstacoesResultado = vetorResultado.nEstacoesResultado;
        vetorResultadoFinal.vetorEstacoesResultado = (Estacao*)malloc(sizeof(Estacao)*(vetorResultado.nEstacoesResultado));
        vetorResultadoFinal.vetorPontosCobertos = (int*)malloc(sizeof(int)*nPontos);
        for(i = 0; i < vetorResultado.nEstacoesResultado; i++)
        {
            vetorResultadoFinal.vetorEstacoesResultado[i] = vetorResultado.vetorEstacoesResultado[i];
        }
        for(i = 0; i < nPontos; i++)
        {
            vetorResultadoFinal.vetorPontosCobertos[i] = vetorResultado.vetorPontosCobertos[i];
        }
    }
    free(vetorResultado.vetorEstacoesResultado);
    free(vetorResultado.vetorPontosCobertos);
}

/* Funcão auxiliar que ordena os nomes das estacoes resultantes */
void bubble(int a[],int n)  
{  
    int i,j,t;  
    for(i=n-2;i>=0;i--)  
    {  
        for(j=0;j<=i;j++)    
        {  
            if(a[j]>a[j+1])  
            {  
                t=a[j];  
                a[j]=a[j+1];  
                a[j+1]=t;  
            }  
        }   
    }
}

/* Imprime o resultado */
void imprimeResultado()
{
    int contEstacoesResultado;
    int i,j;
    int v[vetorResultadoFinal.nEstacoesResultado];
    
    printf("Custo: %.2f\n", vetorResultadoFinal.custo);
    printf("Total: %d\n", vetorResultadoFinal.nEstacoesResultado);
    for(contEstacoesResultado = 0; contEstacoesResultado < vetorResultadoFinal.nEstacoesResultado; contEstacoesResultado++)
    {
        for(i = 0; i < 10 && vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao[i] != '\0'; i++)
        {
            if(vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao[i] < '0' ||
               vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao[i] > '9')
            {
                for(j = 0; j < 10 && vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao[j] != '\0'; j++)
                {
                    vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao[j] = vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao[j + 1];
                }
            }
        }
        v[contEstacoesResultado] = atoi(vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao);
    }
    bubble(v, vetorResultadoFinal.nEstacoesResultado);
    for(contEstacoesResultado = 0; contEstacoesResultado < vetorResultadoFinal.nEstacoesResultado; contEstacoesResultado++)
    {
        printf("S_%d\n", v[contEstacoesResultado]);
    }
}

void timer_handler (int signum)
{
    imprimeResultado();
	exit(0);
}

int main(int argc, char *argv[])
{
    /* Structs para uso do timer */
    struct sigaction sa;
    struct itimerval timer;
    
    /* Instalando o timer_handler como o signal handler para o sinal SIGALRM */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &timer_handler;
    sigaction (SIGALRM, &sa, NULL);
    
    /* Configurando o timer expirar em 58 segundos, dando 2 segundos pro imprimeResultado funcionar */
    timer.it_value.tv_sec = 58; /* Temporario em 5 segundos, mudar para 58 */
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    
    /* Iniciando o timer, estamos usando tempo Real */
    setitimer (ITIMER_REAL, &timer, NULL);
    
    vetorResultadoFinal.custo = -1;

    freopen(argv[1], "r", stdin);
    leDados(); 
    quickSort(vetorEstacoes, 0, nEstacoes - 1);   
    while (1) {
        buscaGulosa();
        buscaLocal();
        comparaResultado();
    }    
    return 0;
}
