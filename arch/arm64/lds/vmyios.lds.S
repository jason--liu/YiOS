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

    . = ALIGN(4096);
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
         init_stack = .;
         KEEP(*(data.init_task))
        . = ALIGN(4096);
        idmap_pg_dir = .;
        . += 4096;
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
