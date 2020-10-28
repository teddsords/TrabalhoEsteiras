#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define QTDPRODUTOS 5000
int simulandoSensor();
int main()
{
  srand(time(NULL));
  int i = 0, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, quantidadeDeProdutosPassados = 0,
       pesosDosProdutos[QTDPRODUTOS], numeroRandomico, c;

  clock_t start, finish;
  double time;

  start = clock();

  do
  {
    while(quantidadeDeProdutosPassados < QTDPRODUTOS)
    {
      if(simulandoSensor() == 1)
      {
        quantidadeDeProdutosPassados++;
        pesosDosProdutos[i] = 1;
        i++;
        printf("Quantidade de Produtos ja passados pelas esteiras: %d\n",quantidadeDeProdutosPassados);
      }
    }

    for(i = 0; i < QTDPRODUTOS; i++)
    {
      somaDosPesosDosProdutos = somaDosPesosDosProdutos + pesosDosProdutos[i];
    }
    printf("Valor total do peso dos produtos: %d\n",somaDosPesosDosProdutos);

    // printf("Deseja executar novamente? (0 para parar e 1 para continuar)\n");
    // scanf("%d\n",&opcaoContinuar);
    // while((c = getchar()) != '\n' && c != EOF){}
    //
    // if(opcaoContinuar == 1)
    // {
    //   quantidadeDeProdutosPassados = 0;
    //   for(i = 0; i < QTDPRODUTOS; i++)
    //   {
    //     pesosDosProdutos[i] = 0;
    //   }
    // }
    //
  }while(opcaoContinuar == 1);
  
  finish = clock();
  time = ((double) (finish - start))/CLOCKS_PER_SEC;
  printf("Demorou isto: %f\n",time );
  return 0;
}

int simulandoSensor ()
{
  int numeroRandomico = rand() + 1;
  if(numeroRandomico%2 == 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
