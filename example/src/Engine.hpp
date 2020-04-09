/*!
 * @defgroup Engine An example game engine
 * @brief This is a brief description to the engine group
 * @details Some detailed description here
 */

#include "Assets/Assets.hpp"
#include "Audio/Audio.hpp"
#include "Exception.hpp"
#include "Graphics/Graphics.hpp"
#include "Utils/Utils.hpp"

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

/*! \mainpage Doxybook2 Example

This is a mainpage of Doxybook2 example output. This file is generated from
`\mainpage` command within Engine.hpp file.

## Images:

\image html doxygen.png

## Refs:

- \ref Engine::Graphics::Texture "Some texture class ref"
- \ref Engine::Audio::AudioBuffer "Some audio buffer class ref"

## Simple list:

- list item 1
    - sub item 1
        - sub sub item 1 with __italic__
        - sub sub item 2 with **bold**
    - sub item 2
- list item 2
    - sub item 3
        - sub sub item 3 with ~~strike through~~
        - sub sub item 4 with `monospaced`

## Enumerated list:

-# list item 1
    -# sub item 1
        -# sub sub item 1 with __italic__
        -# sub sub item 2 with **bold**
    -# sub item 2
-# list item 2
    -# sub item 3
        -# sub sub item 3 with ~~strike through~~

## Hyper links:

- [GitHub repository](https://github.com/matusnovak/doxybook2)

## Code blocks:

\code
#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Hello World" << std::endl;
    return 0;
}
\endcode

## Subpages:

This manual is divided in the following sections:
- \subpage intro
- \subpage advanced "Advanced usage"

## Snippet:

\snippet example-1.cpp TestTest

## Table:

First Header  | Second Header | Third Header
------------- | ------------- | -------------
Content Cell  | Content Cell  | Content Cell
Content Cell  | Content Cell  | Content Cell

*/

//-----------------------------------------------------------

/*! \page intro Introduction
This page introduces the user to the topic.
Now you can proceed to the \ref advanced "advanced section".
*/

//-----------------------------------------------------------

/*! \page advanced Advanced Usage
This page is for advanced users.
Make sure you have first read \ref intro "the introduction".
*/

/*!
 * @example example-1.cpp
 * @brief This is an example
 * @details This is detailed docummentation
 */