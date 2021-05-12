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
	.data :
    {    *(.data)
         . = ALIGN(4096);
         init_thread_union = .;
         KEEP(*(data.init_task))
    }
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
