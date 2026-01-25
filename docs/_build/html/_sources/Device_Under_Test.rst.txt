Motor Side (Device Under Test)
==============================

These HEX files target the motor-side control DIM installed on the
MCLV-48V-300W Development Board (EV18H47A). They are used to showcase different
motor-control algorithms on the dyno.

**2.2.5 Monitor motor behavior with pyX2Cscope**

To view motor behavior on the DUT side, you can use `pyX2Cscope <https://x2cscope.github.io/pyx2cscope/>`_ through Python.
There are also standalone apps you can download and run directly without installing anything beyond the app itself.

.. list-table::
   :widths: 50 50
   :align: center

   * - .. figure:: _static/images/pyX2CscopeStandalone-WebInterface.png
          :alt: pyX2Cscope Standalone Web Interface
          :width: 95%

          pyX2Cscope Standalone Web Interface
     - .. figure:: _static/images/pyX2Cscope-Standalone-App.png
          :alt: pyX2Cscope Standalone App
          :width: 95%

          pyX2Cscope Standalone App
   * - .. figure:: _static/images/DownloadingAndUsingpyX2CscopeStandalone.png
          :alt: Downloading and using pyX2Cscope Standalone
          :width: 95%

          Downloading and using pyX2Cscope Standalone
     - .. figure:: _static/images/DownloadingAndUsingpyX2CscopeStandalone2.png
          :alt: Downloading and using pyX2Cscope Standalone (2)
          :width: 95%

          Downloading and using pyX2Cscope Standalone (2)


Example DIM: dsPIC33CK256MP508 Motor Control DIM (EV62P66A)
https://www.microchip.com/en-us/development-tool/ev62p66a

HEX Location
------------

``motor_ACT57BLF02/mclv48v300w_dim_hex/doc/standalone/``

Downloads
---------

.. container:: motor-dim-downloads

   :download:`Single-shunt (dsPIC33AK128MC106 DIM) <_downloads/dsPIC33AK128MC106_MCHP_Dyno_08_01_2026_SingleShunt_DIM.hex>`

   :download:`ZS/MT (dsPIC33CK256MP508 DIM) <_downloads/dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_ZSMT_DIM.hex>`

   :download:`X2C (dsPIC33CK256MP508 DIM) <_downloads/dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_X2C_DIM.hex>`

   :download:`QuickSpin (ATSAME54P20A DIM) <_downloads/ATSAME54P20A_MCHP_Dyno_08_01_2026_QuickSpin_DIM.hex>`

   :download:`StreamAnalyze (ATSAME54P20A DIM) <_downloads/ATSAME54P20A_MCHP_Dyno_08_01_2026_StreamAnalyze_DIM.hex>`


Algorithm Notes
---------------

DIM mapping:

- Single-shunt demo runs on the dsPIC33AK128MC106 DIM.
- ZS/MT and X2C demos run on the dsPIC33CK256MP508 DIM.

Single-shunt current measurement (MCAF R8):

- Samples the DC-link current twice in a single PWM period and reconstructs
  phase currents based on the inverter switching state.
- Requires a dedicated, high-priority ADC ISR and a minimum switching-state
  window (t_sample + dead time).
- For estimators other than ZS/MT, use single update in Dual Edge Center
  Aligned PWM mode.

Zero-Speed / Maximum Torque (ZS/MT) Estimator

ZS/MT is a sensorless position and speed estimation algorithm specifically
designed to operate where back-EMF based estimators fail, at zero and ultra-low
speeds.

Operating Principle - High-Frequency Signal Injection

Unlike PLL-type estimators, ZS/MT is intrusive: it injects a high-frequency
excitation voltage into the stator and measures the motor's response. This
leverages motor saliency to determine rotor angle even at standstill.

Motor Requirements (Saliency)

ZS/MT requires a motor with significant Ld != Lq saliency. It is thus primarily
intended for IPMSM machines, where q-axis inductance is notably higher than
d-axis inductance.

Startup Sequence - IPC for Zero-Speed Torque Production

To deliver torque from standstill, ZS/MT uses a startup procedure termed
"ZS/MT + Initial Position Correction (IPC)", allowing the controller to
correctly align the stator field before any mechanical motion begins.

Hybrid Operation - Low-Speed to High-Speed Transition

Because the injected signal consumes voltage headroom, ZS/MT cannot achieve the
absolute maximum speed by itself. In practice, it is combined with a back-EMF
estimator (for example, PLL) using a hybrid scheme (for example, "Minotaur
hard-switch") to allow:

- ZS/MT for 0-to-low-speed operation
- PLL for mid-to-high-speed operation

Tuning and Configuration Parameters

Reference hardware:
https://www.microchip.com/en-us/development-tool/ev18h47a
