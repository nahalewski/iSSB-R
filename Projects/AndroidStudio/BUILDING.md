# ISSB-R Android

Verified debug build: ISSB-R 2.22.0, versionCode 2026091801, arm64-v8a.

Use JDK 17 or newer compatible with Gradle 8.14, Android SDK 36, NDK 28.2.13676358, and CMake 3.22.1. Set sdk.dir in your untracked local.properties (or configure ANDROID_HOME). Run `gradlew assembleDebug`. APK: app/build/outputs/apk/debug/app-debug.apk. The debug APK uses the standard local Android debug signing key; preserve that key for compatible updates to these installations. No signing key is committed.

The app targets API 35 and requires API 23 or newer. CMake builds ARM64 native libraries with a shared C++ runtime. The pinned SDL sources are left intact: sdl-android-compat.h adapts its sensor poll to modern NDK headers, and prepareSDLJava supplies current broadcast-receiver and USB PendingIntent flags. The zlib linker flag accommodates its legacy optional export.

On September 17, 2026, this build was installed via wireless ADB on a Google Pixel 9 Pro Fold (API 37) and Razer Edge 5G (API 36). Startup succeeded after correcting SDL's receiver-registration crash. Visual checks showed character selection on the Pixel and an active Ichigo-versus-Sonic match on the Razer. Exhaustive controller hardware testing and release signing are not part of this smoke test.
