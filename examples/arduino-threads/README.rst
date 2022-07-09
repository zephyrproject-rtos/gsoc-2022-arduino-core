.. _arduino_nano_33_ble_multi_thread_blinky:

Basic Thread Example
####################

Overview
********

This example demonstrates spawning multiple threads using
:c:func:`K_THREAD_DEFINE`. It spawns two threads. Each thread is then defined
at compile time using K_THREAD_DEFINE.

These two each control an LED. These LEDs, ``led0`` and ``led1``, have
loop control and timing logic controlled by separate functions.

- ``blink0()`` controls ``led0`` and has a 100ms sleep cycle
- ``blink1()`` controls ``led1`` and has a 1000ms sleep cycle

Requirements
************

The board must have two LEDs connected via GPIO pins. These are called "User
LEDs" on many of Zephyr's :ref:`boards`. The LEDs must be configured using the
``led0`` and ``led1`` :ref:`devicetree <dt-guide>` aliases, usually in the
:ref:`BOARD.dts file <devicetree-in-out-files>`.

You will see one of these errors if you try to build this sample for an
unsupported board:

.. code-block:: none

   Unsupported board: led0 devicetree alias is not defined
   Unsupported board: led1 devicetree alias is not defined

Building
********

For example, to build this sample for :ref:`96b_carbon_board`:

.. zephyr-app-commands::
   :zephyr-app: samples/basic/arduino-threads
   :board: arduino_nano_33_ble
   :goals: build flash
   :compact:

Change ``arduino_nano_33_ble`` appropriately for other supported boards.
