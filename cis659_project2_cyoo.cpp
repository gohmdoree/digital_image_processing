#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation

/*
const int		WIDTH	= 64;
const int		HEIGHT  = 64;
*/
const int		WIDTH	= 256;
const int		HEIGHT  = 256;
const int		MAX_COLORS = 256;
FILE 			*file_in;
FILE			*file_out;
FILE 			*file_out_avg;
FILE 			*file_out_med;
FILE 			*file_out_lap;
FILE 			*file_out_sob;

using namespace std;

// converts ascii file to img file
void asc2img(char * asc_temp, char * out_name, int width, int height);

// creates multidimensional array
void createArray(int width, int height, char * in_name, int array[WIDTH][HEIGHT]);

// sort array using bubblesort
void bubbleSort(int array[], int size);

// find maximum in array
int returnMax(int array[WIDTH][HEIGHT]);

// find minimum in array
int returnMin(int array[WIDTH][HEIGHT]);

// returns absolute value
int absoluteValue(int number);

// performs average filter
int averageFilter(int i, int j, int array[WIDTH][HEIGHT]);

// performs median filter
int medianFilter(int i, int j, int array[WIDTH][HEIGHT]);

// applies laplacian filter
int laplacianFilter(int i, int j, int array[WIDTH][HEIGHT]);

// applies sobel filter
int sobelFilter(int i, int j, int array[WIDTH][HEIGHT]);

// map to G
int mapG(int minimum, int maximum, int fx);

int main(){
	//char	in_name[25] = "c:\\class\\wrench.img";
	char	in_name[25] = "c:\\class\\psg.img";
	int		array[WIDTH][HEIGHT];
	/*
	char	out_name_avg_t[25] = "output\\wrench_avg.asc";
	char	out_name_avg[25] = "output\\wrench_avg.img";
	char	out_name_med_t[25] = "output\\wrench_med.asc";
	char	out_name_med[25] = "output\\wrench_med.img";
	char	out_name_lap_t[25] = "output\\wrench_lap.asc";
	char	out_name_lap[25] = "output\\wrench_lap.img";
	char	out_name_sob_t[25] = "output\\wrench_sob.asc";
	char	out_name_sob[25] = "output\\wrench_sob.img";
	*/
	char	out_name_avg_t[25] = "output\\psg_avg.asc";
	char	out_name_avg[25] = "output\\psg_avg.img";
	char	out_name_med_t[25] = "output\\psg_med.asc";
	char	out_name_med[25] = "output\\psg_med.img";
	char	out_name_lap_t[25] = "output\\psg_lap.asc";
	char	out_name_lap[25] = "output\\psg_lap.img";
	char	out_name_sob_t[25] = "output\\psg_sob.asc";
	char	out_name_sob[25] = "output\\psg_sob.img";

	int		temp0;
	int		temp1;
	int		total;

	// get image name & attributes
	cout << "applying filters for image: " << in_name << endl;

	createArray(WIDTH, HEIGHT, in_name, array);

	file_out_avg = fopen(out_name_avg_t, "wb");
	file_out_med = fopen(out_name_med_t, "wb");
	file_out_lap = fopen(out_name_lap_t, "wb");
	file_out_sob = fopen(out_name_sob_t, "wb");

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			
			fprintf(file_out_avg, "%d ", mapG(returnMin(array),returnMax(array),averageFilter(i,j,array)));
			fprintf(file_out_med, "%d ", mapG(returnMin(array),returnMax(array),medianFilter(i,j,array)));
			fprintf(file_out_lap, "%d ", mapG(returnMin(array),returnMax(array),laplacianFilter(i,j,array)));
			fprintf(file_out_sob, "%d ", mapG(returnMin(array),returnMax(array),sobelFilter(i,j,array)));
			/*
			fprintf(file_out_avg, "%d ", averageFilter(i,j,array));
			fprintf(file_out_med, "%d ", medianFilter(i,j,array));
			fprintf(file_out_lap, "%d ", laplacianFilter(i,j,array));
			fprintf(file_out_sob, "%d ", sobelFilter(i,j,array));
			*/
		}
		fprintf(file_out_avg, "\n");
		fprintf(file_out_med, "\n");
		fprintf(file_out_lap, "\n");
		fprintf(file_out_sob, "\n");
	}

	fclose(file_out_avg);
	fclose(file_out_med);
	fclose(file_out_lap);
	fclose(file_out_sob);

	asc2img(out_name_avg_t, out_name_avg, WIDTH, HEIGHT);
	asc2img(out_name_med_t, out_name_med, WIDTH, HEIGHT);
	asc2img(out_name_lap_t, out_name_lap, WIDTH, HEIGHT);
	asc2img(out_name_sob_t, out_name_sob, WIDTH, HEIGHT);

	return 0;
}

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

