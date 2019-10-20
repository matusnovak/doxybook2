#pragma once

#include "Texture.hpp"

namespace Engine {
	namespace Graphics {
		/*!
		 * @ingroup Graphics
		 */
		class Texture2D: public Texture {
			Texture2D(int width, int height, uint8_t* data, Texture::Type type);
			virtual ~Texture2D();

			int getWidth() const override;
			int getHeight() const override;
			int getDepth() const override;

		};
	}
}