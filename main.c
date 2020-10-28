#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QTDPRODUTOS 800
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
  while(quantidadeDeProdutosPassados < QTDPRODUTOS)
  {
    if (simulandoSensor() == 1)
    {
      if(quantidadeDeProdutosPassados < QTDPRODUTOS)
      {
        pthread_mutex_lock(&lock);
        quantidadeDeProdutosPassados++;
        pesosDosProdutos[i] = 1;
        i++;
        pthread_mutex_unlock(&lock);
        printf("I am locked with: %lu.\n", pthread_self());
        printf("Valor de I: %d\n",i);
      }
    }

  }
  return NULL;
}

int main()
{
  srand(time(NULL));
  int a, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, j, c;

  do
  {
    pthread_t pthEsteira1, pthEsteira2, pthEsteira3;
    pthread_attr_t attr;
    pthread_mutexattr_t mutexAttrPrioInherit;
    int mutexProtocol;

    pthread_mutexattr_init(&mutexAttrPrioInherit);
    pthread_mutexattr_getprotocol(&mutexAttrPrioInherit, &mutexProtocol);
    if (mutexProtocol != PTHREAD_PRIO_INHERIT)
    {
      pthread_mutexattr_setprotocol(&mutexAttrPrioInherit, PTHREAD_PRIO_INHERIT);
      printf("Nao estava com heranca de prioridade.\n"); // apaga depois
    }

    if(pthread_mutex_init(&lock, NULL))
    {
      printf("Deu SERIAMENTE MERDA");
      return 6;
    }
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    //while(quantidadeDeProdutosPassados < QTDPRODUTOS)
    //{
    if(pthread_create(&pthEsteira1, &attr, somandoProdutos, NULL))
    {
      fprintf(stderr, "Error creating thread 2\n");
      return 1;
    }
    if(pthread_create(&pthEsteira2, &attr, somandoProdutos, NULL))
    {
      fprintf(stderr, "Error creating thread 2\n");
      return 2;
    }

    if(pthread_create(&pthEsteira3, &attr, somandoProdutos, NULL))
    {
      fprintf(stderr, "Error creating thread 3\n");
      return 3;
    }

    pthread_attr_destroy(&attr);
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
  //}


    printf("Valor de quantidade de produtos passados: %d\n", quantidadeDeProdutosPassados);

    //iniciar o timer
    for(a = 0; a < QTDPRODUTOS; a++)
    {
      somaDosPesosDosProdutos = somaDosPesosDosProdutos + pesosDosProdutos[a];
    }
    // finalizar o timer
    printf("Valor total do peso dos produtos: %d\n", somaDosPesosDosProdutos);

    printf("Deseja executar novamente? (0 para parar e 1 para continuar).\n");
    scanf("%d", &opcaoContinuar);
    while((c = getchar()) != '\n' && c != EOF){}

    if(opcaoContinuar == 1)
    {
      printf("Zerando as paradas\n");
      quantidadeDeProdutosPassados = 0;
      i = 0;
      pthread_mutex_destroy(&lock);
      for(a = 0; a < QTDPRODUTOS; a++)
      {
        pesosDosProdutos[a] = 0;
      }
    }
    printf("Valor do J: %d\n",j);
    j++;
  }while(opcaoContinuar == 1);

  pthread_exit(NULL);

  return 0;
}
