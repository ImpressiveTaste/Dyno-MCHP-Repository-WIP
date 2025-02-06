/* 
 * File:   main.c
 * Author: M91456
 *
 * Created on 7. Februar 2024, 11:55
 */

#include "xc.h"
#include <stdio.h>
#include <stdlib.h>

#include "hal/board_service.h"
#include "hal/clock.h"
#include "hal/port_config.h"
#include "hal/uart1.h"
#include "hal/pwm.h"
#include "hal/timer1.h"
#include "hal/adc.h"
#include "hal/sccp.h"
#include "x2cscope/diagnostics.h"



const uint16_t PWM_STATE1[8] = {0x3000, 0x3400, 0x3000, 0x1000, 0x1000, 0x3000, 0x3400, 0x3000};
const uint16_t PWM_STATE2[8] = {0x3000, 0x3000, 0x3400, 0x3400, 0x3000, 0x1000, 0x1000, 0x3000};
const uint16_t PWM_STATE3[8] = {0x3000, 0x1000, 0x1000, 0x3000, 0x3400, 0x3400, 0x3000, 0x3000};
const uint16_t CommutationSector[8] = {0, 1, 5, 6, 3, 2, 4, 0};

/*AND & OR operators for masking the active BEMF signal*/
const unsigned int ADC_MASK[8] = {0x0000, 0x0002, 0x0001, 0x0004, 0x0002, 0x0001, 0x0004, 0x0000};
const unsigned int ADC_XOR[8] = {0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000};

/*BEMF Majority Function Filter values*/
const unsigned char ADC_BEMF_FILTER[64] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E,
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x01, 0x01, 0x01, 0x36, 0x01, 0x3A, 0x3C, 0x3E,
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x01, 0x01, 0x01, 0x16, 0x01, 0x1A, 0x1C, 0x1E,
    0x01, 0x01, 0x01, 0x26, 0x01, 0x2A, 0x2C, 0x2E, 0x01, 0x01, 0x01, 0x36, 0x01, 0x3A, 0x3C, 0x3E};

#define MAX_PWM_TICKS 2000 		//for detecting initial rotor position
#define MAX_STARTUP_CNT 10       // Number of Turns for startup
#define RAM_UP_DELAY  0    	 	// Delay for the ramp up sequence, expressed in millisecond
#define ONE_MS      391
#define MIN_PERIOD  5
#define MAX_PERIOD  50
#define STARTUP_DUTY    2000
#define PHASE_ADVANCE_DEGREES 12//Phase advance angles to get the best motor performance
#define BLANKING_COUNT 3		//Blanking count expressed in PWM periods used to avoid false zero-crossing detection after commutating motor
#define ROTOR_POSITION_FLT 5
/*Application Flags to indicate the motor status*/
struct
{
    unsigned RunMotor : 1;
    unsigned RotorAlignment : 1;
    unsigned unused : 14;
} Flags;

uint16_t DutyCycle=MIN_DUTY;
uint16_t OnOff=0;

uint16_t CommutationIdx = 0;
uint16_t CommutationTime;
uint32_t Timer2Value_neu;
uint32_t Timer2Value_alt;
uint32_t Timer2Average;
uint32_t Timer1Value;

uint32_t PWMticks;
uint8_t CommState;
uint16_t ADCCommState;
uint8_t RampUpCommState;
uint8_t adcBackEMFFilter;
uint16_t PhaseAdvance = 0;
uint32_t PhaseAdvanceDegrees = PHASE_ADVANCE_DEGREES;
uint16_t BlankingCounter;
uint8_t LoopCnt_u8;
uint8_t BlankCnt_max_u8;

uint16_t V_M1;
uint16_t V_M2;
uint16_t V_M3;
uint16_t I_a;
uint16_t I_b;
uint16_t I_sum;
uint16_t V_pot;
uint16_t HallValue;		// Variable containing the Hall Value from PORTB
uint16_t RecNeutrVoltage;

uint8_t ComparatorOutputs;
uint8_t RotorPosition;
uint8_t RotorPosition_alt;
uint8_t RotorPosition_cnt;
uint8_t CompOld_u8;
uint8_t EnableCommutate_u8;

