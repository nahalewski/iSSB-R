#include "AndroidBridge.h"
#include "Controls.h"

#ifdef __ANDROID__
#include <jni.h>
#include <SDL.h>
#include "../GameEngine/Output/Console.h"
#include <android/native_activity.h>

using namespace GameEngine;

// Get JNI environment using SDL's internal activity
static JNIEnv* getJNIEnv() {
	JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
	return env;
}

static jobject getActivity() {
	return (jobject)SDL_AndroidGetActivity();
}

namespace SmashBros
{
	namespace AndroidBridge
	{
		int getP1Character()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return -1;
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return -1;
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getP1Character", "()I");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find getP1Character method");
				env->DeleteLocalRef(activityClass);
				return -1;
			}
			
			jint result = env->CallStaticIntMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			
			return (int)result;
		}
		
		int getP2Character()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return -1;
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return -1;
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getP2Character", "()I");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find getP2Character method");
				env->DeleteLocalRef(activityClass);
				return -1;
			}
			
			jint result = env->CallStaticIntMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			
			return (int)result;
		}
		
		bool shouldSkipMenus()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return false;
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return false;
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "shouldSkipMenus", "()Z");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find shouldSkipMenus method");
				env->DeleteLocalRef(activityClass);
				return false;
			}
			
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			
			return (bool)result;
		}
		
		void clearCharacterSelections()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return;
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return;
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "clearCharacterSelections", "()V");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find clearCharacterSelections method");
				env->DeleteLocalRef(activityClass);
				return;
			}
			
			env->CallStaticVoidMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
		}
		
		void setTouchControlsEnabled(bool enabled)
		{
			Console::WriteLine((String)"AndroidBridge: Setting touch controls to " + (enabled ? "ON" : "OFF"));
			Controls::enableTouchControls(enabled);
		}
		
		bool getTouchControlsSetting()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return false; // Default OFF
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return false; // Default OFF
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getTouchControlsSetting", "()Z");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find getTouchControlsSetting method");
				env->DeleteLocalRef(activityClass);
				return false; // Default OFF
			}
			
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			
			return (bool)result;
		}
		
		int getSelectedStage()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return 0; // Default to first stage
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return 0; // Default to first stage
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getSelectedStage", "()I");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find getSelectedStage method");
				env->DeleteLocalRef(activityClass);
				return 0; // Default to first stage
			}
			
			jint result = env->CallStaticIntMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			
			Console::WriteLine((String)"AndroidBridge: Got stage selection: " + (int)result);
			return (int)result;
		}

		int getGameMode()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return 1;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return 1;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getGameMode", "()I");
			if(!methodID){ env->DeleteLocalRef(activityClass); return 1; }
			jint result = env->CallStaticIntMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (int)result;
		}

		int getItemFrequency()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return 2;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return 2;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getItemFrequency", "()I");
			if(!methodID){ env->DeleteLocalRef(activityClass); return 2; }
			jint result = env->CallStaticIntMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (int)result;
		}

		int getStockCount()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return 3;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return 3;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getStockCount", "()I");
			if(!methodID){ env->DeleteLocalRef(activityClass); return 3; }
			jint result = env->CallStaticIntMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (int)result;
		}

		bool getStageHazards()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return true;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return true;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getStageHazards", "()Z");
			if(!methodID){ env->DeleteLocalRef(activityClass); return true; }
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (bool)result;
		}

		bool getFinalSmashMeter()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return false;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return false;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getFinalSmashMeter", "()Z");
			if(!methodID){ env->DeleteLocalRef(activityClass); return false; }
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (bool)result;
		}
		
		bool getMenuMusicSetting()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return true;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return true;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getMenuMusicSetting", "()Z");
			if(!methodID){ env->DeleteLocalRef(activityClass); return true; }
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (bool)result;
		}
		
		bool getMenuSoundFxSetting()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return true;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return true;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getMenuSoundFxSetting", "()Z");
			if(!methodID){ env->DeleteLocalRef(activityClass); return true; }
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (bool)result;
		}
		
		bool getInGameMusicSetting()
		{
			JNIEnv* env = getJNIEnv();
			if(!env) return true;
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if(!activityClass) return true;
			jmethodID methodID = env->GetStaticMethodID(activityClass, "getInGameMusicSetting", "()Z");
			if(!methodID){ env->DeleteLocalRef(activityClass); return true; }
			jboolean result = env->CallStaticBooleanMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			return (bool)result;
		}
		
		void returnToCharacterSelect()
		{
			JNIEnv* env = getJNIEnv();
			if (!env) {
				Console::WriteLine("AndroidBridge: Failed to get JNIEnv");
				return;
			}
			
			jclass activityClass = env->FindClass("com/brokenphysics/issb/MainActivity");
			if (!activityClass) {
				Console::WriteLine("AndroidBridge: Failed to find MainActivity class");
				return;
			}
			
			jmethodID methodID = env->GetStaticMethodID(activityClass, "returnToCharacterSelect", "()V");
			if (!methodID) {
				Console::WriteLine("AndroidBridge: Failed to find returnToCharacterSelect method");
				env->DeleteLocalRef(activityClass);
				return;
			}
			
			env->CallStaticVoidMethod(activityClass, methodID);
			env->DeleteLocalRef(activityClass);
			
			Console::WriteLine("AndroidBridge: Returning to character select");
		}
	}
}

#else

// Stub implementations for non-Android platforms
namespace SmashBros
{
	namespace AndroidBridge
	{
		int getP1Character() { return -1; }
		int getP2Character() { return -1; }
		bool shouldSkipMenus() { return false; }
		void clearCharacterSelections() {}
		void setTouchControlsEnabled(bool enabled) {}
		bool getTouchControlsSetting() { return false; }
		int getSelectedStage() { return 0; }
		int getGameMode() { return 1; }
		int getItemFrequency() { return 2; }
		int getStockCount() { return 3; }
		bool getStageHazards() { return true; }
		bool getFinalSmashMeter() { return false; }
		bool getMenuMusicSetting() { return true; }
		bool getMenuSoundFxSetting() { return true; }
		bool getInGameMusicSetting() { return true; }
		void returnToCharacterSelect() {}
	}
}

#endif

