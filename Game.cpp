#include "./Game.h"
#include "Callbacks.h"

void Game::initGame() {
	//al_init();
	//Graphics screen;
	//Player player;
	srand(time(NULL));
	net.establishConnection();
	srand(time(NULL));
}

void Game::run() {
	//CARGAR NOMBRE POR GRAPHIC
	//Graphics screen;
	//string name = screen.getName();
	string name = "martina";
	net.setName(name.c_str(), name.size());
	bool isMyTurn;
	if (!net.amIServer()) {
		isMyTurn = net.initGame(&(callbackClient),0,0,NULL);
	}
	else {
		string mapName = screen.chooseMap();
		isMyTurn = net.initGame(NULL, mapName.size(), 0, mapName.c_str());
	}
	player.setPlayer(screen.getMap().getTeam(), screen.getMap());
	bool end = false;
	bool change = true;
	while (!end) {
		if (change) {
			screen.updateGraphics(*player.getMap());
			change = false;
		}
		if (isMyTurn) {
			if (player.loser()) {
				end = true;
			}
			Unit * unit;
			player.collectIncome();
			action_s action = screen.getUserAction();
			string code;
			switch (action.act) {
			case A_ATTACK:
				myDice = rand() % 6 + 1;
				player.getMap()->attack(player.getMap()->getUnit(action.positionFrom), action.positionTo, myDice);
				net.sendMessage(ATTACK, action.positionFrom.row, action.positionFrom.column,
					action.positionTo.row, action.positionTo.column, myDice, &callback);
				player.updateInventory();
				change = true;
				//screen.updateGraphics(*player.getMap());
				break;
			case A_PURCHASE:
				unit = player.buyUnit(screen.chooseUnitToBuy(), action.positionTo);
				code = getUnitCode(unit->getUnitClass());
				net.sendMessage(PURCHASE, code.c_str()[0], code.c_str()[1], action.positionFrom.row, action.positionFrom.column);
				player.updateInventory();
				change = true;
				//screen.updateGraphics(*player.getMap());
				break;
			case A_MOVE:
				player.getMap()->move(action.positionTo, player.getMap()->getUnit(action.positionFrom));
				net.sendMessage(MOVE, action.positionFrom.row, action.positionFrom.column, action.positionTo.row, action.positionTo.column);
				change = true;
				//screen.updateGraphics(*player.getMap());
				break;
			case A_PASS:
				isMyTurn = !isMyTurn;
				net.sendMessage(PASS);
				change = true;
				//screen.updateGraphics(*player.getMap());
				break;
			case A_NO_ACTION:
				break;
			case A_CLOSE_GAME:
				end = true;
				break;
			default:
				//end = true;
				break;
			}
		}
		else {
			int temp = net.waitForMyTurn(&callback, &callbackResponseAttack);
			if (temp != -1) {
				change = true;
			}
			/*screen.updateGraphics(*player.getMap());*/
		}
	}

}

string Game::getUnitCode(units_d unitClass)
{
	string ans;
	switch (unitClass)
	{
	case INFANTRY:
	{
		ans = "in";
	}break;
	case MECH:
	{
		ans = "me";
	}break;
	case ROCKET:
	{
		ans = "ro";
	}break;
	case RECON:
	{
		ans = "re";
	}break;
	case APC:
	{
		ans = "ap";
	}break;
	case ANTIAIR:
	{
		ans = "aa";
	}break;
	case ARTILLERY:
	{
		ans = "ar";
	}break;
	case TANK:
	{
		ans = "ta";
	}break;
	case MEDTANK:
	{
		ans = "mt";
	}break;
	default:
	{
		ans = "";
	}break;
	}
	return ans;
}

units_d Game::getUnitFromCode(string code)
{
	units_d ans;

	if (code == "in")
		ans = INFANTRY;
	else if (code == "me")
		ans = MECH;
	else if (code == "ro")
		ans = ROCKET;
	else if (code == "re")
		ans = RECON;
	else if (code == "ap")
		ans = APC;
	else if (code == "aa")
		ans = ANTIAIR;
	else if (code == "ta")
		ans = TANK;
	else if (code == "mt")
		ans = MEDTANK;
	else
		ans = ERRORCLASS;

	return ans;
}