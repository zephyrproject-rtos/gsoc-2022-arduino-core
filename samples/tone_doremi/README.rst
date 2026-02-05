.. _tone_doremi:

Tone DoReMi
###########

Overview
********

Use tone to play the scale.
Play Do, Re, Mi on D4 pin, then Fa, So, La on D5 pin.

Building and Running
********************

Build and flash tone_doremi sample as follows,

.. code-block:: sh

   $> west build -p -b arduino_nano_33_ble samples/tone_doremi/

   $> west flash --bossac=/home/$USER/.arduino15/packages/arduino/tools/bossac/1.9.1-arduino2/bossac
