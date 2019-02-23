//PREDESCU MARIA 334CB

#include "homework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int num_threads;
int resize_factor;

image *t_in, *t_out;

//Functie pentru citirea unei imagini din fisier
void readInput(const char * fileName, image *img) {

	FILE * file;
	file = fopen(fileName, "r");

	//Citire tip de imegine: P5(greyscale) sau P6(color)
	fscanf(file, "%s\n", img->type);

	//Citire dimensiuni imagine 
	fscanf(file, "%d %d\n", &img->width, &img->height);

	//Citire maxval
	fscanf(file, "%d\n", &img->maxval);

	//Se retine numarul de culori in functie de tipul de imagine(alb-negru sau color)
	int numcolors = 1;
	if (strcmp(img->type, "P5") == 0) {
		numcolors = 1;
	} else if (strcmp(img->type, "P6") == 0) {
		numcolors = 3;
	}
	
	//Se aloca memorie pentru matricea de pixeli a imaginii
	img->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * img->height);
	for (int i = 0; i < img->height; i++) {
		img->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) 
			* img->width * numcolors);
	}

	//Citirea din fisier a matricei de pixeli a imaginii
	for (int i = 0; i < img->height; i++) {
			fread((void*)(img->pixels[i]), sizeof(unsigned char), 
				img->width * numcolors, file);
	}

	fclose(file);
}

