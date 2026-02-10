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
public class ConvFnc_TF1_Float32 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: b1                                                                                           
        MaskDouble b1MaskVal = 
            (MaskDouble)this.getMaskParameter("b1").getMaskDataType();
        TNumeric b1MaskData = 
            (TNumeric)b1MaskVal.getData();
        // Mask Parameter: b0                                                                                           
        MaskDouble b0MaskVal = 
            (MaskDouble)this.getMaskParameter("b0").getMaskDataType();
        TNumeric b0MaskData = 
            (TNumeric)b0MaskVal.getData();
        // Mask Parameter: a0                                                                                           
        MaskDouble a0MaskVal = 
            (MaskDouble)this.getMaskParameter("a0").getMaskDataType();
        TNumeric a0MaskData = 
            (TNumeric)a0MaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: b0                                                                                 
        TNumeric b0CtrVal = this.getImplementationParameterData("b0");
        // Implementation Parameter: b1                                                                                 
        TNumeric b1CtrVal = this.getImplementationParameterData("b1");
        // Implementation Parameter: a0                                                                                 
        TNumeric a0CtrVal = this.getImplementationParameterData("a0");
        // Implementation Parameter: in_old                                                                             
        TNumeric in_oldCtrVal = this.getImplementationParameterData("in_old");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		// get mask value and set controller parameter
		b0CtrVal.setReal(0, 0, Double.valueOf(b0MaskVal.getValue()));
		b1CtrVal.setReal(0, 0, Double.valueOf(b1MaskVal.getValue()));
		a0CtrVal.setReal(0, 0, Double.valueOf(a0MaskVal.getValue()));

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("b0", b0CtrVal);
        this.setImplementationParameterData("b1", b1CtrVal);
        this.setImplementationParameterData("a0", a0CtrVal);
        this.setImplementationParameterData("in_old", in_oldCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: b1                                                                                           
        MaskDouble b1MaskVal = 
            (MaskDouble)this.getMaskParameter("b1").getMaskDataType();
        TNumeric b1MaskData = 
            (TNumeric)b1MaskVal.getData();
        // Mask Parameter: b0                                                                                           
        MaskDouble b0MaskVal = 
            (MaskDouble)this.getMaskParameter("b0").getMaskDataType();
        TNumeric b0MaskData = 
            (TNumeric)b0MaskVal.getData();
        // Mask Parameter: a0                                                                                           
        MaskDouble a0MaskVal = 
            (MaskDouble)this.getMaskParameter("a0").getMaskDataType();
        TNumeric a0MaskData = 
            (TNumeric)a0MaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: b0                                                                                 
        TNumeric b0CtrVal = this.getImplementationParameterData("b0");
        // Implementation Parameter: b1                                                                                 
        TNumeric b1CtrVal = this.getImplementationParameterData("b1");
        // Implementation Parameter: a0                                                                                 
        TNumeric a0CtrVal = this.getImplementationParameterData("a0");
        // Implementation Parameter: in_old                                                                             
        TNumeric in_oldCtrVal = this.getImplementationParameterData("in_old");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		// get controller parameter and set mask parameter
		b0MaskData.setReal(0, 0, b0CtrVal.getReal(0, 0));
		b1MaskData.setReal(0, 0, b1CtrVal.getReal(0, 0));
		a0MaskData.setReal(0, 0, a0CtrVal.getReal(0, 0));

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("b1").setValue(b1MaskData.toFullString());
        this.getMaskParameter("b0").setValue(b0MaskData.toFullString());
        this.getMaskParameter("a0").setValue(a0MaskData.toFullString());
        this.getMaskParameter("ts_fact").setValue(ts_factMaskData.toFullString());
    }
}
