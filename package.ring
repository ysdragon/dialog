aPackageInfo = [
	:name = "Ring Dialog",
	:description = "Ring bindings for the osdialog library.",
	:folder = "dialog",
	:developer = "ysdragon",
	:email = "",
	:license = "MIT License",
	:version = "1.0.0",
	:ringversion = "1.23",
	:versions = 	[
		[
			:version = "1.0.0",
			:branch = "master"
		]
	],
	:libs = 	[
		[
			:name = "",
			:version = "",
			:providerusername = ""
		]
	],
	:files = 	[
		"lib.ring",
		"main.ring",
		"CMakeLists.txt",
		"LICENSE",
		"README.md",
		"src/dialog.rh",
		"src/ring_dialog.c",
		"src/utils/color.ring",
		"src/utils/install.ring",
		"src/utils/uninstall.ring",
		"examples/example1.ring"
	],
	:ringfolderfiles = 	[

	],
	:windowsfiles = 	[
		"src/patches/fix_msvc.patch",
		"lib/windows/i386/ring_dialog.dll",
		"lib/windows/amd64/ring_dialog.dll",
		"lib/windows/arm64/ring_dialog.dll"
	],
	:linuxfiles = 	[
		"lib/linux/amd64/libring_dialog.so",
		"lib/linux/arm64/libring_dialog.so"
	],
	:ubuntufiles = 	[

	],
	:fedorafiles = 	[

	],
	:freebsdfiles = 	[
		"lib/freebsd/amd64/libring_dialog.so",
		"lib/freebsd/arm64/libring_dialog.so"
	],
	:macosfiles = 	[
		"lib/macos/amd64/libring_dialog.dylib",
		"lib/macos/arm64/libring_dialog.dylib"
	],
	:windowsringfolderfiles = 	[

	],
	:linuxringfolderfiles = 	[

	],
	:ubunturingfolderfiles = 	[

	],
	:fedoraringfolderfiles = 	[

	],
	:freebsdringfolderfiles = 	[

	],
	:macosringfolderfiles = 	[

	],
	:run = "ring main.ring",
	:windowsrun = "",
	:linuxrun = "",
	:macosrun = "",
	:ubunturun = "",
	:fedorarun = "",
	:setup = "ring src/utils/install.ring",
	:windowssetup = "",
	:linuxsetup = "",
	:macossetup = "",
	:ubuntusetup = "",
	:fedorasetup = "",
	:remove = "ring src/utils/uninstall.ring",
	:windowsremove = "",
	:linuxremove = "",
	:macosremove = "",
	:ubunturemove = "",
	:fedoraremove = ""
]