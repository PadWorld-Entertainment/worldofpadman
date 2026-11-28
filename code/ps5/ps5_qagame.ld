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
