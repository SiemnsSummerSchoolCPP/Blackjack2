#include "Views/GameSessionViews.hpp"
#include "Models/GameSessionModels.h"
#include <sstream>
#include <iomanip>

using namespace BlackjackServer::Views;

GameSessionViews::GameSessionViews(
	const Services::PrintHelper* const printHelper) :
	m_printHelper(printHelper)
{
}

std::string GameSessionViews::startGame_View(
	const Models::GmSessionModels::StartGameModel& model) const
{
	auto ss = std::stringstream();
	
	ss	<< "A game of " << model.joinedUsers->size()
		<< " player(s) will now start." << std::endl;
	
	ss << "Player(s) in the game:" << std::endl;
	for (const auto& user : *model.joinedUsers)
	{
		ss	<< "- " << user->name
			<< ": " << m_printHelper->formatMoney(user->money)
			<< std::endl;
	}
	return ss.str();
}

std::string GameSessionViews::startHitStandPhase_View() const
{
	return "All players have placed their bets. Starting the hit stand phase.";
}

std::string GameSessionViews::initialHandsSetup_View(
	const Models::GmSessionModels::InitialHandsSetupModel& model) const
{
	std::stringstream ss;
	
	ss << "Dealer's first card: " << *model.dealersCard << std::endl;
	for (const auto& player : *model.players)
	{
		ss	<< player->userModel->name << ": "
			<< m_printHelper->formatHands(player->hands)
			<< std::endl;
	}
	return ss.str();
}

std::string GameSessionViews::startBettingPhase_View() const
{
	return
		"Starting the betting phase. Please place your bets.\n"
		"- Ex: bet 42.5\n";
}

/*
** Leave game action.
*/

std::string GameSessionViews::resetingTheGame_View() const
{
	return "The game will now end.";
}

/*
** General player actions views.
*/

std::string GameSessionViews::noGames_View() const
{
	return
		"There are no games being played at the moment. Type 'ready' and "
		"wait for others.";
}

std::string GameSessionViews::notBettingPhase_View() const
{
	return "Can't bet. It's not the betting phase";
}

std::string GameSessionViews::notAPlayer_View() const
{
	return
		"You're not a player of this game. "
		"You need to wait until it ends. Then you can type 'ready' and play.";
}

std::string GameSessionViews::noSuchHand_View(const int nbOfHands) const
{
	return
		"You don't own such a hand. You have only " + std::to_string(nbOfHands)
		+ " hands.";
}

std::string GameSessionViews::handNotSitting_View(const int handIndex) const
{
	return
		"Hand " +
		(handIndex == 0 ? "" : std::to_string(handIndex) + " ") +
		"not sitting.";
}

/*
** Bet views.
*/

std::string GameSessionViews::invalidBet_View(const DataLayer::Bet& bet) const
{
	std::stringstream ss;
	
	ss << m_printHelper->formatMoney(bet.amount) << ": Invalid bet.";
	return ss.str();
}

std::string GameSessionViews::notEnoughMoney_View(
	const double total,
	const double required) const
{
	std::stringstream ss;
	
	ss	<< "Not enough money. Total: " << m_printHelper->formatMoney(total)
		<< ". Required: " << m_printHelper->formatMoney(required) << ". "
		<< "You need " << m_printHelper->formatMoney(required - total)
		<< " more.";
	return ss.str();
}

std::string GameSessionViews::alreadyPlacedABet_View(
	const DataLayer::Bet& bet) const
{
	std::stringstream ss;
	
	ss	<< "Already placed a bet: " << m_printHelper->formatMoney(bet.amount);
	return ss.str();
}

std::string GameSessionViews::successfulBet_View(
	const Models::GmSessionModels::SuccessfulBetModel& model) const
{
	std::stringstream ss;
	
	ss	<< model.player->userModel->name << " placed a bet: "
		<< m_printHelper->formatMoney(model.bet->amount) << ". "
		<< "Money left: "
		<< m_printHelper->formatMoney(model.player->userModel->money);
	return ss.str();
}

std::string GameSessionViews::successfulBet_ServerView(
	const Models::GmSessionModels::SuccessfulBetModel& model) const
{
	std::stringstream ss;
	
	ss	<< model.player->userModel->name << " placed a bet: "
		<< m_printHelper->formatMoney(model.bet->amount) << " on hand "
		<< model.bet->handIndex << ".";
	return ss.str();
}

/*
** Hit stand.
*/

std::string GameSessionViews::notHitStandPhase() const
{
	return "It's not the Hit-Stand phase.";
}

std::string GameSessionViews::successfullHit_View(
	const Models::GmSessionModels::SuccessfulHitModel& model) const
{
	std::stringstream ss;
	
	ss	<< model.player->userModel->name
		<< " has hit another card: " << *model.cardDealt
		<< std::endl
		<< "- His card: " << m_printHelper->formatHands(model.player->hands)
		<< std::endl;
	return ss.str();
}

std::string GameSessionViews::successfullStand(
	const Models::GmSessionModels::SuccessfulStandModel& model) const
{
	std::stringstream ss;
	
	ss	<< model.player->userModel->name << " stands."
		<< std::endl
		<< "- His card: " << m_printHelper->formatHands(model.player->hands)
		<< std::endl;
	return ss.str();
}

/*
** Cashing
*/

std::string GameSessionViews::startCashing_View() const
{
	return "All hands are standing. Now it's the cashing phase.";
}

std::string GameSessionViews::dealersFinalCards_View(
	const DataLayer::Hand& dealersHand,
	const int nbOfCardsDealt) const
{
	std::stringstream ss;
	
	if (nbOfCardsDealt != 0)
	{
		ss	<< "The dealer gets " << nbOfCardsDealt << " more card(s). ";
	}
	
	ss << "Dealer's hand: [ " << m_printHelper->formatHand(dealersHand) << " ]";
	return ss.str();
}

std::string GameSessionViews::handCashResult_ServerView(
	const DataLayer::PlayerModel& player,
	const DataLayer::PlayerHand& hand,
	const DataLayer::CashResult& cashResult) const
{
	std::stringstream ss;
	
	ss	<< player.userModel->name << ": "
		<< "{" << m_printHelper->formatHand(hand) << "}"
		<< ": [" << cashResult.state << "]: "
		<< "Received: " << m_printHelper->formatMoney(cashResult.receivedMoney)
		<< " Income: " << m_printHelper->formatMoney(cashResult.income);
	return ss.str();
}

std::string GameSessionViews::handCashResult_View(
	const DataLayer::PlayerHand& hand,
	const DataLayer::CashResult& cashResult) const
{
	std::stringstream ss;
	
	ss	<< "{" << m_printHelper->formatHand(hand) << "}" << ": "
		<< "[" << cashResult.state << "] ";

	ss	<< "(" << m_printHelper->formatMoney(cashResult.bet->amount) << ") x "
		<< std::fixed << std::setprecision(1) << cashResult.winMultipiler;
	
	ss << " Income: " << m_printHelper->formatMoney(cashResult.income);

	return ss.str();
}
