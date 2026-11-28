/* ld -r script for the linked-in cgame (Makefile.client CGAME_NATIVE_OBJ).
   Brackets its writable data so Sys_LoadGameDll can reset it on every reload. */
SECTIONS
{
  .data : {
    __cgame_data_start = .;
    *(.data .data.* .sdata .sdata.*)
    __cgame_data_end = .;
  }
  .bss : {
    __cgame_bss_start = .;
    *(.bss .bss.* .sbss .sbss.* COMMON)
    __cgame_bss_end = .;
  }
}
