#!/bin/sh
sleep_time=2


echo "\n\n\n  **  Test Team Catedra Base  **\n\n\n"


echo "Ejecutando 'sh test-team-catedra-base.sh'"

rm *.log
cp team.config team.config.auxiliar; rm team.config; cp team.config.catedra.base team.config
1>/dev/null 2>/dev/null     ../team/Debug/team &

echo "\nEjecutar 'sh test-team-catedra-base-2.sh'?"
read "rta"
sh test-team-catedra-base-2.sh
sleep $sleep_time

echo "Comprobacion: TID:1 paso a Unblocked:\n"
cat team.log | grep "Exec->Unblocked"

echo "\n\nEjecutar 'sh test-team-catedra-base-3.sh'?"
read "rta"
sh test-team-catedra-base-3.sh
sleep $(expr $sleep_time \* 2)

echo "Comprobacion: Cumplio objetivo Global:\n"
tail team.log

echo "\n\nEjecutar test?\n"
read "rta"
byexample -l shell team-catedra-base.md


1>/dev/null 2>/dev/nul         sh mataProcesos.sh