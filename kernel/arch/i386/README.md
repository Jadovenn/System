
## i386 Loader

The i386 loader is multiboot compliant. It first load the kernel in higher half virtual memory space.
Then it create the physical memory map and initialize its virtual memory management unit before
giving control to the kernel.

### Current available x86 feature:
 - Protected mode setup
 - Monitor driver initialized
 - Memory management unit configured
 - apic initialized
 - timer initialized