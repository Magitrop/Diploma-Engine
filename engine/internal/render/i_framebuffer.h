#pragma once

#include <array>

#include <engine/core/math/vector4.h>

namespace engine
{
	// Represents an info of a pixel with no specific format.
	struct PixelInfo
	{
		std::array<std::uint8_t, 4> color;
		float depth;

		operator Vector4() { return { color[0], color[1], color[2], color[3] }; }
		// Returns whether two pixels have the same color.
		static bool compareColor(const PixelInfo& a, const PixelInfo& b)
		{
			return 
				a.color[0] == b.color[0] &&
				a.color[1] == b.color[1] &&
				a.color[2] == b.color[2] &&
				a.color[3] == b.color[3];
		}
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