#pragma once

#include "Handle.hpp"

namespace Engine {
	namespace Graphics {
		/*!
		 * @brief This is a base texture class that serves as a base class for
		 * other texture types.
		 * @interface Texture
		 * @ingroup Graphics
		 */
		class Texture: protected Handle {
		public:
			enum class Type {
				/*!
				 * @note This should probably never occur
				 */
				UNKNOWN = 0,
				/*!
				 * @note Thus primarly this for the best performance!
				 */
				RGBA8,
				RGB8,
				RGBA16,
				RGB16,
				RGBA32,
				RGB32
			};

			Texture() = default;
			virtual ~Texture() = 0;
			/*!
			 * @brief Returns the width of the texture in pixels
			 */
			virtual int getWidth() const = 0;
			/*!
			 * @brief Returns the height of the texture in pixels
			 */
			virtual int getHeight() const = 0;
			/*!
			 * @brief Returns the depth of the texture in pixels
			 * @note If this texture is 2D the depth is always 1 pixels
			 */
			virtual int getDepth() const = 0;

		protected:
			bool loaded{false};
			Type type;
		};
	}
}