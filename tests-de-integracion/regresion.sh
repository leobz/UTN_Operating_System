echo Regresion de Todos los Tests

echo
echo Compilo Utils y Gameboy:
byexample -l shell compilar-base.md

echo
echo Compilo Team:
byexample -l shell compilo-team.md

echo
echo Compilo Broker:
byexample -l shell compilo-broker.md

echo
echo Corro Tests Integración Team-Broker:
byexample -l shell team-broker.md

echo
echo Corro Tests de Broker:
byexample -l shell broker.md

echo
echo Corro Tests de Team:
byexample -l shell team.md