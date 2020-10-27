#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define QTDPRODUTOS 800
pthread_mutex_t lock;

int i = 0, quantidadeDeProdutosPassados = 0, pesosDosProdutos[QTDPRODUTOS];
void* somandoProdutos(void * arg)
{
   while(quantidadeDeProdutosPassados < QTDPRODUTOS)
   {
    pthread_mutex_lock(&lock);
    if (quantidadeDeProdutosPassados < QTDPRODUTOS)
    {
      quantidadeDeProdutosPassados++;
      pesosDosProdutos[i] = 1;
      i++;
      printf("I am locked with: %lu.\n", pthread_self());
      printf("Valor de I: %d\n",i);
    }
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main()
{

  int a, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, j, c;

  do
  {
    pthread_t pthEsteira1, pthEsteira2, pthEsteira3;
    if(pthread_mutex_init(&lock, NULL))
    {
      printf("Deu SERIAMENTE MERDA");
      return 6;
    }

    if(pthread_create(&pthEsteira1, NULL, somandoProdutos, NULL))
    {
      fprintf(stderr, "Error creating thread 2\n");
      return 1;
    }
    if(pthread_create(&pthEsteira2, NULL, somandoProdutos, NULL))
    {
      fprintf(stderr, "Error creating thread 2\n");
      return 2;
    }

    if(pthread_create(&pthEsteira3, NULL, somandoProdutos, NULL))
    {
      fprintf(stderr, "Error creating thread 3\n");
      return 3;
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


    printf("Valor de quantidade de produtos passados: %d\n", quantidadeDeProdutosPassados);

    for(a = 0; a < QTDPRODUTOS; a++)
    {
      somaDosPesosDosProdutos = somaDosPesosDosProdutos + pesosDosProdutos[a];
    }
    printf("Valor total do peso dos produtos: %d\n", somaDosPesosDosProdutos);

    printf("Deseja executar novamente? (0 para parar e 1 para continuar).\n");
    scanf("%d", &opcaoContinuar);
    while((c = getchar()) != '\n' && c != EOF){}

    if(opcaoContinuar == 1)
    {
      printf("Zerando as paradas\n");
      somaDosPesosDosProdutos = 0;
      quantidadeDeProdutosPassados = 0;
      a = 0;
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

  return 0;
}
