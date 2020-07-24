echo Restaurando makefiles desde backups

echo "   Restaurando team.."
mkdir team/Debug 2>/dev/null
cp -r deploy/team/* team/Debug/

echo "   Restaurando gamecard.."
mkdir gamecard/Debug 2>/dev/null
cp -r deploy/gamecard/* gamecard/Debug/

echo "   Restaurando broker.."
mkdir broker/Debug 2>/dev/null
cp -r deploy/broker/* broker/Debug/

echo "   Restaurando gameboy.."
mkdir gameboy/Debug 2>/dev/null
cp -r deploy/gameboy/* gameboy/Debug/

echo "   Restaurando utils.."
mkdir utils/Debug 2>/dev/null
cp -r deploy/utils/* utils/Debug/
