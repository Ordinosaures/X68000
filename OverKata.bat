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
rem MPCM.X
ZMSC3.X -J -F4 -N
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
