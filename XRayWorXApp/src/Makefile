TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += XRayWorX

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install XRayWorX.dbd into <top>/dbd
DBD += TubeInterfaceEventHandler.dbd

# specify all source files to be compiled and added to the library
XRayWorX_SRCS += asynTubeInterfaceEventHandler.cpp
XRayWorX_SRCS += TubeInterfaceEventSink.cpp
XRayWorX_SRCS += TubeCmdSingleEventSink.cpp
XRayWorX_SRCS += TubeCmdBoolEventSink.cpp
XRayWorX_SRCS += TubeMonBoolEventSink.cpp
XRayWorX_SRCS += TubeAutoCmdEventSink.cpp
XRayWorX_SRCS += TubeCmdXocEventSink.cpp
XRayWorX_SRCS += TubeFlashoverEventSink.cpp
XRayWorX_SRCS += TubeListMonStringEventSink.cpp
XRayWorX_SRCS += AutoCommandInfoEventSink.cpp
XRayWorX_SRCS += CommandInfoMonitorEventSink.cpp
XRayWorX_SRCS += TubeServiceMonitorEventSink.cpp
XRayWorX_SRCS += CoilDataEventSink.cpp
XRayWorX_SRCS += DataModuleEventSink.cpp
XRayWorX_SRCS += LimitableAutoCommandEventSink.cpp
XRayWorX_SRCS += TurbopumpEventSink.cpp
XRayWorX_SRCS += TubeListEventSink.cpp
XRayWorX_SRCS += InitialStartupEventSink.cpp
XRayWorX_SRCS += TubeTimeSpanEventSink.cpp
XRayWorX_SRCS += FilamentStatsEventSink.cpp
XRayWorX_SRCS += StandbyEventSink.cpp

#XRayWorX_LIBS += asyn

XRayWorX_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
