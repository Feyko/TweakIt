# Changelog

## 0.6.0
Changes may be missed because of heavy refactoring after a long time away from the codebase. Future changelogs will be 100% correct

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
  - Delegate (WIP)
    - You can bind Lua code to be executed when the delegate gets called!!
    - You can also wait for the event to be called
  - Interface
    - Behaves as an object property, does not restrain what you can call like a real interface
  - FieldPath
    - Basically useless!
  - Function (WIP)
    - Call game functions from Lua
    - Change a function's code to call your Lua function
    - Hook functions and run your Lua code before or after
    - The possibilities are endless!!!