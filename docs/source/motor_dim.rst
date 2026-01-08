Motor DIM HEX (MCLV-48V-300W)
=============================

These HEX files target the motor-side control DIM installed on the
MCLV-48V-300W Development Board (EV18H47A). They are used to showcase different
motor-control algorithms on the dyno.

Example DIM: dsPIC33CK256MP508 Motor Control DIM (EV62P66A)
https://www.microchip.com/en-us/development-tool/ev62p66a

HEX Location
------------

``motor_ACT57BLF02/mclv48v300w_dim_hex/doc/standalone/``

Downloads
---------

- :download:`Single-shunt DIM HEX <_downloads/dsPIC33AK128MC106_MCHP_Dyno_08_01_2026_SingleShunt_DIM.hex>`
- :download:`ZS/MT DIM HEX <_downloads/dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_ZSMT_DIM.hex>`
- :download:`X2C DIM HEX <_downloads/dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_X2C_DIM.hex>`

Algorithm Notes
---------------

Single-shunt current measurement (MCAF R8):

- Samples the DC-link current twice in a single PWM period and reconstructs
  phase currents based on the inverter switching state.
- Requires a dedicated, high-priority ADC ISR and a minimum switching-state
  window (t_sample + dead time).
- For estimators other than ZS/MT, use single update in Dual Edge Center
  Aligned PWM mode.

Zero-Speed / Maximum Torque (ZS/MT) estimator (MCAF R8):

- Intrusive estimator that injects a high-frequency voltage signal to estimate
  rotor position at zero or near-zero speed.
- Requires rotor saliency (IPMSM, Lq != Ld) and uses ZS/MT + IPC at startup.
- Typically used in hybrid mode (ZS/MT at low speed, back-EMF estimator at
  higher speed).

Reference hardware:
https://www.microchip.com/en-us/development-tool/ev18h47a
