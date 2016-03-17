#include <iostream>

const int		MAX_COLORS = 256;
FILE 			*file_in;
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

int main(void) {
  // vars
  char 			in_name[15];
  char			bitplane_temp0[15] = "temp0.asc";
  char			bitplane_temp1[15] = "temp1.asc";
  char			bitplane_out0[15];
  char			bitplane_out1[15];
  int			bit_plane0 = 6;
  int			bit_plane1 = 2;
  int			width;
  int			height;

  // get image name & attributes
  printf("enter the .img filename (input): ");
  scanf("%s", in_name);
  printf("enter the .img filename (bit-plane 6 output): ");
  scanf("%s", bitplane_out0);
  printf("enter the .img filename (bit-plane 2 output): ");
  scanf("%s", bitplane_out1);
  printf("enter the width: ");
  scanf("%d", &width);
  printf("enter the height: ");
  scanf("%d", &height);

  bitplane(in_name, bitplane_temp0, bitplane_temp1, width, height, bit_plane0, bit_plane1);

  asc2img(bitplane_temp0, bitplane_out0, width, height);
  asc2img(bitplane_temp1, bitplane_out0, width, height);

  return 0;
}
