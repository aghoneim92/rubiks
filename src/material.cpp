#include "material.h"


Material::Material() {

	//default is red plastic diffuse materials
	// Ar   Ag     Ab   Dr    Dg    Db     Sr    Sg   Sb    shine
	//0.0 | 0.0 | 0.0 | 0.5 | 0.0 | 0.0 | 0.7 | 0.6 | 0.6 | 32
	ambientK  = vec4(0.2f,0.0f,0.0f,1);
	diffuseK  = vec4(0.5f,0,0,1);
	specularK = vec4(0.7f,0.6f,0.6f,1);
	shininess = 32.0f;
}

Material::Material(color4 ambientK, color4 diffuseK, color4 specularK,
				   float shininess) {
					   this->ambientK = ambientK;
					   this->diffuseK = diffuseK;
					   this->specularK = specularK;
					   this->shininess = shininess;
}


Material Material::blackPlastic = Material(
	color4(0,0,0,1),
	color4(0.01f,0.01f, 0.01f ,1),
	color4(0.50f, 0.50f, 0.50f ,1),
	0.25f);

Material Material::greenPlastic = Material(
	color4(0.0,1,0,1),
	color4(0.1f,0.35f,0.1f,1),
	color4(0.45f, 0.55f, 0.45f ,1),
	0.25f);