// Copyright (C) 2011-2026 by Maximilian Hönig

#include <cmath>
#include "Constants.h"
#include "EventReceiver.h"
#include "GameplayStats.h"
#include "Hud.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Player.h"

Nightork::CHud::CHud(irr::IrrlichtDevice* irrDevice_,  CEventReceiver* eventReceiver_, CMonsterManager* monsterManager_, CPlayer* player_, CGameplayStats* gameplayStats_)
	: EventReceiver(eventReceiver_)
	, GuiEnvironment(irrDevice_->getGUIEnvironment())
	, SceneCollisionManager(irrDevice_->getSceneManager()->getSceneCollisionManager())
	, VideoDriver(irrDevice_->getVideoDriver())
	, MonsterManager(monsterManager_)
	, Player(player_)
	, GameplayStats(gameplayStats_)
	, AimedMonster(NULL)
	, DungeonRoot(irrDevice_->getSceneManager()->getSceneNodeFromName("DungeonRoot"))
	, CancelGameText1(NULL)
	, CancelGameText2(NULL)
	, Highlightings()
{
	// set monster and boss monster names
	MonsterNames[EMonsterType::SHIP_RED] = "Elemental Demon";
	MonsterNames[EMonsterType::SHIP_GREEN] = "Nature Demon";
	MonsterNames[EMonsterType::SHIP_BLUE] = "Chaos Demon";
	MonsterNames[EMonsterType::SHIP_YELLOW] = "Order Demon";
	MonsterNames[EMonsterType::SHIP_TEAL] = "Life Demon";
	MonsterNames[EMonsterType::SHIP_PURPLE] = "Arcane Demon";

	MonsterNames[EMonsterType::BOSS_01] = "Zhaa'Krull - Demon of Hate";
	MonsterNames[EMonsterType::BOSS_02] = "Seech - Demon of Corruption";
	MonsterNames[EMonsterType::BOSS_03] = "Yarok Zunitzu - Demon of Fear";
	MonsterNames[EMonsterType::BOSS_04] = "Doomknight - Demonic General";
	MonsterNames[EMonsterType::BOSS_05] = "Kharn - Demon of Agony";
	MonsterNames[EMonsterType::BOSS_06] = "Zoruk - Demon of Entropy";

	EnergyNames[EColor::MAGIC] = "Magic";
	EnergyNames[EColor::ANTIMAGIC] = "Antimagic";

	// set mouse cursor to center screen
	irrDevice_->getCursorControl()->setPosition(0.5f, 0.5f);

	// add crosshair
	irr::video::ITexture * chTex = VideoDriver->getTexture("data/Crosshair_white.png");
	irr::core::dimension2du screenSize = VideoDriver->getScreenSize();
	Crosshair = GuiEnvironment->addImage(chTex,
		irr::core::position2di(screenSize.Width/2-16,screenSize.Height/2-16),	// position
		true,0,-1,0);	// alpha channel?, parent node, id, text
	Aiminghair = GuiEnvironment->addImage(chTex,
		irr::core::position2di(screenSize.Width / 2 - 16, screenSize.Height / 2 - 16),	// position
		true, 0, -1, 0);	// alpha channel?, parent node, id, text

	// add texts
	TSizeX = Constants::Hud::SizeX;
	TSizeY = Constants::Hud::SizeY;

	RTPosX = screenSize.Width-Constants::Hud::BorderText-TSizeX;
	RTPosY = Constants::Hud::BorderText;
	LTPosX = Constants::Hud::BorderText;
	LTPosY = Constants::Hud::BorderText;
	MTPosX = (screenSize.Width-Constants::Hud::SizeX)/2;
	MTPosY = Constants::Hud::PosHpBarY+Constants::Hud::SizeHpBarY+1+Constants::Hud::BorderBackground;
	RBTPosX = screenSize.Width-Constants::Hud::BorderText-TSizeX;
	RBTPosY = screenSize.Height-Constants::Hud::BorderText-2*TSizeY;
	LBTPosX = Constants::Hud::BorderText;
	LBTPosY = screenSize.Height-Constants::Hud::BorderText-3*TSizeY;
	CTPosX = (screenSize.Width-Constants::Hud::SizeX)/2;
	CTPosY = (screenSize.Height-Constants::Hud::SizeY)/2-3*Constants::Hud::SizeY;

	MonsterDescriptionText = GuiEnvironment->addStaticText(L"",irr::core::recti(RTPosX, RTPosY, RTPosX+TSizeX, RTPosY+TSizeY));
	MonsterDescriptionText->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_UPPERLEFT);
	MonsterDescriptionText->setOverrideColor(irr::video::SColor(255,255,255,255));
	MonsterDescriptionText->enableOverrideColor(true);
	MonsterShieldPrimaryText = GuiEnvironment->addStaticText(L"",irr::core::recti(RTPosX, RTPosY+TSizeY, RTPosX+TSizeX, RTPosY+2*TSizeY));
	MonsterShieldPrimaryText->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_UPPERLEFT);
	MonsterShieldPrimaryText->setOverrideColor(irr::video::SColor(255,255,255,255));
	MonsterShieldPrimaryText->enableOverrideColor(true);
	MonsterShieldSecondaryText = GuiEnvironment->addStaticText(L"",irr::core::recti(RTPosX, RTPosY+2*TSizeY, RTPosX+TSizeX, RTPosY+3*TSizeY));
	MonsterShieldSecondaryText->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_UPPERLEFT);
	MonsterShieldSecondaryText->setOverrideColor(irr::video::SColor(255,255,255,255));
	MonsterShieldSecondaryText->enableOverrideColor(true);
	MonsterShieldTertiaryText = GuiEnvironment->addStaticText(L"",irr::core::recti(RTPosX, RTPosY+3*TSizeY, RTPosX+TSizeX, RTPosY+4*TSizeY));
	MonsterShieldTertiaryText->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_UPPERLEFT);
	MonsterShieldTertiaryText->setOverrideColor(irr::video::SColor(255,255,255,255));
	MonsterShieldTertiaryText->enableOverrideColor(true);
	MonsterNameText = GuiEnvironment->addStaticText(L"",irr::core::recti(MTPosX, MTPosY, MTPosX+TSizeX, MTPosY+TSizeY));
	MonsterNameText->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	MonsterNameText->setOverrideColor(irr::video::SColor(255,255,255,255));
	MonsterNameText->enableOverrideColor(true);

	irr::gui::IGUIStaticText* wargText = GuiEnvironment->addStaticText(L"Warg XJ-117 system status:",irr::core::recti(LTPosX, LTPosY, LTPosX+TSizeX, LTPosY+TSizeY));
	wargText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	wargText->setOverrideColor(irr::video::SColor(255,255,255,255));
	wargText->enableOverrideColor(true);
	PlayerHpText = GuiEnvironment->addStaticText(L"Hull: XXX/XXX",irr::core::recti(LTPosX, LTPosY+TSizeY, LTPosX+TSizeX, LTPosY+2*TSizeY));
	PlayerHpText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	PlayerHpText->setOverrideColor(irr::video::SColor(255,255,255,255));
	PlayerHpText->enableOverrideColor(true);
	PlayerAfterburnerText = GuiEnvironment->addStaticText(L"Afterburner: XXX/XXX",irr::core::recti(LTPosX, LTPosY+2*TSizeY, LTPosX+TSizeX, LTPosY+3*TSizeY));
	PlayerAfterburnerText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	PlayerAfterburnerText->setOverrideColor(irr::video::SColor(255,255,255,255));
	PlayerAfterburnerText->enableOverrideColor(true);
	PlayerEnergyChargeText = GuiEnvironment->addStaticText(L"Weapons: XXX/XXX",irr::core::recti(LTPosX, LTPosY+3*TSizeY, LTPosX+TSizeX, LTPosY+4*TSizeY));
	PlayerEnergyChargeText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	PlayerEnergyChargeText->setOverrideColor(irr::video::SColor(255,255,255,255));
	PlayerEnergyChargeText->enableOverrideColor(true);

	PlayerPowerUpOverloadText = GuiEnvironment->addStaticText(L"Overload!", irr::core::recti(screenSize.Width / 2 - 50, screenSize.Height / 2 - 2 * TSizeY, screenSize.Width / 2 + 50, screenSize.Height / 2 - 1 * TSizeY));
	PlayerPowerUpOverloadText->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
	PlayerPowerUpOverloadText->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
	PlayerPowerUpOverloadText->enableOverrideColor(true);
	PlayerPowerUpOverloadText->setVisible(false);

	CurrentPlaytimeText = GuiEnvironment->addStaticText(L"Time: 0",irr::core::recti(RBTPosX, RBTPosY, RBTPosX+TSizeX, RBTPosY+TSizeY));
	CurrentPlaytimeText->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_UPPERLEFT);
	CurrentPlaytimeText->setOverrideColor(irr::video::SColor(255,255,255,255));
	CurrentPlaytimeText->enableOverrideColor(true);
	CurrentDeathText = GuiEnvironment->addStaticText(L"Deaths: 0",irr::core::recti(RBTPosX, RBTPosY+TSizeY, RBTPosX+TSizeX, RBTPosY+2*TSizeY));
	CurrentDeathText->setTextAlignment(irr::gui::EGUIA_LOWERRIGHT,irr::gui::EGUIA_UPPERLEFT);
	CurrentDeathText->setOverrideColor(irr::video::SColor(255,255,255,255));
	CurrentDeathText->enableOverrideColor(true);

	irr::gui::IGUIStaticText* scanText = GuiEnvironment->addStaticText(L"Long range scan:",irr::core::recti(LBTPosX, LBTPosY, LTPosX+TSizeX, LBTPosY+TSizeY));
	scanText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	scanText->setOverrideColor(irr::video::SColor(255,255,255,255));
	scanText->enableOverrideColor(true);
	NearestMonsterText = GuiEnvironment->addStaticText(L"Nearest enemy: 0",irr::core::recti(LBTPosX, LBTPosY+TSizeY, LBTPosX+TSizeX, LBTPosY+2*TSizeY));
	NearestMonsterText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	NearestMonsterText->setOverrideColor(irr::video::SColor(255,255,255,255));
	NearestMonsterText->enableOverrideColor(true);
	RemainingMonsterText = GuiEnvironment->addStaticText(L"Remaining enemies: 0",irr::core::recti(LBTPosX, LBTPosY+2*TSizeY, LBTPosX+TSizeX, LBTPosY+3*TSizeY));
	RemainingMonsterText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT,irr::gui::EGUIA_UPPERLEFT);
	RemainingMonsterText->setOverrideColor(irr::video::SColor(255,255,255,255));
	RemainingMonsterText->enableOverrideColor(true);
}

