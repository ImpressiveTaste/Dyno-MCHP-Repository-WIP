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

package at.lcm.x2c.library.control;

import at.lcm.x2c.core.structure.*;
import at.lcm.bu21.general.dtypes.*;

// USERCODE-BEGIN:Imports                                                                                               
import at.lcm.x2c.utils.QFormat;

// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_PT1_FiP16 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: V                                                                                            
        MaskDouble VMaskVal = 
            (MaskDouble)this.getMaskParameter("V").getMaskDataType();
        TNumeric VMaskData = 
            (TNumeric)VMaskVal.getData();
        // Mask Parameter: fc                                                                                           
        MaskDouble fcMaskVal = 
            (MaskDouble)this.getMaskParameter("fc").getMaskDataType();
        TNumeric fcMaskData = 
            (TNumeric)fcMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();
        // Mask Parameter: method                                                                                       
        MaskComboBox methodMaskVal = 
            (MaskComboBox)this.getMaskParameter("method").getMaskDataType();
        TString methodMaskData = 
            (TString)methodMaskVal.getData();

        // Implementation Parameter: b0                                                                                 
        TNumeric b0CtrVal = this.getImplementationParameterData("b0");
        // Implementation Parameter: b1                                                                                 
        TNumeric b1CtrVal = this.getImplementationParameterData("b1");
        // Implementation Parameter: a0                                                                                 
        TNumeric a0CtrVal = this.getImplementationParameterData("a0");
        // Implementation Parameter: sfrb                                                                               
        TNumeric sfrbCtrVal = this.getImplementationParameterData("sfrb");
        // Implementation Parameter: sfra                                                                               
        TNumeric sfraCtrVal = this.getImplementationParameterData("sfra");
        // Implementation Parameter: in_old                                                                             
        TNumeric in_oldCtrVal = this.getImplementationParameterData("in_old");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		double V, fc, wc, x;
		double Ts = 0;
		int ts_fact;
		double b0 = 0, b1 = 0, a0 = 0;
		final int BITS = 16;

		// get controller parameter values
		V = Double.valueOf(VMaskVal.getValue());
		fc = Double.valueOf(fcMaskVal.getValue());
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();

		// validate controller parameters
		fc = Math.abs(fc);
		if (ts_fact <= 0) {
			ts_fact = 1;
		}

		// calculate temporary variables
		wc = 2 * Math.PI * fc;
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		// calculate coefficients
		if (methodMaskVal.getValue().equals("zoh")) {
			// Zero order hold (zoh):
			// G(s) = V/(s/w + 1) -> G(z) = V*(exp(w*T)-1)/(z*exp(w*T)-1)
			a0 = -Math.exp(-wc * Ts);
			b0 = (1 + a0) * V;
			b1 = 0;
		} else if (methodMaskVal.getValue().equals("tustin")) {
			// Bilinear approximation (tustin):
			// G(s) = V*s/(s/w + 1) -> G(z) = V*w*T*(z+1)/(2z-2+w*T*z+w*T)
			x = wc * Ts;
			a0 = (x - 2) / (x + 2);
			b0 = (V * x) / (x + 2);
			b1 = b0;
		} else {
			throw new Exception("PT1-Block conversion function error: unknown discretization method!");
		}

		sfraCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQFormat(a0, BITS, true)));
		sfrbCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQFormat(b0, BITS, true)));
		a0CtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(a0, (int) (sfraCtrVal.getReal(0, 0)), BITS, true)));
		b0CtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(b0, (int) (sfrbCtrVal.getReal(0, 0)), BITS, true)));
		b1CtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(b1, (int) (sfrbCtrVal.getReal(0, 0)), BITS, true)));

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("b0", b0CtrVal);
        this.setImplementationParameterData("b1", b1CtrVal);
        this.setImplementationParameterData("a0", a0CtrVal);
        this.setImplementationParameterData("sfrb", sfrbCtrVal);
        this.setImplementationParameterData("sfra", sfraCtrVal);
        this.setImplementationParameterData("in_old", in_oldCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: V                                                                                            
        MaskDouble VMaskVal = 
            (MaskDouble)this.getMaskParameter("V").getMaskDataType();
        TNumeric VMaskData = 
            (TNumeric)VMaskVal.getData();
        // Mask Parameter: fc                                                                                           
        MaskDouble fcMaskVal = 
            (MaskDouble)this.getMaskParameter("fc").getMaskDataType();
        TNumeric fcMaskData = 
            (TNumeric)fcMaskVal.getData();
        // Mask Parameter: ts_fact                                                                                      
        MaskDouble ts_factMaskVal = 
            (MaskDouble)this.getMaskParameter("ts_fact").getMaskDataType();
        TNumeric ts_factMaskData = 
            (TNumeric)ts_factMaskVal.getData();
        // Mask Parameter: method                                                                                       
        MaskComboBox methodMaskVal = 
            (MaskComboBox)this.getMaskParameter("method").getMaskDataType();
        TString methodMaskData = 
            (TString)methodMaskVal.getData();

        // Implementation Parameter: b0                                                                                 
        TNumeric b0CtrVal = this.getImplementationParameterData("b0");
        // Implementation Parameter: b1                                                                                 
        TNumeric b1CtrVal = this.getImplementationParameterData("b1");
        // Implementation Parameter: a0                                                                                 
        TNumeric a0CtrVal = this.getImplementationParameterData("a0");
        // Implementation Parameter: sfrb                                                                               
        TNumeric sfrbCtrVal = this.getImplementationParameterData("sfrb");
        // Implementation Parameter: sfra                                                                               
        TNumeric sfraCtrVal = this.getImplementationParameterData("sfra");
        // Implementation Parameter: in_old                                                                             
        TNumeric in_oldCtrVal = this.getImplementationParameterData("in_old");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		double V = 0, fc = 0, wc, x;
		double Ts = 0;
		int ts_fact;
		double b0 = 0, b1 = 0, a0 = 0, sfrb, sfra;
		final int BITS = 16;

		// get controller parameter
		b0 = b0CtrVal.getReal(0, 0);
		b1 = b1CtrVal.getReal(0, 0);
		a0 = a0CtrVal.getReal(0, 0);
		sfrb = sfrbCtrVal.getReal(0, 0);
		sfra = sfraCtrVal.getReal(0, 0);

		// calculate controller coefficients
		b0 = QFormat.getDecValue((long) b0, (int) sfrb, BITS, true);
		b1 = QFormat.getDecValue((long) b1, (int) sfrb, BITS, true);
		a0 = QFormat.getDecValue((long) a0, (int) sfrb, BITS, true);

		// get sample time information from model (sample time information can't be obtained from target)
		ts_fact = Double.valueOf(ts_factMaskVal.getValue()).intValue();
		Ts = ts_fact * this.getDedicatedBlock().getModel().getSampleTime();

		// calculate mask parameter
		if (b1 != b0) {
			// zoh:
			methodMaskData = new TString("zoh");

			wc = -Math.log(-a0) / Ts;
			V = b0 / (1 + a0);
		} else {
			// tustin:
			methodMaskData = new TString("tustin");

			x = (2 * (1 + a0)) / (1 - a0);
			wc = x / Ts;
			V = (b0 * (x + 2)) / x;

		}
		fc = wc / (2 * Math.PI);

		// set (remaining) mask parameter
		VMaskData.setReal(0, 0, V);
		fcMaskData.setReal(0, 0, fc);

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("V").setValue(VMaskData.toFullString());
        this.getMaskParameter("fc").setValue(fcMaskData.toFullString());
        this.getMaskParameter("ts_fact").setValue(ts_factMaskData.toFullString());
        this.getMaskParameter("method").setValue(methodMaskData.getString());
    }
}
