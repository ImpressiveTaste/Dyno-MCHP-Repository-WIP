/*
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 2830 $
 * $LastChangedDate:: 2023-05-08 16:06:26 +0200#$
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: */
/* USERCODE-END:Description                                                                                           */

/* USERCODE-BEGIN:ExternalModules                                                                                     */
/* USERCODE-END:ExternalModules                                                                                       */

/* Block sample time                                                                                                  */
/* bp_ts                                                                                                              */

/* Mask parameter values                                                                                              */
/* mp_Ls (Number)                                                                                                     */
/* mp_Rs (Number)                                                                                                     */
/* mp_U0 (Number)                                                                                                     */
/* mp_I0 (Number)                                                                                                     */
/* mp_CurrentSampleFactor (Number)                                                                                    */
/* mp_n (String)                                                                                                      */
/* mp_Gain (Number)                                                                                                   */
/* mp_p (Number)                                                                                                      */
/* mp_n_max (Number)                                                                                                  */
/* mp_ts_fact (Number)                                                                                                */

/* USERCODE-BEGIN:Convert                                                                                             */
    if (mp_CurrentSampleFactor < 1) { /* Check current sample time validity */
        cp_CurrentSampleFactor = 1;
    }
    else {
        cp_CurrentSampleFactor = mp_CurrentSampleFactor;
    }

    cp_Ls = mp_Ls / (bp_ts * mp_ts_fact * 1000 * cp_CurrentSampleFactor); /* Derivative scaled with sample time and scale mH to Henry*/
    cp_Rs = mp_Rs;

    /* average */
    var n = parseInt(mp_n, 10);
    print(n)
    cp_n = n;

    /* gain */
    cp_V_gain = mp_Gain;

    /* speed2phi */
    // validation
    if (mp_ts_fact < 1) {
        throw new Exception("Sample time factor must not be smaller than one!");
    }
    if (mp_p < 1) {
        throw new Exception("Number of pole pairs must not be smaller than one!");
    }
    var Ki = ((2 * Math.PI) / 60) * mp_p;

    // Zero order hold (zoh):
    // G(s) = Ki/s -> G(z) = Ki*Ts/(z-1)
    cp_b0 = Ki * bp_ts;

/* USERCODE-END:Convert                                                                                               */

/* Implementation Parameter values                                                                                    */
/* cp_Ls                                                                                                              */
/* cp_Rs                                                                                                              */
/* cp_CurrentSampleFactor                                                                                             */
/* cp_n                                                                                                               */
/* cp_V_gain                                                                                                          */
/* cp_b0                                                                                                              */

