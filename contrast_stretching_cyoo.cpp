#include <iostream>

FILE 			*file_in;
FILE 			*file_out;

// converts ascii file to img file
void asc2img(char * asc_temp, char * out_name, int width, int height){
  int pixel;
  int i;

  // convert ascii to img file
  file_in  = fopen(asc_temp, "r");
  file_out = fopen(out_name, "wb");

  for (i = 0; i < width*height; i++) {
    fscanf(file_in, "%d", &pixel);
    fprintf(file_out, "%c", pixel);
  }
 
  fclose(file_in);
  fclose(file_out);
}

void contrast_stretch(char * in_name, char * asc_temp, int r1, int s1, int r2, int s2, int width, int height){
  int i;
  int j;
  float			slope0;
  float			slope1;
  float			slope2;
  unsigned char pixel;

  // slope from the origin to the point (s1,r1)
  if (r1 != 0)
    slope0 = (float) s1 / (float) r1;

  // slope of the middle band  
  if ((r2-r1) != 0)
    slope1 = (float) (s2-s1) / (float) (r2-r1);
  
  // last part
  if (r2!= 255)
    slope2 = (float) s2 / (float) (255-r2);
  
  // apply to image
  file_in  = fopen(in_name, "rb");
  file_out = fopen(asc_temp, "w");

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fscanf(file_in, "%c", &pixel);

      if (pixel < r1)
        pixel = slope0 * pixel;
      else if ((pixel >= r1) && (pixel < r2))
        pixel = slope1 * pixel;
      else if (pixel >= r2)
        pixel = slope2 * pixel;

	  // create out file
      fprintf(file_out, "%d ", pixel);
    }
    fprintf(file_out, "\n");
  }

  fclose(file_in);
  fclose(file_out);
}

int main(){
  char 			in_name[15];
  char			out_name[15];
  char			asc_temp[15] = "temp.asc";
  int 			width;
  int 			height;
  int			r1;
  int			s1;
  int			r2;
  int			s2;

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

  contrast_stretch(in_name, asc_temp, r1, s1, r2, s2, width, height);

  asc2img(asc_temp, out_name, width, height);

  return 1;
}

