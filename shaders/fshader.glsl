// GLSL ES 1.00 / GLSL 1.10 -- compiles on desktop GL 2.1 and on WebGL 2 alike.

#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

varying vec3 N;
varying vec3 L;
varying vec3 E;

uniform vec4 ambient_product;
uniform vec4 diffuse_product;
uniform vec4 specular_product;

uniform float shininess;

uniform bool grayScale;
uniform float redChrome, blueChrome, greenChrome;
uniform bool darkEffect;

// When set, the scene is being rendered into the off-screen picking buffer and
// we write the fragment's depth instead of its colour.  WebGL cannot read a
// depth attachment back, so the depth is encoded into the RGBA8 colour channels
// and unpacked on the CPU.
uniform bool depthPass;

vec4 packDepth(float depth)
{
	const vec4 bitShift = vec4(1.0, 255.0, 65025.0, 16581375.0);
	const vec4 bitMask = vec4(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 0.0);
	vec4 encoded = fract(depth * bitShift);
	encoded -= encoded.yzww * bitMask;
	return encoded;
}

void main()
{
	if (depthPass)
	{
		gl_FragColor = packDepth(gl_FragCoord.z);
		return;
	}

	// we need to normalize again
	// (as N is the interpolated value of 3 normals)
	vec3 NN = normalize(N);
	vec3 EE = normalize(E);
	vec3 LL = normalize(L);

	vec4 ambient = ambient_product;

	// diffuse calculation
	float Kd = max(dot(LL, NN), 0.0);
	vec4 diffuse = Kd * diffuse_product;

	// specular calculation
	vec3 H = normalize(LL + EE);
	float Ks = pow(max(dot(NN, H), 0.0), shininess);
	vec4 specular = Ks * specular_product;

	vec4 color = vec4((ambient + diffuse + specular).xyz, ambient.a);

	if (grayScale)
	{
		float grayValue = color.r * 0.5 + color.g * 0.3 + color.b * 0.2;
		color = vec4(redChrome * grayValue, greenChrome * grayValue, blueChrome * grayValue, 1.0);
	}
	if (darkEffect)
	{
		color /= 2.0;
	}

	gl_FragColor = color;
}
