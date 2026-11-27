/* ld -r script for the linked-in qagame (Makefile.client GAME_NATIVE_OBJ).
   Brackets its writable data so Sys_LoadGameDll can reset it on every reload. */
SECTIONS
{
  .data : {
    __qagame_data_start = .;
    *(.data .data.* .sdata .sdata.*)
    __qagame_data_end = .;
  }
  .bss : {
    __qagame_bss_start = .;
    *(.bss .bss.* .sbss .sbss.* COMMON)
    __qagame_bss_end = .;
  }
}
