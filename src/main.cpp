#include <juce_core/juce_core.h>

#include <iostream>

int main()
{
	const juce::String pluginName{ "TapeLab" };

	std::cout << pluginName.toStdString()
		<< " - JUCE integration successful!\n";

	return 0;
}