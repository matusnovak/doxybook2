#pragma once

#include "Handle.hpp"

namespace Engine {
	namespace Graphics {
		/*!
		 * @brief This is a base texture class that serves as a base class for
		 * other texture types.
		 * @interface Texture
		 * @ingroup Graphics
		 * @bug This has some **weird** bug too!
		 */
		class Texture: protected Handle {
		public:
			enum class Type {
				/*!
				 * @note This should probably never occur
				 */
				UNKNOWN = 0,
				/*!
				 * @note Use primarly this for the best performance!
				 */
				RGBA8,
				RGB8,
				RGBA16,
				RGB16,
				RGBA32,
				RGB32
			};

			explicit Texture(Type type);
			virtual ~Texture() = default;
			/*!
			 * @brief Returns the width of the texture in pixels
			 * @return Width in pixels
			 */
			virtual int getWidth() const = 0;
			/*!
			 * @brief Returns the height of the texture in pixels
			 * @return Height in pixels
			 */
			virtual int getHeight() const = 0;
			/*!
			 * @brief Returns the depth of the texture in pixels
			 * @note If this texture is 2D the depth is always 1 pixels
			 * @return Depth in pixels
			 */
			virtual int getDepth() const = 0;
			inline bool isLoaded() const {
				/*!
				 * Does some more things
				 */
				return loaded;
			}
		protected:
			bool loaded{false};
			Type type;
		};
	}
}