uint16_t DesiredPWMDutyCycle;
uint16_t CurrentPWMDutyCycle;
uint16_t ReferenceSpeed;

uint32_t Counter;
uint32_t PGxIOCONhelp;

void DelayNmSec(uint16_t Counter);
void PreCommutationState(void);
void OpenLoopController(void);
void StartMotor(void);
void StopMotor(void);


/*
 * 
 */
int main(int argc, char** argv)
{
    InitClock();
    SetupGPIOPorts();
    TIMER1_Initialize();
    TIMER1_InputClockSet();
    TIMER1_PeriodSet(TIMER1_PERIOD_COUNT);
    TIMER1_InterruptPrioritySet(5);
    TIMER1_InterruptFlagClear();
    TIMER1_InterruptEnable(); 
    TIMER1_ModuleStart();
#ifdef INTERNAL_OPAMP_CONFIG    
    OpampConfig();
#endif
    Init_ADC();
    InitPWMGenerators();
    DiagnosticsInit();
    BoardServiceInit();
    EnablePWMOutputsInverterA();
    PWMDutyCycleSet(LOOPTIME_TCY/2);
    SCCP1_setup_for_32Bit_Counter();
    SCCP2_setup_for_32Bit_Counter();
    
    while(1)
    {
        DiagnosticsStepMain();
         BoardService();

        if(IsPressed_Button1())
        {
            OnOff = 1;
            EnablePWMOutputsInverterA();
            LED1 = 1;
            StartMotor();
        }

        /****************** Motor is running *********************************/
        if(Flags.RunMotor) // while motor is running
        {
            /****************** Stop Motor *********************************/
            if(IsPressed_Button2())
            {
                OnOff = 0;
                StopMotor();
                DisablePWMOutputsInverterA();
                LED1 = 0;
            }
        }
    }
}

void __attribute__((__interrupt__)) _T1Interrupt()
{
    if(++Counter >= 5)
    {
        BoardServiceStepIsr();
        Counter = 0;
    }
//    LED2 ^= 1;
    _T1IF = 0;
}

void __attribute__((__interrupt__)) _PWM1Interrupt(void)
{
    /* Add your custom callback code here or implement
    the Callback in the application code, without the weak attribute */

    if(Flags.RunMotor)
    {
        //Ramp-up period to detect the rotor position
        if (Flags.RotorAlignment && Flags.RunMotor)
        {
            PG1DC = PG2DC = PG3DC = CurrentPWMDutyCycle;
            ++PWMticks;
        }
        //Check if the Ramp-up value is disabled, if so starts sensorless operation
        if (++PWMticks > MAX_PWM_TICKS)
        {
            PreCommutationState();
        }

        // Masking the BEMF signals according to the SECTOR in order to determine the ACTIVE BEMF signal
        // XOR operator helps to determine the direction of the upcoming zero-crossing slope
        BlankingCounter++;
        if(BlankingCounter > BLANKING_COUNT)
        {
            if((ComparatorOutputs^ADC_XOR[ADCCommState])& ADC_MASK[ADCCommState])
                adcBackEMFFilter|=0x01;

            //Majority detection filter	
            adcBackEMFFilter = ADC_BEMF_FILTER[adcBackEMFFilter];
            if (adcBackEMFFilter&0b00000001)
                PreCommutationState();
        }
    }
    HallValue = (0x0007 &((HALL_A_GetValue()<<2) + (HALL_B_GetValue()<<1) + HALL_C_GetValue()));
//    if(OnOff)
//    {
//        PG1IOCON &= 0xFFFF0000;
//        PG1IOCON += PWM_STATE1[CommutationSector[HallValue]];
//        PG2IOCON &= 0xFFFF0000;
//        PG2IOCON += PWM_STATE2[CommutationSector[HallValue]];
//        PG3IOCON &= 0xFFFF0000;
//        PG3IOCON += PWM_STATE3[CommutationSector[HallValue]];
//    }
//    else
//    {
//        DisablePWMOutputsInverterA();
//    }
    _PWM1IF = 0;
}

