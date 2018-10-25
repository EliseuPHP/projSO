#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high){
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++){
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot){
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high){
    if (low < high){
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

//zapkk
int main(int argc, char const *argv[]) {
  int numT = 0;
  //printf("%d\n", argc);
  numT = atoi(argv[1]);
  int *ord, count = 0;
  int i, j = 0;
  for (i = 2; i < argc-1; i++) {

    char teste[11];
    //printf("filename: %s\n", argv[i]);
    FILE *p = fopen(argv[i],"r");
    int t = 0;
    while (t != 1) // expect 1 successful conversion
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

  quickSort(ord, 0, count-1);

  //for (size_t i = 0; i < j; i++) {
  //  printf("%d\n", ord[i]);
  //}

  FILE *p = fopen(argv[i], "w");
  for (size_t i = 0; i < count; i++) {
    fprintf(p, "%d\n", ord[i]);
  }
  return 0;
}
