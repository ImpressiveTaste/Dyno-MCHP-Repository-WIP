/*******************************************************************************
  Controller Area Network (CAN) Peripheral Library Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_can1.c

  Summary:
    CAN peripheral library interface.

  Description:
    This file defines the interface to the CAN peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Header Includes
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_can1.h"
#include "can1_Lib.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************
#define CAN_STD_ID_Msk    0x7FF

// static CAN_OBJ can1Obj; EL02
CAN_OBJ can1Obj;

static const can_sidfe_registers_t can1StdFilter[] =
{
    {
        .CAN_SIDFE_0 = CAN_SIDFE_0_SFT(0) |
                  CAN_SIDFE_0_SFID1(2032) |
                  CAN_SIDFE_0_SFID2(2047) |
                  CAN_SIDFE_0_SFEC(1)
    },
};

/******************************************************************************
Local Functions
******************************************************************************/

static uint8_t CANDlcToLengthGet(uint8_t dlc)
{
    uint8_t msgLength[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};
    return msgLength[dlc];
}

// *****************************************************************************
// *****************************************************************************
// CAN1 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void CAN1_Initialize(void)

   Summary:
    Initializes given instance of the CAN peripheral.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None
*/
void CAN1_Initialize(void)
{
    /* Start CAN initialization */
    CAN1_REGS->CAN_CCCR = CAN_CCCR_INIT_Msk;
    while ((CAN1_REGS->CAN_CCCR & CAN_CCCR_INIT_Msk) != CAN_CCCR_INIT_Msk);

    /* Set CCE to unlock the configuration registers */
    CAN1_REGS->CAN_CCCR |= CAN_CCCR_CCE_Msk;

    /* Set Nominal Bit timing and Prescaler Register */
    CAN1_REGS->CAN_NBTP  = CAN_NBTP_NTSEG2(14) | CAN_NBTP_NTSEG1(43) | CAN_NBTP_NBRP(0) | CAN_NBTP_NSJW(3);


    /* Global Filter Configuration Register */
    CAN1_REGS->CAN_GFC = CAN_GFC_ANFS_REJECT | CAN_GFC_ANFE_REJECT;

    /* Timestamp Counter Configuration Register */
    CAN1_REGS->CAN_TSCC = CAN_TSCC_TSS_INC | CAN_TSCC_TCP(0);

    /* Set the operation mode */
    CAN1_REGS->CAN_CCCR = (CAN1_REGS->CAN_CCCR & ~CAN_CCCR_INIT_Msk);
    while ((CAN1_REGS->CAN_CCCR & CAN_CCCR_INIT_Msk) == CAN_CCCR_INIT_Msk);

    /* Select interrupt line */
    CAN1_REGS->CAN_ILS = 0x0;

    /* Enable interrupt line */
    CAN1_REGS->CAN_ILE = CAN_ILE_EINT0_Msk;

    /* Enable CAN interrupts */
    CAN1_REGS->CAN_IE = CAN_IE_BOE_Msk; 
    
    // Initialize the CAN PLib Object
    can1Obj.txBufferIndex = 0;
    can1Obj.rxId = 0;
    can1Obj.rxBuffer = 0;
    can1Obj.rxsize = 0;
    
    can1Obj.state = CAN_STATE_TRANSFER_RECEIVE; // CAN_STATE_IDLE; // EL02    
    memset((void*)&can1Obj.msgRAMConfig, 0x00, sizeof(CAN_MSG_RAM_CONFIG));
}

