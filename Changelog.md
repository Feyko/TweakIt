# Changelog

## 0.6.0
Changes may be missed because of heavy refactoring after a long time away from the codebase. Future changelogs will be correct

- Improved stability!
  - No (less) memory leaks
  - Checked functions to fail early
  - (More) Correct Garbage Collection
- New command to run all scripts: /runallscripts or /ras
- Struct copying
- Array assignment & length operator
- WaitForEvent and WaitForMod functions to wait for specific events or another mod to load. TweakIt also loads way earlier now, and runs on game launch
- Independent log files for every script & a main TweakIt log file, located next to the scripts
- Newly supported property types:
  - Object
    - Useful for asset manipulation!
  - Delegate
    - You can bind Lua code to be executed when the delegate gets called!!
    - You can also wait for the event to be called