// creates multidimensional array
void createArray(int width, int height, char * in_name, int array[WIDTH][HEIGHT]){
	unsigned char	pixel;

	file_in  = fopen(in_name, "rb");

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			fscanf(file_in, "%c", &pixel);
			array[i][j] = pixel;
		}
	}

	fclose(file_in);
}

// sort array using bubblesort
void bubbleSort(int array[], int size){
	int	temp;
	
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (array[i] < array[j]){
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;
			}
}

// find maximum in array
int returnMax(int array[WIDTH][HEIGHT]){
	int max = 0;

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			if (array[i][j] > max)
				max = array[i][j];
		}
	}

	return max;
}

// find minimum in array
int returnMin(int array[WIDTH][HEIGHT]){
	int min = 255;

	for (int i = 0; i < HEIGHT; i++){
		for (int j = 0; j < WIDTH; j++){
			if (array[i][j] < min)
				min = array[i][j];
		}
	}

	return min;
}

// returns absolute value
int absoluteValue(int number){
	if(number >= 0)
		return number;
	else
		return -number;
}

/*
	z1 = [i-1][j-1]
	z2 = [i][j-1]
	z3 = [i+1][j-1]
	z4 = [i-1][j]
	z5 = [i][j]
	z6 = [i+1][j]
	z7 = [i-1][j+1]
	z8 = [i][j+1]
	z9 = [i+1][j+1]
  */
// performs average filter
int averageFilter(int i, int j, int array[WIDTH][HEIGHT]){
	int average;

	// nw corner (z5, z6, z8, z9)
	if ((i == 0) && (j == 0))
		average = (array[i][j] + array[i+1][j] + array[i][j+1] + array[i+1][j+1]) / 4;
	// ne corner (z2, z3, z5, z6)
	else if ((i == (WIDTH-1)) && (j == 0))
		average = (array[i-1][j] + array[i-1][j+1] + array[i][j] + array[i][j+1]) / 4;
	// se corner (z5, z6, z8, z9)
	else if ((i == (WIDTH-1)) && (j == (HEIGHT-1)))
		average = (array[i-1][j-1] + array[i-1][j] + array[i][j-1] + array[i][j]) / 4;
	// sw corner (z4, z5, z7, z8)
	else if ((i == 0) && (j == (HEIGHT-1)))
		average = (array[i][j-1] + array[i][j] + array[i+1][j-1] + array[i+1][j]) / 4;
	// top row (z4, z5, z6, z7, z8, z9)
	else if ((i > 0) && (i < (WIDTH-1)) && (j == 0))
		average = (array[i-1][j] + array[i][j] + array[i+1][j] + array[i-1][j+1] + array[i][j+1] + array[i+1][j+1]) / 6;
	// w side (z1, z2, z4, z5, z7, z8)
	else if ((i == 0) && (j > 0) && (j < (HEIGHT-1)))
		average = (array[i][j-1] + array[i][j] + array[i][j+1] + array[i+1][j-1] + array[i+1][j] + array[i+1][j+1]) / 6;
	// e side (z2, z3, z5, z6, z8, z9)
	else if ((i == (WIDTH-1)) && (j > 0) && (j < (HEIGHT-1)))
		average = (array[i-1][j-1] + array[i-1][j] + array[i-1][j+1] + array[i][j-1] + array[i][j] + array[i][j+1]) / 6;
	// s side (z4, z5, z6, z7, z8, z9)
	else if ((i > 0) && (i < (WIDTH-1)) && (j == (HEIGHT-1)))
		average = (array[i-1][j-1] + array[i-1][j] + array[i][j-1] + array[i][j] + array[i+1][j-1] + array[i+1][j]) / 6;
	// everything else (z1, z2, z3, z4, z5, z6, z7, z8, z9)
	else
		average = (array[i-1][j-1] + array[i][j-1] + array[i+1][j-1] + array[i-1][j] + array[i][j] + array[i+1][j] + array[i-1][j+1] + array[i][j+1] + array[i+1][j+1]) / 9;

	return average;
}

