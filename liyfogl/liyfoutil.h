#ifndef LIYFOUTIL_H
#define LIYFOUTIL_H

short liyfo_swap16(short val) {
	return (val << 8) | (val >> 8);
}

uint32_t liyfo_swap32(uint32_t x) {
	uint32_t y = (x >> 24) & 0xff;
	y |= (x >> 8) & 0xff00;
	y |= (x << 8) & 0xff0000;
	y |= (x << 24) & 0xff000000u;
	return y;
}

float liyfo_lieFloat32(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
	uint32_t intdata = a | (b << 8) | (c << 16) | (d << 24);
	//if(LE) intdata = liyfo_swap32(intdata);
	intdata = liyfo_swap32(intdata);
	float floatdata = 0;
	memcpy(&floatdata, &intdata, 4);
	return floatdata;
}

//LO

#define VD_LO_MX       1
#define VD_LO_TX0      2
#define VD_LO_TX1      4
#define VD_LO_TX2      8
#define VD_LO_TX3      16
#define VD_LO_TX4      32
#define VD_LO_TX5      64
#define VD_LO_TX6      128
#define VD_LO_TX7      256
#define VD_LO_VP       512
#define VD_LO_VPIDX8   1024
#define VD_LO_VPIDX16  2048
#define VD_LO_VN       4096
#define VD_LO_VNIDX8   8192
#define VD_LO_VNIDX16  16384
#define VD_LO_VC0      32768
#define VD_LO_VC0IDX8  65536
#define VD_LO_VC0IDX16 131072
#define VD_LO_VC1      262144
#define VD_LO_VC1IDX8  524288
#define VD_LO_VC1IDX16 1048576


//HI

#define VD_HI_TC0      1
#define VD_HI_TC0IDX8  2
#define VD_HI_TC0IDX16 4
#define VD_HI_TC1      8
#define VD_HI_TC1IDX8  16
#define VD_HI_TC1IDX16 32
#define VD_HI_TC2      64
#define VD_HI_TC2IDX8  128
#define VD_HI_TC2IDX16 256
#define VD_HI_TC3      512
#define VD_HI_TC3IDX8  1024
#define VD_HI_TC3IDX16 2048
#define VD_HI_TC4      4096
#define VD_HI_TC4IDX8  8192
#define VD_HI_TC4IDX16 16384
#define VD_HI_TC5      32768
#define VD_HI_TC5IDX8  65536
#define VD_HI_TC5IDX16 131072
#define VD_HI_TC6      262144
#define VD_HI_TC6IDX8  524288
#define VD_HI_TC6IDX16 1048576
#define VD_HI_TC7      2097152
#define VD_HI_TC7IDX8  4194304
#define VD_HI_TC7IDX16 8388608


void liyfoOpcode_CPClearVat(unsigned char vatsel) {
	if(vatsel & 8) { //using 4th bit as lo/hi selection arbitrarily
		//hi
		vathi[vatsel & 7] = 0;
	} else {
		//lo
		vatlo[vatsel & 7] = 0;
	}
}

int liyfoPrim_vertsize(int desclo, int deschi) {
	int size = 0;
	if(desclo & VD_LO_MX ) size += 1;  //1 u8 per vert
	if(desclo & VD_LO_TX0) size += 1;
	if(desclo & VD_LO_TX1) size += 1;
	if(desclo & VD_LO_TX2) size += 1;
	if(desclo & VD_LO_TX3) size += 1;
	if(desclo & VD_LO_TX4) size += 1;
	if(desclo & VD_LO_TX5) size += 1;
	if(desclo & VD_LO_TX6) size += 1;
	if(desclo & VD_LO_TX7) size += 1;

	if(desclo & VD_LO_VP ) size += 12; //3 singles
	if(desclo & VD_LO_VPIDX8) size += 1;
	if(desclo & VD_LO_VPIDX16) size += 2;

	if(desclo & VD_LO_VN ) size += 12;
	if(desclo & VD_LO_VNIDX8) size += 1;
	if(desclo & VD_LO_VNIDX16) size += 2;

	if(desclo & VD_LO_VC0) size += 3;  //3 char (i suppose api converts 3f32 to 3u8)
	if(desclo & VD_LO_VC0IDX8) size += 1;
	if(desclo & VD_LO_VC0IDX16) size += 2;

	if(desclo & VD_LO_VC1) size += 3;
	if(desclo & VD_LO_VC1IDX8) size += 1;
	if(desclo & VD_LO_VC1IDX16) size += 2;
		

	if(deschi & VD_HI_TC0) size += 8;  //2 singles. i suppose if you send 1d texcord it converts to 2d
	if(deschi & VD_HI_TC0IDX8) size += 1;
	if(deschi & VD_HI_TC0IDX16) size += 2;
	if(deschi & VD_HI_TC1) size += 8; 
	if(deschi & VD_HI_TC1IDX8) size += 1;
	if(deschi & VD_HI_TC1IDX16) size += 2;
	if(deschi & VD_HI_TC2) size += 8; 
	if(deschi & VD_HI_TC2IDX8) size += 1;
	if(deschi & VD_HI_TC2IDX16) size += 2;
	if(deschi & VD_HI_TC3) size += 8; 
	if(deschi & VD_HI_TC3IDX8) size += 1;
	if(deschi & VD_HI_TC3IDX16) size += 2;
	if(deschi & VD_HI_TC4) size += 8; 
	if(deschi & VD_HI_TC4IDX8) size += 1;
	if(deschi & VD_HI_TC4IDX16) size += 2;
	if(deschi & VD_HI_TC5) size += 8; 
	if(deschi & VD_HI_TC5IDX8) size += 1;
	if(deschi & VD_HI_TC5IDX16) size += 2;
	if(deschi & VD_HI_TC6) size += 8; 
	if(deschi & VD_HI_TC6IDX8) size += 1;
	if(deschi & VD_HI_TC6IDX16) size += 2;
	if(deschi & VD_HI_TC7) size += 8; 
	if(deschi & VD_HI_TC7IDX8) size += 1;
	if(deschi & VD_HI_TC7IDX16) size += 2;
	return size;
}

void liyfo_printMtx(float * m) {
	printf("Mtx\n");
	for(int i = 0; i < 16; i++) {
		printf("%f, ", m[i]);
		if((i+1)%4==0)printf("\n");
	}
}

#endif
