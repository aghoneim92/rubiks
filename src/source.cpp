#include <globalvariables.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <Angel.h>
#include <material.h>
#include <light_source.h>
#if !CROSS_PLATFORM
#include <Windows.h>
#include <mmsystem.h>
#include <sapi.h>
#endif
#include <ctime>
#include <rubikscube.h>
#include "initfunctions.h"
#include "animations.h"
#include "inputfunctions.h"


void renderAll(){
	for (unsigned int i=0;i<shapes.size();i++){
		//send data of shape[i] to shader

		glUniform4fv(light_position_loc, 1, lights[0]->position);
		glUniform4fv(eye_position_loc, 1, eye);
		glUniform4fv(ambient_product_loc, 1, lights[0]->ambient * shapes[i]->material.ambientK);
		glUniform4fv(diffuse_product_loc, 1, lights[0]->diffuse * shapes[i]->material.diffuseK);
		glUniform4fv(specular_product_loc, 1, lights[0]->specular * shapes[i]->material.specularK);
		glUniform1f(shininess_loc, shapes[i]->material.shininess);
		glUniform3fv(scale_loc,1,shapes[i]->scale);
		glUniform3fv(translation_loc,1,shapes[i]->translation);
		glUniform3fv(rotation_loc,1,shapes[i]->rotation);
		glUniform3fv(rotation_after_loc,1,shapes[i]->rotationAfter);
		glUniform1i(overlay_loc,shapes[i]->overlay);
		shapes[i]->render();
	}
	
	for(unsigned int i=0; i<noise.size(); i++){
		glUniform4fv(light_position_loc, 1, lights[0]->position);
		glUniform4fv(eye_position_loc, 1, eye);
		glUniform4fv(ambient_product_loc, 1, lights[0]->ambient * noise[i]->material.ambientK);
		glUniform4fv(diffuse_product_loc, 1, lights[0]->diffuse * noise[i]->material.diffuseK);
		glUniform4fv(specular_product_loc, 1, lights[0]->specular * noise[i]->material.specularK);
		glUniform1f(shininess_loc, noise[i]->material.shininess);
		glUniform3fv(scale_loc,1,noise[i]->scale);
		glUniform3fv(translation_loc,1,noise[i]->translation);
		glUniform3fv(rotation_loc,1,noise[i]->rotation);
		glUniform3fv(rotation_after_loc,1,noise[i]->rotationAfter);
		glUniform1i(overlay_loc,noise[i]->overlay);
		noise[i]->render();
	}
}

const vec4 vecY(0,1,0,0);

void display()
{   
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	mv = LookAt(eye, at ,up);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, mv);
	renderAll();
	glutSwapBuffers();
}

void redisplay(int unused){
	glutPostRedisplay();
	glutTimerFunc(16,redisplay,0);
}

int main( int argc, char **argv ){
	srand((unsigned int)time(NULL));
	glutInit( &argc, argv );
	if(fullScreen){
		WindowHeight = (GLfloat)glutGet(GLUT_SCREEN_HEIGHT);
		WindowWidth = (GLfloat)glutGet(GLUT_SCREEN_WIDTH);
	}
	glutInitDisplayMode( GLUT_RGBA  | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE );
	// glEnable(GL_MULTISAMPLE_ARB);
	// glEnable (GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutInitWindowSize( (int)(WindowWidth),(int) WindowHeight);
	glutCreateWindow( "Rubik's Cube" );
	


	if(fullScreen) glutFullScreen();

//	glewInit();

	init();
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc(special);
//	glutCloseFunc( cleanup );
	glutTimerFunc(16,redisplay,0);
	glLineWidth(1);
	glutTimerFunc(10,floatView,0);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}