// *****************************************************************************
/* Function:
    bool CAN1_MessageTransmit(uint32_t id, uint8_t length, uint8_t* data, CAN_MODE mode, CAN_MSG_TX_ATTRIBUTE msgAttr)

   Summary:
    Transmits a message into CAN bus.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    id      - 11-bit / 29-bit identifier (ID).
    length  - length of data buffer in number of bytes.
    data    - pointer to source data buffer
    mode    - CAN mode Classic CAN or CAN FD without BRS or CAN FD with BRS
    msgAttr - Data Frame or Remote frame using Tx FIFO or Tx Buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool CAN1_MessageTransmit(uint32_t id, uint8_t length, uint8_t* data, CAN_MODE mode, CAN_MSG_TX_ATTRIBUTE msgAttr)
{
    uint8_t tfqpi = 0;
    can_txbe_registers_t *fifo = NULL;

    
    switch (msgAttr)
    {
        case CAN_MSG_ATTR_TX_FIFO_DATA_FRAME:
        case CAN_MSG_ATTR_TX_FIFO_RTR_FRAME:
            if (CAN1_REGS->CAN_TXFQS & CAN_TXFQS_TFQF_Msk)
            {
                /* The FIFO is full */
                return false;
            }
            tfqpi = (uint8_t)((CAN1_REGS->CAN_TXFQS & CAN_TXFQS_TFQPI_Msk) >> CAN_TXFQS_TFQPI_Pos);
            fifo = (can_txbe_registers_t *) ((uint8_t *)can1Obj.msgRAMConfig.txBuffersAddress + tfqpi * CAN1_TX_FIFO_BUFFER_ELEMENT_SIZE);
            break;
        default:
            /* Invalid Message Attribute */
            return false;
    }
    
    /* If the id is longer than 11 bits, it is considered as extended identifier */
    if (id > CAN_STD_ID_Msk)
    {
        /* An extended identifier is stored into ID */
        fifo->CAN_TXBE_0 = (id & CAN_TXBE_0_ID_Msk) | CAN_TXBE_0_XTD_Msk;
    }
    else
    {
        /* A standard identifier is stored into ID[28:18] */
        fifo->CAN_TXBE_0 = id << 18;
    }

    /* Limit length */
    if (length > 8)
        length = 8;
    fifo->CAN_TXBE_1 = CAN_TXBE_1_DLC(length);

    if (msgAttr == CAN_MSG_ATTR_TX_BUFFER_DATA_FRAME || msgAttr == CAN_MSG_ATTR_TX_FIFO_DATA_FRAME)
    {
        /* copy the data into the payload */
        memcpy((uint8_t *)&fifo->CAN_TXBE_DATA, data, length);
    }
    else if (msgAttr == CAN_MSG_ATTR_TX_BUFFER_RTR_FRAME || msgAttr == CAN_MSG_ATTR_TX_FIFO_RTR_FRAME)
    {
        fifo->CAN_TXBE_0 |= CAN_TXBE_0_RTR_Msk;
    }

    CAN1_REGS->CAN_TXBTIE = 1U << tfqpi;
    can1Obj.state = CAN_STATE_TRANSFER_TRANSMIT;

    /* request the transmit */
    CAN1_REGS->CAN_TXBAR = 1U << tfqpi;

    CAN1_REGS->CAN_IE |= CAN_IE_TCE_Msk;
    return true;
}

// *****************************************************************************
/* Function:
    bool CAN1_MessageReceive(uint32_t *id, uint8_t *length, uint8_t *data, CAN_MSG_RX_ATTRIBUTE msgAttr)

   Summary:
    Receives a message from CAN bus.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    id      - Pointer to 11-bit / 29-bit identifier (ID) to be received.
    length  - Pointer to data length in number of bytes to be received.
    data    - pointer to destination data buffer
    msgAttr - Message to be read from Rx FIFO0 or Rx FIFO1 or Rx Buffer

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool CAN1_MessageReceive(uint32_t *id, uint8_t *length, uint8_t *data, CAN_MSG_RX_ATTRIBUTE msgAttr)
{
    uint32_t rxInterrupt = 0;
    bool status = false;

    switch (msgAttr)
    {
        case CAN_MSG_ATTR_RX_FIFO0:
            rxInterrupt = CAN_IE_RF0NE_Msk;
            status = true;
        break;
        case CAN_MSG_ATTR_RX_FIFO1:
            rxInterrupt = CAN_IE_RF1NE_Msk;
            status = true;
        break;
        default:
        return status;
    }
    // EL02 can1Obj.state = CAN_STATE_TRANSFER_RECEIVE;
    can1Obj.rxId = id;
    can1Obj.rxBuffer = data;
    can1Obj.rxsize = length;
    CAN1_REGS->CAN_IE |= rxInterrupt;
    return status;
}

// *****************************************************************************
/* Function:
    CAN_ERROR CAN1_ErrorGet(void)

   Summary:
    Returns the error during transfer.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    None.

   Returns:
    Error during transfer.
*/
CAN_ERROR CAN1_ErrorGet(void)
{
    CAN_ERROR error;
    uint32_t   errorStatus = CAN1_REGS->CAN_PSR;

    error = (CAN_ERROR) ((errorStatus & CAN_PSR_LEC_Msk) | (errorStatus & CAN_PSR_EP_Msk) | (errorStatus & CAN_PSR_EW_Msk)
            | (errorStatus & CAN_PSR_BO_Msk) | (errorStatus & CAN_PSR_DLEC_Msk) | (errorStatus & CAN_PSR_PXE_Msk));

    if ((CAN1_REGS->CAN_CCCR & CAN_CCCR_INIT_Msk) == CAN_CCCR_INIT_Msk)
    {
        CAN1_REGS->CAN_CCCR |= CAN_CCCR_CCE_Msk;
        CAN1_REGS->CAN_CCCR = (CAN1_REGS->CAN_CCCR & ~CAN_CCCR_INIT_Msk);
        while ((CAN1_REGS->CAN_CCCR & CAN_CCCR_INIT_Msk) == CAN_CCCR_INIT_Msk);
    }

    return error;
}

