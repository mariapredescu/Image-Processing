#ifndef HOMEWORK_H1
#define HOMEWORK_H1

//Structura pentru imagine
typedef struct {
	char type[2];	//tip imagine - P5(alb-negru) sau P6(color)
	int width, height; //dimensiuni imagine
	int maxval; //valoarea maxima a unui pixel sau a unui camp din pixel(r,g sau b)
	unsigned char** pixels; //matricea de pixeli a imaginii
}image;


void initialize(image *im);

void render(image* im);

void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */