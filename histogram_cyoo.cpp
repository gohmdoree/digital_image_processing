#include <iostream>

const int		MAX_COLORS = 256;
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

void histogram_equalization(char * in_name, char * asc_temp, int width, int height){
  int i;
  int j;
  unsigned char pixel;
  double		histogram[MAX_COLORS];
  double		equalized[MAX_COLORS];
  double		sum = 0.0;

  // initiate our 256 element array
  // which will serve as our "histogram"
  for (i = 0; i < MAX_COLORS; i++){
	histogram[i] = 0.0;
	equalized[i] = 0.0;
  }

  // convert file to ascii to work with
  file_in  = fopen(in_name, "rb");
  
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fscanf(file_in, "%c", &pixel);

      // create histogram
      ++histogram[pixel];
    }
  }

  fclose(file_in);

  // test that we got the data
/*
  for (i = 0; i < MAX_COLORS; i++)
    printf ("%d: %f\n", i, histogram[i]);
*/

  // equalize histogram
  for (i = 0; i < MAX_COLORS; i++){
    sum += histogram[i];
	equalized[i] = sum / (width * height);
  }

  // test equalized values
  /*
  for (i = 0; i < MAX_COLORS; i++)
	  printf ("%d: %f\n", i, equalized[i]);
  */

  // apply to image
  file_in  = fopen(in_name, "rb");
  file_out = fopen(asc_temp, "w");

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fscanf(file_in, "%c", &pixel);

	  pixel = equalized[pixel] * 255;

	  // create out file
      fprintf(file_out, "%d ", pixel);
    }
    fprintf(file_out, "\n");
  }

  fclose(file_in);
  fclose(file_out);  
}

int main(void) {
  char 			in_name[15];
  char			out_name[15];
  char			asc_temp[15] = "temp.asc";
  int 			width;
  int 			height;

  // get image name & attributes
  printf("enter the .img filename (input): ");
  scanf("%s", in_name);
  printf("enter the .img filename (output): ");
  scanf("%s", out_name);
  printf("enter the width: ");
  scanf("%d", &width);
  printf("enter the height: ");
  scanf("%d", &height);

  histogram_equalization(in_name, asc_temp, width, height);
  
  asc2img(asc_temp, out_name, width, height);

  return 0;
}
