HU        !�  V�  b  r2  �  �  X  �   V                    screen 1,3 APICG           YGMC /T4 /Z /B  txs -p                          0_MakeTxTp.bat r �t�@�C�������ɑ��݂��܂��B(%s)
 __Clean.bat �����s���Ă�������
 w %s

 %s CG%05d.bmp
 %s
 %s Tx%05d Tp%05d

 %s ���쐬���܂����B
 ---------------------------
 1_MakePCM.bat pcm3pcm.x AD.WAV AD.PCM
 NV��H�:. 
>. <. Bn��C���A��������������Hx Hx  Hn��N�  �O� Hx @Hy   Hn��N�  *O� Hz��&ּ��� /N�  HHz��/N�  V( O� g"/Hz��G�  �N�Hz��N�/N�  �p�` Hz��Hn� N�  V( Hn��Hz��/N�  6O� K���z ڍ`46C//Hz��/I�  N�Hm Hz��/N�///Hz��/N�O� 0�G�Cl�/I�  �N�&ּ��� /Hz�{G�  �N�Hz��N�Hz��/N�  HHz��/N�  V( O�  g/Hz��N�Hz��N�/N�p�`LHz�&ּ��� /N�  V( Hn��Hz�Y/N�  /N�  �/Hz�G�  �N�O�  Hz�N�0.��H�L�8���N^NuLIST_PCM.blk pcmdat:AD.PCM
 pcmend:
 2_MakeBLK.bat blk LIST%d_Tx.blk
 blk LIST%d_Tp.blk
 blk LIST_PCM.blk
 LIST%d_Tx.blk Tx LIST%d_Tp.blk Tp %s%05d:%s%05d
 NV��H�:. 
=n ��=n ��8n B@6@< :@6r�Lg
r�Lg|` Hz�,&ּ��� /N�  HHz��/N�  V( O� g/` PHz��&ּ��� /N�  V( Hz��/G�  N�Hz��/N�/N�  �/Hz��G�  �N�O� $`  �Hz��&ּ��� /N�  HHz�/N�  V( O� g/`  �Hz�8Hn� N�  V( 6PO`$6C/Hz��/I�  N�/Hz��/N�O� �n���n��o�Hz��/N�  /N�  �Hn� Hz�G�  �N�Hz�N�p `  �*ڼ��� .޼��� ��  gr�Lg2K/	Hz�a/N�  (Hz�c`2K/	Hz�\/N�  (Hz�^/N�  HO� Hz�4/N�  V( POg /Hz�"G�  �N�Hz�6N�/N�  �p�`tHz�D/N�  V( PO0C////Hz�/N�  �n��RFO� �|�oBFRK`
�n��o�:| /N�  �/Hz�!N�  �O� ��  g �(Hz� N�  �0H�L�8���N^NuMACSsrc.s .include MACS_SCH.h

 SET_OFFSET

 DRAW Tx%05d
 WAIT %d
 PALETTE Tp%05d
 CHANGE_POSITION

 SCREEN_ON

 PCM_PLAY pcmdat,pcmend-pcmdat

 WAIT 120
 PCM_STOP
 EXIT
 has -u MACSsrc                  hlk -r MACSsrc                  MakeMCS MACSsrc                 3_MakeMACS.bat %s  LIST%d_Tx  LIST%d_Tp  LIST_PCM

  NV�fH�=n 
��=n ��=n ��0. :. BF-|    ��-n����8.��RDJ@g�| g R` hHz�v&ּ��� /N�  HHz��/N�  V-@��O� f dHz�Hn� N�  V-@��Hz�B/ G�  N�Hz�J/.��N�H�/G�  ~N�XO// I�  �N�PO& 0n��/N�XO// N�POK�  ./ /N�PO& 8H�/N�XO// N�PO/ /N�PO* /N�XO// N�PO-@��// N�PO-@��/N�  �XO// N�  <POH�/ Hx <N�  �PO=@��=n����O� 0n��-H�~SH-H�f` �0n��/N�  ~XO// N�  �PO/.��/ N�  �PO-@��Jn��fBE-|    ��-n����`  /.��G�  �N�XO( */.��/.��N�  ZPO& /N�XO, ./.�~K�  ~N�XO// I�  �N�PO/ N�XO-|  V�z/<����/<?�v�//  n�zN�O� -|  b�v// // n�vN�O� -|  n�r// // n�rN�O� // N�PO( /.��N�XO* ,/N�XO-@�j-A�n/.�fN�XO// N�PO/ N�XO/<����/<?�v�//  n�zN�O� // /.�n/.�j n�vN�O� // // n�rN�O� // N�PO& /N�XOI�  X// N�PO( */N�XO// N�PO// //N�  !2O� J�l:.��`:.��SE/.��I�  �N�XO& (0E/N�  ~XO// G�  �N�PO/ N�XO/<����/<?�v�// N�  VO� // //N�  bO� // N�PO-@��/.��/ N�  PO/.��/ N�  ZPO-@��JEn
4.���n��fZ0n��/Hz�5/.��G�  N�O� 4.���n��gH�/Hz� /.��N�O� 0n��/Hz�/.��G�  N�Hz�/.��N�O� 4.���n��fHz�/.��G�  N�Hz�/.��N�O� 4.���n��4.���n��l �Hz�	/.��G�  N�Hz�/.��N�Hz�/.��N�/.��N�  �Hn� Hz�;G�  �N�O� $Hz�@N�p ` *Hx `Hy  Hn��N�  *O� Hz�&ּ��� /N�  HHz�/N�  V-@��O� g$/Hz�nG�  �N�Hz�N�/.��N�  �p�`  �Hz�Hn� N�  V-@��G���/Hz�v/.��I�  N�Hk  Hz��/.��N�=n����O�  `.6n��/Hz��/.��I�  N�/Hz��/.��N�O� 4.���n��4.���n��l�Hz�~/.��G�  N�Hn��Hz�/.��N�/.��N�  �Hn� Hz�N�  �O�  Hz�N�  �0H�L�8��FN^NuCG%05d �` CG%05d �܂� %d ����������o�b�`�t�@�C�����쐬���܂�(���v�F%d��)
 ����̒����� %d��%d�b �ł�
 MACScnv.x <�J�n�ԍ�> <�����l> <�I���ԍ�> <����Đ�����[s] or PCM�̎���[s]>
 ex. >MACScnv.x 0 4 1620 81
 ------------------------------
 �ϊ��ɕK�v�ȃc�[�����t�@�C���F
 =< 0_MakeTxTp.bat >=============
     �摜�\���@�@�@�@�FAPICG.r
     ��ʃ��[�h�ύX�@�FYGMC.x
     �e�L�X�g��ʕۑ��Ftxs.x
 =< 1_MakePCM.bat >==============
     Wav to ADPCM�@�@�Fpcm3pcm.x
 =< 2_MakeBLK.bat >==============
     �o�C�i�������@�@�Fblk.x
 =< 3_MakeMACS.bat >=============
     MACS�w�b�_�@�@�@�FMACS_SCH.h
     �A�Z���u���@�@�@�Fhas.x
     �����J�@�@�@�@�@�Fhlk.x
     MACS�f�[�^�ϊ��@�FMakeMCS.x
 ver1.0.2
 H�(o (o  &f ^/, G�  !�N�r 2 HABAʼ  ����/, N�: /, N�r 2 HABA̼  ����/, N�< $HBH�"O� �Af$HBH� �AgJEnp�` |$HBH�($HBH�& S�6EK�  �/$��/N�PO/ ///Hz��M�  �N�4H� <HB0H�/ 8FHx </N�PO/ Hz��N�O�  Hz�lN�///a �Hx  ///M�  �N�:@O�  Hx ///N�Hx Hx  Hx  Hx  N�O�  Hx :M/Hx Hx  N�/Hx  ///G�  N�O� $Hx  Hx /Hx Hx  N�O� `  �Hz�AG�  �N�Hz��N�Hz��N�Hz��N�Hz��N�Hz��N�Hz� N�Hz�N�O�  Hz�+N�Hz�GN�Hz�bN�Hz�~N�Hz��N�Hz��N�Hz��N�Hz��N�O�  Hz��N�Hz�5N�Hz�N�O� p L�xxNuL�   @J�gL   g�S�g@�I 0HA2 ��   d.p����S k$H�Q����JgSB "A$AH��`Q��� / Nup����S k
$H�Q��� J�S�H�  &(*,"A$AH��`Q���BBS�d�L� x / NuL�   "/ oS��Q���BAS�d�NuL�   �f�NuNV  /N�  "@&@��  fr#�  app `BHx  /. /N�  %�O� J�mHx�/ /. N�  &�'@ O� l/N�  �p XO` &n��N^NuHy  ��Ho // N�  -�O� Nu/&o J� g
/N�  4�XOJ� m/+ N�  5HXO/N�  5�XO&_Nu// Ho // N�  -�O� NuNV��/&n A���!K -K��!|    -|   ��-|  ���r�-A��r -A��/Hn /. N�  -�B3 &n��N^Nu /  @jD�r H@J@f bH@�| �b  ��| bR�| b&�| be  <?�  ` r �H�| �|@ H@` �  fr �H�|@H@` �r �@�| �|@ H@` ʰ| ?b$  fr �|@0H@` �r �| �|��H@�` �  fr 䘀|@PH@` �r ��| �|@`H@` z�|�bR�|�b&  	fr 蘀|@pH@` Zr ��| �|@�H@` H  fr 였|@�H@` 4r ��| �|@�H@` "�|?�b(  fr ���|@�H@` r �H@�| �|@�H@` �J@kr �H@�|@�H@` �r �H@�| �|@�H@` ̰| �b  İ| bR�| b&  fr 0<H@�` �r ��| �|A H@` �  fr 嘀|AH@` ~r ��| �|A H@` l�| ?b&  fr �|A0H@` Tr �| �|��H@��` @  f" p ��|APBHAH@0BA`  " p ��| �|A`BHAH@0BA` �|�b  ��|�b>  	f" p ��|ApBHAH@0BA`  �" p ��| �|A�BHAH@0BA`  �  f" p ��|A�BHAH@0BA`  �" p ��| �|A�BHAH@0BA`x�|?�b8  f" p ���|A�BHAH@0BA`TH@�" �|���| �|A�H@HABA`:J@kH@�" �|���| �|A�H@HABA`H@�" �|���| �|A�H@HABA`  $j� NuL�  H@4 �|�|8 c �|�d$2 �|� �| �B瘀AH@BA癀ANuH@���   Nu�|G�f$ ļ�� ��f�|� �|�H@B@Nu�����H@NuL�   C"D4<�(HD�Bg  �& HC�Bg  ��D�|?�m  �( ��$< ������ �� $E* HE��HA��ʼ  ������* BE��H@0*
r J�k�| �c@k.�`JCg6k$����H@�CH@J�j� (	&Nu <���(	&Nu <�   ؄�(	&Nup (	&Nu / " HA�|�g �|�g " �H@�|���|8 H@�¼�   Nup " NuHA¼ ��f
���  p Nu�����r�NuL�  $ ��gDH@4 4@�| �| H@�|�|?�k*�J�| kg�| 
b2 HA��HA0`DB�4
jD�Nup Nu 
j <�   Nu <���Nu"/  H@�o f
�� 
p 0Nu jD�"/ gjD�`hafD�Nu"/ g
j�D�aVD�Nup NupNur ��Nu$ BBHB�� B4 ��0H@0NuH@HA����  ��Nu"/  H@�o f
�� 
p 0Nu "/ g���g�b���   e�4 �Ag�"@$A��   d$HB; 8��`$HB�J; (PB�䨀���  ��$ "
��HA��HABAԁ��cS�Nu L�   C"D$E4<�& HC�Bg~(HD�Bgv�|?��Dix( ��$< ������ �� $ HB��HA:��BEHEBBHBԅH@��Ђr J�k�JCo,`
�| �iBo"���� ��H@�CH@J�j� *
(	&Nup Nup *
(	&Nu�� <�   ҁ�*
(	&Nu <�   ؄�*
(	&NuL�   C"D4<�& HC�Bg �(HD�Bg �4�Cgj�|�e �DD�D`�|�b �6�A�D$< ��J�k� ҁ�`� ҁ�D�J�k
��� Ѐ`
��� ЀD�Ёg `( jD�H@J@f `H@�| �b  ��| bX�| b,�| be ,p � �| c $` �H@툖|�c ` �  fH@눖| c �` �H@鈖|
�c �` ��| ?b&  fH@爖|
 c �` �H@刖|	�c �` �  fH@Ѐ�|	 c �` lH@�|�c �` ^�|�bR�|�b&  	fH@∖| c |` <H@䈖|�c l` ,  fH@或| c V` H@舖|�c F` �|?�b&  fH@ꈖ| c *`  �H@숖|�c `  �J@kH@| c `  �ሖ|�c  �`  ��| �b  ��| bL�| b$  fH@| c  �`  �H@툖|�c  �`~  fH@눖| c  �`jH@鈖|�c  �`\�| ?b   fH@爖| c  �`BH@刖|�ct`6  fH@Ѐ�| cb`$H@�| �cX`  	fH@�`H@��| �kH`  ��H@�CH@J�j*� (	&NuL�  A ` �< HA�|�g(	&Nu (	&Nup (	&Nu <�   ؄�(	&Nu _�� "/ N� _�� "/ N� _�� "/ N��/T� � � �   g  t`  ��  g  
t#�  ap�'t  "NuA� �� C� E� 
4�T�P� g ��"� "� JPk ��4�JPk ��4�� P� g ��   g  t`  ��  g  
t#�  ap4�t P� g ��Q�.�/Q  "Nu / "/ �0d  h  #�     ap`  #�     apNuH� (/ */ 9    WFg  J9   WFf  @�  g  v `  Hx  � XO& //a �PO( *J�o  6".C|��.A`  (9   WFg  //a ��`  
//a �VPO( * "L� 8NuNV���.T  �.T� � (�.t �� .�������f  J���f  p `  .���	p J�g  p�`  pN^NuA� �� C� E� 
NV��4�T P� g ��"� "� JPk ��4�T�P� g ��"� "� JPk ��4�(JPk ��4�t P� g ��-Q��-Q�� .�������f  J���f  p `  .���	p J�g  p�`  pN^NuNV��/$. &.  . ". �,-@��-A�� .�������f  J���f  p `  .���	p J�g  p�`  p&.��N^NuH� ,/ ./ (/  */ $9    WFg  N9   WFf  @�  g  v `  Hx  � XO& ////a ��O� $ J�o  >".C|��.A`  09   WFg  ////a �4`  ////a �O� $  L� �Nu`#HUPAIR #LIBCV1 O�  ^f/N�  7B o p t g8  g� 	g� +g -ft 0e :d�   ЀҀ�Ё`�JgD�Nu"<  ��A�  ��PHp J�f `t$҂A� $Ђ��  \o�p NuUnknown error No error Domain error Result too large Arg list too long Permission denied Resource temporarily unavailable Bad file descriptor Resource busy No child process Resource deadlock would occur File exists Invalid address File too large Interrupted function call Invalid argument Input/output error Is a directory Too many symbolic links Too many open files Too many links Filename too long Too many files open in system No such device No such file or directory Exec format error No locks available Not enough space No space left on a device Block device required Function not implemented Not a directory Directory not empty Inappropriate I/O control No such device or address Operation not permitted Broken pipe Read only file system Invalid seek No such process Text file busy Improper link Read only device file system  H�&o "o */ �< rgn�< ag`>�< wg`6v`@vh`<v\`8rb��gmr+��g
`rt��g`�| `�| �`�|�`r#�  app�`  �H�0@��  f�r��g(<   �| /N�  9jXOJ�f
p�`rxp Ћ'D r &�'@ '@ 0H����|  '@ r�'A r 'A 0�| �| fp` gp`p  gr�� gr�� g�| �Jlr@��L�8NuNV�~H� (. ".  ��   �gp��fp���-@ t��-@ ". p`��fp`��  WB��-A .  gp`.   gp`p ? /�=& \OlJ.  g0A�  ��  �"9  atF����|� / N�  9�XO? /�<& \Ol,/N�  :�#�  app�`xp�� t��f?�>t
#�  ap`�.  gHx  Hn�?�@O� 
J. l/. /N�  :�POJ�m�Hx /Hn��N�  ;BO� J�g�/. /Hn��N�  ;dJ�m� L� �vN^NuNV��H�,. .. (n *n  G�  ^�t��gmt��g
`&t��g`t-B��*`t-B��z`t-B��z`B���*.��J� g  �.  g  �J���gH ��5 r  .��lF�// //N�  < O� , .t v ////N�  <�O� t��f�`  �r  lF�// //N�  <�O� ( *SKr  lF�// //N�  =�O� ��D�� ,.t v ////N�  <�O� t��f�`@(J���g ��5 $.���f�`&//N�  PO& //N�  >�PO��D�5 (f�p�Ћ$<  ^f�� n  � L�8���N^NuNV��H�&. *. (n (.  t���fv&DHn��Hn��//. /. N�  >� @r J���o`� .��S���J�n�R���`� 0J�n g� .r`�fR�` S�J�n�`� 0 S�J�n�J�g(��  g" ft (�`# 0fR���e� .fSKRK��"n "� L�8��N^Nu0123456789abcdef  NV��H�(n (. *n &. $z���f8| &CHn��Hn��Hl /. /. N�  >� @$.��S��RKO� ��  n g� .`�fRL` SLn�`� 0 SLn���  g" fz *�`# 0fR���e� .fSKRK� #J�lD�� -`� +z	��m� 0Hz�,Hn��Hx 
Hx��/r  lF�// a �� @`� .��S���J�n��"n "� L�88��N^NuNV��H�,. .. (n *n *. &n (.  &r -A��r���fvHn��Hn��///N�  >� .��S�O� r���n��nJ�fJ�gS���//Hn��////a ��`J�oS�/Hk��/Hn��////a �^J���o".���� L�8���N^NuNV  H�(. &n *. (n *n  .  ��  gR� 	gR�"  ��  �gT�.�� l,. `,p`��f&`/Hx  N�  ?POR���n��  g//`B ��  �g@/Hx 0I�  ?N�POJl
/Hx xN�`  g/Hx X` 
g/Hx bN�  ?POp`��t@��f&`/Hx 0N�  ?POR���n�&`/Hx 0N�  ?POR��� m�&.  `/H�H�/ N�  ?PO S�J�n� g&`/Hx  N�  ?POR���n켍l ` L�8���N^Nu0123456789ABCDEF (null) (NaN) (Infinity)  NV��H�&n (n ~ &ּ����-C��`v%��g/. /N�  ?R�POH�0@��  f�SL��  f/. N�  ?jJ�g 6p�` �x -D��v�-C��*DRLH�H�vx��b �Ѐ6;N�0 �xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx�$x�\x,���������xxxxxxxxxx�pxpx�xxTxxxxxxxx,xxx,xxxxxxx�
�pppD�xxLx��xx x0xx�/. N�  ?jJ�gp�` � ` ���  f ��:|  ` ��v��` ��:| +` ��v-��g ��J���n ��v@��` ��v��` ��v��` ��v��` ��RL *f"`,r z  y  X�` �ЁЀH�"@C��"	0 X f�SL-A��` �L *f-[��l �>D���v ��` �2r z  y  X� �ЁЀH�"@C��"	0 X f�-A��SL` �  f  g [0�` � [ �` � g "`  gXKp 0+��* `* r * , g t v ////N�  <�O� v��g�| Hz��/.��Hx /.��///a �z* /.��/.��/.��Hx  ` �v�����fv-C�� g "`  gXKp 0+��* `* r * , g t v ////N�  <�O� v��g�| �Hz�P/.��Hx /.��///a ��* /.��/.��/.��Hx  ` � g "`  gXKp 0+��* `* r * , g t v ////N�  <�O� v��g�| Hz��/.��Hx /.��///a �p* /.��/.��/.��Hx  ` � g "`  gXKp 0+��* `* r * , g t v ////N�  <�O� v��g�| Hz�T/.��Hx /.��///a ��* /.��/.��/.��Hx  ` XKk����Hx /.��/.��Hx  /Hn��` >v�� g "`  gXKp 0+��* `* r Hz��/.��Hx 
/.��/// a �x* /.��/.��/.��Hx  `  �v�� g "`  gXK0k��` [r  lF�* ,t v ////N�  <zO� v��o//N�  ?zPO* ,:| -Hz�N/.��Hx 
/.��///a ��* /.��/.��/.��////. a ��ހO� 8` p*f*<  -wJ���l/N�  ?�XO` /.��Hx  /N�  ?�O� J�g��` .��-@��/ / /.��Hx  `  �*,Hx  Hx  //N�  !2O� J�l//N�  ?�PO* ,:| -//N�  ?�POJ�g*<  -~`//N�  ?�POJ�g*<  -�v�-C��` �R ffHn� /.��Hx  //.��//a �j`T eg Ef*Hn� H�H�/ /.��Hx  //.��//a �* O�  `$Hn� H�H�/ /.��/Hn��//a ��* O� /.��/.��/.��////. a ��ހO� `/. H�H�/ N�  ?R�PORL` �L�8���N^Nu/&o ��  fN�  @`z+  
g*"+ �� c "+ ��//+ /+ N�  @xO� J�n4`(+  
g* Ы Hx D�/ /+ N�  A|O� J�l
 k  
p�`k�� 
'k  r &�'A p &_NuNV  H� *. A�  a| �J�g?�>TOJ�lb/ N�  :�#�  app�`X �( ؼ  a�|�?<��/�C& \O?�>TOJ�l/ N�  :�#�  ap `"J�m?<��/�C\O��g?/�C\O/N�  B�L� x��N^Nu/&o J� g+   
g/+ N�  B�XOr 'A 'A 'A &�'A r�'A r 'A p &_NuH���p #�  ^�L�NsNV  H� r#�  ^�Hy  6(v?�%\O@�/ ?�% gp�` 9  ^�L� ��N^NuNV��/J� g
a�J�gv `v?< Hn��/< �� ��O� 
J�f
N�  Cdr�� &.��N^NuNV  "|  �p�NOr��b@Ѐ2;N� 
    Hx `Hx  a��  WG` "|  �p�NOJ�g
�   WG`�   WG9  WGH�H�N^Nulibc: setblock failed.
  NV  / . #�  `�tԀ B#�  `�м   #�  `�#� $  `�#�    `�#� (  `� ( R�#�  ��/9  `�/ N�  C�#�  ��/9  ��N�  ?�й  `�tЂt���#�  `�#�  ��/9  `�/ /9  ��/9  `�N�  D:#�  ��R��й  `�tЂt���#�  `�"@ y  `� PXHO� `�f�Jf�BpЉt���#�  `�"@#�  �|#�  �� y  `�`"�Jf�Jf�t "�pЉt���#�  `�й  X�tЂt���#�  `�й  X�tЂt���#�  `� y  `�#�   `�.y  `� 9  `�$9  `֔�// �JPOJ�l&<  � < Hz�j�	XO?�LN�  E�pNO#�  �N�  6��  WFN�  FLN�  F�N�  F�N�  G9    �SgHx  � #�  `�XO9   �Sg9��  WFN�  G�t #�  apJ�  �XgN�  G�/9  ��/9  ��/9  ��N�  �/ N�  G�&.��N^Nu / P�/@ r��er /A `pЯ /@ r���/@ // N�  HD @XO��  f // N�  H� @XO��  fr#�  app `0Ho /N�  H� @PO"/ ��w    � / " ��w   !�� X�Nu/$/  ��   �gp `p gv�� gv�� gv�� gv��"  ��  Ig�| � ��  � ��  � fv`�� ��  � ��  @ fv�� ��  � ��  � fv �� &Nu / nrٲ�op `D�A�  WH� 0 NuNV��H� (. .  g?< Hx  ?�BPOJ�lf`&?< Hx��?�BPOJ�mRHx Hn��?�?O� 
J�n/ N�  :�#�  app�`.. ��f$v�?< /?�BPOJ�l/ N�  :�#�  ap `p L� ��N^Nu / "/ $/ Hx  /// N�  K<O� NuNV  H�&.  �&@��  a|Hx Hx  /N�  �O� ?Bg�DXO'@ Hx  � ( Hx `?��TO/ Hk N�  *O� J�o/� XOr&�r 'A 'n  "+ ¼  � 'A /. Hk tN�  Hp L���N^NuL�  �| bg��Nu����Nu�| b$g C& 6HC���H@0H@&`:0H@BAHA`0�|  b g0H@�| ��H@0H@BAHA` r `
 r �|  �NuL� ��nm��bepNupNup NuL� ��be��bepNupNup NuL� ǈJ�jD�@�J�jD�@�` aD�@�NuJ�jD�@�aD�@�NuL� ǈ"D( ��g  �(��g  ���be  ���e  �g  �J�k  �$E/| z x�RDԂ׃k:��be�g4e�څ݆�⒐���k
�| Q���`ЂӃQ���" &(	*
,NuRD`�r p RD�| b	�&(	*
,Nu�|  	�&(	*
,Nup r &(	Nupr &(	Nup ��&(	NuL� $	��fR$����  ��b"��r Nu"C$D"$ ��&HB��HBx 8BBЂt قHA��HAt 4BAЁقHC"HA��҄(
&	NuH� $A& $( ��*HD��HDr 2BDЄx ӄHB��HBx 8BBЂӄHE$HB��ԁ(	*��"HA��HC�AHD���DHCԃ&"
(��*HE��HA�EHC���CHA҂L� 8NuL�   HA�o f�� 
Nu"/  C$ ��&HC��H@�CHA���AH@&NuNV  Hy  ^�/. /. /. /. /. N�  LrO� N^NuNV  Hy  ^�/. /. /. /. /. N�  L�O� N^NuH�&/ &o /N�  M�XOJ�m0S�A� "P�R�+  f+   gr
��f/N�  4�XOJ�lp�` L�Nu o  ( ��   NuL�  D�@�Nu o r҈Jf� ��NuL�   @S�k�W���g
BBS�d�p NuSH NuL�  $ ��g@ NuA� r 0�|�|�f( �< �< fr NuA� r  ��������  fJ� fr NuH�v  �Ѓ�* ڼ  �� �Ѓ�"<  ��P�&@�� �Ѓ( �J�g/N�  4�XOJ�lv�t$ڂG� $؂��  \o� L�8NuNV��H�(n (. *. /N�  M�XOJ�lt`TJ�fp `  �A�  a| �&0Jl//N�  :�POJ�lp�`  � g&//?�@( O� 
l/`f��g  �t#�  app�`v,&DA���x &*.޼���� 
f� R��R�R�S�J�nzJ�f��  �o4//?�@O� 
" ��oJ�l/N�  :�#�  app�` ��Ё`
v  GJ�g� L����N^NuNV� H�.. &n ,. A�  a| �J�gp��
p `  �v ?< /8?�B* PO?< /?�B( POr��fڋ`r��f*ڋ`
J�gr`*J�lr`��lx/N�  M�XOJ�lr#�  app�`  �Hx Hx  Hn� N�  �O� ,ܼ��� `6 ����   o <   ؀/ /?�@O� 
& l/N�  :�#�  app�`8��n�`0Bg/?�B& POl/N�  :�#�  ap `A�  a| �p��
 L����N^NuNV  / . " �&A��  a|?+ ? ?< �D\Ot�� fHk t�AXOHx Hx  /N�  �p &n��N^NuH� / gZ&@YK&Ƽ ��� wg
t#�  ap`>A�8 Jf֐/N�  N*XOJ+��f +��"Ҁ'�8� K�� �`'�8�&�/N�  NbXOL�NuHx  � 3�   ��XOJ�o/ � XONu#HUPAIR H�&o (o xv Hx Hz��Hk��N�  N�O� J�W�#�  �`RK  g�Jf^BBp `dJ�  �ft\��f 'g "f�`8J�g��f.v `,t'��gt"��f& `t ��fB`RK  g�JgR�`�H�H�f�BB L�NuH�(o */  *o $&o (v J�  �g*/9  ��N�  ?� @��  ��Hh Hy  `�N�  HO� `DHl pHy  `�N�  HHl �Hy  `�N�  N�Hy  `�N�  ?�S�A�  `�O� 0 . fB0  �+�  `� R�x I� `  � -f  �+ + f|+ - ft hf+ : fHk N�  !�#�  X�XO`\ sf+ : fHk N�  !�#�  X�XO`: pf
r��  �P`* ff
r��  �P` gf
r#�  �X`
 �+� R�RLJf�R���n �^ �r +�  L�88Nulibc: this version of human68k is not supported.
  NV  /�0��  ����   n&<  � < Hz���	XO?�L��  �n"Hx  � XO0|@!P �XH��  �c�J�o/ � &.��N^NuNV  pNO��  �jм �� N^Nulimit_core  NV  A�  � ����!|��   "9  `ڒ�  `�!A !y  X� Hz��N�  N�XOJ�g
/ N�  !�`p A�  �< �"9  `ڒ�  `�!A r`!A  <  �"@ @XHp r0"�PIPHP���l�N^NuNuNuH�v I�  a|(<  X�G�  Y
///N�  ;dr(�O� I� X�R�r��l�L�NuH�v  <  ��rҀ(A&@PK(<  ��/N�  O.XOJ�gA�  Y2 � 0 `p /  �м  Y/ /N�  %�k�� (�O� I� $G� $r$؁R�r��l�L�NuJST   NuH�6/ 8/ &y  `�` �[f�[f�[f�[f�[f�[f KXK PN���  `�e�L�NuHxC+HxJ�a�PONuHxC-HxJ�a�PONuNV  H�&9  �\S� �&@��  �`` SN�YKS�J�l�J�  �XgN�  G�/. N�  OZL���N^Nu"/  y  ���  g:��c 9  ���g,"h  h `��b `��b 	` h "i Q���B@S�d�p NuH�&/ &y  ���  f&y  `�XK'|   �� Ѓ"9  `�Y���J�o/ N�  O�XOJ�lp `A�8 #�  �� ���� L�NuH�&o (o "& ��t��m/N�  N*(� XO`��&���'C�� L�Nu:/  / NV� H�(n *. /&ּ��� /N�  HHx //N�  O�( R�Hx Hz��Hl N�  PO� J�gHz��/N�  P> @PO��  f/N�  P�`  �//N�  H/N�  P�O� G���&ּ���`h .f.( . f&J( f J�g��g,Hx //N�  O� @PO��d8B`4 .fJ( fJ�g$��f | /��/Hk N�  H//N�  N�O� Hz�Hx  N�  P> @PO��  f�J, fHz��/N�  N�L�8��N^NuNV� H�&n /N�  P� �< AA� | :�| /�B.�R�XOHn�? �G\O /g \f/Hn�`+ : f,+ / g+ \ f/Hn� N�  HPO`2| /��Hk `| /��/G���Hk N�  H/Hn� N�  N�O� &ּ��� /N�  Q(/. /a �//. N�  HL���N^NuNV�tH�(n *. /. Hn�|N�  H-@�t"Ҽ����-A�xv PO.Y�(ؼ���|,T�G��x/.�tN�  P�XOJ�m$�< A@��| :��B.��/.�x/Bg��O� 
J�l/ N�  :�#�  app `  �rP��g(mr@��g
`Fr`��g`>//.�t/.�xa �tvO� `,//.�t/a �`//.�xN�  N�A��tO�   �&�`r`$J�g �Z/.�xN�  QZ/.�xN�  ?�PO�� er#�  app `/.�x/N�  H/ N�  Q( L���XN^NuNV��H�(. &n *. v�֎//. //. /. N�  Q�"C EO� `Jf`� 0 S�J�l�`
� S�J�n�BH�H�/ //N�  Stѓ L�8��N^NuNV��H�(. &n *. v�֎//. //. /. N�  Q�"C E$O� l  D���m" `"`� 0 S�J�n�| &�"҂`Jf`� 0 S�J�l�`
� S�J�n�BH�H�/ //N�  Stѓ L�x��N^Nu/&o  + ��  r��f2+  
f&�  k  
`8J�n4/+ /+ /+ N�  @xO� J�n
 k  
p�`k�� 
'k  &� p &_Nu"/ m*t_��m$A�  a| �J� gA�  a| �tİ fp�`p Nu o "h ��f
p #�  ��`��  ��f#�  �� h !I #H S�  ��Nu"o $y  ���  f#�  ��#I #I ` j !I #H #J %I R�  ��NuL�  $
oS�p r g�V���fBBS�d쐁Nup NuL�   Jf�SH�f�NuH�(/ /N�  ?�& &y  �|XO`(///N�  N�O� J�f S0 =8 f ГR�`XKJ�f�p L�NuA�  a| / �J�gr° gp`
r #�  app NuNV  H�N�  S�N�  T2N�  F�&9  ��S� �&@��  ��` SN�YKS�J�l�J�  �gN�  TzJ�  �gN�  T|?. 
�LL���N^Nu/ / &9  `�J�gй  `�/ N�  T~XOJ�lp�` &Nu o / p g�f S�f�Jf S�NuL�  "
oS��V���f
BAS�d�p Nup  r !��NuH�&o (/ ��  f &y  ab��  fp `l//N�  T�POJ�gH�8@��  f��  fr #�  abp `>&S�`//N�  T�POJ�fH�8@��  f��  f
r #�  ab`
B+��#�  ab L�Nu /  @"y  \�r g� f�NuNV�� n ( : fp  y  \�r 0 t�҂`Hn��/�)r .�� N^Nu"/ "Ap `  y  ]�0  gRI`
 \f� /RIJf� Nu"/ "A Ap $y  ]�`"H2  gJ) gRHRHJf� /g \f��d
) :��gB NuA� C� "¼�   ���  ��f"¼ �� �� ����f ) �� NuNV  H�0. �|��H�|�6 �� M��kBt��m$A�  ZZ �/0/0 /. /. N�  V`  �p��A�  ZZ�"�C�  [�`@"D�t��m"A�  YZ �/0/0 /. /. N�  V`@p��A�  YZ��C�  [Z�G�  V/1/1 /0/0 N�O� /. /. // N�O� -@ -A /. /. /<gN� /<C��ma ��O� J�mR�/<����/<?���`,/. /. /<�ؠ /<Cv4Wa ��O� J�l&S�Hx  /<@$  /. /. N�  VO� -@ -A R�$n $�. �< �p $n $�/. /. /. N�  UPO// N�  U�O� L���N^Nu"/  / t4�� l @`R 9np `� 0SH��c�RH� 1`� 0��b�pNuH�v (I�  ��,� �Ѓ�* ڼ  �� �Ѓ�G�J�g/N�  4�XOJ�lv� Є�4 
gм  ��/ N�  �XOJ�lv�P�r$ځG� $R�r_��l� L�xNuH� x *&/N�  V`XOJ�m/N�  5HXOJ�l*9  apx�R�r_��l�J�l#�  ap L� 8NuNuNuNV  /&. ּ  ��|�  9  `�й  �(��  `�e��br#�  app�`< y  `��� g( 9  `�"��// �JPOJ�l/ N�  :�#�  app�`#�  `�p &.��N^Nu o / g� gf�J gp NuSH NuL�  $ k  ���g  �H@4 �| �| H@�|�|?�k  ��J�| 4kgx�| b| C&6 HC���HC0&`\DB�| gb" C&6 HC���HA2HA&`:2 HAB@H@`0�|  gb2 HA�| ��HA2 HAB@H@`" p `
" p �|  ��ANup " Nup�" NuNV  H� L�  A�  afBPB� B� p?x	��   
�� ��Q���Q���"n t	f
Q���� 0`.�f  0�S�e� 0�  0�Q���BL� N^Nu / mr_��mA�  a|�r�� gp�`p NuJafar 1.1.32    "n  "|  "�  "�  "�  "�  "�  "�  "�  #
  #  #9  #E  #U  #d  #~  #�  #�  #�  #�  #�  #�  #�  $  $+  $E  $W  $j  ${  $�  $�  $�  $�  $�  %  %  %4  %@  %V  %c  %s  %�  %�   +                                                     (      "         
      "         %   "      &      '                  
         
          (((((                  �DDDDDDDDDDAAAAAABBBBBB                                                                                                                                    W�  �    con con con aux prn        !   !   B   !r   w   w   a+b w                    G�  G����p���p���pCv4W�ؠ C��mgN� C�X�`�= D�x��@DK����PD���MՒD�-��J�D�xCyٝ�E �*,(�ET���24E��q���E�'�/�E�1����!F)>Y9���F_���#F����nFȦ�"FɜF�Л��|G3Bar�M�Gh��y �G��CWiG���*��H����JH=c)��\�Hr]�7�H��x��`Hܲ���xI��Y�}KIFk��C\�I|��T3�I��'���\I��1���sCv4W�ؠ CA�y7�� Ck�&4  Bּ��  B�0��@  Bm��   B7Hv�   B�_    A��e    A�ׄ    Ac�    A.��    @�j     @È     @�@     @Y      @$      ?�      ?�������?�z�G�{?PbM���?6��C->�����h�>������>z�򚼯J>Ey��0�;>.�&֖=�|��׽�=���yd�=q���-�=<%�hIv�=����+�?�      F����nM8O�?��S��ubسhZ�wH�04a&��%o��g��$�nqJR��g�?�      9I�#ը�52��D��@,�^���%[�ό���~�{n{?�nꄀm����F6� 	
 !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~��������������������������������������������������������������������������������������������������������������������������������   [�                                 @@@@@@@@@@@@@@@@@@@@@@��������������������������������������������������������������� �����������������������������������������������������������������������������������������������������������������������������      \�f  
       " .    2     "        &   :        (  �       L   .   *     "  � (         ( 6 ( 2     
       0 2   � z  J N ,X         
  P6 j 6    H�  L F  R � * � F     ,   . v  $   " 0   N � � * 2       x "0 v N � � ~ ~ �  T   :      � & * * >       & ( 4     @ H  
  2                    
   "              * 
         
 
          <    � ^  &  ,   . :�     0 �   >   4 ^  : j   :     
 , . .  * N   �    
    
      >       � "   
   
    *    (     
 @ ( 2    
   B     0 B       : 2 
    ` &    ( 2 2  L $ 
    : < < d N :  *      $ R   . "           
  v         * 2 0 �       
  � 8  Z     4    &    
    
 � � $                                          � \ �  "_CGcnv    �_BLTcnv   _MACScnv    �_main   �_memset   *_memcpy   H_strcpy   V_fopen    �_printf   �_fclose   _fprintf    (_sprintf    ~___floatsidf    �___truncdfsf2   .___divsf3   �___extendsfdf2    <___fixdfsi    �___divsi3   ___udivsi3    �___mulsf3   Z___addsf3   ___subsf3   V___muldf3   b___adddf3   n___subdf3   z__f_fabs    �__fpu_fabs    ,__fe_fabs   X_fabs   �__f_cmpdf2     :__fpu_cmpdf2     �__fe_cmpdf2   !2___cmpdf2   !�__main    V�__libc_release    V�__libc_version    ^flibstack    !�_atoi   !�_atol   "@__getemptyiob   V�_sys_errlist    W>_sys_nerr    ap_errno    %�__sopen   WB__fmode   &�_open    at__umask    ax__pause    a|__fddb     �|_environ     ��__argc     ��__comline    ��__envp     ��__argv     ��__iob   -�__doprnt    4�_fflush   5H_close    5�__sclose    WF__havefpu   WG__ohavefpu    6�__iscopro   7B__start   9j_malloc   9�__mode2dos    :�__errcnv    :�__seekeof   ;B__fullpath    ;d__open    < ___lshrdi3    <z___cmpdi2   <�___ucmpdi2    <�___udivdi3    <�___divdi3   =�___umuldi3    =�___muldi3   >�___mulsi3   >�___umulsi3    >�_ecvt   >�_fcvt   ?_fputc    ?j_feof   X�__ctype   ?z___negdi2   ?�_strlen   ?�_memchr   ?�___negdf2   ?�_isnan    ?�_isinf    @_flushall   @x_write    A|_lseek    B�__close   B�_free   Cd__fpuinit   `�__memcp   `�__procp   `�__psta    `�__bsta    `�__dsta    `�__csta    `�__vsta    `�__esta    `�__fsta    `�__ssta    `�__hsta    `�__last    `�__mmax    `�__PSP   `�__PSTA    `�__PEND    `�__BSTA    `�__BEND    `�__DSTA    `�__DEND    `�__SSTA    `�__SEND    `�__HSTA    `�__HEND    `�__SSP   C�__dehupair     �__hupair    D:__enargv    X�__stacksize   X�__heapsize    E�__vectormove    F&_clock     �__inclock   FL__loadrlimits    �__limits    F�__siginit   F�__sigend    F�__finit   G__sinit    �P__superman    YF_tzname   YN_timezone   YR_timezone_dst   YV_timezone_std   G�_tzset     �T_daylight    �X__cplusplus   G�__ctors   G�__dtors   G�_exit    �\__exitnums     �`__exitprcs    HD__ma_search   H�__ma_new     ��__ma_lptr    ��__ma_fnum    ��__ma_flist    H�__ma_recycle    K<__pathexpand    Lr_econvert   L�_fconvert   M�__swrite    M�__writable    N*__ma_remove   Nb__ma_add    N�_strncmp    N�_strcat    � _STACK_SIZE     _HEAP_SIZE    N�_getenv   O._isatty   OZ__exit     ��__onexnums     ��__onexprcs    O�_sbrk   O�_strrchr    P_memcmp   P>_strtok   P�_strupr   P�__getdriveno    Q(__toslash   QZ__dellastsep    Q�__dtos18    St__round   S�_fcloseall    T2_closeall   Tz___display_block_profile     �__bluse   T|___display_function_profile    �__fpuse   T~_brk    T�_strchr   \�__ctype_upper   ]�__mbctype   U___fixunsdfdi   U�__ulltoa    V`__appfd        6   :   n   �   �   �   �   �   �   �   �   �   �         !    "  .   8 &  B (  X *  ^ -  j .  x /  ~ 1  � 2  � 4  � 5  � 8  � :  � <  � >  � ?  � A  � B   7    �   �   � 	  � 
  �   �   �   �   �   �   �   �   �   �         !   $   &  , (  0 )  4 .  H /  V 0  \ 2  ^ 3  ^ 5  ^ 6  b 9  r <  x >  � ?  � <  � A  � D  � F  � H  � I  � L  � O  � R  � W  � X   Y   ]   ^    _  $ b  2 c  6 e  D f  J h  R i  V l  d m  f p  | r  � t  � u  � w  � x  � y  � |  � ~  �   � �  � �  � �  � �  � �  � �  � �  � �   T    6   8   @   @   @   F   F   L   \   p   �   �   �   �   �    � #  � %  � &  � (  � )   *  , +  V 0  r 2  � 4  � 6  � 7  � 8  � <  	h >  	� @  
  B  
& F  
, K  
, L  
� M  
� N  
� U  
� W  
� X  
� Z  
� \   ]   `  ( b  8 c  F 0  Z g  j h  t i  ~ l  � n  � p  � q  � u  � {  � |  � }  �   � �  � �   �   �    �  6 �  D �  P �  f �  r �  � �  � �  � �  � �  � �  � �  � �  � �   {    �   �   �   �   � 	  � 
  �               H   l   v   �   �   �    � !  � #  � $    *   +   ,   -   .  $ /  * 0  0 1  6 2  @ 3  F 4  L 5  R 6  X 7  ^ 8  d 9  j :  t ;  z <  � P  � Q.file      ���  gMACScnv.c         __stdbuf    �� 
       $         __cnt       ��  __bufsiz   ��  __flag     ��  __file     ��  __nback    ��  __ptr      ��  __base     ��  __pback    �� <                __nbuff     �� <                .eos       $��  f      $          _div_t      �� 
                _quot       ��  _rem       ��  .eos       ��  f                _ldiv_t     �� 
                _quot       ��  _rem       ��  .eos       ��  f                _CGcnv    "  #      �       7  .bf       *   e              5         (   �� 	 _inc_val   �� 	 _end_cnt   �� 	        (   ��  _inc_val   ��  _end_cnt   ��  .bb       6   d              5  _fp        ��       $          _fname  ��� �� 2                _ret    ������  _cnt       ��         4������ 2                       @������ �       @        .eb       �   d     A            .ef       �   e     B            _BLTcnv   �  #      N   �   T  .bf       �   e     T         R         (   �� 	 _inc_val   �� 	 _end_cnt   �� 	 _mode      �� 	        (   ��  _inc_val������  _end_cnt������  _mode      ��  .bb       �   d              R  _fp        ��       $          _ret        ��  _i         ��  _j         ��  _flag      ��  _cnt       ��  _fname  ��� �� 2                _str    ��� �� 2                .eb       �   d     �            .ef       �   e     �            _MACScnv    #      �  �   {  .bf          e     �         y         (   �� 	 _inc_val   �� 	 _end_cnt   �� 	 _mode      �� 	 _sec       �� 	        (������  _inc_val������  _end_cnt������  _mode       ��  _sec       ��  .bb       6   d              y  _fp     ������       $          _fname  ��� �� 2                _ret       ��  _frame     ��  _cnt    ������  _r_count   ��  _b_frame������  _f_spf  ������  _f_time ������         L������  _r_time ������  .bb       �   d     q         w         @������ �       `         .eb       �   d     �            .eb       �   d     �            .ef       �   e     �            _main     �  #      �  \   �  .bf       �   e    �         �  _argc      �� 	 _argv      �� R	 _argv      �� R .bb       �   d              �  _ret       ��  _cnt       �� 3                .eb       �   d     P            .ef       �   e     Q            _va_list    ��  _size_t     ��  _fpos_t     ��  _FILE       ��       $          _wchar_t    ��  _div_t      ��                 _ldiv_t     ��                             �� � _uint8_t    ��         
    ��             ��             ��  _int8_t     ��  _int16_t    ��  _int32_t    ��  _int64_t    ��  _BOOL       ��  _FLOAT      ��  _FL         ��  .text          x  �   �          .data     �   x                  .bss      �   x                  _onexit_t _uint16_t _uint32_t _uint64_t _start_cnt  _sStingDef  _sStingMes  _f_r_diff 