#include <iostream>

int main(){
  char 			in_name[15];
  char			out_name[15];
  char			asc_temp[15] = "temp.asc";
  int 			width;
  int 			height;
  int			i;
  int			j;
  int			r1;
  int			s1;
  int			r2;
  int			s2;
  float			slope;
  unsigned char pixel;
  int			ipixel;
  FILE 			*file_in;
  FILE 			*file_out;

  // get image name & attributes
  printf("enter the .img filename (input): ");
  scanf("%s", in_name);
  printf("enter the .img filename (output): ");
  scanf("%s", out_name);
  printf("enter the width: ");
  scanf("%d", &width);
  printf("enter the height: ");
  scanf("%d", &height);
  printf("enter the r1: ");
  scanf("%d", &r1);
  printf("enter the s1: ");
  scanf("%d", &s1);
  printf("enter the r2: ");
  scanf("%d", &r2);
  printf("enter the s2: ");
  scanf("%d", &s2);

  // slope
  slope = (float) (r2 - r1) / (float) (s2 - s1);
  
  // apply to image
  file_in  = fopen(in_name, "rb");
  file_out = fopen(asc_temp, "w");

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fscanf(file_in, "%c", &pixel);

      pixel = (pixel - s1) * slope + r1;

	  // create out file
      fprintf(file_out, "%d ", pixel);
    }
    fprintf(file_out, "\n");
  }

  fclose(file_in);
  fclose(file_out);

  // convert ascii to img file
  file_in  = fopen(asc_temp, "r");
  file_out = fopen(out_name, "wb");

  for (i = 0; i < width*height; i++) {
    fscanf(file_in, "%d", &ipixel);
    fprintf(file_out, "%c", ipixel);
  }
 
  fclose(file_in);
  fclose(file_out);

  return 1;
}