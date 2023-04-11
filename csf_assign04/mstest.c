#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



int compare_i64(const void *a, const void *b) {
  int64_t l = *(int64_t *) a;
  int64_t r = *(int64_t *) b;
  if(l > r) {
    return 1;
  } else if (l < r) {
    return -1;
  } else {
    return 0;
  }
}


void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

int merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  // TODO: implement
  int64_t middle = begin + (end - 1) / 2;
  if (end + 1 <= threshold) {
    size_t size = sizeof(int64_t);
    qsort(arr, end + 1, size, compare_i64);
  }
  else {
    merge_sort(arr, begin, middle, threshold);
    merge_sort(arr, middle, end, threshold);

    int64_t temparr[end + 1];
    merge(arr, begin, middle, end, temparr);
    arr = temparr;
  }

}

void quicksort_test() {
  int64_t arrfill[] = {8, 7, 1, -2, -1, 4, 3, 6, 5, 9};
  size_t end =  8; //sizeof(arrfill)/sizeof(int);
  qsort(arrfill, end+1, sizeof(int64_t), compare_i64);
  // int* ans = merge_sort(&arrfill, 0, end, 5000);
  for (int i = 0; i < 8; i++) printf("%d ", arrfill[i]);
}

int main() {
  // quicksort_test();
  int64_t arrfill[] = {8, 7, 1, -2, -1, 4, 3, 6, 5, 9};
  size_t end =  8; 
  qsort(arrfill, end+1, sizeof(int64_t), compare_i64);
  int* ans = merge_sort(&arrfill, 0, end, 5000);
  for (int i = 0; i < 8; i++) printf("%d ", arrfill[i]);
  printf("\n");
  return 0;
}