#ifndef RENDERER_H
#define RENDERER_H

// Draws the scene.  Kept apart from the main loop so the picking pass can reuse
// exactly the same geometry and transforms when it renders depth off-screen.
namespace renderer
{

// Draws every shape with the currently bound program state.
void renderScene();

// Clears, updates the view matrix and draws one visible frame.
void drawFrame();

} // namespace renderer

#endif
