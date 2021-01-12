---
title: Engine::Audio::AudioManager
summary: An audio manager that accepts multiple Audio::AudioBuffer instances.  

---

# Engine::Audio::AudioManager


**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Audio buffers and devices](/Modules/group__Audio.md)**

An audio manager that _accepts_ multiple **[Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md)** instances.  [More...](#detailed-description)


`#include <AudioManager.hpp>`















## Public Functions

|                | Name           |
| -------------- | -------------- |
|  | **[AudioManager](/Classes/classEngine_1_1Audio_1_1AudioManager.md#function-audiomanager)**(int numOfChannels =128)  |
|  | **[~AudioManager](/Classes/classEngine_1_1Audio_1_1AudioManager.md#function-~audiomanager)**()  |
| void | **[enque](/Classes/classEngine_1_1Audio_1_1AudioManager.md#function-enque)**(const [AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md) & buffer)  |











## Detailed Description

```cpp
class Engine::Audio::AudioManager;
```

An audio manager that _accepts_ multiple **[Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md)** instances. 






**See**: [Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md)




















Lorem Ipsum Donor. Some [Random link with **bold** and _italics_](http://github.com) And the following is a `typewritter` font. And here is some list items:

* First item
* Second item
* Third item **with bold text**











## Public Functions Documentation

### function AudioManager

```cpp
AudioManager(
    int numOfChannels =128
)
```





























### function ~AudioManager

```cpp
~AudioManager()
```





























### function enque

```cpp
void enque(
    const AudioBuffer & buffer
)
```





































-------------------------------

Updated on 12 January 2021 at 21:41:22 UTC