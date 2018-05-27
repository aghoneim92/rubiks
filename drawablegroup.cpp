#include "drawablegroup.h"

DrawableGroup::DrawableGroup(){
	children = NULL;
	_size=0;
}

DrawableGroup::DrawableGroup(Drawable** children, int size){
	this->children = new Drawable*[size];
	for(int i=0; i<size; i++)
		this->children[i] = children[i];
	_size = size;
}

DrawableGroup::DrawableGroup(DrawableGroup* childGroups, int n){
	int i;
	_size = 0;
	for(i=0; i<n; i++)
		_size+=childGroups[i].size();
	children = new Drawable*[_size];
	int offset=0;
	for(i=0; i<n; i++){
		memcpy(children+offset,childGroups[i].getChildren(), sizeof(Drawable*)*childGroups[i].size());
		offset+=childGroups[i].size();
	}
}

Drawable** DrawableGroup::getChildren(){
	return children;
}

Drawable** DrawableGroup::start(){
	return children+0;
}

Drawable** DrawableGroup::end(){
	return children+_size;
}

int DrawableGroup::size(){
	return _size;
}

void DrawableGroup::translate(vec3 translation){
	int i;
	for(i=0; i<_size; i++)
		children[i]->translation+=translation;
}

void DrawableGroup::rotate(vec3 rotation){
	int i;
	for(i=0; i<_size; i++)
		children[i]->rotation+=rotation;
}

void DrawableGroup::rotateAfter(vec3 rotation){
	int i;
	for(i=0; i<_size; i++)
		children[i]->rotationAfter+=rotation;
}

void DrawableGroup::scale(vec3 scale){
	int i;
	for(i=0; i<_size; i++)
		children[i]->scale=scale;
}

void DrawableGroup::rotatePermanentX(float angle){
	int i,j;
	mat4 rx = RotateX(angle*180/M_PI);
	for(i=0; i<_size; i++){
		for(j=0; j<children[i]->pointsNum; j++){
			children[i]->points[j] = rx*children[i]->points[j];
		}
	}
}

void DrawableGroup::rotatePermanentY(float angle){
	int i,j;
	mat4 ry = RotateY(angle*180/M_PI);
	for(i=0; i<_size; i++){
		for(j=0; j<children[i]->pointsNum; j++){
			children[i]->points[j] = ry*children[i]->points[j];
		}
	}
}

void DrawableGroup::rotatePermanentZ(float angle){
	int i,j;
	mat4 rz = RotateZ(angle*180/M_PI);
	for(i=0; i<_size; i++){
		for(j=0; j<children[i]->pointsNum; j++){
			children[i]->points[j] = rz*children[i]->points[j];
		}
	}
}