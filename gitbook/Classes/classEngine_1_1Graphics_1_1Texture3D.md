---
title: Engine::Graphics::Texture3D
summary: Some deprecated class. 

---

# Engine::Graphics::Texture3D

**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Graphical related classes](/Modules/group__Graphics.md)**

Some deprecated class.  [More...](#detailed-description)
`#include <Texture3D.hpp>`


## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Texture3D](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-texture3d)**(int width, int height, int depth, uint8_t * data, [Texture::Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type) type =Texture::Type::RGBA8) |
| virtual | **[~Texture3D](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-~texture3d)**() |
| virtual int | **[getWidth](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getwidth)**() const override<br>Returns the width of the texture in pixels.  |
| virtual int | **[getHeight](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getheight)**() const override<br>Returns the height of the texture in pixels.  |
| virtual int | **[getDepth](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getdepth)**() const override<br>Returns the depth of the texture in pixels.  |

## Detailed Description

```cpp
class Engine::Graphics::Texture3D;
```

Some deprecated class. 

**Deprecated**: 

Don't use this texture type 
## Public Functions Documentation

### function Texture3D

```cpp
Texture3D(
    int width,
    int height,
    int depth,
    uint8_t * data,
    Texture::Type type =Texture::Type::RGBA8
)
```


### function ~Texture3D

```cpp
virtual ~Texture3D()
```


### function getWidth

```cpp
virtual int getWidth() const override
```

Returns the width of the texture in pixels. 

**Return**: Width in pixels 

**Reimplements**: [Engine::Graphics::Texture::getWidth](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getwidth)


### function getHeight

```cpp
virtual int getHeight() const override
```

Returns the height of the texture in pixels. 

**Return**: Height in pixels 

**Reimplements**: [Engine::Graphics::Texture::getHeight](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getheight)


### function getDepth

```cpp
virtual int getDepth() const override
```

Returns the depth of the texture in pixels. 

**Return**: Depth in pixels 

**Note**: If this texture is 2D the depth is always 1 pixels 

**Reimplements**: [Engine::Graphics::Texture::getDepth](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getdepth)


-------------------------------

Updated on 12 January 2021 at 23:37:34 UTC