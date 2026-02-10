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
public class ConvFnc_TDSystemO1_FiP8 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: A                                                                                            
        MaskDouble AMaskVal = 
            (MaskDouble)this.getMaskParameter("A").getMaskDataType();
        TNumeric AMaskData = 
            (TNumeric)AMaskVal.getData();
        // Mask Parameter: B                                                                                            
        MaskDouble BMaskVal = 
            (MaskDouble)this.getMaskParameter("B").getMaskDataType();
        TNumeric BMaskData = 
            (TNumeric)BMaskVal.getData();
        // Mask Parameter: C                                                                                            
        MaskDouble CMaskVal = 
            (MaskDouble)this.getMaskParameter("C").getMaskDataType();
        TNumeric CMaskData = 
            (TNumeric)CMaskVal.getData();
        // Mask Parameter: D                                                                                            
        MaskDouble DMaskVal = 
            (MaskDouble)this.getMaskParameter("D").getMaskDataType();
        TNumeric DMaskData = 
            (TNumeric)DMaskVal.getData();

        // Implementation Parameter: a11                                                                                
        TNumeric a11CtrVal = this.getImplementationParameterData("a11");
        // Implementation Parameter: b11                                                                                
        TNumeric b11CtrVal = this.getImplementationParameterData("b11");
        // Implementation Parameter: c11                                                                                
        TNumeric c11CtrVal = this.getImplementationParameterData("c11");
        // Implementation Parameter: d11                                                                                
        TNumeric d11CtrVal = this.getImplementationParameterData("d11");
        // Implementation Parameter: sfra11                                                                             
        TNumeric sfra11CtrVal = this.getImplementationParameterData("sfra11");
        // Implementation Parameter: sfrb11                                                                             
        TNumeric sfrb11CtrVal = this.getImplementationParameterData("sfrb11");
        // Implementation Parameter: sfrc11                                                                             
        TNumeric sfrc11CtrVal = this.getImplementationParameterData("sfrc11");
        // Implementation Parameter: sfrd11                                                                             
        TNumeric sfrd11CtrVal = this.getImplementationParameterData("sfrd11");
        // Implementation Parameter: x1                                                                                 
        TNumeric x1CtrVal = this.getImplementationParameterData("x1");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		double a11, b11, c11, d11;
		final int BITS = 8;


		// get controller parameter values
		a11 = Double.valueOf(AMaskVal.getValue());
		b11 = Double.valueOf(BMaskVal.getValue());
		c11 = Double.valueOf(CMaskVal.getValue());
		d11 = Double.valueOf(DMaskVal.getValue());


		// set shift factors and Q values
		sfra11CtrVal.setReal(0, 0, QFormat.getQFormat(a11, BITS, true));
		sfrb11CtrVal.setReal(0, 0, QFormat.getQFormat(b11, BITS, true));
		sfrc11CtrVal.setReal(0, 0, QFormat.getQFormat(c11, BITS, true));
		sfrd11CtrVal.setReal(0, 0, QFormat.getQFormat(d11, BITS, true));
		a11CtrVal.setReal(0,0, Double.valueOf(QFormat.getQValue(a11, (int)(sfra11CtrVal.getReal(0,0)), BITS, true)));
		b11CtrVal.setReal(0,0, Double.valueOf(QFormat.getQValue(b11, (int)(sfrb11CtrVal.getReal(0,0)), BITS, true)));
		c11CtrVal.setReal(0,0, Double.valueOf(QFormat.getQValue(c11, (int)(sfrc11CtrVal.getReal(0,0)), BITS, true)));
		d11CtrVal.setReal(0,0, Double.valueOf(QFormat.getQValue(d11, (int)(sfrd11CtrVal.getReal(0,0)), BITS, true)));

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("a11", a11CtrVal);
        this.setImplementationParameterData("b11", b11CtrVal);
        this.setImplementationParameterData("c11", c11CtrVal);
        this.setImplementationParameterData("d11", d11CtrVal);
        this.setImplementationParameterData("sfra11", sfra11CtrVal);
        this.setImplementationParameterData("sfrb11", sfrb11CtrVal);
        this.setImplementationParameterData("sfrc11", sfrc11CtrVal);
        this.setImplementationParameterData("sfrd11", sfrd11CtrVal);
        this.setImplementationParameterData("x1", x1CtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: A                                                                                            
        MaskDouble AMaskVal = 
            (MaskDouble)this.getMaskParameter("A").getMaskDataType();
        TNumeric AMaskData = 
            (TNumeric)AMaskVal.getData();
        // Mask Parameter: B                                                                                            
        MaskDouble BMaskVal = 
            (MaskDouble)this.getMaskParameter("B").getMaskDataType();
        TNumeric BMaskData = 
            (TNumeric)BMaskVal.getData();
        // Mask Parameter: C                                                                                            
        MaskDouble CMaskVal = 
            (MaskDouble)this.getMaskParameter("C").getMaskDataType();
        TNumeric CMaskData = 
            (TNumeric)CMaskVal.getData();
        // Mask Parameter: D                                                                                            
        MaskDouble DMaskVal = 
            (MaskDouble)this.getMaskParameter("D").getMaskDataType();
        TNumeric DMaskData = 
            (TNumeric)DMaskVal.getData();

        // Implementation Parameter: a11                                                                                
        TNumeric a11CtrVal = this.getImplementationParameterData("a11");
        // Implementation Parameter: b11                                                                                
        TNumeric b11CtrVal = this.getImplementationParameterData("b11");
        // Implementation Parameter: c11                                                                                
        TNumeric c11CtrVal = this.getImplementationParameterData("c11");
        // Implementation Parameter: d11                                                                                
        TNumeric d11CtrVal = this.getImplementationParameterData("d11");
        // Implementation Parameter: sfra11                                                                             
        TNumeric sfra11CtrVal = this.getImplementationParameterData("sfra11");
        // Implementation Parameter: sfrb11                                                                             
        TNumeric sfrb11CtrVal = this.getImplementationParameterData("sfrb11");
        // Implementation Parameter: sfrc11                                                                             
        TNumeric sfrc11CtrVal = this.getImplementationParameterData("sfrc11");
        // Implementation Parameter: sfrd11                                                                             
        TNumeric sfrd11CtrVal = this.getImplementationParameterData("sfrd11");
        // Implementation Parameter: x1                                                                                 
        TNumeric x1CtrVal = this.getImplementationParameterData("x1");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		/* get controller values */
		double a11 = a11CtrVal.getReal(0, 0);
		double b11 = b11CtrVal.getReal(0, 0);
		double c11 = c11CtrVal.getReal(0, 0);
		double d11 = d11CtrVal.getReal(0, 0);
		double sfra11 = sfra11CtrVal.getReal(0, 0);
		double sfrb11 = sfrb11CtrVal.getReal(0, 0);
		double sfrc11 = sfrc11CtrVal.getReal(0, 0);
		double sfrd11 = sfrd11CtrVal.getReal(0, 0);

		/* set mask parameter values */
		AMaskData.setReal(0, 0, a11 / Math.pow(2, sfra11));
		BMaskData.setReal(0, 0, b11 / Math.pow(2, sfrb11));
		CMaskData.setReal(0, 0, c11 / Math.pow(2, sfrc11));

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("A").setValue(AMaskData.toFullString());
        this.getMaskParameter("B").setValue(BMaskData.toFullString());
        this.getMaskParameter("C").setValue(CMaskData.toFullString());
        this.getMaskParameter("D").setValue(DMaskData.toFullString());
    }
}
