#pragma once

#include "Texture.hpp"

namespace Engine {
    namespace Graphics {
        /*!
         * @ingroup Graphics
         */
        class Texture2D : public Texture {
          public:
            /*!
             * @param data The pointer to the raw pixels data
             * @param width The width of the image held by the data pointer
             * @param height The height of the image held by the data pointer
             * @param type What is the type of this pixels array?
             */
            Texture2D(int width, int height, uint8_t* data, Texture::Type type = Texture::Type::RGBA8);
            virtual ~Texture2D();

            int getWidth() const override;
            int getHeight() const override;
            int getDepth() const override;
        };
    } // namespace Graphics
} // namespace Engine