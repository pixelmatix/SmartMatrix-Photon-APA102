Port of the [SmartMatrix Library](https://github.com/pixelmatix/SmartMatrix) to the Particle Photon with support for driving APA102 LEDs (only).

How to compile using [Particle Build](https://build.particle.io):

* Import SmartMatrix-Photon-APA102
  * In Libraries View, press "Contribute Library"
  * `pixelmatix/SmartMatrix-Photon-APA102`
  * "Add Library"
  * "Import"

* Import FastLED with SmartMatrix Fix
  * This is needed as the FastLED library currently on Particle Build is 3.0, not the latest 3.1 that has a one-line fix for SmartMatrix3 compatibility.  You need to add this [renamed fork of FastLED](https://github.com/pixelmatix/fastled-sparkcore) with the one-line fix as a workaround for now
  * In Libraries View, press "Contribute Library"
  * `pixelmatix/FastLED-Sparkcore`
  * "Add Library"
  * "Import"

* Compile an Example
  * In Libraries - choose "SmartMatrix3_Photon_Apa102"
  * Select one of the examples ending in `.INO` at the top of the list
  * "Use This Example"
  * With Example example selected from the "Code" View,
  * Change to Libraries View - choose "FastLEDSmartMatrix"
  * Scroll down, "Include In App"
  * Select one of the examples
  * "Add to this App"
  * Delete the first `#include` that was put there automatically by the IDE (the FastLED include needs to be after the SmartMatrix include)

Known Issues:

`matrix.setBrightness()` breaks if value is below 8
`matrix.setRefreshRate()` must be called before `matrix.begin()`, or it has no effect
