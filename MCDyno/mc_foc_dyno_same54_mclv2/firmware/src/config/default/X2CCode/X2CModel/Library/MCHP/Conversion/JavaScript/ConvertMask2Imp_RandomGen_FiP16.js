/*
 * Copyright (c) {YEAR}, {ORGANIZATION_NAME}, {ORGANIZATION_WEB}
 * All rights reserved.
 */
/*
 * This file is licensed according to the BSD 3-clause license as follows:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the "{ORGANIZATION_NAME}" nor
 *     the names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL "{ORGANIZATION_NAME}" BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 2535 $
 * $LastChangedDate:: 2022-03-21 20:28:10 +0100#$
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: */
/* USERCODE-END:Description                                                                                           */

/* USERCODE-BEGIN:ExternalModules                                                                                     */
try {// if you want the script run both on rhino and nashorn
    load("nashorn:mozilla_compat.js");
} catch (e) {}
importPackage(Packages.at.lcm.x2c.utils);
/* USERCODE-END:ExternalModules                                                                                       */

/* Block sample time                                                                                                  */
/* bp_ts                                                                                                              */

/* Mask parameter values                                                                                              */
/* mp_A (Number)                                                                                                      */
/* mp_ts_fact (Number)                                                                                                */
/* mp_D (Number)                                                                                                      */

/* USERCODE-BEGIN:Convert                                                                                             */
if (mp_A < 0)
	mp_A = -mp_A;
if (mp_A > 2)
	mp_A = 2;
cp_noise_amplitude = QFormat.getQValue(mp_A, 16, 16, false);
cp_noise_amplitude_half = QFormat.getQValue(mp_A, 15, 16, true);
if (mp_D < 1.0)
	mp_D = 1.0;
else if (mp_D > 65535)
	mp_D = 65535;
	
cp_sample_time_divisor = mp_D;
cp_sample_counter = 0;
cp_random_num = 0;
/* USERCODE-END:Convert                                                                                               */

/* Implementation Parameter values                                                                                    */
/* cp_noise_amplitude                                                                                                 */
/* cp_noise_amplitude_half                                                                                            */
/* cp_sample_time_divisor                                                                                             */
/* cp_sample_counter                                                                                                  */
/* cp_random_num                                                                                                      */

