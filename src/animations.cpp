#include "animations.h"

void rotateFace(int animIndex){
	if(animIndex==100){
		switch(moveType){
		case 'f':
			rubiksCube->finalizeRotationFront(faceIndex,rotDir);
			rubiksCube->updateIndicesFront(faceIndex,rotDir);
			break;
		case 'h':
			rubiksCube->finalizeRotationHorizontal(faceIndex,rotDir);
			rubiksCube->updateIndicesHorizontal(faceIndex,rotDir);
			break;
		case 's':
			rubiksCube->finalizeRotationSide(faceIndex,rotDir);
			rubiksCube->updateIndicesSide(faceIndex,rotDir);
			break;
		}
		if(!shuffled){
			shuffleIndex++;
			shuffle();
		}
		else{
			if(rubiksCube->checkWin()){
//				PlaySound(TEXT("win.wav"), NULL, SND_ASYNC);
			    initGlowStep();
				glowCube(0);

				
				
			}
		}
		rotDone=true;
		return;
	}
	if(moveType=='f'){
		rubiksCube->rotateFront(faceIndex,rotDir*M_PI/200);
	}
	else if(moveType=='h'){
		rubiksCube->rotateHorizontal(faceIndex,rotDir*M_PI/200);
	}
	else if(moveType=='s'){
		rubiksCube->rotateSide(faceIndex,rotDir*M_PI/200);
	}

	glutTimerFunc(10,rotateFace,animIndex+1);
}
void initGlowStep()
{ vec4 White=vec4(1,1,1,1);
	for (unsigned int i=0;i<shapes.size();i++){
	GlowStep.push_back(shapes[i]->material);
	GlowStep[i].ambientK=(White-GlowStep[i].ambientK)/50;
	GlowStep[i].ambientK.w=1;
	GlowStep[i].specularK=(White-GlowStep[i].specularK)/50;
	GlowStep[i].specularK.w=1;
	GlowStep[i].diffuseK=(White-GlowStep[i].diffuseK)/50;
	GlowStep[i].diffuseK.w=1;	
}
}
void shuffle(){
	const int n=1;
	if(shuffleIndex==n){
		shuffled = true;
		glowRims(0);
		return;
	}
	faceIndex = rand()%3;
	int moveDecision = rand()%3;
	switch(moveDecision){
	case 0:
		moveType = 'f';
		break;
	case 1:
		moveType = 'h';
		break;
	case 2:
		moveType = 's';
		break;
	}
#if !CROSS_PLATFORM
	PlaySound(TEXT("drag.wav"), NULL, SND_ASYNC);
#endif
	rotateFace(0);
}

void glowRims(int animIndex){
	if(rubiksCube->checkWin()){return;}
	const int totalFrames = 50;
	if(faceChanged){
		faceChanged = false;
		switch(moveTypes[glowFace]){
		case 'f':
			rubiksCube->resetRimsFront(glowIndex);
			break;
		case 's':
			rubiksCube->resetRimsSide(glowIndex);
			break;
		case 'h':
			rubiksCube->resetRimsHorizontal(glowIndex);
			break;
		}
		glowIndex = newGlowIndex;
		direction = 1;
		glowFace = newGlowFace;
		if(glowIndex == 3)
			return;
		glowRims(0);
		return;
	}

	if(animIndex==totalFrames){
		direction*=-1;
		glowRims(0);
		return;
	}
	switch(moveTypes[glowFace]){
	case 'f':
		rubiksCube->glowRimsFront(glowIndex,totalFrames,direction);
		break;
	case 's':
		rubiksCube->glowRimsSide(glowIndex,totalFrames,direction);
		break;
	case 'h':
		rubiksCube->glowRimsHorizontal(glowIndex,totalFrames,direction);
		break;
	}
	glutTimerFunc(10,glowRims,animIndex+1);
}

