#pragma once

#include <memory>

#include <engine/dependencies/glad/include/glad/glad.h>
#include <engine/internal/render/i_graphic_api.h>

namespace engine
{
	class GladGraphicAPI final : public IGraphicAPI
	{
	public:
		explicit GladGraphicAPI();
		~GladGraphicAPI();

	private:
		virtual bool initialize() override;
		virtual void finalize() override;

		class Internal;
		std::unique_ptr<Internal> m_internal;
	};
} // namespace engine