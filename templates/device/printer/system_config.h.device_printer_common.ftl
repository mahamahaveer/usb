<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device_printer_common.ftl

  Summary:
    USB Device Freemarker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
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
-->
/* Maximum instances of Printer function driver */
#define USB_DEVICE_PRINTER_INSTANCES_NUMBER          ${__INSTANCE_COUNT} 

/* Printer Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED       ${CONFIG_USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED}

/* Length of the Device ID string including length in the first two bytes */
#define USB_DEVICE_PRINTER_DEVICE_ID_STRING_LENGTH    ${CONFIG_USB_PRINTER_DEVICE_ID_STRING?length?number+2}

/* Device ID string including length in the first two bytes */
#define USB_DEVICE_PRINTER_DEVICE_ID_STRING <#if CONFIG_USB_PRINTER_DEVICE_ID_STRING?length gt 0 > {0,${CONFIG_USB_PRINTER_DEVICE_ID_STRING?length?number+2},<#list 1..CONFIG_USB_PRINTER_DEVICE_ID_STRING?length as index>'${CONFIG_USB_PRINTER_DEVICE_ID_STRING?substring(index-1,index)}'<#if index_has_next>,</#if></#list>},</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

