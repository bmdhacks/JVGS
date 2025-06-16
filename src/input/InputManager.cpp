#include "InputManager.h"

using namespace std;

namespace jvgs
{
    namespace input
    {
        InputManager::InputManager()
        {
#ifdef USE_SDL2
            SDL_InitSubSystem(SDL_INIT_EVENTS);
            keyState = SDL_GetKeyboardState(0);
#else
            SDL_InitSubSystem(SDL_INIT_EVENTTHREAD);
            keyState = SDL_GetKeyState(0);
#endif
            quitEvent = false;
            tickedKeys = new bool[KEY_LAST];
        }

        InputManager::~InputManager()
        {
#ifdef USE_SDL2
            SDL_QuitSubSystem(SDL_INIT_EVENTS);
#else
            SDL_QuitSubSystem(SDL_INIT_EVENTTHREAD);
#endif
            delete[] tickedKeys;
        }

        InputManager *InputManager::getInstance()
        {
            static InputManager instance;
            return &instance;
        }

        bool InputManager::isKeyDown(const Key &key) const
        {
#ifdef USE_SDL2
            if (!keyState) return false;
            SDL_Scancode scancode = SDL_GetScancodeFromKey((SDL_Keycode)key);
            if (scancode == SDL_SCANCODE_UNKNOWN) return false;
            return (bool) keyState[scancode];
#else
            return (bool) keyState[key];
#endif
        }

        bool InputManager::isKeyTicked(const Key &key) const
        {
            // SDL2 key codes can be large, so we need bounds checking
            if (key < 0 || key >= KEY_LAST) return false;
            return tickedKeys[key];
        }

        void InputManager::update(float ms)
        {
            for(int i = 0; i < KEY_LAST; i++)
                tickedKeys[i] = false;

            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                switch(event.type) {
                    /* Key down - alert all listeners. */
                    case SDL_KEYDOWN:
                        if(event.key.keysym.sym == SDLK_ESCAPE) {
                            quitEvent = true;
                        } else {
                            // SDL2 key codes can be large, add bounds checking
                            if (event.key.keysym.sym >= 0 && event.key.keysym.sym < KEY_LAST) {
                                tickedKeys[event.key.keysym.sym] = true;
                            }
                        }
                        break;
                    case SDL_QUIT:
                        quitEvent = true;
                        break;
                }
            }
        }

        bool InputManager::hasQuitEvent() const
        {
            return quitEvent;
        }

        void InputManager::sendQuitEvent()
        {
            quitEvent = true;
        }
    }
}
