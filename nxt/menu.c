#define STR_ARRAY_SIZE 6

void writeRC(void);

string options[STR_ARRAY_SIZE];
int size = 6;
word FILE_SIZE = 255;

task main() {
	options[0] = "TNK_EXP";
	options[1] = "TNK_LIN";
	options[2] = "TNK_LOW";
	options[3] = "ARC_EXP";
	options[4] = "ARC_LIN";
	options[5] = "ARC_LOW";

	writeRC();
}

int menu(void) {
	int index = 0;
	bool selected = false;

	while(!selected) {
		wait1Msec(25);
		eraseDisplay();
		nxtDisplayCenteredTextLine(4, "%s", options[index]);
		nxtDisplayCenteredTextLine(6, "%d", index);
		switch(nNxtButtonPressed) {
			case kRightButton: /* right */
				index++;
				break;
			case kLeftButton: /* left */
				index--;
				break;
			case kEnterButton: /* enter */
				selected = true;
				break;
			default:
		}
		index = index < 0 ? size - 1 : index;
		index = index > size - 1 ? 0 : index;

		while(nNxtButtonPressed != -1) {
			/* do nothing */
		}
	}

	return index;
}

void writeRC(void) {
	TFileHandle file;
	TFileIOResult result;

	string selection = "";

	selection = options[menu()];

	Delete("teleoprc.txt", result);
	OpenWrite(file, result, "teleoprc.txt", FILE_SIZE);
	WriteText(file, result, selection);
	Close(file, result);

	return;
}
