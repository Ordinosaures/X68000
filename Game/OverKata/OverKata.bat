ECHO OFF
rem �J���p
cp -u data\map\course01.csv D:\���d���J����\OverKata\data\map\
cp -u OverKata.x D:\���d���J����\OverKata\
D:
cd D:\���d���J����\OverKata
rem JOYDRV3.X data\SAMPLE.CNF
PCM8A.X
MACSDRV.x
MOON.x -f10 -m512
ZMUSIC.X -M -N -P512 -W0 -Sdata\music\init.ZMD -Bdata\se\OverKata.ZPD
rem ZMSC.X -M -N  -P512 -W0 -B data\se\OverKata.ZPD
rem MPCM.X �L���ɂ���Ɠ���Đ����Ƀt���[�Y����
rem ZMSC3.X -J -F3 -N -S data\seFM\init_se_V3.ZMD -Z data\se\OverKata_V3.ZPD
rem MCDRV.X
rem scd OverKata.x
OverKata.x
rem MCDRV.X -R
rem ZMSC3.X -J -R
rem MPCM.X /r
rem ZMSC.X -R
ZMUSIC.X -R
MOON.x -r
MACSDRV.x -r
PCM8A.X -R
rem JOYDRV3.X -R
W:
