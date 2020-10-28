#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QTDPRODUTOS 1500
pthread_mutex_t lock;


int i = 0, quantidadeDeProdutosPassados = 0, pesosDosProdutos[QTDPRODUTOS], numeroRandomico;

int simulandoSensor ()
{
  numeroRandomico = rand() + 1;
  if(numeroRandomico%2 == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void* somandoProdutos(void * arg)
{
  long id = (long) arg;
  while(quantidadeDeProdutosPassados < QTDPRODUTOS)
  {
    // timer
    if (simulandoSensor() == 1)
    {
      //timer finaliza
      pthread_mutex_lock(&lock);
      if(quantidadeDeProdutosPassados < QTDPRODUTOS)
      {
        pesosDosProdutos[quantidadeDeProdutosPassados] = rand()%20 + 1;
        quantidadeDeProdutosPassados++;
        //printf("Thread Number: %ld\n",id);
      }
      pthread_mutex_unlock(&lock);
    }
  }
  return NULL;
}

int main()
{
  srand(time(NULL));
  int a, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, c;
  long id1 = 1, id2 = 2, id3 = 3;
  pthread_t pthEsteira1, pthEsteira2, pthEsteira3;
  pthread_mutexattr_t mutexAttrPrioInherit;
  int mutexProtocol;

  pthread_mutexattr_init(&mutexAttrPrioInherit);
  pthread_mutexattr_getprotocol(&mutexAttrPrioInherit, &mutexProtocol);
  if (mutexProtocol != PTHREAD_PRIO_INHERIT)
  {
    pthread_mutexattr_setprotocol(&mutexAttrPrioInherit, PTHREAD_PRIO_INHERIT);
    printf("Nao estava com heranca de prioridade.\n"); // apaga depois
  }

  clock_t start, finish;
  double time;


  start = clock();

  do
  {
    if(pthread_mutex_init(&lock, NULL))
    {
      printf("Deu SERIAMENTE MERDA");
      return 6;
    }
//timer
    if(pthread_create(&pthEsteira1, NULL, somandoProdutos, (void*) id1))
    {
      fprintf(stderr, "Error creating thread 2\n");
      return 1;
    }
    if(pthread_create(&pthEsteira2, NULL, somandoProdutos, (void*) id2))
    {
      fprintf(stderr, "Error creating thread 2\n");
      return 2;
    }

    if(pthread_create(&pthEsteira3, NULL, somandoProdutos, (void*) id3))
    {
      fprintf(stderr, "Error creating thread 3\n");
      return 3;
    }

    while(quantidadeDeProdutosPassados < QTDPRODUTOS)
    {
      printf("Valor de quantidadeDeProdutosPassados: %d\n",quantidadeDeProdutosPassados);
    }


    if(pthread_join(pthEsteira1,NULL))
    {
      fprintf(stderr, "Error joining thread 2\n");
      return 4;
    }

    if(pthread_join(pthEsteira2,NULL))
    {
      fprintf(stderr, "Error joining thread 2\n");
      return 5;
    }

    if(pthread_join(pthEsteira3, NULL))
    {
      fprintf(stderr, "Error joining thread 3\n");
      return 6;
    }
    //final timer

    //printf("Valor de quantidadeDeProdutosPassados: %d\n",quantidadeDeProdutosPassados);

    //printf("Valor de quantidade de produtos passados: %d\n", quantidadeDeProdutosPassados);

    //iniciar o timer
    for(a = 0; a < QTDPRODUTOS; a++)
    {
      somaDosPesosDosProdutos += pesosDosProdutos[a];
    }
    // finalizar o timer
    printf("Valor total do peso dos produtos: %d\n", somaDosPesosDosProdutos);

    // printf("Deseja executar novamente? (0 para parar e 1 para continuar).\n");
    // scanf("%d", &opcaoContinuar);
    // while((c = getchar()) != '\n' && c != EOF){}
    //
    // if(opcaoContinuar == 1)
    // {
    //   printf("Zerando as paradas\n");
    //   quantidadeDeProdutosPassados = 0;
    //   i = 0;
    //   pthread_mutex_destroy(&lock);
    //   for(a = 0; a < QTDPRODUTOS; a++)
    //   {
    //     pesosDosProdutos[a] = 0;
    //   }
    // }
  }while(opcaoContinuar == 1);

  finish = clock();
  time = ((double) (finish - start))/CLOCKS_PER_SEC;
  printf("Demorou isto: %f\n",time );

  pthread_exit(NULL);

  return 0;
}
