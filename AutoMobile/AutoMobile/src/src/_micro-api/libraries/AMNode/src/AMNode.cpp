// 
// 
// 

#include "AMNode.h"
#include "AMComponent.h"


AMNode::AMNode() {
	red.init(4);
	green.init(5);

	red.light();
	
	addOption((AMMenuOption*) new Buzz());
	addOption((AMMenuOption*) new PlaySong());

	display(SYSTEM, CONSOLE);

	red.dark();
	green.light();
}

void AMNode::loop() {
	green.blink();

	handle("CONSOLE@NODE", Serial);

	delay(75);
}



System::System() : AMComponent("SYSTEM@NODE", "NodeMCU System") {

}



Console::Console() : AMComponent("CONSOLE@NODE", "NodeMCU Serial Console") {
	Serial.begin(SERIAL_BAUD);
}

void Console::print(String text) {
	Serial.print(text);
}

void Console::println() {
	Serial.println();
}



Buzz::Buzz() : AMMenuOption("Buzzer") { }

void Buzz::trigger(String source, String data) {

}



PlaySong::PlaySong() : AMMenuOption("Play Song") { }

void PlaySong::trigger(String source, String data) {

}
