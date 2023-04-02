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

static int compare_i64(const void *left, const void *right){
 int64_t l = *(int64_t*) left;
 int64_t r = *(int64_t*) right;
 
 if(l < r){
  return -1;
  } 
  if(l > r){
  return 1;
  }else {
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


void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
    assert(end >= begin);
    unsigned num_elements = end - begin;

    if(num_elements <= threshold){
      qsort(arr+begin, num_elements, sizeof(int64_t),compare_i64);
      return;
    }

    if (num_elements < 2) {
    // a sequence of length 0 or 1 is trivially already sorted
       return;
    }


    size_t mid = begin + (end - begin) / 2;
    merge_sort(arr, begin, mid, threshold);
    merge_sort(arr, mid, end, threshold);

    int64_t *temparr = (int64_t*) malloc(num_elements * sizeof(int64_t));

    merge(arr, begin, mid, end, temparr);

    for(int i =0; i<num_elements; i++){
      arr[begin+i] = temparr[i];
    }
    free(temparr);
  
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
  if (end != argv[2] + strlen(argv[2])){
    fprintf(stderr, "%ld Not high enough threshhold", strlen(argv[2]));
    return 1;
  }
    /* TODO: report an error (threshold value is invalid) */;
  
  // TODO: open the file

  // TODO: use fstat to determine the size of the file
  int fd = open(filename, O_RDWR);
if (fd < 0) {
  fprintf(stderr, "Unsupported file");
  return 1;
}

// TODO: map the file into memory using mmap

struct stat statbuf;
int rc = fstat(fd, &statbuf);
if (rc != 0) {
   fprintf(stderr, "unable to get file information");
   return 1;
}
size_t file_size_in_bytes = statbuf.st_size;

int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
// you should immediately close the file descriptor here since mmap maintains a separate
// reference to the file and all open fds will gets duplicated to the children, which will
// cause fd in-use-at-exit leaks.
// TODO: call close()

if (data == MAP_FAILED) {
   fprintf(stderr, "unable to map using mmap");
   return 1;
}
// *data now behaves like a standard array of int64_t. Be careful though! Going off the end
// of the array will silently extend the file, which can rapidly lead to disk space
// depletion!
  // TODO: sort the data!
    merge_sort(data, 0, file_size_in_bytes/sizeof(int64_t), threshold);
  // TODO: unmap and close the file
    int ummap = munmap(data, file_size_in_bytes);

    if(ummap != 0){
      fprintf(stderr, "unable to unmap with munmap");
   return 1;
    }
    int closed = close(fd);
    if(closed != 0){
      fprintf(stderr, "unable to close the file");
      return 1;
    }

  // TODO: exit with a 0 exit code if sort was successful
  return 0;
}
