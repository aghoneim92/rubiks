#version 150

in vec3 N;
in vec3 L;
in vec3 E;

uniform vec4 ambient_product;
uniform vec4 diffuse_product;
uniform vec4 specular_product;

uniform vec4 light_position;
uniform vec4 eye_position;

uniform float shininess;

uniform bool grayScale;
uniform float redChrome, blueChrome, greenChrome;
uniform bool darkEffect;

float rand(vec2 n)
{
  return fract(sin(dot(n.xy, vec2(12.9898, 78.233)))* 43758.5453);
}

void main()
{
    // we need to normalize again
    // (as N is the interpolated value of 3 normals)
    vec3 NN = normalize(N);
    vec3 EE = normalize(E);
    vec3 LL = normalize(L);

    vec4 ambient, diffuse, specular;
    ambient = ambient_product;

    //TODO: Task 5
    // Calculate the values marked with ???

    //diffuse calculation
    float Kd = max(dot(LL,NN),0.0);
    diffuse = Kd*diffuse_product;

    //specular calulation
    vec3 H   = normalize(LL+EE);
    float Ks = pow(max(dot(NN,H),0.0),shininess);
    specular = Ks*specular_product;
	
	vec4 beforeEffects = vec4((ambient + diffuse + specular).xyz, ambient.a);
	
	if(grayScale){
		float grayValue = beforeEffects.r*0.5 + beforeEffects.g*0.3 + beforeEffects.b*0.2;
		beforeEffects = vec4(redChrome*grayValue,greenChrome*grayValue,blueChrome*grayValue,1.0);
	}
	if(darkEffect){
		beforeEffects/=2;
	}
	//final shading
    gl_FragColor = beforeEffects;
}

