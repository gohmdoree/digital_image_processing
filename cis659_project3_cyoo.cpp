#include <iostream>  // I/O 
#include <fstream>   // file I/O
#include <iomanip>   // format manipulation

// added 2, to account for 0 borders
const int		WIDTH	= 27;
const int		HEIGHT  = 32;

FILE 			*file_in;
FILE			*file_out;

using namespace std;

// converts ascii file to img file
void asc2img(char * asc_temp, char * out_name, int width, int height);

// initialize multidimensional array
void initializeArray(int width, int height, int array[WIDTH][HEIGHT]);

// creates multidimensional array
void createArray(int width, int height, char * in_name, int array[WIDTH][HEIGHT]);

// prints multidimensional array
void printArray(int width, int height, int array[WIDTH][HEIGHT]);

// get turn for chain code 8-neighbor
int getTurn(int i, int j, int array[WIDTH][HEIGHT]);

// get code for chain code 8-neighbor
void getCode(int array[WIDTH][HEIGHT]);

int main(){
	char	in_name[25] = "c:\\class\\f.img";
	int		array[WIDTH][HEIGHT];

	// get image name & attributes
	cout << "applying filters for image: " << in_name << endl;
	
	createArray(WIDTH, HEIGHT, in_name, array);

	getCode(array);

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

// initialize multidimensional array
void initializeArray(int width, int height, int array[WIDTH][HEIGHT]){
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; j++) {
			array[i][j] = 0;
		}
	}	
}

// creates multidimensional array
void createArray(int width, int height, char * in_name, int array[WIDTH][HEIGHT]){
	unsigned char	pixel;

	file_in  = fopen(in_name, "rb");

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if ((i == 0) || (i == (width-1)) || (j == 0) || (j == (height-1))){
				array[i][j] = 0;
			}else{
				fscanf(file_in, "%c", &pixel);
				
				// change 255 to 1, so we can view the image 
				if (pixel == 255)
					array[i][j] = 1;
				else
					array[i][j] = 0;
			}
		}
	}

	fclose(file_in);
}

// prints multidimensional array
void printArray(int width, int height, int array[WIDTH][HEIGHT]){
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			printf("%d", array[i][j]);
		}
		cout << endl;
	}
}

// get turn for chain code 8-neighbor
int getTurn(int i, int j, int array[WIDTH][HEIGHT]){
	//	0 0
	//	x 1
	if ((array[i][j-1] == 0) && (array[i+1][j-1] == 0) && (array[i][j] == 1) && (array[i+1][j] == 1)){
		return 0;
	//	x 0
	//	1 1
	}else if ((array[i][j] == 1) && (array[i+1][j] == 0) && (array[i][j+1] == 1) && (array[i+1][j+1] == 1)){
		return 1;
	//	x 0
	//	1 0
	}else if ((array[i][j] == 1) && (array[i+1][j] == 0) && (array[i][j+1] == 1) && (array[i+1][j+1] == 0)){
		return 2;
	//	1 x
	//	1 0
	}else if ((array[i-1][j] == 1) && (array[i][j] == 1) && (array[i-1][j+1] == 1) && (array[i][j+1] == 0)){
		return 3;
	//	1 x
	//	0 0
	}else if ((array[i-1][j] == 1) && (array[i][j] == 1) && (array[i-1][j+1] == 0) && (array[i][j+1] == 0)){
		return 4;
	//	1 1
	//	0 x
	}else if ((array[i-1][j-1] == 1) && (array[i][j-1] == 1) && (array[i-1][j] == 0) && (array[i][j] == 1)){
		return 5;
	//	0 1
	//	0 x
	}else if ((array[i-1][j-1] == 0) && (array[i][j-1] == 1) && (array[i-1][j] == 0) && (array[i][j] == 1)){
		return 6;
	//	0 1
	//	x 1
	}else if ((array[i][j-1] == 0) && (array[i+1][j-1] == 1) && (array[i][j] == 1) && (array[i+1][j] == 1)){
		return 7;
	}else{
		return 8;
	}
}

// get code for chain code 8-neighbor
void getCode(int array[WIDTH][HEIGHT]){
	int		i = 0;
	int		j = 0;
	int		code = 0;
	int		hit = 0;
	int		sx = 0;
	int		sy = 0;

	while(1){
		code = getTurn(i,j,array);

		if ((hit == 0) && (array[i][j] == 1)){
			sx = i;
			sy = j;
			hit = 1;
			i++;
			cout << "\n" << code << " ";
		}else if ((hit == 1) && (sx == i) && (sx ==j)){
			cout << code << " done.\n\n";
			break;
		}else if (code == 8){
			i++;
			if (i == (WIDTH-1)){
				i = 0;
				j++;
			}
		}else if (code == 0){
			cout << code << " ";
			i++;
		}else if (code == 1){
			cout << code << " ";
			i++;
			j++;
		}else if (code == 2){
			cout << code << " ";
			j++;
		}else if (code == 3){
			cout << code << " ";
			i--;
			j++;
		}else if (code == 4){
			cout << code << " ";
			i--;
		}else if (code == 5){
			cout << code << " ";
			i--;
			j--;
		}else if (code == 6){
			cout << code << " ";
			j--;
		}else if (code == 7){
			cout << code << " ";
			i++;
			j--;
		}		
	}
}