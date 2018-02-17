/*
************************************
* Functions for entity handling    *
*                                  *
* EntityHandling.h                 *
*                                  *
* (c) 2017, 2018 Stefan Kubsch     *
************************************
*/

#pragma once

//
// Declare functions
//

void InitEntityConfig(uint_fast32_t LevelNumber);
void InitEntityTextures(uint_fast32_t LevelNumber);
void ChangeEntityDirection(char NewDirection, uint_fast32_t i);
void MoveEntities();
void SortEntities();

//
// Variables and constants
//

// EntityMap, used for collision detection (same principle as walls)
// Currently used values : 0 = empty, 1 = EntityMapEnemyPos, 2 = EntityMapPlayerPos
std::vector<std::vector<uint_fast32_t>> EntityMap;

const uint_fast32_t EntityMapClearField	= 0;
const uint_fast32_t EntityMapEnemyPos	= 1;
const uint_fast32_t EntityMapPlayerPos	= 2;

// 1D Zbuffer
std::vector<float> ZBuffer;

// Vectors used to sort the entities
std::vector<uint_fast32_t> EntityOrder;
std::vector<float> EntityDistance;

// Variables for scaling entities
uint_fast32_t EntitySize;
uint_fast32_t EntitySizeDivU;
uint_fast32_t EntitySizeDivV;
float EntityMoveV;

// Vector and variables used for entities init
std::vector<EntityStruct> Entity;
uint_fast32_t NumberOfUsedEntities;
uint_fast32_t NumberOfUsedEntityTextures;

//
// Functions
//

void InitEntityConfig(uint_fast32_t LevelNumber)
{
	char TempVal[100];
	
	// Build filepath/filename dynamically with LevelNumber
	std::string PathPart1 = "./DATA/EntityData/Level_";
	std::string PathPart2 = std::to_string(LevelNumber);
	std::string PathPart3 = "/Level_";
	std::string PathPart4 = std::to_string(LevelNumber);
	std::string PathPart5 = "_Entity_Config.ini";

	std::string IniFile = PathPart1 + PathPart2 + PathPart3 + PathPart4 + PathPart5;
	
	NumberOfUsedEntities = GetPrivateProfileInt("GENERAL", "NumberOfUsedEntities", 3, IniFile.c_str());
	
	NumberOfUsedEntityTextures = GetPrivateProfileInt("GENERAL", "NumberOfUsedEntityTextures", 8, IniFile.c_str());
	
	EntitySize = GetPrivateProfileInt("GENERAL", "EntitySize", 1024, IniFile.c_str());
	
	EntitySizeDivU = GetPrivateProfileInt("GENERAL", "EntitySizeDivU", 1, IniFile.c_str());
	
	EntitySizeDivV = GetPrivateProfileInt("GENERAL", "EntitySizeDivV", 1, IniFile.c_str());
	
	GetPrivateProfileString("GENERAL", "EntityMoveV", "64.0", TempVal, 100, IniFile.c_str());
	EntityMoveV = static_cast<float>(atof(TempVal));

	// Resize vectors to needed sizes
	ZBuffer.resize(WindowWidth);
	EntityOrder.resize(NumberOfUsedEntities);
	EntityDistance.resize(NumberOfUsedEntities);
	EntityMap = std::vector<std::vector<uint_fast32_t>>(MapWidth, std::vector<uint_fast32_t>(MapHeight, 0));
	Entity.resize(NumberOfUsedEntities);

	for (uint_fast32_t i = 0; i < NumberOfUsedEntities; ++i)
	{
		Entity[i].Number = i;
		
		// Build filepath/filename dynamically with LevelNumber and Entity.Number
		std::string PathPart1 = "./DATA/EntityData/Level_";
		std::string PathPart2 = std::to_string(LevelNumber);
		std::string PathPart3 = "/Level_";
		std::string PathPart4 = std::to_string(LevelNumber);
		std::string PathPart5 = "_Entity_";
		std::string PathPart6 = std::to_string(i);
		std::string PathPart7 = "_Data.ini";

		IniFile = PathPart1 + PathPart2 + PathPart3 + PathPart4 + PathPart5 + PathPart6 + PathPart7;

		// read data from ini file
		GetPrivateProfileString("ENTITY", "Name", "Dummy", TempVal, 100, IniFile.c_str());
		Entity[i].Name = TempVal;
		
		GetPrivateProfileString("ENTITY", "StartPosX", "1.5", TempVal, 100, IniFile.c_str());
		Entity[i].StartPosX = static_cast<float>(atof(TempVal));
		
		GetPrivateProfileString("ENTITY", "StartPosY", "1.5", TempVal, 100, IniFile.c_str());
		Entity[i].StartPosY = static_cast<float>(atof(TempVal));
		
		GetPrivateProfileString("ENTITY", "EndPosX", "0.0", TempVal, 100, IniFile.c_str());
		Entity[i].EndPosX = static_cast<float>(atof(TempVal));
		
		GetPrivateProfileString("ENTITY", "EndPosY", "0.0", TempVal, 100, IniFile.c_str());
		Entity[i].EndPosY = static_cast<float>(atof(TempVal));
		
		GetPrivateProfileString("ENTITY", "MoveSpeed", "0.02", TempVal, 100, IniFile.c_str());
		Entity[i].MoveSpeed = static_cast<float>(atof(TempVal));
		
		GetPrivateProfileString("ENTITY", "MoveSpeed", "0.02", TempVal, 100, IniFile.c_str());
		Entity[i].MoveSpeed = static_cast<float>(atof(TempVal));
		
		Entity[i].MovementBehaviour = GetPrivateProfileInt("ENTITY", "MovementBehaviour", 1, IniFile.c_str());
		
		GetPrivateProfileString("ENTITY", "DirX", "-1.0", TempVal, 100, IniFile.c_str());
		Entity[i].DirX = static_cast<float>(atof(TempVal));
		
		GetPrivateProfileString("ENTITY", "DirY", "0.0", TempVal, 100, IniFile.c_str());
		Entity[i].DirY = static_cast<float>(atof(TempVal));

		// Set initial position of entity
		Entity[i].PosX = Entity[i].StartPosX;
		Entity[i].PosY = Entity[i].StartPosY;
	}
}

