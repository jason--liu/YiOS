ENTRY(_start)
SECTIONS
{
	. = 0x80000,

	_text_boot = .;
	.text.boot :
	{
		*(.text.boot)
	}
	_etext_boot = .;

	_text = .;
	.text :
	{
		*(.text)
	}
	_etext = .;

	_rodata = .;
	.rodata :
	{
		*(.rodata)
	}
	_erodata = .;

	. = ALIGN(0x8);
	_data = .;
	.data : { *(.data) }
	_edata = .;
	. = ALIGN(0x8);

	_bss = .;
	.bss :
	{
		*(COMMON)
		*(.bss*)
	}
	_ebss = .;
}
