#define SDL_MAIN_HANDLED
#include "../../Source/GameEngine/Input/Gamepad.h"
#include <cassert>
#include <cstdio>
struct _SDL_GameController { bool attached; int refs; Uint8 buttons[SDL_CONTROLLER_BUTTON_MAX]; Sint16 axes[SDL_CONTROLLER_AXIS_MAX]; };
static _SDL_GameController fake[5]={};
extern "C" {
int SDLCALL SDL_NumJoysticks(void){return 5;}
SDL_bool SDLCALL SDL_IsGameController(int n){return fake[n].attached?SDL_TRUE:SDL_FALSE;}
SDL_GameController* SDLCALL SDL_GameControllerOpen(int n){fake[n].refs++;return &fake[n];}
void SDLCALL SDL_GameControllerClose(SDL_GameController* p){p->refs--;}
SDL_bool SDLCALL SDL_GameControllerGetAttached(SDL_GameController* p){return p->attached?SDL_TRUE:SDL_FALSE;}
void SDLCALL SDL_GameControllerUpdate(void){}
Uint8 SDLCALL SDL_GameControllerGetButton(SDL_GameController* p,SDL_GameControllerButton b){return p->buttons[b];}
Sint16 SDLCALL SDL_GameControllerGetAxis(SDL_GameController* p,SDL_GameControllerAxis a){return p->axes[a];}
}
int main(){
 using GameEngine::Gamepad;
 auto& p=Gamepad::get();
 for(int i=0;i<5;i++)fake[i].attached=true;
 p.poll(true);for(int i=0;i<4;i++){assert(p.connected(i));assert(fake[i].refs==1);}assert(fake[4].refs==0);
 // Polls retain handles without leaking references; a fifth device waits.
 for(int n=0;n<20;n++)p.poll(true);
 for(int i=0;i<4;i++)assert(fake[i].refs==1);
 assert(Gamepad::deadzone(7000)==0);assert(Gamepad::deadzone(-32768)==-1);
 fake[0].axes[SDL_CONTROLLER_AXIS_LEFTX]=-28000;
 fake[0].buttons[SDL_CONTROLLER_BUTTON_A]=1;
 fake[1].buttons[SDL_CONTROLLER_BUTTON_B]=1;
 p.poll(true);assert(p.action(1,1));assert(p.action(1,5));assert(!p.action(1,5,true));assert(p.action(2,6));assert(!p.action(2,5));
 p.finishUpdate();assert(p.action(1,5,true));
 // Focus loss and disconnect release held actions, leaving other slots alone.
 p.poll(false);assert(!p.action(1,5));assert(p.action(1,5,true));p.finishUpdate();
 p.poll(true);p.finishUpdate();fake[0].attached=false;p.poll(true);
 assert(fake[0].refs==0);assert(fake[4].refs==1);assert(!p.action(1,5));assert(p.action(1,5,true));assert(p.action(2,6));
 p.finishUpdate();assert(!p.action(1,5,true));
 fake[2].buttons[SDL_CONTROLLER_BUTTON_X]=1;fake[2].axes[SDL_CONTROLLER_AXIS_TRIGGERRIGHT]=25000;
 fake[3].buttons[SDL_CONTROLLER_BUTTON_START]=1;p.poll(true);
 assert(p.action(3,4));assert(p.action(3,7));assert(p.any(SDL_CONTROLLER_BUTTON_START));
 p.close();for(int i=0;i<5;i++)assert(fake[i].refs==0);
 std::puts("PASS: four slots, hot-plug, overflow, deadzone, independent actions, edges, focus loss, disconnect releases, jump, grab, pause, cleanup");
}
