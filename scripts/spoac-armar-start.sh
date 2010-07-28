#!/bin/bash

echo "Starting SPOAC services:"

if [ -d $HOME/spoac/spoac-install ]; then
     spoac_dir=$HOME/spoac
     spoac_install_dir=$HOME/spoac/spoac-install
else
     spoac_dir=$MCAPROJECTHOME/spoac
     spoac_install_dir=$MCAPROJECTHOME/spoac-install
fi
spoac_log_dir="$spoac_install_dir/var/log/spoac"
spoac_var_dir="$spoac_install_dir/var/lib/spoac"

if [ ! -d $spoac_log_dir ]; then
     mkdir -p $spoac_log_dir
fi

if [ ! -d "$spoac_var_dir/stormdb" ]; then
     mkdir -p $spoac_var_dir/stormdb
fi

cd $spoac_var_dir
source $spoac_install_dir/share/spoac/scripts/spoac-armar-setenv.sh
echo "    ICE ..."
$spoac_install_dir/share/spoac/scripts/ice-start.sh $spoac_install_dir/share/spoac/ > $spoac_log_dir/ice-start.log 2>&1 &

echo "    LTM ..."
$spoac_install_dir/bin/ltmserver > $spoac_log_dir/ltmserver.log 2>&1 &

pks_dir=$HOME/pem
echo "    PKS Plan Execution Monitor ..."
#cd $pks_dir
#echo "m" | ./build/bin/planexecute -b -c domains/config.pem > $spoac_log_dir/log/pem.log 2>&1 &
echo "        Failure: Interactive startup required"

echo "Done."


