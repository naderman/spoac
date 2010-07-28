message( STATUS "== SCRIPTS ==" )

SET(spoac_path_scripts
    spoac-armar-start.sh
    spoac-armar-stop.sh
)

SET(spoac_other_scripts
    ice-start.sh
    spoac-armar-setenv.sh
)

GBX_ADD_SHARED_PROGRAMS(scripts ${spoac_other_scripts})

GBX_ADD_BIN_SCRIPTS(${spoac_path_scripts})

