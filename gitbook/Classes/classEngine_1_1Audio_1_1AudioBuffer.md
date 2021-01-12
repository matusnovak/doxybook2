---
title: Engine::Audio::AudioBuffer
summary: A simple audio buffer to hold PCM samples 

---

# Engine::Audio::AudioBuffer


**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Audio buffers and devices](/Modules/group__Audio.md)**

_A simple audio buffer to hold PCM samples_ [More...](#detailed-description)


`#include <AudioBuffer.hpp>`





## Public Classes

|                | Name           |
| -------------- | -------------- |
| struct | **[TypedAudioData](/Classes/structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData.md)** <br>Use this to populate the buffer.  |


## Public Types

|                | Name           |
| -------------- | -------------- |
| enum | **[Type](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#enum-type)** { UNKNOWN = 0, INT_8 = 1 << 1, INT_16 = 1 << 2, INT_24 = 1 << 3, INT_32 = 1 << 4, FLOAT_32 = 1 << 5 }<br>Different type of audio formats.  |
| template \<typename T \><br>using [Utils::ArrayView](/Classes/classEngine_1_1Utils_1_1ArrayView.md)< T > | **[AudioData](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#using-audiodata)**  |
| typedef [AudioData](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#using-audiodata)< uint8_t > | **[AudioData8U](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#typedef-audiodata8u)**  |








## Public Functions

|                | Name           |
| -------------- | -------------- |
|  | **[AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-audiobuffer)**(const std::string & filename) <br>Constructor for [Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md).  |
| virtual  | **[~AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-~audiobuffer)**() =default <br>[Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md) destructor.  |
| void | **[play](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-play)**([AudioManager](/Classes/classEngine_1_1Audio_1_1AudioManager.md) & manager) const <br>Play this buffer.  |
| void | **[stop](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-stop)**([AudioManager](/Classes/classEngine_1_1Audio_1_1AudioManager.md) & manager) const <br>Stop this buffer playing.  |
| void | **[loop](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-loop)**([AudioManager](/Classes/classEngine_1_1Audio_1_1AudioManager.md) & manager) const <br>Loop this buffer forever.  |
| void | **[setData](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setdata)**(const [TypedAudioData](/Classes/structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData.md) & data)  |
| template \<size_t Size\><br>void | **[setDataMultiple](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setdatamultiple)**(const [TypedAudioData](/Classes/structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData.md) data[Size])  |
| void | **[setData](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setdata)**(const [TypedAudioData](/Classes/structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData.md) data[], size_t size) <br>Some deprecated function.  |
| void | **[setCallback](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setcallback)**([Callback](/Namespaces/namespaceEngine_1_1Audio.md#typedef-callback) callback) <br>Set the callback function.  |
| void | **[setCallback2](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setcallback2)**([Callback2](/Namespaces/namespaceEngine_1_1Audio.md#using-callback2) callback, void * user) <br>Set the callback function.  |

## Protected Functions

|                | Name           |
| -------------- | -------------- |
| float * | **[getData](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-getdata)**()  |




## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[playing](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#variable-playing)**  |

## Friends

|                | Name           |
| -------------- | -------------- |
| class | **[AudioManager](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#friend-audiomanager)**  |
| void | **[Audio::doSomething](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#friend-audiodosomething)**([AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md) & buffer)  |





## Detailed Description

```cpp
class Engine::Audio::AudioBuffer;
```

_A simple audio buffer to hold PCM samples_








**Author**: Matus Novak 



**Date**: 2017-2019 

**Note**: 

  * Some random note 
  * Some second random note 


**Bug**: Some random bug 

Some other random bug 

**Test**: Some random test description 

**Todo**: Some random todo 

**Warning**: Some random warning 

**Precondition**: First initialize the system. 











Lorem ipsum donor 

```cpp
// Some random code
using namespace Engine;

Audio::AudioBuffer buffer("path/to/file.wav");
buffer.play(audioManager);
```

 More detailed description! 

```cpp
// Another code sample
#include <iostream>
using namespace Engine;
Audio::AudioBuffer buffer("path/to/file.wav");
std::cout << buffer.play(...) << std::endl;
```



## Public Types Documentation

### enum Type


| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
| UNKNOWN | 0 |  Dont use this  |
| INT_8 | 1 << 1 |  8-bit signed integer  |
| INT_16 | 1 << 2 |  16-bit signed integer  |
| INT_24 | 1 << 3 |  24-bit signed integer  |
| INT_32 | 1 << 4 |  32-bit signed integer  |
| FLOAT_32 | 1 << 5 |  32-bit float  |



Different type of audio formats. 




























### using AudioData

```cpp
template <typename T >
using Engine::Audio::AudioBuffer::AudioData =  Utils::ArrayView<T>;
```





























### typedef AudioData8U

```cpp
typedef AudioData<uint8_t> Engine::Audio::AudioBuffer::AudioData8U;
```





































## Public Functions Documentation

### function AudioBuffer

```cpp
explicit AudioBuffer(
    const std::string & filename
)
```

Constructor for [Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md). 




























### function ~AudioBuffer

```cpp
virtual ~AudioBuffer() =default
```

[Audio::AudioBuffer](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md) destructor. 




























### function play

```cpp
void play(
    AudioManager & manager
) const
```

Play this buffer. 

**Parameters**: 

  * **manager** Which manager to play the sound with 



**Exceptions**: 

  * **[Exception](/Classes/classEngine_1_1Exception.md)** If this buffer is already playing 


























### function stop

```cpp
void stop(
    AudioManager & manager
) const
```

Stop this buffer playing. 

**Parameters**: 

  * **manager** Which manager to stop the sound with 



**Exceptions**: 

  * **[Exception](/Classes/classEngine_1_1Exception.md)** If this buffer is already stopped 


























### function loop

```cpp
void loop(
    AudioManager & manager
) const
```

Loop this buffer forever. 

**Parameters**: 

  * **manager** Which manager to loop the sound with 



**Exceptions**: 

  * **[Exception](/Classes/classEngine_1_1Exception.md)** If this buffer is already looping 










**Note**: This will loop forever until you call stop 
















### function setData

```cpp
void setData(
    const TypedAudioData & data
)
```





























### function setDataMultiple

```cpp
template <size_t Size>
inline void setDataMultiple(
    const TypedAudioData data[Size]
)
```





























### function setData

```cpp
void setData(
    const TypedAudioData data[],
    size_t size
)
```

Some deprecated function. 





**Deprecated**: 

Use the [AudioBuffer::setDataMultiple](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setdatamultiple) instead 























### function setCallback

```cpp
void setCallback(
    Callback callback
)
```

Set the callback function. 

**Parameters**: 

  * **callback** The callback function pointer 





**Deprecated**: 

Use the [AudioBuffer::setCallback2](/Classes/classEngine_1_1Audio_1_1AudioBuffer.md#function-setcallback2) instead 

**See**: [Audio::Callback](/Namespaces/namespaceEngine_1_1Audio.md#typedef-callback)






















### function setCallback2

```cpp
void setCallback2(
    Callback2 callback,
    void * user
)
```

Set the callback function. 

**Parameters**: 

  * **callback** The callback function pointer 






**See**: [Audio::Callback](/Namespaces/namespaceEngine_1_1Audio.md#typedef-callback)























## Protected Functions Documentation

### function getData

```cpp
float * getData()
```

































## Protected Attributes Documentation

### variable playing

```cpp
bool playing {false};
```






























## Friends

### friend AudioManager

```cpp
friend class AudioManager;
```





























### friend Audio::doSomething

```cpp
friend void Audio::doSomething(
    AudioBuffer & buffer
);
```































-------------------------------

Updated on 12 January 2021 at 21:41:22 UTC