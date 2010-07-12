#
# When you add a new .ice file, insert its name into this list. This is
# the only file which has to be updated.
#

#
# Since CMake doesn't know how to automatically track dependencies for .ice files,
# these have to be entered manually in proper order: such that the depended-on
# files are listed first.
#
set(SPOAC_SLICE_SOURCE_FILES
    # definitions
    # interfaces
    spoac/SymbolicExecution.ice
    spoac/Planning.ice
    spoac/LTM.ice
)
