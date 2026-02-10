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
var BITS = 32;
	
	if (mp_CurrentSampleFactor < 1) { /* Check current sample time validity */
		cp_CurrentSampleFactor = 1;
	}
	else {
		cp_CurrentSampleFactor = mp_CurrentSampleFactor;
	}

	var my_Ls = mp_Ls*(mp_I0/mp_U0)/1000; /* Scale Ls (scale mH to Henry)*/
	var my_Rs = mp_Rs*(mp_I0/mp_U0); /* Scale Rs*/

	/* Calculate Ls */
	my_Ls = my_Ls/(bp_ts*mp_ts_fact*cp_CurrentSampleFactor); /* Derivative scaled with sample time */
	cp_sfrLs = getQFormat(my_Ls, BITS, true);
	cp_Ls = getQValue(my_Ls, cp_sfrLs, BITS, true);

	/* Calculate Rs */
	cp_sfrRs = getQFormat(my_Rs, BITS, true);
	cp_Rs = getQValue(my_Rs, cp_sfrRs, BITS, true);

	/* average */
	var n = parseInt(mp_n, 10);
	print(n);
	cp_n  = n;
	cp_sfrn = Math.round(Math.log(n)/Math.log(2));
	print(cp_sfrn);
	
	/* gain */
	cp_sfr_gain = getQFormat(mp_Gain, BITS, true);
	cp_V_gain = getQValue(mp_Gain, cp_sfr_gain, BITS, true);
	
	print(cp_sfr_gain)
	print(cp_V_gain)
	
	/* speed2phi */
	if (mp_ts_fact < 1) {
		throw new Exception("Sample time factor must not be smaller than one!");
	}
	if (mp_p < 1) {
		throw new Exception("Number of pole pairs must not be smaller than one!");
	}
	if (mp_n_max <= 0) {
		throw new Exception("n_max must not be negative or zero!");
	}
	var Ki = (mp_n_max / 60) * 2 * mp_p;

	// Zero order hold (zoh):
	// G(s) = Ki/s -> G(z) = Ki*Ts/(z-1)
	var b0 = Ki * bp_ts;
	cp_sfr_b0 = getQFormat(b0, BITS, true);
	cp_b0 = getQValue(b0, cp_sfr_b0, BITS, true);
	
function getQFormat(decValue, maxBits, isSigned) {
	var qFormat, maxExp;
	var maxValue, minValue;

	qFormat = maxBits;

	do {
		qFormat--;

		if (qFormat < 0) {
			throw "Q Format error";
		}

		if (isSigned) {
			maxExp = (-qFormat + maxBits) - 2;
			minValue = -Math.pow(2, maxExp + 1) + Math.pow(2, -qFormat);
			maxValue = Math.pow(2, maxExp + 1) - Math.pow(2, -qFormat);
		} else {
			maxExp = (-qFormat + maxBits) - 1;
			minValue = 0;
			maxValue = Math.pow(2, maxExp + 1) - Math.pow(2, -qFormat);
		}

	} while ((decValue > maxValue) || (decValue < minValue));

	return (qFormat);
}

function getQValue(decValue, qFormat, maxBits, isSigned) {
	var maxExp;
	var maxValue;

	if (isSigned) {
		maxExp = (-qFormat + maxBits) - 2;
		var minValue = -Math.pow(2, maxExp + 1) + Math.pow(2, -qFormat);
		maxValue = Math.pow(2, maxExp + 1) - Math.pow(2, -qFormat);
		if (decValue > maxValue) {
			return (Math.pow(2, maxBits - 1) - 1);
		} else if (decValue < minValue) {
			return (-(Math.pow(2, maxBits - 1) - 1));
		} else {
			return (Math.round(decValue * Math.pow(2, qFormat)));
		}
	} else {
		maxExp = (-qFormat + maxBits) - 1;
		maxValue = Math.pow(2, maxExp + 1) - Math.pow(2, -qFormat);
		if (decValue > maxValue) {
			return (Math.pow(2, maxBits) - 1);
		} else if (decValue < 0) {
			return (0);
		} else {
			return (Math.round(decValue * (Math.pow(2, qFormat + 1))) / 2);
		}
	}
}
/* USERCODE-END:Convert                                                                                               */

/* Implementation Parameter values                                                                                    */
/* cp_Ls                                                                                                              */
/* cp_Rs                                                                                                              */
/* cp_sfrLs                                                                                                           */
/* cp_sfrRs                                                                                                           */
/* cp_CurrentSampleFactor                                                                                             */
/* cp_n                                                                                                               */
/* cp_sfrn                                                                                                            */
/* cp_V_gain                                                                                                          */
/* cp_sfr_gain                                                                                                        */
/* cp_b0                                                                                                              */
/* cp_sfr_b0                                                                                                          */

