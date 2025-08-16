[Discord]: https://discord.gg/DmxAne5hcA
[Atlas Linux]: https://github.com/AtlasLinux
[LICENSE]: https://github.com/AtlasLinux/pnd-language/blob/main/LICENSE
[Issues]: https://github.com/AtlasLinux/pnd-language/issues

# Pandora Language Contribution Guide
Hello! Thank you for your interest in this project! <br/>
This is contribution guide for **Pandora Language** - the package description file for [Atlas Linux] package manager. <br/>
If you wanna take a part in this project, please follow provided instructions.

> [!TIP]
> - Atlas Linux Project: [Atlas Linux]
> - Discord Server: [Discord]
> - License: [LICENSE]

## Github Repository Issues
Found a bug? Want to suggest an improvement? Want to give us some tips? <br/>
You can always open new issue in this repository with [Issues] tab!

> [!NOTE]
> **Issues requirements:**
> 1. Your problem wasn't reported earlier (use search for check).
> 2. Clear and understandable description of problem.
> 3. Provided examples or any other extra information.

## Github Pull Requests
> [!NOTE]
> ### Ensure that
> - your code matches the project's code style
> - your code doesn't break program's work
> - you've written readable and understandable code

> [!NOTE]
> ### Please do...
> - Verify your code with project requirements
> - Provide necessary examples or any other extra information of changes
> - Provide detailed description of your work in pull request
> - Format your code with command: `make format`

## Project Code Style
1. **Namings** must be clear and understandable. <br/>
_Variables/Functions/Structs/Enums_ are using `snake_case` (for types `_t` at the end: `token_t`). <br/>
_Define Constants_ (`#define`) must have `SCREAMING_SNAKE_CASE`, <br/>
_Basic Constants_ must have `camelCase` with `k` prefix (example: `kInputString`)
2. **Includes** are located at the start, _standard library headers_ must be separated from _user header files_.
3. **Repeating** code suggested to be in separated function.
4. **No Magic Numbers**. If you need to use that number, declare a constant with it.
5. **Comments** must contain information that can help others understand your code. <br/>
Always leave necessary documentation for provided API and changes.\
6. **The code blocks** should not be "glued together", they should be separated according to their logical meaning.
