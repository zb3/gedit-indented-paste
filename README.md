# gedit-indented-paste

This plugin allows you to paste text with the indent at cursor position. It trims common indent occuring in each non-blank line of the pasted text and prepends the indent at cursor position to each pasted line (except the first one).

To paste like that, press <kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>V</kbd>. For pasting `PRIMARY` clipboard contents, use <kbd>Shift</kbd> + <kbd>Middle click</kbd> (which uses the current cursor).

## What this plugin doesn't do

This plugin doesn't detect and convert the indentation style of the pasted text to match the indentation style of the current file. That's mainly because it's not universally possible to detect that correctly just by analysing the text, since there are many corner cases (like docblock, indented arguments, "half indents" etc) that would make the conversion behave inconsistently. You'd probably want a language-dependent code formatter anyway.

## Installation
1. Make sure your gedit version is up to date.
2. Copy project folder to `~/.local/share/gedit/plugins`
3. Enable this plugin.

