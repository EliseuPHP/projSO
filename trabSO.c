#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int count = 0;
int numT = 0;

void swap(int* a, int* b){
  int t = *a;
  *a = *b;
  *b = t;
}

int partition (int arr[], int low, int high){
  int pivot = arr[high];
  int i = (low - 1);

  for (int j = low; j <= high- 1; j++){
    if (arr[j] <= pivot){
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return (i + 1);
}

void quickSort(void * arr, int low, int high){
  if (low < high){

    int pi = partition(arr, low, high);

    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

void* tQuickSort(void * arr){

  int low = 0;
  int high = count-1;

  if (low < high){

    int pi = partition(arr, low, high);

    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
  pthread_exit(0);
}

//zapkk
int main(int argc, char const *argv[]) {


  numT = atoi(argv[1]);
  printf("%d\n", numT);
  int *ord;
  int i, j = 0;
  pthread_t tid[numT];
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
      ord = (int *) malloc(count * sizeof(int));
    } else {
      //printf("realloc\n");
      ord = (int *) realloc(ord, count * sizeof(int));
    }

    for (; j < count; j++) {
      fscanf(p,"%d\n", &ord[j]);
      //printf("%d\n", ord[j]);
    }
    fclose(p);
  }
  //printf("\n%s\n\n", "fim");
  for (int i = 0; i < numT; i++) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid[i], &attr, tQuickSort, ord);
  }

  printf("%s\n", "passou o create");

  for (int i = 0; i < numT; i++){
    printf("%d\n", i);
    pthread_join(tid[i], NULL);
  }

  printf("%s\n", "passou o join");

  // for (size_t i = 0; i < j; i++) {
  //   printf("%d\n", ord[i]);
  // }

  FILE *p = fopen(argv[i], "w");
  for (size_t i = 0; i < count; i++) {
    fprintf(p, "%d\n", ord[i]);
  }
  return 0;
}
