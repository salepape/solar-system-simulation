#ifndef RENDER_QUEUE_H
#define RENDER_QUEUE_H

#include <functional>
#include <utility>
#include <deque>



enum class RenderType
{
	ALL = 0,
	OPAQUE_ENTITY,
	TRANSPARENT_ENTITY,
	BACKGROUND,
};

struct RenderCommand
{
	RenderType renderType;

	std::function<void()> Queue;
	std::function<void()> Unqueue = []() {};
};

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