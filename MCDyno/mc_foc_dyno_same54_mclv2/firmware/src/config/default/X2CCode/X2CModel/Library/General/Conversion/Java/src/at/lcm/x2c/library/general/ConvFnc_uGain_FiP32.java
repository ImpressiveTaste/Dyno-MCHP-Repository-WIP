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
//*     Description:                                                           *
// USERCODE-END:Description                                                                                             

package at.lcm.x2c.library.general;

import at.lcm.x2c.core.structure.*;
import at.lcm.bu21.general.dtypes.*;

// USERCODE-BEGIN:Imports                                                                                               
import at.lcm.x2c.utils.QFormat;
// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_uGain_FiP32 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: Gain                                                                                         
        MaskDouble GainMaskVal = 
            (MaskDouble)this.getMaskParameter("Gain").getMaskDataType();
        TNumeric GainMaskData = 
            (TNumeric)GainMaskVal.getData();

        // Implementation Parameter: V                                                                                  
        TNumeric VCtrVal = this.getImplementationParameterData("V");
        // Implementation Parameter: sfr                                                                                
        TNumeric sfrCtrVal = this.getImplementationParameterData("sfr");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
        double V;
        final int BITS = 32;

        // get mask parameter value
        V = Double.valueOf(GainMaskVal.getValue());

        // calculate shift factor and Q-value
        sfrCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQFormat(V, BITS, true)));
        VCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(V, (int) (sfrCtrVal.getReal(0, 0)), BITS, true)));// TODO: change to asymmetric
                                                                                                                 // QFormat!

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("V", VCtrVal);
        this.setImplementationParameterData("sfr", sfrCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: Gain                                                                                         
        MaskDouble GainMaskVal = 
            (MaskDouble)this.getMaskParameter("Gain").getMaskDataType();
        TNumeric GainMaskData = 
            (TNumeric)GainMaskVal.getData();

        // Implementation Parameter: V                                                                                  
        TNumeric VCtrVal = this.getImplementationParameterData("V");
        // Implementation Parameter: sfr                                                                                
        TNumeric sfrCtrVal = this.getImplementationParameterData("sfr");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
        double V, sfr;
        final int BITS = 32;

        // get controller parameter
        V = VCtrVal.getReal(0, 0);
        sfr = sfrCtrVal.getReal(0, 0);

        // calculate mask parameter
        GainMaskData.setReal(0, 0, QFormat.getDecValue((long)V, (int)sfr, BITS, true));

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("Gain").setValue(GainMaskData.toFullString());
    }
}
