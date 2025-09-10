# shiftr C changes
- refactored to match bronto_c final iteration on 8/31/25

## sept. 10 2025
- dev buttons circuit finalized, brought button state into lib/state
    - check [buttons.md](z_docs/buttons.md)
- lib/fx created to handle mode switching by state (fx/switcher.c)
    - bit_chaser and byte_chaser functions originally from shift.c were moved to
    fx/chaser.c


