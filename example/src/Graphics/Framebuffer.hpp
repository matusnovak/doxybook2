#pragma once

#include "Texture.hpp"

namespace Engine {
    namespace Graphics {
        /*!
         * @ingroup Graphics
         */
        class Framebuffer : public Texture {
          public:
            Framebuffer(int width, int height);
            virtual ~Framebuffer();

            int getWidth() const override;
            int getHeight() const override;
            int getDepth() const override;
        };
    } // namespace Graphics
} // namespace Engine
