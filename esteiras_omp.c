#include <stdio.h>
#include <omp.h>

#define QTDPRODUTOS 300

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

void somandoProdutos()
{
  while(quantidadeDeProdutosPassados < QTDPRODUTOS)
  {
    if (simulandoSensor() == 1)
    {
      if(quantidadeDeProdutosPassados < QTDPRODUTOS)
      {
        
        #pragma omp parallel reduction(+:quantidadeDeProdutosPassados,i)
        {
        	quantidadeDeProdutosPassados++;
        	pesosDosProdutos[i] = 1;
        	i++;
            
        	printf("I am locked with: %d.\n", omp_get_thread_num()); // tirar quando for tirar o tempo os 2
        	printf("Valor de I: %d\n",i);
        }
      }
    }

  }
}

int main(int argc, char **argv)
{

	omp_set_num_threads(3);			// usando 3 threads
	srand(time(NULL));

	somandoProdutos();
  
  int a, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, j, c;
	do{
		
	
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
      quantidadeDeProdutosPassados = 0;
      i = 0;
      
      for(a = 0; a < QTDPRODUTOS; a++)
      {
        pesosDosProdutos[a] = 0;
      }
      somandoProdutos();
    }
  }while(opcaoContinuar == 1);

  return 0;
}	
