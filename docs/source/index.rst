MCHP-Dyno
=========

The **MCHP-Dyno** is an **open** project that includes both software and hardware to enable anyone 
to build their own motor dynamometer for testing and tuning motor-control algorithms.
The project supports both high-voltage and low-voltage configurations on the dyno side, 
and can test virtually **any electric motor** on the DUT (Device-Under-Test) side.

.. admonition:: By the way...

   “Dyno” is short for *dynamometer*. In this project the dyno is implemented using a 
   **PMSM motor** which must have an encoder. This motor is connected to a control board
   running the MCHP-Dyno software, and the control board, is connected to a **PC running Scilab/XCOS**
   or external an controller (for example the :doc:`IGAT <igat>`) for interfacing with it. 
   In a motor-dyno setup, the motor is the device that *produces* mechanical power, 
   while the dyno is the machine that *absorbs* mechanical power and measures the motor’s performance.
   Using this configuration you can apply a controllable mechanical load to the motor-under-test
   and observe its response (torque, speed, power, efficiency, thermal behavior, etc.).
   For basic characterization a dedicated torque sensor is not required. Because the **dyno is “smart”**
   (motor-as-brake), you can simulate **different load profiles** and dynamic conditions, enabling more
   meaningful testing than with simple passive loads.

.. image:: _static/images/HighLevel-BlockDiagram.jpg
   :alt: High-level block diagram



Sections
--------

* What MCHP-Dyno is
* How to set it up
* How to run DYNO and MOTOR modes
* How to use X2C Scope for real-time monitoring


--------

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   install
   igat
   motor_dim
   usage
   modes
   x2c_scope
   hardware

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