void InitEntityTextures(uint_fast32_t LevelNumber)
{	
	for (uint_fast32_t i = 0; i < NumberOfUsedEntities; ++i)
	{
		std::string TempString;
		uint_fast32_t j = 0;

		// Build filepath/filename dynamically with LevelNumber and Entity.Number
		std::string PathPart1 = "./DATA/EntityData/Level_";
		std::string PathPart2 = std::to_string(LevelNumber);
		std::string PathPart3 = "/Level_";
		std::string PathPart4 = std::to_string(LevelNumber);
		std::string PathPart5 = "_Entity_";
		std::string PathPart6 = std::to_string(i);
		std::string PathPart7 = "_TexturesData.conf";

		std::string FileName = PathPart1 + PathPart2 + PathPart3 + PathPart4 + PathPart5 + PathPart6 + PathPart7;

		// Read texture filenames from file
		std::ifstream EntityTexturesDataFile;
		EntityTexturesDataFile.open(FileName, std::ios::in);
		
		// resize 3D vector
		Entity[i].Texture.resize(NumberOfUsedEntityTextures);
		
		if (EntityTexturesDataFile.good())
		{
			while (std::getline(EntityTexturesDataFile, TempString))
			{
				// Load & lock texture

				std::wstring FileName = std::wstring(TempString.begin(), TempString.end());
		
				Gdiplus::Bitmap EntityIMG(FileName.c_str(), PixelFormat32bppPARGB);
				LockBits EntityIMGLB(EntityIMG);
				EntityIMGLB.Lock();

				// resize X vector
				Entity[i].Texture[j].resize(EntitySize);

				for (uint_fast32_t x = 0; x < EntitySize; ++x)
				{
					// resize Y vector
					Entity[i].Texture[j][x].resize(EntitySize);

					for (uint_fast32_t y = 0; y < EntitySize; ++y)
					{
						Entity[i].Texture[j][x][y] = EntityIMGLB.GetPixel(x, y);
					}
				}

				EntityIMGLB.Release();

				++j;
			}

			EntityTexturesDataFile.close();
		}
	}
}

