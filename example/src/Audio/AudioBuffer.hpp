#pragma once
#include "../Utils/ArrayView.hpp"

namespace Engine {
	namespace Audio {
		class AudioManager;
		class AudioBuffer;

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
				UNKNOWN = 0,
				INT8 = 1 << 1,
				INT16 = 1 << 2,
				INT24 = 1 << 3,
				INT32 = 1 << 4,
				FLOAT32 = 1 << 5
			};
			typedef Utils::ArrayView<uint8_t> AudioData;
			/*!
		     * @brief Use this to populate the buffer
 		     */
			struct TypedAudioData {
				AudioData buffer;
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

			friend class AudioManager;
			friend void Audio::doSomething(AudioBuffer& buffer);
		protected:
			float* getData();
			bool playing{false};
		};
	}
}
