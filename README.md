# RingDialog

Simple Ring bindings for [osdialog](https://github.com/AndrewBelt/osdialog), a cross-platform library for native OS dialogs.

### Installation

There are two primary ways to install RingDialog: using the Ring Package Manager (RingPM) or by building from source.

#### 1. Using Ring Package Manager (Recommended)

RingPM is the official package manager for the Ring programming language. It simplifies the process of installing and managing libraries.

- **Install the library:**
  ```sh
  ringpm install dialog from ysdragon
  ```

This command will automatically download and install the correct version of the library for your system.

#### 2. Building from Source

If you prefer to build the library manually or want to contribute to its development, please see the [Development](#development) section for detailed instructions.

## Usage

First, load the library in your Ring script:

```ring
load "dialog.ring"
```

### Message Dialog

Display a simple message box.

```ring
dialog_message(DIALOG_INFO, DIALOG_OK, "Hello, World!")
```

**Function:** `dialog_message(level, buttons, message)`

-   `level`: The message level (e.g., `DIALOG_INFO`, `DIALOG_WARNING`, `DIALOG_ERROR`).
-   `buttons`: The button set (e.g., `DIALOG_OK`, `DIALOG_OK_CANCEL`, `DIALOG_YES_NO`).
-   `message`: The text to display in the dialog.

Returns `1` for OK/Yes, `0` for Cancel/No.

### Prompt Dialog

Ask the user for a text input.

```ring
text = dialog_prompt(DIALOG_INFO, "What is your name?", "Ring User")
see "User entered: " + text + nl
```

**Function:** `dialog_prompt(level, message, default_text)`

-   `level`: The message level.
-   `message`: The prompt message.
-   `default_text`: The initial text in the input field.

Returns the entered string, or an empty string if cancelled.

### File Dialog

Open a file-picker dialog.

```ring
// File open dialog
filters = "Source:c,h;Image:jpg,png,gif;Text:txt"
filepath = dialog_file(DIALOG_OPEN, ".", "", filters)
if len(filepath) > 0
    see "Selected file: " + filepath + nl
ok

// Directory open dialog
dirpath = dialog_file(DIALOG_OPEN_DIR, ".", "", "")
if len(dirpath) > 0
    see "Selected directory: " + dirpath + nl
ok
```

**Function:** `dialog_file(action, directory, filename, filters)`

-   `action`: The dialog action (`DIALOG_OPEN`, `DIALOG_OPEN_DIR`, `DIALOG_SAVE`).
-   `directory`: The initial directory.
-   `filename`: The default filename.
-   `filters`: A semicolon-separated list of file filters (e.g., `"Source:c,h;Image:jpg,png"`).

Returns the selected file path, or an empty string if cancelled.

### Color Picker

Open a color picker dialog.

```ring
color = [255, 0, 0, 255]  // [r, g, b, a]
opacity = 1

if dialog_color_picker(color, opacity)
    see "Selected color: " + color[1] + "," + color[2] + "," + color[3] + "," + color[4] + nl
else
    see "Color selection cancelled." + nl
ok
```

**Function:** `dialog_color_picker(color_list, opacity)`

-   `color_list`: A list of 4 numbers `[r, g, b, a]` representing the initial color. The list is updated with the selected color.
-   `opacity`: A number from 0-1 to enable/disable opacity support.

Returns `1` if a color is selected, `0` if cancelled.

### Constants

**Levels:**
- `DIALOG_INFO`
- `DIALOG_WARNING`
- `DIALOG_ERROR`

**Buttons:**
- `DIALOG_OK`
- `DIALOG_OK_CANCEL`
- `DIALOG_YES_NO`

**File Actions:**
- `DIALOG_OPEN`
- `DIALOG_OPEN_DIR`
- `DIALOG_SAVE`

## Development

If you want to contribute to the development of Ring Dialog or build it from source, follow these steps.

### Prerequisites

*   **CMake**: Version 3.16 or higher.
*   **C Compiler**: A C compiler compatible with your platform (e.g., GCC, Clang, MSVC).
*   **Ring**: You need to have the Ring language source code available on your machine.

### Build Steps

1.  **Clone the Repository**: Clone the Dialog repository to your local machine.

    ```bash
    git clone https://github.com/ysdragon/dialog.git --recursive
    ```
    > **NOTE**: Skip this step if you have already installed the library using RingPM.

2.  **Set the `RING` Environment Variable:** Before running CMake, you must set the `RING` environment variable to point to the root directory of the Ring language source code.
    - Windows
      - Command Prompt
          ```cmd
          set RING=X:\path\to\ring
          ```
      - PowerShell
          ```powershell
          $env:RING = "X:\path\to\ring"
          ```

    - Unix
      ```bash
      export RING=/path/to/ring
      ```

3.  **Configure with CMake**: Create a `build` directory and run CMake from within it.
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

4.  **Build the Project**: Compile the source code using the build toolchain configured by CMake (e.g., Make, Ninja).
    ```bash
    cmake --build .
    ```
The compiled library will be placed in the `lib/<os>/<arch>` directory.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.