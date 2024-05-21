#!../../bin/windows-x64-static/XRayWorX

#- You may have to change XRayWorX to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/XRayWorX.dbd"
XRayWorX_registerRecordDeviceDriver pdbbase

## Load record instances
dbLoadRecords("$(XRAYWORX)/db/tubeInterface.template","P=XTube,PORT=Tube,ADDR=0")

TubeInterfaceEventHandlerConfig("Tube")

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=ovs"