//Functia in care se realizeaza paralelizarea procesului de resize al unei imaginii
void* threadFunction(void* var) {
	
	int paralel = *(int*)var;

	image* in = t_in;
	image* out = t_out;
	//Daca dimensiunile matricei nu se impart exact la resize_factor, 
	//restul este eliminat si nu se tine cont de acei pixeli
	int width = in->width - in->width % resize_factor;
	int height = in->height - in->height % resize_factor;

	//Pasii de paralelizare ai procesului in funtie de numarul de thread-uri
	int start = paralel * (height / resize_factor / num_threads);
	int end = (paralel + 1) * (height / resize_factor / num_threads);

	//Daca dimensiunile inaginii nu se impart exact la resize_factor si la
	//num_threads atunci trebuie sa avem in vedere calcularea 
	//separta a pixelilor ramasi 
	int remainder = (height / resize_factor / num_threads) 
		* resize_factor * num_threads;

	//Numarul de culori in functie de tipul imaginii
	int numcolors = 1;
	if (strcmp(in->type, "P5") == 0) {
		numcolors = 1;
	} else if (strcmp(in->type, "P6") == 0) {
	 	numcolors = 3;
	}
	
	//Initializare matrice Gaussiana
	int gaussianKernel[3][3] = {
		{1, 2, 1},
		{2, 4, 2},
		{1, 2, 1}
	};


	//In functie de numarul de culori se realizeaza procesul de resize al imaginii
	//1.Imaginea este color
	if (numcolors == 3) {

		//Daca resize_factor este numar par
		if (resize_factor % 2 == 0) {
			for (int i = start * resize_factor; i < end * resize_factor; 
				i += resize_factor) {
				for (int j = 0; j < width * numcolors; 
					j += (resize_factor * numcolors)) {
					int sum_red = 0;
					int sum_green = 0;
					int sum_blue = 0;

					//Matricea de pixeli este parcursa pe "patrate" de dimensiune
					//resize_factor x resize_factor * numcolors
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor * numcolors; 
			 				l += numcolors) {

			 				//Se realizeaza suma pixelilor din fiecare patrat
			 				//pentru fiecare culoare separat(red, green, blue)
			 				sum_red += in->pixels[i + k][j + l];
			 				sum_green += in->pixels[i + k][j + l + 1];
			 				sum_blue += in->pixels[i + k][j + l + 2];
			 			}
			 		}

			 		//Este populata imaginea rezultat cu media aritmetica a pixelilor
			 		//din fiecare patrat pentru fiecare culoare separat
			 		out->pixels[i / resize_factor][j / resize_factor] = 
			 			sum_red / (resize_factor * resize_factor);
			 		out->pixels[i / resize_factor][j / resize_factor + 1] = 
			 			sum_green / (resize_factor * resize_factor);
			 		out->pixels[i / resize_factor][j / resize_factor + 2] = 
			 			sum_blue / (resize_factor * resize_factor);
				}
			}

			//Algoritmul de mai sus este reluat pentru pixelii ramasi din imaginea 
			//data deoarece dimensiunile nu se impart exact la resize_factor si
			//num_threads
			for (int i = remainder; i < height; i += resize_factor) {
				for (int j = 0; j < width * numcolors; 
					j += (resize_factor * numcolors)) {
					int sum_red = 0;
					int sum_green = 0;
					int sum_blue = 0;
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor * numcolors; 
			 				l += numcolors) {

			 				sum_red += in->pixels[i + k][j + l];
			 				sum_green += in->pixels[i + k][j + l + 1];
			 				sum_blue += in->pixels[i + k][j + l + 2];
			 			}
			 		}
			 		out->pixels[i / resize_factor][j / resize_factor] = 
			 			sum_red / (resize_factor * resize_factor);
			 		out->pixels[i / resize_factor][j / resize_factor + 1] = 
			 			sum_green / (resize_factor * resize_factor);
			 		out->pixels[i / resize_factor][j / resize_factor + 2] = 
			 			sum_blue / (resize_factor * resize_factor);
				}

			}

		//Daca resize_factor este 3 este folosita matricea Gaussiana
		} else if (resize_factor == 3) {
			for (int i = start * resize_factor; i < end * resize_factor; 
				i += resize_factor) {
				for (int j = 0; j < width * numcolors; 
					j += (resize_factor * numcolors)) {
					int sum_red = 0;
					int sum_green = 0;
					int sum_blue = 0;

					//Matricea de pixeli este parcursa pe "patrate" de dimensiune
					//resize_factor x resize_factor * numcolors
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor * numcolors; 
			 				l += numcolors) {

			 				//Se realizeaza suma pixelilor din fiecare patrat inmultiti
			 				//cu elementul corespunzator din matricea Gaussiana
			 				sum_red += (int) (in->pixels[i + k][l + j] 
			 					* gaussianKernel[k][l/numcolors]);
			 				sum_green += (int) (in->pixels[i + k][l + j + 1] 
			 					* gaussianKernel[k][l/numcolors]);
			 				sum_blue += (int) (in->pixels[i + k][l + j + 2] 
			 					* gaussianKernel[k][l/numcolors]);
			 			}
			 		}
			 		//Este populata imaginea rezultat
			 		out->pixels[i / resize_factor][j / resize_factor] = 
			 			(unsigned char) (sum_red / 16);
			 		out->pixels[i / resize_factor][j / resize_factor + 1] = 
			 			(unsigned char) (sum_green / 16);
			 		out->pixels[i / resize_factor][j / resize_factor + 2] = 
			 			(unsigned char) (sum_blue / 16);
				}	
			}

			//Algoritmul de mai sus este reluat pentru pixelii ramasi din imaginea 
			//data deoarece dimensiunile nu se impart exact la resize_factor si
			//num_threads
			for (int i = remainder; i < height; i += resize_factor) {
				for (int j = 0; j < width * numcolors; 
					j += (resize_factor * numcolors)) {
					int sum_red = 0;
					int sum_green = 0;
					int sum_blue = 0;
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor * numcolors; 
			 				l += numcolors) {

			 				sum_red += (int) (in->pixels[i + k][l + j] 
			 					* gaussianKernel[k][l/numcolors]);
			 				sum_green += (int) (in->pixels[i + k][l + j + 1] 
			 					* gaussianKernel[k][l/numcolors]);
			 				sum_blue += (int) (in->pixels[i + k][l + j + 2] 
			 					* gaussianKernel[k][l/numcolors]);
			 			}
			 		}
			 		out->pixels[i / resize_factor][j / resize_factor] = 
			 			(unsigned char) (sum_red / 16);
			 		out->pixels[i / resize_factor][j / resize_factor + 1] = 
			 			(unsigned char) (sum_green / 16);
			 		out->pixels[i / resize_factor][j / resize_factor + 2] = 
			 			(unsigned char) (sum_blue / 16);
				}
			}

		}

	//2.Imaginea este alb-negru
	} else if (numcolors == 1) {
		if (resize_factor % 2 == 0) {
			for (int i = start * resize_factor; i < end * resize_factor; 
				i += resize_factor) {
				for (int j = 0; j < width; j += resize_factor) {
					
					int sum  = 0;
					//Imginea este impartita in parcursa pe patrate de dimenisiune
					//resize_factor * resize_factor
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor; l++) {
			 				
			 				//Suma pixelilor din fiecare patrat
			 				sum += in->pixels[i + k][j + l];
			 			}
			 		}
			 		//Popularea matricei rezultat
			 		out->pixels[i / resize_factor][j / resize_factor] = 
			 			sum / (resize_factor * resize_factor);
				}			
			}

			//Algoritmul de mai sus este reluat pentru pixelii ramasi din imaginea 
			//data deoarece dimensiunile nu se impart exact la resize_factor si
			//num_threads
			for (int i = remainder; i < height; i += resize_factor) {
				for (int j = 0; j < width; j += resize_factor) {
					int sum  = 0;
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor; l++) {
			 				sum += in->pixels[i + k][j + l];
			 			}
			 		}
			 		out->pixels[i / resize_factor][j / resize_factor] = 
			 			sum / (resize_factor * resize_factor);
				}			
			}
		//Daca resize_factor este 3 este folosita matricea Gaussiana
		} else if (resize_factor == 3) {
			for (int i = start * resize_factor; i < end * resize_factor; 
				i += resize_factor) {
				for (int j = 0; j < width; j += resize_factor) {
					
					int sum = 0;
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor ; l++) {
			 					sum += (int)in->pixels[i + k][l + j] 
			 					* gaussianKernel[k][l];
			 			}
			 		}
			 		out->pixels[i / resize_factor][j / resize_factor] = sum / 16;
				}
			}
			//Algoritmul de mai sus este reluat pentru pixelii ramasi din imaginea 
			//data deoarece dimensiunile nu se impart exact la resize_factor si
			//num_threads
			for (int i = remainder; i < height; i += resize_factor) {
				for (int j = 0; j < width; j += resize_factor) {
					
					int sum = 0;
			 		for (int k = 0; k < resize_factor; k++) {
			 			for (int l = 0; l < resize_factor ; l++) {
			 					sum += (int)in->pixels[i + k][l + j] 
			 					* gaussianKernel[k][l];
			 			}
			 		}
			 		out->pixels[i / resize_factor][j / resize_factor] = sum / 16;
				}
			}
		}
	}
	return NULL;
}

