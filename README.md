# µUIKit — a simple framework for building UI on MCUs

## Description

The main idea of this project is to provide minimal set of widgets to build UI for your embedded/IoT project if you are using widely available displays.


## Widgets

Since the project is in active development stage, some widgets are planned but not implemented yet. Here is the list of widgets *implemented* and planned.

| Widget      | Rendering          | Selection | Focus |
|-------------|--------------------|-----------|-------|
| Label       | :heavy_check_mark: | :x:       | :x:   |
| PushButton  | :heavy_check_mark: | :soon:    | :x:   |
| CheckBox    | :heavy_check_mark: | :soon:    | :x:   |
| Menu        | :heavy_check_mark: | :soon:    | :x:   |
| Pixmap      | :heavy_check_mark: | :x:       | :x:   |
| ProgressBar | :x:                | :x:       | :x:   |
| Slider      | :x:                | :x:       | :x:   |
| Dialog      | :x:                | :x:       | :x:   |

:heavy_check_mark: — Implemented, available

:soon: — Will be available soon

:x: — Nope, we don't have it **yet**.


## Important notes and restrictions

Since there are no many developers/test involved in the development(yet) there will be some limitations on hardware for which this library is designed to work.

### Displays

This library is expected to be used with monochrome displays. Additionally out of box input devices support is limited by push switches and encoders.

All code is will be initially tested using SSD1603 (128 x 64 resolution).

### Performance

Hardware like Arduino Uno/Nano/etc are still very popular, but optimization for these MCUs might reflect on API changes and make API more ugly. Also library make require some more abstraction layers which also could affect on efficiency and usability.

Additionally it worth to mention that code is tested using ESP32 and RP2040 microcontrollers. And C++11 support is **required**.

### Protocol

The first is that the display is communicated via SPI.  But in general, since there are not so many animations everything should work just fine if you connected your display via I2C.  The difference of protocol is the maximum speed.  On faster bus sending buffers to render should and will take more time.