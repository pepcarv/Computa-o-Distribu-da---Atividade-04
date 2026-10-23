/*
Alunos:
- Pedro Henrique Carvalho Pereira - 10418861
- Mateus Ribeiro Cerqueira - 10443901

Compilação e execução:
mpicc -o media_mpi media_mpi.c
mpirun -np 4 ./media_mpi 1000

OBS: mesma solução do soma_quadrados para manter a saída na ordem dos ranks:
cada processo imprime na sua vez (controlado por MPI_Barrier), e o fflush com
usleep dá tempo para a linha chegar ao terminal antes do próximo processo imprimir.

Abaixo, as mesmas referências extra aula utilizadas na última atividade:
- https://www.geeksforgeeks.org/c/sum-of-an-array-using-mpi/
- https://stackoverflow.com/questions/15658145/how-to-share-work-roughly-evenly-between-processes-in-mpi-despite-the-array-size
- https://stackoverflow.com/questions/17570996/mpi-printing-in-an-order
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // para usleep
#include <mpi.h>

// calculo da média (usada na local e na global)
double calcula_media(double soma, long quantidade) {
    return soma / quantidade;
}



int main(int argc, char *argv[]) {

    //init
    int rank;
    int tam;



    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tam);



    // entrada: um único argumento inteiro (N)
    if (argc != 2 || atoi(argv[1]) <= 0) {
        if (rank == 0) {
            printf("[Erro] Uso: mpirun -np <processos> %s <N>\n", argv[0]);
        }

        MPI_Finalize();
        return 1;
    }

    int N = atoi(argv[1]);



    // 1 cada processo com seu vetor de valores aleatórios entre 0 e 1
    srand(time(NULL) + rank);

    float *local = malloc(N * sizeof(float));
    for (int i = 0; i < N; i++)
        local[i] = rand() / (float)RAND_MAX;



    // 2 soma local e média local
    double soma_local = 0;
    for (int i = 0; i < N; i++)
        soma_local += local[i];

    double media_local = calcula_media(soma_local, N);



    // ========== resutls ============
    // cada rank imprime a sua soma e média local em ordem
    for (int r = 0; r < tam; r++) {
        if (rank == r) {
            printf("[Processo %d] Soma local: %.3f, Média local: %.4f\n", rank, soma_local, media_local);
            
            
            fflush(stdout);
            usleep(20000); // 20 ms pro mpirun repassar a linha
        }

        // barrier pra sinc
        MPI_Barrier(MPI_COMM_WORLD);
    }



    // 3 reduce pro root (soma global)
    double soma_global = 0;
    MPI_Reduce(&soma_local, &soma_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);



    // 4 root calcula e exibe soma e média global
    if (rank == 0) {
        double media_global = calcula_media(soma_global, (long)N * tam);

        
        
        
        printf("\n[Soma global] %.3f\n", soma_global);
        printf("[Média global] %.4f\n", media_global);
    }

    
    
    // free
    free(local);

    MPI_Finalize();
    return 0;
}