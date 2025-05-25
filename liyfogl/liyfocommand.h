#ifndef LIYFOCOMMAND_H
#define LIYFOCOMMAND_H

//lower 3 bits of opcode is vtx fmt index

#define GO_MASK        248 //0b11111000

#define GO_NOP         0x00  //nop
#define GO_LOAD_CP_REG 0x08  //load cp reg
#define GO_LOAD_XF_REG 0x10  //load xf reg
#define GO_LOAD_INDX_A 0x20  //load indx a
#define GO_LOAD_INDX_B 0x28  //load indx b
#define GO_LOAD_INDX_C 0x30  //load indx c
#define GO_LOAD_INDX_D 0x38  //load indx d
#define GO_CALL_DL     0x40  //call dl
#define GO_IDK_MRX_LOL 0x44  //unknown metrics
#define GO_IVL_VTXCASH 0x48  //invalidate vtx cache
#define GO_LOAD_BP_REG 0x61  //load bp reg
#define GO_BEGIN_QUADS 0x80  //quads
#define GO_BEGIN_QUADZ 0x88  //quads again
#define GO_BEGIN_TRIS  0x90  //triangles
#define GO_BEGIN_TRIST 0x98  //triangle strip
#define GO_BEGIN_TRIFA 0xA0  //triangle fan
#define GO_BEGIN_LINES 0xA8  //lines
#define GO_BEGIN_LINST 0xB0  //linestrip
#define GO_BEGIN_POINT 0xB8  //points
#define GO_END         0xBF  //prim end

int liyfoOpcode_IsPrim(unsigned char opcode) {
	opcode &= GO_MASK;
	if(
		opcode == 0x80 || //quads
		opcode == 0x88 || //quads again
		opcode == 0x90 || //triangles
		opcode == 0x98 || //triangle strip
		opcode == 0xA0 || //triangle fan
		opcode == 0xA8 || //lines
		opcode == 0xB0 || //linestrip
		opcode == 0xB8    //points
	) return 1;
	return 0;
}

int liyfoOpcode_validOpcode(unsigned char opcode) {
	if(
		opcode == 0x00 || //nop
		opcode == 0x08 || //load cp reg
		opcode == 0x10 || //load xf reg
		opcode == 0x20 || //load indx a
		opcode == 0x28 || //load indx b
		opcode == 0x30 || //load indx c
		opcode == 0x38 || //load indx d
		opcode == 0x40 || //call dl
		opcode == 0x44 || //unknown metrics
		opcode == 0x48 || //invalidate vtx cache
		opcode == 0x61    //load bp reg
	) return 1;
	if(liyfoOpcode_IsPrim(opcode)) return 1;
	return 0;
}

int liyfoOpcode_commandFixedSize(unsigned char opcode) {
	//-1 if size dependent on command data or context
	//Size returned includes the opcode

	switch(opcode) {
	case GO_LOAD_CP_REG:
		return 6;
		break;
	case GO_LOAD_INDX_A:
	case GO_LOAD_INDX_B:
	case GO_LOAD_INDX_C:
	case GO_LOAD_INDX_D:
		return 5;
		break;
	case GO_CALL_DL:
		return 9;
		break;
	case GO_IDK_MRX_LOL:
		return 1; //go check dolphin code
		break;
	case GO_IVL_VTXCASH:
		return 1;
		break;
	case GO_LOAD_BP_REG:
		return 5;
		break;
	}
	return -1;
}

int liyfoOpcode_LoadCpReg(unsigned char addr, unsigned char val0, unsigned char val1, unsigned char val2, unsigned char val3) {
	//0 and 1 unused im prty sure
	#ifdef LIYFO_VERBOSE
	printf("LOAD_CP_REG %X %X%X%X%X, \n", addr, val0, val1, val2, val3);
	#endif
	unsigned char vatSel = addr & 7;
	switch(addr) {
		case 0x50:
			#ifdef LIYFO_VERBOSE
			printf("	VCD LO 0:\n");
			#endif
			liyfoOpcode_CPClearVat(vatSel & 247); // 1111 0111
			if(val2 & 1) {
				#ifdef LIYFO_VERBOSE
				printf("	MATRIX INDEX\n");
				#endif
				vatlo[vatSel] |= VD_LO_MX;
			}
			if((val2 & 0x06) == 0x02) {
				#ifdef LIYFO_VERBOSE
				printf("	POSITION DIRECT\n");
				#endif
				vatlo[vatSel] |= VD_LO_VP;
			}
			if((val2 & 0x06) == 0x04) {
				#ifdef LIYFO_VERBOSE
				printf("	POSITION INDEX 8\n");
				#endif
				vatlo[vatSel] |= VD_LO_VPIDX8;
			}
			if((val2 & 0x06) == 0x06) {
				#ifdef LIYFO_VERBOSE
				printf("	POSITION INDEX 16\n");
				#endif
				vatlo[vatSel] |= VD_LO_VPIDX16;
			}
			if((val2 & 0x18) == 0x08) {
				#ifdef LIYFO_VERBOSE
				printf("	NORMAL DIRECT\n");
				#endif
				vatlo[vatSel] |= VD_LO_VN;
			}
			if((val2 & 0x18) == 0x10) {
				#ifdef LIYFO_VERBOSE
				printf("	NORMAL DIRECT INDEX 8\n");
				#endif
				vatlo[vatSel] |= VD_LO_VNIDX8;
			}
			if((val2 & 0x18) == 0x18) {
				#ifdef LIYFO_VERBOSE
				printf("	NORMAL DIRECT INDEX 16\n");
				#endif
				vatlo[vatSel] |= VD_LO_VNIDX16;
			}
			if((val2 & 0x20)) { 
				#ifdef LIYFO_VERBOSE
				printf("	COL0 DIRECT\n");
				#endif
				vatlo[vatSel] |= VD_LO_VC0;
			}
			if((val2 & 0x80)) { 
				#ifdef LIYFO_VERBOSE
				printf("	COL1 DIRECT\n");
				#endif
				vatlo[vatSel] |= VD_LO_VC1;
			}
			break;
		case 0x60:
			#ifdef LIYFO_VERBOSE
			printf("	VCD HI 0\n");
			#endif
			liyfoOpcode_CPClearVat(vatSel | 8); // 0000 1000
			break;
	}
	return 0;
}	

