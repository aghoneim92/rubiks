// GLSL ES 1.00 / GLSL 1.10 -- compiles on desktop GL 2.1 and on WebGL 2 alike.

attribute vec4 vPosition;
attribute vec3 vNormal;

// view matrices
uniform mat4 model_view;
uniform mat4 projection;

// per-object transform
uniform vec3 scale;
uniform vec3 translation;
uniform vec3 rotation;
uniform vec3 rotationAfter;

// shading properties
uniform vec4 light_position;
uniform vec4 eye_position;

// water effect
uniform float waveTime;
uniform float waveWidth;
uniform float waveHeight;
uniform bool waterEffect;

// screen-space overlay (the TV noise specks)
uniform bool overlay;
uniform float windowWidth;
uniform float windowHeight;

// send the normals to each vertex so the fragment shader gets the interpolated
// value of the normals; same for the L and E vectors
varying vec3 N;
varying vec3 L;
varying vec3 E;

mat4 rotationMatrix(vec3 angles)
{
	mat4 mx = mat4(1.0, 0.0,             0.0,             0.0,
	               0.0, cos(angles.x),   sin(angles.x),   0.0,
	               0.0, -sin(angles.x),  cos(angles.x),   0.0,
	               0.0, 0.0,             0.0,             1.0);

	mat4 my = mat4(cos(angles.y),  0.0, -sin(angles.y), 0.0,
	               0.0,            1.0, 0.0,            0.0,
	               sin(angles.y),  0.0, cos(angles.y),  0.0,
	               0.0,            0.0, 0.0,            1.0);

	mat4 mz = mat4(cos(angles.z),  sin(angles.z), 0.0, 0.0,
	               -sin(angles.z), cos(angles.z), 0.0, 0.0,
	               0.0,            0.0,           1.0, 0.0,
	               0.0,            0.0,           0.0, 1.0);

	return mx * my * mz;
}

void main()
{
	mat4 ms = mat4(scale.x, 0.0,     0.0,     0.0,
	               0.0,     scale.y, 0.0,     0.0,
	               0.0,     0.0,     scale.z, 0.0,
	               0.0,     0.0,     0.0,     1.0);

	vec4 transform1 = rotationMatrix(rotation) * ms * vPosition + vec4(translation, 0.0);
	vec4 transform2 = rotationMatrix(rotationAfter) * transform1;

	if (waterEffect)
		transform2.z += sin(waveWidth * transform2.x + waveTime) *
		                cos(waveWidth * transform2.y + waveTime) * waveHeight;

	if (!overlay)
		gl_Position = projection * model_view * transform2;
	else
		gl_Position = ms * vec4(2.0 * (vPosition.x + translation.x) / windowWidth,
		                        2.0 * (vPosition.y + translation.y) / windowHeight,
		                        vPosition.z + translation.z,
		                        1.0);

	N = normalize(vNormal.xyz);
	L = normalize(light_position.xyz - vPosition.xyz);
	E = normalize(eye_position - vPosition).xyz;
}
