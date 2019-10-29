#pragma once

#include "AudioBuffer.hpp"

namespace Engine {
	namespace Audio {
		/*!
		 * @brief An audio manager that *accepts* multiple **Audio::AudioBuffer** instances
		 * @details Lorem Ipsum Donor. Some [Random link with **bold** and _italics_](http://github.com)
         * And the following is a \c typewritter font. And here is some list items:
		 * * First item
		 * * Second item
		 * * Third item **with bold text**
		 * @ingroup Audio
		 * @see Audio::AudioBuffer
 		 */
		class AudioManager final {
		public:
			AudioManager(int numOfChannels = 128);
			~AudioManager();

			void enque(const AudioBuffer& buffer);
		};
	}
}