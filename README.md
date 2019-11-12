# Doxybook2

[![Build status](https://ci.appveyor.com/api/projects/status/uxhq9f3awnjsephc/branch/master?svg=true)](https://ci.appveyor.com/project/matusnovak/doxybook2/branch/master) [![Build Status](https://travis-ci.com/matusnovak/doxybook2.svg?branch=master)](https://travis-ci.com/matusnovak/doxybook2) [![CircleCI](https://circleci.com/gh/matusnovak/doxybook2/tree/master.svg?style=svg)](https://circleci.com/gh/matusnovak/doxybook2/tree/master)

Doxygen XML to Markdown (or JSON) converter. Generate beautiful C++ documentation by converting Doxygen XML output into markdown pages via [MkDocs](https://www.mkdocs.org/), [Hugo](https://gohugo.io/), [VuePress](https://vuepress.vuejs.org/), [GitBook](https://github.com/GitbookIO/gitbook), or your custom generator. Comes with optional templating mechanism and extensive configuration file.

![Screenshot - Hugo Learn](screenshots/screenshot_hugo_learn.jpg)

_[Screenshot taken from here](https://matusnovak.github.io/doxybook2/hugo-learn/classes/classengine_1_1audio_1_1audiobuffer/)_

Table of contents:

* [Description](#Description)
* [Examples](#Examples)
* [Install](#Install)
* [Install from source](#Install-from-source)
* [Usage](#Usage)
* [Config](#Config)
* [Templates](#Templates)

## Description

TODO

<details>
<summary>More screenshots</summary>
<br>

### [Hugo](https://gohugo.io/) with [Book](https://themes.gohugo.io/hugo-book/) theme ([Link](https://matusnovak.github.io/doxybook2/hugo-book/classes/classengine_1_1audio_1_1audiobuffer/))

![Screenshot - Hugo Book](screenshots/screenshot_hugo_book.jpg)

### [MkDocs](https://www.mkdocs.org/) with [ReadTheDocs](https://mkdocs.readthedocs.io/en/stable/) theme ([Link](https://matusnovak.github.io/doxybook2/mkdocs-readthedocs/Classes/classEngine_1_1Audio_1_1AudioBuffer/))

![Screenshot - MkDocs ReadTheDocs](screenshots/screenshot_mkdocs_readthedocs.jpg)

### [MkDocs](https://www.mkdocs.org/) with [Material](https://squidfunk.github.io/mkdocs-material/) theme ([Link](https://matusnovak.github.io/doxybook2/mkdocs-material/Classes/classEngine_1_1Audio_1_1AudioBuffer/))

![Screenshot - MkDocs Material](screenshots/screenshot_mkdocs_material.jpg)

### [MkDocs](https://www.mkdocs.org/) with [Bootstrap](http://mkdocs.github.io/mkdocs-bootstrap/) theme ([Link](https://matusnovak.github.io/doxybook2/mkdocs-bootstrap/Classes/classEngine_1_1Audio_1_1AudioBuffer/) )

![Screenshot - MkDocs Bootstrap](screenshots/screenshot_mkdocs_bootstrap.jpg)

### [VuePress](https://vuepress.vuejs.org/) with default theme ([Link](https://matusnovak.github.io/doxybook2/vuepress/Classes/classEngine_1_1Audio_1_1AudioBuffer.html) )

![Screenshot - VuePress](screenshots/screenshot_vuepress_default.jpg)

### [GitBook](https://github.com/GitbookIO/gitbook) with default theme ([Link](https://matusnovak.github.io/doxybook2/original/classEngine_1_1Audio_1_1AudioBuffer.html))

![Screenshot - GitBook](screenshots/screenshot_gitbook_default.jpg)
</details>

## Examples

| Demo | Generator | Theme | Config |
| ---- | --------- | ----- | ------ |
| [Link](https://matusnovak.github.io/doxybook2/hugo-learn/classes/classengine_1_1audio_1_1audiobuffer/) | [Hugo](https://gohugo.io/) | [Learn](https://themes.gohugo.io/hugo-theme-learn/) | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/hugo-learn/.doxybook) |
| [Link](https://matusnovak.github.io/doxybook2/hugo-book/classes/classengine_1_1audio_1_1audiobuffer/) | [Hugo](https://gohugo.io/) | [Book](https://themes.gohugo.io/hugo-book/) | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/hugo-book/.doxybook) |
| [Link](https://matusnovak.github.io/doxybook2/mkdocs-readthedocs/Classes/classEngine_1_1Audio_1_1AudioBuffer/) | [MkDocs](https://www.mkdocs.org/) | [ReadTheDocs](https://mkdocs.readthedocs.io/en/stable/) | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/mkdocs-readthedocs/.doxybook) |
| [Link](https://matusnovak.github.io/doxybook2/mkdocs-material/Classes/classEngine_1_1Audio_1_1AudioBuffer/) | [MkDocs](https://www.mkdocs.org/) | [Material](https://squidfunk.github.io/mkdocs-material/) | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/mkdocs-material/.doxybook) |
| [Link](https://matusnovak.github.io/doxybook2/mkdocs-bootstrap/Classes/classEngine_1_1Audio_1_1AudioBuffer/) | [MkDocs](https://www.mkdocs.org/) | [Bootstrap](http://mkdocs.github.io/mkdocs-bootstrap/) | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/mkdocs-bootstrap/.doxybook) |
| [Link](https://matusnovak.github.io/doxybook2/vuepress/Classes/classEngine_1_1Audio_1_1AudioBuffer.html) | [VuePress](https://vuepress.vuejs.org/) | Default | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/vuepress/.doxybook) | 
| [Link](https://matusnovak.github.io/doxybook2/gitbook/Classes/classEngine_1_1Audio_1_1AudioBuffer.html) | [GitBook](https://github.com/GitbookIO/gitbook) | Default | [.doxybook](https://github.com/matusnovak/doxybook2/tree/master/example/vuepress/.doxybook) |
| [Link](https://matusnovak.github.io/doxybook2/original/classEngine_1_1Audio_1_1AudioBuffer.html) | Original Doxygen | N/A | N/A |

## Install

TODO

## Install from source

You will need [CMake](https://cmake.org/) at least version 3.0 and a C++11 compiler. The recommended setup is to use GCC 8.1.0 either amd64 or arm64 (for Linux or Raspberry Pi), XCode xcode10.2 or newer (for Mac OSX), Visual Studio 2015 or newer either Win32 or Win64 (for Windows), or MinGW-w64 8.1.0 or newer either i686 or x86_64 (for Wndows).

```bash
# Download the project and all of the submodules
git clone https://github.com/matusnovak/doxybook2.git
cd doxybook2
git submodule update --init

# Configure the project
mkdir build
cd build
cmake -G "Unix Makefiles" \
    -DDOXYBOOK_TESTS=OFF \
    -DDOXYBOOK_STATIC_STDLIB=OFF \
    -DBUILD_TESTS=OFF \
    -DBUILD_TESTING=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    ..
# Build it
cmake --build .

# Done!
```

Use `-G "Visual Studio 15 2017"` when compiling for Windows/Visual Studio, or use `-G "MinGW Makefiles"` when compiling for Windows/MinGW-w64. You may need to use `cmake --build . --config MinSizeRel` when compiling with Visual Studio. The generated executable will be located in `build/src/DoxydownCli`.

## Usage

TODO

## Config

TODO

## Templates

TODO

## Contributing

Pull requests are welcome! If you have any suggestion, feel free to submit it to repository GitHub issues.

## Issues 

Feel free to submit any bugs or issues you find to repository GitHub issues.

## License

```
Copyright (c) 2019 Matus Novak email@matusnovak.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
