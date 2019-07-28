#include "inputfunctions.h"
#include <cfloat>

void special(int key, int x, int y){
	switch(key){
	case GLUT_KEY_UP:
		newGlowIndex = glowIndex-1;
		if(newGlowIndex<0)
			newGlowIndex = 2;
		newGlowFace = glowFace;
		faceChanged = true;
		break;
	case GLUT_KEY_DOWN:
		newGlowIndex = (glowIndex+1) % 3;
		newGlowFace = glowFace;
		faceChanged = true;
		break;
	case GLUT_KEY_RIGHT:
		newGlowIndex = glowIndex;
		newGlowFace = glowFace-1;
		if(newGlowFace<0)
			newGlowFace = 2;
		faceChanged = true;
		break;
	case GLUT_KEY_LEFT:
		newGlowIndex = glowIndex;
		newGlowFace = (glowFace+1)%3;
		faceChanged = true;
		break;
	}
}

GLdouble downC[3], upC[3], deltaC[3], averageC[3];

void checkSwipe(int x, int y, bool up){
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble modelview[16];
	int i;
	for(i=0; i<16; i++)
		modelview[i] = mv[i%4][i/4];

	GLdouble projection[16];
	for(i=0; i<16; i++)
		projection[i] = mp[i%4][i/4];

	y = viewport[3] - y;

	GLfloat z;
	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
	std::cout<<"z depth is reported as: "<<z<<std::endl;
	GLdouble posX, posY, posZ;
	gluUnProject((double)x,(double)y,z,modelview,projection,viewport,&posX,&posY,&posZ);

	if(up){
		bool keepGlowing = false;
		short moveCode=0;
		upC[0] = posX, upC[1] = posY, upC[2] = posZ;
		int i;
		for(i=0; i<3; i++){
			deltaC[i]= upC[i] - downC[i];
			averageC[i] = downC[i] + deltaC[i]/2.0;
		}

		if(abs(deltaC[0])>20)
			moveCode|=1;
		if(abs(deltaC[1])>20)
			moveCode|= (1<<1);
		if(abs(deltaC[2])>20)
			moveCode|= (1<<2);

		switch(moveCode){
		case 1:
			if(abs(averageC[2])<=72.0){
				moveType = 'f';
				rotDir = (deltaC[0]>0?-1:1);
				if(averageC[2]> -75 && averageC[2]<-12.5)
					faceIndex = 0;
				else if(averageC[2]>-12.5 && averageC[2] <12.5)
					faceIndex = 1;
				else if(averageC[2]<75)
					faceIndex = 2;
				rotateFace(0);
				break;
			}
			moveType = 'h';
			rotDir = (deltaC[0]>0?1:-1);
			rotDir*= (averageC[2]>0?1:-1);
			if(averageC[1]> -75 && averageC[1]<-12.5)
				faceIndex = 0;
			else if(averageC[1]>-12.5 && averageC[1] <12.5)
				faceIndex = 1;
			else if(averageC[1]<75)
				faceIndex = 2;
			rotateFace(0);
			break;
		case 2:
			if(averageC[0]>-74 && averageC[0]<74){
				moveType = 's';
				rotDir = (deltaC[1]>0?1:-1);
				rotDir*= (averageC[2]>0?-1:1);
				if(averageC[0]> -75 && averageC[0]<-12.5)
					faceIndex = 0;
				else if(averageC[0]>-12.5 && averageC[0] <12.5)
					faceIndex = 1;
				else if(averageC[0]<75)
					faceIndex = 2;
				rotateFace(0);
			}
			else{
				moveType = 'f';
				rotDir = (deltaC[1]>0?1:-1);
				rotDir*= (averageC[0]>0?1:-1);
				if(averageC[2]> -75 && averageC[2]<-25)
					faceIndex = 0;
				else if(averageC[2]>-25 && averageC[2] <25)
					faceIndex = 1;
				else if(averageC[2]<75)
					faceIndex = 2;
				rotateFace(0);
			}
			break;
		case 4:
			if(averageC[0]<-74 || averageC[0]>74){
				moveType = 'h';
				rotDir = (deltaC[2]>0)?1:-1;
				rotDir *= (averageC[0]>0)?-1:1;
				if(averageC[1]> -75 && averageC[1]<-12.5)
					faceIndex = 0;
				else if(averageC[1]>-12.5 && averageC[1] <12.5)
					faceIndex = 1;
				else if(averageC[1]<75)
					faceIndex = 2;
				rotateFace(0);
			}
			break;
		default:
			keepGlowing = true;
		}
		if(!keepGlowing){
			faceChanged = true;
			newGlowIndex = 3;
		}
	}
	else
		downC[0] = posX, downC[1] = posY, downC[2] = posZ;

	std::cout<<"You just clicked at this location in space: x "<<posX<<" y "<<posY<<" z "<<posZ<<std::endl;
}

