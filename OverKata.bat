ECHO OFF
rem �J���p
cp -u OverKata.x D:\���d���J����\OverKata\
D:
cd D:\���d���J����\OverKata
MACSDRV.x
MOON.x -f10 -m300
rem PCM8A.X
rem ZMUSIC.X -M -N -P128 -W0 -B data\se\OverKata.ZPD
rem ZMSC.X -M -N  -P128 -W0 -B data\se\OverKata.ZPD
rem MPCM.X �L���ɂ���Ɠ���Đ����Ƀt���[�Y����
ZMSC3.X -J -F3 -N -S data\seFM\init_se_V3.ZMD -Z data\se\OverKata_V3.ZPD
rem scd OverKata.x
OverKata.x
ZMSC3.X -J -R
rem MPCM.X /r
rem ZMSC.X -R
rem ZMUSIC.X -R
rem PCM8A.X -R
MOON.x -r
MACSDRV.x -r
L:
