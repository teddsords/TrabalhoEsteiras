/*
    Arquivo: esteiras_omp.c
    Autores: Leonardo Luis Grando e Teddy Ordoñez Nuñez
    Função do arquivo: Mostrar o funcionamento do problema das esteiras utilizando a biblioteca de programação paralela OpenMP
    Criado em 26 de outubro de 2020
    Modificado em 28 de outubro de 2020
*/

#include <stdio.h>
#include <omp.h>
#include <time.h>

#define QTDPRODUTOS 1500			// quantidade de produtos pra parar e fazer a contagem

int i = 0, quantidadeDeProdutosPassados = 0, pesosDosProdutos[QTDPRODUTOS], numeroRandomico=0;
clock_t antes, depois, tAntes, tDepois; // variaveis para contar o tempo

	
int simulandoSensor ()											  // função que simula o sensor
{
	
	numeroRandomico = rand()%2 + 1;                                // Gerando um numero randomico qualquer
  if(numeroRandomico%2 == 0)                                      // Caso o numero seja par, significa que passou um objeto pelo sensor
  {
    return 1;
  }
  else                                                            // Caso contrario o objeto esta "errado"
  {
    return 0;
  }
	
}

int main(int argc, char **argv)
{
	
	srand(time(NULL));				

  
  int a, somaDosPesosDosProdutos = 0, opcaoContinuar = 0, j, c;
	do{
	tAntes = clock();							// pegando inicio p tempo total
	antes = clock();							// medindo tempo antes p esteiras
	//omp_set_dynamic(0);
	printf("numero de threads antes pragma: %d \n", omp_get_num_threads());		// teste pra ver quantas threads tem
    		
	#pragma omp parallel shared (i,quantidadeDeProdutosPassados,pesosDosProdutos) num_threads(4)		// inicia as threads
    {
    		printf("numero de threads dentro do parallel: %d \n", omp_get_num_threads());	// teste pra ver quantas threads tem
    		
 			#pragma omp for  		
    		for (int o = 0; o<4; o++)
    		{
    		printf("inicia threads:%d \n", omp_get_thread_num());	
			}     					
    		
    		if (omp_get_thread_num() == 3 || omp_get_thread_num() == 1 || omp_get_thread_num()==2)
    		{
    		
    				
				while(quantidadeDeProdutosPassados < QTDPRODUTOS) 			//simula ate chegar no item 1500
				{
  					if (simulandoSensor() == 1)								// se o sensor for acionado
    				{		
        				#pragma omp critical								// abre a sessão critica
        				{
        				
        					quantidadeDeProdutosPassados++;					// incrementa a variavel de produtos
        					pesosDosProdutos[i] = 1; // rand()%5 +1			// gera um numero randomico para o peso do produto			
        					i++;											// incrementa contador do peso
        				}
    				}
				}													// chama funcao principal das esteiras que e onde acontecem os calculos
  	
  		
}

    		else																// entra no else quando for a main thread, que é a do display
    		{
    		while(quantidadeDeProdutosPassados < QTDPRODUTOS) 					//  display: printa os produtos ate 1500
			{	
			 	if(quantidadeDeProdutosPassados > 0)							// printa a partir de quando começar a contagem 
				{ 						
        			printf("Numero do produto: %d \n",quantidadeDeProdutosPassados);	// print produtos passados
        			
				}
  			}
  			
    	}
    	printf("Termina thread: %d \n", omp_get_thread_num());				// print terminando as threads
  }  		
  
  	 printf("numero de threads fora do pragma: %d \n", omp_get_num_threads());	// teste pra ver quantas threads tem
    
	depois = clock();															// medindo tempo depois p esteiras
  	printf("Tempo esteiras: %f \n", ((double)  (depois-antes))/ CLOCKS_PER_SEC);
	
	  printf("Valor de quantidade de produtos passados: %d\n", quantidadeDeProdutosPassados);

    antes = clock();					// medindo tempo antes p pesos
    for(a = 0; a < QTDPRODUTOS; a++)	// laço pra somar pesos
    {
      somaDosPesosDosProdutos += pesosDosProdutos[a];

    }
    depois = clock();					// medindo tempo depois p pesos
    
    
    
    printf("Tempo pesos: %f \n", ((double)  (depois-antes))/ CLOCKS_PER_SEC);
    printf("Valor total do peso dos produtos: %d\n", somaDosPesosDosProdutos);
    tDepois = clock();						// aqui termina o tempo total
    printf("Tempo total %f \n", ((double)  (tDepois-tAntes))/ CLOCKS_PER_SEC);
    
    printf("Deseja executar novamente? (0 para parar e 1 para continuar).\n");
    scanf("%d", &opcaoContinuar);
    while((c = getchar()) != '\n' && c != EOF){}

    if(opcaoContinuar == 1)						// opcao pra rodar de novo
    {
      quantidadeDeProdutosPassados = 0;			// zerando as variaveis globais
      i = 0;
      
      for(a = 0; a < QTDPRODUTOS; a++)			// zerando o vetor de pesos
      {
        pesosDosProdutos[a] = 0;
      }
      
    }
  }while(opcaoContinuar == 1);
  
  return 0;
}	
