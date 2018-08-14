#pragma once

#include "BlackjackServer/Models/GameSessionModels.h"
#include "Services/PrintHelper.hpp"
#include "DataLayer/Bet.h"
#include "DataLayer/CashResult.h"
#include <string>

namespace BlackjackServer { namespace Views
{
	class GameSessionViews
	{
	public:
		GameSessionViews(const Services::PrintHelper* printHelper);
	
		std::string startGame_View(
			const Models::GmSessionModels::StartGameModel&) const;
		std::string startBettingPhase_View() const;
		
		std::string startHitStandPhase_View() const;
		std::string initialHandsSetup_View(
			const Models::GmSessionModels::InitialHandsSetupModel&) const;
		
		std::string startCashing_View() const;
		std::string dealersFinalCards_View(
			const DataLayer::Hand& dealersHand,
			int nbOfCardsDealt) const;
		std::string handCashResult_View(
			const DataLayer::PlayerHand&,
			const DataLayer::CashResult&) const;
		std::string handCashResult_ServerView(
			const DataLayer::PlayerModel&,
			const DataLayer::PlayerHand&,
			const DataLayer::CashResult&) const;
		
		std::string resetingTheGame_View() const;
		
		std::string noGames_View() const;
		std::string notBettingPhase_View() const;
		std::string notAPlayer_View() const;
		std::string noSuchHand_View(int nbOfHands) const;
		
		std::string handNotSitting_View(int handIndex) const;
		std::string invalidBet_View(const DataLayer::Bet& bet) const;
		std::string notEnoughMoney_View(double total, double required) const;
		std::string alreadyPlacedABet_View(const DataLayer::Bet& bet) const;
		
		std::string notHitStandPhase() const;
		std::string successfullHit_View(
			const Models::GmSessionModels::SuccessfulHitModel&) const;
		std::string successfullStand(
			const Models::GmSessionModels::SuccessfulStandModel&) const;
		
		std::string successfulBet_View(
			const Models::GmSessionModels::SuccessfulBetModel&) const;
		std::string successfulBet_ServerView(
			const Models::GmSessionModels::SuccessfulBetModel&) const;
		
	private:
		const Services::PrintHelper& m_printHelper;
	};
}}
