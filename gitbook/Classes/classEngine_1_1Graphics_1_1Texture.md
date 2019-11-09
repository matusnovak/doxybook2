---
title: Engine::Graphics::Texture
summary: This is a base texture class that serves as a base class for other texture types.  

---

# Engine::Graphics::Texture


**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Graphical related classes](/Modules/group__Graphics.md)**

This is a base texture class that serves as a base class for other texture types.  [More...](#detailed-description)


`#include <Texture.hpp>`


Inherits from [Engine::Graphics::Handle](/Classes/classEngine_1_1Graphics_1_1Handle.md)

Inherited by [Engine::Graphics::Framebuffer](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md), [Engine::Graphics::Texture2D](/Classes/classEngine_1_1Graphics_1_1Texture2D.md), [Engine::Graphics::Texture3D](/Classes/classEngine_1_1Graphics_1_1Texture3D.md)




## Public Types

|                | Name           |
| -------------- | -------------- |
| enum | **[Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type)** { UNKNOWN, RGBA8, RGB8, RGBA16, RGB16, RGBA32, RGB32 } |


## Public Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[isLoaded](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-isloaded)**() const  |
| virtual int | **[getDepth](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getdepth)**() const =0 <br>Returns the depth of the texture in pixels.  |
| virtual int | **[getHeight](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getheight)**() const =0 <br>Returns the height of the texture in pixels.  |
| virtual int | **[getWidth](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-getwidth)**() const =0 <br>Returns the width of the texture in pixels.  |
| virtual  | **[~Texture](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-~texture)**() =default  |
|  | **[Texture](/Classes/classEngine_1_1Graphics_1_1Texture.md#function-texture)**([Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type) type)  |



## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type) | **[type](/Classes/classEngine_1_1Graphics_1_1Texture.md#variable-type)**  |
| bool | **[loaded](/Classes/classEngine_1_1Graphics_1_1Texture.md#variable-loaded)**  |



## Additional inherited members






**Public Functions inherited from [Engine::Graphics::Handle](/Classes/classEngine_1_1Graphics_1_1Handle.md)**

|                | Name           |
| -------------- | -------------- |
| int | **[getHandle](/Classes/classEngine_1_1Graphics_1_1Handle.md#function-gethandle)**() const  |
| virtual  | **[~Handle](/Classes/classEngine_1_1Graphics_1_1Handle.md#function-~handle)**() =default  |
|  | **[Handle](/Classes/classEngine_1_1Graphics_1_1Handle.md#function-handle)**() =default  |



**Protected Attributes inherited from [Engine::Graphics::Handle](/Classes/classEngine_1_1Graphics_1_1Handle.md)**

|                | Name           |
| -------------- | -------------- |
| int | **[handle](/Classes/classEngine_1_1Graphics_1_1Handle.md#variable-handle)**  |




## Detailed Description

```cpp
interface Engine::Graphics::Texture;
```

This is a base texture class that serves as a base class for other texture types. 


**Bug**: This has some **weird** bug too! 




## Public Types Documentation

### enum Type

```cpp
enum Type {
    UNKNOWN,
    RGBA8,
    RGB8,
    RGBA16,
    RGB16,
    RGBA32,
    RGB32
}
```








## Public Functions Documentation

### function isLoaded

```cpp
inline bool isLoaded() const
```





Does some more things

### function getDepth

```cpp
virtual int getDepth() const =0
```

Returns the depth of the texture in pixels. 


**Return**: Depth in pixels 
**Note**: If this texture is 2D the depth is always 1 pixels 
**Reimplemented by**: [Engine::Graphics::Framebuffer::getDepth](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getdepth), [Engine::Graphics::Texture3D::getDepth](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getdepth), [Engine::Graphics::Texture2D::getDepth](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getdepth)




### function getHeight

```cpp
virtual int getHeight() const =0
```

Returns the height of the texture in pixels. 


**Return**: Height in pixels 
**Reimplemented by**: [Engine::Graphics::Framebuffer::getHeight](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getheight), [Engine::Graphics::Texture3D::getHeight](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getheight), [Engine::Graphics::Texture2D::getHeight](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getheight)




### function getWidth

```cpp
virtual int getWidth() const =0
```

Returns the width of the texture in pixels. 


**Return**: Width in pixels 
**Reimplemented by**: [Engine::Graphics::Framebuffer::getWidth](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getwidth), [Engine::Graphics::Texture3D::getWidth](/Classes/classEngine_1_1Graphics_1_1Texture3D.md#function-getwidth), [Engine::Graphics::Texture2D::getWidth](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getwidth)




### function ~Texture

```cpp
virtual ~Texture() =default
```






### function Texture

```cpp
explicit Texture(
    Type type
)
```









## Protected Attributes Documentation

### variable type

```cpp
Type type;
```






### variable loaded

```cpp
bool loaded {false};
```









-------------------------------

Updated on  9 November 2019 at 18:41:32 UTC