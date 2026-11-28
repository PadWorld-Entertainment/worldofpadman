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
