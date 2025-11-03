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
 * This file is part of X2C. http://www.mechatronic-simulation.org/
 * $LastChangedRevision: 2535 $
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
/* mp_Ls (Number)                                                                                                     */
/* mp_Rs (Number)                                                                                                     */
/* mp_U0 (Number)                                                                                                     */
/* mp_I0 (Number)                                                                                                     */
/* mp_ts_fact (Number)                                                                                                */
/* mp_CurrentSampleFactor (Number)                                                                                    */

/* USERCODE-BEGIN:Convert                                                                                             */
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
cp_sfrLs = QFormat.getQFormat(my_Ls, 16, true);
cp_Ls = QFormat.getQValue(my_Ls, cp_sfrLs, 16, true);

/* Calculate Rs */
cp_sfrRs = QFormat.getQFormat(my_Rs, 16, true);
cp_Rs = QFormat.getQValue(my_Rs, cp_sfrRs, 16, true);

/* USERCODE-END:Convert                                                                                               */

/* Implementation Parameter values                                                                                    */
/* cp_Ls                                                                                                              */
/* cp_Rs                                                                                                              */
/* cp_sfrLs                                                                                                           */
/* cp_sfrRs                                                                                                           */
/* cp_CurrentSampleFactor                                                                                             */

