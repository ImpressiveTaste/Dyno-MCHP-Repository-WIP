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
public class ConvFnc_LookupTable1D_Float64_RAM extends JavaConversionFunction {
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
        // Implementation Parameter: dimX                                                                               
        TNumeric dimXCtrVal = this.getImplementationParameterData("dimX");
        // Implementation Parameter: gainX                                                                              
        TNumeric gainXCtrVal = this.getImplementationParameterData("gainX");
        // Implementation Parameter: offsetX                                                                            
        TNumeric offsetXCtrVal = this.getImplementationParameterData("offsetX");

// USERCODE-BEGIN:ConvMaskToImplementation                                                                              
        final int size_x;
        final int SIZE_ROW = 1;
        final int MAX_DIM = 257;
        double x_min, x_max, offsetX, gainX;

        /* get table size */
        size_x = Integer.valueOf(DimXMaskVal.getValue());

        /* check selected table size */
        if (size_x > MAX_DIM) {
            throw new Exception("DimX must not be greater than " + MAX_DIM + " for RAM-Table implementation of this block!");
        }

        /* check table data for correct size */
        if ((TableDataMaskData.getNumColumns() != size_x) || (TableDataMaskData.getNumRows() != SIZE_ROW)) {
            throw new Exception("Lookup Table Size mismatch in column: expected=(" + SIZE_ROW + ", " + size_x + ") " + "got=("
                    + TableDataMaskData.getNumRows() + "," + TableDataMaskData.getNumColumns() + ")");
        }

        /* convert dimension value */
        dimXCtrVal.setReal(0, 0, size_x - 1);

        /* convert input scaling */
        x_min = Double.valueOf(x_minMaskVal.getValue());
        x_max = Double.valueOf(x_maxMaskVal.getValue());
        if (x_min >= x_max) {
            throw new Exception("Scaling of x-dimension not possible: x_max must be greater than x_min!)");
        }
        offsetX = x_min;
        gainX = (size_x - 1) / (x_max - x_min);
        offsetXCtrVal.setReal(0, 0, offsetX);
        gainXCtrVal.setReal(0, 0, gainX);

        /* convert table data */
        double[] tmpData = new double[MAX_DIM];
        for (int i = 0; i < size_x; i++) {
            tmpData[i] = TableDataMaskData.getReal(0, i);
        }

        /* fill unused remaining data with zeroes */
        for (int i = size_x; i < MAX_DIM; i++) {
            tmpData[i] = 0.0d;
        }

        TableCtrVal = new TNumeric(tmpData);
// USERCODE-END:ConvMaskToImplementation                                                                                
        this.setImplementationParameterData("Table", TableCtrVal);
        this.setImplementationParameterData("dimX", dimXCtrVal);
        this.setImplementationParameterData("gainX", gainXCtrVal);
        this.setImplementationParameterData("offsetX", offsetXCtrVal);
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
        // Implementation Parameter: dimX                                                                               
        TNumeric dimXCtrVal = this.getImplementationParameterData("dimX");
        // Implementation Parameter: gainX                                                                              
        TNumeric gainXCtrVal = this.getImplementationParameterData("gainX");
        // Implementation Parameter: offsetX                                                                            
        TNumeric offsetXCtrVal = this.getImplementationParameterData("offsetX");

// USERCODE-BEGIN:ConvImplementationToMask                                                                              
        int dimX, size_x;
        double offsetX, gainX, x_min, x_max;

        /* get controller parameter */
        dimX = (int) dimXCtrVal.getReal(0, 0);
        offsetX = offsetXCtrVal.getReal(0, 0);
        gainX = gainXCtrVal.getReal(0, 0);

        /* calculate mask parameter */
        size_x = dimX + 1;
        x_min = offsetX;
        x_max = (dimX / gainX) + offsetX;

        /* convert only relevant table data */
        double[] tmpData = new double[size_x];
        for (int i = 0; i < size_x; i++) {
            tmpData[i] = TableCtrVal.getReal(0, i);
        }

        /* set mask parameter */
        TableDataMaskData = new TNumeric(tmpData);
        DimXMaskData = new TString(Integer.toString(size_x));
        x_minMaskData.setReal(0, 0, x_min);
        x_maxMaskData.setReal(0, 0, x_max);

// USERCODE-END:ConvImplementationToMask                                                                                
        this.getMaskParameter("TableData").setValue(TableDataMaskData.toFullString());
        this.getMaskParameter("DimX").setValue(DimXMaskData.getString());
        this.getMaskParameter("x_min").setValue(x_minMaskData.toFullString());
        this.getMaskParameter("x_max").setValue(x_maxMaskData.toFullString());
    }
}
