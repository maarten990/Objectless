#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include <SDL.h>
#include <unordered_map>
#include <functional>
#include <memory>

#include "Engine/System.h"

class GraphicsSystem : public System
{
	struct Callback
	{
		uint64_t id;
		std::function<void()> function;
	};
public:
	GraphicsSystem(shared_ptr<EntityManager> em);
	~GraphicsSystem();
	SDL_Texture* loadTexture(const std::string& path);
	void step(float delta_time);

	SDL_Renderer* getRenderer() const { return _renderer; }

	/* Register a listener for post-render events.
	 * This is called after normal rendering has finished, but before the results
	 * have been presented.
	 * The return value is the callback's unique ID, which can be used to unregister
	 * the listener later if necessary.
	 */
	uint64_t registerPostRenderCallback(const std::function<void()> callback);

	void unregisterPostRenderCallback(uint64_t callback_id);

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	shared_ptr<EntityManager> _entitymanager;

	uint64_t _num_post_render_callbacks_added = 0;
	std::vector<Callback> _post_render_callbacks;
};

#endif
