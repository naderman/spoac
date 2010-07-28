message( STATUS "== SCRIPTS ==" )

SET(spoac_path_scripts
    spoac-armar-start.sh
    spoac-armar-stop.sh
)

SET(spoac_other_scripts
    ice-start.sh
    spoac-armar-setenv.sh
)

SET(spoac_ice_config
    ice-config/icebox_icestorm.cfg
)

GBX_ADD_SHARED_PROGRAMS(scripts ${spoac_other_scripts})

GBX_ADD_BIN_SCRIPTS(${spoac_path_scripts})

GBX_ADD_SHARED_FILES(scripts/ice-config ${spoac_ice_config})