int liyfoOpcode_LoadXfReg(unsigned short length, unsigned short addr, uint8_t * data) {
	#ifdef LIYFO_VERBOSE
	printf("LOAD_XF_REG %d %X\n", length, addr);
	#endif
	float *xfFloatdata = malloc(length * 4);
	for(int i = 0; i < length; i++) {
		xfFloatdata[i] = liyfo_lieFloat32(data[(i*4)+0], data[(i*4)+1], data[(i*4)+2], data[(i*4)+3]);
		#ifdef LIYFO_VERBOSE
		printf("	%f\n", xfFloatdata[i]);	
		#endif
	}
	switch(addr) {
		case 0: //modelview 0
			liyfoGraphic_LoadModelview(xfFloatdata);
			break;
		case 0x1020: //projection
			liyfoGraphic_LoadProjection(xfFloatdata);
			break;
	}
	free(xfFloatdata);
	return 1;
}

int liyfoOpcode_BeginTris(unsigned char vatsel, unsigned short vcount, uint8_t * data) {
	int vsize = liyfoPrim_vertsize(vatlo[vatsel], vathi[vatsel]);
	//printf("tris. %d verts, %d big\n", vcount, vsize);
	//lets make position and color arrays
	//gl wants color in float too
	uint32_t config = vatlo[vatsel];
	float *vertpos;
	float *vertnor;
	float *vertcol;
	if(config & VD_LO_VP) vertpos = malloc(3 * vcount * 4);
	if(config & VD_LO_VN) vertnor = malloc(3 * vcount * 4);
	if(config & VD_LO_VC0) vertcol = malloc(3 * vcount * 4);

	//idk how to handle this dynamically right now
	for(long long i = 0; i < vcount; i++) {
		vertpos[ i * 3   ] =  liyfo_lieFloat32( data[(i * vsize)  ], data[(i * vsize)+1], data[(i * vsize)+2], data[(i * vsize)+3] );
		vertpos[(i * 3)+1] =  liyfo_lieFloat32( data[(i * vsize)+4], data[(i * vsize)+5], data[(i * vsize)+6], data[(i * vsize)+7] );
		vertpos[(i * 3)+2] =  liyfo_lieFloat32( data[(i * vsize)+8], data[(i * vsize)+9], data[(i * vsize)+10], data[(i * vsize)+11] );
		
		uint8_t ivcolr = data[(i * vsize)+12]; 
		uint8_t ivcolg = data[(i * vsize)+13]; 
		uint8_t ivcolb = data[(i * vsize)+14];
		float fvcolr = (float)ivcolr / 255.0f;
		float fvcolg = (float)ivcolg / 255.0f;
		float fvcolb = (float)ivcolb / 255.0f;
		vertcol[i*3] = fvcolr;
		vertcol[(i*3)+1] = fvcolg;
		vertcol[(i*3)+2] = fvcolb;
		
		/*#ifdef LIYFO_VERBOSE
		printf("		%x%x%x%x %x%x%x%x %x%x%x%x %x%x%x\n", data[(i * vsize)  ], data[(i * vsize)+1], data[(i * vsize)+2], 
		data[(i * vsize)+3], data[(i * vsize)+4], data[(i * vsize)+5], data[(i * vsize)+6], data[(i * vsize)+7], data[(i * vsize)+8], 
		data[(i * vsize)+9], data[(i * vsize)+10], data[(i * vsize)+11], data[(i * vsize)+12], data[(i * vsize)+13], data[(i * vsize)+14]);
		printf("		(%f, %f, %f) (%f %f %f)\n", vertpos[i*3], vertpos[(i*3)+1], vertpos[(i*3)+2], fvcolr, fvcolg, fvcolb);
		#endif*/

	}
	liyfoGraphic_drawTrisConfig(vatsel, vcount,
	                            vertpos,
	                            NULL,
	                            vertcol);

	if(config & VD_LO_VP) free(vertpos);
	if(config & VD_LO_VN) free(vertnor);
	if(config & VD_LO_VC0) free(vertcol);
	return 1;
}

#endif
