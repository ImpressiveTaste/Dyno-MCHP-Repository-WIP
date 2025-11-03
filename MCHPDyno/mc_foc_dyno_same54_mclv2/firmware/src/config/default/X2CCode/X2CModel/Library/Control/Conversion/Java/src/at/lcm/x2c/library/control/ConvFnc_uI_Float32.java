/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM), web: www.lcm.at
 * All rights reserved.
 */
/*
 * This file is licensed according to the BSD 3-clause license as follows:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the "Linz Center of Mechatronics GmbH" and "LCM" nor
 *       the names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL "Linz Center of Mechatronics GmbH" BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * This file is part of X2C. web: x2c.lcm.at
 * $LastChangedRevision: 3474 $
 * $LastChangedDate:: 2024-11-06 18:00:53 +0100#$
 */
// USERCODE-BEGIN:Description                                                                                           
/* Description: */
// USERCODE-END:Description                                                                                             

package at.lcm.x2c.library.control;

import at.lcm.x2c.core.structure.*;
import at.lcm.bu21.general.dtypes.*;

// USERCODE-BEGIN:Imports                                                                                               
// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_uI_Float32 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: Ki                                                                                           
        MaskDouble KiMaskVal = 
            (MaskDouble)this.getMaskParameter("Ki").getMaskDataType();
        TNumeric KiMaskData = 
            (TNumeric)KiMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: b0                                                                                 
        TNumeric b0CtrVal = this.getImplementationParameterData("b0");
        // Implementation Parameter: i_old                                                                              
        TNumeric i_oldCtrVal = this.getImplementationParameterData("i_old");
        // Implementation Parameter: enable_old                                                                         
        TNumeric enable_oldCtrVal = this.getImplementationParameterData("enable_old");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		double Ts = 0;
		int ts_fact;
		double Ki, b0;

		// get controller parameter values
		Ki = Double.valueOf(KiMaskVal.getValue());
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();

		// validate controller parameters
		if (ts_fact <= 0) {
			ts_fact = 1;
		}

		// calculate temporary variables
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		// Zero order hold (zoh):
		// G(s) = Ki/s -> G(z) = Ki*Ts/(z-1)
		b0 = Ki * Ts;

		b0CtrVal.setReal(0, 0, b0);

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("b0", b0CtrVal);
        this.setImplementationParameterData("i_old", i_oldCtrVal);
        this.setImplementationParameterData("enable_old", enable_oldCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: Ki                                                                                           
        MaskDouble KiMaskVal = 
            (MaskDouble)this.getMaskParameter("Ki").getMaskDataType();
        TNumeric KiMaskData = 
            (TNumeric)KiMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: b0                                                                                 
        TNumeric b0CtrVal = this.getImplementationParameterData("b0");
        // Implementation Parameter: i_old                                                                              
        TNumeric i_oldCtrVal = this.getImplementationParameterData("i_old");
        // Implementation Parameter: enable_old                                                                         
        TNumeric enable_oldCtrVal = this.getImplementationParameterData("enable_old");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		double Ts = 0;
		int ts_fact;
		double Ki, b0;

		// get controller parameter
		b0 = b0CtrVal.getReal(0, 0);

		// get sample time information from model (sample time information can't be obtained from target)
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		// Zero order hold (zoh):
		Ki = b0 / Ts;

		// set mask parameter
		KiMaskData.setReal(0, 0, Ki);

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("Ki").setValue(KiMaskData.toFullString());
        this.getMaskParameter("ts_fact").setValue(ts_factMaskData.toFullString());
    }
}
