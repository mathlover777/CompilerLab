#include <stdio.h>
#define MAX 20

void read(int [], int);
void print(int [], int);
void sort(int [], int);
void swap(int*, int*);

int main() {
  int a[MAX], len;

  printf("Enter number of elements: ");
  scanf("%d", &len);

  read(a, len); 
  sort(a, len);
  print(a, len);

  return 0; 
}

void read(int a[], int len) {
	int i;
	printf("Enter elements one by one\n");
  for (i = 0; i < len; ++i)
  	scanf("%d", &a[i]);
}

void sort(int a[], int len) {
	int i, j, min;
	for (i = 0; i < len - 1; ++i) {
		min = i;
		for (j = i + 1; j < len; ++j) {
			if (a[j] < a[min]) {
				min = j;
			}
		}
		if (min != i) {
			swap(&a[min], &a[i]);
		}
	}
}

void swap(int *a, int *b) {
	int t;
	t = *a;
	*a = *b;
	*b = t;
}

void print(int a[], int len) {
	int i;
	printf("Sorted Array:\n");
	for (i = 0; i < len; ++i)
		printf("%d\n", a[i]);
}
