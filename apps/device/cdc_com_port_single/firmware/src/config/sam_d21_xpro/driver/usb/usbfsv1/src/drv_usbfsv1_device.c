/*******************************************************************************
  USB Device Driver Implementation of device mode operation routines

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usbfs_device.c

  Summary:
    USB Device Driver Dynamic Implementation of device mode operation routines

  Description:
    The USB device driver provides a simple interface to manage the USB modules
    on Microchip microcontrollers.  This file implements the interface routines
    for the USB driver when operating in device mode.

    While building the driver from source, ALWAYS use this file in the build if
    device mode operation is required.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip  Technology  Inc.   All  rights  reserved.

Microchip licenses to  you  the  right  to  use,  modify,  copy  and  distribute
Software only when embedded on a Microchip  microcontroller  or  digital  signal
controller  that  is  integrated  into  your  product  or  third  party  product
(pursuant to the  sublicense  terms  in  the  accompanying  license  agreement).

You should refer  to  the  license  agreement  accompanying  this  Software  for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
//DOM-IGNORE-END


#include "driver/usb/usbfsv1/src/drv_usbfsv1_local.h"
#include "driver/usb/usbfsv1/drv_usbfsv1.h"
//#include "system/debug/sys_debug.h"

#define COMPILER_WORD_ALIGNED         __attribute__((__aligned__(4)))


usb_descriptor_device_registers_t __attribute__((aligned(512))) usbDeviceEndpointDescriptorTable[DRV_USBFSV1_ENDPOINTS_NUMBER];

volatile uint32_t Counter = 0;

//interruptWasEnabled = system_interrupt_is_enabled(SYSTEM_INTERRUPT_MODULE_USB);
//system_interrupt_disable(SYSTEM_INTERRUPT_MODULE_USB);
//system_interrupt_enable(SYSTEM_INTERRUPT_MODULE_USB);
//
  //USB_0_IRQn               = 80, /**< 80 SAME54N19A Universal Serial Bus (USB): USB_EORSM_DNRSM, USB_EORST_RST, USB_LPMSUSP_DDISC, USB_LPM_DCONN, USB_MSOF, USB_RAMACER, USB_RXSTP_TXSTP_0, USB_RXSTP_TXSTP_1, USB_RXSTP_TXSTP_2, USB_RXSTP_TXSTP_3, USB_RXSTP_TXSTP_4, USB_RXSTP_TXSTP_5, USB_RXSTP_TXSTP_6, USB_RXSTP_TXSTP_7, USB_STALL0_STALL_0, USB_STALL0_STALL_1, USB_STALL0_STALL_2, USB_STALL0_STALL_3, USB_STALL0_STALL_4, USB_STALL0_STALL_5, USB_STALL0_STALL_6, USB_STALL0_STALL_7, USB_STALL1_0, USB_STALL1_1, USB_STALL1_2, USB_STALL1_3, USB_STALL1_4, USB_STALL1_5, USB_STALL1_6, USB_STALL1_7, USB_SUSPEND, USB_TRFAIL0_TRFAIL_0, USB_TRFAIL0_TRFAIL_1, USB_TRFAIL0_TRFAIL_2, USB_TRFAIL0_TRFAIL_3, USB_TRFAIL0_TRFAIL_4, USB_TRFAIL0_TRFAIL_5, USB_TRFAIL0_TRFAIL_6, USB_TRFAIL0_TRFAIL_7, USB_TRFAIL1_PERR_0, USB_TRFAIL1_PERR_1, USB_TRFAIL1_PERR_2, USB_TRFAIL1_PERR_3, USB_TRFAIL1_PERR_4, USB_TRFAIL1_PERR_5, USB_TRFAIL1_PERR_6, USB_TRFAIL1_PERR_7, USB_UPRSM, USB_WAKEUP */
  //USB_1_IRQn               = 81, /**< 81 SAME54N19A Universal Serial Bus (USB): USB_SOF_HSOF */
  //USB_2_IRQn               = 82, /**< 82 SAME54N19A Universal Serial Bus (USB): USB_TRCPT0_0, USB_TRCPT0_1, USB_TRCPT0_2, USB_TRCPT0_3, USB_TRCPT0_4, USB_TRCPT0_5, USB_TRCPT0_6, USB_TRCPT0_7 */
  //USB_3_IRQn               = 83, /**< 83 SAME54N19A Universal Serial Bus (USB): USB_TRCPT1_0, USB_TRCPT1_1, USB_TRCPT1_2, USB_TRCPT1_3, USB_TRCPT1_4, USB_TRCPT1_5, USB_TRCPT1_6, USB_TRCPT1_7 */
//
//#define NVIC_EnableIRQ              __NVIC_EnableIRQ
//#define NVIC_GetEnableIRQ           __NVIC_GetEnableIRQ
//#define NVIC_DisableIRQ             __NVIC_DisableIRQ
//
//#define SYSTEM_INTERRUPT_MODULE_USB USB_0_IRQn
//
//NVIC_GetEnableIRQ((IRQn_Type)SYSTEM_INTERRUPT_MODULE_USB);
//NVIC_DisableIRQ((IRQn_Type)SYSTEM_INTERRUPT_MODULE_USB);
//NVIC_EnableIRQ((IRQn_Type)SYSTEM_INTERRUPT_MODULE_USB);
//
//#define system_interrupt_is_enabled ()



/* Array of endpoint objects. Two objects per endpoint */
DRV_USBFSV1_DEVICE_ENDPOINT_OBJ gDrvUSBControlEndpoints[DRV_USBFSV1_INSTANCES_NUMBER] [2];

/* Array of endpoint objects. Two objects per endpoint */
DRV_USBFSV1_DEVICE_ENDPOINT_OBJ gDrvUSBNonControlEndpoints[DRV_USBFSV1_INSTANCES_NUMBER] [DRV_USBFSV1_ENDPOINTS_NUMBER - 1];

uint8_t DebugMessages1[] = "ISR-EPN TRCPT0\n\r";
uint8_t DebugMessages2[] = "ISR-EPN TRCPT0 IRP NULL\n\r";
uint8_t DebugMessages3[] = "ISR-EPN TRCPT0 IRP INVALID\n\r";
uint8_t DebugMessages4[] = "ISR-EPN TRCPT0 IRP COMPLETED\n\r";
uint8_t DebugMessages5[] = "ISR-EPN TRCPT0 IRP SHORT\n\r";
uint8_t DebugMessages6[] = "IRP-SUB EPN HTD\n\r";
uint8_t DebugMessages7[] = "IRP-SUB EPN HTD TRCPT0\n\r";
uint8_t DebugMessages8[] = "IRP-SUB EPN HTD TRCPT0 SHORT\n\r";
uint8_t DebugMessages9[] = "IRP-SUB EPN HTD TRCPT0 COMPLETED\n\r";
uint8_t DebugMessages10[] = "ISR-EPN TRFAIL0\n\r";
uint8_t DebugMessages11[] = "0C0\n\r";
uint8_t DebugMessages12[] = "0C1\n\r";
uint8_t DebugMessages13[] = "0RS\n\r";
uint8_t DebugMessages14[] = "ISR-EPN TRCPT1\n\r";


/* Array of device speeds. To map the speed as per bit values */
const USB_SPEED gDrvUSBFSV1DeviceSpeedMap[4] =
{
    USB_SPEED_FULL,
    USB_SPEED_HIGH,
    USB_SPEED_LOW,
    USB_SPEED_ERROR

};

/* Array of endpoint types. To map the endpoint type as per bit values */
const uint8_t gDrvUSBFSV1DeviceEndpointTypeMap[4][2] =
{
	{(uint8_t)USB_DEVICE_EPCFG_EPTYPE0(1), (uint8_t)USB_DEVICE_EPCFG_EPTYPE1(1)},
	{(uint8_t)USB_DEVICE_EPCFG_EPTYPE0(2), (uint8_t)USB_DEVICE_EPCFG_EPTYPE1(2)},
	{(uint8_t)USB_DEVICE_EPCFG_EPTYPE0(3), (uint8_t)USB_DEVICE_EPCFG_EPTYPE1(3)},
	{(uint8_t)USB_DEVICE_EPCFG_EPTYPE0(4), (uint8_t)USB_DEVICE_EPCFG_EPTYPE1(4)}
};

/*****************************************************
 * This structure is a pointer to a set of USB Driver
 * Device mode ISRFunctions. This set is exported to the
 * device layer when the device layer must use the
 * PIC32MX USB Controller.
 ******************************************************/

DRV_USB_DEVICE_INTERFACE gDrvUSBFSV1DeviceInterface =
{
    .open = DRV_USBFSV1_Open,
    .close = DRV_USBFSV1_Close,
    .eventHandlerSet = DRV_USBFSV1_ClientEventCallBackSet,
    .deviceAddressSet = DRV_USBFSV1_DEVICE_AddressSet,
    .deviceCurrentSpeedGet = DRV_USBFSV1_DEVICE_CurrentSpeedGet,
    .deviceSOFNumberGet = DRV_USBFSV1_DEVICE_SOFNumberGet,
    .deviceAttach = DRV_USBFSV1_DEVICE_Attach,
    .deviceDetach = DRV_USBFSV1_DEVICE_Detach,
    .deviceEndpointEnable = DRV_USBFSV1_DEVICE_EndpointEnable,
    .deviceEndpointDisable = DRV_USBFSV1_DEVICE_EndpointDisable,
    .deviceEndpointStall = DRV_USBFSV1_DEVICE_EndpointStall,
    .deviceEndpointStallClear = DRV_USBFSV1_DEVICE_EndpointStallClear,
    .deviceEndpointIsEnabled = DRV_USBFSV1_DEVICE_EndpointIsEnabled,
    .deviceEndpointIsStalled = DRV_USBFSV1_DEVICE_EndpointIsStalled,
    .deviceIRPSubmit = DRV_USBFSV1_DEVICE_IRPSubmit,
    .deviceIRPCancel = DRV_USBFSV1_DEVICE_IRPCancel,
    .deviceIRPCancelAll = DRV_USBFSV1_DEVICE_IRPCancelAll,
    .deviceRemoteWakeupStop = DRV_USBFSV1_DEVICE_RemoteWakeupStop,
    .deviceRemoteWakeupStart = DRV_USBFSV1_DEVICE_RemoteWakeupStart,
    .deviceTestModeEnter = NULL
};

// *****************************************************************************
/* Function:
    _DRV_USBFSV1_DEVICE_Initialize(DRV_USBFSV1_OBJ * drvObj, SYS_MODULE_INDEX index)

  Summary:
    This function is called when the driver is initialized for device mode
    operation.

  Description:
    This function is called when the driver is initialized for device mode
    operation. The function enables USB_OTG_INT_SESSION_VALID interrupt to
    detect VBUS session valid\invalid scenario. All the other interrupts will be
    enabled after the device has been attached (DRV_USBFSV1_DEVICE_Attach()
    function will be called)

  Remarks:
    See drv_usbfs.h for usage information.
*/

void _DRV_USBFSV1_DEVICE_Initialize
(
    DRV_USBFSV1_OBJ * drvObj,
    SYS_MODULE_INDEX index
)

