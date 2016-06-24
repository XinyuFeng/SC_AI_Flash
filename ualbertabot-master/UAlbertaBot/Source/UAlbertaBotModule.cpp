/* 
 +----------------------------------------------------------------------+
 | UAlbertaBot                                                          |
 +----------------------------------------------------------------------+
 | University of Alberta - AIIDE StarCraft Competition                  |
 +----------------------------------------------------------------------+
 |                                                                      |
 +----------------------------------------------------------------------+
 | Author: David Churchill <dave.churchill@gmail.com>                   |
 +----------------------------------------------------------------------+
*/

#include "Common.h"
#include "UAlbertaBotModule.h"
#include "JSONTools.h"
#include "ParseUtils.h"
#include "UnitUtil.h"

using namespace UAlbertaBot;
using namespace BWAPI;

// This gets called when the bot starts!

void cheatFlag(bool flag){
	if (flag){
		Broodwar->sendText("%s", "show me the money");
		Broodwar->sendText("%s", "black sheep wall");
		Broodwar->sendText("%s", "operation cwal");
	}
}


void UAlbertaBotModule::onStart()
{
    // Initialize SparCraft, the combat simulation package

	cheatFlag(false);

    SparCraft::init();

    // Initialize BOSS, the Build Order Search System
    BOSS::init();
	

    // Parse the bot's configuration file if it has one, change this file path to where your config file is
    // Any relative path name will be relative to Starcraft installation folder
    ParseUtils::ParseConfigFile(Config::ConfigFile::ConfigFileLocation);
	//ParseUtils::ParseConfigFile("C:/Users/oGhost/Desktop/usc/csci524/starcraft/bwapi-data/AI/UAlbertaBot_Config.txt");


    // Set our BWAPI options here    
	BWAPI::Broodwar->setLocalSpeed(Config::BWAPIOptions::SetLocalSpeed);
	BWAPI::Broodwar->setFrameSkip(Config::BWAPIOptions::SetFrameSkip);
    
    if (Config::BWAPIOptions::EnableCompleteMapInformation)
    {
        BWAPI::Broodwar->enableFlag(BWAPI::Flag::CompleteMapInformation);
    }

    if (Config::BWAPIOptions::EnableUserInput)
    {
        BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
    }

    if (Config::BotInfo::PrintInfoOnStart)
    {
        BWAPI::Broodwar->printf("Hello! I am %s, written by %s", Config::BotInfo::BotName.c_str(), Config::BotInfo::Authors.c_str());
    }

    // Call BWTA to read and analyze the current map
    if (Config::Modules::UsingGameCommander)
	{
		BWTA::readMap();
		BWTA::analyze();

		RegionManager::calcDistance();
        
		if (Config::Modules::UsingStrategyIO)
        {
            StrategyManager::Instance().readResults();
            StrategyManager::Instance().setLearnedStrategy();
        }
	}
}

void UAlbertaBotModule::onEnd(bool isWinner) 
{
	if (Config::Modules::UsingGameCommander)
	{
		StrategyManager::Instance().onEnd(isWinner);
	}	
}

void UAlbertaBotModule::onFrame()
{
	if (Config::Modules::UsingGameCommander) 
	{ 
		_gameCommander.update();
		RegionManager::onDrawRegroupPosition();
	}

    if (Config::Modules::UsingAutoObserver)
    {
        _autoObserver.onFrame();
    }
	//for test

	AutoObserver ob;

}

void UAlbertaBotModule::onUnitDestroy(BWAPI::Unit unit)
{
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitDestroy(unit); }
}

void UAlbertaBotModule::onUnitMorph(BWAPI::Unit unit)
{
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitMorph(unit); }
}

void UAlbertaBotModule::onSendText(std::string text) 
{ 
	ParseUtils::ParseTextCommand(text);
}

void UAlbertaBotModule::onUnitCreate(BWAPI::Unit unit)
{ 
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitCreate(unit); }
}

void UAlbertaBotModule::onUnitComplete(BWAPI::Unit unit)
{
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitComplete(unit); }
}

void UAlbertaBotModule::onUnitShow(BWAPI::Unit unit)
{ 
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitShow(unit); }
}

void UAlbertaBotModule::onUnitHide(BWAPI::Unit unit)
{ 
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitHide(unit); }
}

void UAlbertaBotModule::onUnitRenegade(BWAPI::Unit unit)
{ 
	if (Config::Modules::UsingGameCommander) { _gameCommander.onUnitRenegade(unit); }
}