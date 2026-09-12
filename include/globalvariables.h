#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "Angel.h"
#include "drawable.h"
#include "rubikscube.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

// SHADER Variable Locations

// Viewing
extern GLuint model_view_loc;
extern GLuint projection_loc;

// Color
extern GLuint light_position_loc;
extern GLuint eye_position_loc;
extern GLuint ambient_product_loc;
extern GLuint diffuse_product_loc;
extern GLuint specular_product_loc;
extern GLuint shininess_loc;

// Transform
extern GLuint scale_loc;
extern GLuint rotation_loc;
extern GLuint rotation_after_loc;
extern GLuint translation_loc;

// SFX
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

// Picking
extern GLuint depth_pass_loc;

// Viewing Variables
extern bool fullScreen;

extern GLfloat WindowWidth;
extern GLfloat WindowHeight;

extern float theta;
extern float phi;
extern float radius;

enum ViewingMode
{
	ORTHO,
	PERSPECTIVE_WITH_FRUSTUM,
	PERSPECTIVE_WITH_FOV
};

extern ViewingMode viewMode;

// viewing volume
extern GLfloat viewLeft, viewRight;
extern GLfloat viewBottom, viewTop;
extern GLfloat zNear, zFar;
extern float zoomFactor;
extern float fovy; // field of view in angles with respect to y axis

extern vec4 eye;
extern mat4 mv;
extern mat4 mp;

// The cube owns its geometry; `shapes` is a flat, non-owning view of it.
extern std::vector<Drawable *> shapes;
extern std::vector<std::unique_ptr<Drawable>> noise;
extern std::vector<std::unique_ptr<LightSource>> lights;
extern std::vector<Material> GlowStep;

// Three faces that can rotate: front, horizontal (top) and side.
extern const char moveTypes[3];

extern std::unique_ptr<RubiksCube> rubiksCube;

extern GLfloat waveTime, waveWidth, waveHeight, wavePeriod, xScaleAmp, yScaleAmp, zScaleAmp;
extern bool grayScale, waterEffect, darkEffect, lsdEffect, harlemShaked;
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

// glowIndex/newGlowIndex take this value to mean "no slice is highlighted".
// The glow animation stops on it, and it is what a completed mouse swipe leaves
// behind.  It is deliberately one past the last real slice.
constexpr int noGlowIndex = 3;

// Easter eggs! ;)
extern const std::array<std::string, 8> easterEggs;

// Recording key presses to match with available easter eggs..
extern std::string easterEgg;

extern bool shuffled;
extern bool start;

extern bool stopAnim;

#endif
