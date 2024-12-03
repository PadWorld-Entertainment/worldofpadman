# World of PADMAN Readme

[![GitHub tag](https://img.shields.io/github/v/tag/padworld-entertainment/worldofpadman?logo=github)](https://github.com/PadWorld-Entertainment/worldofpadman/tags)
[![GitHub release](https://img.shields.io/github/v/release/padworld-entertainment/worldofpadman?logo=github)](https://github.com/PadWorld-Entertainment/worldofpadman/releases/latest)
[![GitHub workflow](https://github.com/padworld-entertainment/worldofpadman/actions/workflows/build.yml/badge.svg)](https://github.com/PadWorld-Entertainment/worldofpadman/actions)
[![GitHub milestone](https://img.shields.io/github/milestones/progress-percent/padworld-entertainment/worldofpadman/5?logo=github)](https://github.com/PadWorld-Entertainment/worldofpadman/milestone/5)

[![Discord online](https://img.shields.io/discord/590829990514262038?label=join&logo=discord&style=social)](http://discord.worldofpadman.net)
[![Twitter follower](https://img.shields.io/twitter/follow/worldofpadman?label=follow&style=social)](https://twitter.com/world_of_padman)
[![Subreddit subscriber](https://img.shields.io/reddit/subreddit-subscribers/worldofpadmanreloaded?label=subscribe&style=social)](https://www.reddit.com/r/worldofpadmanreloaded/)
[![YouTube subscriber](https://img.shields.io/youtube/channel/subscribers/UC5lcgR3xEVPwC6E1sKk2rBQ?label=subscribe&style=social)](https://www.youtube.com/channel/UC5lcgR3xEVPwC6E1sKk2rBQ)

[![image](https://worldofpadman.net/wp-content/uploads/wop_header01.png)](https://worldofpadman.net)


World of PADMAN is an incredibly carefully designed and colorful freeware fun shooter for young and young-at-heart people, created by cartoonist and illustrator ENTE and the WoP team, aka. [PadWorld Entertainment](https://worldofpadman.net/about/team/). For more information about the game please visit the official [World of PADMAN website](https://worldofpadman.net).

## License

The WoP Team (PadWorld Entertainment) agrees that any commercial use of World of PADMAN and its individual works of art created by PadWorld Entertainment is prohibited without express consent and permission in written form. Further information and regulation is provided in [COPYING.md](COPYING.md).

The source code of World of PADMAN is published under the [GNU General Public License (Version 2, June 1991)](GPL-2.0.md) and the corresponding rules apply. This does not apply to World of PADMAN's individual works of art such as maps, textures, models, sounds, etc. These assets are the property of their respective authors and are protected by copyright.

Some parts of World of PADMAN were created with contents acquired by PadWorld Entertainment from various sources, with licenses that allow its use and modification. For more information about third-party content used in World of PADMAN and related licenses, please refer to [LICENSES.md](LICENSES.md).

## MapPack

[MAPPACK.md](MAPPACK.md) lists all the maps provided with the Community Map Pack and the associated licenses or terms of use chosen by the original author, which allows PadWorld Entertainment to bundle and release these maps with the World of PADMAN game and to make changes if necessary. If you would like to include your map, please contact us. A license that allows us to make necessary adjustments to the current WoP development status, such as [CC-BY-NC](https://creativecommons.org/licenses/by-nc/4.0/), is a basic requirement.

## Security

**Please do not report security vulnerabilities through public GitHub issues.** If you believe you have found a security vulnerability in any of our repositories please follow instructions provided in [SECURITY.md](SECURITY.md).

## Contributing

If you want to contribute to the further development of World of PADMAN by creating issues or pull requests, please see the information in [CONTRIBUTING.md](CONTRIBUTING.md). PadWorld Entertainment follows and applies the Code of Conduct as described in [Contributor Covenant,
version 2.1](https://www.contributor-covenant.org/version/2/1/code_of_conduct).

## Credits

World of PADMAN is powered by a modified id Tech 3 graphics engine based on [ioquake3](https://ioquake3.org/) and has all the associated features. We would like to thank everyone who contributed to the [ioq3 project](https://github.com/ioquake/ioq3). Since 2001, various programmers have contributed to the source code of World of PADMAN.

* Stefan Langer
* [Uwe Koch](https://github.com/seldomU)
* [Martin Gerhardy](https://github.com/mgerhardy)
* [Thilo Schulz](https://github.com/thiloschulz)
* Herby
* [Niklas Link](https://github.com/robo9k)
* Paul Rabe
* [Kai Bergmann](https://github.com/kai-li-wop)

## Building from source

Make sure that you've cloned the repository with [git-lfs](https://git-lfs.com/) installed.

The dependencies for World of PADMAN are bundled in this repository. Usually you don't have to install any other dependency - just the compiler and [cmake](https://cmake.org/).

If you are using an IDE with direct cmake support (e.g. [vscode](https://code.visualstudio.com/)), you don't have to run cmake from the command line. For everyone else:

```sh
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install
cmake --build .
cmake --install .
```

There is a more detailed guide available at [worldofpadman.net](https://worldofpadman.net/en/tutorials/compiling-wop-with-cmake-in-vs-code/).
