#ifndef HOMEWORK_H
#define HOMEWORK_H

//Structura pentru imagine
typedef struct {
	char type[2];	//tip imagine - P5(alb-negru) sau P6(color)
	int width, height; //dimensiuni imagine
	int maxval; //valoarea maxima a unui pixel sau a unui camp din pixel(r,g sau b)
	unsigned char** pixels; //matricea de pixeli a imaginii
}image;


void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */