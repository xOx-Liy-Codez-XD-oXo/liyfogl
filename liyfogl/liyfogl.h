#ifndef LIYFOGL_H
#define LIYFOGL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


//state. lets just make state global what could happen. those gay gl wrappers do it
void **globalfifo;
long long globalfifosize;
unsigned int vatlo[8];
unsigned int vathi[8];
float projectionMtx[4][4];
float matrix[4][4];

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#else
#include <GL/OpenGL.h>
#include <GL/GLUT.h>
#endif

#include "liyfoutil.h"
#include "liyfofile.h"
#include "liyfographic.h"
#include "liyfocommand.h"

void liyfo_setFifoDeets(void ** fifo, long long fifosize) {
	globalfifo = fifo;
	globalfifosize = fifosize;
}

void liyfo_parseFifo() {
	long long fifosize = globalfifosize;
	unsigned char *charfo = *globalfifo;

	for(long long i = 0; i < fifosize;) {
		#ifdef LIYFO_VERBOSE
		printf("cmd %lld/0x%llX, \n", i, i);
		#endif
		if(i >= 0x18e03) {
			//printf("lily stop\n");
			//return;
		}

		int commandSize = liyfoOpcode_commandFixedSize(charfo[i]);

		if(commandSize == -1) { //command size varies on content

			if(charfo[i] == GO_NOP) {
				i++;
			} else if(charfo[i] == GO_LOAD_XF_REG) {
				short xfLength = (charfo[i+2] | (charfo[i+1] << 8)) + 1;
				unsigned short xfAddr = charfo[i+4] | (charfo[i+3] << 8);
				liyfoOpcode_LoadXfReg(xfLength, xfAddr, charfo + i + 5);
				i += 5 + (4 * xfLength);
			} else if(liyfoOpcode_IsPrim(charfo[i])) {
				unsigned char vatSel = charfo[i] & 7;
				unsigned char vcount0 = charfo[i+1]; unsigned char vcount1 = charfo[i+2];
				unsigned short vcount = (vcount1) | (vcount0 << 8);
				switch(charfo[i]) {
					case GO_BEGIN_TRIS:
						;
						liyfoOpcode_BeginTris(vatSel, vcount, charfo + i + 3);
						i += 3 + (vcount * liyfoPrim_vertsize(vatlo[vatSel], vathi[vatSel]));
						break;
					case GO_BEGIN_LINES:
						;
						i += 3 + (vcount * liyfoPrim_vertsize(vatlo[vatSel], vathi[vatSel]));
						break;
					default:
						;
						i += 3 + (vcount * liyfoPrim_vertsize(vatlo[vatSel], vathi[vatSel]));
						printf("prim. not implemented. continuing\n");
						break;
				}
			} else {
				printf("unknown at %lld, 0x%x. stop.\n", i, charfo[i]);
				return;
			}

		} else {

			//fixed size command handling
			switch(charfo[i]) {
				case GO_LOAD_CP_REG:
					;
					unsigned char cpRegAddr = charfo[i+1];
					unsigned char cpRegVal0 = charfo[i+2]; unsigned char cpRegVal1 = charfo[i+3];
					unsigned char cpRegVal2 = charfo[i+4]; unsigned char cpRegVal3 = charfo[i+5];
					liyfoOpcode_LoadCpReg(cpRegAddr, cpRegVal0, cpRegVal1, cpRegVal2, cpRegVal3);
					break;
				case GO_LOAD_BP_REG:
					;
					unsigned char bpRegAddr = charfo[i+1]; 
					unsigned char bpRegVal0 = charfo[i+2]; unsigned char bpRegVal1 = charfo[i+3]; 
					unsigned char bpRegVal2 = charfo[i+4];
					#ifdef LIYFO_VERBOSE
					printf("LOAD_BP_REG %X, %X%X%X\n", bpRegAddr, bpRegVal0, bpRegVal1, bpRegVal2);
					#endif
					break;
				case GO_IVL_VTXCASH:
					//do nothing lol
					break;
				default:
					printf("fixedsize 0x%x not implemented yet\n", charfo[i]);
			}
			i += commandSize;

		}
	}
	printf("end of fifo reacht\n");
	return;
}


void liyfo_Mainloop() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	liyfo_parseFifo();
	glutSwapBuffers();
}

void liyfo_Init(int argc, char **argv) {
	for(int i = 0; i < 8; i++) {
		vatlo[i] = VD_LO_VP;
		vathi[i] = 0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(-1, -1);
	glutInitWindowSize(640, 480); 
	glutCreateWindow("LiyfoGL");

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);	
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0f, 1.0f);

	glutDisplayFunc(liyfo_Mainloop);
}

#endif
