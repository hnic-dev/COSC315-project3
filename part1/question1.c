#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){

  //open input file to read 
  FILE *input = fopen("input.txt", "r");

  int address, n, m;
  unsigned int pageNum, offset;
  double x, y;
  unsigned int page_mask, offset_mask;

  //extracting n (lowest significant bits that represent the offset)
  fscanf(input, "%d", &n);
  //extracting m (next m bits that represent the page number)
  fscanf(input, "%d", &m);

  //calculating bit mask with n and m
  // x and y why are just doubles used to calculate power
  y = pow(2.0, (double) n) - 1;
  offset_mask = (unsigned int) y;
  x = pow(2.0, (double) m);
  page_mask = (unsigned int) x;
  page_mask = page_mask * offset_mask;

  while (fscanf(input, "%u", &address) != EOF){
    //clears lower n bits with rightshift operator and bitwise AND
    pageNum = (address & page_mask) >> n;
    //bitwise AND to focus on first n bits
    offset = address & offset_mask;

    //print results
    printf("virtual address %u is in page number %u and offset %u\n", address, pageNum, offset);

    /*
    Test the results for the page number and offset using DIV and MOD
    Using ((int) y + 1) is the same as 2^n
    */

    // int test_page = address / ((int) y + 1);
    // int test_offset = address % ((int) y + 1);
    // printf("Expeted page Number: %u\n", test_page);
    // printf("Expeted offset: %u\n", test_offset);

  }
  fclose(input);
  return 0;
}
