# A.K.E.L.A.

<p>
 <span align="left">
  <a href="https://travis-ci.org/algernon/Akela"><img align="left" src="https://travis-ci.org/algernon/Akela.svg?branch=master"></a>
 </span>
 <p align="right">
  <strong>A</strong>nimated <strong>K</strong>eyboardio <strong>E</strong>xtension
  <strong>L</strong>ibrary for <strong>A</strong>nything
 </p>
</p>

[Akela][akela] is a collection of small libraries to extend
the [Keyboardio Model 01 firmware][kbdiofw]. A heavy work in progress, with a
lot of moving or untested parts. While it has been tried on real hardware, a lot
of things are either buggy, or do not work at all yet. But work is under way to
bring everything into a usable state.

 [kbdiofw]: https://github.com/Keyboardio/KeyboardioFirmware
 [akela]: https://github.com/algernon/Akela

Status
======

Individual libraries may be of different quality or maturity. The list below is
an up-to-date report on the current state of each library:

| Library                | Maturity                   |
| ---------------------- | -------------------------- |
| Akela-Colormap         | ![status][st:stable]       |
| Akela-Cycle            | ![status][st:stable]       |
| Akela-DualUse          | ![status][st:stable]       |
| Akela-Heatmap          | ![status][st:broken]       |
| Akela-HostOS           | ![status][st:stable]       |
| Akela-IgnoranceIsBliss | ![status][st:experimental] |
| Akela-KeyLogger        | ![status][st:stable]       |
| Akela-LEDEffects       | ![status][st:stable]       |
| Akela-Leader           | ![status][st:stable]       |
| Akela-MagicCombo       | ![status][st:stable]       |
| Akela-OneShot          | ![status][st:experimental] |
| Akela-ShapeShifter     | ![status][st:stable]       |
| Akela-SpaceCadet       | ![status][st:stable]       |
| Akela-TapDance         | ![status][st:experimental] |
| Akela-TopsyTurvy       | ![status][st:experimental] |
| Akela-Unicode          | ![status][st:stable]       |

 [st:stable]: https://img.shields.io/badge/stable-✔-black.png?style=flat&colorA=44cc11&colorB=494e52
 [st:broken]: https://img.shields.io/badge/broken-X-black.png?style=flat&colorA=e05d44&colorB=494e52
 [st:experimental]: https://img.shields.io/badge/experimental----black.png?style=flat&colorA=dfb317&colorB=494e52

Documentation
=============

Documentation can be found in the `README.md` files in the root of each plugin's
repository.

License
=======

The code is released under the terms of the GNU GPL, version 3 or later. See the
COPYING file for details.
