#!/bin/sh

echo "Stopping SPOAC services:"
echo "    PKS Plan Execution Monitor ..."
killall -q planexecute
echo "    LTM ..."
killall -q ltmserver
echo "    ICE ..."
killall -q icebox

echo "Done."