{
    uint8_t loopIndex;

    /* Point the objects for control endpoint. It is a bidirectional
     * endpoint, so only one object is needed */

    drvObj->deviceEndpointObj[0] = &gDrvUSBControlEndpoints[index][0];

    /* Point the objects for non control endpoints.
     * They are unidirectional endpoints, so multidimensional
     * array with one object per endpoint direction */

    for(loopIndex = 1; loopIndex < DRV_USBFSV1_ENDPOINTS_NUMBER ; loopIndex++)
    {
        drvObj->deviceEndpointObj[loopIndex] = &gDrvUSBNonControlEndpoints[index][loopIndex - 1];
    }

    /* Initialize device specific flags */
    drvObj->vbusIsValid = false;
    drvObj->isAttached = false;
//    drvObj->isSuspended = false;

}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_DEVICE_AddressSet(DRV_HANDLE handle, uint8_t address)

  Summary:
    This function will set the USB module address that is obtained from the Host.

  Description:
    This function will set the USB module address  that  is  obtained  from  the
    Host in a setup transaction. The address is obtained from  the  SET_ADDRESS
    command issued by the Host. The  primary  (first)  client  of  the  driver
    uses this function to set the module's USB address after decoding the  setup
    transaction from the Host.

  Remarks:
    See drv_usbfs.h for usage information.
*/

void DRV_USBFSV1_DEVICE_AddressSet
(
    DRV_HANDLE handle,
    uint8_t address
)

{
    usb_registers_t * usbID;                        /* USB instance pointer */
    DRV_USBFSV1_OBJ * hDriver;            /* USB driver object pointer */

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_AddressSet().");
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Set and Enable the device address */
		usbID->DEVICE.USB_DADD = USB_DEVICE_DADD_ADDEN_Msk | address;

    }
}

// *****************************************************************************
/* Function:
    USB_SPEED DRV_USBFSV1_DEVICE_CurrentSpeedGet(DRV_HANDLE handle)

  Summary:
    This function returns the USB speed at which the device is operating.

  Description:
    This function returns the USB speed at which the device is operating.

  Remarks:
    See drv_usbfs.h for usage information.
*/

USB_SPEED DRV_USBFSV1_DEVICE_CurrentSpeedGet
(
    DRV_HANDLE handle
)

{
    DRV_USBFSV1_OBJ * hDriver;            /* USB driver object pointer */
    USB_SPEED retVal = USB_SPEED_ERROR;

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_CurrentSpeedGet().");
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        retVal = hDriver->deviceSpeed;
        //retVal = USB_SPEED_FULL;

    }

    /* Return the speed */
    return retVal;
}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_DEVICE_RemoteWakeup_Start(DRV_HANDLE handle)

  Summary:
    This function causes the device to start Remote Wakeup Signaling on the
    bus.

  Description:
    This function causes the device to start Remote Wakeup Signaling on the
    bus. This function should be called when the device, presently placed in
    suspend mode by the Host, wants to be wakeup. Note that the device can do
    this only when the Host has enabled the device's Remote Wakeup capability.

  Remarks:
    See drv_usbfs.h for usage information.
*/

void DRV_USBFSV1_DEVICE_RemoteWakeupStart
(
    DRV_HANDLE handle
)

{

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_RemoteWakeupStart().");
    }
    else
    {
        //Not supported as of now - do nothing
    }
}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_DEVICE_RemoteWakeupStop(DRV_HANDLE handle)

  Summary:
    This function causes the device to stop the Remote Wakeup Signaling on the
    bus.

  Description:
    This function causes the device to stop Remote Wakeup Signaling on the bus.
    This function should be called after the DRV_USBFSV1_DEVICE_RemoteWakeupStart
    function was called to start the Remote Wakeup signaling on the bus.

  Remarks:
    See drv_usbfs.h for usage information.
*/

void DRV_USBFSV1_DEVICE_RemoteWakeupStop
(
    DRV_HANDLE handle
)

{

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_RemoteWakeupStop().");
    }
    else
    {
        //Not supported as of now - do nothing
    }
}

// *****************************************************************************
/* Function:
    void DRV_USBFSV1_DEVICE_Attach(DRV_HANDLE handle)

  Summary:
    This function will enable the attach signaling resistors on the D+ and D-
    lines thus letting the USB Host know that a device has been attached on the
    bus.

  Description:
    This function enables the pull-up resistors on the D+ or D- lines thus
    letting the USB Host know that a device has been attached on the bus . This
    function should be called when the driver client is ready  to  receive
    communication  from  the  Host (typically after all initialization is
    complete). The USB 2.0 specification requires VBUS to be detected before the
    data line pull-ups are enabled. The application must ensure the same.

  Remarks:
    See drv_usbfs.h for usage information.
*/

void DRV_USBFSV1_DEVICE_Attach
(
    DRV_HANDLE handle
)

{

    usb_registers_t * usbID;                        /* USB instance pointer */
    DRV_USBFSV1_OBJ * hDriver;            /* USB driver object pointer */

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_Attach().");
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Update the driver flag indicating attach */
        hDriver->isAttached = true;

        /* Enables all interrupts except RESUME. RESUMEIF will be enabled only
         * on getting SUSPEND */
        usbID->DEVICE.USB_INTENSET |= (USB_DEVICE_INTENSET_SUSPEND_Msk | USB_DEVICE_INTENSET_SOF_Msk | USB_DEVICE_INTENSET_EORST_Msk | USB_DEVICE_INTENSET_WAKEUP_Msk | USB_DEVICE_INTENSET_RAMACER_Msk);

        /* Enable the USB device by clearing the . This function
         * also enables the D+ pull up resistor.  */
        usbID->DEVICE.USB_CTRLB &= ~USB_DEVICE_CTRLB_DETACH_Msk;
    }
}

// *****************************************************************************
/* Function:
      void DRV_USBFSV1_DEVICE_Detach(DRV_HANDLE handle)

  Summary:
    This function will disable the attach signaling resistors on the D+ and D-
    lines thus letting the USB Host know that the device has detached from the
    bus.

  Description:
    This function disables the pull-up resistors on the D+ or D- lines. This
    function should be called when the application wants to disconnect the
    device  from  the bus (typically to implement a soft detach or switch  to
    Host  mode operation).  A self-powered device should be detached from the
    bus when the VBUS is not valid.

  Remarks:
    See drv_usbfs.h for usage information.
*/

void DRV_USBFSV1_DEVICE_Detach
(
    DRV_HANDLE handle
)

{

    usb_registers_t * usbID;                        /* USB instance pointer */
    DRV_USBFSV1_OBJ * hDriver;            /* USB driver object pointer */

    /* Check if the handle is invalid, if so return without any action */
    if(DRV_HANDLE_INVALID == handle)
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_Detach().");
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Update the driver flag indicating detach */
        hDriver->isAttached = false;

        /* Clear all interrupts */
        //TODO:

        /* Reset the operating mode */
        usbID->DEVICE.USB_CTRLB |= USB_DEVICE_CTRLB_DETACH_Msk;
    }
}

// *****************************************************************************
/* Function:
    void _DRV_USBFSV1_DEVICE_EndpointObjectEnable
    (
        DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObject,
        uint16_t endpointSize,
        USB_TRANSFER_TYPE endpointType,
        USB_BUFFER_DATA01 dataToggle
     )

  Summary:
    This helper function populates the software endpoint object with provided
    data.

  Description:
    This helper function populates the software endpoint object with provided
    data.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _DRV_USBFSV1_DEVICE_EndpointObjectEnable
(
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObject,
    uint8_t * endpointBuffPtr,
    uint16_t endpointSize,
    USB_TRANSFER_TYPE endpointType,
    USB_BUFFER_DATA01 dataToggle
)

{
    /* This is a helper function */
    endpointObject->endpointBufferPtr = endpointBuffPtr;
    endpointObject->nextDataToggle  = dataToggle;
    endpointObject->irpQueue        = NULL;
    endpointObject->maxPacketSize   = endpointSize;
    endpointObject->endpointType    = endpointType;
    endpointObject->endpointState  |= DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;

}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_EndpointEnable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection,
        USB_TRANSFER_TYPE endpointType,
        uint16_t endpointSize
    )

  Summary:
    This function enables an endpoint for the specified direction and endpoint
    size.

  Description:
    This function enables an endpoint for the specified direction and endpoint
    size. The function will enable the endpoint for communication in one
    direction at a time. It must be called twice if the endpoint is required to
    communicate in both the directions, with the exception of control endpoints.
    If the endpoint type is a control endpoint, the endpoint is always
    bidirectional and the function needs to be called only once.

    The size of the endpoint must match the wMaxPacketSize reported in the
    endpoint descriptor for this endpoint. A transfer that is scheduled over
    this endpoint will be scheduled in wMaxPacketSize transactions. The function
    does not check if the endpoint is already in use. It is the client's
    responsibility to make sure that a endpoint is not accidentally reused.

  Remarks:
    See drv_usbfs.h for usage information.
*/


USB_ERROR DRV_USBFSV1_DEVICE_EndpointEnable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection,
    USB_TRANSFER_TYPE endpointType,
    uint16_t endpointSize
)

