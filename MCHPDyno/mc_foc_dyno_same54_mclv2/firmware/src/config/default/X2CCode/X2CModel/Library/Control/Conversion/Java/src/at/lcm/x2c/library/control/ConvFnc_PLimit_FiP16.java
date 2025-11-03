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
import at.lcm.x2c.utils.QFormat;
// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_PLimit_FiP16 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: Kp                                                                                           
        MaskDouble KpMaskVal = 
            (MaskDouble)this.getMaskParameter("Kp").getMaskDataType();
        TNumeric KpMaskData = 
            (TNumeric)KpMaskVal.getData();

        // Implementation Parameter: b1                                                                                 
        TNumeric b1CtrVal = this.getImplementationParameterData("b1");
        // Implementation Parameter: sfrb1                                                                              
        TNumeric sfrb1CtrVal = this.getImplementationParameterData("sfrb1");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
        double Kp;
        double b1;
        final int BITS = 16;

        // get controller parameter values
        Kp = Double.valueOf(KpMaskVal.getValue());

        // Zero order hold (zoh):
        // G(s) = Kp -> G(z) = Kp
        b1 = Kp;

        sfrb1CtrVal.setReal(0, 0, Double.valueOf(QFormat.getQFormat(b1, BITS, true)));
        b1CtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(b1, (int) (sfrb1CtrVal.getReal(0, 0)), BITS, true)));

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("b1", b1CtrVal);
        this.setImplementationParameterData("sfrb1", sfrb1CtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: Kp                                                                                           
        MaskDouble KpMaskVal = 
            (MaskDouble)this.getMaskParameter("Kp").getMaskDataType();
        TNumeric KpMaskData = 
            (TNumeric)KpMaskVal.getData();

        // Implementation Parameter: b1                                                                                 
        TNumeric b1CtrVal = this.getImplementationParameterData("b1");
        // Implementation Parameter: sfrb1                                                                              
        TNumeric sfrb1CtrVal = this.getImplementationParameterData("sfrb1");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
        double Kp, b1, sfr1;
        final int BITS = 16;

        // get controller parameter
        b1 = b1CtrVal.getReal(0, 0);
        sfr1 = sfrb1CtrVal.getReal(0, 0);

        // calculate controller coefficients
        b1 = QFormat.getDecValue((long) b1, (int) sfr1, BITS, true);

        // Zero order hold (zoh):
        Kp = b1;

        // set mask parameter
        KpMaskData.setReal(0, 0, Kp);

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("Kp").setValue(KpMaskData.toFullString());
    }
}