/*
	z1 = [i-1][j-1]
	z2 = [i][j-1]
	z3 = [i+1][j-1]
	z4 = [i-1][j]
	z5 = [i][j]
	z6 = [i+1][j]
	z7 = [i-1][j+1]
	z8 = [i][j+1]
	z9 = [i+1][j+1]
  */
// performs median filter
int medianFilter(int i, int j, int array[WIDTH][HEIGHT]){
	int median;
	int median_4[4];
	int median_6[6];
	int median_9[9];

	// nw corner (z5, z6, z8, z9)
	if ((i == 0) && (j == 0)){
		median_4[0] = array[i][j];
		median_4[1] = array[i+1][j];
		median_4[2] = array[i][j+1];
		median_4[3] = array[i+1][j+1];

		bubbleSort(median_4,4);

		median = (median_4[1] + median_4[2]) / 2;
	// ne corner (z2, z3, z5, z6)
	}else if ((i == (WIDTH-1)) && (j == 0)){
		median_4[0] = array[i][j-1];
		median_4[1] = array[i+1][j-1];
		median_4[2] = array[i][j];
		median_4[3] = array[i+1][j];

		bubbleSort(median_4,4);

		median = (median_4[1] + median_4[2]) / 2;
	// se corner (z5, z6, z8, z9)
	}else if ((i == (WIDTH-1)) && (j == (HEIGHT-1))){
		median_4[0] = array[i][j];
		median_4[1] = array[i+1][j];
		median_4[2] = array[i][j+1];
		median_4[3] = array[i+1][j+1];

		bubbleSort(median_4,4);

		median = (median_4[1] + median_4[2]) / 2;
	// sw corner (z4, z5, z7, z8)
	}else if ((i == 0) && (j == (HEIGHT-1))){
		median_4[0] = array[i-1][j];
		median_4[1] = array[i][j];
		median_4[2] = array[i-1][j+1];
		median_4[3] = array[i][j+1];

		bubbleSort(median_4,4);

		median = (median_4[1] + median_4[2]) / 2;
	// top row (z4, z5, z6, z7, z8, z9)
	}else if ((i > 0) && (i < (WIDTH-1)) && (j == 0)){
		median_6[0] = array[i-1][j];
		median_6[1] = array[i][j];
		median_6[2] = array[i+1][j];
		median_6[3] = array[i-1][j+1];
		median_6[4] = array[i][j+1];
		median_6[5] = array[i+1][j+1];

		bubbleSort(median_6,6);

		median = (median_6[2] + median_6[3]) / 2;
	// w side (z1, z2, z4, z5, z7, z8)
	}else if ((i == 0) && (j > 0) && (j < (HEIGHT-1))){
		median_6[0] = array[i][j-1];
		median_6[1] = array[i+1][j-1];
		median_6[2] = array[i][j];
		median_6[3] = array[i+1][j];
		median_6[4] = array[i][j+1];
		median_6[5] = array[i+1][j+1];

		bubbleSort(median_6,6);

		median = (median_6[2] + median_6[3]) / 2;
	// e side (z2, z3, z5, z6, z8, z9)
	}else if ((i == (WIDTH-1)) && (j > 0) && (j < (HEIGHT-1))){
		median_6[0] = array[i-1][j-1];
		median_6[1] = array[i][j-1];
		median_6[2] = array[i-1][j];
		median_6[3] = array[i][j];
		median_6[4] = array[i-1][j+1];
		median_6[5] = array[i][j+1];

		bubbleSort(median_6,6);

		median = (median_6[2] + median_6[3]) / 2;
	// s side (z4, z5, z6, z7, z8, z9)
	}else if ((i > 0) && (i < (WIDTH-1)) && (j == (HEIGHT-1))){
		median_6[0] = array[i-1][j-1];
		median_6[1] = array[i][j-1];
		median_6[2] = array[i+1][j-1];
		median_6[3] = array[i-1][j];
		median_6[4] = array[i][j];
		median_6[5] = array[i+1][j];

		bubbleSort(median_6,6);

		median = (median_6[2] + median_6[3]) / 2;
	// everything else (z1, z2, z3, z4, z5, z6, z7, z8, z9)
	}else{
		median_9[0] = array[i-1][j-1];
		median_9[1] = array[i-1][j];
		median_9[2] = array[i-1][j+1];
		median_9[3] = array[i][j-1];
		median_9[4] = array[i][j];
		median_9[5] = array[i][j+1];
		median_9[6] = array[i+1][j-1];
		median_9[7] = array[i+1][j];
		median_9[8] = array[i+1][j+1];

		bubbleSort(median_9,9);

		median = median_9[4];
	}

	return median;
}

