#include <globalvariables.h>

//<--Shader variable locations

//Viewing
GLuint model_view_loc = 0;
GLuint projection_loc = 0;

//Color
GLuint light_position_loc = 0;
GLuint eye_position_loc = 0;
GLuint ambient_product_loc = 0;
GLuint diffuse_product_loc = 0;
GLuint specular_product_loc = 0;
GLuint shininess_loc = 0;

//Transform
GLuint scale_loc = 0;
GLuint rotation_loc = 0;
GLuint rotation_after_loc = 0;
GLuint translation_loc = 0;

//SFX
GLuint wavetime_loc = 0;
GLuint wavewidth_loc = 0;
GLuint waveheight_loc = 0;
GLuint grayscale_loc;
GLuint water_loc = 0;
GLuint dark_loc = 0;
GLuint overlay_loc = 0;
GLuint windowwidth_loc = 0;
GLuint windowheight_loc = 0;
GLuint redchrome_loc;
GLuint greenchrome_loc;
GLuint bluechrome_loc;
GLuint left_loc;
//Shader variable locations-->

//<--Viewing variables
const bool fullScreen = true;
//Default non-fullscreen window width and height
GLfloat WindowWidth = 600;
GLfloat WindowHeight = 600;

float theta = 0;
float phi = 0;
float radius = 300;
vec4 eye;
vec4 at(0.0, 0.0, 0.0, 1.0);
vec4 up(0.0, 1.0, 0.0, 0.0);

mat4 mv;
mat4 mp;

ViewingMode viewMode;

// viewing volume
GLfloat left = -400, right = 400;
GLfloat bottom = -400, top = 400;
GLfloat zNear = 1, zFar = 600;
float zoomFactor = 1.0;
float fovy = 45;
//-->Viewing variables

std::vector<Drawable *> shapes;
std::vector<LightSource *> lights;
std::vector<Drawable *> noise;
std::vector<Material> GlowStep;

//Three faces that can rotate: front, horizontal(top) and side.
char moveTypes[] = {'f', 'h', 's'};

//Status of SFX
GLfloat waveTime = 0, waveWidth = 2, waveHeight = 20, wavePeriod = 50, xScaleAmp = 0, yScaleAmp = 0, zScaleAmp = 0;
const GLfloat sinArg = 0;
bool grayScale = 0, waterEffect = 0, darkEffect = 0, noisy = 0, lsdEffect = 0, harlemShaked = 0;
GLfloat redEffect = 1, greenEffect = 1, blueEffect = 1;

//El bob el kebeer
RubiksCube *rubiksCube;

//<--Animations and game logic

//Initial direction of floating
char dir = 'l';
//If a face is moving, this is its move type, either side, top/horizontal or face
char moveType;
//The index of the face that is moving: 0,1 or 2
int faceIndex;

//The nth shuffle operation.
int shuffleIndex = 0;
//+ve or -ve rotation of a face
short rotDir = 1;
//rotation done -> enable input
bool rotDone = true;
//Currently glowing face should change
bool faceChanged = false;
//Glowing or dimming in current glow cycle
short direction = 1;
//Face to start glowing next (after key press)
char newGlowFace;
//Face index to start glowing next
int newGlowIndex;
//Current glowing face index
int glowIndex = 0;
//Current glowing face direction index (char will be fetched from moveTypes[])
char glowFace = 0;
bool shuffled = false;
//Should start game
bool start = false;
//-->Animations and game logic

//Easter eggs! ;)
const int nEasterEggs = 8;
const std::string easterEggs[] = {
		"bw",
		"imthirsty",
		"itsdarkinhere",
		"lsd",
		"ifeelred",
		"ifeelgreen",
		"ifeelblue",
		"harlemshake"};

//Recording key presses to match with available easter eggs..
std::string easterEgg;

volatile bool stopAnim = false;
