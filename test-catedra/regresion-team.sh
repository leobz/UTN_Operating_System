#!/bin/sh

sleep_time=2

echo "  ** Test de TEAMS **  \n\n"
sh team-base.sh
sh team-espera-circular.sh
sh team-final-fifo.sh
sh team-final-rr.sh
sh team-final-sjf.sh
sh team-final-sjf-cd.sh
