/*!
 * @defgroup Engine An example game engine
 * @brief This is a brief description to the engine group
 * @details Some detailed description here
 */

#include "Assets/Assets.hpp"
#include "Audio/Audio.hpp"
#include "Graphics/Graphics.hpp"
#include "Utils/Utils.hpp"
#include "Exception.hpp"

/*!
 * @brief Some global extern function
 */
extern std::string someGlobalFunc();

/*!
 * @brief This namespace contains all of the necessary engine components
 * @see Engine::Assets
 * @see Engine::Audio
 * @see Engine::Graphics
 * @see Engine::Utils
 * @ingroup Engine
 */
namespace Engine {
	extern std::string getVersion();
}
