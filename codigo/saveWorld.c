#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <string.h>
#include <sys/time.h>



typedef struct Estacao 
{
    char nomeEstacao[10];
    float custoEstacao;
    int *vPontosEstacao;
    int nPontosEstacao;
    int flag;
}Estacao;

typedef struct Resultado
{
    int *vetorPontosCobertos;
    Estacao *vetorEstacoesResultado;
    int nEstacoesResultado;
    float custo;
}Resultado;

Estacao *vetorEstacoes;
Resultado vetorResultado, vetorResultadoFinal;
int nPontos, nEstacoes;


void swap(Estacao* a, Estacao* b) {
    
    Estacao tmp;
    
    tmp = *a;
    *a = *b;
    *b = tmp;
}


int partition(Estacao vec[], int left, int right) 
{
    
    int i, j;
    
    i = left;
    for (j = left + 1; j <= right; ++j) {
        if ((vec[j].custoEstacao/(float)(vec[j].nPontosEstacao)) < (vec[left].custoEstacao/(float)(vec[left].nPontosEstacao))) {
            
            ++i;
            swap(&vec[i], &vec[j]);
        }
        else if ((vec[j].custoEstacao/(float)(vec[j].nPontosEstacao)) == (vec[left].custoEstacao/(float)(vec[left].nPontosEstacao)))
        {
            if (vec[j].nPontosEstacao > vec[left].nPontosEstacao)
            {
                ++i;
                swap(&vec[i], &vec[j]);
            }
        }
    }
    swap(&vec[left], &vec[i]);
    
    return i;
}


void quickSort(Estacao vec[], int left, int right) {
    
    int r;
    
    if (right > left) {
        
        r = partition(vec, left, right);
        quickSort(vec, left, r - 1);
        quickSort(vec, r + 1, right);
    }
}


void leDados()
{
    char flag;
    int lido;
    int contEstacoes, contPontos = 0;
    
    scanf("%c %d\n", &flag, &nPontos);
    fflush(stdin);
    scanf("%c %d\n", &flag, &nEstacoes);
    fflush(stdin);
    
    vetorEstacoes = (Estacao*)malloc(sizeof(Estacao)*nEstacoes);
    
    for(contEstacoes = 0; contEstacoes < nEstacoes; contEstacoes++)
    {
        vetorEstacoes[contEstacoes].vPontosEstacao = (int*)malloc(sizeof(int)*nPontos);
        scanf("%s %f ", vetorEstacoes[contEstacoes].nomeEstacao, &vetorEstacoes[contEstacoes].custoEstacao);
        contPontos = 0;
        do
        {
            scanf("%d%c%c", &lido,&flag,&flag);
            /* Voltando no stdin, esse eh um metodo de capturar o \n sem avancar o ponteiro do stdin sem necessidade */
            fseek(stdin, -1, SEEK_CUR);
            fflush(stdin);
            vetorEstacoes[contEstacoes].vPontosEstacao[contPontos] = lido;
            contPontos++;
        } while(flag != '\n');
        vetorEstacoes[contEstacoes].nPontosEstacao = contPontos;
        vetorEstacoes[contEstacoes].flag = 0;
    }
}

//calcula a RCL do algoritmo semi-guloso
int calcula_RCL() {
    int i;
    float cmin,cmax,max,alpha;

    alpha = 0.5; //porcentagem utilizada do vetor
    cmax = vetorEstacoes[0].custoEstacao/vetorEstacoes[0].nPontosEstacao;
    cmin = vetorEstacoes[nEstacoes - 1].custoEstacao/vetorEstacoes[nEstacoes - 1].nPontosEstacao;
    //verifica se todos os compradores possuem a mesma relação da ordenação, no
    //caso, oferta dividido pelo número de itens para cada comprador
    if(cmax == cmin) {
        alpha = 0.5; //porcentagem utilizada do vetor
        cmax = vetorEstacoes[0].custoEstacao;
        cmin = vetorEstacoes[nEstacoes - 1].custoEstacao;
        max = cmax + (alpha * (cmin - cmax));
        i = 0;
        while(vetorEstacoes[i].custoEstacao < max) {
            i++;
        }
    }
    else if(cmax != cmin) {
        max = cmax + (alpha * (cmin - cmax));
        i = 0;
        while(vetorEstacoes[i].custoEstacao/vetorEstacoes[i].nPontosEstacao < max) {
            i++;
        }
    }
    return i;
}

