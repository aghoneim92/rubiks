#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H
#define CROSS_PLATFORM 1
#include "Angel.h"
#include "drawable.h"
#include "rubikscube.h"
#include <vector>
#include <string>

// SHADER Variable Locations

//Viewing
extern GLuint model_view_loc;
extern GLuint projection_loc;

//Color
extern GLuint light_position_loc;
extern GLuint eye_position_loc;
extern GLuint ambient_product_loc;
extern GLuint diffuse_product_loc;
extern GLuint specular_product_loc;
extern GLuint shininess_loc;

//Transform
extern GLuint scale_loc;
extern GLuint rotation_loc;
extern GLuint rotation_after_loc;
extern GLuint translation_loc;

//SFX
extern GLuint wavetime_loc;
extern GLuint wavewidth_loc;
extern GLuint waveheight_loc;
extern GLuint grayscale_loc;
extern GLuint water_loc;
extern GLuint dark_loc;
extern GLuint overlay_loc;
extern GLuint windowwidth_loc;
extern GLuint windowheight_loc;
extern GLuint redchrome_loc;
extern GLuint greenchrome_loc;
extern GLuint bluechrome_loc;
extern GLuint left_loc;
//Viewing Variables
extern const bool fullScreen;

extern GLfloat WindowWidth;
extern GLfloat WindowHeight;


extern float theta;
extern float phi;
extern float radius;

enum ViewingMode {
	ORTHO, PERSPECTIVE_WITH_FRUSTUM, PERSPECTIVE_WITH_FOV
};

extern ViewingMode viewMode;

// viewing volume
extern GLfloat left, right;
extern GLfloat bottom, top;
extern GLfloat zNear, zFar;
extern float zoomFactor;
extern float fovy; // field of view in angles with respect to y axis

extern vec4 eye;
extern mat4 mv;
extern mat4 mp;

extern std::vector<Drawable*> shapes;
extern std::vector<LightSource*> lights;
extern std::vector<Drawable*> noise;
extern std::vector<Material> GlowStep;

extern char moveTypes[];

extern RubiksCube* rubiksCube;

extern GLfloat waveTime, waveWidth, waveHeight, wavePeriod, xScaleAmp, yScaleAmp, zScaleAmp;
extern const GLfloat sinArg;
extern bool grayScale, waterEffect, darkEffect, noisy, lsdEffect, harlemShaked;
extern GLfloat redEffect, greenEffect, blueEffect;

extern vec4 at;
extern vec4 up;

extern char dir;
extern char moveType;
extern int faceIndex;
extern int shuffleIndex;
extern short rotDir;
extern bool rotDone;
extern bool faceChanged;
extern short direction;
extern char newGlowFace;
extern int newGlowIndex;
extern int glowIndex;
extern char glowFace;

extern const int nEasterEggs;
extern const std::string easterEggs[];

extern std::string easterEgg;

extern bool shuffled;
extern bool start;

extern volatile bool stopAnim;
#endif
