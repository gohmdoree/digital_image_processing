#include <iostream>

const int		MAX_COLORS = 256;
FILE 			*file_in;
FILE			*file_out;
FILE			*file_out0;
FILE			*file_out1;
FILE 			*file_temp0;
FILE 			*file_temp1;

// converts ascii file to img file
void asc2img(char * asc_temp, char * out_name, int width, int height){
  int pixel;
  int i;

  // convert ascii to img file
  file_in  = fopen(asc_temp, "r");
  file_out0 = fopen(out_name, "wb");

  for (i = 0; i < width*height; i++) {
    fscanf(file_in, "%d", &pixel);
    fprintf(file_out0, "%c", pixel);
  }
 
  fclose(file_in);
  fclose(file_out0);
}

void bitplane(char * in_name, char * bitplane_temp0, char * bitplane_temp1, int width, int height, int bit_plane0, int bit_plane1){
  int			bitset0;
  int			bitset1;
  unsigned char pixel;
  int i;
  int j;

  // convert file to ascii to work with
  file_in  = fopen(in_name, "rb");
  file_temp0 = fopen(bitplane_temp0, "w");
  file_temp1 = fopen(bitplane_temp1, "w");
  
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fscanf(file_in, "%c", &pixel);

	  bitset0 = (pixel & (1 << bit_plane0));
	  bitset1 = (pixel & (1 << bit_plane1));

	  if(bitset0)
		fprintf(file_temp0, "%d ", pixel);
	  else
		fprintf(file_temp0, "%d ", 0);

	  if(bitset1)
		fprintf(file_temp1, "%d ", pixel);
	  else
		fprintf(file_temp1, "%d ", 0);
    }
    fprintf(file_temp0, "\n");
	fprintf(file_temp1, "\n");
  }

  fclose(file_in);
  fclose(file_temp0);
  fclose(file_temp1);
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
  // vars
  char 			in_name[15];
  char			out_name[15];
  char			asc_temp[15] = "temp.asc";
  char			bitplane_temp0[15] = "temp0.asc";
  char			bitplane_temp1[15] = "temp1.asc";
  char			bitplane_out0[15];
  char			bitplane_out1[15];
  int			bit_plane0 = 6;
  int			bit_plane1 = 2;
  int			width;
  int			height;
  int			r1;
  int			s1;
  int			r2;
  int			s2;

  // get image name & attributes
  printf("enter the .img filename (input): ");
  scanf("%s", in_name);
  printf("enter the width: ");
  scanf("%d", &width);
  printf("enter the height: ");
  scanf("%d", &height);

  /*** BIT-PLANE SLICING ***/
  // get image name & attributes
  printf("\n\nBIT_PLANE_SLICING\n\n");
  printf("enter the .img filename (bit-plane 6 output): ");
  scanf("%s", bitplane_out0);
  printf("enter the .img filename (bit-plane 2 output): ");
  scanf("%s", bitplane_out1);

  // bit-plane slicing
  bitplane(in_name, bitplane_temp0, bitplane_temp1, width, height, bit_plane0, bit_plane1);

  asc2img(bitplane_temp0, bitplane_out0, width, height);
  asc2img(bitplane_temp1, bitplane_out1, width, height);


  /*** HISTOGRAM-EQUALIZATION ***/
  // get image name & attributes
  printf("\n\nHISTOGRAM_EQUALIZATION\n\n");
  printf("enter the .img filename (output): ");
  scanf("%s", out_name);

  // histogram-equalization
  histogram_equalization(in_name, asc_temp, width, height);
  
  asc2img(asc_temp, out_name, width, height);

  /*** CONTRAST STRETCHING ***/
  // get image name & attributes
  printf("\n\nCONTRAST_STRETCHING\n\n");
  printf("enter the .img filename (output): ");
  scanf("%s", out_name);
  printf("enter the r1: ");
  scanf("%d", &r1);
  printf("enter the s1: ");
  scanf("%d", &s1);
  printf("enter the r2: ");
  scanf("%d", &r2);
  printf("enter the s2: ");
  scanf("%d", &s2);

  // contrast stretching
  contrast_stretch(in_name, asc_temp, r1, s1, r2, s2, width, height);

  asc2img(asc_temp, out_name, width, height);

  return 0;
}
