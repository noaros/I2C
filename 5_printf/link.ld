MEMORY {
  flash(r)  : ORIGIN = 0x08000000, LENGTH = 2048k
  sram : ORIGIN = 0x20000000, LENGTH = 192k
}

SECTIONS {
  .vectors : {
		LONG(0x20030000)
		LONG(mystart | 1)
		. = . + 420;
  }
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