/*
	z1 = [i-1][j-1]
	z2 = [i][j-1]
	z3 = [i+1][j-1]
	z4 = [i-1][j]
	z5 = [i][j]
	z6 = [i+1][j]
	z7 = [i-1][j+1]
	z8 = [i][j+1]
	z9 = [i+1][j+1]
  */
// applies laplacian filter
int laplacianFilter(int i, int j, int array[WIDTH][HEIGHT]){
	int value;

	// nw corner (z5, z6, z8)
	if ((i == 0) && (j == 0))
		value = (4*array[i][j] - array[i+1][j] - array[i][j+1]);
	// ne corner (z2, z5, z6)
	else if ((i == (WIDTH-1)) && (j == 0))
		value = (-array[i][j+1] + 4*array[i][j] - array[i-1][j]);
	// se corner (z5, z6, z8)
	else if ((i == (WIDTH-1)) && (j == (HEIGHT-1)))
		value = (4*array[i][j] - array[i][j-1] - array[i-1][j]);
	// sw corner (z4, z5, z8)
	else if ((i == 0) && (j == (HEIGHT-1)))
		value = (-array[i][j-1] + 4*array[i][j]  - array[i+1][j]);
	// top row (z4, z5, z6, z8)
	else if ((i > 0) && (i < (WIDTH-1)) && (j == 0))
		value = (-array[i-1][j] + 4*array[i][j] - array[i+1][j] - array[i][j+1]);
	// w side (z2, z5, z6, z8)
	else if ((i == 0) && (j > 0) && (j < (HEIGHT-1)))
		value = (-array[i][j-1] + 4*array[i][j] - array[i+1][j] - array[i][j+1]);
	// e side (z2, z5, z6, z8)
	else if ((i == (WIDTH-1)) && (j > 0) && (j < (HEIGHT-1)))
		value = (-array[i][j-1] + 4*array[i][j] - array[i-1][j] - array[i][j+1]);
	// s side (z4, z5, z6, z8)
	else if ((i > 0) && (i < (WIDTH-1)) && (j == (HEIGHT-1)))
		value = (-array[i-1][j] + 4*array[i][j] - array[i+1][j] - array[i][j-1]);
	// everything else (z1, z2, z3, z4, z5, z6, z7, z8, z9)
	else
		value = (-array[i][j-1] - array[i-1][j]  + 4*array[i][j] - array[i+1][j] - array[i][j+1]);

	if (absoluteValue(value) > 255)
		return (absoluteValue(value) - 255);
	else 
		return absoluteValue(value);	
}