{
    /* This function can be called from from the USB ISR. Because an endpoint
     * can be owned by one client only, we don't need mutex protection in this
     * function */

    uint8_t direction;
    uint8_t endpoint;
    DRV_USBFSV1_OBJ * hDriver;
    usb_registers_t * usbID;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    USB_ERROR retVal = USB_ERROR_NONE;
    uint8_t bufferSize = 0;                            /* Buffer size */
    uint16_t defaultEndpointSize = 8;                /* Default size of Endpoint */


    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;
    direction = ((endpointAndDirection & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);

    if(endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER)
    {
        /* Endpoint number is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Unsupported endpoint in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if(endpointSize < 8 || endpointSize > 1024)
    {
        /* Endpoint size is invalid, return with appropriate error message */
        retVal = USB_ERROR_HOST_ENDPOINT_INVALID;
    }
    else if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        /* Find upper 2 power number of endpointSize */
        if(endpointSize)
        {
            while (defaultEndpointSize < endpointSize)
            {
                bufferSize++;
                defaultEndpointSize <<= 1;
            }
        }

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(endpointType == USB_TRANSFER_TYPE_CONTROL)
        {
            /* There are two endpoint objects for a control endpoint.
             * Enable the first endpoint object */

            _DRV_USBFSV1_DEVICE_EndpointObjectEnable
            (
                endpointObj, hDriver->endpointBufferPtr[0][0], endpointSize, USB_TRANSFER_TYPE_CONTROL, USB_BUFFER_DATA0//, nextPingPong
            );

            endpointObj++;

             /* Enable the second endpoint object */

            _DRV_USBFSV1_DEVICE_EndpointObjectEnable
            (
                endpointObj, hDriver->endpointBufferPtr[0][1], endpointSize, USB_TRANSFER_TYPE_CONTROL, USB_BUFFER_DATA1//, nextPingPong
            );

            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPCFG = 0;

            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPCFG = (USB_DEVICE_EPCFG_EPTYPE0(1) | USB_DEVICE_EPCFG_EPTYPE1(1));

            hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_SIZE_Msk;
            hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_SIZE_Msk;

            hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_SIZE(bufferSize);
            hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_SIZE(bufferSize);

            hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_ADDR = (uint32_t) hDriver->endpointBufferPtr[0][0];
            hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_ADDR = (uint32_t) hDriver->endpointBufferPtr[0][1];

            if (true == USB_DEVICE_AUTO_ZLP)
            {
                hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
                hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
            }
            else
            {
                hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
                hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
            }

            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = (USB_DEVICE_EPINTENSET_RXSTP_Msk | USB_DEVICE_EPINTENSET_TRCPT0_Msk | USB_DEVICE_EPINTENSET_TRCPT1_Msk);
        }
        else
        {
            /* Enable the non-zero endpoint object */

            _DRV_USBFSV1_DEVICE_EndpointObjectEnable
            (
                endpointObj, hDriver->endpointBufferPtr[endpoint][direction], endpointSize, endpointType, direction
            );


            if(direction == USB_DATA_DIRECTION_DEVICE_TO_HOST)
            {

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPCFG &= ~USB_DEVICE_EPCFG_EPTYPE1_Msk;

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPCFG |= (uint8_t) gDrvUSBFSV1DeviceEndpointTypeMap[endpointType][direction];

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK1RDY_Msk;

            }
            else
            {

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPCFG &= ~USB_DEVICE_EPCFG_EPTYPE0_Msk;

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPCFG |= (uint8_t) gDrvUSBFSV1DeviceEndpointTypeMap[endpointType][direction];

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0_Msk;
            }

            usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPCFG |= (uint8_t) gDrvUSBFSV1DeviceEndpointTypeMap[endpointType][direction];

            hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_SIZE(bufferSize);

            hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_ADDR = (uint32_t) hDriver->endpointBufferPtr[endpoint][direction];

            if (true == USB_DEVICE_AUTO_ZLP)
            {
                hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
            }
            else
            {
                hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_AUTO_ZLP_Msk;
            }
        }
    }
    return(retVal);
}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_EndpointDisable
    (
        DRV_HANDLE handle,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function disables an endpoint.

  Description:
    This function disables an endpoint. If the endpoint type is a control
    endpoint type, both directions are disabled. For non-control endpoints, the
    function disables the specified direction only. The direction to be disabled
    is specified by the Most Significant Bit (MSB) of the endpointAndDirection
    parameter.

  Remarks:
    See drv_usbfs.h for usage information.
*/

USB_ERROR DRV_USBFSV1_DEVICE_EndpointDisable
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)
{
    /* This routine disables the specified endpoint.  It does not check if there
     * is any ongoing communication on the bus through the endpoint */

    uint8_t direction;                          /* Endpoint Direction */
    uint8_t endpoint;
    DRV_USBFSV1_OBJ * hDriver;
    usb_registers_t * usbID;
    uint32_t loopIndex;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    USB_ERROR retVal = USB_ERROR_NONE;
    bool interruptWasEnabled0 = false;           /* To track interrupt state */
    bool interruptWasEnabled1 = false;           /* To track interrupt state */
    bool interruptWasEnabled2 = false;           /* To track interrupt state */
    bool interruptWasEnabled3 = false;           /* To track interrupt state */

    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;
    direction = ((endpointAndDirection & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);
    //TODO: direction based disabling of endpoint is still pending
    
    direction = direction;
    
    
    if((endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER) && (endpointAndDirection != DRV_USBFSV1_DEVICE_ENDPOINT_ALL))
    {
        /* Endpoint number is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Unsupported endpoint in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        if(hDriver->isInInterruptContext == false)
        {
            /* Disable  the USB Interrupt as this is not called inside ISR */
//            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_0_IRQn);
//            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_1_IRQn);
//            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_2_IRQn);
//            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_3_IRQn);
            
            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_IRQn);
			
//			NVIC_DisableIRQ(USB_0_IRQn);
//			NVIC_DisableIRQ(USB_1_IRQn);
//			NVIC_DisableIRQ(USB_2_IRQn);
//			NVIC_DisableIRQ(USB_3_IRQn);
            
			NVIC_DisableIRQ(USB_IRQn);
			NVIC_DisableIRQ(USB_IRQn);
			NVIC_DisableIRQ(USB_IRQn);
			NVIC_DisableIRQ(USB_IRQn);
        }

        if(endpointAndDirection == DRV_USBFSV1_DEVICE_ENDPOINT_ALL)
        {
            endpointObj = hDriver->deviceEndpointObj[0];
            endpointObj->endpointState  &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;
            endpointObj++;
            endpointObj->endpointState  &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;
            endpointObj++;

            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPCFG = 0;

            for(loopIndex = 1; loopIndex < DRV_USBFSV1_ENDPOINTS_NUMBER; loopIndex ++)
            {
                endpointObj = hDriver->deviceEndpointObj[loopIndex];
                /* Update the endpoint database */
                endpointObj->endpointState  &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;

                usbID->DEVICE.DEVICE_ENDPOINT[loopIndex].USB_EPCFG = 0;
            }
        }
        else
        {
            if(endpoint == 0)
            {
                endpointObj = hDriver->deviceEndpointObj[0];
                endpointObj->endpointState  &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;
                endpointObj++;
                endpointObj->endpointState  &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;
                endpointObj++;
                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPCFG = 0;
            }
            else
            {
                endpointObj = hDriver->deviceEndpointObj[endpoint];

                /* Update the endpoint database */
                endpointObj->endpointState  &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED;

                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPCFG = 0;
            }
        }

        if(hDriver->isInInterruptContext == false)
        {
//	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_0_IRQn);
//			if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_1_IRQn);
//			if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_2_IRQn);
//			if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_3_IRQn);
            
	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_IRQn);
			if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_IRQn);
			if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_IRQn);
			if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_IRQn);
        }
    }

    return(retVal);
}

// *****************************************************************************
/* Function:
    bool DRV_USBFSV1_DEVICE_EndpointIsEnabled
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function returns the enable/disable status of the specified endpoint
    and direction.

  Description:
    This function returns the enable/disable status of the specified endpoint
    and direction.

  Remarks:
    See drv_usbfs.h for usage information.
*/

bool DRV_USBFSV1_DEVICE_EndpointIsEnabled
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)
{

    uint8_t endpoint;
    DRV_USBFSV1_OBJ * hDriver;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    USB_ERROR retVal = false;

    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;

    if(endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER)
    {
        /* Endpoint number is invalid, return with appropriate error message */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Unsupported endpoint in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
                SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if((endpointObj->endpointState & DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED) == 0)
        {
            retVal = false;
        }
        else
        {
            /* return true */
        }
    }

    return(retVal);
}

// *****************************************************************************
/* Function:
    bool DRV_USBFSV1_DEVICE_EndpointIsStalled
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function returns the stall status of the specified endpoint and
    direction.

  Description:
    This function returns the stall status of the specified endpoint and
    direction.

  Remarks:
    See drv_usbfs.h for usage information.
*/

bool DRV_USBFSV1_DEVICE_EndpointIsStalled
(
    DRV_HANDLE client,
    USB_ENDPOINT endpointAndDirection
)
{
    DRV_USBFSV1_OBJ * hDriver;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    uint8_t endpoint = endpointAndDirection & 0xF;
    bool retVal = false;

    /* Check if the handle is valid */
    if((DRV_HANDLE_INVALID != client) && (client != (DRV_HANDLE)(NULL)))
    {
        hDriver = ((DRV_USBFSV1_OBJ *)client);

        if(endpoint < DRV_USBFSV1_ENDPOINTS_NUMBER)
        {

            endpointObj = hDriver->deviceEndpointObj[endpoint];

            if((endpointObj->endpointState & DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED) != 0)
            {
                retVal = true;
            }
        }
        else
        {
            SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFS Driver: Endpoint is invalid");
        }
    }
    else
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFS Driver: Driver Handle is invalid");
    }

    return(retVal);
}

// *****************************************************************************
/* Function:
    void _DRV_USBFSV1_DEVICE_IRPQueueFlush
    (
        DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObject
        USB_DEVICE_IRP_STATUS status
    )

  Summary:
    This function flushes all the IRPs in the queue.

  Description:
    This function flushes all the IRPs in the queue. Function scans for all the
    IRPs on the endpoint queue and cancels them all. status indicate the abort
    status to be returned when the IRP callback is invoked.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/

void _DRV_USBFSV1_DEVICE_IRPQueueFlush
(
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObject,
    USB_DEVICE_IRP_STATUS status
)
{
    USB_DEVICE_IRP_LOCAL * iterator = NULL;

    if(endpointObject != NULL)
    {
        /* Check if any IRPs are assigned on this endpoint and abort them */

        if(endpointObject->irpQueue != NULL)
        {
            /* Scan for all the IRPs on this endpoint Cancel the IRP and
             * deallocate driver IRP objects */

            iterator = endpointObject->irpQueue;
            while(iterator != NULL)
            {
                iterator->status = status;
                if(iterator->callback != NULL)
                {
                    iterator->callback((USB_DEVICE_IRP *)iterator);
                }
                iterator = iterator->next;
            }
        }

        /* Set the head pointer to NULL */
        endpointObject->irpQueue = NULL;
    }
}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_IRPCancelAll
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    Dynamic implementation of DRV_USBFSV1_DEVICE_IRPCancelAll client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USBFSV1_DEVICE_IRPCancelAll client
    interface function for USB device.  Function checks the validity of the
    input arguments and on success cancels all the IRPs on the specific endpoint
    object queue.

  Remarks:
    See drv_usbfs.h for usage information.
*/

USB_ERROR DRV_USBFSV1_DEVICE_IRPCancelAll
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)

{
    uint8_t endpoint;
    DRV_USBFSV1_OBJ * hDriver;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    bool interruptWasEnabled0 = false;           /* To track interrupt state */
    bool interruptWasEnabled1 = false;           /* To track interrupt state */
    bool interruptWasEnabled2 = false;           /* To track interrupt state */
    bool interruptWasEnabled3 = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;

    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;

    if(endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER)
    {
        /* Endpoint number is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Unsupported endpoint in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_EndpointEnable().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;

        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(hDriver->isInInterruptContext == false)
        {
            /* Disable  the USB Interrupt as this is not called inside ISR */
//            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_0_IRQn);
//            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_1_IRQn);
//            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_2_IRQn);
//            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_3_IRQn);
            
            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_IRQn);
            
//            NVIC_DisableIRQ(USB_0_IRQn);
//            NVIC_DisableIRQ(USB_1_IRQn);
//            NVIC_DisableIRQ(USB_2_IRQn);
//            NVIC_DisableIRQ(USB_3_IRQn);
            
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
        }

        /* Flush the endpoint */
        _DRV_USBFSV1_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED);

        if(hDriver->isInInterruptContext == false)
        {
//	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_0_IRQn);
//	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_1_IRQn);
//	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_2_IRQn);
//	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_3_IRQn);
            
	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_IRQn);
        }
    }

    return(retVal);
}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_IRPCancel
    (
        DRV_HANDLE client,
        USB_DEVICE_IRP * irp
    )

  Summary:
    Dynamic implementation of DRV_USBFSV1_DEVICE_IRPCancel client interface
    function.

  Description:
    This is the dynamic implementation of DRV_USBFSV1_DEVICE_IRPCancel client
    interface function for USB device.  Function checks the validity of the
    input arguments and on success cancels  the specific IRP.  An IRP that was
    in the queue but that has been processed yet will be canceled successfully
    and the IRP callback function will be called from this function with
    USB_DEVICE_IRP_STATUS_ABORTED status. The application can release the data
    buffer memory used by the IRP when this callback occurs.  If the IRP was in
    progress (a transaction in on the bus) when the cancel function was called,
    the IRP will be canceled only when an ongoing or the next transaction has
    completed. The IRP callback function will then be called in an interrupt
    context. The application should not release the related data buffer unless
    the IRP callback has occurred.

  Remarks:
    See drv_usbfs.h for usage information.
