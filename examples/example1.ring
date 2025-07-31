load "dialog.ring"

# Message Dialog
see "Testing Message Dialog..." + nl
nResult = dialog_message(DIALOG_INFO, DIALOG_OK_CANCEL, "This is an info message.")
if nResult = 1
	see "User clicked OK!" + nl
else
	see "User clicked Cancel!" + nl
ok

# Prompt Dialog
see "Testing Prompt Dialog..." + nl
cName = dialog_prompt(DIALOG_INFO, "What is your name?", "Ring")
if cName != ""
	see "Hello, " + cName + "!" + nl
else
	see "User cancelled the prompt." + nl
ok

# File Open Dialog
see "Testing File Open Dialog..." + nl
cFilters = "Source:c,cpp,ring;Images:png,jpg,jpeg;All Files:*"
cPath = dialog_file(DIALOG_OPEN, "", "", cFilters)
if cPath != ""
	see "Selected file: " + cPath + nl
else
	see "User cancelled file selection." + nl
ok

# Directory Open Dialog
see "Testing Directory Open Dialog..." + nl
cPath = dialog_file(DIALOG_OPEN_DIR, "", "", "")
if cPath != ""
	see "Selected directory: " + cPath + nl
else
	see "User cancelled directory selection." + nl
ok

# File Save Dialog
see "Testing File Save Dialog..." + nl
cPath = dialog_file(DIALOG_SAVE, "", "my_new_file.txt", cFilters)
if cPath != ""
	see "File to save: " + cPath + nl
else
	see "User cancelled file save." + nl
ok

# Color Picker
see "Testing Color Picker..." + nl
aColor = [255, 0, 0, 255]  # Initial color: Red
nResult = dialog_color_picker(aColor, 1) # 1 = enable opacity
if nResult = 1
	see "Color selected: "
	see "R: " + aColor[1] + ", "
	see "G: " + aColor[2] + ", "
	see "B: " + aColor[3] + ", "
	see "A: " + aColor[4] + nl
else
	see "User cancelled color selection." + nl
ok