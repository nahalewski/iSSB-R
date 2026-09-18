#pragma once
#include <SDL.h>
#include <cstring>
#include <cmath>

namespace GameEngine {
// Header-only so all existing platform project files share the same input backend.
class Gamepad {
 SDL_GameController* devices[4];
 bool actions[4][8], previous[4][8];
 bool buttons[4][SDL_CONTROLLER_BUTTON_MAX];
 bool oldButtons[4][SDL_CONTROLLER_BUTTON_MAX];
 float axes[4][2];
 Gamepad() { std::memset(devices,0,sizeof(devices)); clear(); finishUpdate(); }
 void clear() {
  std::memset(actions,0,sizeof(actions)); std::memset(buttons,0,sizeof(buttons));
  std::memset(axes,0,sizeof(axes));
 }
public:
 static Gamepad& get() { static Gamepad value; return value; }
 static float deadzone(int value) {
  const float zone=8000.0f;
  if(std::abs(value)<=zone)return 0;
  return (value<0?-1.0f:1.0f)*(std::abs(value)-zone)/(32768.0f-zone);
 }
 void close() { for(int i=0;i<4;i++)if(devices[i]) { SDL_GameControllerClose(devices[i]);devices[i]=0; } clear(); }
 void poll(bool focused) {
  for(int i=0;i<4;i++)if(devices[i]&&!SDL_GameControllerGetAttached(devices[i])) {
   SDL_GameControllerClose(devices[i]);devices[i]=0;
  }
  for(int n=0;n<SDL_NumJoysticks();n++)if(SDL_IsGameController(n)) {
   SDL_GameController* candidate=SDL_GameControllerOpen(n);
   if(!candidate)continue;
   bool known=false; int freeSlot=-1;
   for(int i=0;i<4;i++) { if(devices[i]==candidate)known=true; if(!devices[i]&&freeSlot<0)freeSlot=i; }
   if(!known&&freeSlot>=0)devices[freeSlot]=candidate;
   else SDL_GameControllerClose(candidate);
  }
  SDL_GameControllerUpdate(); clear();
  if(!focused)return;
  for(int i=0;i<4;i++)if(devices[i]) {
   for(int b=0;b<SDL_CONTROLLER_BUTTON_MAX;b++)buttons[i][b]=SDL_GameControllerGetButton(devices[i],(SDL_GameControllerButton)b)!=0;
   axes[i][0]=deadzone(SDL_GameControllerGetAxis(devices[i],SDL_CONTROLLER_AXIS_LEFTX));
   axes[i][1]=deadzone(SDL_GameControllerGetAxis(devices[i],SDL_CONTROLLER_AXIS_LEFTY));
   actions[i][0]=buttons[i][SDL_CONTROLLER_BUTTON_DPAD_UP]||axes[i][1]<-0.25f;
   actions[i][1]=buttons[i][SDL_CONTROLLER_BUTTON_DPAD_LEFT]||axes[i][0]<-0.25f;
   actions[i][2]=buttons[i][SDL_CONTROLLER_BUTTON_DPAD_DOWN]||axes[i][1]>0.25f;
   actions[i][3]=buttons[i][SDL_CONTROLLER_BUTTON_DPAD_RIGHT]||axes[i][0]>0.25f;
   actions[i][4]=buttons[i][SDL_CONTROLLER_BUTTON_X]||buttons[i][SDL_CONTROLLER_BUTTON_Y];
   actions[i][5]=buttons[i][SDL_CONTROLLER_BUTTON_A];
   actions[i][6]=buttons[i][SDL_CONTROLLER_BUTTON_B];
   actions[i][7]=buttons[i][SDL_CONTROLLER_BUTTON_LEFTSHOULDER]||buttons[i][SDL_CONTROLLER_BUTTON_RIGHTSHOULDER]
    ||SDL_GameControllerGetAxis(devices[i],SDL_CONTROLLER_AXIS_TRIGGERLEFT)>16000
    ||SDL_GameControllerGetAxis(devices[i],SDL_CONTROLLER_AXIS_TRIGGERRIGHT)>16000;
  }
 }
 void finishUpdate() { std::memcpy(previous,actions,sizeof(actions));std::memcpy(oldButtons,buttons,sizeof(buttons)); }
 bool action(int player,int action,bool prev=false) const { return player>=1&&player<=4&&action>=0&&action<8&&(prev?previous[player-1][action]:actions[player-1][action]); }
 bool button(int slot,SDL_GameControllerButton b,bool prev=false) const { return slot>=0&&slot<4&&(prev?oldButtons[slot][b]:buttons[slot][b]); }
 bool any(SDL_GameControllerButton b) const { for(int i=0;i<4;i++)if(buttons[i][b])return true;return false; }
 bool connected(int slot) const { return slot>=0&&slot<4&&devices[slot]; }
 bool anyConnected() const { for(int i=0;i<4;i++)if(connected(i))return true;return false; }
 float axis(int slot,int axis) const { return axes[slot][axis]; }
};
}
