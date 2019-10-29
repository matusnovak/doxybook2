#pragma once
#include <queue>
#include <string>

namespace Engine {
	namespace Assets {
		/*!
		 * @ingroup Assets
		 */
		class Asset {
		public:
			/*!
			 * @ingroup Assets
			 */
			union Event {
				struct Loaded {
					bool success;
					int usage;
				} loaded;
				struct Unloaded {
					bool success;
				} unloaded;
				struct Waiting {
					/*!
					 * @brief The pointer to the other asset this asset is waiting for
					 * @note May be null
					 */
					Asset* other;
				} waiting;
			};

			explicit Asset(const std::string& name);
			virtual ~Asset() = 0;

			/*!
			 * @brief Call this method to load the asset
			 */
			virtual void load();
			/*!
			 * @brief Call this method to unload the asset
			 */
			virtual void unload();
			/*!
			 * @brief Returns the current status of the asset 
			 * @details Use this to determine the state, for example
			 * if the assets is being loaded or unloaded.
			 */
			const Event& getEvent() const {
				return event;
			}
		private:
			Event event;
		};
	}
}
