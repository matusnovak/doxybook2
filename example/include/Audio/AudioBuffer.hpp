#pragma once
#include "../Utils/ArrayView.hpp"

namespace Engine {
	namespace Audio {
		class AudioManager;

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
		     * @details Use this to populate the buffer
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
 		     */
			void play(AudioManager& manager) const;
			/*!
		     * @brief Stop this buffer playing
			 * @param manager Which manager to stop the sound with
 		     */
			void stop(AudioManager& manager) const;
			/*!
		     * @brief Loop this buffer forever
			 * @param manager Which manager to loop the sound with
			 * @note This will loop forever until you call stop
 		     */
			void loop(AudioManager& manager) const;
			void setData(const TypedAudioData& data);
		protected:
			float* getData();
		};
	}
}