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

package at.lcm.x2c.library.general;

import at.lcm.x2c.core.structure.*;
import at.lcm.bu21.general.dtypes.*;

// USERCODE-BEGIN:Imports                                                                                               
// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_RateLimiter_Float64 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: Tr                                                                                           
        MaskDouble TrMaskVal = 
            (MaskDouble)this.getMaskParameter("Tr").getMaskDataType();
        TNumeric TrMaskData = 
            (TNumeric)TrMaskVal.getData();
        // Mask Parameter: Tf                                                                                           
        MaskDouble TfMaskVal = 
            (MaskDouble)this.getMaskParameter("Tf").getMaskDataType();
        TNumeric TfMaskData = 
            (TNumeric)TfMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: RateUp                                                                             
        TNumeric RateUpCtrVal = this.getImplementationParameterData("RateUp");
        // Implementation Parameter: RateDown                                                                           
        TNumeric RateDownCtrVal = this.getImplementationParameterData("RateDown");
        // Implementation Parameter: enable_old                                                                         
        TNumeric enable_oldCtrVal = this.getImplementationParameterData("enable_old");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		double Tr, Tf;
		double Ts = 0;
		int ts_fact;

		// get parameter values
		Tr = Double.valueOf(TrMaskVal.getValue());
		Tf = Double.valueOf(TfMaskVal.getValue());
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();

		// validate controller parameters
		if (ts_fact <= 0){
			ts_fact = 1;
		}

		// calculate sample time
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		// check range of rising/falling time
		if (Tr < Ts){
			Tr = Ts;
		}
		if (Tf < Ts){
			Tf = Ts;
		}

		// calculate Q-values
		RateUpCtrVal.setReal(0,0, Ts/Tr);
		RateDownCtrVal.setReal(0,0, Ts/Tf);

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("RateUp", RateUpCtrVal);
        this.setImplementationParameterData("RateDown", RateDownCtrVal);
        this.setImplementationParameterData("enable_old", enable_oldCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: Tr                                                                                           
        MaskDouble TrMaskVal = 
            (MaskDouble)this.getMaskParameter("Tr").getMaskDataType();
        TNumeric TrMaskData = 
            (TNumeric)TrMaskVal.getData();
        // Mask Parameter: Tf                                                                                           
        MaskDouble TfMaskVal = 
            (MaskDouble)this.getMaskParameter("Tf").getMaskDataType();
        TNumeric TfMaskData = 
            (TNumeric)TfMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: RateUp                                                                             
        TNumeric RateUpCtrVal = this.getImplementationParameterData("RateUp");
        // Implementation Parameter: RateDown                                                                           
        TNumeric RateDownCtrVal = this.getImplementationParameterData("RateDown");
        // Implementation Parameter: enable_old                                                                         
        TNumeric enable_oldCtrVal = this.getImplementationParameterData("enable_old");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		double Ts = 0;
		int ts_fact;
		double RateUp, RateDown;

		// get controller parameter
		RateUp   = RateUpCtrVal.getReal(0, 0);
		RateDown = RateDownCtrVal.getReal(0, 0);

		// get sample time information from model (sample time information can't be obtained from target)
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		// set mask parameter
		TrMaskData.setReal(0, 0, Ts/RateUp);
		TfMaskData.setReal(0, 0, Ts/RateDown);

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("Tr").setValue(TrMaskData.toFullString());
        this.getMaskParameter("Tf").setValue(TfMaskData.toFullString());
        this.getMaskParameter("ts_fact").setValue(ts_factMaskData.toFullString());
    }
}
