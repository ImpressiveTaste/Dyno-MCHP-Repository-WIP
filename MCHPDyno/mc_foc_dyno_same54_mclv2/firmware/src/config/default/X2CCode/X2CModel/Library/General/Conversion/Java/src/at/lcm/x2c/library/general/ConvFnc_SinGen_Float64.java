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
public class ConvFnc_SinGen_Float64 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: fmax                                                                                         
        MaskDouble fmaxMaskVal = 
            (MaskDouble)this.getMaskParameter("fmax").getMaskDataType();
        TNumeric fmaxMaskData = 
            (TNumeric)fmaxMaskVal.getData();
        // Mask Parameter: Offset                                                                                       
        MaskDouble OffsetMaskVal = 
            (MaskDouble)this.getMaskParameter("Offset").getMaskDataType();
        TNumeric OffsetMaskData = 
            (TNumeric)OffsetMaskVal.getData();
        // Mask Parameter: Phase                                                                                        
        MaskDouble PhaseMaskVal = 
            (MaskDouble)this.getMaskParameter("Phase").getMaskDataType();
        TNumeric PhaseMaskData = 
            (TNumeric)PhaseMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: delta_phi                                                                          
        TNumeric delta_phiCtrVal = this.getImplementationParameterData("delta_phi");
        // Implementation Parameter: phase                                                                              
        TNumeric phaseCtrVal = this.getImplementationParameterData("phase");
        // Implementation Parameter: offset                                                                             
        TNumeric offsetCtrVal = this.getImplementationParameterData("offset");
        // Implementation Parameter: phi                                                                                
        TNumeric phiCtrVal = this.getImplementationParameterData("phi");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		double Offset, Phase;
		int ts_fact;
		double Ts = 0;

		// get controller parameter values
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();
		Offset = Double.valueOf(OffsetMaskVal.getValue());
		Phase = Double.valueOf(PhaseMaskVal.getValue());

		// validate controller parameters
		if (ts_fact <= 0) {
			ts_fact = 1;
		}

		// sample time
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		offsetCtrVal.setReal(0, 0, Offset);
		phaseCtrVal.setReal(0, 0, Phase);
		delta_phiCtrVal.setReal(0, 0, 2 * Math.PI * Ts);

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("delta_phi", delta_phiCtrVal);
        this.setImplementationParameterData("phase", phaseCtrVal);
        this.setImplementationParameterData("offset", offsetCtrVal);
        this.setImplementationParameterData("phi", phiCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: fmax                                                                                         
        MaskDouble fmaxMaskVal = 
            (MaskDouble)this.getMaskParameter("fmax").getMaskDataType();
        TNumeric fmaxMaskData = 
            (TNumeric)fmaxMaskVal.getData();
        // Mask Parameter: Offset                                                                                       
        MaskDouble OffsetMaskVal = 
            (MaskDouble)this.getMaskParameter("Offset").getMaskDataType();
        TNumeric OffsetMaskData = 
            (TNumeric)OffsetMaskVal.getData();
        // Mask Parameter: Phase                                                                                        
        MaskDouble PhaseMaskVal = 
            (MaskDouble)this.getMaskParameter("Phase").getMaskDataType();
        TNumeric PhaseMaskData = 
            (TNumeric)PhaseMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: delta_phi                                                                          
        TNumeric delta_phiCtrVal = this.getImplementationParameterData("delta_phi");
        // Implementation Parameter: phase                                                                              
        TNumeric phaseCtrVal = this.getImplementationParameterData("phase");
        // Implementation Parameter: offset                                                                             
        TNumeric offsetCtrVal = this.getImplementationParameterData("offset");
        // Implementation Parameter: phi                                                                                
        TNumeric phiCtrVal = this.getImplementationParameterData("phi");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		double Phase, Offset;

		// get controller parameter
		Phase = phaseCtrVal.getReal(0, 0);
		Offset = offsetCtrVal.getReal(0, 0);

		// set mask parameter
		OffsetMaskData.setReal(0, 0, Offset);
		PhaseMaskData.setReal(0, 0, Phase);

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("fmax").setValue(fmaxMaskData.toFullString());
        this.getMaskParameter("Offset").setValue(OffsetMaskData.toFullString());
        this.getMaskParameter("Phase").setValue(PhaseMaskData.toFullString());
        this.getMaskParameter("ts_fact").setValue(ts_factMaskData.toFullString());
    }
}
