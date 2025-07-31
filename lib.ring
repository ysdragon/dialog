# This file is part of the Ring Dialog library.

# Load the Ring Dialog library based on the operating system.
if isWindows()
	loadlib("ring_dialog.dll")
but isLinux() or isFreeBSD()
	loadlib("libring_dialog.so")
but isMacOSX()
	loadlib("libring_dialog.dylib")
else
	raise("Unsupported OS! You need to build the library for your OS.")
ok

# Load the Ring Dialog Constants.
load "src/dialog.rh"