/*
*********************************************
* Global definitions of vars and constants  *
*                                           *
* GlobalDefinitions.h                       *
*                                           *
* (c) 2017, 2018 Stefan Kubsch              *
*********************************************
*/

// Init main window settings
std::string WindowName;

// Position of window on screen (x/y)
uint_fast32_t WindowPosX;
uint_fast32_t WindowPosY;

// size of viewport
int_fast32_t WindowViewPortWidth;
int_fast32_t WindowViewPortHeight;

// middle of window
uint_fast32_t WindowWidthMid;
uint_fast32_t WindowHeightMid;

// complete size of window including controls, border etc.
uint_fast32_t WindowOverallWidth;
uint_fast32_t WindowOverallHeight;

// Setting planes/viewport for raycaster
float RayPosX;
float RayPosY;

float PlaneX;
float PlaneY;

// Current Level (will be replaced with dynamic solution...)
uint_fast32_t SelectedLevel;

// Size of textures (width and height)
uint_fast32_t TextureSize;

// Factor for bitwise texture operations
// Must be initialised with something or else -> no ceiling or walls are displayed
// is calculated correctly dependent on TextureSize in "GameConfig.h"
uint_fast32_t TextureSizeBitwiseAnd = 9999;

// Set factor for bitshifting from TextureSize
// 7 for 128x128, 8 for 256x256, 9 for 512x512, 10 for 1024x1024
// is calculated in "GameConfig.h" dependent on given TextureSize
uint_fast32_t TextureSizeShiftFactor;

// Initial settings for fixed timestep gameloop
uint_fast32_t FrameLock;
uint_fast32_t MaxFrameSkip;

// FPS Counter
uint_fast32_t FPSUpdate;
uint_fast32_t FPSFrames;
uint_fast32_t FPS;
uint_fast32_t SystemTime;

// Initial settings for Head Up Display (HUD)
Gdiplus::REAL HUDMenuPosX;
Gdiplus::REAL HUDMenuPosY;
bool HUDEnabled = true;
std::wstring HUDOptionLabel;
std::string HUDHelpText;
std::string HUDToggleMinimapText;
std::wstring HUDMouseSensitivityText;
std::wstring HUDExitText;

// Settings for MiniMap
bool MinimapEnabled = true;
uint_fast32_t MinimapPosX;
uint_fast32_t MinimapPosY;
uint_fast32_t MinimapTileSize;

// Initial settings for player movement speed
float PlayerMoveSpeed;
float PlayerCollisionDetectionFactor;

// crosshair settings
std::wstring CrosshairFileName;
uint_fast32_t CrosshairPosX;
uint_fast32_t CrosshairPosY;
// Mouse settings
float MouseSensitivity;

// Control key definitions
char MoveForwardKey;
char MoveBackwardKey;
char StrafeLeftKey;
char StrafeRightKey;
char HUDKey;
char MiniMapKey;