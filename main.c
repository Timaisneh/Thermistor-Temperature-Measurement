/* ========================================
 *
 * 2023 - 03 - 25
 * Michael Olagunju
 * 
 *
 * ECEA 5341
 * Thermistor Lab
 *
 * ========================================
*/

//required libraries
#include "project.h"
#include "math.h"
#include "float.h"

// Variable Declarations
uint32 loop_counter, LCD_write_counter, thermistor_R;
uint16 R_1_result, thermistor_result;
int16 thermistor_mV, R_1_mV, temperature;
float R_1_result_f, R_1_mV_f, thermistor_mV_f, ratio_f, voltage_f;
float thermistor_RA_f, thermistor_RB_f;
float current_mA_f, temperature_K_f;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_DelSig_1_Start(); //powers up the ADC
    AMux_1_Start();       //powers up the analog converter
    
    LCD_Start(); //Powers up the LCD
    LCD_Position(0,0); //initializes the lcd
    LCD_PrintString("Thermistor Test");
    CyDelay(1000);
    
    LCD_Position(0,0); //The next character will be written to row zero column zero

    //infinte loop
    while(1)
    {
        loop_counter++;
        LCD_write_counter++; //to control writing to the LCD
        
        p0_1_Write(1); //can use this digital output with a scope to measure the loop
        
        AMux_1_Select(0);  //select the mux input for the 10k resistor
        ADC_DelSig_1_StartConvert(); //This starts the ADC doing conversions
        
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT); //pauses the program until adc finishes converting
        
        p0_1_Write(0);
        
        R_1_result = ADC_DelSig_1_GetResult16(); //16bit reading for R1
        R_1_result_f = (float)R_1_result;
        
        R_1_mV = (int16)ADC_DelSig_1_CountsTo_mVolts(R_1_result); // convert type to int16
        R_1_mV_f = (float)R_1_mV; //convert to floating point
        
        current_mA_f = R_1_mV_f/ 10000;
        
        AMux_1_Select(1);  //select the mux input for the Thermistor
        ADC_DelSig_1_StartConvert(); //This starts the ADC doing conversions
        
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT); //pauses the program until adc finishes converting
        
        thermistor_result = ADC_DelSig_1_GetResult16();
        thermistor_mV = (int16)ADC_DelSig_1_CountsTo_mVolts(thermistor_result);
        thermistor_mV_f = (float)thermistor_mV;
        thermistor_RA_f = thermistor_mV_f / current_mA_f; //caluclate thermistor resistance w V = IR
        thermistor_R = Thermistor_1_GetResistance(R_1_mV, thermistor_mV); //using the thermistor API to calculate resistance
        thermistor_RB_f = (float)thermistor_R;
        
        temperature = Thermistor_1_GetTemperature(thermistor_R);
        
        
    
    if(LCD_write_counter++ > 1000)
       {
        LCD_write_counter = 0;
        p0_0_Write(1); //test point for measurement
        LCD_IsReady();
        LCD_Position(0,0);
        LCD_PrintString("loop cnt:    "); //display loop count on lcd
        LCD_Position(0,10); //row zero column 10
        LCD_PrintU32Number(loop_counter); //shows that the loop is running
        LCD_Position(1,0); //First Character of the second line
        
        LCD_PrintString("      degrees"); //clears the line by writing blanks
        LCD_Position(1,0); //Move back to the first character of the second line
        LCD_PrintNumber( temperature/100); //displays first two significant digits
        LCD_Position(1,2);
        LCD_PrintString(".");
        LCD_PrintNumber( temperature/100); //display last two significant digits
        
        p0_0_Write(0);
    
        }
    }
}

/* [] END OF FILE */
