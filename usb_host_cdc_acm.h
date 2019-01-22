/*******************************************************************************
  USB Host CDC Client Driver Interface Definition

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_cdc_acm.h

  Summary:
    USB Host CDC Client Driver Interface Header

  Description:
    This header file contains the CDC ACM specific function prototypes and
    definitions of the data types and constants that make up the interface to
    the USB Host CDC Client Driver.
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
#ifndef _USB_HOST_CDC_ACM_H_
#define _USB_HOST_CDC_ACM_H_

//DOM-IGNORE-END

// ****************************************************************************
// ****************************************************************************
// Section: Included Files.
// ****************************************************************************
// ****************************************************************************

#include "usb/usb_host_cdc.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END  

// ****************************************************************************
// ****************************************************************************
// Section: CDC Class Specific Commands.
// ****************************************************************************
// ****************************************************************************

// ****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_LineCodingSet
    (
        USB_HOST_CDC_HANDLE handle, 
        USB_HOST_CDC_REQUEST_HANDLE * requestHandle, 
        USB_CDC_LINE_CODING * lineCoding
    );
       
  Summary:
    This function sends a request to the attached device to set its Line Coding.

  Description:
    This function sends a request to the attached device to set its line coding.
    The function schedules a SET LINE CODING control transfer. If successful,
    the requestHandle parameter will contain a valid request handle, else it
    will contain USB_HOST_CDC_REQUEST_HANDLE_INVALID. When completed, the CDC
    client driver will generate a USB_HOST_CDC_EVENT_ACM_SET_LINE_CODING_COMPLETE
    event. 

  Precondition:
    The client handle should be valid.

  Input:
    handle - handle to the CDC device instance to which the request should be
    sent.
    
    requestHandle - Pointer to USB_HOST_CDC_REQUEST_HANDLE type of a variable.
    This will contain a valid transfer handle if the request was successful.

    lineCoding - Pointer to the line coding data structure containing the line
    coding to be set. The contents of this data structure should not be changed
    until the USB_HOST_CDC_EVENT_ACM_SET_LINE_CODING_COMPLETE event has
    been generated.

  Return:
    USB_HOST_CDC_RESULT_SUCCESS - The operation was successful.

    USB_HOST_CDC_RESULT_DEVICE_UNKNOWN - The device that this request was
    targeted to does not exist in the system.

    USB_HOST_CDC_RESULT_BUSY - The request could not be scheduled at this time.
    The client should try again.

    USB_HOST_CDC_RESULT_INVALID_PARAMETER - An input parameter was NULL.

    USB_HOST_CDC_RESULT_FAILURE - An unknown failure occurred.

    USB_HOST_CDC_RESULT_HANDLE_INVALID - The client handle is not valid.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/

USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_LineCodingSet
(
    USB_HOST_CDC_HANDLE handle, 
    USB_HOST_CDC_REQUEST_HANDLE * requestHandle, 
    USB_CDC_LINE_CODING * lineCoding
);

// ****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_LineCodingGet
    (
        USB_HOST_CDC_HANDLE handle, 
        USB_HOST_CDC_REQUEST_HANDLE * requestHandle, 
        USB_CDC_LINE_CODING * lineCoding
    );
   
  Summary:
    This function sends a request to the attached device to get its Line Coding.

  Description:
    This function sends a request to the attached device to get its line coding.
    The function schedules a GET LINE CODING control transfer. If successful,
    the requestHandle parameter will contain a valid request handle, else it
    will contain USB_HOST_CDC_REQUEST_HANDLE_INVALID. When completed, the CDC
    client driver will generate a USB_HOST_CDC_EVENT_ACM_GET_LINE_CODING_COMPLETE
    event. 

  Precondition:
    The client handle should be valid.

  Input:
    handle - handle to the CDC device instance to which the request should be
    sent.
    
    requestHandle - Pointer to USB_HOST_CDC_REQUEST_HANDLE type of a variable.
    This will contain a valid transfer handle if the request was successful.

    lineCoding - Pointer to the line coding data structure where the obtained
    line coding will be stored. The contents of this data structure will be
    valid only when the USB_HOST_CDC_EVENT_ACM_GET_LINE_CODING_COMPLETE event has
    been generated.

  Return:
    USB_HOST_CDC_RESULT_SUCCESS - The operation was successful.

    USB_HOST_CDC_RESULT_DEVICE_UNKNOWN - The device that this request was
    targeted to does not exist in the system.

    USB_HOST_CDC_RESULT_BUSY - The request could not be scheduled at this time.
    The client should try again.

    USB_HOST_CDC_RESULT_INVALID_PARAMETER - An input parameter was NULL.

    USB_HOST_CDC_RESULT_FAILURE - An unknown failure occurred.

    USB_HOST_CDC_RESULT_HANDLE_INVALID - The client handle is not valid.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/

USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_LineCodingGet
(
    USB_HOST_CDC_HANDLE handle, 
    USB_HOST_CDC_REQUEST_HANDLE * requestHandle, 
    USB_CDC_LINE_CODING * lineCoding
);

// ****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_ControlLineStateSet
    (
        USB_HOST_CDC_HANDLE handle,
        USB_HOST_CDC_REQUEST_HANDLE * requestHandle, 
        USB_CDC_HOST_CONTROL_LINE_STATE * controlLineState
    );
   
  Summary:
    This function sends a request to the attached device to set its Control Line
    State.

  Description:
    This function sends a request to the attached to set its Control Line State.
    The function schedules a SET CONTROL LINE STATE control transfer. If
    successful, the requestHandle parameter will contain a valid request handle,
    else it will contain USB_HOST_CDC_REQUEST_HANDLE_INVALID. When completed,
    the CDC client driver will generate a
    USB_HOST_CDC_EVENT_ACM_SET_CONTROL_LINE_STATE_COMPLETE event. 

  Precondition:
    The client handle should be valid.

  Input:
    handle - handle to the CDC device instance to which the request should be
    sent.
    
    requestHandle - Pointer to USB_HOST_CDC_REQUEST_HANDLE type of a variable.
    This will contain a valid transfer handle if the request was successful.

    controlLineState - Pointer to the control line state data structure.

  Return:
    USB_HOST_CDC_RESULT_SUCCESS - The operation was successful.

    USB_HOST_CDC_RESULT_DEVICE_UNKNOWN - The device that this request was
    targeted to does not exist in the system.

    USB_HOST_CDC_RESULT_BUSY - The request could not be scheduled at this time.
    The client should try again.

    USB_HOST_CDC_RESULT_INVALID_PARAMETER - An input parameter was NULL.

    USB_HOST_CDC_RESULT_FAILURE - An unknown failure occurred.

    USB_HOST_CDC_RESULT_HANDLE_INVALID - The client handle is not valid.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/

USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_ControlLineStateSet
(
    USB_HOST_CDC_HANDLE handle,
    USB_HOST_CDC_REQUEST_HANDLE * requestHandle, 
    USB_CDC_CONTROL_LINE_STATE * controlLineState
);

// ****************************************************************************
/* Function:
    USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_BreakSend
    (
        USB_HOST_CDC_HANDLE handle,
        USB_HOST_CDC_REQUEST_HANDLE * requestHandle,
        uint16_t breakDuration
    );
   
  Summary:
    This function sends a request to the attached device to update its break
    duration.

  Description:
    This function sends a request to the attached to update its break
    duration. The function schedules a SEND BREAK control transfer. If
    successful, the transferHandle parameter will contain a valid request
    handle, else it will contain USB_HOST_CDC_REQUEST_HANDLE_INVALID. When
    completed, the CDC client driver will generate a
    USB_HOST_CDC_EVENT_ACM_SEND_BREAK_COMPLETE event. 

  Precondition:
    The client handle should be valid.

  Input:
    handle - handle to the CDC device instance to which the request should be
    sent.
    
    requestHandle - Pointer to USB_HOST_CDC_REQUEST_HANDLE type of a variable.
    This will contain a valid transfer handle if the request was successful.

    breakDuration - Break duration.

  Return:
    USB_HOST_CDC_RESULT_SUCCESS - The operation was successful.

    USB_HOST_CDC_RESULT_DEVICE_UNKNOWN - The device that this request was
    targeted to does not exist in the system.

    USB_HOST_CDC_RESULT_BUSY - The request could not be scheduled at this time.
    The client should try again.

    USB_HOST_CDC_RESULT_INVALID_PARAMETER - An input parameter was NULL.

    USB_HOST_CDC_RESULT_FAILURE - An unknown failure occurred.

    USB_HOST_CDC_RESULT_HANDLE_INVALID - The client handle is not valid.
    
  Example:
    <code>
    </code>

  Remarks:
    None.                                                                   
*/

USB_HOST_CDC_RESULT USB_HOST_CDC_ACM_BreakSend
(
    USB_HOST_CDC_HANDLE handle,
    USB_HOST_CDC_REQUEST_HANDLE * requestHandle,
    uint16_t breakDuration
);

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END


#endif

