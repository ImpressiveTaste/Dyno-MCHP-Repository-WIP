/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CK256MP508
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/X2Cscope/X2CscopeComm.h"
#include "mcc_generated_files/X2Cscope/X2Cscope.h"
#include "mcc_generated_files/adc1.h"
#include "mcc_generated_files/sccp1_tmr.h"
#include "mcc_generated_files/sccp3_tmr.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/pwm.h"

const uint16_t PWM_STATE1[8] = {0x3000, 0x3400, 0x3000, 0x1000, 0x1000, 0x3000, 0x3400, 0x3000};
const uint16_t PWM_STATE2[8] = {0x3000, 0x3000, 0x3400, 0x3400, 0x3000, 0x1000, 0x1000, 0x3000};
const uint16_t PWM_STATE3[8] = {0x3000, 0x1000, 0x1000, 0x3000, 0x3400, 0x3400, 0x3000, 0x3000};
uint16_t commutation_sector[8] = {0, 1, 2, 3, 4, 5, 6, 0};

/* AND & OR operators for masking the active BEMF signal */
const unsigned int ADC_MASK[8] = {0x0000, 0x0002, 0x0001, 0x0004, 0x0002, 0x0001, 0x0004, 0x0000};
const unsigned int ADC_XOR[8] = {0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000};

/* BEMF Majority Function Filter values */
const unsigned char ADC_BEMF_FILTER[64] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E,
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x01, 0x01, 0x01, 0x36, 0x01, 0x3A, 0x3C, 0x3E,
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x01, 0x01, 0x01, 0x16, 0x01, 0x1A, 0x1C, 0x1E,
    0x01, 0x01, 0x01, 0x26, 0x01, 0x2A, 0x2C, 0x2E, 0x01, 0x01, 0x01, 0x36, 0x01, 0x3A, 0x3C, 0x3E
};


#define MAX_PWM_TICKS           200     // To detect initial rotor position
#define MAX_STARTUP_CNT         10      // Number of Turns for startup
#define RAM_UP_DELAY            0    	// Delay for the ramp up sequence, expressed in millisecond
#define ONE_MS                  391
#define MIN_PERIOD              5
#define MAX_PERIOD              50
#define MIN_DUTY                200
#define STARTUP_DUTY            800
#define MAX_DUTY                4999
#define PHASE_ADVANCE_DEGREES   12      // Phase advance angles to get the best motor performance
#define BLANKING_COUNT          3		// Blanking counts after section commuting for zero-crossing detection
#define ROTOR_POSITION_FLT      5

#define DIRECTION_LED_HIGH      600
#define DIRECTION_LED_LOW       100

/*Application Flags to indicate the motor status*/
struct
{
    unsigned running : 1;
    unsigned alignment : 1;
    unsigned unused : 14;
} Motor;

uint16_t duty_cycle = MIN_DUTY;

uint16_t commutation_idx = 0;
uint16_t timer_60deg_new;
uint16_t timer_60deg_old;
uint16_t timer_60deg_average;
uint16_t timer_30deg_value;

uint16_t PWMticks;
uint16_t adc_comm_state;
uint8_t rampUp_comm_state;
uint8_t adc_BEMF_filter;
uint16_t phase_advance_value = 0;
uint16_t phase_advance_degrees = PHASE_ADVANCE_DEGREES;
uint16_t blanking_counter;
uint8_t blank_max_count;

uint16_t V_M1;
uint16_t V_M2;
uint16_t V_M3;
uint16_t I_a;
uint16_t I_b;
uint16_t I_sum;
uint16_t V_pot;
uint16_t neutral_voltage;

uint8_t comparator_output;

uint16_t target_PWM_dutyCycle;
uint16_t current_PWM_dutyCycle;
uint16_t ref_speed;

uint16_t millis = 0;
uint8_t direction = 0;
uint8_t motor_active = 0;
uint16_t led_counter = 0;

void pre_commutation_state(void);
void open_loop_controller(void);
void delay_millis(uint16_t counter);
void start_stop_motor(void);
void start_motor(void);
void stop_motor(void);
void change_direction(void);

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    SW1_SetInterruptHandler(change_direction);
    SW2_SetInterruptHandler(start_stop_motor);
    
    while(1)
    {
        X2CScope_Communicate();
    }
    return 1; 
}

void start_stop_motor(void)
{
    motor_active ^= 1;
    if(motor_active)
    {
        start_motor();
    }
    else
    {
        stop_motor();
    }
}

