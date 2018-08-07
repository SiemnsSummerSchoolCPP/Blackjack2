#pragma once

#include <string>
#include <vector>
#include <functional>

namespace KeyboardInputTools
{
	class KeyboardCmdManager
	{
	public:
		typedef std::function<bool (std::string)> CmdParser;
		
		static std::string readKeyboardInput();
		
		void subscribeParser(CmdParser newParser);
		bool parseCmd(std::string input) const;
		
	private:
		std::vector<CmdParser> m_cmdParser;
	};
}
