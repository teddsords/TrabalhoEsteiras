#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define QTDPRODUTOS 1500
int simulandoSensor();
int main()
{
  srand(time(NULL));
  int i = 0, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, quantidadeDeProdutosPassados = 0,
       pesosDosProdutos[QTDPRODUTOS], numeroRandomico, c;
  clock_t startSoma, finishSoma,
       startEsteiras, finishEsteiras,
       startGeral, finishGeral;                                // Variaveis para obter os clocks passados
  double timeSoma, timeEsteiras, timeGeral;                       // Variavel para mostrar o tempo passado

  startGeral = clock();

  do
  {
    startEsteiras = clock();
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
    finishEsteiras = clock();
    timeEsteiras = ((double) (finishEsteiras - startEsteiras)) / CLOCKS_PER_SEC;

    startSoma = clock();
    for(i = 0; i < QTDPRODUTOS; i++)
    {
      somaDosPesosDosProdutos = somaDosPesosDosProdutos + pesosDosProdutos[i];
    }
    finishSoma = clock();
    timeSoma = ((double) (finishSoma - startSoma)) / CLOCKS_PER_SEC;
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

  finishGeral = clock();
  timeGeral = ((double) (finishGeral - startGeral))/CLOCKS_PER_SEC;

  printf("Tempos para programa Single thread.\n");
  printf("Tempo em geral: %f\n",timeGeral);                        // Printando quanto tempo demorou
  printf("Tempo na soma: %f\n", timeSoma);
  printf("Tempo nas Esteiras: %f\n",timeEsteiras);

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
