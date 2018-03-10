Option Explicit

Dim WSH,sc

Set WSH=CreateObject("WScript.Shell")

Set sc = WSH.CreateShortcut("C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Startup\hq.lnk")
sc.TargetPath = "C:\projects\hq\hq.exe"
sc.save

Set sc = Nothing
Set WSH = nothing