void __attribute__((__interrupt__))HAL_MC1ADCInterrupt(void)
{   
    I_a = ADCBUF_IA;
    I_b = ADCBUF_IB;
    V_pot = ReferenceSpeed = ADCBUF_POT * 5;
    V_M1 = ADCBUF_VM1;
    V_M2 = ADCBUF_VM2;
    V_M3 = ADCBUF_VM3;

    //Reconstrucs Voltage at the  Motor Neutral Point
    RecNeutrVoltage = (V_M1 + V_M2 + V_M3) / 3;

    /********************* ADC SAMPLING & BMEF signals comparison ****************/

    if (BlankingCounter > BlankCnt_max_u8)
    {        
        ComparatorOutputs = 0;						// Precondition all comparator bits as zeros
		if(V_M1 > RecNeutrVoltage)
			ComparatorOutputs += 1;					// Set bit 0 when Phase C is higher than Neutural
		if(V_M2 > RecNeutrVoltage)
			ComparatorOutputs += 2;					// Set bit 1 when Phase B is higher than Neutural
		if(V_M3 > RecNeutrVoltage)
			ComparatorOutputs += 4;					// Set bit 2 when Phase A is higher than Neutral
    }    

    DiagnosticsStepIsr();
    HAL_MC1ADCInterruptFlagClear;
}

void __attribute__((__interrupt__))_CCT1Interrupt(void)
{
    // Add your custom callback code here
    PG1IOCON &= 0xFFFF0000;
    PG1IOCON += PWM_STATE1[ADCCommState];
    PG2IOCON &= 0xFFFF0000;
    PG2IOCON += PWM_STATE2[ADCCommState];
    PG3IOCON &= 0xFFFF0000;
    PG3IOCON += PWM_STATE3[ADCCommState];

    BlankingCounter = 0;

    SCCP1_Stop(); // Stop TIMER1
    SCCP1_Timer_Clear();
    _CCT1IF = 0;
    
}

/******************************************************************************
 * Function:     PreCommutationState(void)
 *
 * Output:		None
 *
 * Overview:		This function measures the 60 and 30 electrical degrees
 *				using the TIMER2. The 60 electrical degrees is proportional
 *				to the elpased time between zero-crossing events.
 *				The zero-crossing events occur 30 electrical degrees in advace
 *				of the commutation point. Hence a delay proportional to the 30
 *				electrical degrees is added using the TIMER1
 *
 * Note:			None
 *******************************************************************************/
void PreCommutationState(void)
{
// Calculate the time proportional to the 60 electrical degrees
    SCCP2_Stop(); // Stop TIMER2
    Timer2Value_neu = SCCP2_Get_Timer_32Bit();
    Timer2Average = ((Timer2Average + Timer2Value_alt + Timer2Value_neu) / 3);
    Timer2Value_alt = Timer2Value_neu;
    SCCP2_Timer_Clear();
    SCCP2_Start(); // Start TIMER2
//Calculate the delay in TIMER1 counts proportional to the Phase Adv angle
    PhaseAdvance = ((Timer2Average * PhaseAdvanceDegrees) / 60);

// Calculate the time proportional to the 30 electrical degrees
// Load the TIMER1 with  the TIMER1 counts porportional to 30 deg	minus the PHASE ADV angle delay
    Timer1Value = (((Timer2Average) >> 1) - PhaseAdvance);
    if (Timer1Value > 1)
    {
        SCCP1_Set_Period_32Bit( Timer1Value );
    }
    else
    {
        Timer1Value = 1;
        SCCP1_Set_Period_32Bit( Timer1Value );
    }
    // Start TIMER1
    SCCP1_Timer_Clear();
    SCCP1_Start();

    //disabling rotor alignment & ramp-up sequence
    PWMticks = 0;
    RampUpCommState = 7;
    Flags.RotorAlignment = false;

//if Motor is runnining in sensorless mode and the PI button is ON on DMIC window
//then the PI controller is enabled if the PI button is OFF then motor runs in open loop
    if ((!Flags.RotorAlignment) && Flags.RunMotor)
    {
        OpenLoopController();
    }

    // Change The Six-Step Commutation Sector
    adcBackEMFFilter = 0;
    if (++ADCCommState > 6)
        ADCCommState = 1;
}

