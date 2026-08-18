MEMORY {
  flash(r)  : ORIGIN = 0x08000000, LENGTH = 2048k
  sram : ORIGIN = 0x20000000, LENGTH = 192k
}
_estack = 0x20030000;
SECTIONS {
  vectors  : { KEEP(*(.isr_vector)) }
  .text     : { *(.text*) }          
  .data : {
    _sdata = .;
    *(.data SORT(.data.*))
    _edata = .;
  } > sram AT > flash
  _sidata = LOADADDR(.data);

  .bss : {
    _sbss = .;
    *(.bss*)
    _ebss = .;
  }

  . = ALIGN(8);
  _end = .;
}