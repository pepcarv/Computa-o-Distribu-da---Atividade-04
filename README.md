# ATIVIDADE 4 - Média com `MPI_Reduce`

**Alunos:**
- Pedro Henrique Carvalho Pereira - 10418861
- Mateus Ribeiro Cerqueira - 10443901

## Funcionamento

Cada processo gera um vetor local de `N` números aleatórios em [0,1], calcula a soma e a média local (função `calcula_media`) e imprime o resultado. Depois, `MPI_Reduce` soma todas as somas locais no processo de rank 0, que calcula e exibe a soma global e a média global (`soma_global / (N * número de processos)`).

## Compilação e execução

```bash
mpicc -o media_mpi media_mpi.c
mpirun -np 4 ./media_mpi 1000
```

## Como a ordem da saída foi mantida

Como cada processo imprime de forma independente, as linhas podiam sair embaralhadas no terminal. A solução foi:

1. Um laço `for (r = 0; r < tam; r++)` em que só o processo `rank == r` imprime na sua vez.
2. `MPI_Barrier` ao final de cada volta, para os demais esperarem.
3. `fflush(stdout)` seguido de `usleep(20000)` (20 ms), para a linha chegar ao terminal antes de o próximo processo imprimir. Apenas o `MPI_Barrier` não foi suficiente, pois o `mpirun` ainda podia repassar as linhas fora de ordem.

## Alguns exemplos de execução

`mpirun -np 4 ./media_mpi 1000`

**Exec 1**
```
[Processo 0] Soma local: 489.970, Média local: 0.4900
[Processo 1] Soma local: 502.906, Média local: 0.5029
[Processo 2] Soma local: 496.537, Média local: 0.4965
[Processo 3] Soma local: 485.594, Média local: 0.4856

[Soma global] 1975.008
[Média global] 0.4938
```

**Exec 2**
```
[Processo 0] Soma local: 485.594, Média local: 0.4856
[Processo 1] Soma local: 488.582, Média local: 0.4886
[Processo 2] Soma local: 493.373, Média local: 0.4934
[Processo 3] Soma local: 496.257, Média local: 0.4963

[Soma global] 1963.806
[Média global] 0.4910
```

**Exec 3**
```
[Processo 0] Soma local: 496.257, Média local: 0.4963
[Processo 1] Soma local: 504.895, Média local: 0.5049
[Processo 2] Soma local: 498.205, Média local: 0.4982
[Processo 3] Soma local: 521.813, Média local: 0.5218

[Soma global] 2021.170
[Média global] 0.5053
```

**Exec 4**
```
[Processo 0] Soma local: 504.895, Média local: 0.5049
[Processo 1] Soma local: 498.205, Média local: 0.4982
[Processo 2] Soma local: 521.813, Média local: 0.5218
[Processo 3] Soma local: 492.297, Média local: 0.4923

[Soma global] 2017.210
[Média global] 0.5043
```
