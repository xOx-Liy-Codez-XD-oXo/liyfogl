#ifndef LIYFOFILE_H
#define LIYFOFILE_H

long long liyfo_loadFifodumpFromFile(char * filename, void ** fifo) {
	FILE *fifofile = fopen(filename, "r");
	if(fifofile == NULL) { 
		#ifdef LIYFO_VERBOSE
		printf("Could not open fifofile\n"); 
		#endif
		return 0; 
	}
	int cur;
	long long fifosize = 0;
	do {
		cur = fgetc(fifofile);
		fifosize++;
	} while (cur != EOF);
	#ifdef LIYFO_VERBOSE
	printf("Loading %lld byte fifo from file\n", fifosize);
	#endif
	*fifo = malloc(fifosize * 4);
	fclose(fifofile);
	fifofile = fopen(filename, "r");
	for(long i = 0; i < fifosize; i++) {
		cur = fgetc(fifofile);
		if(cur == EOF) break;
		unsigned char * charfo = *fifo;
		charfo[i] = cur & 255;
	}
	fclose(fifofile);
	#ifdef LIYFO_VERBOSE
	printf("Done\n");
	#endif
	return fifosize;
}

long long liyfo_loadDolphinFifodumpFromFile(char * filename, void ** fifo) {
	FILE *fifofile = fopen(filename, "r");
	if(fifofile == NULL) { 
		#ifdef LIYFO_VERBOSE
		printf("Could not open fifofile\n"); 
		#endif
		return 0; 
	}
	int cur;

	uint8_t magic1 = fgetc(fifofile);
	uint8_t magic2 = fgetc(fifofile);

	if(magic1 != 0xF0 || magic2 != 0xF1) { //magic word F0F1 lol
		printf("This is not a Dolphin fifo file\n"); 
		return 0; 
	}

	fseek(fifofile, 128, SEEK_SET);

	uint8_t fifolocationu8[4];
	for(int i = 0; i < 4; i++) {
		fifolocationu8[i] = fgetc(fifofile);
	}
	uint32_t fifolocation = *(uint32_t *)&fifolocationu8;
	if(!LE) fifolocation = liyfo_swap32(fifolocation);
	
	printf("fifolocation at %d\n", fifolocation);

	fseek(fifofile, fifolocation, SEEK_SET);

	long long fifosize = 0;
	do {
		cur = fgetc(fifofile);
		fifosize++;
	} while (cur != EOF);
	#ifdef LIYFO_VERBOSE
	printf("Loading %lld byte fifo from file\n", fifosize);
	#endif
	*fifo = malloc(fifosize * 4);
	
	fseek(fifofile, fifolocation, SEEK_SET);

	for(long i = 0; i < fifosize; i++) {
		cur = fgetc(fifofile);
		if(cur == EOF) break;
		unsigned char * charfo = *fifo;
		charfo[i] = cur & 255;
	}
	fclose(fifofile);
	#ifdef LIYFO_VERBOSE
	printf("Done\n");
	#endif
	return fifosize;
}

#endif
