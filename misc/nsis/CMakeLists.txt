configure_file(wop-full-installer.nsi.in wop-full-installer.nsi @ONLY)

find_program(NSIS_MAKE
    NAMES
        makensis
    PATHS
        C:/NSIS/Bin
        "$ENV{PROGRAMFILES}/NSIS"
)

add_custom_target(nsis
    COMMAND ${NSIS_MAKE} wop-full-installer.nsi
    DEPENDS pk3 wop-full-installer.nsi
    COMMENT "Generate nsis installer"
)