/******************************************************************************
 * Function:     OpenLoopController(void)
 *
 * Output:		None
 *
 * Overview:		When the PI button is OFF on the DMCI window
 *				the motor operates in open loop mode.
 *
 * Note:			None
 *******************************************************************************/
void OpenLoopController(void)
{
    //PWM duty cycle = pot value *3/2
    DesiredPWMDutyCycle = ReferenceSpeed;
    //Update the duty cycle according to the POT value, a POT follower is implemented here
    if (CurrentPWMDutyCycle != DesiredPWMDutyCycle)
    {
        if (CurrentPWMDutyCycle < DesiredPWMDutyCycle)
            CurrentPWMDutyCycle+=50;
        if (CurrentPWMDutyCycle > DesiredPWMDutyCycle)
            CurrentPWMDutyCycle-=50;
    }
    // Max and Min PWM duty cycle limits
    if (CurrentPWMDutyCycle < MIN_DUTY)
        CurrentPWMDutyCycle = MIN_DUTY;
    if (CurrentPWMDutyCycle > MAX_DUTY)
        CurrentPWMDutyCycle = MAX_DUTY;
    //Assigning new duty cycles to the PWM channels
    PG1DC = PG2DC = PG3DC = CurrentPWMDutyCycle;
//    PG2DC = CurrentPWMDutyCycle;
//    PG3DC = CurrentPWMDutyCycle;

}

/******************************************************************************
 * Function:     DelayNmSec(unsigned int N)
 *
 * Output:		None
 *
 * Overview:		Delay function used for push buttons debounce loop and for the
 *				motor start-up sequence
 *
 * Note:			None
 *******************************************************************************/
void DelayNmSec( uint16_t Counter )
{
uint16_t j;
    while (Counter--)
    {
        for (j = 0; j < 3000; j++){};
    }
}

void StartMotor(void)
{
    OnOff = true;
    SCCP2_Start();

    /*ROTOR ALIGNMENT SEQUENCE*/
    Flags.RotorAlignment = 1; // TURN ON rotor alignment sequence
    Flags.RunMotor = 1; // Indicating that motor is running
    CurrentPWMDutyCycle = STARTUP_DUTY; //Init PWM values
    DesiredPWMDutyCycle = STARTUP_DUTY; //Init PWM values
    PWMticks = 0; //Init Rotor aligment counter
    BlankCnt_max_u8 = BLANKING_COUNT;

    /************* Rotor alignment sequence and ramp-up sequence ************/
    for (RampUpCommState = 1; RampUpCommState < 7; RampUpCommState++)
    {
        while (PWMticks < MAX_PWM_TICKS && Flags.RotorAlignment)
        {
            PG1IOCON &= 0xFFFF0000;
            PG1IOCON += PWM_STATE1[RampUpCommState];
            PG2IOCON &= 0xFFFF0000;
            PG2IOCON += PWM_STATE2[RampUpCommState];
            PG3IOCON &= 0xFFFF0000;
            PG3IOCON += PWM_STATE3[RampUpCommState];
        }
        PWMticks = 0;
    }
    Flags.RotorAlignment = 0; // TURN OFF rotor alignment sequence
    PWMticks = MAX_PWM_TICKS + 1; // RAMP UP for breaking the motor IDLE state
    //DelayNmSec(RAM_UP_DELAY); // RAMP UP DELAY
}

void StopMotor(void)
{
    OnOff = false;
    Flags.RotorAlignment = 0; // turn on RAMP UP
    Flags.RunMotor = 0; // reset run flag
    CurrentPWMDutyCycle = 1; // Set PWM to the min value

    //Initi speed measurement variables & timer
    SCCP2_Stop();
    SCCP2_Timer_Clear();
    Timer2Value_alt = 0;
    Timer2Average = 0;
    SCCP1_Stop(); // Stop TIMER1
    SCCP1_Timer_Clear();
    PG1DC = PG2DC = PG3DC = 0;
    DisablePWMOutputsInverterA();
}

