#ifndef LIYFOGRAPHIC_H
#define LIYFOGRAPHIC_H

void liyfoGraphic_LoadProjection(float * data) {
	memset(&projectionMtx, 0, 64);
	projectionMtx[0][0] = data[0];
	projectionMtx[0][1] = 0.0f;
	projectionMtx[0][2] = data[1]; //0
	projectionMtx[0][3] = 0.0f;

	projectionMtx[1][0] = 0.0f;
	projectionMtx[1][1] = data[2];
	projectionMtx[1][2] = data[3]; //0
	projectionMtx[1][3] = 0.0f;

	projectionMtx[2][0] = 0.0f;
	projectionMtx[2][1] = 0.0f;
	projectionMtx[2][2] = data[4];
	projectionMtx[2][3] = data[5];

	projectionMtx[3][0] = 0.0f;
	projectionMtx[3][1] = 0.0f;
	projectionMtx[3][2] = -1.0f;
	projectionMtx[3][3] = 0.0f;

	liyfo_printMtx(projectionMtx);

	glMatrixMode(GL_PROJECTION);
	glLoadTransposeMatrixf((GLfloat *)&projectionMtx);
}

void liyfoGraphic_LoadModelview(float * data) {
	matrix[0][0] = data[0];
	matrix[1][0] = data[1];
	matrix[2][0] = data[2];
	matrix[3][0] = data[3];

	matrix[0][1] = data[4];
	matrix[1][1] = data[5];
	matrix[2][1] = data[6];
	matrix[3][1] = data[7];

	matrix[0][2] = data[8];
	matrix[1][2] = data[9];
	matrix[2][2] = data[10];
	matrix[3][2] = data[11];

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat *)&matrix);
}

void liyfoGraphic_drawTrisConfig(uint8_t vatsel, uint16_t vertcount,
				float *vertpos,
				float *vertnor,
				float * vertcol
				) {
	uint32_t config = vatlo[vatsel];
	//if(vertcount != 2514) return;
	glBegin(GL_TRIANGLES);
	for(long i = 0; i < vertcount; i++) {
		int i3 = i * 3;
		if(config & VD_LO_VC0) glColor3f(vertcol[i3], vertcol[i3+1], vertcol[i3+2]);
		if(config & VD_LO_VP) glVertex3f(vertpos[i3], vertpos[i3+1], vertpos[i3+2]);
	}
	glEnd();
}

#endif