void buscaGulosa()
{
    int contEstacoes, contPontos, contPontosEstacao, contResultado = 0;
    int RCL = calcula_RCL();
    struct timeval random;
    int r_RCL;
    
    gettimeofday(&random,NULL);
    r_RCL = random.tv_usec % RCL; //sorteia estacão da RCL
        
    vetorResultado.custo = 0;
    vetorResultado.vetorEstacoesResultado = (Estacao*)malloc(sizeof(Estacao)*nEstacoes);
    vetorResultado.vetorPontosCobertos = (int*)malloc(sizeof(int)*nPontos);
    
    for(contPontos = 0; contPontos < nPontos; contPontos++)
    {
        vetorResultado.vetorPontosCobertos[contPontos] = 0;
    }
    
    gettimeofday(&random,NULL);
    r_RCL = random.tv_usec % RCL; //sorteia estacão da RCL
    
    vetorResultado.vetorEstacoesResultado[contResultado] = vetorEstacoes[r_RCL];
    vetorResultado.custo += vetorEstacoes[r_RCL].custoEstacao;
    for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[r_RCL].nPontosEstacao; contPontosEstacao++)
    {
        vetorResultado.vetorPontosCobertos[vetorEstacoes[r_RCL].vPontosEstacao[contPontosEstacao] - 1]++;
    }
    contResultado++;
    vetorEstacoes[r_RCL].flag = 1;
    
    for(contEstacoes = 0; contEstacoes < RCL; contEstacoes++)
    {
        for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[contEstacoes].nPontosEstacao; contPontosEstacao++)
        {
            if(vetorResultado.vetorPontosCobertos[vetorEstacoes[contEstacoes].vPontosEstacao[contPontosEstacao] - 1] == 0)
            {
                break;
            }
        }
        if(contPontosEstacao != vetorEstacoes[contEstacoes].nPontosEstacao)
        {
            vetorResultado.vetorEstacoesResultado[contResultado] = vetorEstacoes[contEstacoes];
            vetorResultado.custo += vetorEstacoes[contEstacoes].custoEstacao;
            for(contPontosEstacao = 0; contPontosEstacao < vetorEstacoes[contEstacoes].nPontosEstacao; contPontosEstacao++)
            {
                vetorResultado.vetorPontosCobertos[vetorEstacoes[contEstacoes].vPontosEstacao[contPontosEstacao] - 1]++;
            }
            contResultado++;
            vetorEstacoes[contEstacoes].flag = 1;
        }
        for(contPontos = 0; contPontos < nPontos; contPontos++)
        {
            if(vetorResultado.vetorPontosCobertos[contPontos] == 0)
            {
                break;
            }
        }
        if(contPontos == nPontos)
        {
            break;
        }
        if(contPontos != nPontos && contEstacoes == RCL - 1 && RCL < nEstacoes)
        {
            RCL = RCL + 1;
        }
    }
    vetorResultado.nEstacoesResultado = contResultado;
}

void removeEstacao(int i) {
	int nextIndex = i + 1;
    int custoEstacaoRemovida = vetorResultado.vetorEstacoesResultado[i].custoEstacao;
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

void buscaLocal() {
	Estacao e;
	int i, j;
	/* Vamos procurar estacoes que tem todos os pontos com multiplicidade maior que 1
	/  Ou seja, essa estacao esta so adicionando custo porque os pontos ja foram cobertos
	/  Detalhe que se houverem mais de 1 estacao nessa situacao a escolha vai ser gulosa
	/  e deterministica, ja que estamos correndo o vetor em ordem crescente */
	for(i = 0; i < vetorResultado.nEstacoesResultado; i++)
	{
		e = vetorResultado.vetorEstacoesResultado[i];
		for(j = 0; j < e.nPontosEstacao; j++)
		{
			int pontoASerRemovido = e.vPontosEstacao[j];
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

void comparaResultado()
{
    if(vetorResultadoFinal.custo > vetorResultado.custo || vetorResultadoFinal.custo == -1)
    {
        vetorResultadoFinal = vetorResultado;
    }
}

void imprimeResultado()
{
    int contEstacoesResultado;
    
    printf("Custo: %.2f\n", vetorResultadoFinal.custo);
    printf("Total: %d\n", vetorResultadoFinal.nEstacoesResultado);
    for(contEstacoesResultado = 0; contEstacoesResultado < vetorResultadoFinal.nEstacoesResultado; contEstacoesResultado++)
    {
        printf("%s\n", vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nomeEstacao);
        printf("%d\n", vetorResultadoFinal.vetorEstacoesResultado[contEstacoesResultado].nPontosEstacao);
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
    timer.it_value.tv_sec = 5; /* Temporario em 5 segundos, mudar para 58 */
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    
    /* Iniciando o timer, estamos usando tempo Real */
    setitimer (ITIMER_REAL, &timer, NULL);
    
    vetorResultadoFinal.custo = -1;

    freopen(argv[1], "r", stdin);
    leDados();    
    while (1) {
        quickSort(vetorEstacoes, 0, nEstacoes - 1);
        buscaGulosa();
        buscaLocal();
        comparaResultado();
    }    
    return 0;
}
