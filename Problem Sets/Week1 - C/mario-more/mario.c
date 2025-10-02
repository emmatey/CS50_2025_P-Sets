#include <cs50.h>
#include <stdio.h>

void print_segment (int width) {
   int i;
   for (i = 0; i < width; i++) {
      printf("#");
   }
}

void print_blank (int width) {
   int i;
   for (i = 0; i < width; i++){
      printf(" ");
   }
}

void assemble_pyramid (int width){
   int i;
   for (i = 1; i <= width; i++){
      print_blank(width - i);
      print_segment(i);
      printf("  ");
      print_segment(i);
      printf("\n");
   }
}

int main (void){

 int width;
 int valid = 0;
 while (valid == 0) {
    width = get_int("Enter desired pyramid height: ");
    if (width > 0 && width < 9){
      valid = 1;
    }
   }

 assemble_pyramid(width);
}
