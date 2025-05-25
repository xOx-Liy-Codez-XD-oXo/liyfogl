#define GL_SILENCE_DEPRECATION 1
#define LE 1

#define LIYFO_VERBOSE

#include "liyfogl/liyfogl.h"

void *gpfifo;
long long fifosize;

int main(int argc, char **argv) {
	
	//fifosize = liyfo_loadFifodumpFromFile("120.fifo", &gpfifo);	
	fifosize = liyfo_loadDolphinFifodumpFromFile("scissor.dff", &gpfifo);	
	if(fifosize == 0) { printf("oop\n"); return 0; }
	if(gpfifo == NULL) { printf("what\n"); return 0; }
	
	liyfo_Init(argc, argv);

	liyfo_setFifoDeets(&gpfifo, fifosize);

	glutMainLoop();

	free(gpfifo);
	return 0;
}
