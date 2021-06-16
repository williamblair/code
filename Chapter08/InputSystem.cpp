// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "InputSystem.h"
#include <SDL2/SDL.h>
#include <cstring>
#include <fstream>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (SDL_BUTTON(button) & mCurrButtons) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	int mask = SDL_BUTTON(button);
	if ((mask & mPrevButtons) == 0)
	{
		if ((mask & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if ((mask & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const
{
	return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const
{
	if (mPrevButtons[button] == 0)
	{
		if (mCurrButtons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else // Prev state must be 1
	{
		if (mCurrButtons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool InputSystem::Initialize()
{
	// Keyboard
	// Assign current state pointer
	mState.Keyboard.mCurrState = SDL_GetKeyboardState(NULL);
	// Clear previous state memory
	memset(mState.Keyboard.mPrevState, 0,
		SDL_NUM_SCANCODES);

	// Mouse (just set everything to 0)
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

    // Initialize all possible controllers as disconnected
    for (int i = 0; i < ControllerState::MAX_CONTROLLERS; ++i)
    {
        mState.Controllers[i].mIsConnected = false;
        memset(mState.Controllers[i].mCurrButtons, 0,
            SDL_CONTROLLER_BUTTON_MAX);
        memset(mState.Controllers[i].mPrevButtons, 0,
            SDL_CONTROLLER_BUTTON_MAX);

        mControllers[i] = nullptr;
    }

	// Get the connected controllers, if it exists
    int numJoy = SDL_NumJoysticks();
    SDL_Log("Num joysticks: %d\n", numJoy);
    for (int i = 0; i < numJoy; ++i)
    {
        if (SDL_IsGameController(i))
        {
            mControllers[i] = SDL_GameControllerOpen(i);
            mState.Controllers[i].mIsConnected = (mControllers[i] != nullptr);
        }
        else
        {
            SDL_LogWarn(0,"Controller %d not a game controller, skipping\n",
                          i);
        }
    }

	return true;
}

void InputSystem::Shutdown()
{
}

void InputSystem::PrepareForUpdate()
{
	// Copy current state to previous
	// Keyboard
	memcpy(mState.Keyboard.mPrevState,
		mState.Keyboard.mCurrState,
		SDL_NUM_SCANCODES);

	// Mouse
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	mState.Mouse.mScrollWheel = Vector2::Zero;

	// Controller
    for (int i = 0; i < ControllerState::MAX_CONTROLLERS; ++i)
    {
        if (mControllers[i] != nullptr &&
            mState.Controllers[i].mIsConnected)
        {
    	    memcpy(mState.Controllers[i].mPrevButtons,
        		mState.Controllers[i].mCurrButtons,
        		SDL_CONTROLLER_BUTTON_MAX);
        }
    }
}

void InputSystem::Update()
{
	// Mouse
	int x = 0, y = 0;
	if (mState.Mouse.mIsRelative)
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetRelativeMouseState(&x, &y);
	}
	else
	{
		mState.Mouse.mCurrButtons = 
			SDL_GetMouseState(&x, &y);
	}

	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);

	// Controller
	// Buttons
    for (int ctrl = 0; ctrl < ControllerState::MAX_CONTROLLERS; ++ctrl)
    {
        // don't check disconnected controllers
        if (mControllers[ctrl] == nullptr ||
            !mState.Controllers[ctrl].mIsConnected)
        {
            continue;
        }
        
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
        {
            mState.Controllers[ctrl].mCurrButtons[i] =
                SDL_GameControllerGetButton(mControllers[ctrl], 
                    SDL_GameControllerButton(i));
        }
        
        // Triggers
        mState.Controllers[ctrl].mLeftTrigger =
            Filter1D(SDL_GameControllerGetAxis(mControllers[ctrl],
                SDL_CONTROLLER_AXIS_TRIGGERLEFT));
        mState.Controllers[ctrl].mRightTrigger =
            Filter1D(SDL_GameControllerGetAxis(mControllers[ctrl],
                SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

        // Sticks
        x = SDL_GameControllerGetAxis(mControllers[ctrl],
            SDL_CONTROLLER_AXIS_LEFTX);
        y = -SDL_GameControllerGetAxis(mControllers[ctrl],
            SDL_CONTROLLER_AXIS_LEFTY);
        mState.Controllers[ctrl].mLeftStick = Filter2D(x, y);

        x = SDL_GameControllerGetAxis(mControllers[ctrl],
            SDL_CONTROLLER_AXIS_RIGHTX);
        y = -SDL_GameControllerGetAxis(mControllers[ctrl],
            SDL_CONTROLLER_AXIS_RIGHTY);
        mState.Controllers[ctrl].mRightStick = Filter2D(x, y);
    }

}

void InputSystem::ProcessEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mState.Mouse.mScrollWheel = Vector2(
			static_cast<float>(event.wheel.x),
			static_cast<float>(event.wheel.y));
		break;
    case SDL_JOYDEVICEADDED:
    {
        int ctrl = event.jdevice.which;
        SDL_Log("Joy device added: %d\n", ctrl);
        if (mControllers[ctrl] != nullptr)
        {
            SDL_GameControllerClose(mControllers[ctrl]);
            mControllers[ctrl] = nullptr;
        }
        mState.Controllers[ctrl].mIsConnected = false;
        mControllers[ctrl] = SDL_GameControllerOpen(ctrl);
        if (mControllers[ctrl] != nullptr)
        {
            mState.Controllers[ctrl].mIsConnected = true;
            memset(mState.Controllers[ctrl].mCurrButtons, 0,
                SDL_CONTROLLER_BUTTON_MAX);
            memset(mState.Controllers[ctrl].mPrevButtons, 0,
                SDL_CONTROLLER_BUTTON_MAX);
        }
        break;
    }
    case SDL_JOYDEVICEREMOVED:
    {
        int ctrl = event.jdevice.which;
        SDL_Log("Joy device removed: %d\n", ctrl);
        if (mControllers[ctrl] != nullptr)
        {
            SDL_GameControllerClose(mControllers[ctrl]);
            mControllers[ctrl] = nullptr;
            mState.Controllers[ctrl].mIsConnected = false;
            memset(mState.Controllers[ctrl].mCurrButtons, 0,
                SDL_CONTROLLER_BUTTON_MAX);
            memset(mState.Controllers[ctrl].mPrevButtons, 0,
                SDL_CONTROLLER_BUTTON_MAX);
        }
        break;
    }
	default:
		break;
	}
}

void InputSystem::SetRelativeMouseMode(bool value)
{
	SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
	SDL_SetRelativeMouseMode(set);

	mState.Mouse.mIsRelative = value;
}

bool InputSystem::ParseMappingFile(const std::string& fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        SDL_LogError(0, "ParseMappingFile failed to open %s\n",
                        fileName.c_str());
        return false;
    }

    std::string curLine;
    std::string action;
    std::string device;
    std::string button;
    while (std::getline(inFile, curLine))
    {
        // skip empty or commented lines
        if (curLine.size() == 0 ||
            curLine[0] == '#')
        {
            continue;
        }

        int index = 0;
        #define GetStr(res)                     \
            res = "";                           \
            while (index < curLine.size() &&    \
                   curLine[index] != ',')       \
            {                                   \
                res += curLine[index++];        \
            }                                   \
            ++index; // skip , character

        GetStr(action);
        GetStr(device);
        GetStr(button);

        #undef GetStr
        
        if (action.size() == 0 ||
            device.size() == 0 ||
            button.size() == 0)
        {
            SDL_LogError(0, "ParseMappingFile failed to parse line: %s\n",
                         curLine.c_str());
        }

        if (device == "Keyboard")
        {
            // Supported key presses for actions
            std::map<std::string, SDL_Scancode> scancodeMap = {
                { "A", SDL_SCANCODE_A },
                { "Backspace", SDL_SCANCODE_BACKSPACE },
                { "Control", SDL_SCANCODE_LCTRL }, // left control
                { "D", SDL_SCANCODE_D },
                { "Down", SDL_SCANCODE_DOWN },
                { "Escape", SDL_SCANCODE_ESCAPE },
                { "Left", SDL_SCANCODE_LEFT },
                { "Return", SDL_SCANCODE_RETURN },
                { "Right", SDL_SCANCODE_RIGHT },
                { "S", SDL_SCANCODE_S },
                { "Shift", SDL_SCANCODE_LSHIFT }, // left shift
                { "Space", SDL_SCANCODE_SPACE },
                { "Tab", SDL_SCANCODE_TAB },
                { "Up", SDL_SCANCODE_UP },
                { "W", SDL_SCANCODE_W }
            };

            auto mapIt = scancodeMap.find(button);
            if (mapIt == scancodeMap.end())
            {
                SDL_LogError(0, "ParseMappingFile invalid key: %s\n", button.c_str());
                return false;
            }

            // add the action name and key scancode to the map
            mKeyboardActionMap[action] = mapIt->second;
        }
        else if (device == "Mouse")
        {
            // Support mouse button presses for actions
            std::map<std::string, int> buttonMap = {
                { "Left", SDL_BUTTON_LEFT },
                { "Right", SDL_BUTTON_RIGHT },
                { "Middle", SDL_BUTTON_MIDDLE },
                { "X1", SDL_BUTTON_X1 },
                { "X2", SDL_BUTTON_X2 }
            }; 

            auto mapIt = buttonMap.find(button);
            if (mapIt == buttonMap.end())
            {
                SDL_LogError(0, "ParseMappingFile invalid mouse button: %s\n",
                             button.c_str());
                return false;
            }

            // add the action name and button num to the map
            mMouseActionMap[action] = mapIt->second;
        }
        else if (device == "Controller")
        {
            // Supported controller buttons
            std::map<std::string, SDL_GameControllerButton> buttonMap = {
                { "A", SDL_CONTROLLER_BUTTON_A },
                { "B", SDL_CONTROLLER_BUTTON_B },
                { "X", SDL_CONTROLLER_BUTTON_X },
                { "Y", SDL_CONTROLLER_BUTTON_Y },
                { "Back", SDL_CONTROLLER_BUTTON_BACK },
                { "Guide", SDL_CONTROLLER_BUTTON_GUIDE },
                { "Start", SDL_CONTROLLER_BUTTON_START },
                { "LeftStick", SDL_CONTROLLER_BUTTON_LEFTSTICK },
                { "RightStick", SDL_CONTROLLER_BUTTON_RIGHTSTICK },
                { "LeftShoulder", SDL_CONTROLLER_BUTTON_LEFTSHOULDER },
                { "RightShoulder", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER },
                { "Up", SDL_CONTROLLER_BUTTON_DPAD_UP },
                { "Down", SDL_CONTROLLER_BUTTON_DPAD_DOWN },
                { "Left", SDL_CONTROLLER_BUTTON_DPAD_LEFT },
                { "Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT }
            };

            auto mapIt = buttonMap.find(button);
            if (mapIt == buttonMap.end())
            {
                SDL_LogError(0, "ParseMappingFile invalid controller button: %s\n",
                                button.c_str());
                return false;
            }

            // add the action name and button to the map
            mCtrlActionMap[action] = mapIt->second;
        }
        else
        {
            SDL_LogError(0, "ParseMappingFile invalid device: %s\n",
                         device.c_str());
            return false;
        }

    }

    return true;
}

ButtonState InputSystem::GetMappedButtonState(const std::string& actionName) const
{
    ButtonState retState = ENone;

    // check controller
    {
        auto mapIt = mCtrlActionMap.find(actionName);
        if (mapIt != mCtrlActionMap.end())
        {
            SDL_GameControllerButton btn = mapIt->second;
            // TODO - support multiple controllers
            if (mControllers[0] != nullptr &&
                mState.Controllers[0].mIsConnected)
            {
                retState = mState.Controllers[0].GetButtonState(btn);
                if (retState != ENone)
                {
                    return retState;
                }
            }
        }
    }
    // check mouse
    {
        auto mapIt = mMouseActionMap.find(actionName);
        if (mapIt != mMouseActionMap.end())
        {
            int btn = mapIt->second;
            retState = mState.Mouse.GetButtonState(btn);
            if (retState != ENone)
            {
                return retState;
            }
        }
    }
    // check keyboard
    {
        auto mapIt = mKeyboardActionMap.find(actionName);
        if (mapIt != mKeyboardActionMap.end())
        {
            SDL_Scancode btn = mapIt->second;
            retState = mState.Keyboard.GetKeyState(btn);
            if (retState != ENone)
            {
                return retState;
            }
        }
    }

    // none found
    return retState;
}

float InputSystem::Filter1D(int input)
{
	// A value < dead zone is interpreted as 0%
	const int deadZone = 250;
	// A value > max value is interpreted as 100%
	const int maxValue = 30000;

	float retVal = 0.0f;

	// Take absolute value of input
	int absValue = input > 0 ? input : -input;
	// Ignore input within dead zone
	if (absValue > deadZone)
	{
		// Compute fractional value between dead zone and max value
		retVal = static_cast<float>(absValue - deadZone) /
		(maxValue - deadZone);
		// Make sure sign matches original value
		retVal = input > 0 ? retVal : -1.0f * retVal;
		// Clamp between -1.0f and 1.0f
		retVal = Math::Clamp(retVal, -1.0f, 1.0f);
	}

	return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY)
{
	const float deadZone = 8000.0f;
	const float maxValue = 30000.0f;

	// Make into 2D vector
	Vector2 dir;
	dir.x = static_cast<float>(inputX);
	dir.y = static_cast<float>(inputY);

	float length = dir.Length();

	// If length < deadZone, should be no input
	if (length < deadZone)
	{
		dir = Vector2::Zero;
	}
	else
	{
		// Calculate fractional value between
		// dead zone and max value circles
		float f = (length - deadZone) / (maxValue - deadZone);
		// Clamp f between 0.0f and 1.0f
		f = Math::Clamp(f, 0.0f, 1.0f);
		// Normalize the vector, and then scale it to the
		// fractional value
		dir *= f / length;
	}

	return dir;
}
