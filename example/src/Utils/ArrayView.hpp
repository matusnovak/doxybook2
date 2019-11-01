#pragma once

namespace Engine {
	namespace Utils {
		/*!
		 * @brief Filesystem path utility functions
		 * @ingroup Utils
		 * @tparam T The type of the data, for example uint8_t
 		 */
		template<typename T = uint8_t>
		class ArrayView {
		public:
			ArrayView(const T* src, size_t len);
			virtual ~ArrayView() = default;

			virtual const T* data() const;
			virtual size_t size() const;
		};
	}
}