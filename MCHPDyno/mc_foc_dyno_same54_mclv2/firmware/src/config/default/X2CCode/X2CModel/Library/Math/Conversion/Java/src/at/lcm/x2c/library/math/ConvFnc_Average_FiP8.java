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

package at.lcm.x2c.library.math;

import at.lcm.x2c.core.structure.*;
import at.lcm.bu21.general.dtypes.*;

// USERCODE-BEGIN:Imports                                                                                               
// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_Average_FiP8 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: n                                                                                            
        MaskComboBox nMaskVal = 
            (MaskComboBox)this.getMaskParameter("n").getMaskDataType();
        TString nMaskData = 
            (TString)nMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: n                                                                                  
        TNumeric nCtrVal = this.getImplementationParameterData("n");
        // Implementation Parameter: sfrn                                                                               
        TNumeric sfrnCtrVal = this.getImplementationParameterData("sfrn");
        // Implementation Parameter: sum                                                                                
        TNumeric sumCtrVal = this.getImplementationParameterData("sum");
        // Implementation Parameter: count                                                                              
        TNumeric countCtrVal = this.getImplementationParameterData("count");
        // Implementation Parameter: avg                                                                                
        TNumeric avgCtrVal = this.getImplementationParameterData("avg");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
          int n;
          
          /* get window size for averaging */
          n = Integer.valueOf(nMaskVal.getValue());
          
          /* set window size for average calculation */
          nCtrVal.setReal(0,0, n);
          
          /* calculate and set shift factor for average calculation */
          sfrnCtrVal.setReal(0, 0, Math.round(Math.log(n)/Math.log(2)));	/* rounding shouldn't be necessary, but just to be sure */  
          
// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("n", nCtrVal);
        this.setImplementationParameterData("sfrn", sfrnCtrVal);
        this.setImplementationParameterData("sum", sumCtrVal);
        this.setImplementationParameterData("count", countCtrVal);
        this.setImplementationParameterData("avg", avgCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: n                                                                                            
        MaskComboBox nMaskVal = 
            (MaskComboBox)this.getMaskParameter("n").getMaskDataType();
        TString nMaskData = 
            (TString)nMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();

        // Implementation Parameter: n                                                                                  
        TNumeric nCtrVal = this.getImplementationParameterData("n");
        // Implementation Parameter: sfrn                                                                               
        TNumeric sfrnCtrVal = this.getImplementationParameterData("sfrn");
        // Implementation Parameter: sum                                                                                
        TNumeric sumCtrVal = this.getImplementationParameterData("sum");
        // Implementation Parameter: count                                                                              
        TNumeric countCtrVal = this.getImplementationParameterData("count");
        // Implementation Parameter: avg                                                                                
        TNumeric avgCtrVal = this.getImplementationParameterData("avg");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
          int n;
          
          /* get controller parameter */
          n = (int)nCtrVal.getReal(0, 0);
          
          /* calculate mask parameter */
          nMaskData = new TString(String.valueOf(n));
          
// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("n").setValue(nMaskData.getString());
        this.getMaskParameter("ts_fact").setValue(ts_factMaskData.toFullString());
    }
}
