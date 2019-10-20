#pragma once
#pragma once

#include "Texture.hpp"

namespace Engine {
	namespace Graphics {
		/*!
		 * @ingroup Graphics
		 */
		class Texture3D: public Texture {
			Texture3D(int width, int height, int depth, uint8_t* data, Texture::Type type);
			virtual ~Texture3D();

			int getWidth() const override;
			int getHeight() const override;
			int getDepth() const override;

		};
	}
}
