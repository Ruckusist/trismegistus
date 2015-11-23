// 
// 
// 

#include "AMMenu.h"
#include "AMComponent.h"


AMMenu::AMMenu() {

	// Add the default option
	addOption((AMMenuOption*) new ShowMenu(this));

}

void AMMenu::display(AMComponent& src, AMComponent& dest) {
	for (int i = 0; i <= optionIdx; i++) {
		src.tag(dest);
//		dest.print(String("/") + source + "/");
		dest.print(String(i));
		dest.print(".   ");
		options[i]->display(serial);
		dest.println();
	}
}

void AMMenu::handle(String source, HardwareSerial& serial) {
	if (serial.available() > 0) {
		String line = serial.readStringUntil('\n');

		if (line != NULL && line.length() > 0) {
			if (line.charAt(0) == '/') {
				int slash = line.indexOf('/', 1);

				if (slash > 1) {
					source = line.substring(1, slash);
					line = line.substring(slash + 1);
				}
			}

			// Sometimes we get carriage-returns, grr... ditch them
			if (line.charAt(line.length() - 1) == '\r') {
				line = line.substring(0, line.length() - 1);
			}

			int comma = line.indexOf(',');
			String cmdStr = (comma > 0) ? line.substring(0, comma) : line;
			int cmd = cmdStr.toInt();
			String data = (comma > 0) ? line.substring(comma + 1) : "";

			if (cmd >= 0 && cmd <= optionIdx) {
				options[cmd]->trigger(source, data);

			}
			else {
				serial.println(String("UNKNOWN OPTION: ") + cmd + " (send 0 for menu)");
			}
		}
		else {
			display(source, serial);
		}
	}
}

void AMMenu::addOption(AMMenuOption* option) {
	options[++optionIdx] = option;
}





AMMenuOption::AMMenuOption(String text) {
	this->text = text;
}

void AMMenuOption::display(HardwareSerial& serial) {
	serial.print(text);
}




ShowMenu::ShowMenu(AMMenu* menu) : AMMenuOption("Show Menu") {
	this->menu = menu;
}

void ShowMenu::trigger(String source, String data) {
//	menu->display(source)
}
