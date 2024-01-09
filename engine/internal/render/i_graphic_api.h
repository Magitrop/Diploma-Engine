#pragma once

namespace engine
{
	class IGraphicAPI
	{
	public:
		virtual bool initialize() = 0;
		virtual void finalize() = 0;
	};
} // namespace engine