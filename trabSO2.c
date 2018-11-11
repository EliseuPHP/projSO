#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//tamanho do vetor
int count = 0;
// variaveis para controle da quantidade de threads criadas
int countT = 0, numT = 0;

typedef struct pos{
  int *arr;
  int start;
  int end;
} pos;

// Declaracao das funcoess
void * startMergeSort(void *pv);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void countPositions(int nPos, pos * pos, int * arr);

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
  pos pos[numT];

  countPositions(count, pos, arr);

  pthread_t thrd[numT];

  for (int i = 0; i < numT; i++) {
    printf("%d -- %d\n", pos[i].start, pos[i].end);
    pthread_create(&thrd[i], NULL, startMergeSort, &pos[i]);
  }

  for (int i = 0; i < numT; i++) {
    pthread_join(thrd[i], NULL);
  }

  mergeSort(arr, pos[0].start, pos[numT-1].end-1);

  FILE *p = fopen(argv[i], "w");
  for (size_t i = 0; i < count; i++) {
    fprintf(p, "%d\n", arr[i]);
  }
  return 0;
}

//***********************MERGESORT**********************************************

void merge(int arr[], int s, int m, int e){
  int i, j, k;
  int n1 = m - s + 1;
  int n2 =  e - m;

  int L[n1], R[n2];

  for (i = 0; i < n1; i++)
  L[i] = arr[s + i];
  for (j = 0; j < n2; j++)
  R[j] = arr[m + 1+ j];

  i = 0;
  j = 0;
  k = s;
  while (i < n1 && j < n2){
    if (L[i] <= R[j]){
      arr[k] = L[i];
      i++;
    }
    else{
      arr[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1){
    arr[k] = L[i];
    i++;
    k++;
  }


  while (j < n2){
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(int arr[], int s, int e){
  if (s < e){

    int m = s+(e-s)/2;

    mergeSort(arr, s, m);
    mergeSort(arr, m+1, e);

    merge(arr, s, m, e);
  }
}

// funcao para chamar o sort usando struct
void * startMergeSort(void *structPos){
  pos * pos = structPos;
  mergeSort(pos->arr, pos->start, pos->end-1);
  return pos;
}

void countPositions(int nPos, pos * pos, int * arr){
  int size = nPos/numT;
  for (int i = 0; i < numT; i++) {
    pos[i].start = i*size;
    pos[i].end = ((i+1)*size)-1;
    pos[i].arr = arr;
    if (i == numT-1) {
      pos[i].end = nPos;
    }
  }
  // for (int i = 0; i < numT; i++) {
  //   printf("%d -- %d\n", pos[i].start, pos[i].end);
  // }
  // printf("%d\n", nPos);
}