void glowCube(int animIndex){
	const int totalFrames = 50;

	if(animIndex==totalFrames){
		direction*=-1;
		glowCube(0);
		return;
	}
	for (unsigned int i=0;i<shapes.size();i++){
		shapes[i]->material.ambientK=shapes[i]->material.ambientK+(GlowStep[i].ambientK*direction);
		shapes[i]->material.ambientK.w=1;

		shapes[i]->material.specularK=shapes[i]->material.specularK+(GlowStep[i].specularK*direction);
		shapes[i]->material.specularK.w=1;

		shapes[i]->material.diffuseK=shapes[i]->material.diffuseK+(GlowStep[i].diffuseK*direction);
		shapes[i]->material.diffuseK.w=1;

	}
	
	
	glutTimerFunc(10,glowCube,animIndex+1);
}

void floatView(int animIndex){
	if(stopAnim){
		std::cout<<"Stopped!"<<std::endl;
		glutTimerFunc(10,floatView,animIndex);
		return;
	}
	if(animIndex==400 && dir=='l'){
#if !CROSS_PLATFORM
		PlaySound(TEXT("swoosh2.wav"), NULL, SND_ASYNC);
#endif
		animateAssemble(0);
		return;
	}
	else if(start && dir=='r'){
		shuffle();
		return;
	}
	mat4 my = RotateY(dir=='l'?0.5f:-0.5f);
	eye = my*eye;
	glutTimerFunc(10,floatView,animIndex+1);
}

void animateAssemble(int unused){
	#if !CROSS_PLATFORM
		PlaySound(TEXT("ShuffleNote.wav"), NULL, SND_ASYNC);
    #endif
	mat4 rotation = RotateY(-10);
	eye = rotation*eye;
	if(rubiksCube->assemble())
		glutTimerFunc(20,animateAssemble,0);
	else{
		dir = 'r';
		floatView(0);
	}
	
	return;
}

void animateWater(int unused){
	if(!waterEffect)
		return;
	waveTime+=wavePeriod/1000;
	glUniform1f(wavetime_loc,waveTime);
	glutTimerFunc((unsigned int)wavePeriod,animateWater,0);
}

void animateLSD(int animIndex){
	const int n=200;
	unsigned int i;
	if(!lsdEffect){
		for(i=0; i<shapes.size(); i++)
			shapes[i]->scale = vec3(1.0f,1.0f,1.0f);
		return;
	}
	if(animIndex==0){
		xScaleAmp = ((GLfloat)(rand()%200)+1) / 100.0f;
		yScaleAmp = ((GLfloat)(rand()%200)+1) / 100.0f;
		zScaleAmp = ((GLfloat)(rand()%200)+1) / 100.0f;
	}
	else if(animIndex==n){
		animateLSD(0);
		return;
	}
	GLfloat xScale = xScaleAmp*sin(2.0f*M_PI*animIndex/(n-1))+1.0f,
		yScale = yScaleAmp*sin(2.0f*M_PI*animIndex/(n-1))+1.0f,
		zScale = zScaleAmp*sin(2.0f*M_PI*animIndex/(n-1))+1.0f;
	for(i=0; i<shapes.size(); i++){
		shapes[i]->scale = vec3(xScale, yScale, zScale);
	}
	glutTimerFunc(10,animateLSD,animIndex+1);
}

void animateNoise(int animIndex){
	if(!grayScale)
		return;
	GLfloat x, y;
	unsigned int i;
	for(i=0; i<noise.size(); i++){
		x= 2*(rand()%2-0.5)*(rand()%((int)WindowWidth/6));
		y = 2*(rand()%2-0.5)*(rand()%((int)WindowHeight/3));
		noise[i]->translation = vec3(x,y,0);
	}
	glutTimerFunc(10, animateNoise, animIndex);
}

short shakeDir = 1;
const int maxX = 3, maxY = 3, maxZ=3;

enum shakeType {Float, Shake, Rotate};
short shakeTypes[3][3][3];
vec3 shakeMag[3][3][3];
bool squeezes[3][3][3] = {0};

