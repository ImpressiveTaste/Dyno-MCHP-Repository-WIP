Procedure for using the MC-Dyno:
================================

.. contents::
   :local:
   :depth: 2

1. Purpose
----------

Step-by-step operator checklist to prepare, run, present, and safely shut down the
DUT + MC-Dyno. 

2. Checklist before starting up the MC-Dyno
-------------------------------------------

**Hardware**

- PC, power, keyboard/mouse.
- Two MCLV boards mounted with printed brackets (use the STL brackets), motor and dyno physically connected.
- **Single DC power supply connected to both boards**.
- USB cable connected to the two boards for getting data in and out.

**Software**

- Scilab installed (tested version is OK; recommended 2024.1.0 but 6.1.1 works).
- X2C (X2C blocks in Scilab) installed and visible in Scilab on startup.
- pyX2Cscope / X2C-scope app (standalone) or web-interface app to be used for the DUT side. Can be downlaoded here: https://github.com/X2Cscope/pyx2cscope/releases

3. Power / Safety (Must-Read)
-----------------------------

.. warning::

   - **Use one DC power supply only**. Do NOT power the motor board and dyno board with separate supplies.
     Link + / - terminals so both boards share the same supply.
   - Fault / safety LED: All MCLV-48V-300W boards have a hardware safety switch ("fault" / overcurrent protection).
     If that LED is ON the PWMs are disconnected (no power to gate drivers). Press the small fault-reset button
     (one press) to clear and re-enable the power stage.
   - Never press mechanical switches on the Dyno Side while the motor is running. If behavior becomes uncontrollable:
     STOP the DUT side (switch1), reset fault if lights up, then restart.

4. Start-up Procedure (Order Is Important)
------------------------------------------

#. Turn on power only after cabling is verified.
#. Start Scilab.
#. Confirm X2C is loaded in Scilab (X2C menu appears). If X2C does not initialize on Scilab startup,
   run the X2C init script.
#. In the Scilab file browser, right-click ``init_project.sce`` and Execute.
   Wait for XCOS to open with the MC-Dyno model.
#. Double-click "Transform model" to generate the runtime model for the communicator.
#. Leave the Communicator window open. Check the communicator window title: model name should appear
   (for example, ``MC_FOC_Dyno``). If it matches, proceed.
#. Open Windows Device Manager -> Ports to see COM assignments.
   If unsure which COM is which, unplug the motor-side USB to identify dyno vs motor port.
   Typical mapping used in testing: Dyno = ``COM5``, Motor = ``COM4`` -- verify locally.
#. In the Scilab model: double-click Start Communicator -> Setup -> select the COM port and refresh ports.
   Connect. If COM error appears, choose the other COM and try again.
#. In the Scilab model: set torque mode (double-click torque-mode selector). 
   IN here you can choose different torque mdoes, such as trapezoidal dynamic loads (safer for long running) or constant torque for a simple demo. For a quick demo:
   use constant torque, then switch to trapezoidal or unbalanced load to analyze effects.

6. X2C-scope / pyX2Cscope Usage (Data and Visualization)
--------------------------------------------------------

#. Launch the pyX2Cscope / X2C-scope app (standalone) or open the web-interface app. Can be downlaoded here: https://github.com/X2Cscope/pyx2cscope/releases
#. Select the ELF file associated to the code that is running in the DUT (this lets the scope map variable names to MCU memory). You can find the elf file in *production/dist*, inside the project folder. 
#. Select the port (auto-connect tries COM ports; if it chooses wrong, manually select the COM).
#. Add variables to view (if you are running an MCAF based prject):

   - ``motor.IDQ.Q`` (quadrature current -> torque)
   - ``motor.IDQ.D`` (direct current)
   - ``IABC.IA`` (phase current IA)
   - ``motor.OmegaElectrical`` (or ``OmegaMechanical`` / velocity scaled to RPM)

#. If you are running fixed point code copy gain values into each variable's "gain" field (this is optional but usefull since numeric changes are helpful to show RPM/current in real units).
#. Sample time factor:

   - For slow trapezoidal loads and long runs: use Sample Time Factor ~ 50
   - For dynamic/transient viewing of current/IDQ: reduce factor (5 or 1) for higher time resolution
   - If scope is noisy, increase factor (longer time window) or pick appropriate variables

#. Press SAMPLE to display live traces.

7. Shutdown Procedure (In Order)
--------------------------------

#. Stop motor (switch1).
#. In X2C scope: press STOP, disconnect.
#. In Scilab: stop communicator, close model windows. When prompted to save, select DO NOT SAVE
   (prevents accidental model changes).
#. Close Scilab.
#. Power down boards.

8. Common Problems and Fixes
----------------------------

- **Two separate power supplies:** do NOT use two supplies. Link green terminals and use single supply --
  otherwise regenerative braking can raise voltage and damage boards.
- **Motor cogging / erratic below ~50 RPM:** encoder resolution limits low-speed smoothness. Keep demo speeds
  above ~50-100 RPM or use 0-speed torque cautiously. If controller locks / spins fast unexpectedly: STOP motor,
  press switch1 to reset, restart.
- **Scope shows unexpected values or no values:** verify ELF-file loaded in scope, correct COM port, and that the
  Communicator model matches the model name in the communicator window.