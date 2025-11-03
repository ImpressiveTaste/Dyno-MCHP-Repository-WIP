/*
 * Copyright (c) 2017, Linz Center of Mechatronics GmbH (LCM), web: www.lcm.at
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
public class ConvFnc_Constant_Float32 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: Value                                                                                        
        MaskDouble ValueMaskVal = 
            (MaskDouble)this.getMaskParameter("Value").getMaskDataType();
        TNumeric ValueMaskData = 
            (TNumeric)ValueMaskVal.getData();

        // Implementation Parameter: K                                                                                  
        TNumeric KCtrVal = this.getImplementationParameterData("K");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
          double K;
          
          // get mask parameter value
          K = Double.valueOf(ValueMaskVal.getValue());
          
          // set controller parameter value
     	  KCtrVal.setReal(0, 0, K);  
     	  
// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("K", KCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: Value                                                                                        
        MaskDouble ValueMaskVal = 
            (MaskDouble)this.getMaskParameter("Value").getMaskDataType();
        TNumeric ValueMaskData = 
            (TNumeric)ValueMaskVal.getData();

        // Implementation Parameter: K                                                                                  
        TNumeric KCtrVal = this.getImplementationParameterData("K");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
          double K;
      	 
 		 // get controller parameter
          K = KCtrVal.getReal(0, 0);
          
          // calculate mask parameter
          ValueMaskData.setReal(0, 0, K); 
          
// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("Value").setValue(ValueMaskData.toFullString());
    }
}