// *****************************************************************************
/* Function:
    bool CAN1_InterruptGet(CAN_INTERRUPT_MASK interruptMask)

   Summary:
    Returns the Interrupt status.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    interruptMask - Interrupt source number

   Returns:
    true - Requested interrupt is occurred.
    false - Requested interrupt is not occurred.
*/
bool CAN1_InterruptGet(CAN_INTERRUPT_MASK interruptMask)
{
    return ((CAN1_REGS->CAN_IR & interruptMask) != 0x0);
}

// *****************************************************************************
/* Function:
    void CAN1_InterruptClear(CAN_INTERRUPT_MASK interruptMask)

   Summary:
    Clears Interrupt status.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    interruptMask - Interrupt to be cleared

   Returns:
    None
*/
void CAN1_InterruptClear(CAN_INTERRUPT_MASK interruptMask)
{
    CAN1_REGS->CAN_IR = interruptMask;
}

// *****************************************************************************
/* Function:
    void CAN1_MessageRAMConfigSet(uint8_t *msgRAMConfigBaseAddress)

   Summary:
    Set the Message RAM Configuration.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    msgRAMConfigBaseAddress - Pointer to application allocated buffer base address.
                              Application must allocate buffer from non-cached
                              contiguous memory and buffer size must be
                              CAN1_MESSAGE_RAM_CONFIG_SIZE

   Returns:
    None
*/
void CAN1_MessageRAMConfigSet(uint8_t *msgRAMConfigBaseAddress)
{
    uint32_t offset = 0;

    memset((void*)msgRAMConfigBaseAddress, 0x00, CAN1_MESSAGE_RAM_CONFIG_SIZE);

    /* Set CAN CCCR Init for Message RAM Configuration */
    CAN1_REGS->CAN_CCCR = CAN_CCCR_INIT_Msk;
    while ((CAN1_REGS->CAN_CCCR & CAN_CCCR_INIT_Msk) != CAN_CCCR_INIT_Msk);

    /* Set CCE to unlock the configuration registers */
    CAN1_REGS->CAN_CCCR |= CAN_CCCR_CCE_Msk;

    can1Obj.msgRAMConfig.rxFIFO0Address = (can_rxf0e_registers_t *)msgRAMConfigBaseAddress;
    offset = CAN1_RX_FIFO0_SIZE;
    /* Receive FIFO 0 Configuration Register */
    CAN1_REGS->CAN_RXF0C = CAN_RXF0C_F0S(1) | CAN_RXF0C_F0WM(0) | CAN_RXF0C_F0OM_Msk |
            CAN_RXF0C_F0SA((uint32_t)can1Obj.msgRAMConfig.rxFIFO0Address);

    can1Obj.msgRAMConfig.rxFIFO1Address = (can_rxf1e_registers_t *)(msgRAMConfigBaseAddress + offset);
    offset += CAN1_RX_FIFO1_SIZE;
    /* Receive FIFO 1 Configuration Register */
    CAN1_REGS->CAN_RXF1C = CAN_RXF1C_F1S(1) | CAN_RXF1C_F1WM(0) | CAN_RXF1C_F1OM_Msk |
            CAN_RXF1C_F1SA((uint32_t)can1Obj.msgRAMConfig.rxFIFO1Address);

    can1Obj.msgRAMConfig.txBuffersAddress = (can_txbe_registers_t *)(msgRAMConfigBaseAddress + offset);
    offset += CAN1_TX_FIFO_BUFFER_SIZE;
    /* Transmit Buffer/FIFO Configuration Register */
    CAN1_REGS->CAN_TXBC = CAN_TXBC_TFQS(1) |
            CAN_TXBC_TBSA((uint32_t)can1Obj.msgRAMConfig.txBuffersAddress);

    can1Obj.msgRAMConfig.txEventFIFOAddress =  (can_txefe_registers_t *)(msgRAMConfigBaseAddress + offset);
    offset += CAN1_TX_EVENT_FIFO_SIZE;
    /* Transmit Event FIFO Configuration Register */
    CAN1_REGS->CAN_TXEFC = CAN_TXEFC_EFWM(0) | CAN_TXEFC_EFS(1) |
            CAN_TXEFC_EFSA((uint32_t)can1Obj.msgRAMConfig.txEventFIFOAddress);

    can1Obj.msgRAMConfig.stdMsgIDFilterAddress = (can_sidfe_registers_t *)(msgRAMConfigBaseAddress + offset);
    memcpy((void *)can1Obj.msgRAMConfig.stdMsgIDFilterAddress,
           (const void *)can1StdFilter,
           CAN1_STD_MSG_ID_FILTER_SIZE);
    offset += CAN1_STD_MSG_ID_FILTER_SIZE;
    /* Standard ID Filter Configuration Register */
    CAN1_REGS->CAN_SIDFC = CAN_SIDFC_LSS(1) |
            CAN_SIDFC_FLSSA((uint32_t)can1Obj.msgRAMConfig.stdMsgIDFilterAddress);

    /* Complete Message RAM Configuration by clearing CAN CCCR Init */
    CAN1_REGS->CAN_CCCR = (CAN1_REGS->CAN_CCCR & ~CAN_CCCR_INIT_Msk);
    while ((CAN1_REGS->CAN_CCCR & CAN_CCCR_INIT_Msk) == CAN_CCCR_INIT_Msk);
}

