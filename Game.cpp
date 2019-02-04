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
	string name = screen.getName();
	net.setName(name.c_str(), name.size());
	int wildcard;
	bool isMyTurn;
	if (!net.amIServer()) {
		isMyTurn = net.initGame(&(callbackClient),0,0,NULL, &screen);
	}
	else {
		string mapName = screen.chooseMap();
		isMyTurn = net.initGame(NULL, mapName.size(), screen.getChecksum(), mapName.c_str());
	}
	player.setPlayer(screen.getMap().getTeam(), screen.getMap());
	screen.setEnemyName(net.getOpponentName());
	bool end = false;
	bool change = true;
	player.getMap()->removeFogStart();
	if (isMyTurn) {
		player.startTurn();
	}
	while (!end) {
		if (change) {
			screen.updateGraphics(*player.getMap());
			change = false;
		}
		if (isMyTurn) {
			if (player.finish()) {
				end = true;
				if (player.iWin()) {
					screen.drawSpecificMessage("You won the game, congratulations!", 2000);
				}
				else {
					screen.drawSpecificMessage("You lost the game :(", 2000);
				}
			}
			Unit * unit;
			action_s action = screen.getUserAction(player.getMoney());
			if (action.act == A_ATTACK && player.getMap()->getUnitPtr(action.positionFrom) != nullptr &&!player.getMap()->IsValidAttack(player.getMap()->getUnitPtr(action.positionFrom),action.positionTo)) {
				action.act = A_NO_ACTION;
			}
			else if (action.act == A_MOVE && player.getMap()->getUnitPtr(action.positionFrom) != nullptr && !player.getMap()->IsValidMove(player.getMap()->getUnitPtr(action.positionFrom), action.positionTo)) {
				action.act = A_NO_ACTION;
			}
			string code;
			switch (action.act) {
			case A_ATTACK:
				if (player.getStatus() != PURCHASING) {
					wildcard = rand() % 6 + 1;
					myDice = wildcard;
					player.getMap()->attack(player.getMap()->getUnitPtr(action.positionFrom), action.positionTo, myDice);
					myDice = wildcard;
					net.sendMessage(this, ATTACK, action.positionFrom.row, action.positionFrom.column,
						action.positionTo.row, action.positionTo.column, myDice, &callback);
					player.updateInventory();
				}
				else
					screen.drawSpecificMessage("You can't attack after you buy!", 1000);
				change = true;
				break;
			case A_PURCHASE:
				unit = player.buyUnit(screen.chooseUnitToBuy(player.getUnitsAvailableToBuy()), action.positionFrom, player.getMap()->getTeam());
				if (unit != nullptr) {
					code = getUnitCode(unit->getUnitClass());
					net.sendMessage(this, PURCHASE, code.c_str()[0], code.c_str()[1], unit->getPosition().row, unit->getPosition().column);
					player.updateInventory();
					player.setStatus(PURCHASING);
				}
				change = true;
				break;
			case A_MOVE:
				if (player.getStatus() != PURCHASING) {
					player.getMap()->move(action.positionTo, player.getMap()->getUnitPtr(action.positionFrom));
					net.sendMessage(this, MOVE, action.positionFrom.row, action.positionFrom.column, action.positionTo.row, action.positionTo.column);
				}
				else
					screen.drawSpecificMessage("You can't move after you buy!", 1000);
				change = true;

				break;
			case A_PASS:
				isMyTurn = !isMyTurn;
				player.endTurn();
				net.sendMessage(this, PASS);
				player.endTurn();
				screen.showTransition();
				change = true;
				break;
			case A_UNLOAD:
				player.getMap()->unloadAPC(action.positionFrom, action.positionTo);
				net.sendMessage(this, MOVE, action.positionFrom.row, action.positionFrom.column, action.positionFrom.row, action.positionFrom.column);
				net.sendMessage(this, MOVE, action.positionFrom.row, action.positionFrom.column, action.positionTo.row, action.positionTo.column);
				break;
			case A_NO_ACTION:
				break;
			case A_CLOSE_GAME:
				end = true;
				screen.drawSpecificMessage("See you back soon!", 2000);
				net.sendMessage(this, QUIT);
				break;
			default:
				break;
			}
		}
		else {
			directives_s temp = D_NOTHING;
			do {
				temp = net.waitForMyTurn(&callback, &callbackResponseAttack, this);
				int quit = screen.checkIfUserClose();
				if (quit) {
					net.sendMessage(this, QUIT);
					end = true;
				}
				if (temp == D_PASS) {
					isMyTurn = true;
					player.startTurn();
					change = true;
					screen.showTransition();
				}
				else if (temp == D_QUIT) {
					screen.drawSpecificMessage("The Enemy has finished the game!", 500);
					end = true;
				}
			} while (temp != D_NOTHING);
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