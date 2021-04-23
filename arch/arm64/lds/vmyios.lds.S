ENTRY(_start)
SECTIONS
{
	. = 0x80000,

	.text.boot :
	{
		*(.text.boot)
	}

	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}

	. = ALIGN(0x8);
	.data : { *(.data) }
	. = ALIGN(0x8);

	bss_begin = .;
	.bss :
	{
		*(COMMON)
		*(.bss*)
	}
	bss_end = .;
}
