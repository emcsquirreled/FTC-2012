/* Holds the maximum number of menu options */
#define STR_ARRAY_SIZE 6
/* Holds the different menu options */
string options[STR_ARRAY_SIZE];
/* Holds the number of menu options */
int size = 6;
/* Holds the maximum file size */
word FILE_SIZE = 255;

/* Displays a menu allowing the user to select between the
   options in options[], and returns the index of the
   selected item */
int menu(void) {
	/* Initialize variables */
	int index = 0;
	bool selected = false;

	/* Run until and entry is selected */
	while(!selected) {
		/* Pause, to remove LCD flickering */
		wait1Msec(25);

		/* Clear the display */
		eraseDisplay();

		/* Display the current option and index */
		nxtDisplayCenteredTextLine(4, "%s", options[index]);
		nxtDisplayCenteredTextLine(6, "%d", index);

		/* Check for a button press */
		switch(nNxtButtonPressed) {
			/* Right arrow pressed */
			case kRightButton:
				index++;
				break;

			/* Left arrow pressed */
			case kLeftButton: /* left */
				index--;
				break;

			/* Enter button pressed */
			case kEnterButton: /* enter */
				selected = true;
				break;

			/* Anything else */
			default:
				/* Do nothing */
		}

		/* Keep the index inbounds */
		index = index < 0 ? size - 1 : index;
		index = index > size - 1 ? 0 : index;

		/* Wait for a depressed button to be released */
		while(nNxtButtonPressed != -1) {
			/* do nothing */
		}
	}

	return index;
}

/* Entry point for the code */
task main() {
	/* File IO variables */
	TFileHandle file;
	TFileIOResult result;

	/* Setup teleop options */
	string selection = "";
	options[0] = "TNK_EXP";
	options[1] = "TNK_LIN";
	options[2] = "TNK_LOW";
	options[3] = "ARC_EXP";
	options[4] = "ARC_LIN";
	options[5] = "ARC_LOW";
	size = 6;

	/* Get the selection */
	selection = menu();

	/* Write the option to teleoprc.txt, overwriting it if necessary */
	Delete("teleoprc.txt", result);
	OpenWrite(file, result, "teleoprc.txt", FILE_SIZE);
	WriteText(file, result, selection);
	Close(file, result);
}