Nightork::CHud::~CHud()
{
}

void Nightork::CHud::Update(clock_t actTime_)
{
	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	// aim
	irr::core::vector3df shootPos = Player->GetAbsoluteShootPosition();
	irr::core::vector2di crosshairPos = SceneCollisionManager->getScreenCoordinatesFrom3DPosition(shootPos);

	irr::core::vector3df aimingPos;
	irr::core::triangle3df tmpTri;
	irr::core::line3df ray = SceneCollisionManager->getRayFromScreenCoordinates(crosshairPos);
	ray.setLine(ray.start+ray.getVector().normalize()*Constants::Hud::AimingOffset,ray.end);
	irr::scene::ISceneNode* node = SceneCollisionManager->getSceneNodeAndCollisionPointFromRay(
		ray,
		aimingPos,
		tmpTri,
		0,
		DungeonRoot);

	// crosshair
	{
		Crosshair->remove();
		irr::video::ITexture* chTex = Player->IsPowerUpActive() ? VideoDriver->getTexture("data/Crosshair_blue.png") : VideoDriver->getTexture("data/Crosshair_white.png");
		Crosshair = GuiEnvironment->addImage(chTex,
			crosshairPos - irr::core::position2di(16, 16),	// position
			true, 0, -1, 0);								// alpha channel?, parent node, id, text

		PlayerPowerUpOverloadText->remove();
		PlayerPowerUpOverloadText = GuiEnvironment->addStaticText(L"Overload!", irr::core::recti(crosshairPos.X - 50, crosshairPos.Y - 24 - TSizeY, crosshairPos.X + 50, crosshairPos.Y - 24));
		PlayerPowerUpOverloadText->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
		PlayerPowerUpOverloadText->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
		PlayerPowerUpOverloadText->enableOverrideColor(true);
		PlayerPowerUpOverloadText->setVisible(Player->IsPowerUpActive() && (actTime_ - Player->GetPowerUpStartTick()) <= Constants::PowerUp::DurationOfText);
	}

	const int crosshairDimensionHalf = 64;
	const irr::video::SColor colorMagic(128, 0, 255, 0);
	const irr::video::SColor colorAntimagic(128, 255, 96, 0);

	// monster information
	{
		irr::core::stringw text = "";
		irr::core::stringw text2 = "";
		irr::core::stringw text3 = "";
		irr::core::stringw text4 = "";
		irr::core::stringw text5 = "";

		CMonster* monster = MonsterManager->GetMonster(node);
		if (monster == NULL && IsAimedMonsterValid())
		{
			monster = AimedMonster;
		}
		AimedMonster = monster;

		if (Aiminghair)
		{
			Aiminghair->remove();
			Aiminghair = NULL;
		}

		irr::core::vector3df monsterPos;
		irr::core::vector2di aiminghairPos;

		if (monster)
		{
			text = MonsterNames[monster->GetMonsterType()].c_str();
			text2 = irr::core::stringw("Primary: ")
				+ EnergyNames[monster->GetHitpointsColor(EShield::PRIMARY)].c_str() + irr::core::stringw(" ")
				+ irr::core::stringw(monster->GetHitpointsCurrent(EShield::PRIMARY)) + irr::core::stringw("/")
				+ irr::core::stringw(monster->GetHitpointsMaximum(EShield::PRIMARY));
			text3 = irr::core::stringw("Secondary: ")
				+ EnergyNames[monster->GetHitpointsColor(EShield::SECONDARY)].c_str() + irr::core::stringw(" ")
				+ irr::core::stringw(monster->GetHitpointsCurrent(EShield::SECONDARY)) + irr::core::stringw("/")
				+ irr::core::stringw(monster->GetHitpointsMaximum(EShield::SECONDARY));
			text4 = irr::core::stringw("Tertiary: ")
				+ EnergyNames[monster->GetHitpointsColor(EShield::TERTIARY)].c_str() + irr::core::stringw(" ")
				+ irr::core::stringw(monster->GetHitpointsCurrent(EShield::TERTIARY)) + irr::core::stringw("/")
				+ irr::core::stringw(monster->GetHitpointsMaximum(EShield::TERTIARY));
			text5 = irr::core::stringw("Target status:");

			monsterPos = monster->GetMonsterNode()->getAbsolutePosition();
			aiminghairPos = SceneCollisionManager->getScreenCoordinatesFrom3DPosition(monsterPos);

			irr::video::ITexture* ahTex = VideoDriver->getTexture("data/Aiminghair.png");
			Aiminghair = GuiEnvironment->addImage(ahTex,
				aiminghairPos - irr::core::position2di(crosshairDimensionHalf, crosshairDimensionHalf),	// position
				true, 0, -1, 0);								// alpha channel?, parent node, id, text
		}

		MonsterNameText->setText(text.c_str());
		MonsterShieldPrimaryText->setText(text2.c_str());
		MonsterShieldSecondaryText->setText(text3.c_str());
		MonsterShieldTertiaryText->setText(text4.c_str());
		MonsterDescriptionText->setText(text5.c_str());

		if (Aiminghair)
		{
			VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 0),
				irr::core::recti(RTPosX - Constants::Hud::BorderBackground, RTPosY - Constants::Hud::BorderBackground,
					RTPosX + TSizeX + Constants::Hud::BorderBackground, RTPosY + 4 * TSizeY + Constants::Hud::BorderBackground));
			VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 0),
				irr::core::recti(MTPosX - Constants::Hud::BorderBackground, MTPosY - Constants::Hud::BorderBackground,
					MTPosX + TSizeX + Constants::Hud::BorderBackground, MTPosY + TSizeY + Constants::Hud::BorderBackground));

			if (monster->GetHitpointsCurrent(EShield::PRIMARY) > 0)
			{
				const irr::video::SColor barColor = monster->GetHitpointsColor(EShield::PRIMARY) == EColor::MAGIC ? colorMagic : colorAntimagic;

				const float ratio = static_cast<float>(monster->GetHitpointsCurrent(EShield::PRIMARY)) / static_cast<float>(monster->GetHitpointsMaximum(EShield::PRIMARY));
				const int extend = static_cast<int>(ratio * crosshairDimensionHalf);
				VideoDriver->draw2DRectangle(barColor, irr::core::recti(aiminghairPos.X - extend, aiminghairPos.Y - 128,
					aiminghairPos.X + extend, aiminghairPos.Y - 112));
			}
			if (monster->GetHitpointsCurrent(EShield::SECONDARY) > 0)
			{
				const irr::video::SColor barColor = monster->GetHitpointsColor(EShield::SECONDARY) == EColor::MAGIC ? colorMagic : colorAntimagic;

				const float ratio = static_cast<float>(monster->GetHitpointsCurrent(EShield::SECONDARY)) / static_cast<float>(monster->GetHitpointsMaximum(EShield::SECONDARY));
				const int extend = static_cast<int>(ratio * crosshairDimensionHalf);
				VideoDriver->draw2DRectangle(barColor, irr::core::recti(aiminghairPos.X - extend, aiminghairPos.Y - 104,
					aiminghairPos.X + extend, aiminghairPos.Y - 88));
			}
			if (monster->GetHitpointsCurrent(EShield::TERTIARY) > 0)
			{
				const irr::video::SColor barColor = monster->GetHitpointsColor(EShield::TERTIARY) == EColor::MAGIC ? colorMagic : colorAntimagic;

				const float ratio = static_cast<float>(monster->GetHitpointsCurrent(EShield::TERTIARY)) / static_cast<float>(monster->GetHitpointsMaximum(EShield::TERTIARY));
				const int extend = static_cast<int>(ratio * crosshairDimensionHalf);
				VideoDriver->draw2DRectangle(barColor, irr::core::recti(aiminghairPos.X - extend, aiminghairPos.Y - 80,
					aiminghairPos.X + extend, aiminghairPos.Y - 64));
			}
		}
	}

	// highlightings
	{
		for (unsigned int i = 0; i < Highlightings.size(); ++i)
		{
			Highlightings[i]->remove();
		}
		Highlightings.clear();

		if (EventReceiver->IsManeuver(CPilot::SControls::Highlighting))
		{
			irr::video::ITexture* highlightTexture = VideoDriver->getTexture("data/Highlighting.png");
			const std::vector<CMonster*> monsters = *MonsterManager->GetMonsters();
			for (unsigned int i = 0; i < monsters.size(); ++i)
			{
				const irr::core::vector3df monsterWorldPos = monsters[i]->GetMonsterNode()->getAbsolutePosition();
				const irr::core::vector2di monsterScreenPos = SceneCollisionManager->getScreenCoordinatesFrom3DPosition(monsterWorldPos);

				irr::gui::IGUIImage* highlighting = GuiEnvironment->addImage(highlightTexture,
					monsterScreenPos - irr::core::position2di(crosshairDimensionHalf, crosshairDimensionHalf),	// position
					true, 0, -1, 0);
				Highlightings.push_back(highlighting);
			}
		}
	}

	// upper bars
	{
		const float minHp = static_cast<float>(2 * Constants::Hud::BorderText + Constants::Hud::SizeX);
		const float maxWp= static_cast<float>(screenSize.Width) - minHp;
		const float length = maxWp - minHp;
		const float lengthThird = length * 0.3333333f;

		const float maxHp = minHp + lengthThird - 5;
		const float minAb = maxHp + 5;
		const float maxAb = minAb + lengthThird - 5;
		const float minWp = maxAb + 5;

		const float actHp = minHp + (maxHp - minHp) * (static_cast<float>(Player->GetHpCurrent()) / static_cast<float>(Player->GetHpMaximum()) );
		const float actAb = minAb + (maxAb - minAb) * (static_cast<float>(Player->GetAfterburnerCurrent()) / static_cast<float>(Player->GetAfterburnerMaximum()));
		const float actWp = minWp + (maxWp - minWp) * (static_cast<float>(Player->GetEnergyCurrent()) / static_cast<float>(Player->GetEnergyMaximum()));

		VideoDriver->draw2DRectangle(irr::video::SColor(128, 255, 0, 0), irr::core::recti(static_cast<unsigned int>(minHp), Constants::Hud::PosHpBarY,
			static_cast<unsigned int>(actHp), Constants::Hud::PosHpBarY + Constants::Hud::SizeHpBarY));

		VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 255, 0), irr::core::recti(static_cast<unsigned int>(minAb), Constants::Hud::PosHpBarY,
			static_cast<unsigned int>(actAb) - 5, Constants::Hud::PosHpBarY + Constants::Hud::SizeHpBarY));

		VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 255), irr::core::recti(static_cast<unsigned int>(minWp), Constants::Hud::PosHpBarY,
			static_cast<unsigned int>(actWp), Constants::Hud::PosHpBarY + Constants::Hud::SizeHpBarY));
	}

	// critical destructions
	{
		for (int i = 0; i < CriticalDestructionTexts.size(); ++i)
			CriticalDestructionTexts[i]->remove();
		CriticalDestructionTexts.clear();

		const std::vector<std::pair<irr::core::vector3df, clock_t> >* criticalDestructions = MonsterManager->GetCriticalDestructions();
		for (int i = 0; i < criticalDestructions->size(); ++i)
		{
			irr::core::vector2di screenPos = SceneCollisionManager->getScreenCoordinatesFrom3DPosition(criticalDestructions->at(i).first);
			if (screenPos.X >= 0 && screenPos.Y >= 0)
			{
				irr::gui::IGUIStaticText* critical = GuiEnvironment->addStaticText(L"Critical!", irr::core::recti(screenPos.X, screenPos.Y, screenPos.X + 100, screenPos.Y + Constants::Hud::SizeY));
				critical->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
				critical->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
				critical->enableOverrideColor(true);

				CriticalDestructionTexts.push_back(critical);
			}
		}
	}

	// player information
	{
		PlayerHpText->setText((irr::core::stringw("Hull: ") + irr::core::stringw(Player->GetHpCurrent())
			+ irr::core::stringw("/") + irr::core::stringw(Player->GetHpMaximum())).c_str());
		PlayerAfterburnerText->setText((irr::core::stringw("Afterburner: ") + irr::core::stringw(Player->GetAfterburnerCurrent())
			+ irr::core::stringw("/") + irr::core::stringw(Player->GetAfterburnerMaximum())).c_str());
		PlayerEnergyChargeText->setText((irr::core::stringw("Weapons: ") + irr::core::stringw(Player->GetEnergyCurrent())
			+ irr::core::stringw("/") + irr::core::stringw(Player->GetEnergyMaximum())).c_str());

		VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 0),
			irr::core::recti(LTPosX - Constants::Hud::BorderBackground, LTPosY - Constants::Hud::BorderBackground,
				LTPosX + TSizeX + Constants::Hud::BorderBackground, LTPosY + 4 * TSizeY + Constants::Hud::BorderBackground));
	}

	// gameplay statistics
	{
		const unsigned int time = GameplayStats->GetPlayTime(actTime_);
		irr::core::stringw text = irr::core::stringw("Playtime: ") + GetTime(time, false);

		CurrentPlaytimeText->setText(text.c_str());
		CurrentDeathText->setText((irr::core::stringw("Deaths: ") + irr::core::stringw(GameplayStats->GetDeathCount())).c_str());

		VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 0),
			irr::core::recti(RBTPosX - Constants::Hud::BorderBackground, RBTPosY - Constants::Hud::BorderBackground,
				RBTPosX + TSizeX + Constants::Hud::BorderBackground, RBTPosY + 2 * TSizeY + Constants::Hud::BorderBackground));

		// update left bottom hud: long range scan
		NearestMonsterText->setText((irr::core::stringw("Nearest enemy: ")
			+ irr::core::stringw(static_cast<int>(10.0f * MonsterManager->GetNearestDistancePlayerShipToMonster()))
			+ irr::core::stringw(" m")).c_str());
		RemainingMonsterText->setText((irr::core::stringw("Remaining enemies: ") + irr::core::stringw(MonsterManager->GetNumberMonsters())).c_str());
		VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 0),
			irr::core::recti(LBTPosX - Constants::Hud::BorderBackground, LBTPosY - Constants::Hud::BorderBackground,
				LBTPosX + TSizeX + Constants::Hud::BorderBackground, LBTPosY + 3 * TSizeY + Constants::Hud::BorderBackground));
	}

	// cancel game dialog
	{
		SetCancelGameOverlay(EventReceiver->IsCancelGameOverlayActive());
		if (CancelGameText1)
			VideoDriver->draw2DRectangle(irr::video::SColor(128, 0, 0, 0),
				irr::core::recti(
					CTPosX - Constants::Hud::BorderBackground,
					CTPosY - Constants::Hud::BorderBackground,
					CTPosX + TSizeX + Constants::Hud::BorderBackground,
					CTPosY + 2 * TSizeY + Constants::Hud::BorderBackground));
	}

	// sets the 3D position and the monster, the player is aiming to
	Player->SetAimingPosition(aimingPos, AimedMonster);
}

