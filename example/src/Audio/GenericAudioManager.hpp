#pragma once

#include <array>
#include <cstdint>

namespace Engine {
    namespace Audio {
        namespace Some{
            namespace More{
                namespace Namespace{
                    template<std::size_t SIZE>
                    class Frobnicator final
                    {
                    };
                }
            }
        }

        /**
         * @brief A generic audio manager.
         * @tparam Foo The type of the audio manager implementation.
         */
        template<typename Foo>
        class GenericAudioManager final
        {
        };

        /**
         * @brief A generic audio manager.
         * @tparam SIZE The size of the internal array.
         */
        template<std::size_t SIZE>
        class GenericAudioManager<Engine::Audio::Some::More::Namespace::Frobnicator<SIZE>> final
        {
            std::array<float, SIZE> _values;
        public:
            /**
             * @brief Constructs this instance.
             */
            explicit GenericAudioManager(float value);
        };
    } // namespace Audio
} // namespace Engine