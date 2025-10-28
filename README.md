# Godot Embed External Editor

A GDExtension which allows embedding an external code editor such as Visual Studio Code directly into your Godot editor!

**[Quick Start Guide](#quick-start)**

![Preview Image](docs/preview.png)

# Features

- Automatically attach and detach external code editors into Godot editor.
  - A toggle button in the scene tab bar to manually attach and detach the external code editor.

# Quick Start

- Ensure that
  - you are using Godot 4.1 or later.
  - you are using Windows.
  - in Godot's editor settings, "Single-window mode" is **disabled** (otherwise dialogs will not appear in front of the code editor).
  - in Godot's editor settings, external code editor is enabled and [correctly set up](https://docs.godotengine.org/en/stable/tutorials/editor/external_editor.html) (double-clicking a script file in Godot's file explorer should open your editor of choice).
  - you follow [the setup instructions](#per-editor-setup) for the editor of your choice.
- [Download and extract the latest build artifact](https://nightly.link/RedMser/godot-embed-external-editor/workflows/build/master/godot-embed-external-editor_master.zip) or compile from source.
- Copy the `addons` folder into your game project folder.
- Enable the plug-in in the project settings.
- Open a script file by double-clicking it in Godot's file browser.
- Switch to the Script tab and enjoy!

## Per-Editor Setup

Currently, following external editors are supported:

- [Visual Studio Code](#visual-studio-code)

### Visual Studio Code

Following settings changes can either be done in the user settings (Ctrl+Comma), or for each workspace folder:

- Set `window.titleBarStyle` to `native`. Otherwise the titlebar can not be hidden and the editor window will be resizable independently from its container.
- Ensure that the settings `window.title` and `window.titleSeparator` use their default values. Follow these instructions if you have modified them:
  - The `window.title` setting must contain both the string `Visual Studio Code` and the name of the currently open folder `${rootName}`. This is needed in order for the addon to detect whether the instance has the project opened.
  - Similarly, the `window.titleSeparator` needs leading and trailing whitespace to work correctly.

If you prefer using Visual Studio Code as a plain text editor without its IDE features, consider using Zen Mode:

- Set `zenMode.centerLayout` to `false` and `zenMode.fullScreen` to `false`.
- Enable Zen Mode by running the command `View: Toggle Zen Mode`.

# Limitations

This addon is very experimental and hacky. I'm trying to improve upon it, but there are some things that can not be fixed as easily:

## General

- Can not view documentation in editor without detaching editor.
  - **Workaround:** You can use VSCode's "List native classes" option as an alternative.
- There has been occasional freezes in the past which I've tried to fix. If they still persist, please [open an issue](https://github.com/redmser/godot-embed-external-editor/issues/new).
  - **Workaround:** If you get a freeze, it seems like Alt+Tab fixes it most of the time.
- Editor's titlebar does not show after undocked.
  - **Workaround:** Minimize and restore the window.
- The embedded window may get native window borders and be resizable at times (e.g. when the debugger breaks).
  - **Workaround:** Undock and dock the window again.
- Keyboard input will only be accepted by whichever window is in focus (e.g. F5 to run the game).
- Godot does not automatically switch to the script tab.
- Hover thumbnails for scene tabs do not show in front of the embedded code editor.

## Visual Studio Code

- Rarely, a black strip appears at the top of VSCode which offsets all input events
  - **Workaround:** Restart VSCode to fix this (sadly not even the "Reload Window" command solves this).

# Contributing

I don't plan on working on this project for more than my personal use-cases, so I would greatly appreciate help from others to improve upon this extension:

- Support other OSes like Mac or Linux.
- Support more editors, including documentation on how to set them up.
- Fix usability issues and bugs.

Create issues for ideas, feedback or bugs. Open pull requests if you implemented something. All help is appreciated! :)

If you want to show your appreciation in another way, consider [donating on GitHub sponsors](https://github.com/RedMser/godot-embed-external-editor).
