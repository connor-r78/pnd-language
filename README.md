[LICENSE]: https://github.com/AtlasLinux/pnd-language/blob/main/LICENSE
[CONTRIBUTING.md]: https://github.com/AtlasLinux/pnd-language/blob/main/CONTRIBUTING.md

# 🧩 Pandora Language
**Pandora Language** is a declarative interpreter langauge with simple syntax. <br/>
It is used to describe packages for [Atlas Linux Package Manager](https://github.com/AtlasLinux/pandora).

Project provides fast analyzer and executor for packages installation instructions.<br/>
The main idea is provide user-friendly package manager and package language in [Atlas Linux System](https://github.com/AtlasLinux/)

**⚠️ The project is currently under active development and may be unstable.**

## ⚙️ Technical Details
- **Main Language:** `C`
- **Build System:** `Make`
- **C Compiler:** `gcc` (you can use any supported for yourself)

## 🛠️ Building Guide
1. Install C compiler (Makefile uses [gcc](https://gcc.gnu.org/))
2. Clone this repository:
```command
git clone https://github.com/AtlasLinux/pnd-language
```
4. Build from Makefile:
```command
make
```
5. Compiled binary will be at `build/pnd`

## 👮 Repository
Project is licensed under the MIT License. <br/>
See more in [LICENSE] file. <br/>
You can check contribution guide in [CONTRIBUTING.md]
