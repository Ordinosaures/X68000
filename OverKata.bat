ECHO OFF
rem �J���p
cp -u OverKata.x D:\���d���J����\OverKata\
D:
cd D:\���d���J����\OverKata
JOYDRV3.X data\SAMPLE.CNF
MACSDRV.x
MOON.x -f10 -m300
PCM8A.X
ZMUSIC.X -M -N -P512 -W0 -Sdata\music\init.ZMD -Bdata\se\OverKata.ZPD
rem ZMSC.X -M -N  -P128 -W0 -B data\se\OverKata.ZPD
rem MPCM.X �L���ɂ���Ɠ���Đ����Ƀt���[�Y����
rem ZMSC3.X -J -F3 -N -S data\seFM\init_se_V3.ZMD -Z data\se\OverKata_V3.ZPD
rem MCDRV.X
rem scd OverKata.x
OverKata.x
rem MCDRV.X -R
rem ZMSC3.X -J -R
rem MPCM.X /r
ZMUSIC.X -R
PCM8A.X -R
MOON.x -r
MACSDRV.x -r
JOYDRV3.X -R
L:
