cl App22.cpp SkinnedMeshApp.cpp FrameResource.cpp LoadM3d.cpp ShadowMap.cpp SkinnedData.cpp Ssao.cpp  ..\Common\d3dApp.cpp ..\Common\d3dUtils.cpp ..\Common\GameTimer.cpp ..\Common\MathHelper.cpp ^
..\Common\GeometryGenerator.cpp ..\Common\DDSTextureLoader.cpp ..\Common\Camera.cpp ^
/O2 /Oi /DSUBSYSTEM_WINDOWS  /EHsc /I..\Common -DUNICODE /link gdi32.lib user32.lib /SUBSYSTEM:WINDOWS