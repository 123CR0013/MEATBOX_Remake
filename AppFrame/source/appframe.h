#pragma once

#pragma comment(lib,"AppFrame.lib")

#include "DxLib.h"

// メモリリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define NEW new
#endif

//Application
#include "Application/ApplicationBase.h"
#include "Application/UtilMacro.h"

//CFile
#include "CFile/CFile.h"

// XInput
#include "XInput/Input/XInput.h"
#include "XInput/Vibration/Vibration.h"

//Sound
#include "Sound/SoundItem.h"
#include "Sound/SoundServer.h"

// FpsController
#include "FpsController/FpsController.h"

// Easing
#include "Easing/Easing.h"

// Effect
#include "ScreenEffect/ScreenEffect.h"

// MyDraw
#include "MyDraw/mydraw.h"

// Math
#include "Math/MyMath/mymath.h"
#include "Math/Vector2/Vector2.h"
#include "Math/Vector3/Vector3.h"
#include "Math/Matrix4/Matrix4.h"
#include "Math/Quaternion/Quaternion.h"
#include "Math/Transform2/Transform2.h"

//Collision
#include "Collision/2DCollision.h"
#include "Collision/3DCollision.h"
#include "Collision/MyStructure.h"

// ResourceServer
#include "ResourceServer/ResourceServer.h"

// Timer
#include "Timer/Timer.h"



// Global
#include "Application/Global.h"