/*
	z1 = [i-1][j-1]
	z2 = [i][j-1]
	z3 = [i+1][j-1]
	z4 = [i-1][j]
	z5 = [i][j]
	z6 = [i+1][j]
	z7 = [i-1][j+1]
	z8 = [i][j+1]
	z9 = [i+1][j+1]
  */
// applies sobel filter
int sobelFilter(int i, int j, int array[WIDTH][HEIGHT]){
	int value;

	// nw corner (z5, z6, z8, z9)
	if ((i == 0) && (j == 0))
		value = absoluteValue((2*array[i][j+1] + array[i+1][j+1])) + absoluteValue((2*array[i+1][j] + array[i+1][j+1]));
	// ne corner (z4, z5, z7, z8)
	else if ((i == (WIDTH-1)) && (j == 0))
		value = absoluteValue((array[i-1][j+1] + 2*array[i][j+1])) + absoluteValue(-(2*array[i-1][j] + array[i-1][j+1]));
	// se corner (z1, z2, z4, z5)
	else if ((i == (WIDTH-1)) && (j == (HEIGHT-1)))
		value = absoluteValue(-(array[i-1][j-1] + 2*array[i][j-1])) + absoluteValue(-(array[i-1][j]));
	// sw corner (z2, z3, z5, z6)
	else if ((i == 0) && (j == (HEIGHT-1)))
		value = absoluteValue(-(2*array[i][j-1] + array[i+1][j-1])) + absoluteValue((array[i+1][j] + 2*array[i+1][j]));
	// top row (z4, z5, z6, z7, z8, z9)
	else if ((i > 0) && (i < (WIDTH-1)) && (j == 0))
		value = absoluteValue((array[i-1][j+1] + 2*array[i][j+1] + array[i+1][j+1])) + absoluteValue((2*array[i+1][j] + array[i+1][j+1]) - (2*array[i-1][j] + array[i-1][j+1]));
	// w side (z2, z3, z5, z6, z8, z9)
	else if ((i == 0) && (j > 0) && (j < (HEIGHT-1)))
		value = absoluteValue((2*array[i][j+1] + array[i+1][j+1]) - (2*array[i][j-1] + array[i+1][j-1])) + absoluteValue((array[i+1][j-1] + 2*array[i+1][j] + array[i+1][j+1]));
	// e side (z1, z2, z4, z5, z7, z8)
	else if ((i == (WIDTH-1)) && (j > 0) && (j < (HEIGHT-1)))
		value = absoluteValue((2*array[i-1][j+1] + array[i][j+1]) - (array[i-1][j-1] + 2*array[i][j-1])) + absoluteValue((2*array[i-1][j] + array[i-1][j+1]));
	// s side (z1, z2, z3, z4, z5, z6)
	else if ((i > 0) && (i < (WIDTH-1)) && (j == (HEIGHT-1)))
		value = absoluteValue(-(array[i-1][j-1] + 2*array[i][j-1] + array[i+1][j-1])) + absoluteValue((array[i+1][j-1] + 2*array[i+1][j]) - (array[i-1][j-1] + 2*array[i-1][j]));
	// everything else (z1, z2, z3, z4, z5, z6, z7, z8, z9)
	else
		value = absoluteValue((array[i-1][j+1] + 2*array[i][j+1] + array[i+1][j+1]) - (array[i-1][j-1] + 2*array[i][j-1] + array[i+1][j-1])) + absoluteValue((array[i+1][j-1] + 2*array[i+1][j] + array[i+1][j+1]) - (array[i-1][j-1] + 2*array[i-1][j] + array[i-1][j+1]));

	if (value > 255)
		return value % 255;
	else
		return value;	
}

// map to G
int mapG(int minimum, int maximum, int fx){
	return absoluteValue(255 / (maximum - minimum) * (fx - minimum));
}