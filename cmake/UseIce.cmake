#
# Links to libIce only.
# see also: UseIceBasics.cmake
#

include( ${SPOAC_CMAKE_DIR}/UseZeroC.cmake )

if( NOT SPOAC_OS_WIN )

    link_libraries( Ice IceUtil )

else( NOT SPOAC_OS_WIN )

    # windows... have to link to different libs depending on build type
    link_libraries( optimized Ice       debug Iced
                    optimized IceUtil   debug IceUtild )

endif( NOT SPOAC_OS_WIN )