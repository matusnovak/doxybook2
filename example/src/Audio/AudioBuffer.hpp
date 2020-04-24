#pragma once
#include "../Utils/ArrayView.hpp"

namespace Engine {
    namespace Audio {
        class AudioManager;
        class AudioBuffer;

        /*!
         * @brief Some callback function pointer type definition
         * @param buffer The buffer this callback is called from
         * @param offset The offset of the buffer currently playing
         * @deprecated Use the Audio::Callback2 instead
         */
        typedef int (*Callback)(AudioBuffer& buffer, size_t offset);

        /*!
         * @brief Some callback function pointer type definition
         * @param buffer The buffer this callback is called from
         * @param offset The offset of the buffer currently playing
         * @param user User specific data
         */
        using Callback2 = int (*)(AudioBuffer& buffer, size_t offset, void* user);

        /*!
         * @brief Do something with the buffer
         */
        extern void doSomething(AudioBuffer& buffer);

        /*!
         * @brief _A simple audio buffer to hold PCM samples_
         * @ingroup Audio
         * @author Matus Novak
         * @date 2017-2019
         * @pre First initialize the system.
         * @test Some random test description
         * @warning Some random warning
         * @note Some random note
         * @note Some second random note
         * @bug Some random bug
         * @bug Some other random bug
         * @details Lorem ipsum donor
         * @code
         * // Some random code
         * using namespace Engine;
         *
         * Audio::AudioBuffer buffer("path/to/file.wav");
         * buffer.play(audioManager);
         * @endcode
         * More detailed description!
         * @code
         * // Another code sample
         * #include <iostream>
         * using namespace Engine;
         * Audio::AudioBuffer buffer("path/to/file.wav");
         * std::cout << buffer.play(...) << std::endl;
         * @endcode
         * @todo Some random todo
         */
        class AudioBuffer {
          public:
            /*!
             * @brief Different type of audio formats
             */
            enum class Type {
                /*!
                 * Dont use this
                 */
                UNKNOWN = 0,
                /*!
                 * 8-bit signed integer
                 */
                INT_8 = 1 << 1,
                /*!
                 * 16-bit signed integer
                 */
                INT_16 = 1 << 2,
                /*!
                 * 24-bit signed integer
                 */
                INT_24 = 1 << 3,
                /*!
                 * 32-bit signed integer
                 */
                INT_32 = 1 << 4,
                /*!
                 * 32-bit float
                 */
                FLOAT_32 = 1 << 5
            };
            template <typename T> using AudioData = Utils::ArrayView<T>;
            typedef AudioData<uint8_t> AudioData8U;
            /*!
             * @brief Use this to populate the buffer
             */
            struct TypedAudioData {
                AudioData8U buffer;
                Type type;
            };
            /*!
             * @brief Constructor for Audio::AudioBuffer
             */
            explicit AudioBuffer(const std::string& filename);
            /*!
             * @brief Audio::AudioBuffer destructor
             */
            virtual ~AudioBuffer() = default;
            /*!
             * @brief Play this buffer
             * @param manager Which manager to play the sound with
             * @throw Exception If this buffer is already playing
             */
            void play(AudioManager& manager) const;
            /*!
             * @brief Stop this buffer playing
             * @param manager Which manager to stop the sound with
             * @throw Exception If this buffer is already stopped
             */
            void stop(AudioManager& manager) const;
            /*!
             * @brief Loop this buffer forever
             * @param manager Which manager to loop the sound with
             * @note This will loop forever until you call stop
             * @throw Exception If this buffer is already looping
             */
            void loop(AudioManager& manager) const;
            void setData(const TypedAudioData& data);

            template <size_t Size> void setDataMultiple(const TypedAudioData[Size] data) {
            }

            /*!
             * @brief Some deprecated function
             * @deprecated Use the AudioBuffer::setDataMultiple instead
             */
            void setData(const TypedAudioData[] data, size_t size);

            friend class AudioManager;
            friend void Audio::doSomething(AudioBuffer& buffer);

            /*!
             * @brief Set the callback function
             * @see Audio::Callback
             * @param callback The callback function pointer
             * @deprecated Use the AudioBuffer::setCallback2 instead
             */
            void setCallback(Callback callback);

            /*!
             * @brief Set the callback function
             * @see Audio::Callback
             * @param callback The callback function pointer
             */
            void setCallback2(Callback2 callback, void* user);

          protected:
            float* getData();
            bool playing{false};
        };
    } // namespace Audio
} // namespace Engine
