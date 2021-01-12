---
title: Engine::Graphics::Framebuffer


---

# Engine::Graphics::Framebuffer


**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Graphical related classes](/Modules/group__Graphics.md)**



`#include <Framebuffer.hpp>`















## Public Functions

|                | Name           |
| -------------- | -------------- |
|  | **[Framebuffer](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-framebuffer)**(int width, int height)  |
| virtual  | **[~Framebuffer](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-~framebuffer)**()  |
| virtual int | **[getWidth](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getwidth)**() const override <br>Returns the width of the texture in pixels.  |
| virtual int | **[getHeight](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getheight)**() const override <br>Returns the height of the texture in pixels.  |
| virtual int | **[getDepth](/Classes/classEngine_1_1Graphics_1_1Framebuffer.md#function-getdepth)**() const override <br>Returns the depth of the texture in pixels.  |





















## Public Functions Documentation

### function Framebuffer

```cpp
Framebuffer(
    int width,
    int height
)
```





























### function ~Framebuffer

```cpp
virtual ~Framebuffer()
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

Updated on 12 January 2021 at 21:36:39 UTC