*/

USB_ERROR DRV_USBFSV1_DEVICE_IRPCancel
(
    DRV_HANDLE handle,
    USB_DEVICE_IRP * irp
)

{
    DRV_USBFSV1_OBJ * hDriver;
    USB_DEVICE_IRP_LOCAL * irpToCancel;
    bool interruptWasEnabled0 = false;           /* To track interrupt state */
    bool interruptWasEnabled1 = false;           /* To track interrupt state */
    bool interruptWasEnabled2 = false;           /* To track interrupt state */
    bool interruptWasEnabled3 = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;

    /* Check if the handle is valid */
    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_IRPCancel().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else if(irp == NULL)
    {
        /* IRP is NULL, send appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: IRP is invalid in DRV_USBFSV1_DEVICE_IRPCancel().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = ((DRV_USBFSV1_OBJ *)handle);

        irpToCancel = (USB_DEVICE_IRP_LOCAL *) irp;

        if(hDriver->isInInterruptContext == false)
        {
	        /* Disable  the USB Interrupt as this is not called inside ISR */
//	        interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_0_IRQn);
//	        interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_1_IRQn);
//	        interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_2_IRQn);
//	        interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_3_IRQn);
            
	        interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_IRQn);
	        interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_IRQn);
	        interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_IRQn);
	        interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_IRQn);
	        
//	        NVIC_DisableIRQ(USB_0_IRQn);
//	        NVIC_DisableIRQ(USB_1_IRQn);
//	        NVIC_DisableIRQ(USB_2_IRQn);
//	        NVIC_DisableIRQ(USB_3_IRQn);
            
	        NVIC_DisableIRQ(USB_IRQn);
	        NVIC_DisableIRQ(USB_IRQn);
	        NVIC_DisableIRQ(USB_IRQn);
	        NVIC_DisableIRQ(USB_IRQn);
        }

        if(irpToCancel->status <= USB_DEVICE_IRP_STATUS_COMPLETED_SHORT)
        {
            /* This IRP has either completed or has been aborted.*/
            retVal = USB_ERROR_PARAMETER_INVALID;
        }
        else
        {
            /* The code will come here both when the IRP is NOT the 1st
                * in queue as well as when it is at the HEAD. We will change
                * the IRP status for either scenario but will give the callback
                * only if it is NOT at the HEAD of the queue.
                *
                * What it means for HEAD IRP case is it will be caught in USB
                * ISR and will be further processed in ISR. This is done to
                * make sure that the user cannot release the IRP buffer before
                * ABORT callback*/

            /* Mark the IRP status as aborted */
            irpToCancel->status = USB_DEVICE_IRP_STATUS_ABORTED;

            /* No data for this IRP was sent or received */
            irpToCancel->size = 0;

            if(irpToCancel->previous != NULL)
            {
                /* This means this is not the HEAD IRP in the IRP queue.
                    Can be removed from the endpoint object queue safely.*/
                irpToCancel->previous->next = irpToCancel->next;

                if(irpToCancel->next != NULL)
                {
                    /* If this is not the last IRP in the queue then update
                        the previous link connection for the next IRP */
                    irpToCancel->next->previous = irpToCancel->previous;
                }

                irpToCancel->previous = NULL;
                irpToCancel->next = NULL;

                if(irpToCancel->callback != NULL)
                {
                    irpToCancel->callback((USB_DEVICE_IRP *) irpToCancel);
                }
            }
        }

        if(hDriver->isInInterruptContext == false)
        {
//	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_0_IRQn);
//	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_1_IRQn);
//	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_2_IRQn);
//	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_3_IRQn);
            
	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_IRQn);
        }
    }

    return retVal;
}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_EndpointStall
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function stalls an endpoint in the specified direction.

  Description:
    This function stalls an endpoint in the specified direction.

  Remarks:
    See drv_usbfs.h for usage information.
*/

USB_ERROR DRV_USBFSV1_DEVICE_EndpointStall
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)
{
    uint8_t direction;
    uint8_t endpoint;
    DRV_USBFSV1_OBJ * hDriver;
    usb_registers_t * usbID;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    bool interruptWasEnabled0 = false;           /* To track interrupt state */
    bool interruptWasEnabled1 = false;           /* To track interrupt state */
    bool interruptWasEnabled2 = false;           /* To track interrupt state */
    bool interruptWasEnabled3 = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;
    //TODO: direction based stall of endpoint is still pending


    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;
    direction = ((endpointAndDirection & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);

    if(endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER)
    {
        /* Endpoint number is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Unsupported endpoint in DRV_USBFSV1_DEVICE_EndpointStall().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_EndpointStall().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        if(hDriver->isInInterruptContext == false)
        {
            // Sundar: Add Mutex grab later
            /* Disable  the USB Interrupt as this is not called inside ISR */
//            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_0_IRQn);
//            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_1_IRQn);
//            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_2_IRQn);
//            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_3_IRQn);
            
            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_IRQn);
            
//            NVIC_DisableIRQ(USB_0_IRQn);
//            NVIC_DisableIRQ(USB_1_IRQn);
//            NVIC_DisableIRQ(USB_2_IRQn);
//            NVIC_DisableIRQ(USB_3_IRQn);
            
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
        }

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

            //if(!(hDriver->isInInterruptContext))
            //{
//// Sundar:     if(OSAL_MUTEX_Lock(&hDriver->mutexID, OSAL_WAIT_FOREVER) == OSAL_RESULT_TRUE)
                //{
                    ///* Disable the interrupt */
                    //interruptWasEnabled = _DRV_USBFSV1_InterruptSourceDisable(hDriver->interruptSource);
                //}
//// Sundar:                 else
                //{
                    ///* There was an error in getting the mutex */
                    SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFS Driver: Mutex lock failed");
//// Sundar:                     returnValue = USB_ERROR_OSAL_FUNCTION;
                //}
            //}

        if(endpoint == 0)
        {
            /* For zero endpoint we stall both directions */

            _DRV_USBFSV1_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

            endpointObj->endpointState |= DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED;

            endpointObj++;

            _DRV_USBFSV1_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

            endpointObj->endpointState |= DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED;

            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_STALLRQ1_Msk;

            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_STALLRQ0_Msk;
        }
        else
        {
            /* For non zero endpoints we stall the specified direction.
                * Get the endpoint object. */

            _DRV_USBFSV1_DEVICE_IRPQueueFlush(endpointObj, USB_DEVICE_IRP_STATUS_ABORTED_ENDPOINT_HALT);

            endpointObj->endpointState |= DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED;

            if(direction == USB_DATA_DIRECTION_DEVICE_TO_HOST)
            {
                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_STALLRQ1_Msk;
            }
            else
            {
                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_STALLRQ0_Msk;
            }
        }

        /* Restore the interrupt enable status if this was modified. */
        if(hDriver->isInInterruptContext == false)
        {
//	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_0_IRQn);
//	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_1_IRQn);
//	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_2_IRQn);
//	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_3_IRQn);
            
	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_IRQn);
            //OSAL_MUTEX_Unlock(&hDriver->mutexID);
        }
    }

    return(retVal);
}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_EndpointStallClear
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection
    )

  Summary:
    This function clears the stall on an endpoint in the specified direction.

  Description:
    This function clears the stall on an endpoint in the specified direction.

  Remarks:
    See drv_usbfs.h for usage information.
*/

USB_ERROR DRV_USBFSV1_DEVICE_EndpointStallClear
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection
)
{
    uint8_t direction;
    uint8_t endpoint;
    DRV_USBFSV1_OBJ * hDriver;
    usb_registers_t * usbID;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    bool interruptWasEnabled0 = false;           /* To track interrupt state */
    bool interruptWasEnabled1 = false;           /* To track interrupt state */
    bool interruptWasEnabled2 = false;           /* To track interrupt state */
    bool interruptWasEnabled3 = false;           /* To track interrupt state */
    USB_ERROR retVal = USB_ERROR_NONE;
    //TODO: direction based stall clearing of endpoint is still pending


    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;
    direction = ((endpointAndDirection & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);


    if(endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER)
    {
        /* Endpoint number is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Unsupported endpoint in DRV_USBFSV1_DEVICE_EndpointStallClear().");

        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_EndpointStallClear().");

        retVal = USB_ERROR_PARAMETER_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;

        if(hDriver->isInInterruptContext == false)
        {
            //TODO: Add Mutex grab later
            /* Disable  the USB Interrupt as this is not called inside ISR */
//            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_0_IRQn);
//            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_1_IRQn);
//            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_2_IRQn);
//            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_3_IRQn);
            
            interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_IRQn);
            interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_IRQn);
            
//            NVIC_DisableIRQ(USB_0_IRQn);
//            NVIC_DisableIRQ(USB_1_IRQn);
//            NVIC_DisableIRQ(USB_2_IRQn);
//            NVIC_DisableIRQ(USB_3_IRQn);
            
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
            NVIC_DisableIRQ(USB_IRQn);
        }

        /* Get the endpoint object */
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if((endpoint != 0) && (direction == USB_DATA_DIRECTION_DEVICE_TO_HOST))
        {
            if(usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUS & USB_DEVICE_EPSTATUSSET_STALLRQ1_Msk)
            {
                // Remove stall request
                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_STALLRQ1_Msk;
                if (usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_STALL1_Msk)
                {
                    usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_STALL1_Msk;
                    // The Stall has occurred, then reset data toggle
                    usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSSET_DTGLIN_Msk;
                }
            }
        }
        else
        {
            if(usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUS & USB_DEVICE_EPSTATUSSET_STALLRQ0_Msk)
            {
                // Remove stall request
                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_STALLRQ0_Msk;
                if (usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_STALL0_Msk)
                {
                    usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_STALL0_Msk;
                    // The Stall has occurred, then reset data toggle
                    usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSSET_DTGLOUT_Msk;
                }
            }
        }

        if((endpoint == 0) && (direction == USB_DATA_DIRECTION_DEVICE_TO_HOST))
        {
            endpointObj->nextDataToggle = USB_BUFFER_DATA1;
        }
        else
        {
            endpointObj->nextDataToggle = USB_BUFFER_DATA0;
        }

        /* Restore the interrupt enable status if this was modified. */
        if(hDriver->isInInterruptContext == false)
        {
//	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_0_IRQn);
//	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_1_IRQn);
//	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_2_IRQn);
//	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_3_IRQn);
            
	        if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_IRQn);
	        if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_IRQn);

            //OSAL_MUTEX_Unlock(&hDriver->mutexID);
        }
    }

    return(retVal);
}

