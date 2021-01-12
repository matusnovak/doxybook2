---
title: Engine::Gui::GuiWidget
summary: Some generic gui widget class. 

---

# Engine::Gui::GuiWidget

**Module:** **[An example game engine](/Modules/group__Engine.md)** **/** **[Some gui related stuff and whatever](/Modules/group__Gui.md)**

Some generic gui widget class. `#include <GuiWidget.hpp>`


## Public Slots

|                | Name           |
| -------------- | -------------- |
| void | **[mySlot](/Classes/classEngine_1_1Gui_1_1GuiWidget.md#slot-myslot)**(QObject * object, int a, bool b) |

## Public Signals

|                | Name           |
| -------------- | -------------- |
| void | **[mySignal](/Classes/classEngine_1_1Gui_1_1GuiWidget.md#signal-mysignal)**(QObject * object, int a, bool b) |

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[GuiWidget](/Classes/classEngine_1_1Gui_1_1GuiWidget.md#function-guiwidget)**(QObject * parent) |
| virtual | **[~GuiWidget](/Classes/classEngine_1_1Gui_1_1GuiWidget.md#function-~guiwidget)**() =default |

## Public Slots Documentation

### slot mySlot

```cpp
void mySlot(
    QObject * object,
    int a,
    bool b
)
```


## Public Signals Documentation

### signal mySignal

```cpp
void mySignal(
    QObject * object,
    int a,
    bool b
)
```


## Public Functions Documentation

### function GuiWidget

```cpp
explicit GuiWidget(
    QObject * parent
)
```


### function ~GuiWidget

```cpp
virtual ~GuiWidget() =default
```


-------------------------------

Updated on 12 January 2021 at 23:37:34 UTC