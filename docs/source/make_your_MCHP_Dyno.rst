Make Your MCHP-Dyno
===================

.. contents::
   :local:
   :depth: 2

1. Introduction
---------------

This page intent is to guide you on the creation of your first MCHP-Dyno project.
The MCHP-Dyno proposed here, is the cheapest and easiest assembly that can be done
to get a working MCHP-Dyno setup.
You can use this page as a guide even if you have a different motor, a different
controller board and/or different motor holders. 

.. tip::

   In some instances you will find **tip** blocks like these... in here you
   will find tips and tricks on how to improve or "hack" the MCHP-Dyno for your
   specific needs.

.. warning::

   Warning blocks are not decoration. If you ignore them, your MCHP-Dyno may
   respond with mysterious smoke. Read carefully—future-you will thank present-you.
   This warning has already saved several MCHP-Dyno projects...
   
All of the files and links that are required during the built are available here. 
These are mainly 3D models and source code.

Once you start following the different chapters, open the corresponding files and
links to have all of the required files handy.

The content might be updated over time. Legacy code will still be available in the **legacy** folder 
where the legacy files will be organized by date when they where out-commissioned.


1.1 Suggested tools
~~~~~~~~~~~~~~~~~~~

You will need the screwdriver for the screws, nuts and shaft coupling.

No other particular tools are needed.

1.2 Suggested hardware
~~~~~~~~~~~~~~~~~~~~~~

**Required hardware**

- Two `MCLV-48V-300W Development Board <https://www.microchip.com/en-us/development-tool/ev18h47a>`_ if you also want to create the driver for the Device-Under-Test side (motor).
- One `MCLV-48V-300W Development Board <https://www.microchip.com/en-us/development-tool/ev18h47a>`_ if you only need the dynamometer side. 
- For the Dynamometer side you will need a motor with encoder. The standard one used is the: `AC300022 - 24V 3-PHASE BRUSHLESS DC MOTOR WITH ENCODER <https://www.microchip.com/en-us/development-tool/ev78u65a>`_. 
- For the Device-Under-Test (motor driver) code demonstrations, in this repository, the majority was made for the `ACT57BLF02 Motor <https://www.act-motor.com/brushless-dc-motor-57blf-product/>`_ and some for the `AC300022 - 24V 3-PHASE BRUSHLESS DC MOTOR WITH ENCODER <https://www.microchip.com/en-us/development-tool/ev78u65a>`_.
- One `3A 24V Power Supply <https://www.microchipdirect.com/dev-tools/AC002013>`_
- Flexible aluminum jaw shaft coupling (if the above suggested motors are used, then 8mm bore should be selected).
- Eight M4 × 10 mm hex-socket screws
- 3D printed brackets, which can be found in the `3Dparts <../../../3Dparts/>`_ folder (OpenSCAD + STL).
- Wood mounting base or 4 (20×20) T-slot aluminum profiles, minimum length 500 mm
- For T-slot mounting Eight M5 socket-head cap screws (M5 SHCS) - 16 mm
- For T-slot mounting Eight M5 T-slot nuts
- Windows PC running Windows 10.

**Alternative Hardware** 

- For High Voltage Motor Testing - Two `MCHV-230V-1.5kW Development Board <https://www.microchip.com/en-us/development-tool/ev78u65a>`_ if you also want to create the driver for the Device-Under-Test side (motor).
- For High Voltage Motor Testing - One `MCHV-230V-1.5kW Development Board <https://www.microchip.com/en-us/development-tool/ev78u65a>`_ if you only need the dynamometer side. 
- `MCLV-2 Development Board <https://www.microchip.com/en-us/development-tool/dm330021-2>`_ can be used instead of the MCLV-48V-300W for the dynamomter side, performance is very similar. 
- An alternative motor that can be used for the Dynamometer side is the `ACT57BLF02 Motor <https://www.act-motor.com/brushless-dc-motor-57blf-product/>`_, this motor is also the one most used for the Device-Under-Test side (motor) demonstrations.


.. tip::

   If you want the most up-to-date hardware, that is currently used in the majority of the MCHP-Dyno projects is: 
   - Two `MCLV-48V-300W Development Board <https://www.microchip.com/en-us/development-tool/ev18h47a>`_.
   - Two `ACT57BLF02 Motor <https://www.act-motor.com/brushless-dc-motor-57blf-product/>`_

1.3 Software needed
~~~~~~~~~~~~~~~~~~~

- Scilab 6.1.1 (installation directory: https://www.scilab.org/download/previous-versions); newer versions will not work  
- X2C 6.4 (Installation tutorial: https://www.youtube.com/watch?v=H_EVY1D95eY)  
- MPLAB X IDE and MPLAB X IPE (installation: https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- XC-DSC 3.21, which you can install during process of installation of MPLAB X

2. Assembling the hardware
--------------------------

In this section the steps to build a MCHP-Dyno will be described. 
This is the easiest possible MCHP-Dyno you can make, but as you saw in the home page, you are free to play around
and make the MCHP-Dyno that best suits your needs.

Before starting this guide, you will need to navigate to the file section `3Dparts <../../../3Dparts/>`_
and 3D print the motor mounts for the motors used, in this case, the ACT57BLF02 motors (so the file needed will be the ACT_57BLF0x_unified_V1_00.stl). 
You'll need to print one of these per
motor. You can also find the openSCAD files for the brackets
in there, so if you would like to modify this model to fit a different motor, you're welcome to do so. 

You can print this model in PLA or PETG and make sure to set the infill to 100%. 
You don't need to enable any supports.



2.1 Organzie the hardware
~~~~~~~~~~~~~~~~~~~~~~~~~

Placeholder: add the first assembly subsection here.

2.2 Subsection title
~~~~~~~~~~~~~~~~~~~~

Placeholder: add the second assembly subsection here.

3. Load the software
--------------------

4. Run the MCHP-Dyno - step-by-step
-----------------------------------
