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
