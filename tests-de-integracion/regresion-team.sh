#!/bin/sh

sleep_time=2

echo "  ** Test de TEAMS **  \n\n"
sh test-team-catedra-base.sh
sh test-team-catedra-espera-circular.sh
sh test-team-catedra-final.sh
sh test-team-catedra-final-rr.sh
sh test-team-catedra-final-sjf.sh