void mouse(int a, int up, int x, int y){
	if(dir=='l')
		return;
	if(!start && up){
		//start = true;
	}
	else{
		if(shuffled){
			checkSwipe(x,y, up);
		}
	}

}

void anyKey(unsigned char key){
	easterEgg.append(1, key);
	int i;
	bool oneMatchAtLeast = false;

	for(i=0; i<nEasterEggs; i++)
		if(easterEggs[i].compare(0,std::min(easterEggs[i].size(),easterEgg.size()),easterEgg,0,std::min(easterEggs[i].size(),easterEgg.size()))==0){
			if(easterEgg.compare(easterEggs[i])==0){
				switch(i){
				case 0:
					glUniform1i(grayscale_loc, grayScale=!grayScale);
					if(grayScale){
						for(unsigned int i=0; i<noise.size(); i++){
							noise[i]->scale = vec3(1.0F,1.0F,1.0F);
						}
						animateNoise(0);
					}
					else{
						for(unsigned int i=0; i<noise.size(); i++)
							noise[i]->scale = vec3(0,0,0);
					}
					break;
				case 1:
					glUniform1i(water_loc, waterEffect=!waterEffect);
					if(waterEffect)
						animateWater(0);
					break;
				case 2:
					glUniform1i(dark_loc, darkEffect=!darkEffect);
					break;
				case 3:
					lsdEffect = !lsdEffect;
					animateLSD(0);
					break;
				case 4:
					if(redEffect-1.0f<FLT_EPSILON){
						glUniform1f(redchrome_loc, redEffect=2.0f);
						glUniform1i(grayscale_loc, grayScale=true);
					}
					else{
						glUniform1f(redchrome_loc, redEffect=1.0f);
						if(greenEffect-1.0f<FLT_EPSILON && blueEffect-1.0f<FLT_EPSILON)
							glUniform1i(grayscale_loc, grayScale=false);
					}
					break;
				case 5:
					if(greenEffect-1.0f<FLT_EPSILON){
						glUniform1f(greenchrome_loc, greenEffect=2.0f);
						glUniform1i(grayscale_loc, grayScale=true);
					}
					else{
						glUniform1f(greenchrome_loc, greenEffect=1.0f);
						if(redEffect-1.0f<FLT_EPSILON && blueEffect-1.0f<FLT_EPSILON)
							glUniform1i(grayscale_loc, grayScale=false);
					}
					break;
				case 6:
					if(blueEffect-1.0f<FLT_EPSILON){
						glUniform1f(bluechrome_loc, blueEffect=2.0f);
						glUniform1i(grayscale_loc, grayScale=true);
					}
					else{
						glUniform1f(bluechrome_loc, blueEffect=1.0f);
						if(redEffect-1.0f<FLT_EPSILON && greenEffect-1.0f<FLT_EPSILON)
							glUniform1i(grayscale_loc, grayScale=false);
					}
					break;
				case 7:
					if(harlemShaked){
						unsigned int i;
						for(i=0; i<shapes.size(); i++){
							shapes[i]->scale = vec3(1.0f,1.0f,1.0f);
							shapes[i]->rotation = vec3(0,0,0);
							shapes[i]->translation = vec3(0,0,0);
						}
						harlemShaked = false;
					}
					else{
//						PlaySound(TEXT("h.wav"), NULL, SND_ASYNC);
						animateHarlemShake1(0);
					}
					break;
				}
				easterEgg.clear();
				break;
			}
			oneMatchAtLeast = true;
		}

		if(!oneMatchAtLeast)
			easterEgg.clear();
}

void keyboard( unsigned char key, int x, int y )
{
	if(dir=='l')
		return;
	float deltaAng = 4;
	mat4 mx,my;
	switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit( EXIT_SUCCESS );
		break;
	case ' ':
		if(!start && up){
		start = true;
	}
		break;
	case 'a':
		my = RotateY(deltaAng);
		eye = my*eye;
		break;
	case 'd':
		my = RotateY(-deltaAng);
		eye = my*eye;
		break;
	case 'w':
		mx = RotateX(deltaAng);
		eye = mx*eye;
		break;
	case 's':
		mx = RotateX(-deltaAng);
		eye = mx*eye;
		break;
	case '-':
		if(rotDone){
		rotDone=false;
		rotDir = -1;
		moveType = moveTypes[glowFace];
		faceIndex = glowIndex;
		rotateFace(0);
#if !CROSS_PLATFORM
		PlaySound(TEXT("drag.wav"), NULL, SND_ASYNC);
#endif
		}
		break;
	case '+':
		if(rotDone){
		rotDone=false;
		rotDir = 1;
		moveType = moveTypes[glowFace];
		faceIndex = glowIndex;
		rotateFace(0);
#if !CROSS_PLATFORM
		PlaySound(TEXT("drag.wav"), NULL, SND_ASYNC);
#endif
		}
		break;
	}
	anyKey(key);
}
