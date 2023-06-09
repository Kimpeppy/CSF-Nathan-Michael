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

// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
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

  size_t length = end - begin;
  int64_t mid = (begin + end) / 2;

  if (length <= threshold) {
    size_t size = sizeof(int64_t);
    qsort(arr+begin, (end-begin), size, compare_i64);
  }
  else {
   pid_t pidLeft = fork();
    if(pidLeft == 0) {
      int retcode = merge_sort(arr, begin, mid, threshold);
      exit(retcode); //since these two are children, they need to exit
    }


    if(pidLeft == -1) {
      //print error
      fprintf(stderr, "Error: Left child process couldn't be created");
      return -1;
    }


    pid_t pidRight = fork();
    if(pidRight == 0) {
      int retcode = merge_sort(arr, mid, end, threshold);
      exit(retcode); //since these two are children, they need to exit
    }


    if(pidRight == -1) {
      //print error
      fprintf(stderr, "Error: Right child process couldn't be created");
      return -1;
    }


    int wstatusLeft;
    pid_t actual_pidLeft = waitpid(pidLeft, &wstatusLeft, 0);
    if(actual_pidLeft == -1) {
      fprintf(stderr, "Error: Left side waitpid failure!");
      return -1; //will return 1
    }


    if(!WIFEXITED(wstatusLeft)) {
    // subprocess crashed, was interrupted, or did not exit normally
    // handle as error
      fprintf(stderr, "Error: Left subprocess crashed, was interrupted, or did not exit normally.");
      return -1; //will return 1
    }

    if (WEXITSTATUS(wstatusLeft) != 0) {
      fprintf(stderr, "Error: subprocess returned an error.");
      return -1;
      // subprocess returned a non-zero exit code
      // if following standard UNIX conventions, this is also an error
    }

    int wstatusRight;
    pid_t actual_pidRight = waitpid(pidRight, &wstatusRight, 0);
    if(actual_pidRight == -1) {
      fprintf(stderr, "Error: Right side waitpid failure!");
      return -1;
    }

    if(!WIFEXITED(wstatusRight)) {
      fprintf(stderr, "Error: Right subprocess crashed, was interrupted, or did not exit normally.");
      return -1;
    // subprocess crashed, was interrupted, or did not exit normally
    // handle as error
    }

    if (WEXITSTATUS(wstatusRight) != 0) {
      fprintf(stderr, "Error: Right subprocess returned an error");
      return -1;
    // subprocess returned a non-zero exit code
    // if following standard UNIX conventions, this is also an error
    }

    
    int64_t *tempArr = malloc(length << 3);
    merge(arr, begin, mid, end, tempArr);

    memcpy(arr + begin, tempArr, length << 3); //length = begin - end
  }
  return 0;
}



int main(int argc, char **argv) {
  // check for correct number of command line arguments

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2])) {
    /* TODO: report an error (threshold value is invalid) */
    fprintf(stderr, "Error: Invalid threshold\n");
    return -1;
  }
    

  // TODO: open the file
  int fd = open(filename, O_RDWR);
  if (fd < 0) {
    fprintf(stderr, "Error: Failed to open file\n");
    return -1;
  }  

  // TODO: use fstat to determine the size of the file
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "Error: fstat failed\n");
    return -1;
  }

  size_t file_size_in_bytes = statbuf.st_size;

  // TODO: map the file into memory using mmap
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);

  if (data == MAP_FAILED) {
      fprintf(stderr, "Error: MMap failed\n");
      return -1;
  }


  // TODO: sort the data!
  size_t length = file_size_in_bytes/8;
  int error_code;
  error_code = merge_sort(data, 0, length, threshold);
  // TODO: unmap and close the file
  int unmap = munmap(data, file_size_in_bytes);
  close(fd); //<this might be redundant
  
  if(unmap != 0) {
    fprintf(stderr, "Error: munmap failed\n");
  }
  return error_code;
}



 /*
merge_sort(arr, begin, middle, threshold);
    pid_t pid = fork();
    if(pid == 0) {
      merge_sort(arr, begin, middle, threshold);
      exit(0);
    }
    else if (pid == -1) {
      fprintf(stderr, "Fork failed!\n");
      exit(1);
    }
    else {
      int status;
      waitpid(pid, &status, 0);
    }
    pid = fork();
    if(pid == 0) {
      merge_sort(arr, begin, middle, threshold);
      exit(0);
    }
    else if (pid == -1) {
      fprintf(stderr, "Fork failed!\n");
      exit(1);
    }
    else {
      int status;
      waitpid(pid, &status, 0);
    }

    int64_t temparr[end + 1];
    merge(arr, begin, middle, end, temparr);
    arr = temparr;
  */