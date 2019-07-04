"""*****************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
*****************************************************************************"""
def loadModule():
	print Variables.get("__PROCESSOR")
	# Initially Set all USB Library modules to False. We will enable each -  
	# Library Module depending on the MCU/MPU selected.  
	loadUSBHostLayer = False
	loadUSBHostCDC = False
	loadUSBHostMSD = False
	loadUSBHostHID = False
	loadUSBHostAudio = False 
	
	loadUSBDeviceLayer = False
	loadUSBDeviceCDC = False
	loadUSBDeviceHID = False
	loadUSBDeviceAudio = False
	loadUSBDeviceMSD = False
	loadUSBDeviceVendor = False 
	loadUSBDevicePrinter = False 
	
	# Below variables used to help using different capability names for SAMA5D2 
	USBDeviceDriverCapabilityName = "DRV_USB" 
	USBHostDriverCapabilityName = "DRV_USB" 
	
	if any(x in Variables.get("__PROCESSOR") for x in ["SAMA5D2"]):
		# Create USB High Speed Host Port Driver Component for SAMA5D2
		usbUhpHsDriverComponent =  Module.CreateComponent("drv_usb_uhphs", "USB UHPHS Host Driver", "/Harmony/Drivers", "config/usb_uhp_driver.py")
		usbUhpHsDriverComponent.addCapability("DRV_USB", "DRV_USB", True)
		usbUhpHsDriverComponent.addDependency("usb_uhphs_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
		
		# Use different name for UHPHS Driver 
		USBHostDriverCapabilityName = "DRV_UHPHS"
		
		# Load USB Host Layer Components  
		loadUSBHostLayer = True 
		loadUSBHostCDC = True
		loadUSBHostMSD = True
		
		# Create USB Device Port High Speed (UDPHS) Driver Component for SAMA5D2
		usbDevicePortHighSpeedDriverComponent =  Module.CreateComponent("drv_usb_udphs", "USB UDPHS Device Driver", "/Harmony/Drivers", "config/usb_udphs_driver.py")
		usbDevicePortHighSpeedDriverComponent.addCapability("DRV_UDPHS", "DRV_UDPHS",True)
		
		# Add Generic Dependency on Core Service
		usbDevicePortHighSpeedDriverComponent.addDependency("usb_udphs_HarmonyCoreDependency", "Core Service", "Core Service", True, True)

		USBDriverCapabilityName = "DRV_UDPHS"
		loadUSBHostLayer = True 
		loadUSBHostCDC = True
		loadUSBHostMSD = True		
		loadUSBDeviceLayer = True
		loadUSBDeviceCDC = True
		loadUSBDeviceHID = True
		loadUSBDeviceMSD = True
		loadUSBDeviceVendor = True 
		loadUSBDevicePrinter = True 
		

	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMV70", "SAMV71", "SAME70", "SAMS70", "PIC32MZ"]):
		# Create USB High Speed Driver Component
		usbDriverComponent =  Module.CreateComponent("drv_usbhs", "USB High Speed Driver", "/Harmony/Drivers", "config/usbhs_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
		usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
		
		
		# Enable USB Library modules 
		loadUSBHostLayer = True
		loadUSBHostCDC = True
		loadUSBHostMSD = True
		loadUSBHostHID = True
		loadUSBHostAudio = True 
	
		loadUSBDeviceLayer = True
		loadUSBDeviceCDC = True
		loadUSBDeviceHID = True
		loadUSBDeviceAudio = True
		loadUSBDeviceMSD = True
		loadUSBDeviceVendor = True 
		loadUSBDevicePrinter = True 
	
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAMD21", "SAMD5", "SAME5", "SAML21", "PIC32MK", "PIC32MX2", "PIC32MX3", "PIC32MX4", "PIC32MX5", "PIC32MX6", "PIC32MX7"]):
		# Create USB Full Speed Driver Component
		usbDriverComponent =  Module.CreateComponent("drv_usbfs", "USB Full Speed Driver", "/Harmony/Drivers", "config/usbfs_v1_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
		usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
		
		
		# Enable USB Library modules 
		loadUSBHostLayer = True
		loadUSBHostCDC = True
		loadUSBHostMSD = True
		loadUSBHostHID = True
		loadUSBHostAudio = True 
	
		loadUSBDeviceLayer = True
		loadUSBDeviceCDC = True
		loadUSBDeviceHID = True
		loadUSBDeviceAudio = True
		loadUSBDeviceMSD = True
		loadUSBDeviceVendor = True
		loadUSBDevicePrinter = True  
		
	elif any(x in Variables.get("__PROCESSOR") for x in ["SAML22"]):
		# Create USB Full Speed Driver Component
		usbDriverComponent =  Module.CreateComponent("drv_usbfs", "USB Full Speed Driver", "/Harmony/Drivers", "config/usbfs_v1_driver.py")
		usbDriverComponent.addCapability("DRV_USB", "DRV_USB",True)
		usbDriverComponent.addDependency("drv_usb_HarmonyCoreDependency", "Core Service", "Core Service", True, True)
		
		
		loadUSBDeviceLayer = True
		loadUSBDeviceCDC = True
		loadUSBDeviceHID = True
		loadUSBDeviceAudio = True
		loadUSBDeviceMSD = True
		loadUSBDeviceVendor = True
		loadUSBDevicePrinter = True  
		
	# Create USB Device Layer Component
	if  loadUSBDeviceLayer == True:	 
		usbDeviceComponent = Module.CreateSharedComponent("usb_device", "USB Device Layer", "/Libraries/USB/Device Stack", "config/usb_device.py")
		usbDeviceComponent.addDependency("usb_driver_dependency", USBDeviceDriverCapabilityName, True, True)
		usbDeviceComponent.addCapability("USB Device", "USB_DEVICE", True)
	
	# Create USB Device CDC Function driver Component 
	if loadUSBDeviceCDC == True:
		usbDeviceCdcComponent = Module.CreateGeneratorComponent("usb_device_cdc", "CDC Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_cdc_common.py", "config/usb_device_cdc.py")
		usbDeviceCdcComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
		usbDeviceCdcComponent.addCapability("USB Device", "USB_DEVICE_CDC")
	
	# Create USB Device Vendor Component 
	if loadUSBDeviceVendor == True:	
		usbDeviceVendorComponent = Module.CreateGeneratorComponent("usb_device_vendor", "Vendor Function", "/Libraries/USB/Device Stack", "config/usb_device_vendor_common.py", "config/usb_device_vendor.py")
		usbDeviceVendorComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
		usbDeviceVendorComponent.addCapability("USB Device", "USB_DEVICE_VENDOR")
	
	# Create USB Device Audio Component 
	if loadUSBDeviceAudio == True:
		usbDeviceAudioComponent = Module.CreateGeneratorComponent("usb_device_audio", "Audio Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_audio_common.py", "config/usb_device_audio.py")
		usbDeviceAudioComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
		usbDeviceAudioComponent.addCapability("USB Device", "USB_DEVICE_AUDIO")
		
	# Create USB Device HID Component 
	if loadUSBDeviceHID == True:
		usbDeviceHidComponent = Module.CreateGeneratorComponent("usb_device_hid", "HID Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_hid_common.py", "config/usb_device_hid.py")
		usbDeviceHidComponent.addDependency("usb_device_dependency", "USB_DEVICE" , True, True)
		usbDeviceHidComponent.addCapability("USB Device", "USB_DEVICE_HID")
	
	# Create USB Device MSD Component 
	if loadUSBDeviceMSD == True:
		usbDeviceMsdComponent = Module.CreateGeneratorComponent("usb_device_msd", "MSD Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_msd_common.py", "config/usb_device_msd.py")
		usbDeviceMsdComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
		usbDeviceMsdComponent.addDependency("usb_device_msd_media_dependency", "DRV_MEDIA", False, True)
		usbDeviceMsdComponent.addCapability("USB Device", "USB_DEVICE_MSD")

	# Create USB Device Printer Component 
	if loadUSBDevicePrinter == True:
		usbDevicePrinterComponent = Module.CreateGeneratorComponent("usb_device_printer", "Printer Function Driver", "/Libraries/USB/Device Stack", "config/usb_device_printer_common.py", "config/usb_device_printer.py")
		usbDevicePrinterComponent.addDependency("usb_device_dependency", "USB_DEVICE", True, True)
		usbDevicePrinterComponent.addCapability("USB Device", "USB_DEVICE_PRINTER")		
	
	# Create USB Host Layer Component 	
	if loadUSBHostLayer == True:
		usbHostComponent = Module.CreateSharedComponent("usb_host", "Host Layer", "/Libraries/USB/Host Stack", "config/usb_host.py")
		usbHostComponent.addDependency("usb_driver_dependency", USBHostDriverCapabilityName, True, True)
		usbHostComponent.addDependency("usb_host_tmr_dependency", "SYS_TIME", True, True)
		usbHostComponent.addCapability("usb_host", "USB_HOST", True)
	
	# Create USB Host Stack MSD Component 
	if loadUSBHostMSD == True:
		usbHostMsdComponent = Module.CreateComponent("usb_host_msd", "MSD Client Driver", "/Libraries/USB/Host Stack","config/usb_host_msd.py")
		usbHostMsdComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
		usbHostMsdComponent.addCapability("USB Host MSD", "DRV_MEDIA")
	
	# Create USB Host Stack CDC Component 
	if loadUSBHostCDC == True:	
		usbHostCdcComponent = Module.CreateComponent("usb_host_cdc", "CDC Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_cdc.py")
		usbHostCdcComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
	
	# Create USB Host Stack HID Component 	
	if loadUSBHostHID == True:
		usbHostHidComponent = Module.CreateComponent("usb_host_hid", "HID Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_hid.py")
		usbHostHidComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
	
	# Create USB Host Stack Audio Component 
	if loadUSBHostAudio == True:
		usbHostAudioComponent = Module.CreateComponent("usb_host_audio", "Audio Client Driver", "/Libraries/USB/Host Stack", "config/usb_host_audio.py")
		usbHostAudioComponent.addDependency("usb_host_dependency", "USB_HOST", True, True)
		