void start_motor(void)
{
    if(Motor.running) return;
    LED1_SetHigh();
    
    // Start 60 degrees timer counter
    SCCP3_TMR_Start();

    /* ROTOR ALIGNMENT SEQUENCE */
    Motor.alignment = 1; // TURN ON rotor alignment sequence
    Motor.running = 1; // Indicating that motor is running
    current_PWM_dutyCycle = STARTUP_DUTY; // Initiate PWM values
    target_PWM_dutyCycle = STARTUP_DUTY; // Initiate PWM values
    PG1DC = PG2DC = PG3DC = current_PWM_dutyCycle;
    PWMticks = 0; // Initiate Rotor alignment counter
    blank_max_count = BLANKING_COUNT;

    /************* Rotor alignment sequence and ramp-up sequence ************/
    for (rampUp_comm_state = 1; rampUp_comm_state < 7; rampUp_comm_state++)
    {
        while (PWMticks < MAX_PWM_TICKS && Motor.alignment)
        {
            commutation_idx = commutation_sector[rampUp_comm_state];
            PG1IOCONL = PWM_STATE1[commutation_idx];
            PG2IOCONL = PWM_STATE2[commutation_idx];
            PG3IOCONL = PWM_STATE3[commutation_idx];
        }
        PWMticks = 0;
        delay_millis(RAM_UP_DELAY); // RAMP UP DELAY   
    }
    Motor.alignment = 0; // TURN OFF rotor alignment sequence
    PWMticks = MAX_PWM_TICKS + 1; // RAMP UP for breaking the motor IDLE state
}

void stop_motor(void)
{
    if(!Motor.running) return;
    LED1_SetLow();
    
    Motor.alignment = 0; // turn on RAMP UP
    Motor.running = 0; // reset run flag
    current_PWM_dutyCycle = 0; // Set PWM to the min value

    //Initi speed measurement variables & timer
    SCCP3_TMR_Stop();
    SCCP3_TMR_Counter16BitPrimarySet(0);
    timer_60deg_old = 0;
    timer_60deg_average = 0;
    SCCP1_TMR_Stop(); // Stop TIMER1
    SCCP1_TMR_Counter16BitPrimarySet(0);
    PG1DC = PG2DC = PG4DC = 0;
    PG1IOCONL = PWM_STATE1[0];
    PG2IOCONL = PWM_STATE2[0];
    PG3IOCONL = PWM_STATE3[0];    
}

void revert_commutation(void)
{
    uint8_t temp;
    for(uint8_t i = 1; i < 4; i++)
    {
        temp = commutation_sector[i];
        commutation_sector[i] = commutation_sector[7-i];
        commutation_sector[7-i] = temp;
    }
    direction ^= 1;
}

void change_direction(void)
{
    if(Motor.running)
    {
        stop_motor();
        revert_commutation();
        delay_millis(300);
        start_motor();            
    }
    else
    {
        revert_commutation();
    }
}

void PWM_Generator1_CallBack(void)
{
    if(!Motor.running) return;
    
    PWMticks++;
    if(Motor.alignment)
    {
        //Ramp-up period to detect the rotor position
        PG1DC = PG2DC = PG3DC = current_PWM_dutyCycle;
    }
    // Sensorless operation. PreCommutationState clears PWMticks (PWMticks = 0)
    if(PWMticks > MAX_PWM_TICKS) pre_commutation_state();

    // skip the first measurements after the sector commutation due high noise.
    blanking_counter++;
    if(blanking_counter > BLANKING_COUNT)
    {
        // Masking the BEMF signals according to the SECTOR in order to determine the ACTIVE BEMF signal
        // XOR operator helps to determine the direction of the upcoming zero-crossing slope
        if((comparator_output ^ ADC_XOR[adc_comm_state]) & ADC_MASK[adc_comm_state])
            adc_BEMF_filter |= 0x01;

        // Majority detection filter 
        adc_BEMF_filter = ADC_BEMF_FILTER[adc_BEMF_filter];
        if(adc_BEMF_filter & 0x01) pre_commutation_state();
    }
}

void  ADC1_CallBack(void)
{
    /********************* ADC SAMPLING & BMEF signals comparison ****************/
    I_a = ADC1_ConversionResultGet(I_A);
    I_b = ADC1_ConversionResultGet(I_B);
    I_sum = ADC1_ConversionResultGet(I_BUS);
    V_pot = ref_speed = ADC1_ConversionResultGet(V_POT);
    V_M1 = ADC1_ConversionResultGet(V_A);
    V_M2 = ADC1_ConversionResultGet(V_B);
    V_M3 = ADC1_ConversionResultGet(V_C);

    //Reconstructs Voltage at the  Motor Neutral Point
    neutral_voltage = (V_M1 + V_M2 + V_M3) / 3;

    if (blanking_counter > blank_max_count)
    {        
        comparator_output = 0;  // Precondition all comparator bits as zeros
        if(direction == 0)
        {
            if(V_M1 > neutral_voltage) comparator_output += 1;  // Set bit 0 when Phase A is higher than Neutral
            if(V_M2 > neutral_voltage) comparator_output += 2;	// Set bit 1 when Phase B is higher than Neutral
            if(V_M3 > neutral_voltage) comparator_output += 4;	// Set bit 2 when Phase C is higher than Neutral            
        }
        else
        {
            if(V_M1 > neutral_voltage) comparator_output += 1;  // Set bit 0 when Phase A is higher than Neutral
            if(V_M3 > neutral_voltage) comparator_output += 2;	// Set bit 1 when Phase C is higher than Neutral
            if(V_M2 > neutral_voltage) comparator_output += 4;	// Set bit 2 when Phase B is higher than Neutral            
        }
    }    
    X2CScope_Update();
}