void ChangeEntityDirection(char NewDirection, uint_fast32_t i)
{
	// Possible directions (DirX / DirY)
	//		-1.0 / 0.0		Up
	//		1.0 / 0.0		Down
	//		0.0 / 1.0		Right
	//		0.0 / -1.0		Left

	switch (NewDirection)
	{
		case 'l':
		{
			// Turn left if moved up previously
			if (Entity[i].DirX == -1.0f)
			{
				Entity[i].DirX = 0.0;;
				Entity[i].DirY = -1.0f;
				return;
			}

			// Turn left if moved down previously
			if (Entity[i].DirX == 1.0f)
			{
				Entity[i].DirX = 0.0;;
				Entity[i].DirY = 1.0f;
				return;
			}

			// Turn left if moved right previously
			if (Entity[i].DirY == 1.0f)
			{
				Entity[i].DirX = -1.0;;
				Entity[i].DirY = 0.0f;
				return;
			}

			// Turn left if moved left previously
			if (Entity[i].DirY == -1.0f)
			{
				Entity[i].DirX = 1.0;;
				Entity[i].DirY = 0.0f;
				return;
			}

			break;
		}

		case 'r':
		{
			// Turn right if moved up previously
			if (Entity[i].DirX == -1.0f)
			{
				Entity[i].DirX = 0.0;;
				Entity[i].DirY = 1.0f;
				return;
			}

			// Turn right if moved down previously
			if (Entity[i].DirX == 1.0f)
			{
				Entity[i].DirX = 0.0;;
				Entity[i].DirY = -1.0f;
				return;
			}

			// Turn right if moved right previously
			if (Entity[i].DirY == 1.0f)
			{
				Entity[i].DirX = 1.0;;
				Entity[i].DirY = 0.0f;
				return;
			}

			// Turn right if moved left previously
			if (Entity[i].DirY == -1.0f)
			{
				Entity[i].DirX = -1.0;;
				Entity[i].DirY = 0.0f;
				return;
			}

			break;
		}
	}
}

void MoveEntities()
{
	for (uint_fast32_t i = 0; i < NumberOfUsedEntities; ++i)
	{
		float EntityCollisionDetectionFactor = Entity[i].MoveSpeed + 0.5f;
				
		switch (Entity[i].MovementBehaviour)
		{
			case 2:
			{
				//
				// Free roaming mode
				//
			
				// Clear old entry on EntityMap
				EntityMap[static_cast<uint_fast32_t>(Entity[i].PosX)][static_cast<uint_fast32_t>(Entity[i].PosY)] = EntityMapClearField;

				// Change direction by chance
				if (static_cast<uint_fast32_t>(rand() % 500) == 11)
				{
					// Random choice of new direction (left or right)
					// ChangeEntityDirection("lr"[rand() % 2], i);
				}
				
				// Wait by chance
				// Check if a chance hit occured and if no current timer is running
				if (static_cast<uint_fast32_t>(rand() % 666) == 99 && Entity[i].WaitTimer == 0)
				{
					Entity[i].WaitTimer = static_cast<uint_fast32_t>(rand() % 200);
				}

				if (Entity[i].WaitTimer > 0)
				{
					Entity[i].WaitTimer -= 1;
				}
				else
				{
					// Move forward
					Entity[i].PosX += Entity[i].DirX * Entity[i].MoveSpeed;
					Entity[i].PosY += Entity[i].DirY * Entity[i].MoveSpeed;
				}

				// Collision detection against walls, player and other entities
				if ((LevelMap[static_cast<uint_fast32_t>(Entity[i].PosX + Entity[i].DirX * EntityCollisionDetectionFactor)][static_cast<uint_fast32_t>(Entity[i].PosY + Entity[i].DirY * EntityCollisionDetectionFactor)])
					|| (EntityMap[static_cast<uint_fast32_t>(Entity[i].PosX + Entity[i].DirX * EntityCollisionDetectionFactor)][static_cast<uint_fast32_t>(Entity[i].PosY + Entity[i].DirY * EntityCollisionDetectionFactor)]))
				{
					Entity[i].PosX -= Entity[i].DirX * Entity[i].MoveSpeed;
					Entity[i].PosY -= Entity[i].DirY * Entity[i].MoveSpeed;

					// Random choice of new direction (left or right)
					ChangeEntityDirection("lr"[rand() % 2], i);
				}

				break;
			}
		}

		// Remark new position of entity on EntityMap (used for collision detection)
		EntityMap[static_cast<uint_fast32_t>(Entity[i].PosX)][static_cast<uint_fast32_t>(Entity[i].PosY)] = EntityMapEnemyPos;
	}
}

void SortEntities()
{
	for (uint_fast32_t i = 0; i < NumberOfUsedEntities; ++i)
	{
		EntityOrder[i] = i;
		EntityDistance[i] = ((PlayerPosX - Entity[i].PosX) * (PlayerPosX - Entity[i].PosX) + (PlayerPosY - Entity[i].PosY) * (PlayerPosY - Entity[i].PosY));
	}

	// Selection sort of entities in depth
	for (uint_fast32_t i = 0; i < NumberOfUsedEntities - 1; ++i)
	{
		float Min = EntityDistance[i];
		uint_fast32_t Index = i;
		for (uint_fast32_t j = i + 1; j < NumberOfUsedEntities; ++j)
		{
			if (Min < EntityDistance[j])
			{
				Min = EntityDistance[j];
				Index = j;
			}
		}

		std::swap(EntityDistance[i], EntityDistance[Index]);
		std::swap(EntityOrder[i], EntityOrder[Index]);
	}
}