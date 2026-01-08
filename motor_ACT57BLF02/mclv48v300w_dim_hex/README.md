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
  - Target DIM: dsPIC33AK128MC106.
  - MCAF R8 single-shunt current measurement demo.
  - Uses DC-link shunt reconstruction with two samples per PWM period.
  - Requires a dedicated, high-priority ADC ISR that triggers twice per PWM
    cycle and a minimum switching-state window (t_sample + dead time).
  - For estimators other than ZS/MT, use single update in Dual Edge Center
    Aligned PWM mode.

- `dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_ZSMT_DIM.hex`
  - Target DIM: dsPIC33CK256MP508.
  - Zero-Speed / Maximum Torque (ZS/MT) Estimator
  - ZS/MT is a sensorless position and speed estimation algorithm specifically
    designed to operate where back-EMF based estimators fail, at zero and
    ultra-low speeds.
  - Operating Principle - High-Frequency Signal Injection:
    ZS/MT injects a high-frequency excitation voltage into the stator and
    measures the motor response to determine rotor angle at standstill.
  - Motor Requirements (Saliency):
    Requires significant Ld != Lq saliency, intended for IPMSM machines.
  - Startup Sequence - IPC for Zero-Speed Torque Production:
    Uses "ZS/MT + Initial Position Correction (IPC)" to align the stator field
    before motion begins.
  - Hybrid Operation - Low-Speed to High-Speed Transition:
    Uses ZS/MT at low speed and a back-EMF estimator (for example, PLL) at
    higher speed due to voltage headroom limits.
  - Tuning and Configuration Parameters

- `dsPIC33CK256MP508_MCHP_Dyno_08_01_2026_X2C_DIM.hex`
  - Target DIM: dsPIC33CK256MP508.
  - X2C-enabled variant for real-time monitoring and tuning.

## Programming (Standalone)

1. Insert the motor control DIM into the MCLV-48V-300W board.
2. Connect a programmer (ICD4, PICkit 4, SNAP).
3. In MPLAB IPE or MPLAB X, select the DIM device and program the HEX from
   `doc/standalone/`.
4. Reset the board.

Reference hardware:
https://www.microchip.com/en-us/development-tool/ev18h47a