void SCCP1_TMR_PrimaryTimerCallBack(void)
{
    commutation_idx = commutation_sector[adc_comm_state];
    PG1IOCONL = PWM_STATE1[commutation_idx];
    PG2IOCONL = PWM_STATE2[commutation_idx];
    PG3IOCONL = PWM_STATE3[commutation_idx];
    blanking_counter = 0;
    SCCP1_TMR_Stop(); // Stop commutation trigger timer
    SCCP1_TMR_Counter16BitPrimarySet(0);
}

/******************************************************************************
 * Function:    pre_commutation_state
 *
 * Output:		None
 *
 * Overview:	This function measures the 60 and 30 electrical degrees. 
 *              The 60 electrical degrees is proportional to the elapsed time 
 *              between two consecutive zero-crossing events. 
 *              The zero-crossing event occurs 30 electrical degrees before the
 *              the commutation point. Hence a delay proportional to 30 
 *              electrical degrees is calculated and programmed on a separated
 *              peripheral timer.
 *
 * Note:			None
 *******************************************************************************/
void pre_commutation_state(void)
{
    // Calculate the time proportional to the 60 electrical degrees
    SCCP3_TMR_Stop(); 
    timer_60deg_new = SCCP3_TMR_Counter16BitPrimaryGet();
    timer_60deg_average = ((timer_60deg_average + timer_60deg_old + timer_60deg_new) / 3);
    timer_60deg_old = timer_60deg_new;
    SCCP3_TMR_Counter16BitPrimarySet(0);
    SCCP3_TMR_Start(); 

    // Calculate the delay proportional to the phase advance angle for best torch
    phase_advance_value = ((timer_60deg_average * phase_advance_degrees) / 60);

    // Calculate the time proportional to the 30 electrical degrees
    // Load the TIMER1 with  the TIMER1 counts proportional to 30 degrees - the PHASE ADV angle delay
    timer_30deg_value = (timer_60deg_average >> 1) - phase_advance_value;
    if(timer_30deg_value == 0) timer_30deg_value = 1;
    SCCP1_TMR_Period16BitPrimarySet(timer_30deg_value);
    SCCP1_TMR_Counter16BitPrimarySet(0);
    SCCP1_TMR_Start();

    // Clear PWM ticks
    PWMticks = 0;

    // PI controller is enabled
    open_loop_controller();

    // Change The Six-Step Commutation Sector
    adc_BEMF_filter = 0;
    if (++adc_comm_state > 6) adc_comm_state = 1;
}

/******************************************************************************
 * Function:    open_loop_controller(void)
 *
 * Output:		None
 *
 * Overview:	When the PI button is OFF on the DMCI window
 *				the motor operates in open loop mode.
 *
 * Note:		None
 *******************************************************************************/
void open_loop_controller(void)
{
    target_PWM_dutyCycle = ref_speed;
    
    //Update the duty cycle according to the POT value, a POT follower is implemented here
    if (current_PWM_dutyCycle != target_PWM_dutyCycle)
    {
        if (current_PWM_dutyCycle < target_PWM_dutyCycle)
            current_PWM_dutyCycle+=10;
        if (current_PWM_dutyCycle > target_PWM_dutyCycle)
            current_PWM_dutyCycle-=10;
    }
    
    // Max and Min PWM duty cycle limits
    if (current_PWM_dutyCycle < MIN_DUTY) current_PWM_dutyCycle = MIN_DUTY;
    if (current_PWM_dutyCycle > MAX_DUTY) current_PWM_dutyCycle = MAX_DUTY;
    
    // Assigning new duty cycles to the PWM channels
    PG1DC = PG2DC = PG3DC = current_PWM_dutyCycle;
}

/******************************************************************************
 * Function:    delay_millis(unsigned int counter)
 *
 * Output:		None
 *
 * Overview:    delay an amount of 'counter' milliseconds
 *
 * Note:		None
 *******************************************************************************/
void delay_millis(uint16_t counter)
{
    millis = counter;
    while(millis > 0);
}

void TMR1_CallBack()
{
    if(millis > 0) millis--;
    if(led_counter > 0) led_counter--;
    else
    {
        LED2_Toggle();
        if(direction) led_counter = LED2_GetValue()? DIRECTION_LED_HIGH: DIRECTION_LED_LOW;
        else          led_counter = LED2_GetValue()? DIRECTION_LED_LOW: DIRECTION_LED_HIGH;
    }
}
/**
 End of File
*/