// *****************************************************************************
/* Function:
    uint16_t DRV_USBFSV1_DEVICE_SOFNumberGet(DRV_HANDLE client)

  Summary:
    This function will return the USB SOF packet number.

  Description:
    This function will return the USB SOF packet number..

  Remarks:
    See drv_usbfs.h for usage information.
*/

uint16_t DRV_USBFSV1_DEVICE_SOFNumberGet
(
    DRV_HANDLE handle
)

{
    DRV_USBFSV1_OBJ * hDriver;
    usb_registers_t * usbID;
    uint16_t retVal = 0;

    if(DRV_HANDLE_INVALID == handle)
    {
        /* The handle is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver Handle is invalid in DRV_USBFSV1_DEVICE_SOFNumberGet().");
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;
        usbID = usbID;

        /* Get the Frame count */
//TODO         usbID = ((DRV_USBFSV1_OBJ *)client)->usbID;
//TODO         sofNumber = PLIB_USB_FrameNumberGet(usbID);
    }
    return retVal;
}

// *****************************************************************************
/* Function:
    USB_ERROR DRV_USBFSV1_DEVICE_IRPSubmit
    (
        DRV_HANDLE client,
        USB_ENDPOINT endpointAndDirection,
        USB_DEVICE_IRP * inputIRP
    )

  Summary:
    This function submits an I/O Request Packet (IRP) for processing to the
    Hi-Speed USB Driver.

  Description:
    This function submits an I/O Request Packet (IRP) for processing to the USB
    Driver. The IRP allows a client to send and receive data from the USB Host.
    The data will be sent or received through the specified endpoint. The direction
    of the data transfer is indicated by the direction flag in the
    endpointAndDirection parameter. Submitting an IRP arms the endpoint to
    either send data to or receive data from the Host.  If an IRP is already
    being processed on the endpoint, the subsequent IRP submit operation
    will be queued. The contents of the IRP (including the application buffers)
    should not be changed until the IRP has been processed.

    Particular attention should be paid to the size parameter of IRP. The
    following should be noted:

      * The size parameter while sending data to the Host can be less than,
        greater than, equal to, or be an exact multiple of the maximum packet size
        for the endpoint. The maximum packet size for the endpoint determines
        the number of transactions required to process the IRP.
      * If the size parameter, while sending data to the Host is less than the
        maximum packet size, the transfer will complete in one transaction.
      * If the size parameter, while sending data to the Host is greater
        than the maximum packet size, the IRP will be processed in multiple
        transactions.
      * If the size parameter, while sending data to the Host is equal to or
        an exact multiple of the maximum packet size, the client can optionally
        ask the driver to send a Zero Length Packet(ZLP) by specifying the
        USB_DEVICE_IRP_FLAG_DATA_COMPLETE flag as the flag parameter.
      * The size parameter, while receiving data from the Host must be an
        exact multiple of the maximum packet size of the endpoint. If this is
        not the case, the driver will return a USB_ERROR_IRP_SIZE_INVALID
        result. If while processing the IRP, the driver receives less than
        maximum packet size or a ZLP from the Host, the driver considers the
        IRP as processed. The size parameter at this point contains the actual
        amount of data received from the Host. The IRP status is returned as
        USB_DEVICE_IRP_STATUS_COMPLETED_SHORT.
      * If a ZLP needs to be sent to Host, the IRP size should be specified
        as 0 and the flag parameter should be set as
        USB_DEVICE_IRP_FLAG_DATA_COMPLETE.
      * If the IRP size is an exact multiple of the endpoint size, the client
        can request the driver to not send a ZLP by setting the flag parameter
        to USB_DEVICE_IRP_FLAG_DATA_PENDING. This flag indicates that there is
        more data pending in this transfer.
      * Specifying a size less than the endpoint size along with the
        USB_DEVICE_IRP_FLAG_DATA_PENDING flag will cause the driver to return a
        USB_ERROR_IRP_SIZE_INVALID.
      * If the size is greater than but not a multiple of the endpoint size, and
        the flag is specified as USB_DEVICE_IRP_FLAG_DATA_PENDING, the driver
        will send multiple of endpoint size number of bytes. For example, if the
        IRP size is 130 and the endpoint size if 64, the number of bytes sent
        will be 128.

  Remarks:
    See drv_usbfs.h for usage information.
*/


