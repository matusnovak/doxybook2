#pragma once

namespace Engine {
    namespace Graphics {
        /*!
         * @ingroup Graphics
         */
        class Handle {
          public:
            Handle() = default;
            virtual ~Handle() = default;

            int getHandle() const {
                return handle;
            }

          protected:
            // OpenGL handle
            int handle;
        };

        /*!
         * @brief Some namespace inline function
         * @ingroup Graphics
         */
        inline int getHandle(Handle& handle) {
            return handle.getHandle();
        }
    } // namespace Graphics
} // namespace Engine