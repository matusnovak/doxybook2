---
title: Engine::Graphics::Texture2D


---

# Engine::Graphics::Texture2D


**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Graphical related classes](/Modules/group__Graphics.md)**



`#include <Texture2D.hpp>`















## Public Functions

|                | Name           |
| -------------- | -------------- |
|  | **[Texture2D](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-texture2d)**(int width, int height, uint8_t * data, [Texture::Type](/Classes/classEngine_1_1Graphics_1_1Texture.md#enum-type) type =Texture::Type::RGBA8)  |
| virtual  | **[~Texture2D](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-~texture2d)**()  |
| virtual int | **[getWidth](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getwidth)**() const override <br>Returns the width of the texture in pixels.  |
| virtual int | **[getHeight](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getheight)**() const override <br>Returns the height of the texture in pixels.  |
| virtual int | **[getDepth](/Classes/classEngine_1_1Graphics_1_1Texture2D.md#function-getdepth)**() const override <br>Returns the depth of the texture in pixels.  |





















## Public Functions Documentation

### function Texture2D

```cpp
Texture2D(
    int width,
    int height,
    uint8_t * data,
    Texture::Type type =Texture::Type::RGBA8
)
```


**Parameters**: 

  * **data** The pointer to the raw pixels data 
  * **width** The width of the image held by the data pointer 
  * **height** The height of the image held by the data pointer 
  * **type** What is the type of this pixels array? 




























### function ~Texture2D

```cpp
virtual ~Texture2D()
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

Updated on 12 January 2021 at 21:41:22 UTC