// *****************************************************************************
/* Function:
    bool CAN1_StandardFilterElementSet(uint8_t filterNumber, can_sidfe_registers_t *stdMsgIDFilterElement)

   Summary:
    Set a standard filter element configuration.

   Precondition:
    CAN1_Initialize and CAN1_MessageRAMConfigSet must have been called
    for the associated CAN instance.

   Parameters:
    filterNumber          - Standard Filter number to be configured.
    stdMsgIDFilterElement - Pointer to Standard Filter Element configuration to be set on specific filterNumber.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool CAN1_StandardFilterElementSet(uint8_t filterNumber, can_sidfe_registers_t *stdMsgIDFilterElement)
{
    if ((filterNumber > 1) || (stdMsgIDFilterElement == NULL))
    {
        return false;
    }
    can1Obj.msgRAMConfig.stdMsgIDFilterAddress[filterNumber - 1].CAN_SIDFE_0 = stdMsgIDFilterElement->CAN_SIDFE_0;

    return true;
}

// *****************************************************************************
/* Function:
    bool CAN1_StandardFilterElementGet(uint8_t filterNumber, can_sidfe_registers_t *stdMsgIDFilterElement)

   Summary:
    Get a standard filter element configuration.

   Precondition:
    CAN1_Initialize and CAN1_MessageRAMConfigSet must have been called
    for the associated CAN instance.

   Parameters:
    filterNumber          - Standard Filter number to get filter configuration.
    stdMsgIDFilterElement - Pointer to Standard Filter Element configuration for storing filter configuration.

   Returns:
    Request status.
    true  - Request was successful.
    false - Request has failed.
*/
bool CAN1_StandardFilterElementGet(uint8_t filterNumber, can_sidfe_registers_t *stdMsgIDFilterElement)
{
    if ((filterNumber > 1) || (stdMsgIDFilterElement == NULL))
    {
        return false;
    }
    stdMsgIDFilterElement->CAN_SIDFE_0 = can1Obj.msgRAMConfig.stdMsgIDFilterAddress[filterNumber - 1].CAN_SIDFE_0;

    return true;
}


