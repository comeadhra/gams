#!/bin/bash
TIME=3600
MADARA_DEBUG=0
PERIOD=0.5
NUM=10
SIM=shield
AREA=small

# launch drone controllers
for (( i=0; i<$NUM; i++ ))
do
  xterm -hold -e "$GAMS_ROOT/gams_controller -i $i -n $NUM -p vrep -lt $TIME -pd $PERIOD -mf $GAMS_ROOT/scripts/simulation/$SIM/madara_init_${i}.mf $GAMS_ROOT/scripts/simulation/areas/${AREA}.mf $GAMS_ROOT/scripts/simulation/madara_init_common.mf -l $MADARA_DEBUG" &
done

# launch simulation controller
$GAMS_ROOT/dynamic_simulation -n $NUM -mf $GAMS_ROOT/scripts/simulation/areas/${AREA}.mf