---
title: Engine::Graphics::Texture
summary: This is a base texture class that serves as a base class for other texture types. 

---

# Engine::Graphics::Texture

**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Graphical related classes](/Modules/group__Graphics.md)**

This is a base texture class that serves as a base class for other texture types.  [More...](#detailed-description)
`#include <Texture.hpp>`


## Public Types

|                | Name           |
| -------------- | -------------- |
| enum| **[Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type)** { UNKNOWN = 0, RGBA_8, RGB_8, RGBA_16, RGB_16, RGBA_32, RGB_32} |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Texture](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-texture)**([Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type) type) |
| virtual | **[~Texture](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-~texture)**() =default |
| virtual int | **[getWidth](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getwidth)**() const =0<br>Returns the width of the texture in pixels.  |
| virtual int | **[getHeight](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getheight)**() const =0<br>Returns the height of the texture in pixels.  |
| virtual int | **[getDepth](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getdepth)**() const =0<br>Returns the depth of the texture in pixels.  |
| bool | **[isLoaded](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-isloaded)**() const |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[loaded](/Classes/classEngine_1_1Graphics_1_1Texture.md#variable-loaded)**  |
| [Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type) | **[type](/Classes/classEngine_1_1Graphics_1_1Texture.md#variable-type)**  |

## Detailed Description

```cpp
class Engine::Graphics::Texture;
```

This is a base texture class that serves as a base class for other texture types. 

**Bug**: This has some **weird** bug too! 



```cpp
#include <iostream>
#include <memory>
#include "Engine.hpp"

// A dummy example 
int main() {
    // Create pixels buffer
    const auto pixels = std::make_unique<uint8_t[]>(new uint8_t[1024*1024*3]);
    fillData(*pixels, "path/to/texture.png");

    // Create a texture out of the pixels
    Engine::Graphics::Texture2D texture(1024, 1024, *data);

    // Done
    return 0;
}
```

_Filename: example-1.cpp_

## Public Types Documentation

### enum Type

| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| UNKNOWN | 0|   |
| RGBA_8 | |   |
| RGB_8 | |   |
| RGBA_16 | |   |
| RGB_16 | |   |
| RGBA_32 | |   |
| RGB_32 | |   |




## Public Functions Documentation

### function Texture

```cpp
explicit Texture(
    Type type
)
```


### function ~Texture

```cpp
virtual ~Texture() =default
```


### function getWidth

```cpp
virtual int getWidth() const =0
```

Returns the width of the texture in pixels. 

**Return**: Width in pixels 

**Reimplemented by**: [Engine::Graphics::Framebuffer::getWidth](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getwidth), [Engine::Graphics::Texture3D::getWidth](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getwidth), [Engine::Graphics::Texture2D::getWidth](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getwidth)


### function getHeight

```cpp
virtual int getHeight() const =0
```

Returns the height of the texture in pixels. 

**Return**: Height in pixels 

**Reimplemented by**: [Engine::Graphics::Framebuffer::getHeight](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getheight), [Engine::Graphics::Texture3D::getHeight](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getheight), [Engine::Graphics::Texture2D::getHeight](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getheight)


### function getDepth

```cpp
virtual int getDepth() const =0
```

Returns the depth of the texture in pixels. 

**Return**: Depth in pixels 

**Note**: If this texture is 2D the depth is always 1 pixels 

**Reimplemented by**: [Engine::Graphics::Framebuffer::getDepth](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getdepth), [Engine::Graphics::Texture3D::getDepth](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getdepth), [Engine::Graphics::Texture2D::getDepth](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getdepth)


### function isLoaded

```cpp
inline bool isLoaded() const
```


Does some more things


## Protected Attributes Documentation

### variable loaded

```cpp
bool loaded {false};
```


### variable type

```cpp
Type type;
```


-------------------------------

Updated on 12 January 2021 at 23:37:34 UTC