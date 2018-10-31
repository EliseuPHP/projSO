#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//tamanho do vetor
int count = 0;
// variaveis para controle da quantidade de threads criadas
int countT = 0, numT = 0;

typedef struct Params{
  int *arr;
  size_t len;
  int depth;
} Params;

// nao bugar os printf
pthread_mutex_t print = PTHREAD_MUTEX_INITIALIZER;

// Declaracao das funcoes
void merge(int *arr, int *mid, int *end);
void mergeSort(int *arr, size_t len, int depth);
void *mergeSortStruct(void *pv);
void mergeSortIni(int *arr, size_t len);

//zapkk
int main(int argc, char const *argv[]) {


  numT = atoi(argv[1]);
  //printf("%d\n", numT);
  int *arr;
  int i, j = 0;
  for (i = 2; i < argc-1; i++) {

    char teste[11];
    //printf("filename: %s\n", argv[i]);
    FILE *p = fopen(argv[i],"r");
    int t = 0;
    while (t != 1)
    {
      fscanf(p,"%s\n", teste);
      if (feof(p)) {
        t = 1;
      }
      count++;
    }
    //printf("%d\n", count);
    rewind(p);
    if (i == 2) {
      //printf("malloc\n");
      arr = (int *) malloc(count * sizeof(int));
    } else {
      //printf("realloc\n");
      arr = (int *) realloc(arr, count * sizeof(int));
    }

    for (; j < count; j++) {
      fscanf(p,"%d\n", &arr[j]);
      //printf("%d\n", arr[j]);
    }
    fclose(p);
  }
  //printf("\n%s\n\n", "fim");

  mergeSortIni(arr, count);

  FILE *p = fopen(argv[i], "w");
  for (size_t i = 0; i < count; i++) {
    fprintf(p, "%d\n", arr[i]);
  }
  return 0;
}

//***********************MERGESORT**********************************************

void merge(int *arr, int *mid, int *end){
  int *res = malloc((end - arr)*sizeof(*res));
  int *lhs = arr, *rhs = mid, *dst = res;

  while (lhs != mid && rhs != end){
    if (*lhs < *rhs) {
      *dst++ = *lhs++;
    } else {
      *dst++ = *rhs++;
    }
  }

  while (lhs != mid){
    *dst++ = *lhs++;
  }

  memcpy(arr, res, (rhs - arr) * sizeof *res);
  free(res);
}

void mergeSort(int *arr, size_t len, int depth){
  if (len < 2){
    return;
  }


  if (depth <= 0 || len < 4){
    mergeSort(arr, len/2, 0);
    mergeSort(arr+len/2, len-len/2, 0);
  }else{
    pthread_mutex_lock(&print);
    printf("%d e menor que %d?: ", countT, numT);
    pthread_mutex_unlock(&print);
    //criar quantidade de threads informadas pelo usuario
    if (countT < numT) {
      pthread_mutex_lock(&print);
      printf("sim\n");
      countT++;
      pthread_mutex_unlock(&print);

      Params left = { arr, len/2, depth/2 };

      pthread_t thrd;

      pthread_mutex_lock(&print);
      printf("Inicializando thead n: %d\n", countT);
      pthread_mutex_unlock(&print);

      pthread_create(&thrd, NULL, mergeSortStruct, &left);
      //recursividade
      mergeSort(arr+len/2, len-len/2, depth/2);

      pthread_join(thrd, NULL);
    } else{
      pthread_mutex_lock(&print);
      printf("nao\n");
      pthread_mutex_unlock(&print);

      mergeSort(arr, len/2, depth/2);
      mergeSort(arr+len/2, len-len/2, depth/2);
    }
  }
  //envia o endedreco inicial do vetor, endereco do meio e endereco final
  merge(arr, arr+len/2, arr+len);
}

// funcao recursiva para ler o struct
void *mergeSortStruct(void *pv){
  Params *params = pv;
  mergeSort(params->arr, params->len, params->depth);
  return pv;
}

void mergeSortIni(int *arr, size_t len){
  mergeSort(arr, len, numT);
}
