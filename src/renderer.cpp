#include "renderer.h"

#include "globalvariables.h"

namespace
{

void renderDrawable(Drawable &shape)
{
	const LightSource &light = *lights[0];

	glUniform4fv(light_position_loc, 1, light.position);
	glUniform4fv(eye_position_loc, 1, eye);
	glUniform4fv(ambient_product_loc, 1, light.ambient * shape.material.ambientK);
	glUniform4fv(diffuse_product_loc, 1, light.diffuse * shape.material.diffuseK);
	glUniform4fv(specular_product_loc, 1, light.specular * shape.material.specularK);
	glUniform1f(shininess_loc, shape.material.shininess);
	glUniform3fv(scale_loc, 1, shape.scale);
	glUniform3fv(translation_loc, 1, shape.translation);
	glUniform3fv(rotation_loc, 1, shape.rotation);
	glUniform3fv(rotation_after_loc, 1, shape.rotationAfter);
	glUniform1i(overlay_loc, shape.overlay);

	shape.render();
}

} // namespace

void renderer::renderScene()
{
	for (Drawable *shape : shapes)
		renderDrawable(*shape);

	for (const std::unique_ptr<Drawable> &speck : noise)
	{
		// The TV-noise specks live at zero scale unless the "bw" easter egg is
		// on, and a zero-scale fan rasterises nothing.  Skipping them there saves
		// 300 draw calls a frame without changing a pixel.
		if (speck->scale.x == 0.0f && speck->scale.y == 0.0f && speck->scale.z == 0.0f)
			continue;
		renderDrawable(*speck);
	}
}

void renderer::drawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mv = LookAt(eye, at, up);
	glUniformMatrix4fv(model_view_loc, 1, GL_TRUE, mv);
	renderScene();
}
