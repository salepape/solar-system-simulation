#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <deque>
#include <functional>
#include <utility>



// @todo - Implement packed ID as a concatenation of bit info translating which IRenderables to batch, and in which order
// Group of Scene Entities that can be rendered to screen in a batch
enum class RenderableType
{
	ALL = 0,
	OPAQUE_ENTITY,
	TRANSPARENT_ENTITY,
	BACKGROUND,
};

// Container of a chain of OpenGL functions to submit draw settings/functions to GPU for a group of IRenderables
struct RenderCommand
{
	RenderableType renderType;

	std::function<void()> Queue;
	std::function<void()> Unqueue = []() {};
};

// Data structure to push/pop render commands for OpenGL draw settings/functions
struct RenderQueue
{
	void Push(RenderCommand&& renderCommand)
	{
		queue.emplace_back(std::move(renderCommand));
	}

	void Pop()
	{
		if (queue.empty())
		{
			return;
		}

		queue.pop_front();
	}

	void PopAll()
	{
		if (queue.empty())
		{
			return;
		}

		queue.clear();
	}

	std::deque<RenderCommand> queue;
};



#endif // RENDER_QUEUE_H
