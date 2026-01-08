# MCLV-48V-300W Motor DIM HEX Files

These HEX files are for the motor-side control DIM plugged into the
MCLV-48V-300W Development Board (EV18H47A). Use them to showcase different
control algorithms on the dyno.

Example DIM: dsPIC33CK256MP508 Motor Control DIM (EV62P66A)
https://www.microchip.com/en-us/development-tool/ev62p66a

HEX location:

`doc/standalone/`

## Included Firmware

- `dsPIC33AK128MC106_MCHP_Dyno_08_01_2026_SingleShunt_DIM.hex`
  - MCAF R8 single-shunt current measurement demo.
  - Uses DC-link shunt reconstruction with two samples per PWM period.
  - Requires a dedicated, high-priority ADC ISR that triggers twice per PWM
    cycle and a minimum switching-state window (t_sample + dead time).
  - For estimators other than ZS/MT, use single update in Dual Edge Center
    Aligned PWM mode.

- `dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_ZSMT_DIM.hex`
  - MCAF R8 Zero-Speed / Maximum Torque (ZS/MT) estimator demo.
  - Intrusive, high-frequency signal injection for zero-speed position sensing.
  - Best with IPMSM motors (requires rotor saliency, Lq != Ld).
  - Uses ZS/MT + initial position correction (IPC) at startup; commonly paired
    with a high-speed back-EMF estimator in hybrid mode.

- `dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_X2C_DIM.hex`
  - X2C-enabled variant for real-time monitoring and tuning.

## Programming (Standalone)

1. Insert the motor control DIM into the MCLV-48V-300W board.
2. Connect a programmer (ICD4, PICkit 4, SNAP).
3. In MPLAB IPE or MPLAB X, select the DIM device and program the HEX from
   `doc/standalone/`.
4. Reset the board.

Reference hardware:
https://www.microchip.com/en-us/development-tool/ev18h47a