void animateHarlemShake1(int animIndex){
	const int n=950;
	unsigned int i;
	if(animIndex==n){
		int j, k;
		int shakeDecision;
		for(i=0; i<3; i++)
			for(j=0; j<3; j++)
				for(k=0; k<3; k++){
					rubiksCube->translateCubelet(i,j,k,
						vec3( rand()%maxX+1, rand()%maxY+1, rand()%maxZ+1));
					shakeDecision = rand()%10;
					if(shakeDecision<6){
						shakeTypes[i][j][k] = Shake;
						shakeMag[i][j][k] = vec3(rand()%30+1, rand()%30+1, rand()%30+1);
						if(shakeDecision>=2)
							squeezes[i][j][k] = true;
					}
					else if(shakeDecision<9){
						shakeTypes[i][j][k] = Rotate;
						shakeMag[i][j][k] = vec3(rand()%360+1, rand()%360+1, rand()%360+1);
					}
					else{
						shakeTypes[i][j][k] = Float;
						shakeMag[i][j][k] = vec3(rand()%10, rand()%10, rand()%10);
					}
				}

				animateHarlemShake2(0);
				return;
	}
	if(animIndex%15==0)
		shakeDir*=-1;
	for(i=0; i<shapes.size(); i++){
		shapes[i]->translation = vec3(0,0, shapes[i]->translation.z+(shakeDir==1?2.5f:-2.5f));
	}
	glutTimerFunc(10, animateHarlemShake1, animIndex+1);
}

void animateHarlemShake2(int animIndex){
	const int n=800;
	if(animIndex==n){
		animateHarlemShake3(0);
		return;
	}
	if(animIndex%26==0)
		shakeDir*=-1;
	unsigned int i, j, k;
	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
			for(k=0; k<3; k++)
				switch(shakeTypes[i][j][k]){
				case Float:
					rubiksCube->translateCubeletNormal(i,j,k, shakeMag[i][j][k]*0.05f);
					break;
				case Shake:
					rubiksCube->translateCubeletNormal(i,j,k, shakeMag[i][j][k]*shakeDir*0.5f);
					if(squeezes[i][j][k])
						rubiksCube->scaleCubelet(i,j,k, sin(animIndex*2*M_PI/52.0f)*vec3( 1-10/shakeMag[i][j][k].x,
						1-10/shakeMag[i][j][k].y,
						1-10/shakeMag[i][j][k].z )+vec3(1.0f));
					break;
				case Rotate:

					rubiksCube->rotateCubelet(i,j,k, shakeDir*DegreesToRadians*shakeMag[i][j][k]/52.0f);
					break;
			}
			glutTimerFunc(10, animateHarlemShake2, animIndex+1);
}

void animateHarlemShake3(int animIndex){
	const int n=90;
	if(animIndex==n){
		harlemShaked = true;
		return;
	}
	unsigned int i, j, k;
	for(i=0; i<3; i++)
		for(j=0; j<3; j++)
			for(k=0; k<3; k++)
				switch(shakeTypes[i][j][k]){
				case Float:
					rubiksCube->translateCubeletNormal(i,j,k, shakeMag[i][j][k]*0.05f);
					break;
				case Shake:
					rubiksCube->translateCubeletNormal(i,j,k, shakeMag[i][j][k]*shakeDir*0.05f);
					if(squeezes[i][j][k])
						rubiksCube->scaleCubelet(i,j,k, sin(animIndex*2*M_PI/200.0f)*vec3( 1-10/shakeMag[i][j][k].x,
						1-10/shakeMag[i][j][k].y,
						1-10/shakeMag[i][j][k].z )+vec3(1.0f));
					break;
				case Rotate:

					rubiksCube->rotateCubelet(i,j,k, shakeDir*DegreesToRadians*shakeMag[i][j][k]/200.0f);
					break;
			}
			glutTimerFunc(10, animateHarlemShake3, animIndex+1);
}
