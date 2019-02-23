//PREDESCU MARIA 334CB

#include "homework1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

int num_threads;
int resolution;
image* img;

//Functie de initializare a imaginii ce trebuie creata
void initialize(image* im) {

	//Initializare tip imagine(alb-negru)
	strcpy(im->type, "P5");
	
	//Initializare dimensiuni imagine
	im->width = resolution;
	im->height = resolution;

	//Initializare maxval
	im->maxval = 255;

	//Alocare memorie pentru matricea de pixeli a imaginii
	im->pixels = (unsigned char**)malloc(sizeof(unsigned char*) * im->height);
	for(int i = 0; i < im->height; i++) {
		im->pixels[i] = (unsigned char*)malloc(sizeof(unsigned char) * im->width);
	}

	//Initializare pixeli imagine la culoarea alb
	for(int i = 0; i < im->height; i++) {
		for (int j = 0; j < im->width; j++) {
	 		im->pixels[i][j] = 255;
	 	}
	}
}

//Functia in care se realizeaza paralelizarea procesului
void* threadFunction(void* var) {

	int paralel = *(int*)var;

	//Pasii de paralelizare ai procesului in functie de rezolutia imaginii
	//si numarul de thread-uri
	int start = paralel * resolution / num_threads;
	int end = (paralel + 1) * resolution / num_threads;
	
	//Se calculeaza dimensiunea unui pixel raportata la rezolutia imaginii
	float pixelSize = 100 / (1.0f * resolution);
	float distance = 0;
	
	//Se decide daca un pixel trebuie sa fie alb sau negru in functie de
	//distanta acestuia fata de dreapta ce trebuie construita
	for (int i = 0; i < resolution; i++) {
		for (int j = start; j < end; j++) {
			
			distance = abs(2 * (resolution - i - 1) * pixelSize 
				+ (-j) * pixelSize + pixelSize / 2.0f) / sqrt(5);

			if (distance <= 3) {
				img->pixels[i][j] = 0;
			} else {
				img->pixels[i][j] = 255;
			}
		}
	}
	return NULL;
}

//Functia in care se creeaza thread-urile
void render(image* im) {

	img = im;
	pthread_t tid[num_threads];
	int thread_id[num_threads];

	for(int i = 0; i < num_threads; i++)
		thread_id[i] = i;

	//Se creeaza thread-urile
	for(int i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	//Se asteapta ca thread-urile sa isi termine executia
	for(int i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}

}

//Functia de scriere a imaginii rezultate in fisier
void writeData(const char* fileName, image* img) {

	FILE * file;
	file = fopen(fileName, "w");

	//Scriere tip imagine
	fprintf(file, "%s\n", img->type);
	
	//Scriere dimensiuni imagine
	fprintf(file, "%d %d\n", img->width, img->height);

	//Scriere maxval
	fprintf(file, "%d\n", img->maxval);

	//Scriere matrice de pixeli a imaginii
	for (int i = 0; i < img->height; i++) {
		for (int j = 0; j < img->width; j++) {
			fwrite(&(img->pixels[i][j]), sizeof(unsigned char), 1, file);
		}
	}

	fclose(file);
}

