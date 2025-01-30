.. _eeprom_operations:

.. _eeprom_operations:

EEPROM Operations
#################

Overview
********

Read and write data from flash memory using this sample.

Prerequisites and Setup
***********************

Use any board that supports Zephyr RTOS.  
This example uses **BeagleConnect Freedom**.

For setup, refer to the `documentation <https://docs.beagle.cc/boards/beagleconnect/freedom/demos-and-tutorials/using-arduino-zephyr-template.html#setup-arduino-workspace>`_.

Build and Test
**************

Follow these steps to set up and run the sample in the `arduino-workspace` directory:

1. **Set up the virtual environment:**

   .. code-block:: bash

      source .venv/bin/activate

2. **Build the EEPROM operations sample:**

   .. code-block:: bash

      west build -b beagleconnect_freedom modules/lib/Arduino-Zephyr-API/samples/eeprom_operations -p

3. **Flash the code after connecting BeagleConnect Freedom to your device:**

   .. code-block:: bash

      west flash

4. **Open the serial console to view the output:**

   .. code-block:: bash

      tio /dev/ttyACM0

Sample Output
*************

Run the code and observe the following output:

.. code-block:: text

    Serial communication initialized
    NVS initialized
    Data written successfully
    Data read: 1234
