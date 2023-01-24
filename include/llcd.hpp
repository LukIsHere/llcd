/*
a great pice of code

coded and delivered by L team
code by luk the oop programmer
debbugged by zoz the glaceon

it may break everything it touches
or something i dont know why
some people state that but yes

also no touch touch credits
without premission but if
you want you can modify code
itself so yes
*/

#pragma oncd
#include <functional>
#include "input.hpp"
#include "ctx.hpp"
#include "graphic.hpp"
#include "audio.hpp"
#include "objects.hpp"
#include "scenes.hpp"
#include "event.hpp"

namespace llcd{
    enum platform : uint8_t
    {
        pico,desktop,psp,web  
    };
    class llcd{
        private:
            uint32_t lastscene = -1;
        public:
            ctx context;
            uint32_t sceneNow = 0;
            
            llcd(int height, int width,const char* name = "name");
            ///direct checks///
            
            /// @brief tells you what platform we are currently on
            /// @return the platform
            platform system();
            
            /// @brief tells you the latest mouse position
            /// @return the latest mouse position
            vector2D getMousePosition();
            
            /// @brief tells you if a key is pressed
            /// @param key key you want to check
            /// @return is pressed
            bool keboardIsPressed(input::KeyboardInput key);
            
            /// @brief tells you if a mouse button is pressed
            /// @param button button you want to check (not working rn)
            /// @return is pressed
            bool mouseIsDown(bool left);

            /// @brief tells you if legacy button is pressed
            /// @param key button you want to check
            /// @return is pressed
            bool IsPressed(input::legacyInput key);
            
            /// @brief tells you if psp button is pressed
            /// @param button button you want to check
            /// @return is pressed
            bool pspIsPressed(input::PspInput key);
            
            /// @brief tells you the position of the psp joystick
            /// @return the position of the psp joystick
            fvector2D pspJoystickPosition();

            ///events///

            events::event_handler eventHandler;
            void triggerEvent(events::event ev);

            //super event listener
        private:
            std::function<bool()> exitListener = [](){return true;};
        public:
            /// @brief SETS event that runs when user tries to exit
            /// @param listener <bool()> returns true if the user wants to exit
            /// @return returns the class itself
            llcd& addExitListener(std::function<bool()> listener);

            ///setups///
            
            scenes::sceneManager s_menager;
            llcd& addScene(scenes::scene s);
            
            ///start///

            /// @brief calling this function will start the game
            /// and the rest of the code will be executed
            /// when the game will end
            void start();
        private:
            void eventChecker();

    };
}