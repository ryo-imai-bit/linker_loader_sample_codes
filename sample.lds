SECTIONS
{
 VALUEABLE = .;
 .text : { *(.text) }
 .rodata : { *(.rodata) }
 .data : { *(.data) }
 .bss : { *(.bss) }
}
