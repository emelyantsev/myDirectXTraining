cl App6.cpp ShapesApp.cpp FrameResource.cpp ..\Common\d3dApp.cpp ..\Common\d3dUtils.cpp ..\Common\GameTimer.cpp ..\Common\MathHelper.cpp ^
..\Common\GeometryGenerator.cpp ^
/DSUBSYSTEM_WINDOWS  /EHsc /I..\Common -DUNICODE /link gdi32.lib user32.lib /SUBSYSTEM:WINDOWS