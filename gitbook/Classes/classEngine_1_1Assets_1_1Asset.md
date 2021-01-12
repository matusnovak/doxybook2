---
title: Engine::Assets::Asset

---

# Engine::Assets::Asset

**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Generic assets](/Modules/group__Assets.md)**

`#include <Asset.hpp>`


## Public Classes

|                | Name           |
| -------------- | -------------- |
| union | **[Event](/Classes/unionEngine_1_1Assets_1_1Asset_1_1Event.md)**  |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Asset](/Classes/classEngine_1_1Assets_1_1Asset.md#function-asset)**(const std::string & name) |
| virtual | **[~Asset](/Classes/classEngine_1_1Assets_1_1Asset.md#function-~asset)**() =0 |
| virtual void | **[load](/Classes/classEngine_1_1Assets_1_1Asset.md#function-load)**()<br>Call this method to load the asset.  |
| virtual void | **[unload](/Classes/classEngine_1_1Assets_1_1Asset.md#function-unload)**()<br>Call this method to unload the asset.  |
| const [Event](/Classes/unionEngine_1_1Assets_1_1Asset_1_1Event.md) & | **[getEvent](/Classes/classEngine_1_1Assets_1_1Asset.md#function-getevent)**() const<br>Returns the current status of the asset.  |

## Public Functions Documentation

### function Asset

```cpp
explicit Asset(
    const std::string & name
)
```


### function ~Asset

```cpp
virtual ~Asset() =0
```


### function load

```cpp
virtual void load()
```

Call this method to load the asset. 

### function unload

```cpp
virtual void unload()
```

Call this method to unload the asset. 

### function getEvent

```cpp
inline const Event & getEvent() const
```

Returns the current status of the asset. 

Use this to determine the state, for example if the assets is being loaded or unloaded. 


-------------------------------

Updated on 12 January 2021 at 23:37:34 UTC