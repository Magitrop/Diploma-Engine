#pragma once

#include <array>

namespace engine
{
	// Represents an info of a pixel with no specific format.
	struct PixelInfo
	{
		std::array<std::uint16_t, 4> color;
		std::uint16_t depth;
	};

	class IFramebuffer
	{
	public:
		virtual ~IFramebuffer() = 0;

		virtual std::uint32_t width() const = 0;
		virtual std::uint32_t height() const = 0;
		virtual float aspectRatio() const = 0;

		virtual PixelInfo readPixel(std::uint32_t x, std::uint32_t y) = 0;
	};
} // namespace engine