void Nightork::CHud::SetCancelGameOverlay(bool value_)
{
	if (CancelGameText1)
	{
		if (!value_)
		{
			CancelGameText1->remove();
			CancelGameText1 = NULL;
			CancelGameText2->remove();
			CancelGameText2 = NULL;
		}
		return;
	}
	if (!value_)
	{
		return;
	}

	const irr::core::dimension2du screenSize = VideoDriver->getScreenSize();

	CancelGameText1 = GuiEnvironment->addStaticText(L"Cancel game?",irr::core::recti(
		CTPosX, CTPosY, CTPosX+TSizeX, CTPosY+TSizeY));
	CancelGameText1->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	CancelGameText1->setOverrideColor(irr::video::SColor(255,255,255,255));
	CancelGameText1->enableOverrideColor(true);

	CancelGameText2 = GuiEnvironment->addStaticText(L"(Y)es (N)o",irr::core::recti(
		CTPosX, CTPosY+TSizeY, CTPosX+TSizeX, CTPosY+2*TSizeY));
	CancelGameText2->setTextAlignment(irr::gui::EGUIA_CENTER,irr::gui::EGUIA_CENTER);
	CancelGameText2->setOverrideColor(irr::video::SColor(255,255,255,255));
	CancelGameText2->enableOverrideColor(true);
}

bool Nightork::CHud::IsAimedMonsterValid() const
{
	if (AimedMonster == NULL)
		return false;

	if (AimedMonster->IsDestroyed())
		return false;

	irr::core::vector3df playerDir = Player->GetAbsoluteShipPosition() - Player->GetCamera()->getAbsolutePosition();
	const float playerDirLengthSQ = playerDir.getLengthSQ();
	const irr::core::vector3df playerDirNormalized = playerDir.normalize();

	irr::core::vector3df monsterDir = AimedMonster->GetMonsterNode()->getAbsolutePosition() - Player->GetCamera()->getAbsolutePosition();
	const float monsterDirLengthSQ = monsterDir.getLengthSQ();
	const irr::core::vector3df monsterDirNormalized = monsterDir.normalize();

	const irr::core::vector3df cameraLookAt = (Player->GetCamera()->getTarget() - Player->GetCamera()->getAbsolutePosition()).normalize();

	if (monsterDirLengthSQ <= playerDirLengthSQ)
		return false;

	const float coneCos = std::cos(irr::core::degToRad(30.0f));

	const float dot = cameraLookAt.dotProduct(monsterDir);

	return dot >= coneCos;
}
