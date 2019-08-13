// #version 150
attribute vec4 vPosition;
attribute vec3 vNormal;

uniform mat4 model_view;
uniform mat4 projection;

uniform vec3 scale;
uniform vec3 translation;
uniform vec3 rotation;
uniform vec3 rotationAfter;
//view matrices

//shading properties
uniform vec4 light_position;
uniform vec4 eye_position;

uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;

uniform float shininess;

uniform bool waterEffect;

uniform bool overlay;

uniform float windowWidth;
uniform float windowHeight;
uniform int left;
// send the normals to each vertex so the fragment shader get the interpolated value of the normals
varying vec3 N;
//same for K and E vectors
varying vec3 L;
varying vec3 E;

void main()
{
	mat4 ms = mat4(scale.x, 		0.0,	0.0,	0.0,
				   0.0,		scale.y,	0.0,	0.0,
				   0.0,		0.0,	scale.z,	0.0,
				   0.0,		0.0,	0.0,	1.0);
				   
	mat4 mt = mat4(1,				0,				0,				0,
				   0,				1,				0,				0,
				   0,				0,				1,				0,
				   translation.x,   translation.y,  translation.z,	1);
	
	mat4 mx = mat4(1,	0,					0,					0,
				   0,   cos(rotation.x),    sin(rotation.x),	0,
				   0,	-sin(rotation.x),   cos(rotation.x),	0,
				   0,	0,					0,					1);
				   
	mat4 my = mat4(cos(rotation.y),0,-sin(rotation.y),0,
				   0,1,0,0,
				   sin(rotation.y),0,cos(rotation.y),0,
				   0,0,0,1);
				   
	mat4 mz = mat4(cos(rotation.z), sin(rotation.z), 0,0,	
				  -sin(rotation.z), cos(rotation.z), 0,0,
				  0,0,1,0,
				  0,0,0,1);
    //vertex position after projection
	vec4 transform1 = mx*my*mz*ms*vPosition+vec4(translation,0);
	mx = mat4(1,	0,					0,					0,
				   0,   cos(rotationAfter.x),    sin(rotationAfter.x),	0,
				   0,	-sin(rotationAfter.x),   cos(rotationAfter.x),	0,
				   0,	0,					0,					1);
				   
	my = mat4(cos(rotationAfter.y),0,-sin(rotationAfter.y),0,
				   0,1,0,0,
				   sin(rotationAfter.y),0,cos(rotationAfter.y),0,
				   0,0,0,1);
				   
	mz = mat4(cos(rotationAfter.z), sin(rotationAfter.z), 0,0,	
				  -sin(rotationAfter.z), cos(rotationAfter.z), 0,0,
				  0,0,1,0,
				  0,0,0,1);
	vec4 transform2 = (mx*my*mz*transform1);
	
	if(waterEffect)
		transform2.z+= sin(waveWidth*transform2.x + waveTime) * cos(waveWidth*transform2.y + waveTime) *waveHeight;
    if(!overlay){
	    vec3 beforeSphere = transform2.xyz;
		//transform2.z = beforeSphere.z*cos(beforeSphere.x*3.142/600.0);
		//transform2.x = beforeSphere.x*cos(beforeSphere.z*3.142/600.0);
		//transform2.y = beforeSphere.y*cos(beforeSphere.x*3.142/600.0);
		gl_Position = projection*model_view*transform2;/*vec4(before3D.x/2+left*0.5, before3D.y, before3D.z, before3D.w);*/
	}
    else
		gl_Position = ms*vec4(2.0*(vPosition.x+translation.x)/windowWidth, 2.0*(vPosition.y+translation.y)/windowHeight, vPosition.z+translation.z,1.0);
    N = normalize(vNormal.xyz);
    L = normalize(light_position.xyz - vPosition.xyz);
    E = normalize(eye_position - vPosition).xyz;

}