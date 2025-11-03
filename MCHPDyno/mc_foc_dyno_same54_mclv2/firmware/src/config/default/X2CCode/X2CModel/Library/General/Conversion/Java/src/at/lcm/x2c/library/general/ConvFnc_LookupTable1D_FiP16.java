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
import at.lcm.x2c.utils.QFormat;
// USERCODE-END:Imports                                                                                                 

@SuppressWarnings("unused")
public class ConvFnc_LookupTable1D_FiP16 extends JavaConversionFunction {
    private static final long serialVersionUID = 1L;

    @Override
    public void convertMaskToImplementation() throws Exception {

        // Mask Parameter: TableData                                                                                    
        MaskDouble TableDataMaskVal = 
            (MaskDouble)this.getMaskParameter("TableData").getMaskDataType();
        TNumeric TableDataMaskData = 
            (TNumeric)TableDataMaskVal.getData();
        // Mask Parameter: DimX                                                                                         
        MaskComboBox DimXMaskVal = 
            (MaskComboBox)this.getMaskParameter("DimX").getMaskDataType();
        TString DimXMaskData = 
            (TString)DimXMaskVal.getData();
        // Mask Parameter: x_min                                                                                        
        MaskDouble x_minMaskVal = 
            (MaskDouble)this.getMaskParameter("x_min").getMaskDataType();
        TNumeric x_minMaskData = 
            (TNumeric)x_minMaskVal.getData();
        // Mask Parameter: x_max                                                                                        
        MaskDouble x_maxMaskVal = 
            (MaskDouble)this.getMaskParameter("x_max").getMaskDataType();
        TNumeric x_maxMaskData = 
            (TNumeric)x_maxMaskVal.getData();

        // Implementation Parameter: Table                                                                              
        TNumeric TableCtrVal = this.getImplementationParameterData("Table");
        // Implementation Parameter: sfrX                                                                               
        TNumeric sfrXCtrVal = this.getImplementationParameterData("sfrX");
        // Implementation Parameter: maskX                                                                              
        TNumeric maskXCtrVal = this.getImplementationParameterData("maskX");
        // Implementation Parameter: idxOffsetX                                                                         
        TNumeric idxOffsetXCtrVal = this.getImplementationParameterData("idxOffsetX");
        // Implementation Parameter: gainX                                                                              
        TNumeric gainXCtrVal = this.getImplementationParameterData("gainX");
        // Implementation Parameter: gainXsfr                                                                           
        TNumeric gainXsfrCtrVal = this.getImplementationParameterData("gainXsfr");
        // Implementation Parameter: offsetX                                                                            
        TNumeric offsetXCtrVal = this.getImplementationParameterData("offsetX");
        // Implementation Parameter: minX                                                                               
        TNumeric minXCtrVal = this.getImplementationParameterData("minX");
        // Implementation Parameter: maxX                                                                               
        TNumeric maxXCtrVal = this.getImplementationParameterData("maxX");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
        int size_x;
        final int SIZE_ROW = 1;
        final int BITS = 16;
        double sfrX;
        double x_min, x_max, offsetX, gainX;

        /* get table size */
        size_x = Integer.valueOf(DimXMaskVal.getValue());

        /* check table data for correct size */
        if ((TableDataMaskData.getNumColumns() != size_x) || (TableDataMaskData.getNumRows() != SIZE_ROW)) {
            throw new Exception("Lookup Table Size mismatch in column: expected=(" + SIZE_ROW + ", " + size_x + ") "
                    + "got=(" + TableDataMaskData.getNumRows() + "," + TableDataMaskData.getNumColumns() + ")");
        }

        /* convert input scaling */
        x_min = Double.valueOf(x_minMaskVal.getValue());
        x_max = Double.valueOf(x_maxMaskVal.getValue());
        if (x_min >= x_max) {
            throw new Exception("Scaling of x-dimension not possible: x_max must be greater than x_min!)");
        }
        offsetX = x_min + ((x_max - x_min) / 2);
        gainX = 2 / (x_max - x_min);
        offsetXCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(offsetX, BITS - 1, BITS, true)));
        gainXsfrCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQFormat(gainX, BITS, true)));
        gainXCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(gainX, (int) (gainXsfrCtrVal.getReal(0, 0)), BITS, true)));

        /* convert boundaries */
        minXCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(x_min, BITS - 1, BITS, true)));
        maxXCtrVal.setReal(0, 0, Double.valueOf(QFormat.getQValue(x_max, BITS - 1, BITS, true)));

        /* convert FiP scaling */
        sfrX = Double.valueOf(QFormat.getQFormat(size_x - 1, BITS + 2, true));
        sfrXCtrVal.setReal(0, 0, sfrX);
        maskXCtrVal.setReal(0, 0, Math.pow(2, sfrX) - 1);
        idxOffsetXCtrVal.setReal(0, 0, Math.pow(2, BITS - 1 - sfrX));

        /* convert table data */
        Double[] tmpData = new Double[size_x];
        for (int i = 0; i < size_x; i++) {
            tmpData[i] = Double.valueOf(QFormat.getQValue(TableDataMaskData.getReal(0, i), BITS - 1, BITS, true));
        }

        TableCtrVal = new TNumeric(tmpData);

// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("Table", TableCtrVal);
        this.setImplementationParameterData("sfrX", sfrXCtrVal);
        this.setImplementationParameterData("maskX", maskXCtrVal);
        this.setImplementationParameterData("idxOffsetX", idxOffsetXCtrVal);
        this.setImplementationParameterData("gainX", gainXCtrVal);
        this.setImplementationParameterData("gainXsfr", gainXsfrCtrVal);
        this.setImplementationParameterData("offsetX", offsetXCtrVal);
        this.setImplementationParameterData("minX", minXCtrVal);
        this.setImplementationParameterData("maxX", maxXCtrVal);
    }

    @Override
    public void convertImplementationToMask() throws Exception {

        // Mask Parameter: TableData                                                                                    
        MaskDouble TableDataMaskVal = 
            (MaskDouble)this.getMaskParameter("TableData").getMaskDataType();
        TNumeric TableDataMaskData = 
            (TNumeric)TableDataMaskVal.getData();
        // Mask Parameter: DimX                                                                                         
        MaskComboBox DimXMaskVal = 
            (MaskComboBox)this.getMaskParameter("DimX").getMaskDataType();
        TString DimXMaskData = 
            (TString)DimXMaskVal.getData();
        // Mask Parameter: x_min                                                                                        
        MaskDouble x_minMaskVal = 
            (MaskDouble)this.getMaskParameter("x_min").getMaskDataType();
        TNumeric x_minMaskData = 
            (TNumeric)x_minMaskVal.getData();
        // Mask Parameter: x_max                                                                                        
        MaskDouble x_maxMaskVal = 
            (MaskDouble)this.getMaskParameter("x_max").getMaskDataType();
        TNumeric x_maxMaskData = 
            (TNumeric)x_maxMaskVal.getData();

        // Implementation Parameter: Table                                                                              
        TNumeric TableCtrVal = this.getImplementationParameterData("Table");
        // Implementation Parameter: sfrX                                                                               
        TNumeric sfrXCtrVal = this.getImplementationParameterData("sfrX");
        // Implementation Parameter: maskX                                                                              
        TNumeric maskXCtrVal = this.getImplementationParameterData("maskX");
        // Implementation Parameter: idxOffsetX                                                                         
        TNumeric idxOffsetXCtrVal = this.getImplementationParameterData("idxOffsetX");
        // Implementation Parameter: gainX                                                                              
        TNumeric gainXCtrVal = this.getImplementationParameterData("gainX");
        // Implementation Parameter: gainXsfr                                                                           
        TNumeric gainXsfrCtrVal = this.getImplementationParameterData("gainXsfr");
        // Implementation Parameter: offsetX                                                                            
        TNumeric offsetXCtrVal = this.getImplementationParameterData("offsetX");
        // Implementation Parameter: minX                                                                               
        TNumeric minXCtrVal = this.getImplementationParameterData("minX");
        // Implementation Parameter: maxX                                                                               
        TNumeric maxXCtrVal = this.getImplementationParameterData("maxX");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
        /* TODO Add Implementation- to Mask conversion */
// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("TableData").setValue(TableDataMaskData.toFullString());
        this.getMaskParameter("DimX").setValue(DimXMaskData.getString());
        this.getMaskParameter("x_min").setValue(x_minMaskData.toFullString());
        this.getMaskParameter("x_max").setValue(x_maxMaskData.toFullString());
    }
}
