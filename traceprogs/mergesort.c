#include <stdio.h>
#define MAX 1000

int a[MAX];
int b[MAX];

void merging(int low, int mid, int high) {
	int l1, l2, i;

	for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high; i++) {
      if(a[l1] <= a[l2])
         b[i] = a[l1++];
      else
         b[i] = a[l2++];
	}
   
	while(l1 <= mid)    
      	b[i++] = a[l1++];

   	while(l2 <= high)   
      	b[i++] = a[l2++];

   	for(i = low; i <= high; i++)
      	a[i] = b[i];
}

void sort(int low, int high) {
   	int mid;
   
   	if(low < high) {
      	mid = (low + high) / 2;
      	sort(low, mid);
      	sort(mid+1, high);
      	merging(low, mid, high);
   	} else { 
      	return;
   	}   
}

int main() { 
	int i;

	printf("Generating list from 999-0\n");
	for (i = 0; i < MAX; i++) {
		a[i] = MAX - i;
	}

	printf("Sorting...\n");
	sort(0, MAX);

	printf("Done!\n");
	return 0;
}
