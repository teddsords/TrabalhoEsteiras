#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QTDPRODUTOS 1500                                          // Quantidade de produtos que irao passar pelas esteiras
pthread_mutex_t lock;                                             // Variavel para poder realizar o lock nas threads
pthread_cond_t contagemAtingida;                                  // Variavel condicional para saber quando a contagem foi atingida


int quantidadeDeProdutosPassados = 0,                             // Variavel para a contagem de produtos passados
pesosDosProdutos[QTDPRODUTOS],                                    // Vetor para armazenar o peso sos produtos passados
numeroRandomico;                                                  // Variavel para armazenar o numero randomico criado pelo "sensor"

int simulandoSensor ();                                           // Funcao para simular a passagem de um objeto pelos sensores das esteiras
void *countProdutos (void *arg);                                  // Funcao que a thread de contagem ira executar para saber se a contagem ja finalizou, usando conditional variable
void* somandoProdutos(void * arg);                                // Funcao que as threads das esteiras irao executar para somar um produto ao total de produtos passados

int main()
{
  srand(time(NULL));                                              // Para que sempre que se rode o codigo seja um novo numero sorteado
  int indice,                                                     // Variavel usada para indexar nos vetores onde for necessario
  somaDosPesosDosProdutos = 0,                                    // Variavel para armazenar o resultado da soma dos pesos de todos os produtos passados pelas esteiras
  opcaoContinuar = 0,                                             // Variavel para saber se o programa executara novamente
  caracterLixo;                                                   // Variavel para pegar o lixo e nao ler lixo no seguinte scanf
  long id1 = 1, id2 = 2, id3 = 3, id4 = 4;                        // Variaveis para ids das threads
  clock_t startSoma, finishSoma,
          startEsteiras, finishEsteiras,
          startGeral, finishGeral;                                // Variaveis para obter os clocks passados
  double timeSoma, timeEsteiras, timeGeral;                       // Variavel para mostrar o tempo passado
  pthread_mutexattr_t mutexAttrPrioInherit;                       // Variavel para atributo do mutex
  int mutexProtocol;                                              // Variavel para receber o protocolo utilizado pelo mutex

  pthread_mutexattr_init(&mutexAttrPrioInherit);                  // inicializando a variavel de atributo do mutex
  pthread_mutexattr_getprotocol(&mutexAttrPrioInherit, &mutexProtocol);         // Obtendo que protocolo o mutex estava utilizando inicialmente
  if (mutexProtocol != PTHREAD_PRIO_INHERIT)                      // Caso o protocolo nao seja heranca de prioridade
  {
    pthread_mutexattr_setprotocol(&mutexAttrPrioInherit, PTHREAD_PRIO_INHERIT); // iremos definir que o protocolo a ser utilizado sera esse
  }

  startGeral = clock();

  do
  {
    if(pthread_mutex_init(&lock, NULL))                           // Inicializando o mutex, mas nao fazendo lock
    {
      printf("Nao foi possivel inicializar o mutex\n");
      return 6;
    }

    startEsteiras = clock();
    pthread_t threadEsteira[3], threadContadora;                  // Criando as variaveis que estarao anexadas as threads
    pthread_create(&threadEsteira[0], NULL, somandoProdutos, (void *) id1);     // Criando a thread 1, ira executar a funcao somandoProdutos
    pthread_create(&threadEsteira[1], NULL, somandoProdutos, (void *) id2);     // Criando a thread 2, ira executar a funcao somandoProdutos
    pthread_create(&threadEsteira[2], NULL, somandoProdutos, (void *) id3);     // Criando a thread 3, ira executar a funcao somandoProdutos
    pthread_create(&threadContadora, NULL, countProdutos, (void *) id4);        // Criando a thread encarregada de observar se ja chegamos ao objetivo, ira executar a funcao countProdutos

    while(quantidadeDeProdutosPassados < QTDPRODUTOS)            // Atualizando o display enquanto nao tenhamos atingido o objetivo
    {
      printf("Valor de quantidadeDeProdutosPassados: %d\n",quantidadeDeProdutosPassados);    // Printando o valor dos produtos ja passados
    }

    for(indice = 0; indice < 3; indice++)
    {
      pthread_join(threadEsteira[indice], NULL);                  // Esperando para que todas as threads das esteiras voltem
    }
    pthread_join(threadContadora, NULL);                          // Esperando para que a thread contadora volte
    finishEsteiras = clock();
    timeEsteiras = ((double) (finishEsteiras - startEsteiras)) / CLOCKS_PER_SEC;// Tempo para contar os 1500 produtos

    startSoma = clock();
    for(indice = 0; indice < QTDPRODUTOS; indice++)
    {
      somaDosPesosDosProdutos += pesosDosProdutos[indice];        // Somando os pesos dos  produtos que passaram pelas 3 esteiras
    }
    finishSoma = clock();
    timeSoma = ((double) (finishSoma - startSoma)) / CLOCKS_PER_SEC;            // Tempo para realizar a soma

    printf("Valor total do peso dos produtos: %d\n", somaDosPesosDosProdutos);  // Pritando o peso total
    printf("Quantidade de produtos: %d\n", quantidadeDeProdutosPassados);       // Mostrando a quantidade de produtos contados

    finishGeral = clock();
    timeGeral = ((double) (finishGeral - startGeral)) / CLOCKS_PER_SEC;
    //printf("Deseja executar novamente? (0 para parar e 1 para continuar).\n");//Perguntando se o programa ira executar novamente
    //scanf("%d", &opcaoContinuar);                               // Lendo input do teclado
    //while((c = getchar()) != '\n' && c != EOF){}                // Limpando buffer
    //if(opcaoContinuar == 1)                                     // Caso a condicao seja verdadeira,
    //{
    // quantidadeDeProdutosPassados = 0;                          // Iremos reiniciar a contagem
    // for(indice = 0; indice < QTDPRODUTOS; indice++)
    // {
    //   pesosDosProdutos[indice] = 0;                            // Reiniciar os pesos para evitar lixo de memoria
    // }
    //}
  }while(opcaoContinuar == 1);

  printf("Tempos para programa Multithread.\n");
  printf("Tempo em geral: %f\n",timeGeral);                        // Printando quanto tempo demorou
  printf("Tempo na soma: %f\n", timeSoma);
  printf("Tempo nas Esteiras: %f\n",timeEsteiras);
  pthread_mutex_destroy(&lock);                                   // Destruindo o Mutex
  pthread_cond_destroy(&contagemAtingida);                        // Destruindo a variavel condicional
  pthread_exit(NULL);                                             // Invocando o metodo de exit para todas as threads

  return 0;
}

