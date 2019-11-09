---
title: Engine::Graphics::Texture
summary: This is a base texture class that serves as a base class for other texture types.  

---

# Engine::Graphics::Texture


**Module:** **[An example game engine](/doxydown/gitbook/Modules/group__Engine.html)** **/** **[Graphical related classes](/doxydown/gitbook/Modules/group__Graphics.html)**

This is a base texture class that serves as a base class for other texture types.  [More...](#detailed-description)


`#include <Texture.hpp>`


Inherits from [Engine::Graphics::Handle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html)

Inherited by [Engine::Graphics::Framebuffer](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Framebuffer.html), [Engine::Graphics::Texture2D](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture2D.html), [Engine::Graphics::Texture3D](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture3D.html)




## Public Types

|                | Name           |
| -------------- | -------------- |
| enum | **[Type](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#enum-type)** { UNKNOWN, RGBA8, RGB8, RGBA16, RGB16, RGBA32, RGB32 } |


## Public Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[isLoaded](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#function-isloaded)**() const  |
| virtual int | **[getDepth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#function-getdepth)**() const =0 <br/>Returns the depth of the texture in pixels.  |
| virtual int | **[getHeight](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#function-getheight)**() const =0 <br/>Returns the height of the texture in pixels.  |
| virtual int | **[getWidth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#function-getwidth)**() const =0 <br/>Returns the width of the texture in pixels.  |
| virtual  | **[~Texture](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#function-~texture)**() =default  |
|  | **[Texture](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#function-texture)**([Type](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#enum-type) type)  |



## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| [Type](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#enum-type) | **[type](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#variable-type)**  |
| bool | **[loaded](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture.html#variable-loaded)**  |



## Additional inherited members






**Public Functions inherited from [Engine::Graphics::Handle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html)**

|                | Name           |
| -------------- | -------------- |
| int | **[getHandle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html#function-gethandle)**() const  |
| virtual  | **[~Handle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html#function-~handle)**() =default  |
|  | **[Handle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html#function-handle)**() =default  |



**Protected Attributes inherited from [Engine::Graphics::Handle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html)**

|                | Name           |
| -------------- | -------------- |
| int | **[handle](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Handle.html#variable-handle)**  |




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
**Reimplemented by**: [Engine::Graphics::Framebuffer::getDepth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Framebuffer.html#function-getdepth), [Engine::Graphics::Texture3D::getDepth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture3D.html#function-getdepth), [Engine::Graphics::Texture2D::getDepth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture2D.html#function-getdepth)




### function getHeight

```cpp
virtual int getHeight() const =0
```

Returns the height of the texture in pixels. 


**Return**: Height in pixels 
**Reimplemented by**: [Engine::Graphics::Framebuffer::getHeight](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Framebuffer.html#function-getheight), [Engine::Graphics::Texture3D::getHeight](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture3D.html#function-getheight), [Engine::Graphics::Texture2D::getHeight](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture2D.html#function-getheight)




### function getWidth

```cpp
virtual int getWidth() const =0
```

Returns the width of the texture in pixels. 


**Return**: Width in pixels 
**Reimplemented by**: [Engine::Graphics::Framebuffer::getWidth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Framebuffer.html#function-getwidth), [Engine::Graphics::Texture3D::getWidth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture3D.html#function-getwidth), [Engine::Graphics::Texture2D::getWidth](/doxydown/gitbook/Classes/classEngine_1_1Graphics_1_1Texture2D.html#function-getwidth)




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

Updated on  9 November 2019 at 18:38:17 UTC