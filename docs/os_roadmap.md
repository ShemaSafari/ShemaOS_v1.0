# OS roadmap

This document shows the path i took, my step-by-step plan in building my OS

---

## Phase 1: Boot process

✓ Bootlaoder (jumps to loader)

✓ Loader (set up stack and segmentation)

✓ Enable A20 gate

✓ Load the kernel into memory

✓ Set up GDT and build it

✓ Switch to protected mode

☐ jump to `"kernel_entry.asm"`


## Phase 2: Basic kernel
