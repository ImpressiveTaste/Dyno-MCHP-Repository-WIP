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
public class ConvFnc_TDSystemO2_Float64 extends JavaConversionFunction {
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

        // Implementation Parameter: a11                                                                                
        TNumeric a11CtrVal = this.getImplementationParameterData("a11");
        // Implementation Parameter: a12                                                                                
        TNumeric a12CtrVal = this.getImplementationParameterData("a12");
        // Implementation Parameter: a21                                                                                
        TNumeric a21CtrVal = this.getImplementationParameterData("a21");
        // Implementation Parameter: a22                                                                                
        TNumeric a22CtrVal = this.getImplementationParameterData("a22");
        // Implementation Parameter: b11                                                                                
        TNumeric b11CtrVal = this.getImplementationParameterData("b11");
        // Implementation Parameter: b12                                                                                
        TNumeric b12CtrVal = this.getImplementationParameterData("b12");
        // Implementation Parameter: b21                                                                                
        TNumeric b21CtrVal = this.getImplementationParameterData("b21");
        // Implementation Parameter: b22                                                                                
        TNumeric b22CtrVal = this.getImplementationParameterData("b22");
        // Implementation Parameter: x1                                                                                 
        TNumeric x1CtrVal = this.getImplementationParameterData("x1");
        // Implementation Parameter: x2                                                                                 
        TNumeric x2CtrVal = this.getImplementationParameterData("x2");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
		double[] a = new double[4];
		double[] b = new double[4];

		/* check for correct dimension of matrices */
		if (AMaskData.getNumColumns() != 4) {
			throw new Exception("Matrix A size mismatch");
		}
		if (BMaskData.getNumColumns() != 4) {
			throw new Exception("Matrix B size mismatch");
		}

		/* get controller parameter values */
		for (int i = 0; i < 4; i++) {
			a[i] = AMaskData.getReal(0, i);
			b[i] = BMaskData.getReal(0, i);
		}

		/* set mask parameter values */
		a11CtrVal.setReal(0, 0, a[0]);
		a12CtrVal.setReal(0, 0, a[1]);
		a21CtrVal.setReal(0, 0, a[2]);
		a22CtrVal.setReal(0, 0, a[3]);

		b11CtrVal.setReal(0, 0, b[0]);
		b12CtrVal.setReal(0, 0, b[1]);
		b21CtrVal.setReal(0, 0, b[2]);
		b22CtrVal.setReal(0, 0, b[3]);

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("a11", a11CtrVal);
        this.setImplementationParameterData("a12", a12CtrVal);
        this.setImplementationParameterData("a21", a21CtrVal);
        this.setImplementationParameterData("a22", a22CtrVal);
        this.setImplementationParameterData("b11", b11CtrVal);
        this.setImplementationParameterData("b12", b12CtrVal);
        this.setImplementationParameterData("b21", b21CtrVal);
        this.setImplementationParameterData("b22", b22CtrVal);
        this.setImplementationParameterData("x1", x1CtrVal);
        this.setImplementationParameterData("x2", x2CtrVal);
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

        // Implementation Parameter: a11                                                                                
        TNumeric a11CtrVal = this.getImplementationParameterData("a11");
        // Implementation Parameter: a12                                                                                
        TNumeric a12CtrVal = this.getImplementationParameterData("a12");
        // Implementation Parameter: a21                                                                                
        TNumeric a21CtrVal = this.getImplementationParameterData("a21");
        // Implementation Parameter: a22                                                                                
        TNumeric a22CtrVal = this.getImplementationParameterData("a22");
        // Implementation Parameter: b11                                                                                
        TNumeric b11CtrVal = this.getImplementationParameterData("b11");
        // Implementation Parameter: b12                                                                                
        TNumeric b12CtrVal = this.getImplementationParameterData("b12");
        // Implementation Parameter: b21                                                                                
        TNumeric b21CtrVal = this.getImplementationParameterData("b21");
        // Implementation Parameter: b22                                                                                
        TNumeric b22CtrVal = this.getImplementationParameterData("b22");
        // Implementation Parameter: x1                                                                                 
        TNumeric x1CtrVal = this.getImplementationParameterData("x1");
        // Implementation Parameter: x2                                                                                 
        TNumeric x2CtrVal = this.getImplementationParameterData("x2");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
		double a[] = new double[4];
		double b[] = new double[4];

		/* get controller parameter values */
		a[0] = a11CtrVal.getReal(0, 0);
		a[1] = a12CtrVal.getReal(0, 0);
		a[2] = a21CtrVal.getReal(0, 0);
		a[3] = a22CtrVal.getReal(0, 0);

		b[0] = b11CtrVal.getReal(0, 0);
		b[1] = b12CtrVal.getReal(0, 0);
		b[2] = b21CtrVal.getReal(0, 0);
		b[3] = b22CtrVal.getReal(0, 0);

		/* set mask parameter values */
		for (int i = 0; i < 4; i++) {
			AMaskData.setReal(0, i, a[i]);
			BMaskData.setReal(0, i, b[i]);
		}

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("A").setValue(AMaskData.toFullString());
        this.getMaskParameter("B").setValue(BMaskData.toFullString());
    }
}
