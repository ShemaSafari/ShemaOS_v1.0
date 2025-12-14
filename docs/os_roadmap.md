# OS roadmap

This document shows the path i took, my step-by-step plan in building my OS

---

## Phase 1: Boot process

✓ -- Bootlaoder (jumps to loader) --

✓ -- Loader (set up stack and segmentation to known values) --

✓ -- Enable A20 gate --

✓ -- Load the kernel into memory --

✓ -- Set up GDT and build it --

✓ -- Switch to protected mode --

✓ -- jump to `"kernel_entry.asm"` --
> *"spent a week losing my mind debugging and trying to figure out what stopped me from jumping to the kernel, but now we are finally designing and building the kernel."*


## Phase 2: Basic kernel

✓ -- Starting in `'kernel_entry.asm'`, cleared the screen now its all black --

✓ -- Made the VGA driver (I can now print text on screen) --

✓ -- Set up the IDT, ISR, IRQ now my OS can handle exceptions and not freak out or crash on incoming interrupts --

✓ -- Impliment the Keyboard driver (my OS can now take in keyboard inputs)--

✓ -- Impliment a basic Shell (with commands like clear, about, reboot, help and echo)--

☐

☐

☐