USB_ERROR DRV_USBFSV1_DEVICE_IRPSubmit
(
    DRV_HANDLE handle,
    USB_ENDPOINT endpointAndDirection,
    USB_DEVICE_IRP * inputIRP
)
{
    uint8_t endpoint;
    bool interruptWasEnabled0 = false;           /* To track interrupt state */
    bool interruptWasEnabled1 = false;           /* To track interrupt state */
    bool interruptWasEnabled2 = false;           /* To track interrupt state */
    bool interruptWasEnabled3 = false;           /* To track interrupt state */
    uint16_t direction;
    usb_registers_t * usbID;
    uint32_t loopIndex;
    uint16_t endpoint0DataStageSize;
    uint16_t endpoint0DataStageDirection;
    uint8_t * endpointDataPtr;
    uint8_t * irpDataPtr;
    uint16_t offset;
    DRV_USBFSV1_OBJ * hDriver;
    USB_DEVICE_IRP_LOCAL * irp;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObj;
    USB_ERROR retVal = USB_ERROR_NONE;
    uint16_t byteCount = 0;                 /* To hold received byte count */


    /* Check for a valid endpoint */
    endpoint = endpointAndDirection & USB_DEVICE_ENDPOINT_NUMBER_MASK;
    direction = ((endpointAndDirection & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);
    irp = (USB_DEVICE_IRP_LOCAL *) inputIRP;

    /* Check if the client handle is valid */
    if((DRV_HANDLE_INVALID == handle) || ((DRV_HANDLE)(NULL) == handle))
    {
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFS Driver: Invalid handle");
        retVal =  USB_ERROR_PARAMETER_INVALID;
    }
    else if(irp->status > USB_DEVICE_IRP_STATUS_SETUP)
    {
        /* This means that the IRP is in use */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFS Driver: Device IRP is already in use");
        retVal = USB_ERROR_DEVICE_IRP_IN_USE;
    }
    else if(endpoint >= DRV_USBFSV1_ENDPOINTS_NUMBER)
    {
        /* Endpoint number is invalid, return with appropriate error message */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSBFS Driver: Endpoint is not provisioned for");
        retVal = USB_ERROR_DEVICE_ENDPOINT_INVALID;
    }
    else
    {
        hDriver = (DRV_USBFSV1_OBJ *) handle;
        usbID = hDriver->usbID;
        endpointObj = hDriver->deviceEndpointObj[endpoint];

        if(endpoint == 0)
        {
            endpointObj += direction;
        }

        if((endpointObj->endpointState & DRV_USBFSV1_DEVICE_ENDPOINT_STATE_ENABLED) == 0)
        {
            /* This means the endpoint is disabled */
            retVal = USB_ERROR_ENDPOINT_NOT_CONFIGURED;
        }
        else
        {
            /* Check the size of the IRP. If the endpoint receives data from
             * the host, then IRP size must be multiple of maxPacketSize. If
             * the send ZLP flag is set, then size must be multiple of
             * endpoint size. */
            uint32_t reminder;

            reminder = irp->size % endpointObj->maxPacketSize;

            if((reminder != 0) && (USB_DATA_DIRECTION_HOST_TO_DEVICE == direction))
            {
                /* For receive IRP it needs to exact multiple of maxPacketSize.
                    * Hence this is an error condition. */
                retVal = USB_ERROR_PARAMETER_INVALID;
            }
            else
            {
                if((reminder == 0) && (USB_DATA_DIRECTION_HOST_TO_DEVICE != direction))
                {
                    /* If the IRP size is an exact multiple of endpoint size and
                     * size is not 0 and if data complete flag is set,
                     * then we must send a ZLP */
                    if(((irp->flags & USB_DEVICE_IRP_FLAG_DATA_COMPLETE) == USB_DEVICE_IRP_FLAG_DATA_COMPLETE) && (irp->size != 0))
                    {
                        /* This means a ZLP should be sent after the data is sent */
                        irp->flags |= USB_DEVICE_IRP_FLAG_SEND_ZLP;
                    }
                }

                /* Now we check if the interrupt context is active. If so the we dont need
                 * to get a mutex or disable interrupts.  If this were being done in non
                 * interrupt context, we, then we would disable the interrupt. In which case
                 * we would get the mutex and then disable the interrupt */

                if(hDriver->isInInterruptContext == false)
                {
	                /* Disable  the USB Interrupt as this is not called inside ISR */
//	                interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_0_IRQn);
//	                interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_1_IRQn);
//	                interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_2_IRQn);
//	                interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_3_IRQn);
//	                
//	                NVIC_DisableIRQ(USB_0_IRQn);
//	                NVIC_DisableIRQ(USB_1_IRQn);
//	                NVIC_DisableIRQ(USB_2_IRQn);
//	                NVIC_DisableIRQ(USB_3_IRQn);

	                interruptWasEnabled0 = NVIC_GetEnableIRQ(USB_IRQn);
	                interruptWasEnabled1 = NVIC_GetEnableIRQ(USB_IRQn);
	                interruptWasEnabled2 = NVIC_GetEnableIRQ(USB_IRQn);
	                interruptWasEnabled3 = NVIC_GetEnableIRQ(USB_IRQn);
	                
	                NVIC_DisableIRQ(USB_IRQn);
	                NVIC_DisableIRQ(USB_IRQn);
	                NVIC_DisableIRQ(USB_IRQn);
	                NVIC_DisableIRQ(USB_IRQn);

                }
                irp->next = NULL;

                /* Mark the IRP status as pending */
                irp->status = USB_DEVICE_IRP_STATUS_PENDING;

                /* If the data is moving from device to host then pending bytes is data
                 * remaining to be sent to the host. If the data is moving from host to
                 * device, nPendingBytes tracks the amount of data received so far */

                if(USB_DATA_DIRECTION_DEVICE_TO_HOST == direction)
                {
                    irp->nPendingBytes = irp->size;
                }
                else
                {
                    irp->nPendingBytes = 0;
                }

                /* Get the last object in the endpoint object IRP Queue */
                if(endpointObj->irpQueue == NULL)
                {
                    /* Queue is empty */
                    irp->status = USB_DEVICE_IRP_STATUS_IN_PROGRESS;
                    irp->previous = NULL;

                    endpointObj->irpQueue = irp;
                    
                    if(irp->size == 67)
                    {
                        Counter++;
                    }


                    if(endpoint == 0)
                    {

                        if(direction == USB_DATA_DIRECTION_HOST_TO_DEVICE)
                        {
                            switch(hDriver->endpoint0State)
                            {

                                case DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST:  // Case condition covered and tested

                                    /* This is the default initialization value of Endpoint
                                     * 0.  In this state EPO is waiting for the setup packet
                                     * from the host. The IRP is already added to the queue.
                                     * When the host send the Setup packet, this IRP will be
                                     * processed in the interrupt. This means we don't have
                                     * to do anything in this state. */

                                    break;


                                case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT:

                                    /* In this state, the driver has received the Setup
                                     * packet from the host, but was waiting for an IRP from
                                     * the client. The driver now has the IRP. We can unload
                                     * the setup packet into the IRP */

                                    /* Get 8-bit access to endpoint 0 OUT Data buffer address from
                                     * USB Device Descriptor Bank 0 and copy the data into IRP data buffer */

                                    endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_ADDR;

                                    irpDataPtr  = (uint8_t *)irp->data;

                                    for(loopIndex = 0; loopIndex < 8; loopIndex++)
                                    {
                                        *((uint8_t *)(irpDataPtr + loopIndex)) = *endpointDataPtr++;
                                    }

                                    /* Clear the Setup Interrupt flag and also re-enable the
                                     * setup interrupt. */

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR |= USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_RXSTP_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_RXSTP_Msk;

                                    /* Analyze the setup packet. We need to check if the
                                     * control transfer contains a data stage and if so,
                                     * what is its direction. */
                                    endpoint0DataStageSize = *((unsigned short int *) (irpDataPtr + 6));

                                    endpoint0DataStageDirection = ((irpDataPtr[0] & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);

                                    if(endpoint0DataStageSize == 0)
                                    {
                                        /* This means there is no data stage. We wait for
                                         * the client to submit the status IRP. */
                                        hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;
                                    }
                                    else
                                    {
                                        /* This means there is a data stage. Analyze the
                                         * direction. */

                                        if(endpoint0DataStageDirection == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                                        {
                                            /* If data is moving from device to host, then
                                             * we wait for the client to submit an transmit
                                             * IRP */

                                            hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;
                                        }
                                        else
                                        {
                                            /* Data is moving from host to device. We wait
                                             * for the host to send the data. */
                                            hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST;
                                        }
                                    }

                                    /* Update the IRP queue so that the client can submit an
                                     * IRP in the IRP callback. */
                                    endpointObj->irpQueue = irp->next;

                                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                    /* IRP callback */
                                    if(irp->callback != NULL)
                                    {
                                        irp->callback((USB_DEVICE_IRP *)irp);
                                    }

                                    break;


                                case DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST:
                                case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE:


                                    break;

                                case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT:

                                    /* In this state, the host sent a data stage packet, an
                                    * interrupt occurred but there was no RX data stage
                                    * IRP. The RX IRP is now being submitted. We should
                                    * unload the fifo. */

                                    byteCount = hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE & USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                                    irpDataPtr = (uint8_t *) irp->data;

                                    /* Get 8-bit access to endpoint 0 FIFO from USB RAM address */
                                    endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_ADDR;

                                    irpDataPtr = (uint8_t *)&irpDataPtr[irp->nPendingBytes];

                                    if((irp->nPendingBytes + byteCount) > irp->size)
                                    {
                                        /* This is not acceptable as it may corrupt the ram location */
                                        byteCount = irp->size - irp->nPendingBytes;
                                    }
                                    else
                                    {

                                        for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                                        {
                                            *((uint8_t *)(irpDataPtr + loopIndex)) = *endpointDataPtr++;
                                        }

                                        /* Update the pending byte count */
                                        irp->nPendingBytes += byteCount;

                                        if(irp->nPendingBytes >= irp->size)
                                        {
                                            /* This means we have received all the data that
                                             * we were supposed to receive */
                                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                            /* Change endpoint state to waiting to the
                                             * status stage */
                                            hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                            /* Clear and re-enable the interrupt */
                                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0_Msk;

                                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                                            /* Update the queue, update irp-size to indicate
                                             * how much data was received from the host. */
                                            irp->size = irp->nPendingBytes;

                                            endpointObj->irpQueue = irp->next;

                                            if(irp->callback != NULL)
                                            {
                                                irp->callback((USB_DEVICE_IRP *)irp);
                                            }
                                        }
                                        else
                                        {
                                            //Chk state alone is different - others same as top.
                                            if(byteCount < endpointObj->maxPacketSize)
                                            {
                                                /* This means we received a short packet. We
                                                 * should end the transfer. */
                                                irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;

                                                /* The data stage is complete. We now wait
                                                 * for the status stage. */
                                                hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;

                                                /* Clear and enable the interrupt. */
                                                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                                                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0_Msk;

                                                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                                                irp->size = irp->nPendingBytes;

                                                endpointObj->irpQueue = irp->next;

                                                if(irp->callback != NULL)
                                                {
                                                    irp->callback((USB_DEVICE_IRP *)irp);
                                                }
                                            }
                                            else
                                            {
                                                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                                                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0_Msk;

                                                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;
                                            }

                                        }
                                    }

                                    break;

                                    case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT:

                                    /* This means the host has already sent an RX status
                                     * stage but there was not IRP to receive this. We have
                                     * the IRP now. We change the EP0 state to waiting for
                                     * the next setup from the host. */

                                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                                    endpointObj->irpQueue = irp->next;

                                    if(irp->callback != NULL)
                                    {
                                        irp->callback((USB_DEVICE_IRP *)irp);
                                    }


                                    break;

                                case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT:


                                    break;

                                default:

                                    break;
                            }

                        }
                        else
                        {       // Device to Host

                            switch(hDriver->endpoint0State)
                            {

                                case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT:

                                    /* Driver is waiting for an IRP from the client and has
                                        * received it. Determine the transaction size. */

                                    if(irp->nPendingBytes < endpointObj->maxPacketSize)
                                    {
                                        /* This is the last transaction in the transfer. */
                                        byteCount = irp->nPendingBytes;
                                    }
                                    else
                                    {
                                        /* This is first or a continuing transaction in the
                                            * transfer and the transaction size must be
                                            * maxPacketSize */

                                        byteCount = endpointObj->maxPacketSize;
                                    }

                                    endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_ADDR;

                                    irpDataPtr = (uint8_t *)irp->data;

                                    for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                                    {
                                        *endpointDataPtr++ = *((uint8_t *)(irpDataPtr + loopIndex));
                                    }

                                    irp->nPendingBytes -= byteCount;

                                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_TX_DATA_STAGE_IN_PROGRESS;

                                    hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                                    hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_BYTE_COUNT(byteCount);

//                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_DTGLIN_Msk;   // To send as DATA1

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;   // NAK will be sent until EPSTATUS.BK1RDY is zero

                                    // This is to make sure the Status stage OUT packet from host is not acknowledged without our knowledge.
                                    // It keeps sending NAK from device.

//                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK0RDY;

                                    // When host sends IN token, device sends data of size PCKSIZE.bit.BYTE_COUNT from DeviceDescBank[1].USB_ADDR
                                    // when host sends ACK, EPSTATUS.BK1RDY is cleared, EPINTFLAG.TRCPT1 is set, EPSTATUS.DTGLIN is toggled.
                                    // If ACK is not received, device returns to idle and waits for next IN token.
                                    // Wait for EPINTFLAG.TRCPT1 to be set and process it in ISR

                                    break;


                                case DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT:

                                    /* This means the driver is expecting the client to
                                     * submit a TX status stage IRP. */
                                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE;
                                    
                                    hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;

                                    break;


                                default:

                                    break;

                            }
                        }
                    }
                    else
                    {   // Non Control Endpoint
				
                        if(direction == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                        {
                            usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                            /* Sending from Device to Host */
                            if(irp->nPendingBytes < endpointObj->maxPacketSize)
                            {
                                byteCount = irp->nPendingBytes;
                            }
                            else
                            {
                                byteCount = endpointObj->maxPacketSize;
                            }

                            irpDataPtr = (uint8_t *) irp->data;

                            offset = irp->size - irp->nPendingBytes;

                            irpDataPtr = (uint8_t *)(irp->data + offset);

                            /* Copy data to the FIFO */
                            endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_ADDR;

                            for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                            {
                                *endpointDataPtr++ = *((uint8_t *)(irpDataPtr + loopIndex));
                            }

                            irp->nPendingBytes -= byteCount;

                            hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_BYTE_COUNT(byteCount);

                            /* Enable the TXINI interrupt and clear the interrupt flag
                             * to initiate a Tx the packet */

                            usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                            usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;   // NAK will be sent until EPSTATUS.BK1RDY is zero

                            /* The rest of the IRP processing takes place in ISR */
                        }
                        else
                        {
							
							DEBUG_MESSAGE(DebugMessages6);
														
                            /* direction is Host to Device */
                            if((usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT0_Msk) == USB_DEVICE_EPINTFLAG_TRCPT0_Msk)
                            {
								DEBUG_MESSAGE(DebugMessages7);
								
								
                                /* Data is already available in the FIFO */
                                byteCount = hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_PCKSIZE & USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                                /* Get FIFO Address */
                                endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[endpoint].DEVICE_DESC_BANK[direction].USB_ADDR;

                                if((irp->nPendingBytes + byteCount) > irp->size)
                                {
                                    /* This is not acceptable as it may corrupt the ram location */
                                    byteCount = irp->size - irp->nPendingBytes;
                                }

                                irpDataPtr = (uint8_t *)irp->data;

                                for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                                {
                                    *((uint8_t *)(irpDataPtr + loopIndex)) = *endpointDataPtr++;
                                }

                                /* Update the pending byte count */
                                irp->nPendingBytes += byteCount;

                                if((irp->nPendingBytes >= irp->size) || (byteCount < endpointObj->maxPacketSize))
                                {
                                    if(byteCount < endpointObj->maxPacketSize)
                                    {
                                        /* This means we have received a short packet */
                                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
										
										DEBUG_MESSAGE(DebugMessages8);
									
                                    }
                                    else
                                    {
                                        /* This means we have received all the data that
                                        * we were supposed to receive */
                                        irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
										
										DEBUG_MESSAGE(DebugMessages9);
                                    }
                                    /* Update the queue, update irp-size to indicate
                                        * how much data was received from the host. */
                                    irp->size = irp->nPendingBytes;

                                    endpointObj->irpQueue = irp->next;

                                    if(irp->callback != NULL)
                                    {
                                        irp->callback((USB_DEVICE_IRP *)irp);
                                    }
                                }

                                /* Clear and re-enable the interrupt */

                                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT0_Msk;

                                usbID->DEVICE.DEVICE_ENDPOINT[endpoint].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;
                            }
                            else
                            {
                                /* Host has not sent any data and IRP is already added
                                 * to the queue. IRP will be processed in the ISR */
                            }
                        }/* End of non zero RX IRP submit */
                    }/* End of non zero IRP submit */
                }
                else
                {
                    /* This means we should surf the linked list to get to the last entry . */
                    USB_DEVICE_IRP_LOCAL * iterator;
                    iterator = endpointObj->irpQueue;
                    while (iterator->next != NULL)
                    {
                        iterator = iterator->next;
                    }
                    iterator->next = irp;
                    irp->previous = iterator;
                    irp->status = USB_DEVICE_IRP_STATUS_PENDING;
                }

                if(hDriver->isInInterruptContext == false)
                {
//	                if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_0_IRQn);
//	                if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_1_IRQn);
//	                if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_2_IRQn);
//	                if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_3_IRQn);
	                if(interruptWasEnabled0)		        NVIC_EnableIRQ(USB_IRQn);
	                if(interruptWasEnabled1)				NVIC_EnableIRQ(USB_IRQn);
	                if(interruptWasEnabled2)				NVIC_EnableIRQ(USB_IRQn);
	                if(interruptWasEnabled3)				NVIC_EnableIRQ(USB_IRQn);
                }
            }
        }
    }
    return(retVal);
}

// *****************************************************************************
/* Function:
      void _DRV_USBFSV1_DEVICE_Tasks_ISR(DRV_USBFSV1_OBJ * hDriver)

  Summary:
    Dynamic implementation of _DRV_USBFSV1_DEVICE_Tasks_ISR ISR handler function.

  Description:
    This is the dynamic implementation of _DRV_USBFSV1_DEVICE_Tasks_ISR ISR handler
    function for USB device.  Function will get called automatically due to USB
    interrupts in interrupt mode.  In polling mode this function will be
    routinely called from USB driver DRV_USBFSV1_Tasks() function.  This function
    performs necessary action based on the interrupt and clears the interrupt
    after that. The USB device layer callback is called with the interrupt event
    details, if callback function is registered.

  Remarks:
    This is a local function and should not be called directly by the
    application.
*/


void _DRV_USBFSV1_DEVICE_Tasks_ISR(DRV_USBFSV1_OBJ * hDriver)
{
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObjReceive;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObjTransmit;
    DRV_USBFSV1_DEVICE_ENDPOINT_OBJ * endpointObjNonZero;
    USB_DEVICE_IRP_LOCAL * irp;
    uint16_t endpoint0DataStageSize;
    uint16_t endpoint0DataStageDirection;
    usb_registers_t * usbID = USB_REGS;
    uint8_t * endpointDataPtr;
    uint8_t * irpDataPtr;
    uint16_t byteCount;
    uint16_t offset;
    uint32_t loopIndex;
    uint8_t epNonZeroIndex;
//    uint32_t epNonZeroStatus;
//    uint32_t epNonZeroMaskStatus = 0;

    if(!hDriver->isOpened)
    {
        /* We need a valid client */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver does not have a client in _DRV_USBFSV1_DEVICE_Tasks_ISR().");
    }
    else if(hDriver->pEventCallBack == NULL)
    {
        /* We need a valid event handler */
        SYS_DEBUG_MESSAGE(SYS_ERROR_INFO, "\r\nUSB USBFS Device Driver: Driver needs a event handler in _DRV_USBFSV1_DEVICE_Tasks_ISR().");
    }

    usbID = hDriver->usbID;

    if(((usbID->DEVICE.USB_INTFLAG & USB_DEVICE_INTFLAG_SOF_Msk) == USB_DEVICE_INTFLAG_SOF_Msk) && ((usbID->DEVICE.USB_INTENSET & USB_DEVICE_INTENSET_SOF_Msk) == USB_DEVICE_INTENSET_SOF_Msk))
    {
        usbID->DEVICE.USB_INTFLAG |= USB_DEVICE_INTFLAG_SOF_Msk;

        hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBFSV1_EVENT_SOF_DETECT, NULL);

    }

    if(((usbID->DEVICE.USB_INTFLAG & USB_DEVICE_INTFLAG_WAKEUP_Msk) == USB_DEVICE_INTFLAG_WAKEUP_Msk) && ((usbID->DEVICE.USB_INTENSET & USB_DEVICE_INTENSET_WAKEUP_Msk) == USB_DEVICE_INTENSET_WAKEUP_Msk))
    {        
        usbID->DEVICE.USB_INTFLAG |= USB_DEVICE_INTFLAG_WAKEUP_Msk;
    }

    if(((usbID->DEVICE.USB_INTFLAG & USB_DEVICE_INTFLAG_SUSPEND_Msk) == USB_DEVICE_INTFLAG_SUSPEND_Msk) && ((usbID->DEVICE.USB_INTENSET & USB_DEVICE_INTENSET_SUSPEND_Msk) == USB_DEVICE_INTENSET_SUSPEND_Msk))
    {        
        usbID->DEVICE.USB_INTFLAG |= USB_DEVICE_INTFLAG_SUSPEND_Msk;
    }

    if(((usbID->DEVICE.USB_INTFLAG & USB_DEVICE_INTFLAG_RAMACER_Msk) == USB_DEVICE_INTFLAG_RAMACER_Msk) && ((usbID->DEVICE.USB_INTENSET & USB_DEVICE_INTENSET_RAMACER_Msk) == USB_DEVICE_INTENSET_RAMACER_Msk))
    {        
        usbID->DEVICE.USB_INTFLAG |= USB_DEVICE_INTFLAG_RAMACER_Msk;
    }

    if(((usbID->DEVICE.USB_INTFLAG & USB_DEVICE_INTFLAG_EORST_Msk) == USB_DEVICE_INTFLAG_EORST_Msk) &&
        ((usbID->DEVICE.USB_INTENSET & USB_DEVICE_INTENSET_EORST_Msk) == USB_DEVICE_INTENSET_EORST_Msk))
    {

        hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

        hDriver->deviceSpeed = gDrvUSBFSV1DeviceSpeedMap[(usbID->DEVICE.USB_STATUS & USB_DEVICE_STATUS_SPEED_Msk) >> USB_DEVICE_STATUS_SPEED_Pos];

        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_ADDR = 0x00000000;
        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_ADDR = 0x00000000;

        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_EXTREG = 0x00000000;
        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_EXTREG = 0x00000000;

        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE = 0x00000000;
        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE = 0x00000000;

        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_STATUS_BK = 0x00000000;
        hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_STATUS_BK = 0x00000000;

        if(hDriver->pEventCallBack != NULL)
        {
            /* Send this event to the client */
            hDriver->pEventCallBack(hDriver->hClientArg, DRV_USBFSV1_EVENT_RESET_DETECT, NULL);
        }

        usbID->DEVICE.USB_DADD = USB_DEVICE_DADD_ADDEN_Msk;

        usbID->DEVICE.USB_INTFLAG |= (USB_DEVICE_INTFLAG_EORST_Msk | USB_DEVICE_INTFLAG_WAKEUP_Msk | USB_DEVICE_INTFLAG_SUSPEND_Msk);

    }

    if(USB_DEVICE_EPINTSMRY_EPINT0_Msk == (usbID->DEVICE.USB_EPINTSMRY & USB_DEVICE_EPINTSMRY_EPINT0_Msk))
    {

        endpointObjReceive = hDriver->deviceEndpointObj[0];
        endpointObjTransmit = endpointObjReceive + 1;

        if(((usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_RXSTP_Msk) == USB_DEVICE_EPINTFLAG_RXSTP_Msk) && ((usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET & USB_DEVICE_EPINTENSET_RXSTP_Msk) == USB_DEVICE_EPINTENSET_RXSTP_Msk))
        {

			DEBUG_MESSAGE(DebugMessages13);
            /* This means we have received a setup packet. Let's clear the
             * stall condition on the endpoint. */
            endpointObjReceive->endpointState &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED;

            endpointObjTransmit->endpointState &= ~DRV_USBFSV1_DEVICE_ENDPOINT_STATE_STALLED;

            irp = endpointObjReceive->irpQueue;

            if(irp != NULL)
            {

                /* Get 8-bit access to endpoint 0 OUT Data buffer address from
                 * USB Device Descriptor Bank 0 and copy the data into IRP data buffer */
                endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_ADDR;

                irpDataPtr  = (uint8_t *)irp->data;

                for(loopIndex = 0; loopIndex < 8; loopIndex++)
                {
                    *((uint8_t *)(irpDataPtr + loopIndex)) = *endpointDataPtr++;
                }

				/* This means that there was a RXSTP. */
				usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_RXSTP_Msk;

				usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                /* Analyze the setup packet. We need to check if the
                 * control transfer contains a data stage and if so,
                 * what is its direction. */
                endpoint0DataStageSize = *((unsigned short int *) (irpDataPtr + 6));

                endpoint0DataStageDirection = ((irpDataPtr[0] & USB_DEVICE_ENDPOINT_DIRECTION_MASK) != 0);

                if(endpoint0DataStageSize == 0)
                {
                    //Done: Tested this

                    /* This means there is no data stage. We wait for
                        * the client to submit the status IRP. */
                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;
                }
                else
                {
                    /* This means there is a data stage. Analyze the
                     * direction. */

                    if(endpoint0DataStageDirection == USB_DATA_DIRECTION_DEVICE_TO_HOST)
                    {

                        /* If data is moving from device to host, then
                         * we wait for the client to submit an transmit
                         * IRP */
                        hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_DATA_IRP_FROM_CLIENT;
                    }
                    else
                    {
                        //Done: Tested this

                        /* Data is moving from host to device. We wait
                         * for the host to send the data. */
                        hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_RX_DATA_STAGE_FROM_HOST;
                    }
                }

                /* Indicate that this is a setup IRP */
                irp->status = USB_DEVICE_IRP_STATUS_SETUP;

                irp->size = 8;

                endpointObjReceive->irpQueue = irp->next;

                if(irp->callback != NULL)
                {
                    irp->callback((USB_DEVICE_IRP *)irp);
                }


            }
            else
            {
                /* This is when device has received an endpoint and
                 * are informed  about the same. */
                hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_SETUP_IRP_FROM_CLIENT;

                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENCLR = USB_DEVICE_EPINTENCLR_RXSTP_Msk;

            }

        }

//TODO: uncomment checking for interrupt enable later
        if(((usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT1_Msk) == USB_DEVICE_EPINTFLAG_TRCPT1_Msk) && ((usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET & USB_DEVICE_EPINTENSET_TRCPT1_Msk) == USB_DEVICE_EPINTENSET_TRCPT1_Msk))

        { // This happens when a transfer is complete - specifically on two conditions:
            // 1. Transmission of Data stage (Device has successfully sent data to host in the data stage)
            // 2. Transmission of Status Stage (Device has successfully sent status to host in the status stage)

            if(Counter == 0)
            {
                Counter++;
            }
            else if(Counter == 1)
            {
                Counter++;
            }
            else if(Counter == 2)
            {
                Counter++;
            }
            else if(Counter == 3)
            {
                Counter++;
            }
            else if(Counter == 4)
            {
                Counter++;
            }
            else if(Counter == 5)
            {
                Counter++;
            }
            
            
			DEBUG_MESSAGE(DebugMessages12);
            irp = endpointObjTransmit->irpQueue;

            irpDataPtr = (uint8_t *) irp->data;

            if(hDriver->endpoint0State == DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_COMPLETE)
            {    // 2. Transmission of Status Stage (Device has successfully sent status to host in the status stage)
                // Now get ready to receive the next setup packet.

                irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                endpointObjTransmit->irpQueue = irp->next;

                irp->size = 0;

                if(irp->callback != NULL)
                {
                    irp->callback((USB_DEVICE_IRP *)irp);
                }

                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

            }
            else if(irp->nPendingBytes == 0)
            {
                if((irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP) == USB_DEVICE_IRP_FLAG_SEND_ZLP)
                {

                    irp->flags &= ~USB_DEVICE_IRP_FLAG_SEND_ZLP;
                    
                    hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;   // NAK will be sent until EPSTATUS.BK1RDY is zero
                }
                else
                {

                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE;

                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                    endpointObjTransmit->irpQueue = irp->next;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_DEVICE_IRP *)irp);
                    }

                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

//TODO: uncomment later when enabling disabling of interrupt
//                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENCLR &= ~USB_DEVICE_EPINTENCLR_TRCPT1_Msk;

//TODO: revisit to see if this needs to be commented
                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                }
            }
            else
            {

                if(irp->nPendingBytes <= endpointObjTransmit->maxPacketSize)
                {
                    byteCount = irp->nPendingBytes;
                }
                else
                {
                    byteCount = endpointObjTransmit->maxPacketSize;
                }

                irpDataPtr = (uint8_t *) irp->data;

                offset = irp->size - irp->nPendingBytes;

                irpDataPtr = (uint8_t *)(irp->data + offset);

                endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_ADDR;

                for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                {
                    *endpointDataPtr++ = *((uint8_t *)(irpDataPtr + loopIndex));
                }

                irp->nPendingBytes -= byteCount;

                hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[1].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_BYTE_COUNT(byteCount);

                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

//                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSCLR_BK1RDY_Msk;  // NAK will be sent until EPSTATUS.BK1RDY is zero
            }
        }


//TODO: uncomment checking for interrupt enable later
        if(((usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT0_Msk) == USB_DEVICE_EPINTFLAG_TRCPT0_Msk) && ((usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENSET & USB_DEVICE_EPINTENSET_TRCPT0_Msk) == USB_DEVICE_EPINTENSET_TRCPT0_Msk))
        {
			DEBUG_MESSAGE(DebugMessages11);
            /* This means we have received data from the host in the
             * data stage of the control transfer */
            irp = endpointObjReceive->irpQueue;

            if(hDriver->endpoint0State == DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_COMPLETE) //OUT status
            {

                if(irp != NULL)
                {
                    irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_EXPECTING_SETUP_FROM_HOST;

                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                    endpointObjReceive->irpQueue = irp->next;

                    irp->size = 0;

                    if(irp->callback != NULL)
                    {
                        irp->callback((USB_DEVICE_IRP *)irp);
                    }
                    //Done: Tested this condition
                }
                else
                {

                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_STATUS_IRP_FROM_CLIENT;

                    //TODO: uncomment checking for interrupt enable later
                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENCLR = USB_DEVICE_EPINTENCLR_TRCPT0_Msk;
                }
            }
            else   //OUT Data
            {

                if(irp == NULL)
                {
                    hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_RX_DATA_IRP_FROM_CLIENT;

                    //TODO: uncomment checking for interrupt enable later
                    usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTENCLR = USB_DEVICE_EPINTENCLR_TRCPT0_Msk;
                }
                else
                {
                    byteCount = hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_PCKSIZE & USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                    irpDataPtr = (uint8_t *)irp->data;

                    /* Get 8-bit access to endpoint 0 FIFO from USB RAM address */
                    endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[0].DEVICE_DESC_BANK[0].USB_ADDR;

                    irpDataPtr = (uint8_t *)&irpDataPtr[irp->nPendingBytes];

                    if((irp->nPendingBytes + byteCount) > irp->size)
                    {
                        /* This is not acceptable as it may corrupt the ram location */
                        byteCount = irp->size - irp->nPendingBytes;
                    }
                    else
                    {
                        for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                        {
                            *((uint8_t *)(irpDataPtr + loopIndex)) = *endpointDataPtr++;
                        }

                        irp->nPendingBytes += byteCount;

                        if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObjReceive->maxPacketSize))
                        {
                            // This is neither short packet, nor complete - means we are expecting more data
                            // clr flag and continue to receive

                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;
                        }
                        else
                        {
                            if(irp->nPendingBytes >= irp->size)
                            {
                                irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
                            }
                            else
                            {
                                /* Short Packet */
                                irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
                            }

                            hDriver->endpoint0State = DRV_USBFSV1_DEVICE_EP0_STATE_WAITING_FOR_TX_STATUS_IRP_FROM_CLIENT;

                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                            usbID->DEVICE.DEVICE_ENDPOINT[0].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                            endpointObjReceive->irpQueue = irp->next;

                            irp->size = irp->nPendingBytes;

                            if(irp->callback != NULL)
                            {
                                irp->callback((USB_DEVICE_IRP *)irp);
                            }
                        }
                    }
                }
            }
        }

    }

    /* This means this is non-EP0 interrupt. Read the endpoint status
        * register. */
    for(epNonZeroIndex = 1; epNonZeroIndex < DEVICE_ENDPOINT_NUMBER; epNonZeroIndex++)
    {
        if((usbID->DEVICE.USB_EPINTSMRY & (0x01 << epNonZeroIndex)) == 0x0000)
        {
            continue;
        }

//        epNonZeroStatus = usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG;
//        epNonZeroMaskStatus = usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET;

            /* Get the pointer to the endpoint object */
            endpointObjNonZero = hDriver->deviceEndpointObj[epNonZeroIndex];

            if((usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRFAIL0_Msk) == USB_DEVICE_EPINTFLAG_TRFAIL0_Msk)// && ((usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET & USB_DEVICE_EPINTENSET_TRFAIL0) == USB_DEVICE_EPINTENSET_TRFAIL0))
            {
				DEBUG_MESSAGE(DebugMessages10);
			}

            if(((usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT1_Msk) == USB_DEVICE_EPINTFLAG_TRCPT1_Msk) && ((usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET & USB_DEVICE_EPINTENSET_TRCPT1_Msk) == USB_DEVICE_EPINTENSET_TRCPT1_Msk))
            {
	            DEBUG_MESSAGE(DebugMessages14);

                if(endpointObjNonZero->irpQueue != NULL)
                {

                    irp = endpointObjNonZero->irpQueue;

                    if((irp->nPendingBytes == 0) && ((irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP) == USB_DEVICE_IRP_FLAG_SEND_ZLP))
                    {
                        irp->flags &= ~USB_DEVICE_IRP_FLAG_SEND_ZLP;
                        
                        hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_SIZE_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;   // NAK will be sent until EPSTATUS.BK1RDY is zero
                    }
                    else if (irp->nPendingBytes != 0)
                    {

                        if(irp->nPendingBytes >= endpointObjNonZero->maxPacketSize)
                        {
                            byteCount = endpointObjNonZero->maxPacketSize;
                        }
                        else
                        {
                            byteCount = irp->nPendingBytes;
                        }

                        irpDataPtr = (uint8_t *) irp->data;

                        offset = irp->size - irp->nPendingBytes;

                        irpDataPtr = (uint8_t *)(irp->data + offset);

                        endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_ADDR;


                        for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                        {
                            *endpointDataPtr++ = *((uint8_t *)(irpDataPtr + loopIndex));
                        }

                        irp->nPendingBytes -= byteCount;

                        hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                        hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_BYTE_COUNT(byteCount);

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET = USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPSTATUSSET = USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;   // NAK will be sent until EPSTATUS.BK1RDY is zero
                    }
                    else
                    {
                        if(!(irp->flags & USB_DEVICE_IRP_FLAG_SEND_ZLP))
                        {
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;

                            endpointObjNonZero->irpQueue = irp->next;

                            if(irp->callback != NULL)
                            {
                                irp->callback((USB_DEVICE_IRP *)irp);
                            }

                            if(endpointObjNonZero->irpQueue == NULL)
                            {

                                usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                                usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENCLR = USB_DEVICE_EPINTENCLR_TRCPT1_Msk;
                            }
                            else
                            {
                                irp = endpointObjNonZero->irpQueue;

                                if(irp->nPendingBytes >= endpointObjNonZero->maxPacketSize)
                                {
                                    byteCount = irp->nPendingBytes;
                                }
                                else
                                {
                                    byteCount = endpointObjNonZero->maxPacketSize;
                                }

                                irpDataPtr = (uint8_t *) irp->data;

                                offset = irp->size - irp->nPendingBytes;

                                irpDataPtr = (uint8_t *)(irp->data + offset);

                                endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_ADDR;

                                for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                                {
                                    *endpointDataPtr++ = *((uint8_t *)(irpDataPtr + loopIndex));
                                }

                                irp->nPendingBytes -= byteCount;

                                hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_PCKSIZE &= ~USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                                hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[1].USB_PCKSIZE |= USB_DEVICE_PCKSIZE_BYTE_COUNT(byteCount);

                                usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT1_Msk;

                                usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET |= USB_DEVICE_EPINTENSET_TRCPT1_Msk;

                                usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPSTATUSSET |= USB_DEVICE_EPSTATUSSET_BK1RDY_Msk;   // NAK will be sent until EPSTATUS.BK1RDY is zero
                            }
                        }
                    }
                }
            }
			

            if(((usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG & USB_DEVICE_EPINTFLAG_TRCPT0_Msk) == USB_DEVICE_EPINTFLAG_TRCPT0_Msk) && ((usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENSET & USB_DEVICE_EPINTENSET_TRCPT0_Msk) == USB_DEVICE_EPINTENSET_TRCPT0_Msk))
            {
                /* This means we have received RXOUTI interrupt */
                /* This means we have received data from the host */
				
				DEBUG_MESSAGE(DebugMessages1);

                if(endpointObjNonZero->irpQueue == NULL)
                {
                    usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTENCLR = USB_DEVICE_EPINTENCLR_TRCPT0_Msk;

					DEBUG_MESSAGE(DebugMessages2);
										
                }
                else
                {
					
                    irp = endpointObjNonZero->irpQueue;

                    byteCount = hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[0].USB_PCKSIZE & USB_DEVICE_PCKSIZE_BYTE_COUNT_Msk;

                    irpDataPtr = (uint8_t *) irp->data;

                    /* Get 8-bit access to endpoint 0 FIFO from USB RAM address */
                    endpointDataPtr = (uint8_t *)hDriver->endpointDescriptorTable[epNonZeroIndex].DEVICE_DESC_BANK[0].USB_ADDR;

                    irpDataPtr = (uint8_t *)&irpDataPtr[irp->nPendingBytes];

                    if((irp->nPendingBytes + byteCount) > irp->size)
                    {
                        byteCount = irp->size - irp->nPendingBytes;
                    }

                    for(loopIndex = 0; loopIndex < byteCount; loopIndex++)
                    {
                        *((uint8_t *)(irpDataPtr + loopIndex)) = *endpointDataPtr++;
                    }

                    irp->nPendingBytes += byteCount;

                    if((irp->nPendingBytes < irp->size) && (byteCount >= endpointObjNonZero->maxPacketSize))
                    {
                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;
						
						DEBUG_MESSAGE(DebugMessages3);
                    }
                    else
                    {
                        if(irp->nPendingBytes >= irp->size)
                        {
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED;
						
							DEBUG_MESSAGE(DebugMessages4);

                        }

                        else
                        {
                            /* Short Packet */
                            irp->status = USB_DEVICE_IRP_STATUS_COMPLETED_SHORT;
						
							DEBUG_MESSAGE(DebugMessages5);
                        }


                        endpointObjNonZero->irpQueue = irp->next;

                        irp->size = irp->nPendingBytes;

                        if(irp->callback != NULL)
                        {
                            irp->callback((USB_DEVICE_IRP *)irp);
                        }
                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPINTFLAG |= USB_DEVICE_EPINTFLAG_TRCPT0_Msk;

                        usbID->DEVICE.DEVICE_ENDPOINT[epNonZeroIndex].USB_EPSTATUSCLR = USB_DEVICE_EPSTATUSCLR_BK0RDY_Msk;

                    }
                }
            }
        }
}