int simulandoSensor ()                                            // Funcao para simular o sensor na esteira
{
  numeroRandomico = rand() + 1;                                   // Gerando um numero randomico qualquer
  if(numeroRandomico%2 == 0)                                      // Caso o numero seja par, significa que passou um objeto pelo sensor
  {
    return 1;
  }
  else                                                            // Caso contrario o objeto esta "errado"
  {
    return 0;
  }
}

void *countProdutos (void *arg)                                   // Trecho de codigo que a threadContadora ira executar
{
  long id = (long) arg;                                           // Variavel para armazenar o parametro

  printf("Starting countProduts(): thread %ld\n", id);            // Pritando para saber que estamos por aqui

  pthread_mutex_lock(&lock);                                      // Bloquendo a seguinte secao
  while(quantidadeDeProdutosPassados < QTDPRODUTOS)
  {
    pthread_cond_wait(&contagemAtingida, &lock);                  // Esperar pelo sinal que a contagem foi atingida
    printf("countProdutos(): thread %ld Condition signal received.\n", id);     // Informando sobre o recebimento do sinal
  }
  pthread_mutex_unlock(&lock);                                    // Desbloqueando secao critica
  pthread_exit(NULL);                                             // saindo da thread
}

void* somandoProdutos(void * arg)                                 // Trecho de codigo que as threadEsteira irao executar
{
  long id = (long) arg;                                           // Variavel para salvar o valor do argumento
  while(quantidadeDeProdutosPassados < QTDPRODUTOS)               // Executando enquanto nao tenhamos atingido o valor predeterminado
  {
    // timer
    if (simulandoSensor() == 1)                                   // Caso tenha retornado valor 1, ou seja, passou produto pelo sensor
    {
      pthread_mutex_lock(&lock);                                  // Bloquearemos a seguinte parte do codigo pois e a secao critica
      pesosDosProdutos[quantidadeDeProdutosPassados] = rand()%20 + 1;           // Peso aleatorio
      quantidadeDeProdutosPassados++;                             // Passou um produto, soma um na quantidade

      if(quantidadeDeProdutosPassados == QTDPRODUTOS)             // Caso tenhamos atingido o objetivo
      {
        pthread_cond_signal(&contagemAtingida);                   // Temos que avisar a threadContadora para finalizar o processo de contagem
        printf("somandoProdutos(): thread: %ld, QTD: %d    Atingimos marca\n", id, quantidadeDeProdutosPassados);
      }
      //printf("somandoProdutos(): thread: %ld, QTD: %d       Desbloqueando mutex\n", id, quantidadeDeProdutosPassados); // So para ilustrar quem esta contando no momento
      pthread_mutex_unlock(&lock);                                // Desbloqueando a secao critica
      // Mandando fazer algum trabalho para alternar no mutex
      //usleep(1);
    }
  }
  pthread_exit(NULL);
}
