#include "KeyboardCmdManager.hpp"
#include <iostream>

using namespace KeyboardInputTools;

std::string KeyboardCmdManager::readKeyboardInput()
{
	std::string input;
	
	std::getline(std::cin, input);
	return input;
}

void KeyboardCmdManager::subscribeParser(const CmdParser newParser)
{
	m_cmdParser.push_back(newParser);
}

bool KeyboardCmdManager::parseCmd(std::string input) const
{
	for (const auto& parser : m_cmdParser)
	{
		if (parser(input))
			return true;
	}
	
	return false;
}
