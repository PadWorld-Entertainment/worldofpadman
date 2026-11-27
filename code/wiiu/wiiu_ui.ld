/* ld -r script for the linked-in ui.
   Brackets its writable data so Sys_LoadGameDll can reset it on every reload. */
SECTIONS
{
  .data : {
    __ui_data_start = .;
    *(.data .data.* .sdata .sdata.*)
    __ui_data_end = .;
  }
  .bss : {
    __ui_bss_start = .;
    *(.bss .bss.* .sbss .sbss.* COMMON)
    __ui_bss_end = .;
  }
}
