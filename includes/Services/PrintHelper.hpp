#pragma once

#include "DataLayer/PlayerHand.h"
#include "BlackjackLogic/PointsTools.hpp"
#include <string>

namespace Services
{
	class PrintHelper
	{
	public:
		PrintHelper(
			const BlackjackLogic::PointsTools* pointsTools,
			size_t moneyPrecision = 2,
			const std::string& moneySign = "$");

		std::string formatMoney(const double& money) const;
		std::string formatHand(const DataLayer::Hand& hand) const;
		std::string formatPlayerHand(const DataLayer::PlayerHand& hand) const;
		std::string formatHands(
			const std::vector<DataLayer::PlayerHandPtr>&) const;
	
	private:
		const BlackjackLogic::PointsTools* m_pointsTools;
		const size_t m_moneyPrecision;
		const std::string m_moneySign;
	};
}
