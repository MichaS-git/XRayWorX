Dieser Treiber benötigt die dll-Dateien welche mit der Installation von X-COM (die originale XRayWorX-Software) bereitgestellt werden. Siehe auch Anleitung "X-COM_Programming_Manual".

- die RegisterCOM.bat, die UnRegisterCOM.bat und ALLE .dll-Dateien aus dem Ordner C:\Program Files\X-COM nach ...\XRayWorX\XRayWorXApp\src kopieren
- RegisterCOM.bat als Admin ausführen
- das EPICS-XRayWorX-Modul kompilieren
- einen XRayWorX-IOC kompilieren 
- die Ordner Config und ErrorCodes aus dem Ordner C:\Program Files\X-COM in den bin Ordner des IOCs kopieren, z.B. nach ...\bin\windows-x64-static 
- XBaseConfig.xml entsprechend editieren (IP, Port ... etc., siehe XRayWorX-Anleitung)
