# This file is part of the Ring Dialog library.
# It provides functionality to remove the library files and clean up the environment.

load "stdlibcore.ring"

cPathSep = "/"

if isWindows()
	cPathSep = "\\"
ok

# Remove the dialog.ring file from the load directory
remove(exefolder() + "load" + cPathSep + "dialog.ring")

# Remove the dialog.ring file from the Ring2EXE libs directory
remove(exefolder() + ".." + cPathSep + "tools" + cPathSep + "ring2exe" + cPathSep + "libs" + cPathSep + "dialog.ring")

# Change current directory to the samples directory
chdir(exefolder() + ".." + cPathSep + "samples")

# Remove the UsingDialog directory if it exists
if direxists("UsingDialog")
	OSDeleteFolder("UsingDialog")
ok