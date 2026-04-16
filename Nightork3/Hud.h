// Copyright (C) 2011-2026 by Maximilian Hönig

#ifndef HUD_H
#define HUD_H

#include <irrlicht.h>
#include <string>
#include "Color.h"
#include "Helper.h"
#include "MonsterTypes.h"

namespace Nightork
{

class CEventReceiver;
class CGameplayStats;
class CMonster;
class CMonsterManager;
class CPlayer;

class CHud
{
public:
	/// Constructor.
	CHud(irr::IrrlichtDevice* irrDevice_, CEventReceiver* eventReceiver_, CMonsterManager* monsterManager_, CPlayer* player_, CGameplayStats* gameplayStats_);

	/// Destructor.
	~CHud();

	/// Updates the Hud.
	void Update(clock_t actTime_);

	/// Set cancel dialog game overlay
	void SetCancelGameOverlay(bool value_);

private:
	/// Is the aimed monster inside the camera aiming cone?
	bool IsAimedMonsterValid() const;

private:
	CEventReceiver* EventReceiver;								///< The event receiver.
	irr::gui::IGUIEnvironment* GuiEnvironment;					///< The GUI environement.
	irr::scene::ISceneCollisionManager* SceneCollisionManager;	///< The scene collision manager.
	irr::video::IVideoDriver* VideoDriver;						///< The video driver.

	CMonsterManager* MonsterManager;							///< The monster manager.
	CPlayer* Player;											///< The player object.
	CGameplayStats* GameplayStats;								///< The gameplay statistics.
	CMonster* AimedMonster;										///< The monster that is in aim lock.

	irr::scene::ISceneNode* DungeonRoot;						///< The root node for monster and boss monster objects.

	std::string MonsterNames[EMonsterType::NUMBER_OF_MONSTERTYPES];///< The names of the monsters.
	std::string EnergyNames[EColor::NUMBER];					///< The names of the energies.

	unsigned int TSizeX;										///< X size of text.
	unsigned int TSizeY;										///< Y size of text.

	unsigned int RTPosX;										///< X position of right text.
	unsigned int RTPosY;										///< Y position of right text.

	unsigned int LTPosX;										///< X position of left text.
	unsigned int LTPosY;										///< Y position of left text.

	unsigned int MTPosX;										///< X position of middle text.
	unsigned int MTPosY;										///< Y position of middle text.

	unsigned int RBTPosX;										///< X position of right bottom text.
	unsigned int RBTPosY;										///< Y position of right bottom text.

	unsigned int LBTPosX;										///< X position of left bottom text.
	unsigned int LBTPosY;										///< Y position of left bottom text.

	unsigned int CTPosX;										///< X position of cancle game text.
	unsigned int CTPosY;										///< Y position of cancle game text.
	irr::gui::IGUIImage* Crosshair;								///< The crosshair.
	irr::gui::IGUIImage* Aiminghair;							///< The target marker.
	std::vector<irr::gui::IGUIImage*> Highlightings;			///< The highlightings of the monsters.

	irr::gui::IGUIStaticText* MonsterNameText;					///< Monster name.
	irr::gui::IGUIStaticText* MonsterDescriptionText;			///< Monster description text.
	irr::gui::IGUIStaticText* MonsterShieldPrimaryText;			///< Monster hitpoints red.
	irr::gui::IGUIStaticText* MonsterShieldSecondaryText;		///< Monster hitpoints green.
	irr::gui::IGUIStaticText* MonsterShieldTertiaryText;		///< Monster hitpoints blue.

	irr::gui::IGUIStaticText* PlayerHpText;						///< Player hitpoints.
	irr::gui::IGUIStaticText* PlayerAfterburnerText;			///< Player after burner.
	irr::gui::IGUIStaticText* PlayerEnergyChargeText;			///< Player energy charge.
	irr::gui::IGUIStaticText* PlayerPowerUpOverloadText;		///< Player overload text.

	irr::gui::IGUIStaticText* NearestMonsterText;				///< The nearest monster.
	irr::gui::IGUIStaticText* RemainingMonsterText; 			///< The remaining monsters.

	irr::gui::IGUIStaticText* CurrentDeathText;					///< The current number of player ship deaths.
	irr::gui::IGUIStaticText* CurrentPlaytimeText;				///< The played time.

	irr::gui::IGUIStaticText* CancelGameText1;					///< The cancle game dialog text 1.
	irr::gui::IGUIStaticText* CancelGameText2;					///< The cancle game dialog text 2.

	std::vector<irr::gui::IGUIStaticText*> CriticalDestructionTexts; ///< The texts for critical destructions.

};

}

#endif