//Functia in care se creeaza thread-urile
void resize(image* in, image* out) { 

	t_in = in;
	t_out = out;

	//Determinarea numcolors in functie de tipul imaginii
	int numcolors = 1;
	if (strcmp(in->type, "P5") == 0) {
		numcolors = 1;
	} else if (strcmp(in->type, "P6") == 0) {
	 	numcolors = 3;
	}

	//Se initializeaza imaginea de output
	strcpy(out->type, in->type);
	out->width = in->width / resize_factor;
	out->height = in->height / resize_factor;
	out->maxval = in->maxval;

	//Se aloca memorie pentru matricea redimensionata
	out->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * out->height);
	for (int i = 0; i < out->height; i++) {
		out->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) 
			* out->width * numcolors);
	}

	pthread_t tid[num_threads];
	int thread_id[num_threads];

	for(int i = 0; i < num_threads; i++) {
		thread_id[i] = i;
	}

	//Se creeaza thread-urile
	for(int i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	//Se asteapta ca thread-urile sa termine executia
	for(int i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
}

//Functie pentru scrierea imaginii rezultate in urma redimensionarii in fisier
void writeData(const char * fileName, image *out) {
	
	FILE * file;
	file = fopen(fileName, "w");
	
	//Scriere tip imagine
	fprintf(file, "%s\n", out->type);

	//Scriere dimensiuni imagine
	fprintf(file, "%d %d\n", out->width, out->height);

	//Scriere maxval
	fprintf(file, "%d\n", out->maxval);

	//Determinarea numcolors in functie de tipul imaginii
	int numcolors = 1;
	if (strcmp(out->type, "P5") == 0) {
		numcolors = 1;
	} else if (strcmp(out->type, "P6") == 0) {
		numcolors = 3;
	}

	//Scriere matrice de pixeli in fisierul de output
	for (int i = 0; i < out->height; i++) {
		fwrite((void*)(out->pixels[i]), sizeof(unsigned char), 
			out->width * numcolors, file);
	}

	fclose(file);
}