// *****************************************************************************
/* Function:
    bool CAN1_IsBusy(void)

   Summary:
    Returns the Peripheral busy status.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    None.

   Returns:
    true - Busy.
    false - Not busy.
*/
bool CAN1_IsBusy(void)
{
    if (can1Obj.state == CAN_STATE_IDLE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// *****************************************************************************
/* Function:
    void CAN1_CallbackRegister(CAN_CALLBACK callback, uintptr_t contextHandle)

   Summary:
    Sets the pointer to the function (and it's context) to be called when the
    given CAN's transfer events occur.

   Precondition:
    CAN1_Initialize must have been called for the associated CAN instance.

   Parameters:
    callback - A pointer to a function with a calling signature defined
    by the CAN_CALLBACK data type.

    context - A value (usually a pointer) passed (unused) into the function
    identified by the callback parameter.

   Returns:
    None.
*/
void CAN1_CallbackRegister(CAN_CALLBACK callback, uintptr_t contextHandle)
{
    if (callback == NULL)
    {
        return;
    }

    can1Obj.callback = callback;
    can1Obj.context = contextHandle;
}

// *****************************************************************************
/* Function:
    void CAN1_InterruptHandler(void)

   Summary:
    CAN1 Peripheral Interrupt Handler.

   Description:
    This function is CAN1 Peripheral Interrupt Handler and will
    called on every CAN1 interrupt.

   Precondition:
    None.

   Parameters:
    None.

   Returns:
    None.

   Remarks:
    The function is called as peripheral instance's interrupt handler if the
    instance interrupt is enabled. If peripheral instance's interrupt is not
    enabled user need to call it from the main while loop of the application.
*/

void CAN1_InterruptHandler(void)
{
    uint8_t length = 0;
    uint8_t rxgi = 0;
    can_rxf0e_registers_t *rxf0eFifo = NULL;
    can_rxf1e_registers_t *rxf1eFifo = NULL;
    uint32_t ir = CAN1_REGS->CAN_IR;

    
    /* Check if error occurred */
    if (ir & CAN_IR_BO_Msk)
    {
        CAN1_REGS->CAN_IR = CAN_IR_BO_Msk;
        can1Obj.state = CAN_STATE_ERROR;
        /* Client must call CAN1_ErrorGet function to get and clear errors */
        if (can1Obj.callback != NULL)
        {
            can1Obj.callback(can1Obj.context);
            can1Obj.state = CAN_STATE_IDLE;
        }
    }
    /* New Message in Rx FIFO 0 */
    if (ir & CAN_IR_RF0N_Msk)
    {   
        if(IsMaster())Can1_Param.TOTAL_RX_MASTER++;
        else Can1_Param.TOTAL_RX_AFTER_FILTER++;
        
        CAN1_REGS->CAN_IR = CAN_IR_RF0N_Msk;
        CAN1_REGS->CAN_IE &= (~CAN_IE_RF0NE_Msk);

        if (CAN1_REGS->CAN_RXF0S & CAN_RXF0S_F0FL_Msk)
        {            
            /* Read data from the Rx FIFO0 */
            rxgi = (uint8_t)((CAN1_REGS->CAN_RXF0S & CAN_RXF0S_F0GI_Msk) >> CAN_RXF0S_F0GI_Pos);
            rxf0eFifo = (can_rxf0e_registers_t *) ((uint8_t *)can1Obj.msgRAMConfig.rxFIFO0Address + rxgi * CAN1_RX_FIFO0_ELEMENT_SIZE);
          
            /* Get received identifier */
            if (rxf0eFifo->CAN_RXF0E_0 & CAN_RXF0E_0_XTD_Msk)
            {
                *can1Obj.rxId = rxf0eFifo->CAN_RXF0E_0 & CAN_RXF0E_0_ID_Msk;
            }
            else
            {
                *can1Obj.rxId = (rxf0eFifo->CAN_RXF0E_0 >> 18) & CAN_STD_ID_Msk;
            }
            /* Get received data length */
            length = CANDlcToLengthGet(((rxf0eFifo->CAN_RXF0E_1 & CAN_RXF0E_1_DLC_Msk) >> CAN_RXF0E_1_DLC_Pos));

            /* Copy data to user buffer */
            memcpy(can1Obj.rxBuffer, (uint8_t *)&rxf0eFifo->CAN_RXF0E_DATA, length);
            
            *can1Obj.rxsize = length;
            
            /* Ack the fifo position */
            CAN1_REGS->CAN_RXF0A = CAN_RXF0A_F0AI(rxgi);            
            can1Obj.state = CAN_STATE_TRANSFER_DONE;
        }
    }
    /* New Message in Rx FIFO 1 */
    if (ir & CAN_IR_RF1N_Msk)
    {
        CAN1_REGS->CAN_IR = CAN_IR_RF1N_Msk;
        CAN1_REGS->CAN_IE &= (~CAN_IE_RF1NE_Msk);

        if (CAN1_REGS->CAN_RXF1S & CAN_RXF1S_F1FL_Msk)
        {
            /* Read data from the Rx FIFO1 */
            rxgi = (uint8_t)((CAN1_REGS->CAN_RXF1S & CAN_RXF1S_F1GI_Msk) >> CAN_RXF1S_F1GI_Pos);
            rxf1eFifo = (can_rxf1e_registers_t *) ((uint8_t *)can1Obj.msgRAMConfig.rxFIFO1Address + rxgi * CAN1_RX_FIFO1_ELEMENT_SIZE);

            /* Get received identifier */
            if (rxf1eFifo->CAN_RXF1E_0 & CAN_RXF1E_0_XTD_Msk)
            {
                *can1Obj.rxId = rxf1eFifo->CAN_RXF1E_0 & CAN_RXF1E_0_ID_Msk;
            }
            else
            {
                *can1Obj.rxId = (rxf1eFifo->CAN_RXF1E_0 >> 18) & CAN_STD_ID_Msk;
            }

            /* Get received data length */
            length = CANDlcToLengthGet(((rxf1eFifo->CAN_RXF1E_1 & CAN_RXF1E_1_DLC_Msk) >> CAN_RXF1E_1_DLC_Pos));

            /* Copy data to user buffer */
            memcpy(can1Obj.rxBuffer, (uint8_t *)&rxf1eFifo->CAN_RXF1E_DATA, length);
            
            *can1Obj.rxsize = length;

            /* Ack the fifo position */
            CAN1_REGS->CAN_RXF1A = CAN_RXF1A_F1AI(rxgi);
            can1Obj.state = CAN_STATE_TRANSFER_DONE;
        }
    }

    /* TX Completed */
    if (ir & CAN_IR_TC_Msk)
    {
        CAN1_REGS->CAN_IR = CAN_IR_TC_Msk;
        CAN1_REGS->CAN_IE &= (~CAN_IE_TCE_Msk);
        for (uint8_t bufferIndex = 0; bufferIndex < (CAN1_TX_FIFO_BUFFER_SIZE/CAN1_TX_FIFO_BUFFER_ELEMENT_SIZE); bufferIndex++)
        {
            if ((CAN1_REGS->CAN_TXBTO & (1 << (bufferIndex & 0x1F))) &&
                (CAN1_REGS->CAN_TXBTIE & (1 << (bufferIndex & 0x1F))))
            {
                CAN1_REGS->CAN_TXBTIE &= ~(1 << (bufferIndex & 0x1F));
                can1Obj.state = CAN_STATE_TRANSFER_DONE;
            }
        }
    }

    /* TX FIFO is empty */
    if (ir & CAN_IR_TFE_Msk)
    {      
        if(IsMaster())Can1_Param.TOTAL_TX_MASTER++;
        else Can1_Param.TOTAL_TX_TO_MASTER++;
        
        CAN1_REGS->CAN_IR = CAN_IR_TFE_Msk;
        uint8_t getIndex = (uint8_t)((CAN1_REGS->CAN_TXFQS & CAN_TXFQS_TFGI_Msk) >> CAN_TXFQS_TFGI_Pos);
        uint8_t putIndex = (uint8_t)((CAN1_REGS->CAN_TXFQS & CAN_TXFQS_TFQPI_Msk) >> CAN_TXFQS_TFQPI_Pos);
        for (uint8_t fifoIndex = getIndex; ; fifoIndex++)
        {
            if (fifoIndex >= putIndex)
            {
                break;
            }
            can1Obj.state = CAN_STATE_TRANSFER_DONE;
        }
    }
    if (can1Obj.state == CAN_STATE_TRANSFER_DONE)
    {
        if (can1Obj.callback != NULL)
        {
            can1Obj.callback(can1Obj.context);
            //can1Obj.state = CAN_STATE_IDLE;// EL02 
        }
    }
}



/*******************************************************************************
 End of File
*/
