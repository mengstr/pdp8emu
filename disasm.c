/* File: disasm.c
   Author: Mats Engstrom, April 2020
   Language: C (UNIX)
   Purpose: PDP-8 emulator debugging tool

*/

char *ops[4096] = {
	"AND 0000   ;AND operand with AC, ZP 0000 ", // op 0000
	"AND 0001   ;AND operand with AC, ZP 0001", // op 0001
	"AND 0002   ;AND operand with AC, ZP 0002", // op 0002
	"AND 0003   ;AND operand with AC, ZP 0003", // op 0003
	"AND 0004   ;AND operand with AC, ZP 0004", // op 0004
	"AND 0005   ;AND operand with AC, ZP 0005", // op 0005
	"AND 0006   ;AND operand with AC, ZP 0006", // op 0006
	"AND 0007   ;AND operand with AC, ZP 0007", // op 0007
	"AND 0010   ;AND operand with AC, ZP 0010", // op 0010
	"AND 0011   ;AND operand with AC, ZP 0011", // op 0011
	"AND 0012   ;AND operand with AC, ZP 0012", // op 0012
	"AND 0013   ;AND operand with AC, ZP 0013", // op 0013
	"AND 0014   ;AND operand with AC, ZP 0014", // op 0014
	"AND 0015   ;AND operand with AC, ZP 0015", // op 0015
	"AND 0016   ;AND operand with AC, ZP 0016", // op 0016
	"AND 0017   ;AND operand with AC, ZP 0017", // op 0017
	"AND 0020   ;AND operand with AC, ZP 0020", // op 0020
	"AND 0021   ;AND operand with AC, ZP 0021", // op 0021
	"AND 0022   ;AND operand with AC, ZP 0022", // op 0022
	"AND 0023   ;AND operand with AC, ZP 0023", // op 0023
	"AND 0024   ;AND operand with AC, ZP 0024", // op 0024
	"AND 0025   ;AND operand with AC, ZP 0025", // op 0025
	"AND 0026   ;AND operand with AC, ZP 0026", // op 0026
	"AND 0027   ;AND operand with AC, ZP 0027", // op 0027
	"AND 0030   ;AND operand with AC, ZP 0030", // op 0030
	"AND 0031   ;AND operand with AC, ZP 0031", // op 0031
	"AND 0032   ;AND operand with AC, ZP 0032", // op 0032
	"AND 0033   ;AND operand with AC, ZP 0033", // op 0033
	"AND 0034   ;AND operand with AC, ZP 0034", // op 0034
	"AND 0035   ;AND operand with AC, ZP 0035", // op 0035
	"AND 0036   ;AND operand with AC, ZP 0036", // op 0036
	"AND 0037   ;AND operand with AC, ZP 0037", // op 0037
	"AND 0040   ;AND operand with AC, ZP 0040", // op 0040
	"AND 0041   ;AND operand with AC, ZP 0041", // op 0041
	"AND 0042   ;AND operand with AC, ZP 0042", // op 0042
	"AND 0043   ;AND operand with AC, ZP 0043", // op 0043
	"AND 0044   ;AND operand with AC, ZP 0044", // op 0044
	"AND 0045   ;AND operand with AC, ZP 0045", // op 0045
	"AND 0046   ;AND operand with AC, ZP 0046", // op 0046
	"AND 0047   ;AND operand with AC, ZP 0047", // op 0047
	"AND 0050   ;AND operand with AC, ZP 0050", // op 0050
	"AND 0051   ;AND operand with AC, ZP 0051", // op 0051
	"AND 0052   ;AND operand with AC, ZP 0052", // op 0052
	"AND 0053   ;AND operand with AC, ZP 0053", // op 0053
	"AND 0054   ;AND operand with AC, ZP 0054", // op 0054
	"AND 0055   ;AND operand with AC, ZP 0055", // op 0055
	"AND 0056   ;AND operand with AC, ZP 0056", // op 0056
	"AND 0057   ;AND operand with AC, ZP 0057", // op 0057
	"AND 0060   ;AND operand with AC, ZP 0060", // op 0060
	"AND 0061   ;AND operand with AC, ZP 0061", // op 0061
	"AND 0062   ;AND operand with AC, ZP 0062", // op 0062
	"AND 0063   ;AND operand with AC, ZP 0063", // op 0063
	"AND 0064   ;AND operand with AC, ZP 0064", // op 0064
	"AND 0065   ;AND operand with AC, ZP 0065", // op 0065
	"AND 0066   ;AND operand with AC, ZP 0066", // op 0066
	"AND 0067   ;AND operand with AC, ZP 0067", // op 0067
	"AND 0070   ;AND operand with AC, ZP 0070", // op 0070
	"AND 0071   ;AND operand with AC, ZP 0071", // op 0071
	"AND 0072   ;AND operand with AC, ZP 0072", // op 0072
	"AND 0073   ;AND operand with AC, ZP 0073", // op 0073
	"AND 0074   ;AND operand with AC, ZP 0074", // op 0074
	"AND 0075   ;AND operand with AC, ZP 0075", // op 0075
	"AND 0076   ;AND operand with AC, ZP 0076", // op 0076
	"AND 0077   ;AND operand with AC, ZP 0077", // op 0077
	"AND 0100   ;AND operand with AC, ZP 0100", // op 0100
	"AND 0101   ;AND operand with AC, ZP 0101", // op 0101
	"AND 0102   ;AND operand with AC, ZP 0102", // op 0102
	"AND 0103   ;AND operand with AC, ZP 0103", // op 0103
	"AND 0104   ;AND operand with AC, ZP 0104", // op 0104
	"AND 0105   ;AND operand with AC, ZP 0105", // op 0105
	"AND 0106   ;AND operand with AC, ZP 0106", // op 0106
	"AND 0107   ;AND operand with AC, ZP 0107", // op 0107
	"AND 0110   ;AND operand with AC, ZP 0110", // op 0110
	"AND 0111   ;AND operand with AC, ZP 0111", // op 0111
	"AND 0112   ;AND operand with AC, ZP 0112", // op 0112
	"AND 0113   ;AND operand with AC, ZP 0113", // op 0113
	"AND 0114   ;AND operand with AC, ZP 0114", // op 0114
	"AND 0115   ;AND operand with AC, ZP 0115", // op 0115
	"AND 0116   ;AND operand with AC, ZP 0116", // op 0116
	"AND 0117   ;AND operand with AC, ZP 0117", // op 0117
	"AND 0120   ;AND operand with AC, ZP 0120", // op 0120
	"AND 0121   ;AND operand with AC, ZP 0121", // op 0121
	"AND 0122   ;AND operand with AC, ZP 0122", // op 0122
	"AND 0123   ;AND operand with AC, ZP 0123", // op 0123
	"AND 0124   ;AND operand with AC, ZP 0124", // op 0124
	"AND 0125   ;AND operand with AC, ZP 0125", // op 0125
	"AND 0126   ;AND operand with AC, ZP 0126", // op 0126
	"AND 0127   ;AND operand with AC, ZP 0127", // op 0127
	"AND 0130   ;AND operand with AC, ZP 0130", // op 0130
	"AND 0131   ;AND operand with AC, ZP 0131", // op 0131
	"AND 0132   ;AND operand with AC, ZP 0132", // op 0132
	"AND 0133   ;AND operand with AC, ZP 0133", // op 0133
	"AND 0134   ;AND operand with AC, ZP 0134", // op 0134
	"AND 0135   ;AND operand with AC, ZP 0135", // op 0135
	"AND 0136   ;AND operand with AC, ZP 0136", // op 0136
	"AND 0137   ;AND operand with AC, ZP 0137", // op 0137
	"AND 0140   ;AND operand with AC, ZP 0140", // op 0140
	"AND 0141   ;AND operand with AC, ZP 0141", // op 0141
	"AND 0142   ;AND operand with AC, ZP 0142", // op 0142
	"AND 0143   ;AND operand with AC, ZP 0143", // op 0143
	"AND 0144   ;AND operand with AC, ZP 0144", // op 0144
	"AND 0145   ;AND operand with AC, ZP 0145", // op 0145
	"AND 0146   ;AND operand with AC, ZP 0146", // op 0146
	"AND 0147   ;AND operand with AC, ZP 0147", // op 0147
	"AND 0150   ;AND operand with AC, ZP 0150", // op 0150
	"AND 0151   ;AND operand with AC, ZP 0151", // op 0151
	"AND 0152   ;AND operand with AC, ZP 0152", // op 0152
	"AND 0153   ;AND operand with AC, ZP 0153", // op 0153
	"AND 0154   ;AND operand with AC, ZP 0154", // op 0154
	"AND 0155   ;AND operand with AC, ZP 0155", // op 0155
	"AND 0156   ;AND operand with AC, ZP 0156", // op 0156
	"AND 0157   ;AND operand with AC, ZP 0157", // op 0157
	"AND 0160   ;AND operand with AC, ZP 0160", // op 0160
	"AND 0161   ;AND operand with AC, ZP 0161", // op 0161
	"AND 0162   ;AND operand with AC, ZP 0162", // op 0162
	"AND 0163   ;AND operand with AC, ZP 0163", // op 0163
	"AND 0164   ;AND operand with AC, ZP 0164", // op 0164
	"AND 0165   ;AND operand with AC, ZP 0165", // op 0165
	"AND 0166   ;AND operand with AC, ZP 0166", // op 0166
	"AND 0167   ;AND operand with AC, ZP 0167", // op 0167
	"AND 0170   ;AND operand with AC, ZP 0170", // op 0170
	"AND 0171   ;AND operand with AC, ZP 0171", // op 0171
	"AND 0172   ;AND operand with AC, ZP 0172", // op 0172
	"AND 0173   ;AND operand with AC, ZP 0173", // op 0173
	"AND 0174   ;AND operand with AC, ZP 0174", // op 0174
	"AND 0175   ;AND operand with AC, ZP 0175", // op 0175
	"AND 0176   ;AND operand with AC, ZP 0176", // op 0176
	"AND 0177   ;AND operand with AC, ZP 0177", // op 0177
	"AND @@00   ;AND operand with AC, Current page @@00", // op 0200
	"AND @@01   ;AND operand with AC, Current page @@01", // op 0201
	"AND @@02   ;AND operand with AC, Current page @@02", // op 0202
	"AND @@03   ;AND operand with AC, Current page @@03", // op 0203
	"AND @@04   ;AND operand with AC, Current page @@04", // op 0204
	"AND @@05   ;AND operand with AC, Current page @@05", // op 0205
	"AND @@06   ;AND operand with AC, Current page @@06", // op 0206
	"AND @@07   ;AND operand with AC, Current page @@07", // op 0207
	"AND @@10   ;AND operand with AC, Current page @@10", // op 0210
	"AND @@11   ;AND operand with AC, Current page @@11", // op 0211
	"AND @@12   ;AND operand with AC, Current page @@12", // op 0212
	"AND @@13   ;AND operand with AC, Current page @@13", // op 0213
	"AND @@14   ;AND operand with AC, Current page @@14", // op 0214
	"AND @@15   ;AND operand with AC, Current page @@15", // op 0215
	"AND @@16   ;AND operand with AC, Current page @@16", // op 0216
	"AND @@17   ;AND operand with AC, Current page @@17", // op 0217
	"AND @@20   ;AND operand with AC, Current page @@20", // op 0220
	"AND @@21   ;AND operand with AC, Current page @@21", // op 0221
	"AND @@22   ;AND operand with AC, Current page @@22", // op 0222
	"AND @@23   ;AND operand with AC, Current page @@23", // op 0223
	"AND @@24   ;AND operand with AC, Current page @@24", // op 0224
	"AND @@25   ;AND operand with AC, Current page @@25", // op 0225
	"AND @@26   ;AND operand with AC, Current page @@26", // op 0226
	"AND @@27   ;AND operand with AC, Current page @@27", // op 0227
	"AND @@30   ;AND operand with AC, Current page @@30", // op 0230
	"AND @@31   ;AND operand with AC, Current page @@31", // op 0231
	"AND @@32   ;AND operand with AC, Current page @@32", // op 0232
	"AND @@33   ;AND operand with AC, Current page @@33", // op 0233
	"AND @@34   ;AND operand with AC, Current page @@34", // op 0234
	"AND @@35   ;AND operand with AC, Current page @@35", // op 0235
	"AND @@36   ;AND operand with AC, Current page @@36", // op 0236
	"AND @@37   ;AND operand with AC, Current page @@37", // op 0237
	"AND @@40   ;AND operand with AC, Current page @@40", // op 0240
	"AND @@41   ;AND operand with AC, Current page @@41", // op 0241
	"AND @@42   ;AND operand with AC, Current page @@42", // op 0242
	"AND @@43   ;AND operand with AC, Current page @@43", // op 0243
	"AND @@44   ;AND operand with AC, Current page @@44", // op 0244
	"AND @@45   ;AND operand with AC, Current page @@45", // op 0245
	"AND @@46   ;AND operand with AC, Current page @@46", // op 0246
	"AND @@47   ;AND operand with AC, Current page @@47", // op 0247
	"AND @@50   ;AND operand with AC, Current page @@50", // op 0250
	"AND @@51   ;AND operand with AC, Current page @@51", // op 0251
	"AND @@52   ;AND operand with AC, Current page @@52", // op 0252
	"AND @@53   ;AND operand with AC, Current page @@53", // op 0253
	"AND @@54   ;AND operand with AC, Current page @@54", // op 0254
	"AND @@55   ;AND operand with AC, Current page @@55", // op 0255
	"AND @@56   ;AND operand with AC, Current page @@56", // op 0256
	"AND @@57   ;AND operand with AC, Current page @@57", // op 0257
	"AND @@60   ;AND operand with AC, Current page @@60", // op 0260
	"AND @@61   ;AND operand with AC, Current page @@61", // op 0261
	"AND @@62   ;AND operand with AC, Current page @@62", // op 0262
	"AND @@63   ;AND operand with AC, Current page @@63", // op 0263
	"AND @@64   ;AND operand with AC, Current page @@64", // op 0264
	"AND @@65   ;AND operand with AC, Current page @@65", // op 0265
	"AND @@66   ;AND operand with AC, Current page @@66", // op 0266
	"AND @@67   ;AND operand with AC, Current page @@67", // op 0267
	"AND @@70   ;AND operand with AC, Current page @@70", // op 0270
	"AND @@71   ;AND operand with AC, Current page @@71", // op 0271
	"AND @@72   ;AND operand with AC, Current page @@72", // op 0272
	"AND @@73   ;AND operand with AC, Current page @@73", // op 0273
	"AND @@74   ;AND operand with AC, Current page @@74", // op 0274
	"AND @@75   ;AND operand with AC, Current page @@75", // op 0275
	"AND @@76   ;AND operand with AC, Current page @@76", // op 0276
	"AND @@77   ;AND operand with AC, Current page @@77", // op 0277
	"AND @@00   ;AND operand with AC, Current page @@00", // op 0300
	"AND @@01   ;AND operand with AC, Current page @@01", // op 0301
	"AND @@02   ;AND operand with AC, Current page @@02", // op 0302
	"AND @@03   ;AND operand with AC, Current page @@03", // op 0303
	"AND @@04   ;AND operand with AC, Current page @@04", // op 0304
	"AND @@05   ;AND operand with AC, Current page @@05", // op 0305
	"AND @@06   ;AND operand with AC, Current page @@06", // op 0306
	"AND @@07   ;AND operand with AC, Current page @@07", // op 0307
	"AND @@10   ;AND operand with AC, Current page @@10", // op 0310
	"AND @@11   ;AND operand with AC, Current page @@11", // op 0311
	"AND @@12   ;AND operand with AC, Current page @@12", // op 0312
	"AND @@13   ;AND operand with AC, Current page @@13", // op 0313
	"AND @@14   ;AND operand with AC, Current page @@14", // op 0314
	"AND @@15   ;AND operand with AC, Current page @@15", // op 0315
	"AND @@16   ;AND operand with AC, Current page @@16", // op 0316
	"AND @@17   ;AND operand with AC, Current page @@17", // op 0317
	"AND @@20   ;AND operand with AC, Current page @@20", // op 0320
	"AND @@21   ;AND operand with AC, Current page @@21", // op 0321
	"AND @@22   ;AND operand with AC, Current page @@22", // op 0322
	"AND @@23   ;AND operand with AC, Current page @@23", // op 0323
	"AND @@24   ;AND operand with AC, Current page @@24", // op 0324
	"AND @@25   ;AND operand with AC, Current page @@25", // op 0325
	"AND @@26   ;AND operand with AC, Current page @@26", // op 0326
	"AND @@27   ;AND operand with AC, Current page @@27", // op 0327
	"AND @@30   ;AND operand with AC, Current page @@30", // op 0330
	"AND @@31   ;AND operand with AC, Current page @@31", // op 0331
	"AND @@32   ;AND operand with AC, Current page @@32", // op 0332
	"AND @@33   ;AND operand with AC, Current page @@33", // op 0333
	"AND @@34   ;AND operand with AC, Current page @@34", // op 0334
	"AND @@35   ;AND operand with AC, Current page @@35", // op 0335
	"AND @@36   ;AND operand with AC, Current page @@36", // op 0336
	"AND @@37   ;AND operand with AC, Current page @@37", // op 0337
	"AND @@40   ;AND operand with AC, Current page @@40", // op 0340
	"AND @@41   ;AND operand with AC, Current page @@41", // op 0341
	"AND @@42   ;AND operand with AC, Current page @@42", // op 0342
	"AND @@43   ;AND operand with AC, Current page @@43", // op 0343
	"AND @@44   ;AND operand with AC, Current page @@44", // op 0344
	"AND @@45   ;AND operand with AC, Current page @@45", // op 0345
	"AND @@46   ;AND operand with AC, Current page @@46", // op 0346
	"AND @@47   ;AND operand with AC, Current page @@47", // op 0347
	"AND @@50   ;AND operand with AC, Current page @@50", // op 0350
	"AND @@51   ;AND operand with AC, Current page @@51", // op 0351
	"AND @@52   ;AND operand with AC, Current page @@52", // op 0352
	"AND @@53   ;AND operand with AC, Current page @@53", // op 0353
	"AND @@54   ;AND operand with AC, Current page @@54", // op 0354
	"AND @@55   ;AND operand with AC, Current page @@55", // op 0355
	"AND @@56   ;AND operand with AC, Current page @@56", // op 0356
	"AND @@57   ;AND operand with AC, Current page @@57", // op 0357
	"AND @@60   ;AND operand with AC, Current page @@60", // op 0360
	"AND @@61   ;AND operand with AC, Current page @@61", // op 0361
	"AND @@62   ;AND operand with AC, Current page @@62", // op 0362
	"AND @@63   ;AND operand with AC, Current page @@63", // op 0363
	"AND @@64   ;AND operand with AC, Current page @@64", // op 0364
	"AND @@65   ;AND operand with AC, Current page @@65", // op 0365
	"AND @@66   ;AND operand with AC, Current page @@66", // op 0366
	"AND @@67   ;AND operand with AC, Current page @@67", // op 0367
	"AND @@70   ;AND operand with AC, Current page @@70", // op 0370
	"AND @@71   ;AND operand with AC, Current page @@71", // op 0371
	"AND @@72   ;AND operand with AC, Current page @@72", // op 0372
	"AND @@73   ;AND operand with AC, Current page @@73", // op 0373
	"AND @@74   ;AND operand with AC, Current page @@74", // op 0374
	"AND @@75   ;AND operand with AC, Current page @@75", // op 0375
	"AND @@76   ;AND operand with AC, Current page @@76", // op 0376
	"AND @@77   ;AND operand with AC, Current page @@77", // op 0377
	"AND I 0000 ;AND operand with AC, Indexed ZP 0000", // op 0400
	"AND I 0001 ;AND operand with AC, Indexed ZP 0001", // op 0401
	"AND I 0002 ;AND operand with AC, Indexed ZP 0002", // op 0402
	"AND I 0003 ;AND operand with AC, Indexed ZP 0003", // op 0403
	"AND I 0004 ;AND operand with AC, Indexed ZP 0004", // op 0404
	"AND I 0005 ;AND operand with AC, Indexed ZP 0005", // op 0405
	"AND I 0006 ;AND operand with AC, Indexed ZP 0006", // op 0406
	"AND I 0007 ;AND operand with AC, Indexed ZP 0007", // op 0407
	"AND I 0010 ;AND operand with AC, Indexed ZP 0010 [Auto pre-inc]", // op 0410
	"AND I 0011 ;AND operand with AC, Indexed ZP 0011 [Auto pre-inc]", // op 0411
	"AND I 0012 ;AND operand with AC, Indexed ZP 0012 [Auto pre-inc]", // op 0412
	"AND I 0013 ;AND operand with AC, Indexed ZP 0013 [Auto pre-inc]", // op 0413
	"AND I 0014 ;AND operand with AC, Indexed ZP 0014 [Auto pre-inc]", // op 0414
	"AND I 0015 ;AND operand with AC, Indexed ZP 0015 [Auto pre-inc]", // op 0415
	"AND I 0016 ;AND operand with AC, Indexed ZP 0016 [Auto pre-inc]", // op 0416
	"AND I 0017 ;AND operand with AC, Indexed ZP 0017 [Auto pre-inc]", // op 0417
	"AND I 0020 ;AND operand with AC, Indexed ZP 0020", // op 0420
	"AND I 0021 ;AND operand with AC, Indexed ZP 0021", // op 0421
	"AND I 0022 ;AND operand with AC, Indexed ZP 0022", // op 0422
	"AND I 0023 ;AND operand with AC, Indexed ZP 0023", // op 0423
	"AND I 0024 ;AND operand with AC, Indexed ZP 0024", // op 0424
	"AND I 0025 ;AND operand with AC, Indexed ZP 0025", // op 0425
	"AND I 0026 ;AND operand with AC, Indexed ZP 0026", // op 0426
	"AND I 0027 ;AND operand with AC, Indexed ZP 0027", // op 0427
	"AND I 0030 ;AND operand with AC, Indexed ZP 0030", // op 0430
	"AND I 0031 ;AND operand with AC, Indexed ZP 0031", // op 0431
	"AND I 0032 ;AND operand with AC, Indexed ZP 0032", // op 0432
	"AND I 0033 ;AND operand with AC, Indexed ZP 0033", // op 0433
	"AND I 0034 ;AND operand with AC, Indexed ZP 0034", // op 0434
	"AND I 0035 ;AND operand with AC, Indexed ZP 0035", // op 0435
	"AND I 0036 ;AND operand with AC, Indexed ZP 0036", // op 0436
	"AND I 0037 ;AND operand with AC, Indexed ZP 0037", // op 0437
	"AND I 0040 ;AND operand with AC, Indexed ZP 0040", // op 0440
	"AND I 0041 ;AND operand with AC, Indexed ZP 0041", // op 0441
	"AND I 0042 ;AND operand with AC, Indexed ZP 0042", // op 0442
	"AND I 0043 ;AND operand with AC, Indexed ZP 0043", // op 0443
	"AND I 0044 ;AND operand with AC, Indexed ZP 0044", // op 0444
	"AND I 0045 ;AND operand with AC, Indexed ZP 0045", // op 0445
	"AND I 0046 ;AND operand with AC, Indexed ZP 0046", // op 0446
	"AND I 0047 ;AND operand with AC, Indexed ZP 0047", // op 0447
	"AND I 0050 ;AND operand with AC, Indexed ZP 0050", // op 0450
	"AND I 0051 ;AND operand with AC, Indexed ZP 0051", // op 0451
	"AND I 0052 ;AND operand with AC, Indexed ZP 0052", // op 0452
	"AND I 0053 ;AND operand with AC, Indexed ZP 0053", // op 0453
	"AND I 0054 ;AND operand with AC, Indexed ZP 0054", // op 0454
	"AND I 0055 ;AND operand with AC, Indexed ZP 0055", // op 0455
	"AND I 0056 ;AND operand with AC, Indexed ZP 0056", // op 0456
	"AND I 0057 ;AND operand with AC, Indexed ZP 0057", // op 0457
	"AND I 0060 ;AND operand with AC, Indexed ZP 0060", // op 0460
	"AND I 0061 ;AND operand with AC, Indexed ZP 0061", // op 0461
	"AND I 0062 ;AND operand with AC, Indexed ZP 0062", // op 0462
	"AND I 0063 ;AND operand with AC, Indexed ZP 0063", // op 0463
	"AND I 0064 ;AND operand with AC, Indexed ZP 0064", // op 0464
	"AND I 0065 ;AND operand with AC, Indexed ZP 0065", // op 0465
	"AND I 0066 ;AND operand with AC, Indexed ZP 0066", // op 0466
	"AND I 0067 ;AND operand with AC, Indexed ZP 0067", // op 0467
	"AND I 0070 ;AND operand with AC, Indexed ZP 0070", // op 0470
	"AND I 0071 ;AND operand with AC, Indexed ZP 0071", // op 0471
	"AND I 0072 ;AND operand with AC, Indexed ZP 0072", // op 0472
	"AND I 0073 ;AND operand with AC, Indexed ZP 0073", // op 0473
	"AND I 0074 ;AND operand with AC, Indexed ZP 0074", // op 0474
	"AND I 0075 ;AND operand with AC, Indexed ZP 0075", // op 0475
	"AND I 0076 ;AND operand with AC, Indexed ZP 0076", // op 0476
	"AND I 0077 ;AND operand with AC, Indexed ZP 0077", // op 0477
	"AND I 0100 ;AND operand with AC, Indexed ZP 0100", // op 0500
	"AND I 0101 ;AND operand with AC, Indexed ZP 0101", // op 0501
	"AND I 0102 ;AND operand with AC, Indexed ZP 0102", // op 0502
	"AND I 0103 ;AND operand with AC, Indexed ZP 0103", // op 0503
	"AND I 0104 ;AND operand with AC, Indexed ZP 0104", // op 0504
	"AND I 0105 ;AND operand with AC, Indexed ZP 0105", // op 0505
	"AND I 0106 ;AND operand with AC, Indexed ZP 0106", // op 0506
	"AND I 0107 ;AND operand with AC, Indexed ZP 0107", // op 0507
	"AND I 0110 ;AND operand with AC, Indexed ZP 0110", // op 0510
	"AND I 0111 ;AND operand with AC, Indexed ZP 0111", // op 0511
	"AND I 0112 ;AND operand with AC, Indexed ZP 0112", // op 0512
	"AND I 0113 ;AND operand with AC, Indexed ZP 0113", // op 0513
	"AND I 0114 ;AND operand with AC, Indexed ZP 0114", // op 0514
	"AND I 0115 ;AND operand with AC, Indexed ZP 0115", // op 0515
	"AND I 0116 ;AND operand with AC, Indexed ZP 0116", // op 0516
	"AND I 0117 ;AND operand with AC, Indexed ZP 0117", // op 0517
	"AND I 0120 ;AND operand with AC, Indexed ZP 0120", // op 0520
	"AND I 0121 ;AND operand with AC, Indexed ZP 0121", // op 0521
	"AND I 0122 ;AND operand with AC, Indexed ZP 0122", // op 0522
	"AND I 0123 ;AND operand with AC, Indexed ZP 0123", // op 0523
	"AND I 0124 ;AND operand with AC, Indexed ZP 0124", // op 0524
	"AND I 0125 ;AND operand with AC, Indexed ZP 0125", // op 0525
	"AND I 0126 ;AND operand with AC, Indexed ZP 0126", // op 0526
	"AND I 0127 ;AND operand with AC, Indexed ZP 0127", // op 0527
	"AND I 0130 ;AND operand with AC, Indexed ZP 0130", // op 0530
	"AND I 0131 ;AND operand with AC, Indexed ZP 0131", // op 0531
	"AND I 0132 ;AND operand with AC, Indexed ZP 0132", // op 0532
	"AND I 0133 ;AND operand with AC, Indexed ZP 0133", // op 0533
	"AND I 0134 ;AND operand with AC, Indexed ZP 0134", // op 0534
	"AND I 0135 ;AND operand with AC, Indexed ZP 0135", // op 0535
	"AND I 0136 ;AND operand with AC, Indexed ZP 0136", // op 0536
	"AND I 0137 ;AND operand with AC, Indexed ZP 0137", // op 0537
	"AND I 0140 ;AND operand with AC, Indexed ZP 0140", // op 0540
	"AND I 0141 ;AND operand with AC, Indexed ZP 0141", // op 0541
	"AND I 0142 ;AND operand with AC, Indexed ZP 0142", // op 0542
	"AND I 0143 ;AND operand with AC, Indexed ZP 0143", // op 0543
	"AND I 0144 ;AND operand with AC, Indexed ZP 0144", // op 0544
	"AND I 0145 ;AND operand with AC, Indexed ZP 0145", // op 0545
	"AND I 0146 ;AND operand with AC, Indexed ZP 0146", // op 0546
	"AND I 0147 ;AND operand with AC, Indexed ZP 0147", // op 0547
	"AND I 0150 ;AND operand with AC, Indexed ZP 0150", // op 0550
	"AND I 0151 ;AND operand with AC, Indexed ZP 0151", // op 0551
	"AND I 0152 ;AND operand with AC, Indexed ZP 0152", // op 0552
	"AND I 0153 ;AND operand with AC, Indexed ZP 0153", // op 0553
	"AND I 0154 ;AND operand with AC, Indexed ZP 0154", // op 0554
	"AND I 0155 ;AND operand with AC, Indexed ZP 0155", // op 0555
	"AND I 0156 ;AND operand with AC, Indexed ZP 0156", // op 0556
	"AND I 0157 ;AND operand with AC, Indexed ZP 0157", // op 0557
	"AND I 0160 ;AND operand with AC, Indexed ZP 0160", // op 0560
	"AND I 0161 ;AND operand with AC, Indexed ZP 0161", // op 0561
	"AND I 0162 ;AND operand with AC, Indexed ZP 0162", // op 0562
	"AND I 0163 ;AND operand with AC, Indexed ZP 0163", // op 0563
	"AND I 0164 ;AND operand with AC, Indexed ZP 0164", // op 0564
	"AND I 0165 ;AND operand with AC, Indexed ZP 0165", // op 0565
	"AND I 0166 ;AND operand with AC, Indexed ZP 0166", // op 0566
	"AND I 0167 ;AND operand with AC, Indexed ZP 0167", // op 0567
	"AND I 0170 ;AND operand with AC, Indexed ZP 0170", // op 0570
	"AND I 0171 ;AND operand with AC, Indexed ZP 0171", // op 0571
	"AND I 0172 ;AND operand with AC, Indexed ZP 0172", // op 0572
	"AND I 0173 ;AND operand with AC, Indexed ZP 0173", // op 0573
	"AND I 0174 ;AND operand with AC, Indexed ZP 0174", // op 0574
	"AND I 0175 ;AND operand with AC, Indexed ZP 0175", // op 0575
	"AND I 0176 ;AND operand with AC, Indexed ZP 0176", // op 0576
	"AND I 0177 ;AND operand with AC, Indexed ZP 0177", // op 0577
	"AND I @@00 ;AND operand with AC, Indexed Current page @@00", // op 0600
	"AND I @@01 ;AND operand with AC, Indexed Current page @@01", // op 0601
	"AND I @@02 ;AND operand with AC, Indexed Current page @@02", // op 0602
	"AND I @@03 ;AND operand with AC, Indexed Current page @@03", // op 0603
	"AND I @@04 ;AND operand with AC, Indexed Current page @@04", // op 0604
	"AND I @@05 ;AND operand with AC, Indexed Current page @@05", // op 0605
	"AND I @@06 ;AND operand with AC, Indexed Current page @@06", // op 0606
	"AND I @@07 ;AND operand with AC, Indexed Current page @@07", // op 0607
	"AND I @@10 ;AND operand with AC, Indexed Current page @@10", // op 0610
	"AND I @@11 ;AND operand with AC, Indexed Current page @@11", // op 0611
	"AND I @@12 ;AND operand with AC, Indexed Current page @@12", // op 0612
	"AND I @@13 ;AND operand with AC, Indexed Current page @@13", // op 0613
	"AND I @@14 ;AND operand with AC, Indexed Current page @@14", // op 0614
	"AND I @@15 ;AND operand with AC, Indexed Current page @@15", // op 0615
	"AND I @@16 ;AND operand with AC, Indexed Current page @@16", // op 0616
	"AND I @@17 ;AND operand with AC, Indexed Current page @@17", // op 0617
	"AND I @@20 ;AND operand with AC, Indexed Current page @@20", // op 0620
	"AND I @@21 ;AND operand with AC, Indexed Current page @@21", // op 0621
	"AND I @@22 ;AND operand with AC, Indexed Current page @@22", // op 0622
	"AND I @@23 ;AND operand with AC, Indexed Current page @@23", // op 0623
	"AND I @@24 ;AND operand with AC, Indexed Current page @@24", // op 0624
	"AND I @@25 ;AND operand with AC, Indexed Current page @@25", // op 0625
	"AND I @@26 ;AND operand with AC, Indexed Current page @@26", // op 0626
	"AND I @@27 ;AND operand with AC, Indexed Current page @@27", // op 0627
	"AND I @@30 ;AND operand with AC, Indexed Current page @@30", // op 0630
	"AND I @@31 ;AND operand with AC, Indexed Current page @@31", // op 0631
	"AND I @@32 ;AND operand with AC, Indexed Current page @@32", // op 0632
	"AND I @@33 ;AND operand with AC, Indexed Current page @@33", // op 0633
	"AND I @@34 ;AND operand with AC, Indexed Current page @@34", // op 0634
	"AND I @@35 ;AND operand with AC, Indexed Current page @@35", // op 0635
	"AND I @@36 ;AND operand with AC, Indexed Current page @@36", // op 0636
	"AND I @@37 ;AND operand with AC, Indexed Current page @@37", // op 0637
	"AND I @@40 ;AND operand with AC, Indexed Current page @@40", // op 0640
	"AND I @@41 ;AND operand with AC, Indexed Current page @@41", // op 0641
	"AND I @@42 ;AND operand with AC, Indexed Current page @@42", // op 0642
	"AND I @@43 ;AND operand with AC, Indexed Current page @@43", // op 0643
	"AND I @@44 ;AND operand with AC, Indexed Current page @@44", // op 0644
	"AND I @@45 ;AND operand with AC, Indexed Current page @@45", // op 0645
	"AND I @@46 ;AND operand with AC, Indexed Current page @@46", // op 0646
	"AND I @@47 ;AND operand with AC, Indexed Current page @@47", // op 0647
	"AND I @@50 ;AND operand with AC, Indexed Current page @@50", // op 0650
	"AND I @@51 ;AND operand with AC, Indexed Current page @@51", // op 0651
	"AND I @@52 ;AND operand with AC, Indexed Current page @@52", // op 0652
	"AND I @@53 ;AND operand with AC, Indexed Current page @@53", // op 0653
	"AND I @@54 ;AND operand with AC, Indexed Current page @@54", // op 0654
	"AND I @@55 ;AND operand with AC, Indexed Current page @@55", // op 0655
	"AND I @@56 ;AND operand with AC, Indexed Current page @@56", // op 0656
	"AND I @@57 ;AND operand with AC, Indexed Current page @@57", // op 0657
	"AND I @@60 ;AND operand with AC, Indexed Current page @@60", // op 0660
	"AND I @@61 ;AND operand with AC, Indexed Current page @@61", // op 0661
	"AND I @@62 ;AND operand with AC, Indexed Current page @@62", // op 0662
	"AND I @@63 ;AND operand with AC, Indexed Current page @@63", // op 0663
	"AND I @@64 ;AND operand with AC, Indexed Current page @@64", // op 0664
	"AND I @@65 ;AND operand with AC, Indexed Current page @@65", // op 0665
	"AND I @@66 ;AND operand with AC, Indexed Current page @@66", // op 0666
	"AND I @@67 ;AND operand with AC, Indexed Current page @@67", // op 0667
	"AND I @@70 ;AND operand with AC, Indexed Current page @@70", // op 0670
	"AND I @@71 ;AND operand with AC, Indexed Current page @@71", // op 0671
	"AND I @@72 ;AND operand with AC, Indexed Current page @@72", // op 0672
	"AND I @@73 ;AND operand with AC, Indexed Current page @@73", // op 0673
	"AND I @@74 ;AND operand with AC, Indexed Current page @@74", // op 0674
	"AND I @@75 ;AND operand with AC, Indexed Current page @@75", // op 0675
	"AND I @@76 ;AND operand with AC, Indexed Current page @@76", // op 0676
	"AND I @@77 ;AND operand with AC, Indexed Current page @@77", // op 0677
	"AND I @@00 ;AND operand with AC, Indexed Current page @@00", // op 0700
	"AND I @@01 ;AND operand with AC, Indexed Current page @@01", // op 0701
	"AND I @@02 ;AND operand with AC, Indexed Current page @@02", // op 0702
	"AND I @@03 ;AND operand with AC, Indexed Current page @@03", // op 0703
	"AND I @@04 ;AND operand with AC, Indexed Current page @@04", // op 0704
	"AND I @@05 ;AND operand with AC, Indexed Current page @@05", // op 0705
	"AND I @@06 ;AND operand with AC, Indexed Current page @@06", // op 0706
	"AND I @@07 ;AND operand with AC, Indexed Current page @@07", // op 0707
	"AND I @@10 ;AND operand with AC, Indexed Current page @@10", // op 0710
	"AND I @@11 ;AND operand with AC, Indexed Current page @@11", // op 0711
	"AND I @@12 ;AND operand with AC, Indexed Current page @@12", // op 0712
	"AND I @@13 ;AND operand with AC, Indexed Current page @@13", // op 0713
	"AND I @@14 ;AND operand with AC, Indexed Current page @@14", // op 0714
	"AND I @@15 ;AND operand with AC, Indexed Current page @@15", // op 0715
	"AND I @@16 ;AND operand with AC, Indexed Current page @@16", // op 0716
	"AND I @@17 ;AND operand with AC, Indexed Current page @@17", // op 0717
	"AND I @@20 ;AND operand with AC, Indexed Current page @@20", // op 0720
	"AND I @@21 ;AND operand with AC, Indexed Current page @@21", // op 0721
	"AND I @@22 ;AND operand with AC, Indexed Current page @@22", // op 0722
	"AND I @@23 ;AND operand with AC, Indexed Current page @@23", // op 0723
	"AND I @@24 ;AND operand with AC, Indexed Current page @@24", // op 0724
	"AND I @@25 ;AND operand with AC, Indexed Current page @@25", // op 0725
	"AND I @@26 ;AND operand with AC, Indexed Current page @@26", // op 0726
	"AND I @@27 ;AND operand with AC, Indexed Current page @@27", // op 0727
	"AND I @@30 ;AND operand with AC, Indexed Current page @@30", // op 0730
	"AND I @@31 ;AND operand with AC, Indexed Current page @@31", // op 0731
	"AND I @@32 ;AND operand with AC, Indexed Current page @@32", // op 0732
	"AND I @@33 ;AND operand with AC, Indexed Current page @@33", // op 0733
	"AND I @@34 ;AND operand with AC, Indexed Current page @@34", // op 0734
	"AND I @@35 ;AND operand with AC, Indexed Current page @@35", // op 0735
	"AND I @@36 ;AND operand with AC, Indexed Current page @@36", // op 0736
	"AND I @@37 ;AND operand with AC, Indexed Current page @@37", // op 0737
	"AND I @@40 ;AND operand with AC, Indexed Current page @@40", // op 0740
	"AND I @@41 ;AND operand with AC, Indexed Current page @@41", // op 0741
	"AND I @@42 ;AND operand with AC, Indexed Current page @@42", // op 0742
	"AND I @@43 ;AND operand with AC, Indexed Current page @@43", // op 0743
	"AND I @@44 ;AND operand with AC, Indexed Current page @@44", // op 0744
	"AND I @@45 ;AND operand with AC, Indexed Current page @@45", // op 0745
	"AND I @@46 ;AND operand with AC, Indexed Current page @@46", // op 0746
	"AND I @@47 ;AND operand with AC, Indexed Current page @@47", // op 0747
	"AND I @@50 ;AND operand with AC, Indexed Current page @@50", // op 0750
	"AND I @@51 ;AND operand with AC, Indexed Current page @@51", // op 0751
	"AND I @@52 ;AND operand with AC, Indexed Current page @@52", // op 0752
	"AND I @@53 ;AND operand with AC, Indexed Current page @@53", // op 0753
	"AND I @@54 ;AND operand with AC, Indexed Current page @@54", // op 0754
	"AND I @@55 ;AND operand with AC, Indexed Current page @@55", // op 0755
	"AND I @@56 ;AND operand with AC, Indexed Current page @@56", // op 0756
	"AND I @@57 ;AND operand with AC, Indexed Current page @@57", // op 0757
	"AND I @@60 ;AND operand with AC, Indexed Current page @@60", // op 0760
	"AND I @@61 ;AND operand with AC, Indexed Current page @@61", // op 0761
	"AND I @@62 ;AND operand with AC, Indexed Current page @@62", // op 0762
	"AND I @@63 ;AND operand with AC, Indexed Current page @@63", // op 0763
	"AND I @@64 ;AND operand with AC, Indexed Current page @@64", // op 0764
	"AND I @@65 ;AND operand with AC, Indexed Current page @@65", // op 0765
	"AND I @@66 ;AND operand with AC, Indexed Current page @@66", // op 0766
	"AND I @@67 ;AND operand with AC, Indexed Current page @@67", // op 0767
	"AND I @@70 ;AND operand with AC, Indexed Current page @@70", // op 0770
	"AND I @@71 ;AND operand with AC, Indexed Current page @@71", // op 0771
	"AND I @@72 ;AND operand with AC, Indexed Current page @@72", // op 0772
	"AND I @@73 ;AND operand with AC, Indexed Current page @@73", // op 0773
	"AND I @@74 ;AND operand with AC, Indexed Current page @@74", // op 0774
	"AND I @@75 ;AND operand with AC, Indexed Current page @@75", // op 0775
	"AND I @@76 ;AND operand with AC, Indexed Current page @@76", // op 0776
	"AND I @@77 ;AND operand with AC, Indexed Current page @@77", // op 0777
	"TAD 0000   ;Add operand to AC, ZP 0000 ", // op 1000
	"TAD 0001   ;Add operand to AC, ZP 0001", // op 1001
	"TAD 0002   ;Add operand to AC, ZP 0002", // op 1002
	"TAD 0003   ;Add operand to AC, ZP 0003", // op 1003
	"TAD 0004   ;Add operand to AC, ZP 0004", // op 1004
	"TAD 0005   ;Add operand to AC, ZP 0005", // op 1005
	"TAD 0006   ;Add operand to AC, ZP 0006", // op 1006
	"TAD 0007   ;Add operand to AC, ZP 0007", // op 1007
	"TAD 0010   ;Add operand to AC, ZP 0010", // op 1010
	"TAD 0011   ;Add operand to AC, ZP 0011", // op 1011
	"TAD 0012   ;Add operand to AC, ZP 0012", // op 1012
	"TAD 0013   ;Add operand to AC, ZP 0013", // op 1013
	"TAD 0014   ;Add operand to AC, ZP 0014", // op 1014
	"TAD 0015   ;Add operand to AC, ZP 0015", // op 1015
	"TAD 0016   ;Add operand to AC, ZP 0016", // op 1016
	"TAD 0017   ;Add operand to AC, ZP 0017", // op 1017
	"TAD 0020   ;Add operand to AC, ZP 0020", // op 1020
	"TAD 0021   ;Add operand to AC, ZP 0021", // op 1021
	"TAD 0022   ;Add operand to AC, ZP 0022", // op 1022
	"TAD 0023   ;Add operand to AC, ZP 0023", // op 1023
	"TAD 0024   ;Add operand to AC, ZP 0024", // op 1024
	"TAD 0025   ;Add operand to AC, ZP 0025", // op 1025
	"TAD 0026   ;Add operand to AC, ZP 0026", // op 1026
	"TAD 0027   ;Add operand to AC, ZP 0027", // op 1027
	"TAD 0030   ;Add operand to AC, ZP 0030", // op 1030
	"TAD 0031   ;Add operand to AC, ZP 0031", // op 1031
	"TAD 0032   ;Add operand to AC, ZP 0032", // op 1032
	"TAD 0033   ;Add operand to AC, ZP 0033", // op 1033
	"TAD 0034   ;Add operand to AC, ZP 0034", // op 1034
	"TAD 0035   ;Add operand to AC, ZP 0035", // op 1035
	"TAD 0036   ;Add operand to AC, ZP 0036", // op 1036
	"TAD 0037   ;Add operand to AC, ZP 0037", // op 1037
	"TAD 0040   ;Add operand to AC, ZP 0040", // op 1040
	"TAD 0041   ;Add operand to AC, ZP 0041", // op 1041
	"TAD 0042   ;Add operand to AC, ZP 0042", // op 1042
	"TAD 0043   ;Add operand to AC, ZP 0043", // op 1043
	"TAD 0044   ;Add operand to AC, ZP 0044", // op 1044
	"TAD 0045   ;Add operand to AC, ZP 0045", // op 1045
	"TAD 0046   ;Add operand to AC, ZP 0046", // op 1046
	"TAD 0047   ;Add operand to AC, ZP 0047", // op 1047
	"TAD 0050   ;Add operand to AC, ZP 0050", // op 1050
	"TAD 0051   ;Add operand to AC, ZP 0051", // op 1051
	"TAD 0052   ;Add operand to AC, ZP 0052", // op 1052
	"TAD 0053   ;Add operand to AC, ZP 0053", // op 1053
	"TAD 0054   ;Add operand to AC, ZP 0054", // op 1054
	"TAD 0055   ;Add operand to AC, ZP 0055", // op 1055
	"TAD 0056   ;Add operand to AC, ZP 0056", // op 1056
	"TAD 0057   ;Add operand to AC, ZP 0057", // op 1057
	"TAD 0060   ;Add operand to AC, ZP 0060", // op 1060
	"TAD 0061   ;Add operand to AC, ZP 0061", // op 1061
	"TAD 0062   ;Add operand to AC, ZP 0062", // op 1062
	"TAD 0063   ;Add operand to AC, ZP 0063", // op 1063
	"TAD 0064   ;Add operand to AC, ZP 0064", // op 1064
	"TAD 0065   ;Add operand to AC, ZP 0065", // op 1065
	"TAD 0066   ;Add operand to AC, ZP 0066", // op 1066
	"TAD 0067   ;Add operand to AC, ZP 0067", // op 1067
	"TAD 0070   ;Add operand to AC, ZP 0070", // op 1070
	"TAD 0071   ;Add operand to AC, ZP 0071", // op 1071
	"TAD 0072   ;Add operand to AC, ZP 0072", // op 1072
	"TAD 0073   ;Add operand to AC, ZP 0073", // op 1073
	"TAD 0074   ;Add operand to AC, ZP 0074", // op 1074
	"TAD 0075   ;Add operand to AC, ZP 0075", // op 1075
	"TAD 0076   ;Add operand to AC, ZP 0076", // op 1076
	"TAD 0077   ;Add operand to AC, ZP 0077", // op 1077
	"TAD 0100   ;Add operand to AC, ZP 0100", // op 1100
	"TAD 0101   ;Add operand to AC, ZP 0101", // op 1101
	"TAD 0102   ;Add operand to AC, ZP 0102", // op 1102
	"TAD 0103   ;Add operand to AC, ZP 0103", // op 1103
	"TAD 0104   ;Add operand to AC, ZP 0104", // op 1104
	"TAD 0105   ;Add operand to AC, ZP 0105", // op 1105
	"TAD 0106   ;Add operand to AC, ZP 0106", // op 1106
	"TAD 0107   ;Add operand to AC, ZP 0107", // op 1107
	"TAD 0110   ;Add operand to AC, ZP 0110", // op 1110
	"TAD 0111   ;Add operand to AC, ZP 0111", // op 1111
	"TAD 0112   ;Add operand to AC, ZP 0112", // op 1112
	"TAD 0113   ;Add operand to AC, ZP 0113", // op 1113
	"TAD 0114   ;Add operand to AC, ZP 0114", // op 1114
	"TAD 0115   ;Add operand to AC, ZP 0115", // op 1115
	"TAD 0116   ;Add operand to AC, ZP 0116", // op 1116
	"TAD 0117   ;Add operand to AC, ZP 0117", // op 1117
	"TAD 0120   ;Add operand to AC, ZP 0120", // op 1120
	"TAD 0121   ;Add operand to AC, ZP 0121", // op 1121
	"TAD 0122   ;Add operand to AC, ZP 0122", // op 1122
	"TAD 0123   ;Add operand to AC, ZP 0123", // op 1123
	"TAD 0124   ;Add operand to AC, ZP 0124", // op 1124
	"TAD 0125   ;Add operand to AC, ZP 0125", // op 1125
	"TAD 0126   ;Add operand to AC, ZP 0126", // op 1126
	"TAD 0127   ;Add operand to AC, ZP 0127", // op 1127
	"TAD 0130   ;Add operand to AC, ZP 0130", // op 1130
	"TAD 0131   ;Add operand to AC, ZP 0131", // op 1131
	"TAD 0132   ;Add operand to AC, ZP 0132", // op 1132
	"TAD 0133   ;Add operand to AC, ZP 0133", // op 1133
	"TAD 0134   ;Add operand to AC, ZP 0134", // op 1134
	"TAD 0135   ;Add operand to AC, ZP 0135", // op 1135
	"TAD 0136   ;Add operand to AC, ZP 0136", // op 1136
	"TAD 0137   ;Add operand to AC, ZP 0137", // op 1137
	"TAD 0140   ;Add operand to AC, ZP 0140", // op 1140
	"TAD 0141   ;Add operand to AC, ZP 0141", // op 1141
	"TAD 0142   ;Add operand to AC, ZP 0142", // op 1142
	"TAD 0143   ;Add operand to AC, ZP 0143", // op 1143
	"TAD 0144   ;Add operand to AC, ZP 0144", // op 1144
	"TAD 0145   ;Add operand to AC, ZP 0145", // op 1145
	"TAD 0146   ;Add operand to AC, ZP 0146", // op 1146
	"TAD 0147   ;Add operand to AC, ZP 0147", // op 1147
	"TAD 0150   ;Add operand to AC, ZP 0150", // op 1150
	"TAD 0151   ;Add operand to AC, ZP 0151", // op 1151
	"TAD 0152   ;Add operand to AC, ZP 0152", // op 1152
	"TAD 0153   ;Add operand to AC, ZP 0153", // op 1153
	"TAD 0154   ;Add operand to AC, ZP 0154", // op 1154
	"TAD 0155   ;Add operand to AC, ZP 0155", // op 1155
	"TAD 0156   ;Add operand to AC, ZP 0156", // op 1156
	"TAD 0157   ;Add operand to AC, ZP 0157", // op 1157
	"TAD 0160   ;Add operand to AC, ZP 0160", // op 1160
	"TAD 0161   ;Add operand to AC, ZP 0161", // op 1161
	"TAD 0162   ;Add operand to AC, ZP 0162", // op 1162
	"TAD 0163   ;Add operand to AC, ZP 0163", // op 1163
	"TAD 0164   ;Add operand to AC, ZP 0164", // op 1164
	"TAD 0165   ;Add operand to AC, ZP 0165", // op 1165
	"TAD 0166   ;Add operand to AC, ZP 0166", // op 1166
	"TAD 0167   ;Add operand to AC, ZP 0167", // op 1167
	"TAD 0170   ;Add operand to AC, ZP 0170", // op 1170
	"TAD 0171   ;Add operand to AC, ZP 0171", // op 1171
	"TAD 0172   ;Add operand to AC, ZP 0172", // op 1172
	"TAD 0173   ;Add operand to AC, ZP 0173", // op 1173
	"TAD 0174   ;Add operand to AC, ZP 0174", // op 1174
	"TAD 0175   ;Add operand to AC, ZP 0175", // op 1175
	"TAD 0176   ;Add operand to AC, ZP 0176", // op 1176
	"TAD 0177   ;Add operand to AC, ZP 0177", // op 1177
	"TAD @@00   ;Add operand to AC, Current page @@00", // op 1200
	"TAD @@01   ;Add operand to AC, Current page @@01", // op 1201
	"TAD @@02   ;Add operand to AC, Current page @@02", // op 1202
	"TAD @@03   ;Add operand to AC, Current page @@03", // op 1203
	"TAD @@04   ;Add operand to AC, Current page @@04", // op 1204
	"TAD @@05   ;Add operand to AC, Current page @@05", // op 1205
	"TAD @@06   ;Add operand to AC, Current page @@06", // op 1206
	"TAD @@07   ;Add operand to AC, Current page @@07", // op 1207
	"TAD @@10   ;Add operand to AC, Current page @@10", // op 1210
	"TAD @@11   ;Add operand to AC, Current page @@11", // op 1211
	"TAD @@12   ;Add operand to AC, Current page @@12", // op 1212
	"TAD @@13   ;Add operand to AC, Current page @@13", // op 1213
	"TAD @@14   ;Add operand to AC, Current page @@14", // op 1214
	"TAD @@15   ;Add operand to AC, Current page @@15", // op 1215
	"TAD @@16   ;Add operand to AC, Current page @@16", // op 1216
	"TAD @@17   ;Add operand to AC, Current page @@17", // op 1217
	"TAD @@20   ;Add operand to AC, Current page @@20", // op 1220
	"TAD @@21   ;Add operand to AC, Current page @@21", // op 1221
	"TAD @@22   ;Add operand to AC, Current page @@22", // op 1222
	"TAD @@23   ;Add operand to AC, Current page @@23", // op 1223
	"TAD @@24   ;Add operand to AC, Current page @@24", // op 1224
	"TAD @@25   ;Add operand to AC, Current page @@25", // op 1225
	"TAD @@26   ;Add operand to AC, Current page @@26", // op 1226
	"TAD @@27   ;Add operand to AC, Current page @@27", // op 1227
	"TAD @@30   ;Add operand to AC, Current page @@30", // op 1230
	"TAD @@31   ;Add operand to AC, Current page @@31", // op 1231
	"TAD @@32   ;Add operand to AC, Current page @@32", // op 1232
	"TAD @@33   ;Add operand to AC, Current page @@33", // op 1233
	"TAD @@34   ;Add operand to AC, Current page @@34", // op 1234
	"TAD @@35   ;Add operand to AC, Current page @@35", // op 1235
	"TAD @@36   ;Add operand to AC, Current page @@36", // op 1236
	"TAD @@37   ;Add operand to AC, Current page @@37", // op 1237
	"TAD @@40   ;Add operand to AC, Current page @@40", // op 1240
	"TAD @@41   ;Add operand to AC, Current page @@41", // op 1241
	"TAD @@42   ;Add operand to AC, Current page @@42", // op 1242
	"TAD @@43   ;Add operand to AC, Current page @@43", // op 1243
	"TAD @@44   ;Add operand to AC, Current page @@44", // op 1244
	"TAD @@45   ;Add operand to AC, Current page @@45", // op 1245
	"TAD @@46   ;Add operand to AC, Current page @@46", // op 1246
	"TAD @@47   ;Add operand to AC, Current page @@47", // op 1247
	"TAD @@50   ;Add operand to AC, Current page @@50", // op 1250
	"TAD @@51   ;Add operand to AC, Current page @@51", // op 1251
	"TAD @@52   ;Add operand to AC, Current page @@52", // op 1252
	"TAD @@53   ;Add operand to AC, Current page @@53", // op 1253
	"TAD @@54   ;Add operand to AC, Current page @@54", // op 1254
	"TAD @@55   ;Add operand to AC, Current page @@55", // op 1255
	"TAD @@56   ;Add operand to AC, Current page @@56", // op 1256
	"TAD @@57   ;Add operand to AC, Current page @@57", // op 1257
	"TAD @@60   ;Add operand to AC, Current page @@60", // op 1260
	"TAD @@61   ;Add operand to AC, Current page @@61", // op 1261
	"TAD @@62   ;Add operand to AC, Current page @@62", // op 1262
	"TAD @@63   ;Add operand to AC, Current page @@63", // op 1263
	"TAD @@64   ;Add operand to AC, Current page @@64", // op 1264
	"TAD @@65   ;Add operand to AC, Current page @@65", // op 1265
	"TAD @@66   ;Add operand to AC, Current page @@66", // op 1266
	"TAD @@67   ;Add operand to AC, Current page @@67", // op 1267
	"TAD @@70   ;Add operand to AC, Current page @@70", // op 1270
	"TAD @@71   ;Add operand to AC, Current page @@71", // op 1271
	"TAD @@72   ;Add operand to AC, Current page @@72", // op 1272
	"TAD @@73   ;Add operand to AC, Current page @@73", // op 1273
	"TAD @@74   ;Add operand to AC, Current page @@74", // op 1274
	"TAD @@75   ;Add operand to AC, Current page @@75", // op 1275
	"TAD @@76   ;Add operand to AC, Current page @@76", // op 1276
	"TAD @@77   ;Add operand to AC, Current page @@77", // op 1277
	"TAD @@00   ;Add operand to AC, Current page @@00", // op 1300
	"TAD @@01   ;Add operand to AC, Current page @@01", // op 1301
	"TAD @@02   ;Add operand to AC, Current page @@02", // op 1302
	"TAD @@03   ;Add operand to AC, Current page @@03", // op 1303
	"TAD @@04   ;Add operand to AC, Current page @@04", // op 1304
	"TAD @@05   ;Add operand to AC, Current page @@05", // op 1305
	"TAD @@06   ;Add operand to AC, Current page @@06", // op 1306
	"TAD @@07   ;Add operand to AC, Current page @@07", // op 1307
	"TAD @@10   ;Add operand to AC, Current page @@10", // op 1310
	"TAD @@11   ;Add operand to AC, Current page @@11", // op 1311
	"TAD @@12   ;Add operand to AC, Current page @@12", // op 1312
	"TAD @@13   ;Add operand to AC, Current page @@13", // op 1313
	"TAD @@14   ;Add operand to AC, Current page @@14", // op 1314
	"TAD @@15   ;Add operand to AC, Current page @@15", // op 1315
	"TAD @@16   ;Add operand to AC, Current page @@16", // op 1316
	"TAD @@17   ;Add operand to AC, Current page @@17", // op 1317
	"TAD @@20   ;Add operand to AC, Current page @@20", // op 1320
	"TAD @@21   ;Add operand to AC, Current page @@21", // op 1321
	"TAD @@22   ;Add operand to AC, Current page @@22", // op 1322
	"TAD @@23   ;Add operand to AC, Current page @@23", // op 1323
	"TAD @@24   ;Add operand to AC, Current page @@24", // op 1324
	"TAD @@25   ;Add operand to AC, Current page @@25", // op 1325
	"TAD @@26   ;Add operand to AC, Current page @@26", // op 1326
	"TAD @@27   ;Add operand to AC, Current page @@27", // op 1327
	"TAD @@30   ;Add operand to AC, Current page @@30", // op 1330
	"TAD @@31   ;Add operand to AC, Current page @@31", // op 1331
	"TAD @@32   ;Add operand to AC, Current page @@32", // op 1332
	"TAD @@33   ;Add operand to AC, Current page @@33", // op 1333
	"TAD @@34   ;Add operand to AC, Current page @@34", // op 1334
	"TAD @@35   ;Add operand to AC, Current page @@35", // op 1335
	"TAD @@36   ;Add operand to AC, Current page @@36", // op 1336
	"TAD @@37   ;Add operand to AC, Current page @@37", // op 1337
	"TAD @@40   ;Add operand to AC, Current page @@40", // op 1340
	"TAD @@41   ;Add operand to AC, Current page @@41", // op 1341
	"TAD @@42   ;Add operand to AC, Current page @@42", // op 1342
	"TAD @@43   ;Add operand to AC, Current page @@43", // op 1343
	"TAD @@44   ;Add operand to AC, Current page @@44", // op 1344
	"TAD @@45   ;Add operand to AC, Current page @@45", // op 1345
	"TAD @@46   ;Add operand to AC, Current page @@46", // op 1346
	"TAD @@47   ;Add operand to AC, Current page @@47", // op 1347
	"TAD @@50   ;Add operand to AC, Current page @@50", // op 1350
	"TAD @@51   ;Add operand to AC, Current page @@51", // op 1351
	"TAD @@52   ;Add operand to AC, Current page @@52", // op 1352
	"TAD @@53   ;Add operand to AC, Current page @@53", // op 1353
	"TAD @@54   ;Add operand to AC, Current page @@54", // op 1354
	"TAD @@55   ;Add operand to AC, Current page @@55", // op 1355
	"TAD @@56   ;Add operand to AC, Current page @@56", // op 1356
	"TAD @@57   ;Add operand to AC, Current page @@57", // op 1357
	"TAD @@60   ;Add operand to AC, Current page @@60", // op 1360
	"TAD @@61   ;Add operand to AC, Current page @@61", // op 1361
	"TAD @@62   ;Add operand to AC, Current page @@62", // op 1362
	"TAD @@63   ;Add operand to AC, Current page @@63", // op 1363
	"TAD @@64   ;Add operand to AC, Current page @@64", // op 1364
	"TAD @@65   ;Add operand to AC, Current page @@65", // op 1365
	"TAD @@66   ;Add operand to AC, Current page @@66", // op 1366
	"TAD @@67   ;Add operand to AC, Current page @@67", // op 1367
	"TAD @@70   ;Add operand to AC, Current page @@70", // op 1370
	"TAD @@71   ;Add operand to AC, Current page @@71", // op 1371
	"TAD @@72   ;Add operand to AC, Current page @@72", // op 1372
	"TAD @@73   ;Add operand to AC, Current page @@73", // op 1373
	"TAD @@74   ;Add operand to AC, Current page @@74", // op 1374
	"TAD @@75   ;Add operand to AC, Current page @@75", // op 1375
	"TAD @@76   ;Add operand to AC, Current page @@76", // op 1376
	"TAD @@77   ;Add operand to AC, Current page @@77", // op 1377
	"TAD I 0000 ;Add operand to AC, Indexed ZP 0000", // op 1400
	"TAD I 0001 ;Add operand to AC, Indexed ZP 0001", // op 1401
	"TAD I 0002 ;Add operand to AC, Indexed ZP 0002", // op 1402
	"TAD I 0003 ;Add operand to AC, Indexed ZP 0003", // op 1403
	"TAD I 0004 ;Add operand to AC, Indexed ZP 0004", // op 1404
	"TAD I 0005 ;Add operand to AC, Indexed ZP 0005", // op 1405
	"TAD I 0006 ;Add operand to AC, Indexed ZP 0006", // op 1406
	"TAD I 0007 ;Add operand to AC, Indexed ZP 0007", // op 1407
	"TAD I 0010 ;Add operand to AC, Indexed ZP 0010 [Auto pre-inc]", // op 1410
	"TAD I 0011 ;Add operand to AC, Indexed ZP 0011 [Auto pre-inc]", // op 1411
	"TAD I 0012 ;Add operand to AC, Indexed ZP 0012 [Auto pre-inc]", // op 1412
	"TAD I 0013 ;Add operand to AC, Indexed ZP 0013 [Auto pre-inc]", // op 1413
	"TAD I 0014 ;Add operand to AC, Indexed ZP 0014 [Auto pre-inc]", // op 1414
	"TAD I 0015 ;Add operand to AC, Indexed ZP 0015 [Auto pre-inc]", // op 1415
	"TAD I 0016 ;Add operand to AC, Indexed ZP 0016 [Auto pre-inc]", // op 1416
	"TAD I 0017 ;Add operand to AC, Indexed ZP 0017 [Auto pre-inc]", // op 1417
	"TAD I 0020 ;Add operand to AC, Indexed ZP 0020", // op 1420
	"TAD I 0021 ;Add operand to AC, Indexed ZP 0021", // op 1421
	"TAD I 0022 ;Add operand to AC, Indexed ZP 0022", // op 1422
	"TAD I 0023 ;Add operand to AC, Indexed ZP 0023", // op 1423
	"TAD I 0024 ;Add operand to AC, Indexed ZP 0024", // op 1424
	"TAD I 0025 ;Add operand to AC, Indexed ZP 0025", // op 1425
	"TAD I 0026 ;Add operand to AC, Indexed ZP 0026", // op 1426
	"TAD I 0027 ;Add operand to AC, Indexed ZP 0027", // op 1427
	"TAD I 0030 ;Add operand to AC, Indexed ZP 0030", // op 1430
	"TAD I 0031 ;Add operand to AC, Indexed ZP 0031", // op 1431
	"TAD I 0032 ;Add operand to AC, Indexed ZP 0032", // op 1432
	"TAD I 0033 ;Add operand to AC, Indexed ZP 0033", // op 1433
	"TAD I 0034 ;Add operand to AC, Indexed ZP 0034", // op 1434
	"TAD I 0035 ;Add operand to AC, Indexed ZP 0035", // op 1435
	"TAD I 0036 ;Add operand to AC, Indexed ZP 0036", // op 1436
	"TAD I 0037 ;Add operand to AC, Indexed ZP 0037", // op 1437
	"TAD I 0040 ;Add operand to AC, Indexed ZP 0040", // op 1440
	"TAD I 0041 ;Add operand to AC, Indexed ZP 0041", // op 1441
	"TAD I 0042 ;Add operand to AC, Indexed ZP 0042", // op 1442
	"TAD I 0043 ;Add operand to AC, Indexed ZP 0043", // op 1443
	"TAD I 0044 ;Add operand to AC, Indexed ZP 0044", // op 1444
	"TAD I 0045 ;Add operand to AC, Indexed ZP 0045", // op 1445
	"TAD I 0046 ;Add operand to AC, Indexed ZP 0046", // op 1446
	"TAD I 0047 ;Add operand to AC, Indexed ZP 0047", // op 1447
	"TAD I 0050 ;Add operand to AC, Indexed ZP 0050", // op 1450
	"TAD I 0051 ;Add operand to AC, Indexed ZP 0051", // op 1451
	"TAD I 0052 ;Add operand to AC, Indexed ZP 0052", // op 1452
	"TAD I 0053 ;Add operand to AC, Indexed ZP 0053", // op 1453
	"TAD I 0054 ;Add operand to AC, Indexed ZP 0054", // op 1454
	"TAD I 0055 ;Add operand to AC, Indexed ZP 0055", // op 1455
	"TAD I 0056 ;Add operand to AC, Indexed ZP 0056", // op 1456
	"TAD I 0057 ;Add operand to AC, Indexed ZP 0057", // op 1457
	"TAD I 0060 ;Add operand to AC, Indexed ZP 0060", // op 1460
	"TAD I 0061 ;Add operand to AC, Indexed ZP 0061", // op 1461
	"TAD I 0062 ;Add operand to AC, Indexed ZP 0062", // op 1462
	"TAD I 0063 ;Add operand to AC, Indexed ZP 0063", // op 1463
	"TAD I 0064 ;Add operand to AC, Indexed ZP 0064", // op 1464
	"TAD I 0065 ;Add operand to AC, Indexed ZP 0065", // op 1465
	"TAD I 0066 ;Add operand to AC, Indexed ZP 0066", // op 1466
	"TAD I 0067 ;Add operand to AC, Indexed ZP 0067", // op 1467
	"TAD I 0070 ;Add operand to AC, Indexed ZP 0070", // op 1470
	"TAD I 0071 ;Add operand to AC, Indexed ZP 0071", // op 1471
	"TAD I 0072 ;Add operand to AC, Indexed ZP 0072", // op 1472
	"TAD I 0073 ;Add operand to AC, Indexed ZP 0073", // op 1473
	"TAD I 0074 ;Add operand to AC, Indexed ZP 0074", // op 1474
	"TAD I 0075 ;Add operand to AC, Indexed ZP 0075", // op 1475
	"TAD I 0076 ;Add operand to AC, Indexed ZP 0076", // op 1476
	"TAD I 0077 ;Add operand to AC, Indexed ZP 0077", // op 1477
	"TAD I 0100 ;Add operand to AC, Indexed ZP 0100", // op 1500
	"TAD I 0101 ;Add operand to AC, Indexed ZP 0101", // op 1501
	"TAD I 0102 ;Add operand to AC, Indexed ZP 0102", // op 1502
	"TAD I 0103 ;Add operand to AC, Indexed ZP 0103", // op 1503
	"TAD I 0104 ;Add operand to AC, Indexed ZP 0104", // op 1504
	"TAD I 0105 ;Add operand to AC, Indexed ZP 0105", // op 1505
	"TAD I 0106 ;Add operand to AC, Indexed ZP 0106", // op 1506
	"TAD I 0107 ;Add operand to AC, Indexed ZP 0107", // op 1507
	"TAD I 0110 ;Add operand to AC, Indexed ZP 0110", // op 1510
	"TAD I 0111 ;Add operand to AC, Indexed ZP 0111", // op 1511
	"TAD I 0112 ;Add operand to AC, Indexed ZP 0112", // op 1512
	"TAD I 0113 ;Add operand to AC, Indexed ZP 0113", // op 1513
	"TAD I 0114 ;Add operand to AC, Indexed ZP 0114", // op 1514
	"TAD I 0115 ;Add operand to AC, Indexed ZP 0115", // op 1515
	"TAD I 0116 ;Add operand to AC, Indexed ZP 0116", // op 1516
	"TAD I 0117 ;Add operand to AC, Indexed ZP 0117", // op 1517
	"TAD I 0120 ;Add operand to AC, Indexed ZP 0120", // op 1520
	"TAD I 0121 ;Add operand to AC, Indexed ZP 0121", // op 1521
	"TAD I 0122 ;Add operand to AC, Indexed ZP 0122", // op 1522
	"TAD I 0123 ;Add operand to AC, Indexed ZP 0123", // op 1523
	"TAD I 0124 ;Add operand to AC, Indexed ZP 0124", // op 1524
	"TAD I 0125 ;Add operand to AC, Indexed ZP 0125", // op 1525
	"TAD I 0126 ;Add operand to AC, Indexed ZP 0126", // op 1526
	"TAD I 0127 ;Add operand to AC, Indexed ZP 0127", // op 1527
	"TAD I 0130 ;Add operand to AC, Indexed ZP 0130", // op 1530
	"TAD I 0131 ;Add operand to AC, Indexed ZP 0131", // op 1531
	"TAD I 0132 ;Add operand to AC, Indexed ZP 0132", // op 1532
	"TAD I 0133 ;Add operand to AC, Indexed ZP 0133", // op 1533
	"TAD I 0134 ;Add operand to AC, Indexed ZP 0134", // op 1534
	"TAD I 0135 ;Add operand to AC, Indexed ZP 0135", // op 1535
	"TAD I 0136 ;Add operand to AC, Indexed ZP 0136", // op 1536
	"TAD I 0137 ;Add operand to AC, Indexed ZP 0137", // op 1537
	"TAD I 0140 ;Add operand to AC, Indexed ZP 0140", // op 1540
	"TAD I 0141 ;Add operand to AC, Indexed ZP 0141", // op 1541
	"TAD I 0142 ;Add operand to AC, Indexed ZP 0142", // op 1542
	"TAD I 0143 ;Add operand to AC, Indexed ZP 0143", // op 1543
	"TAD I 0144 ;Add operand to AC, Indexed ZP 0144", // op 1544
	"TAD I 0145 ;Add operand to AC, Indexed ZP 0145", // op 1545
	"TAD I 0146 ;Add operand to AC, Indexed ZP 0146", // op 1546
	"TAD I 0147 ;Add operand to AC, Indexed ZP 0147", // op 1547
	"TAD I 0150 ;Add operand to AC, Indexed ZP 0150", // op 1550
	"TAD I 0151 ;Add operand to AC, Indexed ZP 0151", // op 1551
	"TAD I 0152 ;Add operand to AC, Indexed ZP 0152", // op 1552
	"TAD I 0153 ;Add operand to AC, Indexed ZP 0153", // op 1553
	"TAD I 0154 ;Add operand to AC, Indexed ZP 0154", // op 1554
	"TAD I 0155 ;Add operand to AC, Indexed ZP 0155", // op 1555
	"TAD I 0156 ;Add operand to AC, Indexed ZP 0156", // op 1556
	"TAD I 0157 ;Add operand to AC, Indexed ZP 0157", // op 1557
	"TAD I 0160 ;Add operand to AC, Indexed ZP 0160", // op 1560
	"TAD I 0161 ;Add operand to AC, Indexed ZP 0161", // op 1561
	"TAD I 0162 ;Add operand to AC, Indexed ZP 0162", // op 1562
	"TAD I 0163 ;Add operand to AC, Indexed ZP 0163", // op 1563
	"TAD I 0164 ;Add operand to AC, Indexed ZP 0164", // op 1564
	"TAD I 0165 ;Add operand to AC, Indexed ZP 0165", // op 1565
	"TAD I 0166 ;Add operand to AC, Indexed ZP 0166", // op 1566
	"TAD I 0167 ;Add operand to AC, Indexed ZP 0167", // op 1567
	"TAD I 0170 ;Add operand to AC, Indexed ZP 0170", // op 1570
	"TAD I 0171 ;Add operand to AC, Indexed ZP 0171", // op 1571
	"TAD I 0172 ;Add operand to AC, Indexed ZP 0172", // op 1572
	"TAD I 0173 ;Add operand to AC, Indexed ZP 0173", // op 1573
	"TAD I 0174 ;Add operand to AC, Indexed ZP 0174", // op 1574
	"TAD I 0175 ;Add operand to AC, Indexed ZP 0175", // op 1575
	"TAD I 0176 ;Add operand to AC, Indexed ZP 0176", // op 1576
	"TAD I 0177 ;Add operand to AC, Indexed ZP 0177", // op 1577
	"TAD I @@00 ;Add operand to AC, Indexed Current page @@00", // op 1600
	"TAD I @@01 ;Add operand to AC, Indexed Current page @@01", // op 1601
	"TAD I @@02 ;Add operand to AC, Indexed Current page @@02", // op 1602
	"TAD I @@03 ;Add operand to AC, Indexed Current page @@03", // op 1603
	"TAD I @@04 ;Add operand to AC, Indexed Current page @@04", // op 1604
	"TAD I @@05 ;Add operand to AC, Indexed Current page @@05", // op 1605
	"TAD I @@06 ;Add operand to AC, Indexed Current page @@06", // op 1606
	"TAD I @@07 ;Add operand to AC, Indexed Current page @@07", // op 1607
	"TAD I @@10 ;Add operand to AC, Indexed Current page @@10", // op 1610
	"TAD I @@11 ;Add operand to AC, Indexed Current page @@11", // op 1611
	"TAD I @@12 ;Add operand to AC, Indexed Current page @@12", // op 1612
	"TAD I @@13 ;Add operand to AC, Indexed Current page @@13", // op 1613
	"TAD I @@14 ;Add operand to AC, Indexed Current page @@14", // op 1614
	"TAD I @@15 ;Add operand to AC, Indexed Current page @@15", // op 1615
	"TAD I @@16 ;Add operand to AC, Indexed Current page @@16", // op 1616
	"TAD I @@17 ;Add operand to AC, Indexed Current page @@17", // op 1617
	"TAD I @@20 ;Add operand to AC, Indexed Current page @@20", // op 1620
	"TAD I @@21 ;Add operand to AC, Indexed Current page @@21", // op 1621
	"TAD I @@22 ;Add operand to AC, Indexed Current page @@22", // op 1622
	"TAD I @@23 ;Add operand to AC, Indexed Current page @@23", // op 1623
	"TAD I @@24 ;Add operand to AC, Indexed Current page @@24", // op 1624
	"TAD I @@25 ;Add operand to AC, Indexed Current page @@25", // op 1625
	"TAD I @@26 ;Add operand to AC, Indexed Current page @@26", // op 1626
	"TAD I @@27 ;Add operand to AC, Indexed Current page @@27", // op 1627
	"TAD I @@30 ;Add operand to AC, Indexed Current page @@30", // op 1630
	"TAD I @@31 ;Add operand to AC, Indexed Current page @@31", // op 1631
	"TAD I @@32 ;Add operand to AC, Indexed Current page @@32", // op 1632
	"TAD I @@33 ;Add operand to AC, Indexed Current page @@33", // op 1633
	"TAD I @@34 ;Add operand to AC, Indexed Current page @@34", // op 1634
	"TAD I @@35 ;Add operand to AC, Indexed Current page @@35", // op 1635
	"TAD I @@36 ;Add operand to AC, Indexed Current page @@36", // op 1636
	"TAD I @@37 ;Add operand to AC, Indexed Current page @@37", // op 1637
	"TAD I @@40 ;Add operand to AC, Indexed Current page @@40", // op 1640
	"TAD I @@41 ;Add operand to AC, Indexed Current page @@41", // op 1641
	"TAD I @@42 ;Add operand to AC, Indexed Current page @@42", // op 1642
	"TAD I @@43 ;Add operand to AC, Indexed Current page @@43", // op 1643
	"TAD I @@44 ;Add operand to AC, Indexed Current page @@44", // op 1644
	"TAD I @@45 ;Add operand to AC, Indexed Current page @@45", // op 1645
	"TAD I @@46 ;Add operand to AC, Indexed Current page @@46", // op 1646
	"TAD I @@47 ;Add operand to AC, Indexed Current page @@47", // op 1647
	"TAD I @@50 ;Add operand to AC, Indexed Current page @@50", // op 1650
	"TAD I @@51 ;Add operand to AC, Indexed Current page @@51", // op 1651
	"TAD I @@52 ;Add operand to AC, Indexed Current page @@52", // op 1652
	"TAD I @@53 ;Add operand to AC, Indexed Current page @@53", // op 1653
	"TAD I @@54 ;Add operand to AC, Indexed Current page @@54", // op 1654
	"TAD I @@55 ;Add operand to AC, Indexed Current page @@55", // op 1655
	"TAD I @@56 ;Add operand to AC, Indexed Current page @@56", // op 1656
	"TAD I @@57 ;Add operand to AC, Indexed Current page @@57", // op 1657
	"TAD I @@60 ;Add operand to AC, Indexed Current page @@60", // op 1660
	"TAD I @@61 ;Add operand to AC, Indexed Current page @@61", // op 1661
	"TAD I @@62 ;Add operand to AC, Indexed Current page @@62", // op 1662
	"TAD I @@63 ;Add operand to AC, Indexed Current page @@63", // op 1663
	"TAD I @@64 ;Add operand to AC, Indexed Current page @@64", // op 1664
	"TAD I @@65 ;Add operand to AC, Indexed Current page @@65", // op 1665
	"TAD I @@66 ;Add operand to AC, Indexed Current page @@66", // op 1666
	"TAD I @@67 ;Add operand to AC, Indexed Current page @@67", // op 1667
	"TAD I @@70 ;Add operand to AC, Indexed Current page @@70", // op 1670
	"TAD I @@71 ;Add operand to AC, Indexed Current page @@71", // op 1671
	"TAD I @@72 ;Add operand to AC, Indexed Current page @@72", // op 1672
	"TAD I @@73 ;Add operand to AC, Indexed Current page @@73", // op 1673
	"TAD I @@74 ;Add operand to AC, Indexed Current page @@74", // op 1674
	"TAD I @@75 ;Add operand to AC, Indexed Current page @@75", // op 1675
	"TAD I @@76 ;Add operand to AC, Indexed Current page @@76", // op 1676
	"TAD I @@77 ;Add operand to AC, Indexed Current page @@77", // op 1677
	"TAD I @@00 ;Add operand to AC, Indexed Current page @@00", // op 1700
	"TAD I @@01 ;Add operand to AC, Indexed Current page @@01", // op 1701
	"TAD I @@02 ;Add operand to AC, Indexed Current page @@02", // op 1702
	"TAD I @@03 ;Add operand to AC, Indexed Current page @@03", // op 1703
	"TAD I @@04 ;Add operand to AC, Indexed Current page @@04", // op 1704
	"TAD I @@05 ;Add operand to AC, Indexed Current page @@05", // op 1705
	"TAD I @@06 ;Add operand to AC, Indexed Current page @@06", // op 1706
	"TAD I @@07 ;Add operand to AC, Indexed Current page @@07", // op 1707
	"TAD I @@10 ;Add operand to AC, Indexed Current page @@10", // op 1710
	"TAD I @@11 ;Add operand to AC, Indexed Current page @@11", // op 1711
	"TAD I @@12 ;Add operand to AC, Indexed Current page @@12", // op 1712
	"TAD I @@13 ;Add operand to AC, Indexed Current page @@13", // op 1713
	"TAD I @@14 ;Add operand to AC, Indexed Current page @@14", // op 1714
	"TAD I @@15 ;Add operand to AC, Indexed Current page @@15", // op 1715
	"TAD I @@16 ;Add operand to AC, Indexed Current page @@16", // op 1716
	"TAD I @@17 ;Add operand to AC, Indexed Current page @@17", // op 1717
	"TAD I @@20 ;Add operand to AC, Indexed Current page @@20", // op 1720
	"TAD I @@21 ;Add operand to AC, Indexed Current page @@21", // op 1721
	"TAD I @@22 ;Add operand to AC, Indexed Current page @@22", // op 1722
	"TAD I @@23 ;Add operand to AC, Indexed Current page @@23", // op 1723
	"TAD I @@24 ;Add operand to AC, Indexed Current page @@24", // op 1724
	"TAD I @@25 ;Add operand to AC, Indexed Current page @@25", // op 1725
	"TAD I @@26 ;Add operand to AC, Indexed Current page @@26", // op 1726
	"TAD I @@27 ;Add operand to AC, Indexed Current page @@27", // op 1727
	"TAD I @@30 ;Add operand to AC, Indexed Current page @@30", // op 1730
	"TAD I @@31 ;Add operand to AC, Indexed Current page @@31", // op 1731
	"TAD I @@32 ;Add operand to AC, Indexed Current page @@32", // op 1732
	"TAD I @@33 ;Add operand to AC, Indexed Current page @@33", // op 1733
	"TAD I @@34 ;Add operand to AC, Indexed Current page @@34", // op 1734
	"TAD I @@35 ;Add operand to AC, Indexed Current page @@35", // op 1735
	"TAD I @@36 ;Add operand to AC, Indexed Current page @@36", // op 1736
	"TAD I @@37 ;Add operand to AC, Indexed Current page @@37", // op 1737
	"TAD I @@40 ;Add operand to AC, Indexed Current page @@40", // op 1740
	"TAD I @@41 ;Add operand to AC, Indexed Current page @@41", // op 1741
	"TAD I @@42 ;Add operand to AC, Indexed Current page @@42", // op 1742
	"TAD I @@43 ;Add operand to AC, Indexed Current page @@43", // op 1743
	"TAD I @@44 ;Add operand to AC, Indexed Current page @@44", // op 1744
	"TAD I @@45 ;Add operand to AC, Indexed Current page @@45", // op 1745
	"TAD I @@46 ;Add operand to AC, Indexed Current page @@46", // op 1746
	"TAD I @@47 ;Add operand to AC, Indexed Current page @@47", // op 1747
	"TAD I @@50 ;Add operand to AC, Indexed Current page @@50", // op 1750
	"TAD I @@51 ;Add operand to AC, Indexed Current page @@51", // op 1751
	"TAD I @@52 ;Add operand to AC, Indexed Current page @@52", // op 1752
	"TAD I @@53 ;Add operand to AC, Indexed Current page @@53", // op 1753
	"TAD I @@54 ;Add operand to AC, Indexed Current page @@54", // op 1754
	"TAD I @@55 ;Add operand to AC, Indexed Current page @@55", // op 1755
	"TAD I @@56 ;Add operand to AC, Indexed Current page @@56", // op 1756
	"TAD I @@57 ;Add operand to AC, Indexed Current page @@57", // op 1757
	"TAD I @@60 ;Add operand to AC, Indexed Current page @@60", // op 1760
	"TAD I @@61 ;Add operand to AC, Indexed Current page @@61", // op 1761
	"TAD I @@62 ;Add operand to AC, Indexed Current page @@62", // op 1762
	"TAD I @@63 ;Add operand to AC, Indexed Current page @@63", // op 1763
	"TAD I @@64 ;Add operand to AC, Indexed Current page @@64", // op 1764
	"TAD I @@65 ;Add operand to AC, Indexed Current page @@65", // op 1765
	"TAD I @@66 ;Add operand to AC, Indexed Current page @@66", // op 1766
	"TAD I @@67 ;Add operand to AC, Indexed Current page @@67", // op 1767
	"TAD I @@70 ;Add operand to AC, Indexed Current page @@70", // op 1770
	"TAD I @@71 ;Add operand to AC, Indexed Current page @@71", // op 1771
	"TAD I @@72 ;Add operand to AC, Indexed Current page @@72", // op 1772
	"TAD I @@73 ;Add operand to AC, Indexed Current page @@73", // op 1773
	"TAD I @@74 ;Add operand to AC, Indexed Current page @@74", // op 1774
	"TAD I @@75 ;Add operand to AC, Indexed Current page @@75", // op 1775
	"TAD I @@76 ;Add operand to AC, Indexed Current page @@76", // op 1776
	"TAD I @@77 ;Add operand to AC, Indexed Current page @@77", // op 1777
	"ISZ 0000   ;Increment operand and skip if zero, ZP 0000 ", // op 2000
	"ISZ 0001   ;Increment operand and skip if zero, ZP 0001", // op 2001
	"ISZ 0002   ;Increment operand and skip if zero, ZP 0002", // op 2002
	"ISZ 0003   ;Increment operand and skip if zero, ZP 0003", // op 2003
	"ISZ 0004   ;Increment operand and skip if zero, ZP 0004", // op 2004
	"ISZ 0005   ;Increment operand and skip if zero, ZP 0005", // op 2005
	"ISZ 0006   ;Increment operand and skip if zero, ZP 0006", // op 2006
	"ISZ 0007   ;Increment operand and skip if zero, ZP 0007", // op 2007
	"ISZ 0010   ;Increment operand and skip if zero, ZP 0010", // op 2010
	"ISZ 0011   ;Increment operand and skip if zero, ZP 0011", // op 2011
	"ISZ 0012   ;Increment operand and skip if zero, ZP 0012", // op 2012
	"ISZ 0013   ;Increment operand and skip if zero, ZP 0013", // op 2013
	"ISZ 0014   ;Increment operand and skip if zero, ZP 0014", // op 2014
	"ISZ 0015   ;Increment operand and skip if zero, ZP 0015", // op 2015
	"ISZ 0016   ;Increment operand and skip if zero, ZP 0016", // op 2016
	"ISZ 0017   ;Increment operand and skip if zero, ZP 0017", // op 2017
	"ISZ 0020   ;Increment operand and skip if zero, ZP 0020", // op 2020
	"ISZ 0021   ;Increment operand and skip if zero, ZP 0021", // op 2021
	"ISZ 0022   ;Increment operand and skip if zero, ZP 0022", // op 2022
	"ISZ 0023   ;Increment operand and skip if zero, ZP 0023", // op 2023
	"ISZ 0024   ;Increment operand and skip if zero, ZP 0024", // op 2024
	"ISZ 0025   ;Increment operand and skip if zero, ZP 0025", // op 2025
	"ISZ 0026   ;Increment operand and skip if zero, ZP 0026", // op 2026
	"ISZ 0027   ;Increment operand and skip if zero, ZP 0027", // op 2027
	"ISZ 0030   ;Increment operand and skip if zero, ZP 0030", // op 2030
	"ISZ 0031   ;Increment operand and skip if zero, ZP 0031", // op 2031
	"ISZ 0032   ;Increment operand and skip if zero, ZP 0032", // op 2032
	"ISZ 0033   ;Increment operand and skip if zero, ZP 0033", // op 2033
	"ISZ 0034   ;Increment operand and skip if zero, ZP 0034", // op 2034
	"ISZ 0035   ;Increment operand and skip if zero, ZP 0035", // op 2035
	"ISZ 0036   ;Increment operand and skip if zero, ZP 0036", // op 2036
	"ISZ 0037   ;Increment operand and skip if zero, ZP 0037", // op 2037
	"ISZ 0040   ;Increment operand and skip if zero, ZP 0040", // op 2040
	"ISZ 0041   ;Increment operand and skip if zero, ZP 0041", // op 2041
	"ISZ 0042   ;Increment operand and skip if zero, ZP 0042", // op 2042
	"ISZ 0043   ;Increment operand and skip if zero, ZP 0043", // op 2043
	"ISZ 0044   ;Increment operand and skip if zero, ZP 0044", // op 2044
	"ISZ 0045   ;Increment operand and skip if zero, ZP 0045", // op 2045
	"ISZ 0046   ;Increment operand and skip if zero, ZP 0046", // op 2046
	"ISZ 0047   ;Increment operand and skip if zero, ZP 0047", // op 2047
	"ISZ 0050   ;Increment operand and skip if zero, ZP 0050", // op 2050
	"ISZ 0051   ;Increment operand and skip if zero, ZP 0051", // op 2051
	"ISZ 0052   ;Increment operand and skip if zero, ZP 0052", // op 2052
	"ISZ 0053   ;Increment operand and skip if zero, ZP 0053", // op 2053
	"ISZ 0054   ;Increment operand and skip if zero, ZP 0054", // op 2054
	"ISZ 0055   ;Increment operand and skip if zero, ZP 0055", // op 2055
	"ISZ 0056   ;Increment operand and skip if zero, ZP 0056", // op 2056
	"ISZ 0057   ;Increment operand and skip if zero, ZP 0057", // op 2057
	"ISZ 0060   ;Increment operand and skip if zero, ZP 0060", // op 2060
	"ISZ 0061   ;Increment operand and skip if zero, ZP 0061", // op 2061
	"ISZ 0062   ;Increment operand and skip if zero, ZP 0062", // op 2062
	"ISZ 0063   ;Increment operand and skip if zero, ZP 0063", // op 2063
	"ISZ 0064   ;Increment operand and skip if zero, ZP 0064", // op 2064
	"ISZ 0065   ;Increment operand and skip if zero, ZP 0065", // op 2065
	"ISZ 0066   ;Increment operand and skip if zero, ZP 0066", // op 2066
	"ISZ 0067   ;Increment operand and skip if zero, ZP 0067", // op 2067
	"ISZ 0070   ;Increment operand and skip if zero, ZP 0070", // op 2070
	"ISZ 0071   ;Increment operand and skip if zero, ZP 0071", // op 2071
	"ISZ 0072   ;Increment operand and skip if zero, ZP 0072", // op 2072
	"ISZ 0073   ;Increment operand and skip if zero, ZP 0073", // op 2073
	"ISZ 0074   ;Increment operand and skip if zero, ZP 0074", // op 2074
	"ISZ 0075   ;Increment operand and skip if zero, ZP 0075", // op 2075
	"ISZ 0076   ;Increment operand and skip if zero, ZP 0076", // op 2076
	"ISZ 0077   ;Increment operand and skip if zero, ZP 0077", // op 2077
	"ISZ 0100   ;Increment operand and skip if zero, ZP 0100", // op 2100
	"ISZ 0101   ;Increment operand and skip if zero, ZP 0101", // op 2101
	"ISZ 0102   ;Increment operand and skip if zero, ZP 0102", // op 2102
	"ISZ 0103   ;Increment operand and skip if zero, ZP 0103", // op 2103
	"ISZ 0104   ;Increment operand and skip if zero, ZP 0104", // op 2104
	"ISZ 0105   ;Increment operand and skip if zero, ZP 0105", // op 2105
	"ISZ 0106   ;Increment operand and skip if zero, ZP 0106", // op 2106
	"ISZ 0107   ;Increment operand and skip if zero, ZP 0107", // op 2107
	"ISZ 0110   ;Increment operand and skip if zero, ZP 0110", // op 2110
	"ISZ 0111   ;Increment operand and skip if zero, ZP 0111", // op 2111
	"ISZ 0112   ;Increment operand and skip if zero, ZP 0112", // op 2112
	"ISZ 0113   ;Increment operand and skip if zero, ZP 0113", // op 2113
	"ISZ 0114   ;Increment operand and skip if zero, ZP 0114", // op 2114
	"ISZ 0115   ;Increment operand and skip if zero, ZP 0115", // op 2115
	"ISZ 0116   ;Increment operand and skip if zero, ZP 0116", // op 2116
	"ISZ 0117   ;Increment operand and skip if zero, ZP 0117", // op 2117
	"ISZ 0120   ;Increment operand and skip if zero, ZP 0120", // op 2120
	"ISZ 0121   ;Increment operand and skip if zero, ZP 0121", // op 2121
	"ISZ 0122   ;Increment operand and skip if zero, ZP 0122", // op 2122
	"ISZ 0123   ;Increment operand and skip if zero, ZP 0123", // op 2123
	"ISZ 0124   ;Increment operand and skip if zero, ZP 0124", // op 2124
	"ISZ 0125   ;Increment operand and skip if zero, ZP 0125", // op 2125
	"ISZ 0126   ;Increment operand and skip if zero, ZP 0126", // op 2126
	"ISZ 0127   ;Increment operand and skip if zero, ZP 0127", // op 2127
	"ISZ 0130   ;Increment operand and skip if zero, ZP 0130", // op 2130
	"ISZ 0131   ;Increment operand and skip if zero, ZP 0131", // op 2131
	"ISZ 0132   ;Increment operand and skip if zero, ZP 0132", // op 2132
	"ISZ 0133   ;Increment operand and skip if zero, ZP 0133", // op 2133
	"ISZ 0134   ;Increment operand and skip if zero, ZP 0134", // op 2134
	"ISZ 0135   ;Increment operand and skip if zero, ZP 0135", // op 2135
	"ISZ 0136   ;Increment operand and skip if zero, ZP 0136", // op 2136
	"ISZ 0137   ;Increment operand and skip if zero, ZP 0137", // op 2137
	"ISZ 0140   ;Increment operand and skip if zero, ZP 0140", // op 2140
	"ISZ 0141   ;Increment operand and skip if zero, ZP 0141", // op 2141
	"ISZ 0142   ;Increment operand and skip if zero, ZP 0142", // op 2142
	"ISZ 0143   ;Increment operand and skip if zero, ZP 0143", // op 2143
	"ISZ 0144   ;Increment operand and skip if zero, ZP 0144", // op 2144
	"ISZ 0145   ;Increment operand and skip if zero, ZP 0145", // op 2145
	"ISZ 0146   ;Increment operand and skip if zero, ZP 0146", // op 2146
	"ISZ 0147   ;Increment operand and skip if zero, ZP 0147", // op 2147
	"ISZ 0150   ;Increment operand and skip if zero, ZP 0150", // op 2150
	"ISZ 0151   ;Increment operand and skip if zero, ZP 0151", // op 2151
	"ISZ 0152   ;Increment operand and skip if zero, ZP 0152", // op 2152
	"ISZ 0153   ;Increment operand and skip if zero, ZP 0153", // op 2153
	"ISZ 0154   ;Increment operand and skip if zero, ZP 0154", // op 2154
	"ISZ 0155   ;Increment operand and skip if zero, ZP 0155", // op 2155
	"ISZ 0156   ;Increment operand and skip if zero, ZP 0156", // op 2156
	"ISZ 0157   ;Increment operand and skip if zero, ZP 0157", // op 2157
	"ISZ 0160   ;Increment operand and skip if zero, ZP 0160", // op 2160
	"ISZ 0161   ;Increment operand and skip if zero, ZP 0161", // op 2161
	"ISZ 0162   ;Increment operand and skip if zero, ZP 0162", // op 2162
	"ISZ 0163   ;Increment operand and skip if zero, ZP 0163", // op 2163
	"ISZ 0164   ;Increment operand and skip if zero, ZP 0164", // op 2164
	"ISZ 0165   ;Increment operand and skip if zero, ZP 0165", // op 2165
	"ISZ 0166   ;Increment operand and skip if zero, ZP 0166", // op 2166
	"ISZ 0167   ;Increment operand and skip if zero, ZP 0167", // op 2167
	"ISZ 0170   ;Increment operand and skip if zero, ZP 0170", // op 2170
	"ISZ 0171   ;Increment operand and skip if zero, ZP 0171", // op 2171
	"ISZ 0172   ;Increment operand and skip if zero, ZP 0172", // op 2172
	"ISZ 0173   ;Increment operand and skip if zero, ZP 0173", // op 2173
	"ISZ 0174   ;Increment operand and skip if zero, ZP 0174", // op 2174
	"ISZ 0175   ;Increment operand and skip if zero, ZP 0175", // op 2175
	"ISZ 0176   ;Increment operand and skip if zero, ZP 0176", // op 2176
	"ISZ 0177   ;Increment operand and skip if zero, ZP 0177", // op 2177
	"ISZ @@00   ;Increment operand and skip if zero, Current page @@00", // op 2200
	"ISZ @@01   ;Increment operand and skip if zero, Current page @@01", // op 2201
	"ISZ @@02   ;Increment operand and skip if zero, Current page @@02", // op 2202
	"ISZ @@03   ;Increment operand and skip if zero, Current page @@03", // op 2203
	"ISZ @@04   ;Increment operand and skip if zero, Current page @@04", // op 2204
	"ISZ @@05   ;Increment operand and skip if zero, Current page @@05", // op 2205
	"ISZ @@06   ;Increment operand and skip if zero, Current page @@06", // op 2206
	"ISZ @@07   ;Increment operand and skip if zero, Current page @@07", // op 2207
	"ISZ @@10   ;Increment operand and skip if zero, Current page @@10", // op 2210
	"ISZ @@11   ;Increment operand and skip if zero, Current page @@11", // op 2211
	"ISZ @@12   ;Increment operand and skip if zero, Current page @@12", // op 2212
	"ISZ @@13   ;Increment operand and skip if zero, Current page @@13", // op 2213
	"ISZ @@14   ;Increment operand and skip if zero, Current page @@14", // op 2214
	"ISZ @@15   ;Increment operand and skip if zero, Current page @@15", // op 2215
	"ISZ @@16   ;Increment operand and skip if zero, Current page @@16", // op 2216
	"ISZ @@17   ;Increment operand and skip if zero, Current page @@17", // op 2217
	"ISZ @@20   ;Increment operand and skip if zero, Current page @@20", // op 2220
	"ISZ @@21   ;Increment operand and skip if zero, Current page @@21", // op 2221
	"ISZ @@22   ;Increment operand and skip if zero, Current page @@22", // op 2222
	"ISZ @@23   ;Increment operand and skip if zero, Current page @@23", // op 2223
	"ISZ @@24   ;Increment operand and skip if zero, Current page @@24", // op 2224
	"ISZ @@25   ;Increment operand and skip if zero, Current page @@25", // op 2225
	"ISZ @@26   ;Increment operand and skip if zero, Current page @@26", // op 2226
	"ISZ @@27   ;Increment operand and skip if zero, Current page @@27", // op 2227
	"ISZ @@30   ;Increment operand and skip if zero, Current page @@30", // op 2230
	"ISZ @@31   ;Increment operand and skip if zero, Current page @@31", // op 2231
	"ISZ @@32   ;Increment operand and skip if zero, Current page @@32", // op 2232
	"ISZ @@33   ;Increment operand and skip if zero, Current page @@33", // op 2233
	"ISZ @@34   ;Increment operand and skip if zero, Current page @@34", // op 2234
	"ISZ @@35   ;Increment operand and skip if zero, Current page @@35", // op 2235
	"ISZ @@36   ;Increment operand and skip if zero, Current page @@36", // op 2236
	"ISZ @@37   ;Increment operand and skip if zero, Current page @@37", // op 2237
	"ISZ @@40   ;Increment operand and skip if zero, Current page @@40", // op 2240
	"ISZ @@41   ;Increment operand and skip if zero, Current page @@41", // op 2241
	"ISZ @@42   ;Increment operand and skip if zero, Current page @@42", // op 2242
	"ISZ @@43   ;Increment operand and skip if zero, Current page @@43", // op 2243
	"ISZ @@44   ;Increment operand and skip if zero, Current page @@44", // op 2244
	"ISZ @@45   ;Increment operand and skip if zero, Current page @@45", // op 2245
	"ISZ @@46   ;Increment operand and skip if zero, Current page @@46", // op 2246
	"ISZ @@47   ;Increment operand and skip if zero, Current page @@47", // op 2247
	"ISZ @@50   ;Increment operand and skip if zero, Current page @@50", // op 2250
	"ISZ @@51   ;Increment operand and skip if zero, Current page @@51", // op 2251
	"ISZ @@52   ;Increment operand and skip if zero, Current page @@52", // op 2252
	"ISZ @@53   ;Increment operand and skip if zero, Current page @@53", // op 2253
	"ISZ @@54   ;Increment operand and skip if zero, Current page @@54", // op 2254
	"ISZ @@55   ;Increment operand and skip if zero, Current page @@55", // op 2255
	"ISZ @@56   ;Increment operand and skip if zero, Current page @@56", // op 2256
	"ISZ @@57   ;Increment operand and skip if zero, Current page @@57", // op 2257
	"ISZ @@60   ;Increment operand and skip if zero, Current page @@60", // op 2260
	"ISZ @@61   ;Increment operand and skip if zero, Current page @@61", // op 2261
	"ISZ @@62   ;Increment operand and skip if zero, Current page @@62", // op 2262
	"ISZ @@63   ;Increment operand and skip if zero, Current page @@63", // op 2263
	"ISZ @@64   ;Increment operand and skip if zero, Current page @@64", // op 2264
	"ISZ @@65   ;Increment operand and skip if zero, Current page @@65", // op 2265
	"ISZ @@66   ;Increment operand and skip if zero, Current page @@66", // op 2266
	"ISZ @@67   ;Increment operand and skip if zero, Current page @@67", // op 2267
	"ISZ @@70   ;Increment operand and skip if zero, Current page @@70", // op 2270
	"ISZ @@71   ;Increment operand and skip if zero, Current page @@71", // op 2271
	"ISZ @@72   ;Increment operand and skip if zero, Current page @@72", // op 2272
	"ISZ @@73   ;Increment operand and skip if zero, Current page @@73", // op 2273
	"ISZ @@74   ;Increment operand and skip if zero, Current page @@74", // op 2274
	"ISZ @@75   ;Increment operand and skip if zero, Current page @@75", // op 2275
	"ISZ @@76   ;Increment operand and skip if zero, Current page @@76", // op 2276
	"ISZ @@77   ;Increment operand and skip if zero, Current page @@77", // op 2277
	"ISZ @@00   ;Increment operand and skip if zero, Current page @@00", // op 2300
	"ISZ @@01   ;Increment operand and skip if zero, Current page @@01", // op 2301
	"ISZ @@02   ;Increment operand and skip if zero, Current page @@02", // op 2302
	"ISZ @@03   ;Increment operand and skip if zero, Current page @@03", // op 2303
	"ISZ @@04   ;Increment operand and skip if zero, Current page @@04", // op 2304
	"ISZ @@05   ;Increment operand and skip if zero, Current page @@05", // op 2305
	"ISZ @@06   ;Increment operand and skip if zero, Current page @@06", // op 2306
	"ISZ @@07   ;Increment operand and skip if zero, Current page @@07", // op 2307
	"ISZ @@10   ;Increment operand and skip if zero, Current page @@10", // op 2310
	"ISZ @@11   ;Increment operand and skip if zero, Current page @@11", // op 2311
	"ISZ @@12   ;Increment operand and skip if zero, Current page @@12", // op 2312
	"ISZ @@13   ;Increment operand and skip if zero, Current page @@13", // op 2313
	"ISZ @@14   ;Increment operand and skip if zero, Current page @@14", // op 2314
	"ISZ @@15   ;Increment operand and skip if zero, Current page @@15", // op 2315
	"ISZ @@16   ;Increment operand and skip if zero, Current page @@16", // op 2316
	"ISZ @@17   ;Increment operand and skip if zero, Current page @@17", // op 2317
	"ISZ @@20   ;Increment operand and skip if zero, Current page @@20", // op 2320
	"ISZ @@21   ;Increment operand and skip if zero, Current page @@21", // op 2321
	"ISZ @@22   ;Increment operand and skip if zero, Current page @@22", // op 2322
	"ISZ @@23   ;Increment operand and skip if zero, Current page @@23", // op 2323
	"ISZ @@24   ;Increment operand and skip if zero, Current page @@24", // op 2324
	"ISZ @@25   ;Increment operand and skip if zero, Current page @@25", // op 2325
	"ISZ @@26   ;Increment operand and skip if zero, Current page @@26", // op 2326
	"ISZ @@27   ;Increment operand and skip if zero, Current page @@27", // op 2327
	"ISZ @@30   ;Increment operand and skip if zero, Current page @@30", // op 2330
	"ISZ @@31   ;Increment operand and skip if zero, Current page @@31", // op 2331
	"ISZ @@32   ;Increment operand and skip if zero, Current page @@32", // op 2332
	"ISZ @@33   ;Increment operand and skip if zero, Current page @@33", // op 2333
	"ISZ @@34   ;Increment operand and skip if zero, Current page @@34", // op 2334
	"ISZ @@35   ;Increment operand and skip if zero, Current page @@35", // op 2335
	"ISZ @@36   ;Increment operand and skip if zero, Current page @@36", // op 2336
	"ISZ @@37   ;Increment operand and skip if zero, Current page @@37", // op 2337
	"ISZ @@40   ;Increment operand and skip if zero, Current page @@40", // op 2340
	"ISZ @@41   ;Increment operand and skip if zero, Current page @@41", // op 2341
	"ISZ @@42   ;Increment operand and skip if zero, Current page @@42", // op 2342
	"ISZ @@43   ;Increment operand and skip if zero, Current page @@43", // op 2343
	"ISZ @@44   ;Increment operand and skip if zero, Current page @@44", // op 2344
	"ISZ @@45   ;Increment operand and skip if zero, Current page @@45", // op 2345
	"ISZ @@46   ;Increment operand and skip if zero, Current page @@46", // op 2346
	"ISZ @@47   ;Increment operand and skip if zero, Current page @@47", // op 2347
	"ISZ @@50   ;Increment operand and skip if zero, Current page @@50", // op 2350
	"ISZ @@51   ;Increment operand and skip if zero, Current page @@51", // op 2351
	"ISZ @@52   ;Increment operand and skip if zero, Current page @@52", // op 2352
	"ISZ @@53   ;Increment operand and skip if zero, Current page @@53", // op 2353
	"ISZ @@54   ;Increment operand and skip if zero, Current page @@54", // op 2354
	"ISZ @@55   ;Increment operand and skip if zero, Current page @@55", // op 2355
	"ISZ @@56   ;Increment operand and skip if zero, Current page @@56", // op 2356
	"ISZ @@57   ;Increment operand and skip if zero, Current page @@57", // op 2357
	"ISZ @@60   ;Increment operand and skip if zero, Current page @@60", // op 2360
	"ISZ @@61   ;Increment operand and skip if zero, Current page @@61", // op 2361
	"ISZ @@62   ;Increment operand and skip if zero, Current page @@62", // op 2362
	"ISZ @@63   ;Increment operand and skip if zero, Current page @@63", // op 2363
	"ISZ @@64   ;Increment operand and skip if zero, Current page @@64", // op 2364
	"ISZ @@65   ;Increment operand and skip if zero, Current page @@65", // op 2365
	"ISZ @@66   ;Increment operand and skip if zero, Current page @@66", // op 2366
	"ISZ @@67   ;Increment operand and skip if zero, Current page @@67", // op 2367
	"ISZ @@70   ;Increment operand and skip if zero, Current page @@70", // op 2370
	"ISZ @@71   ;Increment operand and skip if zero, Current page @@71", // op 2371
	"ISZ @@72   ;Increment operand and skip if zero, Current page @@72", // op 2372
	"ISZ @@73   ;Increment operand and skip if zero, Current page @@73", // op 2373
	"ISZ @@74   ;Increment operand and skip if zero, Current page @@74", // op 2374
	"ISZ @@75   ;Increment operand and skip if zero, Current page @@75", // op 2375
	"ISZ @@76   ;Increment operand and skip if zero, Current page @@76", // op 2376
	"ISZ @@77   ;Increment operand and skip if zero, Current page @@77", // op 2377
	"ISZ I 0000 ;Increment operand and skip if zero, Indexed ZP 0000", // op 2400
	"ISZ I 0001 ;Increment operand and skip if zero, Indexed ZP 0001", // op 2401
	"ISZ I 0002 ;Increment operand and skip if zero, Indexed ZP 0002", // op 2402
	"ISZ I 0003 ;Increment operand and skip if zero, Indexed ZP 0003", // op 2403
	"ISZ I 0004 ;Increment operand and skip if zero, Indexed ZP 0004", // op 2404
	"ISZ I 0005 ;Increment operand and skip if zero, Indexed ZP 0005", // op 2405
	"ISZ I 0006 ;Increment operand and skip if zero, Indexed ZP 0006", // op 2406
	"ISZ I 0007 ;Increment operand and skip if zero, Indexed ZP 0007", // op 2407
	"ISZ I 0010 ;Increment operand and skip if zero, Indexed ZP 0010 [Auto pre-inc]", // op 2410
	"ISZ I 0011 ;Increment operand and skip if zero, Indexed ZP 0011 [Auto pre-inc]", // op 2411
	"ISZ I 0012 ;Increment operand and skip if zero, Indexed ZP 0012 [Auto pre-inc]", // op 2412
	"ISZ I 0013 ;Increment operand and skip if zero, Indexed ZP 0013 [Auto pre-inc]", // op 2413
	"ISZ I 0014 ;Increment operand and skip if zero, Indexed ZP 0014 [Auto pre-inc]", // op 2414
	"ISZ I 0015 ;Increment operand and skip if zero, Indexed ZP 0015 [Auto pre-inc]", // op 2415
	"ISZ I 0016 ;Increment operand and skip if zero, Indexed ZP 0016 [Auto pre-inc]", // op 2416
	"ISZ I 0017 ;Increment operand and skip if zero, Indexed ZP 0017 [Auto pre-inc]", // op 2417
	"ISZ I 0020 ;Increment operand and skip if zero, Indexed ZP 0020", // op 2420
	"ISZ I 0021 ;Increment operand and skip if zero, Indexed ZP 0021", // op 2421
	"ISZ I 0022 ;Increment operand and skip if zero, Indexed ZP 0022", // op 2422
	"ISZ I 0023 ;Increment operand and skip if zero, Indexed ZP 0023", // op 2423
	"ISZ I 0024 ;Increment operand and skip if zero, Indexed ZP 0024", // op 2424
	"ISZ I 0025 ;Increment operand and skip if zero, Indexed ZP 0025", // op 2425
	"ISZ I 0026 ;Increment operand and skip if zero, Indexed ZP 0026", // op 2426
	"ISZ I 0027 ;Increment operand and skip if zero, Indexed ZP 0027", // op 2427
	"ISZ I 0030 ;Increment operand and skip if zero, Indexed ZP 0030", // op 2430
	"ISZ I 0031 ;Increment operand and skip if zero, Indexed ZP 0031", // op 2431
	"ISZ I 0032 ;Increment operand and skip if zero, Indexed ZP 0032", // op 2432
	"ISZ I 0033 ;Increment operand and skip if zero, Indexed ZP 0033", // op 2433
	"ISZ I 0034 ;Increment operand and skip if zero, Indexed ZP 0034", // op 2434
	"ISZ I 0035 ;Increment operand and skip if zero, Indexed ZP 0035", // op 2435
	"ISZ I 0036 ;Increment operand and skip if zero, Indexed ZP 0036", // op 2436
	"ISZ I 0037 ;Increment operand and skip if zero, Indexed ZP 0037", // op 2437
	"ISZ I 0040 ;Increment operand and skip if zero, Indexed ZP 0040", // op 2440
	"ISZ I 0041 ;Increment operand and skip if zero, Indexed ZP 0041", // op 2441
	"ISZ I 0042 ;Increment operand and skip if zero, Indexed ZP 0042", // op 2442
	"ISZ I 0043 ;Increment operand and skip if zero, Indexed ZP 0043", // op 2443
	"ISZ I 0044 ;Increment operand and skip if zero, Indexed ZP 0044", // op 2444
	"ISZ I 0045 ;Increment operand and skip if zero, Indexed ZP 0045", // op 2445
	"ISZ I 0046 ;Increment operand and skip if zero, Indexed ZP 0046", // op 2446
	"ISZ I 0047 ;Increment operand and skip if zero, Indexed ZP 0047", // op 2447
	"ISZ I 0050 ;Increment operand and skip if zero, Indexed ZP 0050", // op 2450
	"ISZ I 0051 ;Increment operand and skip if zero, Indexed ZP 0051", // op 2451
	"ISZ I 0052 ;Increment operand and skip if zero, Indexed ZP 0052", // op 2452
	"ISZ I 0053 ;Increment operand and skip if zero, Indexed ZP 0053", // op 2453
	"ISZ I 0054 ;Increment operand and skip if zero, Indexed ZP 0054", // op 2454
	"ISZ I 0055 ;Increment operand and skip if zero, Indexed ZP 0055", // op 2455
	"ISZ I 0056 ;Increment operand and skip if zero, Indexed ZP 0056", // op 2456
	"ISZ I 0057 ;Increment operand and skip if zero, Indexed ZP 0057", // op 2457
	"ISZ I 0060 ;Increment operand and skip if zero, Indexed ZP 0060", // op 2460
	"ISZ I 0061 ;Increment operand and skip if zero, Indexed ZP 0061", // op 2461
	"ISZ I 0062 ;Increment operand and skip if zero, Indexed ZP 0062", // op 2462
	"ISZ I 0063 ;Increment operand and skip if zero, Indexed ZP 0063", // op 2463
	"ISZ I 0064 ;Increment operand and skip if zero, Indexed ZP 0064", // op 2464
	"ISZ I 0065 ;Increment operand and skip if zero, Indexed ZP 0065", // op 2465
	"ISZ I 0066 ;Increment operand and skip if zero, Indexed ZP 0066", // op 2466
	"ISZ I 0067 ;Increment operand and skip if zero, Indexed ZP 0067", // op 2467
	"ISZ I 0070 ;Increment operand and skip if zero, Indexed ZP 0070", // op 2470
	"ISZ I 0071 ;Increment operand and skip if zero, Indexed ZP 0071", // op 2471
	"ISZ I 0072 ;Increment operand and skip if zero, Indexed ZP 0072", // op 2472
	"ISZ I 0073 ;Increment operand and skip if zero, Indexed ZP 0073", // op 2473
	"ISZ I 0074 ;Increment operand and skip if zero, Indexed ZP 0074", // op 2474
	"ISZ I 0075 ;Increment operand and skip if zero, Indexed ZP 0075", // op 2475
	"ISZ I 0076 ;Increment operand and skip if zero, Indexed ZP 0076", // op 2476
	"ISZ I 0077 ;Increment operand and skip if zero, Indexed ZP 0077", // op 2477
	"ISZ I 0100 ;Increment operand and skip if zero, Indexed ZP 0100", // op 2500
	"ISZ I 0101 ;Increment operand and skip if zero, Indexed ZP 0101", // op 2501
	"ISZ I 0102 ;Increment operand and skip if zero, Indexed ZP 0102", // op 2502
	"ISZ I 0103 ;Increment operand and skip if zero, Indexed ZP 0103", // op 2503
	"ISZ I 0104 ;Increment operand and skip if zero, Indexed ZP 0104", // op 2504
	"ISZ I 0105 ;Increment operand and skip if zero, Indexed ZP 0105", // op 2505
	"ISZ I 0106 ;Increment operand and skip if zero, Indexed ZP 0106", // op 2506
	"ISZ I 0107 ;Increment operand and skip if zero, Indexed ZP 0107", // op 2507
	"ISZ I 0110 ;Increment operand and skip if zero, Indexed ZP 0110", // op 2510
	"ISZ I 0111 ;Increment operand and skip if zero, Indexed ZP 0111", // op 2511
	"ISZ I 0112 ;Increment operand and skip if zero, Indexed ZP 0112", // op 2512
	"ISZ I 0113 ;Increment operand and skip if zero, Indexed ZP 0113", // op 2513
	"ISZ I 0114 ;Increment operand and skip if zero, Indexed ZP 0114", // op 2514
	"ISZ I 0115 ;Increment operand and skip if zero, Indexed ZP 0115", // op 2515
	"ISZ I 0116 ;Increment operand and skip if zero, Indexed ZP 0116", // op 2516
	"ISZ I 0117 ;Increment operand and skip if zero, Indexed ZP 0117", // op 2517
	"ISZ I 0120 ;Increment operand and skip if zero, Indexed ZP 0120", // op 2520
	"ISZ I 0121 ;Increment operand and skip if zero, Indexed ZP 0121", // op 2521
	"ISZ I 0122 ;Increment operand and skip if zero, Indexed ZP 0122", // op 2522
	"ISZ I 0123 ;Increment operand and skip if zero, Indexed ZP 0123", // op 2523
	"ISZ I 0124 ;Increment operand and skip if zero, Indexed ZP 0124", // op 2524
	"ISZ I 0125 ;Increment operand and skip if zero, Indexed ZP 0125", // op 2525
	"ISZ I 0126 ;Increment operand and skip if zero, Indexed ZP 0126", // op 2526
	"ISZ I 0127 ;Increment operand and skip if zero, Indexed ZP 0127", // op 2527
	"ISZ I 0130 ;Increment operand and skip if zero, Indexed ZP 0130", // op 2530
	"ISZ I 0131 ;Increment operand and skip if zero, Indexed ZP 0131", // op 2531
	"ISZ I 0132 ;Increment operand and skip if zero, Indexed ZP 0132", // op 2532
	"ISZ I 0133 ;Increment operand and skip if zero, Indexed ZP 0133", // op 2533
	"ISZ I 0134 ;Increment operand and skip if zero, Indexed ZP 0134", // op 2534
	"ISZ I 0135 ;Increment operand and skip if zero, Indexed ZP 0135", // op 2535
	"ISZ I 0136 ;Increment operand and skip if zero, Indexed ZP 0136", // op 2536
	"ISZ I 0137 ;Increment operand and skip if zero, Indexed ZP 0137", // op 2537
	"ISZ I 0140 ;Increment operand and skip if zero, Indexed ZP 0140", // op 2540
	"ISZ I 0141 ;Increment operand and skip if zero, Indexed ZP 0141", // op 2541
	"ISZ I 0142 ;Increment operand and skip if zero, Indexed ZP 0142", // op 2542
	"ISZ I 0143 ;Increment operand and skip if zero, Indexed ZP 0143", // op 2543
	"ISZ I 0144 ;Increment operand and skip if zero, Indexed ZP 0144", // op 2544
	"ISZ I 0145 ;Increment operand and skip if zero, Indexed ZP 0145", // op 2545
	"ISZ I 0146 ;Increment operand and skip if zero, Indexed ZP 0146", // op 2546
	"ISZ I 0147 ;Increment operand and skip if zero, Indexed ZP 0147", // op 2547
	"ISZ I 0150 ;Increment operand and skip if zero, Indexed ZP 0150", // op 2550
	"ISZ I 0151 ;Increment operand and skip if zero, Indexed ZP 0151", // op 2551
	"ISZ I 0152 ;Increment operand and skip if zero, Indexed ZP 0152", // op 2552
	"ISZ I 0153 ;Increment operand and skip if zero, Indexed ZP 0153", // op 2553
	"ISZ I 0154 ;Increment operand and skip if zero, Indexed ZP 0154", // op 2554
	"ISZ I 0155 ;Increment operand and skip if zero, Indexed ZP 0155", // op 2555
	"ISZ I 0156 ;Increment operand and skip if zero, Indexed ZP 0156", // op 2556
	"ISZ I 0157 ;Increment operand and skip if zero, Indexed ZP 0157", // op 2557
	"ISZ I 0160 ;Increment operand and skip if zero, Indexed ZP 0160", // op 2560
	"ISZ I 0161 ;Increment operand and skip if zero, Indexed ZP 0161", // op 2561
	"ISZ I 0162 ;Increment operand and skip if zero, Indexed ZP 0162", // op 2562
	"ISZ I 0163 ;Increment operand and skip if zero, Indexed ZP 0163", // op 2563
	"ISZ I 0164 ;Increment operand and skip if zero, Indexed ZP 0164", // op 2564
	"ISZ I 0165 ;Increment operand and skip if zero, Indexed ZP 0165", // op 2565
	"ISZ I 0166 ;Increment operand and skip if zero, Indexed ZP 0166", // op 2566
	"ISZ I 0167 ;Increment operand and skip if zero, Indexed ZP 0167", // op 2567
	"ISZ I 0170 ;Increment operand and skip if zero, Indexed ZP 0170", // op 2570
	"ISZ I 0171 ;Increment operand and skip if zero, Indexed ZP 0171", // op 2571
	"ISZ I 0172 ;Increment operand and skip if zero, Indexed ZP 0172", // op 2572
	"ISZ I 0173 ;Increment operand and skip if zero, Indexed ZP 0173", // op 2573
	"ISZ I 0174 ;Increment operand and skip if zero, Indexed ZP 0174", // op 2574
	"ISZ I 0175 ;Increment operand and skip if zero, Indexed ZP 0175", // op 2575
	"ISZ I 0176 ;Increment operand and skip if zero, Indexed ZP 0176", // op 2576
	"ISZ I 0177 ;Increment operand and skip if zero, Indexed ZP 0177", // op 2577
	"ISZ I @@00 ;Increment operand and skip if zero, Indexed Current page @@00", // op 2600
	"ISZ I @@01 ;Increment operand and skip if zero, Indexed Current page @@01", // op 2601
	"ISZ I @@02 ;Increment operand and skip if zero, Indexed Current page @@02", // op 2602
	"ISZ I @@03 ;Increment operand and skip if zero, Indexed Current page @@03", // op 2603
	"ISZ I @@04 ;Increment operand and skip if zero, Indexed Current page @@04", // op 2604
	"ISZ I @@05 ;Increment operand and skip if zero, Indexed Current page @@05", // op 2605
	"ISZ I @@06 ;Increment operand and skip if zero, Indexed Current page @@06", // op 2606
	"ISZ I @@07 ;Increment operand and skip if zero, Indexed Current page @@07", // op 2607
	"ISZ I @@10 ;Increment operand and skip if zero, Indexed Current page @@10", // op 2610
	"ISZ I @@11 ;Increment operand and skip if zero, Indexed Current page @@11", // op 2611
	"ISZ I @@12 ;Increment operand and skip if zero, Indexed Current page @@12", // op 2612
	"ISZ I @@13 ;Increment operand and skip if zero, Indexed Current page @@13", // op 2613
	"ISZ I @@14 ;Increment operand and skip if zero, Indexed Current page @@14", // op 2614
	"ISZ I @@15 ;Increment operand and skip if zero, Indexed Current page @@15", // op 2615
	"ISZ I @@16 ;Increment operand and skip if zero, Indexed Current page @@16", // op 2616
	"ISZ I @@17 ;Increment operand and skip if zero, Indexed Current page @@17", // op 2617
	"ISZ I @@20 ;Increment operand and skip if zero, Indexed Current page @@20", // op 2620
	"ISZ I @@21 ;Increment operand and skip if zero, Indexed Current page @@21", // op 2621
	"ISZ I @@22 ;Increment operand and skip if zero, Indexed Current page @@22", // op 2622
	"ISZ I @@23 ;Increment operand and skip if zero, Indexed Current page @@23", // op 2623
	"ISZ I @@24 ;Increment operand and skip if zero, Indexed Current page @@24", // op 2624
	"ISZ I @@25 ;Increment operand and skip if zero, Indexed Current page @@25", // op 2625
	"ISZ I @@26 ;Increment operand and skip if zero, Indexed Current page @@26", // op 2626
	"ISZ I @@27 ;Increment operand and skip if zero, Indexed Current page @@27", // op 2627
	"ISZ I @@30 ;Increment operand and skip if zero, Indexed Current page @@30", // op 2630
	"ISZ I @@31 ;Increment operand and skip if zero, Indexed Current page @@31", // op 2631
	"ISZ I @@32 ;Increment operand and skip if zero, Indexed Current page @@32", // op 2632
	"ISZ I @@33 ;Increment operand and skip if zero, Indexed Current page @@33", // op 2633
	"ISZ I @@34 ;Increment operand and skip if zero, Indexed Current page @@34", // op 2634
	"ISZ I @@35 ;Increment operand and skip if zero, Indexed Current page @@35", // op 2635
	"ISZ I @@36 ;Increment operand and skip if zero, Indexed Current page @@36", // op 2636
	"ISZ I @@37 ;Increment operand and skip if zero, Indexed Current page @@37", // op 2637
	"ISZ I @@40 ;Increment operand and skip if zero, Indexed Current page @@40", // op 2640
	"ISZ I @@41 ;Increment operand and skip if zero, Indexed Current page @@41", // op 2641
	"ISZ I @@42 ;Increment operand and skip if zero, Indexed Current page @@42", // op 2642
	"ISZ I @@43 ;Increment operand and skip if zero, Indexed Current page @@43", // op 2643
	"ISZ I @@44 ;Increment operand and skip if zero, Indexed Current page @@44", // op 2644
	"ISZ I @@45 ;Increment operand and skip if zero, Indexed Current page @@45", // op 2645
	"ISZ I @@46 ;Increment operand and skip if zero, Indexed Current page @@46", // op 2646
	"ISZ I @@47 ;Increment operand and skip if zero, Indexed Current page @@47", // op 2647
	"ISZ I @@50 ;Increment operand and skip if zero, Indexed Current page @@50", // op 2650
	"ISZ I @@51 ;Increment operand and skip if zero, Indexed Current page @@51", // op 2651
	"ISZ I @@52 ;Increment operand and skip if zero, Indexed Current page @@52", // op 2652
	"ISZ I @@53 ;Increment operand and skip if zero, Indexed Current page @@53", // op 2653
	"ISZ I @@54 ;Increment operand and skip if zero, Indexed Current page @@54", // op 2654
	"ISZ I @@55 ;Increment operand and skip if zero, Indexed Current page @@55", // op 2655
	"ISZ I @@56 ;Increment operand and skip if zero, Indexed Current page @@56", // op 2656
	"ISZ I @@57 ;Increment operand and skip if zero, Indexed Current page @@57", // op 2657
	"ISZ I @@60 ;Increment operand and skip if zero, Indexed Current page @@60", // op 2660
	"ISZ I @@61 ;Increment operand and skip if zero, Indexed Current page @@61", // op 2661
	"ISZ I @@62 ;Increment operand and skip if zero, Indexed Current page @@62", // op 2662
	"ISZ I @@63 ;Increment operand and skip if zero, Indexed Current page @@63", // op 2663
	"ISZ I @@64 ;Increment operand and skip if zero, Indexed Current page @@64", // op 2664
	"ISZ I @@65 ;Increment operand and skip if zero, Indexed Current page @@65", // op 2665
	"ISZ I @@66 ;Increment operand and skip if zero, Indexed Current page @@66", // op 2666
	"ISZ I @@67 ;Increment operand and skip if zero, Indexed Current page @@67", // op 2667
	"ISZ I @@70 ;Increment operand and skip if zero, Indexed Current page @@70", // op 2670
	"ISZ I @@71 ;Increment operand and skip if zero, Indexed Current page @@71", // op 2671
	"ISZ I @@72 ;Increment operand and skip if zero, Indexed Current page @@72", // op 2672
	"ISZ I @@73 ;Increment operand and skip if zero, Indexed Current page @@73", // op 2673
	"ISZ I @@74 ;Increment operand and skip if zero, Indexed Current page @@74", // op 2674
	"ISZ I @@75 ;Increment operand and skip if zero, Indexed Current page @@75", // op 2675
	"ISZ I @@76 ;Increment operand and skip if zero, Indexed Current page @@76", // op 2676
	"ISZ I @@77 ;Increment operand and skip if zero, Indexed Current page @@77", // op 2677
	"ISZ I @@00 ;Increment operand and skip if zero, Indexed Current page @@00", // op 2700
	"ISZ I @@01 ;Increment operand and skip if zero, Indexed Current page @@01", // op 2701
	"ISZ I @@02 ;Increment operand and skip if zero, Indexed Current page @@02", // op 2702
	"ISZ I @@03 ;Increment operand and skip if zero, Indexed Current page @@03", // op 2703
	"ISZ I @@04 ;Increment operand and skip if zero, Indexed Current page @@04", // op 2704
	"ISZ I @@05 ;Increment operand and skip if zero, Indexed Current page @@05", // op 2705
	"ISZ I @@06 ;Increment operand and skip if zero, Indexed Current page @@06", // op 2706
	"ISZ I @@07 ;Increment operand and skip if zero, Indexed Current page @@07", // op 2707
	"ISZ I @@10 ;Increment operand and skip if zero, Indexed Current page @@10", // op 2710
	"ISZ I @@11 ;Increment operand and skip if zero, Indexed Current page @@11", // op 2711
	"ISZ I @@12 ;Increment operand and skip if zero, Indexed Current page @@12", // op 2712
	"ISZ I @@13 ;Increment operand and skip if zero, Indexed Current page @@13", // op 2713
	"ISZ I @@14 ;Increment operand and skip if zero, Indexed Current page @@14", // op 2714
	"ISZ I @@15 ;Increment operand and skip if zero, Indexed Current page @@15", // op 2715
	"ISZ I @@16 ;Increment operand and skip if zero, Indexed Current page @@16", // op 2716
	"ISZ I @@17 ;Increment operand and skip if zero, Indexed Current page @@17", // op 2717
	"ISZ I @@20 ;Increment operand and skip if zero, Indexed Current page @@20", // op 2720
	"ISZ I @@21 ;Increment operand and skip if zero, Indexed Current page @@21", // op 2721
	"ISZ I @@22 ;Increment operand and skip if zero, Indexed Current page @@22", // op 2722
	"ISZ I @@23 ;Increment operand and skip if zero, Indexed Current page @@23", // op 2723
	"ISZ I @@24 ;Increment operand and skip if zero, Indexed Current page @@24", // op 2724
	"ISZ I @@25 ;Increment operand and skip if zero, Indexed Current page @@25", // op 2725
	"ISZ I @@26 ;Increment operand and skip if zero, Indexed Current page @@26", // op 2726
	"ISZ I @@27 ;Increment operand and skip if zero, Indexed Current page @@27", // op 2727
	"ISZ I @@30 ;Increment operand and skip if zero, Indexed Current page @@30", // op 2730
	"ISZ I @@31 ;Increment operand and skip if zero, Indexed Current page @@31", // op 2731
	"ISZ I @@32 ;Increment operand and skip if zero, Indexed Current page @@32", // op 2732
	"ISZ I @@33 ;Increment operand and skip if zero, Indexed Current page @@33", // op 2733
	"ISZ I @@34 ;Increment operand and skip if zero, Indexed Current page @@34", // op 2734
	"ISZ I @@35 ;Increment operand and skip if zero, Indexed Current page @@35", // op 2735
	"ISZ I @@36 ;Increment operand and skip if zero, Indexed Current page @@36", // op 2736
	"ISZ I @@37 ;Increment operand and skip if zero, Indexed Current page @@37", // op 2737
	"ISZ I @@40 ;Increment operand and skip if zero, Indexed Current page @@40", // op 2740
	"ISZ I @@41 ;Increment operand and skip if zero, Indexed Current page @@41", // op 2741
	"ISZ I @@42 ;Increment operand and skip if zero, Indexed Current page @@42", // op 2742
	"ISZ I @@43 ;Increment operand and skip if zero, Indexed Current page @@43", // op 2743
	"ISZ I @@44 ;Increment operand and skip if zero, Indexed Current page @@44", // op 2744
	"ISZ I @@45 ;Increment operand and skip if zero, Indexed Current page @@45", // op 2745
	"ISZ I @@46 ;Increment operand and skip if zero, Indexed Current page @@46", // op 2746
	"ISZ I @@47 ;Increment operand and skip if zero, Indexed Current page @@47", // op 2747
	"ISZ I @@50 ;Increment operand and skip if zero, Indexed Current page @@50", // op 2750
	"ISZ I @@51 ;Increment operand and skip if zero, Indexed Current page @@51", // op 2751
	"ISZ I @@52 ;Increment operand and skip if zero, Indexed Current page @@52", // op 2752
	"ISZ I @@53 ;Increment operand and skip if zero, Indexed Current page @@53", // op 2753
	"ISZ I @@54 ;Increment operand and skip if zero, Indexed Current page @@54", // op 2754
	"ISZ I @@55 ;Increment operand and skip if zero, Indexed Current page @@55", // op 2755
	"ISZ I @@56 ;Increment operand and skip if zero, Indexed Current page @@56", // op 2756
	"ISZ I @@57 ;Increment operand and skip if zero, Indexed Current page @@57", // op 2757
	"ISZ I @@60 ;Increment operand and skip if zero, Indexed Current page @@60", // op 2760
	"ISZ I @@61 ;Increment operand and skip if zero, Indexed Current page @@61", // op 2761
	"ISZ I @@62 ;Increment operand and skip if zero, Indexed Current page @@62", // op 2762
	"ISZ I @@63 ;Increment operand and skip if zero, Indexed Current page @@63", // op 2763
	"ISZ I @@64 ;Increment operand and skip if zero, Indexed Current page @@64", // op 2764
	"ISZ I @@65 ;Increment operand and skip if zero, Indexed Current page @@65", // op 2765
	"ISZ I @@66 ;Increment operand and skip if zero, Indexed Current page @@66", // op 2766
	"ISZ I @@67 ;Increment operand and skip if zero, Indexed Current page @@67", // op 2767
	"ISZ I @@70 ;Increment operand and skip if zero, Indexed Current page @@70", // op 2770
	"ISZ I @@71 ;Increment operand and skip if zero, Indexed Current page @@71", // op 2771
	"ISZ I @@72 ;Increment operand and skip if zero, Indexed Current page @@72", // op 2772
	"ISZ I @@73 ;Increment operand and skip if zero, Indexed Current page @@73", // op 2773
	"ISZ I @@74 ;Increment operand and skip if zero, Indexed Current page @@74", // op 2774
	"ISZ I @@75 ;Increment operand and skip if zero, Indexed Current page @@75", // op 2775
	"ISZ I @@76 ;Increment operand and skip if zero, Indexed Current page @@76", // op 2776
	"ISZ I @@77 ;Increment operand and skip if zero, Indexed Current page @@77", // op 2777
	"DCA 0000   ;Deposit AC to memory then clear AC, ZP 0000 ", // op 3000
	"DCA 0001   ;Deposit AC to memory then clear AC, ZP 0001", // op 3001
	"DCA 0002   ;Deposit AC to memory then clear AC, ZP 0002", // op 3002
	"DCA 0003   ;Deposit AC to memory then clear AC, ZP 0003", // op 3003
	"DCA 0004   ;Deposit AC to memory then clear AC, ZP 0004", // op 3004
	"DCA 0005   ;Deposit AC to memory then clear AC, ZP 0005", // op 3005
	"DCA 0006   ;Deposit AC to memory then clear AC, ZP 0006", // op 3006
	"DCA 0007   ;Deposit AC to memory then clear AC, ZP 0007", // op 3007
	"DCA 0010   ;Deposit AC to memory then clear AC, ZP 0010", // op 3010
	"DCA 0011   ;Deposit AC to memory then clear AC, ZP 0011", // op 3011
	"DCA 0012   ;Deposit AC to memory then clear AC, ZP 0012", // op 3012
	"DCA 0013   ;Deposit AC to memory then clear AC, ZP 0013", // op 3013
	"DCA 0014   ;Deposit AC to memory then clear AC, ZP 0014", // op 3014
	"DCA 0015   ;Deposit AC to memory then clear AC, ZP 0015", // op 3015
	"DCA 0016   ;Deposit AC to memory then clear AC, ZP 0016", // op 3016
	"DCA 0017   ;Deposit AC to memory then clear AC, ZP 0017", // op 3017
	"DCA 0020   ;Deposit AC to memory then clear AC, ZP 0020", // op 3020
	"DCA 0021   ;Deposit AC to memory then clear AC, ZP 0021", // op 3021
	"DCA 0022   ;Deposit AC to memory then clear AC, ZP 0022", // op 3022
	"DCA 0023   ;Deposit AC to memory then clear AC, ZP 0023", // op 3023
	"DCA 0024   ;Deposit AC to memory then clear AC, ZP 0024", // op 3024
	"DCA 0025   ;Deposit AC to memory then clear AC, ZP 0025", // op 3025
	"DCA 0026   ;Deposit AC to memory then clear AC, ZP 0026", // op 3026
	"DCA 0027   ;Deposit AC to memory then clear AC, ZP 0027", // op 3027
	"DCA 0030   ;Deposit AC to memory then clear AC, ZP 0030", // op 3030
	"DCA 0031   ;Deposit AC to memory then clear AC, ZP 0031", // op 3031
	"DCA 0032   ;Deposit AC to memory then clear AC, ZP 0032", // op 3032
	"DCA 0033   ;Deposit AC to memory then clear AC, ZP 0033", // op 3033
	"DCA 0034   ;Deposit AC to memory then clear AC, ZP 0034", // op 3034
	"DCA 0035   ;Deposit AC to memory then clear AC, ZP 0035", // op 3035
	"DCA 0036   ;Deposit AC to memory then clear AC, ZP 0036", // op 3036
	"DCA 0037   ;Deposit AC to memory then clear AC, ZP 0037", // op 3037
	"DCA 0040   ;Deposit AC to memory then clear AC, ZP 0040", // op 3040
	"DCA 0041   ;Deposit AC to memory then clear AC, ZP 0041", // op 3041
	"DCA 0042   ;Deposit AC to memory then clear AC, ZP 0042", // op 3042
	"DCA 0043   ;Deposit AC to memory then clear AC, ZP 0043", // op 3043
	"DCA 0044   ;Deposit AC to memory then clear AC, ZP 0044", // op 3044
	"DCA 0045   ;Deposit AC to memory then clear AC, ZP 0045", // op 3045
	"DCA 0046   ;Deposit AC to memory then clear AC, ZP 0046", // op 3046
	"DCA 0047   ;Deposit AC to memory then clear AC, ZP 0047", // op 3047
	"DCA 0050   ;Deposit AC to memory then clear AC, ZP 0050", // op 3050
	"DCA 0051   ;Deposit AC to memory then clear AC, ZP 0051", // op 3051
	"DCA 0052   ;Deposit AC to memory then clear AC, ZP 0052", // op 3052
	"DCA 0053   ;Deposit AC to memory then clear AC, ZP 0053", // op 3053
	"DCA 0054   ;Deposit AC to memory then clear AC, ZP 0054", // op 3054
	"DCA 0055   ;Deposit AC to memory then clear AC, ZP 0055", // op 3055
	"DCA 0056   ;Deposit AC to memory then clear AC, ZP 0056", // op 3056
	"DCA 0057   ;Deposit AC to memory then clear AC, ZP 0057", // op 3057
	"DCA 0060   ;Deposit AC to memory then clear AC, ZP 0060", // op 3060
	"DCA 0061   ;Deposit AC to memory then clear AC, ZP 0061", // op 3061
	"DCA 0062   ;Deposit AC to memory then clear AC, ZP 0062", // op 3062
	"DCA 0063   ;Deposit AC to memory then clear AC, ZP 0063", // op 3063
	"DCA 0064   ;Deposit AC to memory then clear AC, ZP 0064", // op 3064
	"DCA 0065   ;Deposit AC to memory then clear AC, ZP 0065", // op 3065
	"DCA 0066   ;Deposit AC to memory then clear AC, ZP 0066", // op 3066
	"DCA 0067   ;Deposit AC to memory then clear AC, ZP 0067", // op 3067
	"DCA 0070   ;Deposit AC to memory then clear AC, ZP 0070", // op 3070
	"DCA 0071   ;Deposit AC to memory then clear AC, ZP 0071", // op 3071
	"DCA 0072   ;Deposit AC to memory then clear AC, ZP 0072", // op 3072
	"DCA 0073   ;Deposit AC to memory then clear AC, ZP 0073", // op 3073
	"DCA 0074   ;Deposit AC to memory then clear AC, ZP 0074", // op 3074
	"DCA 0075   ;Deposit AC to memory then clear AC, ZP 0075", // op 3075
	"DCA 0076   ;Deposit AC to memory then clear AC, ZP 0076", // op 3076
	"DCA 0077   ;Deposit AC to memory then clear AC, ZP 0077", // op 3077
	"DCA 0100   ;Deposit AC to memory then clear AC, ZP 0100", // op 3100
	"DCA 0101   ;Deposit AC to memory then clear AC, ZP 0101", // op 3101
	"DCA 0102   ;Deposit AC to memory then clear AC, ZP 0102", // op 3102
	"DCA 0103   ;Deposit AC to memory then clear AC, ZP 0103", // op 3103
	"DCA 0104   ;Deposit AC to memory then clear AC, ZP 0104", // op 3104
	"DCA 0105   ;Deposit AC to memory then clear AC, ZP 0105", // op 3105
	"DCA 0106   ;Deposit AC to memory then clear AC, ZP 0106", // op 3106
	"DCA 0107   ;Deposit AC to memory then clear AC, ZP 0107", // op 3107
	"DCA 0110   ;Deposit AC to memory then clear AC, ZP 0110", // op 3110
	"DCA 0111   ;Deposit AC to memory then clear AC, ZP 0111", // op 3111
	"DCA 0112   ;Deposit AC to memory then clear AC, ZP 0112", // op 3112
	"DCA 0113   ;Deposit AC to memory then clear AC, ZP 0113", // op 3113
	"DCA 0114   ;Deposit AC to memory then clear AC, ZP 0114", // op 3114
	"DCA 0115   ;Deposit AC to memory then clear AC, ZP 0115", // op 3115
	"DCA 0116   ;Deposit AC to memory then clear AC, ZP 0116", // op 3116
	"DCA 0117   ;Deposit AC to memory then clear AC, ZP 0117", // op 3117
	"DCA 0120   ;Deposit AC to memory then clear AC, ZP 0120", // op 3120
	"DCA 0121   ;Deposit AC to memory then clear AC, ZP 0121", // op 3121
	"DCA 0122   ;Deposit AC to memory then clear AC, ZP 0122", // op 3122
	"DCA 0123   ;Deposit AC to memory then clear AC, ZP 0123", // op 3123
	"DCA 0124   ;Deposit AC to memory then clear AC, ZP 0124", // op 3124
	"DCA 0125   ;Deposit AC to memory then clear AC, ZP 0125", // op 3125
	"DCA 0126   ;Deposit AC to memory then clear AC, ZP 0126", // op 3126
	"DCA 0127   ;Deposit AC to memory then clear AC, ZP 0127", // op 3127
	"DCA 0130   ;Deposit AC to memory then clear AC, ZP 0130", // op 3130
	"DCA 0131   ;Deposit AC to memory then clear AC, ZP 0131", // op 3131
	"DCA 0132   ;Deposit AC to memory then clear AC, ZP 0132", // op 3132
	"DCA 0133   ;Deposit AC to memory then clear AC, ZP 0133", // op 3133
	"DCA 0134   ;Deposit AC to memory then clear AC, ZP 0134", // op 3134
	"DCA 0135   ;Deposit AC to memory then clear AC, ZP 0135", // op 3135
	"DCA 0136   ;Deposit AC to memory then clear AC, ZP 0136", // op 3136
	"DCA 0137   ;Deposit AC to memory then clear AC, ZP 0137", // op 3137
	"DCA 0140   ;Deposit AC to memory then clear AC, ZP 0140", // op 3140
	"DCA 0141   ;Deposit AC to memory then clear AC, ZP 0141", // op 3141
	"DCA 0142   ;Deposit AC to memory then clear AC, ZP 0142", // op 3142
	"DCA 0143   ;Deposit AC to memory then clear AC, ZP 0143", // op 3143
	"DCA 0144   ;Deposit AC to memory then clear AC, ZP 0144", // op 3144
	"DCA 0145   ;Deposit AC to memory then clear AC, ZP 0145", // op 3145
	"DCA 0146   ;Deposit AC to memory then clear AC, ZP 0146", // op 3146
	"DCA 0147   ;Deposit AC to memory then clear AC, ZP 0147", // op 3147
	"DCA 0150   ;Deposit AC to memory then clear AC, ZP 0150", // op 3150
	"DCA 0151   ;Deposit AC to memory then clear AC, ZP 0151", // op 3151
	"DCA 0152   ;Deposit AC to memory then clear AC, ZP 0152", // op 3152
	"DCA 0153   ;Deposit AC to memory then clear AC, ZP 0153", // op 3153
	"DCA 0154   ;Deposit AC to memory then clear AC, ZP 0154", // op 3154
	"DCA 0155   ;Deposit AC to memory then clear AC, ZP 0155", // op 3155
	"DCA 0156   ;Deposit AC to memory then clear AC, ZP 0156", // op 3156
	"DCA 0157   ;Deposit AC to memory then clear AC, ZP 0157", // op 3157
	"DCA 0160   ;Deposit AC to memory then clear AC, ZP 0160", // op 3160
	"DCA 0161   ;Deposit AC to memory then clear AC, ZP 0161", // op 3161
	"DCA 0162   ;Deposit AC to memory then clear AC, ZP 0162", // op 3162
	"DCA 0163   ;Deposit AC to memory then clear AC, ZP 0163", // op 3163
	"DCA 0164   ;Deposit AC to memory then clear AC, ZP 0164", // op 3164
	"DCA 0165   ;Deposit AC to memory then clear AC, ZP 0165", // op 3165
	"DCA 0166   ;Deposit AC to memory then clear AC, ZP 0166", // op 3166
	"DCA 0167   ;Deposit AC to memory then clear AC, ZP 0167", // op 3167
	"DCA 0170   ;Deposit AC to memory then clear AC, ZP 0170", // op 3170
	"DCA 0171   ;Deposit AC to memory then clear AC, ZP 0171", // op 3171
	"DCA 0172   ;Deposit AC to memory then clear AC, ZP 0172", // op 3172
	"DCA 0173   ;Deposit AC to memory then clear AC, ZP 0173", // op 3173
	"DCA 0174   ;Deposit AC to memory then clear AC, ZP 0174", // op 3174
	"DCA 0175   ;Deposit AC to memory then clear AC, ZP 0175", // op 3175
	"DCA 0176   ;Deposit AC to memory then clear AC, ZP 0176", // op 3176
	"DCA 0177   ;Deposit AC to memory then clear AC, ZP 0177", // op 3177
	"DCA @@00   ;Deposit AC to memory then clear AC, Current page @@00", // op 3200
	"DCA @@01   ;Deposit AC to memory then clear AC, Current page @@01", // op 3201
	"DCA @@02   ;Deposit AC to memory then clear AC, Current page @@02", // op 3202
	"DCA @@03   ;Deposit AC to memory then clear AC, Current page @@03", // op 3203
	"DCA @@04   ;Deposit AC to memory then clear AC, Current page @@04", // op 3204
	"DCA @@05   ;Deposit AC to memory then clear AC, Current page @@05", // op 3205
	"DCA @@06   ;Deposit AC to memory then clear AC, Current page @@06", // op 3206
	"DCA @@07   ;Deposit AC to memory then clear AC, Current page @@07", // op 3207
	"DCA @@10   ;Deposit AC to memory then clear AC, Current page @@10", // op 3210
	"DCA @@11   ;Deposit AC to memory then clear AC, Current page @@11", // op 3211
	"DCA @@12   ;Deposit AC to memory then clear AC, Current page @@12", // op 3212
	"DCA @@13   ;Deposit AC to memory then clear AC, Current page @@13", // op 3213
	"DCA @@14   ;Deposit AC to memory then clear AC, Current page @@14", // op 3214
	"DCA @@15   ;Deposit AC to memory then clear AC, Current page @@15", // op 3215
	"DCA @@16   ;Deposit AC to memory then clear AC, Current page @@16", // op 3216
	"DCA @@17   ;Deposit AC to memory then clear AC, Current page @@17", // op 3217
	"DCA @@20   ;Deposit AC to memory then clear AC, Current page @@20", // op 3220
	"DCA @@21   ;Deposit AC to memory then clear AC, Current page @@21", // op 3221
	"DCA @@22   ;Deposit AC to memory then clear AC, Current page @@22", // op 3222
	"DCA @@23   ;Deposit AC to memory then clear AC, Current page @@23", // op 3223
	"DCA @@24   ;Deposit AC to memory then clear AC, Current page @@24", // op 3224
	"DCA @@25   ;Deposit AC to memory then clear AC, Current page @@25", // op 3225
	"DCA @@26   ;Deposit AC to memory then clear AC, Current page @@26", // op 3226
	"DCA @@27   ;Deposit AC to memory then clear AC, Current page @@27", // op 3227
	"DCA @@30   ;Deposit AC to memory then clear AC, Current page @@30", // op 3230
	"DCA @@31   ;Deposit AC to memory then clear AC, Current page @@31", // op 3231
	"DCA @@32   ;Deposit AC to memory then clear AC, Current page @@32", // op 3232
	"DCA @@33   ;Deposit AC to memory then clear AC, Current page @@33", // op 3233
	"DCA @@34   ;Deposit AC to memory then clear AC, Current page @@34", // op 3234
	"DCA @@35   ;Deposit AC to memory then clear AC, Current page @@35", // op 3235
	"DCA @@36   ;Deposit AC to memory then clear AC, Current page @@36", // op 3236
	"DCA @@37   ;Deposit AC to memory then clear AC, Current page @@37", // op 3237
	"DCA @@40   ;Deposit AC to memory then clear AC, Current page @@40", // op 3240
	"DCA @@41   ;Deposit AC to memory then clear AC, Current page @@41", // op 3241
	"DCA @@42   ;Deposit AC to memory then clear AC, Current page @@42", // op 3242
	"DCA @@43   ;Deposit AC to memory then clear AC, Current page @@43", // op 3243
	"DCA @@44   ;Deposit AC to memory then clear AC, Current page @@44", // op 3244
	"DCA @@45   ;Deposit AC to memory then clear AC, Current page @@45", // op 3245
	"DCA @@46   ;Deposit AC to memory then clear AC, Current page @@46", // op 3246
	"DCA @@47   ;Deposit AC to memory then clear AC, Current page @@47", // op 3247
	"DCA @@50   ;Deposit AC to memory then clear AC, Current page @@50", // op 3250
	"DCA @@51   ;Deposit AC to memory then clear AC, Current page @@51", // op 3251
	"DCA @@52   ;Deposit AC to memory then clear AC, Current page @@52", // op 3252
	"DCA @@53   ;Deposit AC to memory then clear AC, Current page @@53", // op 3253
	"DCA @@54   ;Deposit AC to memory then clear AC, Current page @@54", // op 3254
	"DCA @@55   ;Deposit AC to memory then clear AC, Current page @@55", // op 3255
	"DCA @@56   ;Deposit AC to memory then clear AC, Current page @@56", // op 3256
	"DCA @@57   ;Deposit AC to memory then clear AC, Current page @@57", // op 3257
	"DCA @@60   ;Deposit AC to memory then clear AC, Current page @@60", // op 3260
	"DCA @@61   ;Deposit AC to memory then clear AC, Current page @@61", // op 3261
	"DCA @@62   ;Deposit AC to memory then clear AC, Current page @@62", // op 3262
	"DCA @@63   ;Deposit AC to memory then clear AC, Current page @@63", // op 3263
	"DCA @@64   ;Deposit AC to memory then clear AC, Current page @@64", // op 3264
	"DCA @@65   ;Deposit AC to memory then clear AC, Current page @@65", // op 3265
	"DCA @@66   ;Deposit AC to memory then clear AC, Current page @@66", // op 3266
	"DCA @@67   ;Deposit AC to memory then clear AC, Current page @@67", // op 3267
	"DCA @@70   ;Deposit AC to memory then clear AC, Current page @@70", // op 3270
	"DCA @@71   ;Deposit AC to memory then clear AC, Current page @@71", // op 3271
	"DCA @@72   ;Deposit AC to memory then clear AC, Current page @@72", // op 3272
	"DCA @@73   ;Deposit AC to memory then clear AC, Current page @@73", // op 3273
	"DCA @@74   ;Deposit AC to memory then clear AC, Current page @@74", // op 3274
	"DCA @@75   ;Deposit AC to memory then clear AC, Current page @@75", // op 3275
	"DCA @@76   ;Deposit AC to memory then clear AC, Current page @@76", // op 3276
	"DCA @@77   ;Deposit AC to memory then clear AC, Current page @@77", // op 3277
	"DCA @@00   ;Deposit AC to memory then clear AC, Current page @@00", // op 3300
	"DCA @@01   ;Deposit AC to memory then clear AC, Current page @@01", // op 3301
	"DCA @@02   ;Deposit AC to memory then clear AC, Current page @@02", // op 3302
	"DCA @@03   ;Deposit AC to memory then clear AC, Current page @@03", // op 3303
	"DCA @@04   ;Deposit AC to memory then clear AC, Current page @@04", // op 3304
	"DCA @@05   ;Deposit AC to memory then clear AC, Current page @@05", // op 3305
	"DCA @@06   ;Deposit AC to memory then clear AC, Current page @@06", // op 3306
	"DCA @@07   ;Deposit AC to memory then clear AC, Current page @@07", // op 3307
	"DCA @@10   ;Deposit AC to memory then clear AC, Current page @@10", // op 3310
	"DCA @@11   ;Deposit AC to memory then clear AC, Current page @@11", // op 3311
	"DCA @@12   ;Deposit AC to memory then clear AC, Current page @@12", // op 3312
	"DCA @@13   ;Deposit AC to memory then clear AC, Current page @@13", // op 3313
	"DCA @@14   ;Deposit AC to memory then clear AC, Current page @@14", // op 3314
	"DCA @@15   ;Deposit AC to memory then clear AC, Current page @@15", // op 3315
	"DCA @@16   ;Deposit AC to memory then clear AC, Current page @@16", // op 3316
	"DCA @@17   ;Deposit AC to memory then clear AC, Current page @@17", // op 3317
	"DCA @@20   ;Deposit AC to memory then clear AC, Current page @@20", // op 3320
	"DCA @@21   ;Deposit AC to memory then clear AC, Current page @@21", // op 3321
	"DCA @@22   ;Deposit AC to memory then clear AC, Current page @@22", // op 3322
	"DCA @@23   ;Deposit AC to memory then clear AC, Current page @@23", // op 3323
	"DCA @@24   ;Deposit AC to memory then clear AC, Current page @@24", // op 3324
	"DCA @@25   ;Deposit AC to memory then clear AC, Current page @@25", // op 3325
	"DCA @@26   ;Deposit AC to memory then clear AC, Current page @@26", // op 3326
	"DCA @@27   ;Deposit AC to memory then clear AC, Current page @@27", // op 3327
	"DCA @@30   ;Deposit AC to memory then clear AC, Current page @@30", // op 3330
	"DCA @@31   ;Deposit AC to memory then clear AC, Current page @@31", // op 3331
	"DCA @@32   ;Deposit AC to memory then clear AC, Current page @@32", // op 3332
	"DCA @@33   ;Deposit AC to memory then clear AC, Current page @@33", // op 3333
	"DCA @@34   ;Deposit AC to memory then clear AC, Current page @@34", // op 3334
	"DCA @@35   ;Deposit AC to memory then clear AC, Current page @@35", // op 3335
	"DCA @@36   ;Deposit AC to memory then clear AC, Current page @@36", // op 3336
	"DCA @@37   ;Deposit AC to memory then clear AC, Current page @@37", // op 3337
	"DCA @@40   ;Deposit AC to memory then clear AC, Current page @@40", // op 3340
	"DCA @@41   ;Deposit AC to memory then clear AC, Current page @@41", // op 3341
	"DCA @@42   ;Deposit AC to memory then clear AC, Current page @@42", // op 3342
	"DCA @@43   ;Deposit AC to memory then clear AC, Current page @@43", // op 3343
	"DCA @@44   ;Deposit AC to memory then clear AC, Current page @@44", // op 3344
	"DCA @@45   ;Deposit AC to memory then clear AC, Current page @@45", // op 3345
	"DCA @@46   ;Deposit AC to memory then clear AC, Current page @@46", // op 3346
	"DCA @@47   ;Deposit AC to memory then clear AC, Current page @@47", // op 3347
	"DCA @@50   ;Deposit AC to memory then clear AC, Current page @@50", // op 3350
	"DCA @@51   ;Deposit AC to memory then clear AC, Current page @@51", // op 3351
	"DCA @@52   ;Deposit AC to memory then clear AC, Current page @@52", // op 3352
	"DCA @@53   ;Deposit AC to memory then clear AC, Current page @@53", // op 3353
	"DCA @@54   ;Deposit AC to memory then clear AC, Current page @@54", // op 3354
	"DCA @@55   ;Deposit AC to memory then clear AC, Current page @@55", // op 3355
	"DCA @@56   ;Deposit AC to memory then clear AC, Current page @@56", // op 3356
	"DCA @@57   ;Deposit AC to memory then clear AC, Current page @@57", // op 3357
	"DCA @@60   ;Deposit AC to memory then clear AC, Current page @@60", // op 3360
	"DCA @@61   ;Deposit AC to memory then clear AC, Current page @@61", // op 3361
	"DCA @@62   ;Deposit AC to memory then clear AC, Current page @@62", // op 3362
	"DCA @@63   ;Deposit AC to memory then clear AC, Current page @@63", // op 3363
	"DCA @@64   ;Deposit AC to memory then clear AC, Current page @@64", // op 3364
	"DCA @@65   ;Deposit AC to memory then clear AC, Current page @@65", // op 3365
	"DCA @@66   ;Deposit AC to memory then clear AC, Current page @@66", // op 3366
	"DCA @@67   ;Deposit AC to memory then clear AC, Current page @@67", // op 3367
	"DCA @@70   ;Deposit AC to memory then clear AC, Current page @@70", // op 3370
	"DCA @@71   ;Deposit AC to memory then clear AC, Current page @@71", // op 3371
	"DCA @@72   ;Deposit AC to memory then clear AC, Current page @@72", // op 3372
	"DCA @@73   ;Deposit AC to memory then clear AC, Current page @@73", // op 3373
	"DCA @@74   ;Deposit AC to memory then clear AC, Current page @@74", // op 3374
	"DCA @@75   ;Deposit AC to memory then clear AC, Current page @@75", // op 3375
	"DCA @@76   ;Deposit AC to memory then clear AC, Current page @@76", // op 3376
	"DCA @@77   ;Deposit AC to memory then clear AC, Current page @@77", // op 3377
	"DCA I 0000 ;Deposit AC to memory then clear AC, Indexed ZP 0000", // op 3400
	"DCA I 0001 ;Deposit AC to memory then clear AC, Indexed ZP 0001", // op 3401
	"DCA I 0002 ;Deposit AC to memory then clear AC, Indexed ZP 0002", // op 3402
	"DCA I 0003 ;Deposit AC to memory then clear AC, Indexed ZP 0003", // op 3403
	"DCA I 0004 ;Deposit AC to memory then clear AC, Indexed ZP 0004", // op 3404
	"DCA I 0005 ;Deposit AC to memory then clear AC, Indexed ZP 0005", // op 3405
	"DCA I 0006 ;Deposit AC to memory then clear AC, Indexed ZP 0006", // op 3406
	"DCA I 0007 ;Deposit AC to memory then clear AC, Indexed ZP 0007", // op 3407
	"DCA I 0010 ;Deposit AC to memory then clear AC, Indexed ZP 0010 [Auto pre-inc]", // op 3410
	"DCA I 0011 ;Deposit AC to memory then clear AC, Indexed ZP 0011 [Auto pre-inc]", // op 3411
	"DCA I 0012 ;Deposit AC to memory then clear AC, Indexed ZP 0012 [Auto pre-inc]", // op 3412
	"DCA I 0013 ;Deposit AC to memory then clear AC, Indexed ZP 0013 [Auto pre-inc]", // op 3413
	"DCA I 0014 ;Deposit AC to memory then clear AC, Indexed ZP 0014 [Auto pre-inc]", // op 3414
	"DCA I 0015 ;Deposit AC to memory then clear AC, Indexed ZP 0015 [Auto pre-inc]", // op 3415
	"DCA I 0016 ;Deposit AC to memory then clear AC, Indexed ZP 0016 [Auto pre-inc]", // op 3416
	"DCA I 0017 ;Deposit AC to memory then clear AC, Indexed ZP 0017 [Auto pre-inc]", // op 3417
	"DCA I 0020 ;Deposit AC to memory then clear AC, Indexed ZP 0020", // op 3420
	"DCA I 0021 ;Deposit AC to memory then clear AC, Indexed ZP 0021", // op 3421
	"DCA I 0022 ;Deposit AC to memory then clear AC, Indexed ZP 0022", // op 3422
	"DCA I 0023 ;Deposit AC to memory then clear AC, Indexed ZP 0023", // op 3423
	"DCA I 0024 ;Deposit AC to memory then clear AC, Indexed ZP 0024", // op 3424
	"DCA I 0025 ;Deposit AC to memory then clear AC, Indexed ZP 0025", // op 3425
	"DCA I 0026 ;Deposit AC to memory then clear AC, Indexed ZP 0026", // op 3426
	"DCA I 0027 ;Deposit AC to memory then clear AC, Indexed ZP 0027", // op 3427
	"DCA I 0030 ;Deposit AC to memory then clear AC, Indexed ZP 0030", // op 3430
	"DCA I 0031 ;Deposit AC to memory then clear AC, Indexed ZP 0031", // op 3431
	"DCA I 0032 ;Deposit AC to memory then clear AC, Indexed ZP 0032", // op 3432
	"DCA I 0033 ;Deposit AC to memory then clear AC, Indexed ZP 0033", // op 3433
	"DCA I 0034 ;Deposit AC to memory then clear AC, Indexed ZP 0034", // op 3434
	"DCA I 0035 ;Deposit AC to memory then clear AC, Indexed ZP 0035", // op 3435
	"DCA I 0036 ;Deposit AC to memory then clear AC, Indexed ZP 0036", // op 3436
	"DCA I 0037 ;Deposit AC to memory then clear AC, Indexed ZP 0037", // op 3437
	"DCA I 0040 ;Deposit AC to memory then clear AC, Indexed ZP 0040", // op 3440
	"DCA I 0041 ;Deposit AC to memory then clear AC, Indexed ZP 0041", // op 3441
	"DCA I 0042 ;Deposit AC to memory then clear AC, Indexed ZP 0042", // op 3442
	"DCA I 0043 ;Deposit AC to memory then clear AC, Indexed ZP 0043", // op 3443
	"DCA I 0044 ;Deposit AC to memory then clear AC, Indexed ZP 0044", // op 3444
	"DCA I 0045 ;Deposit AC to memory then clear AC, Indexed ZP 0045", // op 3445
	"DCA I 0046 ;Deposit AC to memory then clear AC, Indexed ZP 0046", // op 3446
	"DCA I 0047 ;Deposit AC to memory then clear AC, Indexed ZP 0047", // op 3447
	"DCA I 0050 ;Deposit AC to memory then clear AC, Indexed ZP 0050", // op 3450
	"DCA I 0051 ;Deposit AC to memory then clear AC, Indexed ZP 0051", // op 3451
	"DCA I 0052 ;Deposit AC to memory then clear AC, Indexed ZP 0052", // op 3452
	"DCA I 0053 ;Deposit AC to memory then clear AC, Indexed ZP 0053", // op 3453
	"DCA I 0054 ;Deposit AC to memory then clear AC, Indexed ZP 0054", // op 3454
	"DCA I 0055 ;Deposit AC to memory then clear AC, Indexed ZP 0055", // op 3455
	"DCA I 0056 ;Deposit AC to memory then clear AC, Indexed ZP 0056", // op 3456
	"DCA I 0057 ;Deposit AC to memory then clear AC, Indexed ZP 0057", // op 3457
	"DCA I 0060 ;Deposit AC to memory then clear AC, Indexed ZP 0060", // op 3460
	"DCA I 0061 ;Deposit AC to memory then clear AC, Indexed ZP 0061", // op 3461
	"DCA I 0062 ;Deposit AC to memory then clear AC, Indexed ZP 0062", // op 3462
	"DCA I 0063 ;Deposit AC to memory then clear AC, Indexed ZP 0063", // op 3463
	"DCA I 0064 ;Deposit AC to memory then clear AC, Indexed ZP 0064", // op 3464
	"DCA I 0065 ;Deposit AC to memory then clear AC, Indexed ZP 0065", // op 3465
	"DCA I 0066 ;Deposit AC to memory then clear AC, Indexed ZP 0066", // op 3466
	"DCA I 0067 ;Deposit AC to memory then clear AC, Indexed ZP 0067", // op 3467
	"DCA I 0070 ;Deposit AC to memory then clear AC, Indexed ZP 0070", // op 3470
	"DCA I 0071 ;Deposit AC to memory then clear AC, Indexed ZP 0071", // op 3471
	"DCA I 0072 ;Deposit AC to memory then clear AC, Indexed ZP 0072", // op 3472
	"DCA I 0073 ;Deposit AC to memory then clear AC, Indexed ZP 0073", // op 3473
	"DCA I 0074 ;Deposit AC to memory then clear AC, Indexed ZP 0074", // op 3474
	"DCA I 0075 ;Deposit AC to memory then clear AC, Indexed ZP 0075", // op 3475
	"DCA I 0076 ;Deposit AC to memory then clear AC, Indexed ZP 0076", // op 3476
	"DCA I 0077 ;Deposit AC to memory then clear AC, Indexed ZP 0077", // op 3477
	"DCA I 0100 ;Deposit AC to memory then clear AC, Indexed ZP 0100", // op 3500
	"DCA I 0101 ;Deposit AC to memory then clear AC, Indexed ZP 0101", // op 3501
	"DCA I 0102 ;Deposit AC to memory then clear AC, Indexed ZP 0102", // op 3502
	"DCA I 0103 ;Deposit AC to memory then clear AC, Indexed ZP 0103", // op 3503
	"DCA I 0104 ;Deposit AC to memory then clear AC, Indexed ZP 0104", // op 3504
	"DCA I 0105 ;Deposit AC to memory then clear AC, Indexed ZP 0105", // op 3505
	"DCA I 0106 ;Deposit AC to memory then clear AC, Indexed ZP 0106", // op 3506
	"DCA I 0107 ;Deposit AC to memory then clear AC, Indexed ZP 0107", // op 3507
	"DCA I 0110 ;Deposit AC to memory then clear AC, Indexed ZP 0110", // op 3510
	"DCA I 0111 ;Deposit AC to memory then clear AC, Indexed ZP 0111", // op 3511
	"DCA I 0112 ;Deposit AC to memory then clear AC, Indexed ZP 0112", // op 3512
	"DCA I 0113 ;Deposit AC to memory then clear AC, Indexed ZP 0113", // op 3513
	"DCA I 0114 ;Deposit AC to memory then clear AC, Indexed ZP 0114", // op 3514
	"DCA I 0115 ;Deposit AC to memory then clear AC, Indexed ZP 0115", // op 3515
	"DCA I 0116 ;Deposit AC to memory then clear AC, Indexed ZP 0116", // op 3516
	"DCA I 0117 ;Deposit AC to memory then clear AC, Indexed ZP 0117", // op 3517
	"DCA I 0120 ;Deposit AC to memory then clear AC, Indexed ZP 0120", // op 3520
	"DCA I 0121 ;Deposit AC to memory then clear AC, Indexed ZP 0121", // op 3521
	"DCA I 0122 ;Deposit AC to memory then clear AC, Indexed ZP 0122", // op 3522
	"DCA I 0123 ;Deposit AC to memory then clear AC, Indexed ZP 0123", // op 3523
	"DCA I 0124 ;Deposit AC to memory then clear AC, Indexed ZP 0124", // op 3524
	"DCA I 0125 ;Deposit AC to memory then clear AC, Indexed ZP 0125", // op 3525
	"DCA I 0126 ;Deposit AC to memory then clear AC, Indexed ZP 0126", // op 3526
	"DCA I 0127 ;Deposit AC to memory then clear AC, Indexed ZP 0127", // op 3527
	"DCA I 0130 ;Deposit AC to memory then clear AC, Indexed ZP 0130", // op 3530
	"DCA I 0131 ;Deposit AC to memory then clear AC, Indexed ZP 0131", // op 3531
	"DCA I 0132 ;Deposit AC to memory then clear AC, Indexed ZP 0132", // op 3532
	"DCA I 0133 ;Deposit AC to memory then clear AC, Indexed ZP 0133", // op 3533
	"DCA I 0134 ;Deposit AC to memory then clear AC, Indexed ZP 0134", // op 3534
	"DCA I 0135 ;Deposit AC to memory then clear AC, Indexed ZP 0135", // op 3535
	"DCA I 0136 ;Deposit AC to memory then clear AC, Indexed ZP 0136", // op 3536
	"DCA I 0137 ;Deposit AC to memory then clear AC, Indexed ZP 0137", // op 3537
	"DCA I 0140 ;Deposit AC to memory then clear AC, Indexed ZP 0140", // op 3540
	"DCA I 0141 ;Deposit AC to memory then clear AC, Indexed ZP 0141", // op 3541
	"DCA I 0142 ;Deposit AC to memory then clear AC, Indexed ZP 0142", // op 3542
	"DCA I 0143 ;Deposit AC to memory then clear AC, Indexed ZP 0143", // op 3543
	"DCA I 0144 ;Deposit AC to memory then clear AC, Indexed ZP 0144", // op 3544
	"DCA I 0145 ;Deposit AC to memory then clear AC, Indexed ZP 0145", // op 3545
	"DCA I 0146 ;Deposit AC to memory then clear AC, Indexed ZP 0146", // op 3546
	"DCA I 0147 ;Deposit AC to memory then clear AC, Indexed ZP 0147", // op 3547
	"DCA I 0150 ;Deposit AC to memory then clear AC, Indexed ZP 0150", // op 3550
	"DCA I 0151 ;Deposit AC to memory then clear AC, Indexed ZP 0151", // op 3551
	"DCA I 0152 ;Deposit AC to memory then clear AC, Indexed ZP 0152", // op 3552
	"DCA I 0153 ;Deposit AC to memory then clear AC, Indexed ZP 0153", // op 3553
	"DCA I 0154 ;Deposit AC to memory then clear AC, Indexed ZP 0154", // op 3554
	"DCA I 0155 ;Deposit AC to memory then clear AC, Indexed ZP 0155", // op 3555
	"DCA I 0156 ;Deposit AC to memory then clear AC, Indexed ZP 0156", // op 3556
	"DCA I 0157 ;Deposit AC to memory then clear AC, Indexed ZP 0157", // op 3557
	"DCA I 0160 ;Deposit AC to memory then clear AC, Indexed ZP 0160", // op 3560
	"DCA I 0161 ;Deposit AC to memory then clear AC, Indexed ZP 0161", // op 3561
	"DCA I 0162 ;Deposit AC to memory then clear AC, Indexed ZP 0162", // op 3562
	"DCA I 0163 ;Deposit AC to memory then clear AC, Indexed ZP 0163", // op 3563
	"DCA I 0164 ;Deposit AC to memory then clear AC, Indexed ZP 0164", // op 3564
	"DCA I 0165 ;Deposit AC to memory then clear AC, Indexed ZP 0165", // op 3565
	"DCA I 0166 ;Deposit AC to memory then clear AC, Indexed ZP 0166", // op 3566
	"DCA I 0167 ;Deposit AC to memory then clear AC, Indexed ZP 0167", // op 3567
	"DCA I 0170 ;Deposit AC to memory then clear AC, Indexed ZP 0170", // op 3570
	"DCA I 0171 ;Deposit AC to memory then clear AC, Indexed ZP 0171", // op 3571
	"DCA I 0172 ;Deposit AC to memory then clear AC, Indexed ZP 0172", // op 3572
	"DCA I 0173 ;Deposit AC to memory then clear AC, Indexed ZP 0173", // op 3573
	"DCA I 0174 ;Deposit AC to memory then clear AC, Indexed ZP 0174", // op 3574
	"DCA I 0175 ;Deposit AC to memory then clear AC, Indexed ZP 0175", // op 3575
	"DCA I 0176 ;Deposit AC to memory then clear AC, Indexed ZP 0176", // op 3576
	"DCA I 0177 ;Deposit AC to memory then clear AC, Indexed ZP 0177", // op 3577
	"DCA I @@00 ;Deposit AC to memory then clear AC, Indexed Current page @@00", // op 3600
	"DCA I @@01 ;Deposit AC to memory then clear AC, Indexed Current page @@01", // op 3601
	"DCA I @@02 ;Deposit AC to memory then clear AC, Indexed Current page @@02", // op 3602
	"DCA I @@03 ;Deposit AC to memory then clear AC, Indexed Current page @@03", // op 3603
	"DCA I @@04 ;Deposit AC to memory then clear AC, Indexed Current page @@04", // op 3604
	"DCA I @@05 ;Deposit AC to memory then clear AC, Indexed Current page @@05", // op 3605
	"DCA I @@06 ;Deposit AC to memory then clear AC, Indexed Current page @@06", // op 3606
	"DCA I @@07 ;Deposit AC to memory then clear AC, Indexed Current page @@07", // op 3607
	"DCA I @@10 ;Deposit AC to memory then clear AC, Indexed Current page @@10", // op 3610
	"DCA I @@11 ;Deposit AC to memory then clear AC, Indexed Current page @@11", // op 3611
	"DCA I @@12 ;Deposit AC to memory then clear AC, Indexed Current page @@12", // op 3612
	"DCA I @@13 ;Deposit AC to memory then clear AC, Indexed Current page @@13", // op 3613
	"DCA I @@14 ;Deposit AC to memory then clear AC, Indexed Current page @@14", // op 3614
	"DCA I @@15 ;Deposit AC to memory then clear AC, Indexed Current page @@15", // op 3615
	"DCA I @@16 ;Deposit AC to memory then clear AC, Indexed Current page @@16", // op 3616
	"DCA I @@17 ;Deposit AC to memory then clear AC, Indexed Current page @@17", // op 3617
	"DCA I @@20 ;Deposit AC to memory then clear AC, Indexed Current page @@20", // op 3620
	"DCA I @@21 ;Deposit AC to memory then clear AC, Indexed Current page @@21", // op 3621
	"DCA I @@22 ;Deposit AC to memory then clear AC, Indexed Current page @@22", // op 3622
	"DCA I @@23 ;Deposit AC to memory then clear AC, Indexed Current page @@23", // op 3623
	"DCA I @@24 ;Deposit AC to memory then clear AC, Indexed Current page @@24", // op 3624
	"DCA I @@25 ;Deposit AC to memory then clear AC, Indexed Current page @@25", // op 3625
	"DCA I @@26 ;Deposit AC to memory then clear AC, Indexed Current page @@26", // op 3626
	"DCA I @@27 ;Deposit AC to memory then clear AC, Indexed Current page @@27", // op 3627
	"DCA I @@30 ;Deposit AC to memory then clear AC, Indexed Current page @@30", // op 3630
	"DCA I @@31 ;Deposit AC to memory then clear AC, Indexed Current page @@31", // op 3631
	"DCA I @@32 ;Deposit AC to memory then clear AC, Indexed Current page @@32", // op 3632
	"DCA I @@33 ;Deposit AC to memory then clear AC, Indexed Current page @@33", // op 3633
	"DCA I @@34 ;Deposit AC to memory then clear AC, Indexed Current page @@34", // op 3634
	"DCA I @@35 ;Deposit AC to memory then clear AC, Indexed Current page @@35", // op 3635
	"DCA I @@36 ;Deposit AC to memory then clear AC, Indexed Current page @@36", // op 3636
	"DCA I @@37 ;Deposit AC to memory then clear AC, Indexed Current page @@37", // op 3637
	"DCA I @@40 ;Deposit AC to memory then clear AC, Indexed Current page @@40", // op 3640
	"DCA I @@41 ;Deposit AC to memory then clear AC, Indexed Current page @@41", // op 3641
	"DCA I @@42 ;Deposit AC to memory then clear AC, Indexed Current page @@42", // op 3642
	"DCA I @@43 ;Deposit AC to memory then clear AC, Indexed Current page @@43", // op 3643
	"DCA I @@44 ;Deposit AC to memory then clear AC, Indexed Current page @@44", // op 3644
	"DCA I @@45 ;Deposit AC to memory then clear AC, Indexed Current page @@45", // op 3645
	"DCA I @@46 ;Deposit AC to memory then clear AC, Indexed Current page @@46", // op 3646
	"DCA I @@47 ;Deposit AC to memory then clear AC, Indexed Current page @@47", // op 3647
	"DCA I @@50 ;Deposit AC to memory then clear AC, Indexed Current page @@50", // op 3650
	"DCA I @@51 ;Deposit AC to memory then clear AC, Indexed Current page @@51", // op 3651
	"DCA I @@52 ;Deposit AC to memory then clear AC, Indexed Current page @@52", // op 3652
	"DCA I @@53 ;Deposit AC to memory then clear AC, Indexed Current page @@53", // op 3653
	"DCA I @@54 ;Deposit AC to memory then clear AC, Indexed Current page @@54", // op 3654
	"DCA I @@55 ;Deposit AC to memory then clear AC, Indexed Current page @@55", // op 3655
	"DCA I @@56 ;Deposit AC to memory then clear AC, Indexed Current page @@56", // op 3656
	"DCA I @@57 ;Deposit AC to memory then clear AC, Indexed Current page @@57", // op 3657
	"DCA I @@60 ;Deposit AC to memory then clear AC, Indexed Current page @@60", // op 3660
	"DCA I @@61 ;Deposit AC to memory then clear AC, Indexed Current page @@61", // op 3661
	"DCA I @@62 ;Deposit AC to memory then clear AC, Indexed Current page @@62", // op 3662
	"DCA I @@63 ;Deposit AC to memory then clear AC, Indexed Current page @@63", // op 3663
	"DCA I @@64 ;Deposit AC to memory then clear AC, Indexed Current page @@64", // op 3664
	"DCA I @@65 ;Deposit AC to memory then clear AC, Indexed Current page @@65", // op 3665
	"DCA I @@66 ;Deposit AC to memory then clear AC, Indexed Current page @@66", // op 3666
	"DCA I @@67 ;Deposit AC to memory then clear AC, Indexed Current page @@67", // op 3667
	"DCA I @@70 ;Deposit AC to memory then clear AC, Indexed Current page @@70", // op 3670
	"DCA I @@71 ;Deposit AC to memory then clear AC, Indexed Current page @@71", // op 3671
	"DCA I @@72 ;Deposit AC to memory then clear AC, Indexed Current page @@72", // op 3672
	"DCA I @@73 ;Deposit AC to memory then clear AC, Indexed Current page @@73", // op 3673
	"DCA I @@74 ;Deposit AC to memory then clear AC, Indexed Current page @@74", // op 3674
	"DCA I @@75 ;Deposit AC to memory then clear AC, Indexed Current page @@75", // op 3675
	"DCA I @@76 ;Deposit AC to memory then clear AC, Indexed Current page @@76", // op 3676
	"DCA I @@77 ;Deposit AC to memory then clear AC, Indexed Current page @@77", // op 3677
	"DCA I @@00 ;Deposit AC to memory then clear AC, Indexed Current page @@00", // op 3700
	"DCA I @@01 ;Deposit AC to memory then clear AC, Indexed Current page @@01", // op 3701
	"DCA I @@02 ;Deposit AC to memory then clear AC, Indexed Current page @@02", // op 3702
	"DCA I @@03 ;Deposit AC to memory then clear AC, Indexed Current page @@03", // op 3703
	"DCA I @@04 ;Deposit AC to memory then clear AC, Indexed Current page @@04", // op 3704
	"DCA I @@05 ;Deposit AC to memory then clear AC, Indexed Current page @@05", // op 3705
	"DCA I @@06 ;Deposit AC to memory then clear AC, Indexed Current page @@06", // op 3706
	"DCA I @@07 ;Deposit AC to memory then clear AC, Indexed Current page @@07", // op 3707
	"DCA I @@10 ;Deposit AC to memory then clear AC, Indexed Current page @@10", // op 3710
	"DCA I @@11 ;Deposit AC to memory then clear AC, Indexed Current page @@11", // op 3711
	"DCA I @@12 ;Deposit AC to memory then clear AC, Indexed Current page @@12", // op 3712
	"DCA I @@13 ;Deposit AC to memory then clear AC, Indexed Current page @@13", // op 3713
	"DCA I @@14 ;Deposit AC to memory then clear AC, Indexed Current page @@14", // op 3714
	"DCA I @@15 ;Deposit AC to memory then clear AC, Indexed Current page @@15", // op 3715
	"DCA I @@16 ;Deposit AC to memory then clear AC, Indexed Current page @@16", // op 3716
	"DCA I @@17 ;Deposit AC to memory then clear AC, Indexed Current page @@17", // op 3717
	"DCA I @@20 ;Deposit AC to memory then clear AC, Indexed Current page @@20", // op 3720
	"DCA I @@21 ;Deposit AC to memory then clear AC, Indexed Current page @@21", // op 3721
	"DCA I @@22 ;Deposit AC to memory then clear AC, Indexed Current page @@22", // op 3722
	"DCA I @@23 ;Deposit AC to memory then clear AC, Indexed Current page @@23", // op 3723
	"DCA I @@24 ;Deposit AC to memory then clear AC, Indexed Current page @@24", // op 3724
	"DCA I @@25 ;Deposit AC to memory then clear AC, Indexed Current page @@25", // op 3725
	"DCA I @@26 ;Deposit AC to memory then clear AC, Indexed Current page @@26", // op 3726
	"DCA I @@27 ;Deposit AC to memory then clear AC, Indexed Current page @@27", // op 3727
	"DCA I @@30 ;Deposit AC to memory then clear AC, Indexed Current page @@30", // op 3730
	"DCA I @@31 ;Deposit AC to memory then clear AC, Indexed Current page @@31", // op 3731
	"DCA I @@32 ;Deposit AC to memory then clear AC, Indexed Current page @@32", // op 3732
	"DCA I @@33 ;Deposit AC to memory then clear AC, Indexed Current page @@33", // op 3733
	"DCA I @@34 ;Deposit AC to memory then clear AC, Indexed Current page @@34", // op 3734
	"DCA I @@35 ;Deposit AC to memory then clear AC, Indexed Current page @@35", // op 3735
	"DCA I @@36 ;Deposit AC to memory then clear AC, Indexed Current page @@36", // op 3736
	"DCA I @@37 ;Deposit AC to memory then clear AC, Indexed Current page @@37", // op 3737
	"DCA I @@40 ;Deposit AC to memory then clear AC, Indexed Current page @@40", // op 3740
	"DCA I @@41 ;Deposit AC to memory then clear AC, Indexed Current page @@41", // op 3741
	"DCA I @@42 ;Deposit AC to memory then clear AC, Indexed Current page @@42", // op 3742
	"DCA I @@43 ;Deposit AC to memory then clear AC, Indexed Current page @@43", // op 3743
	"DCA I @@44 ;Deposit AC to memory then clear AC, Indexed Current page @@44", // op 3744
	"DCA I @@45 ;Deposit AC to memory then clear AC, Indexed Current page @@45", // op 3745
	"DCA I @@46 ;Deposit AC to memory then clear AC, Indexed Current page @@46", // op 3746
	"DCA I @@47 ;Deposit AC to memory then clear AC, Indexed Current page @@47", // op 3747
	"DCA I @@50 ;Deposit AC to memory then clear AC, Indexed Current page @@50", // op 3750
	"DCA I @@51 ;Deposit AC to memory then clear AC, Indexed Current page @@51", // op 3751
	"DCA I @@52 ;Deposit AC to memory then clear AC, Indexed Current page @@52", // op 3752
	"DCA I @@53 ;Deposit AC to memory then clear AC, Indexed Current page @@53", // op 3753
	"DCA I @@54 ;Deposit AC to memory then clear AC, Indexed Current page @@54", // op 3754
	"DCA I @@55 ;Deposit AC to memory then clear AC, Indexed Current page @@55", // op 3755
	"DCA I @@56 ;Deposit AC to memory then clear AC, Indexed Current page @@56", // op 3756
	"DCA I @@57 ;Deposit AC to memory then clear AC, Indexed Current page @@57", // op 3757
	"DCA I @@60 ;Deposit AC to memory then clear AC, Indexed Current page @@60", // op 3760
	"DCA I @@61 ;Deposit AC to memory then clear AC, Indexed Current page @@61", // op 3761
	"DCA I @@62 ;Deposit AC to memory then clear AC, Indexed Current page @@62", // op 3762
	"DCA I @@63 ;Deposit AC to memory then clear AC, Indexed Current page @@63", // op 3763
	"DCA I @@64 ;Deposit AC to memory then clear AC, Indexed Current page @@64", // op 3764
	"DCA I @@65 ;Deposit AC to memory then clear AC, Indexed Current page @@65", // op 3765
	"DCA I @@66 ;Deposit AC to memory then clear AC, Indexed Current page @@66", // op 3766
	"DCA I @@67 ;Deposit AC to memory then clear AC, Indexed Current page @@67", // op 3767
	"DCA I @@70 ;Deposit AC to memory then clear AC, Indexed Current page @@70", // op 3770
	"DCA I @@71 ;Deposit AC to memory then clear AC, Indexed Current page @@71", // op 3771
	"DCA I @@72 ;Deposit AC to memory then clear AC, Indexed Current page @@72", // op 3772
	"DCA I @@73 ;Deposit AC to memory then clear AC, Indexed Current page @@73", // op 3773
	"DCA I @@74 ;Deposit AC to memory then clear AC, Indexed Current page @@74", // op 3774
	"DCA I @@75 ;Deposit AC to memory then clear AC, Indexed Current page @@75", // op 3775
	"DCA I @@76 ;Deposit AC to memory then clear AC, Indexed Current page @@76", // op 3776
	"DCA I @@77 ;Deposit AC to memory then clear AC, Indexed Current page @@77", // op 3777
	"JMS 0000   ;Jump to subroutine ZP 0000 ", // op 4000
	"JMS 0001   ;Jump to subroutine ZP 0001", // op 4001
	"JMS 0002   ;Jump to subroutine ZP 0002", // op 4002
	"JMS 0003   ;Jump to subroutine ZP 0003", // op 4003
	"JMS 0004   ;Jump to subroutine ZP 0004", // op 4004
	"JMS 0005   ;Jump to subroutine ZP 0005", // op 4005
	"JMS 0006   ;Jump to subroutine ZP 0006", // op 4006
	"JMS 0007   ;Jump to subroutine ZP 0007", // op 4007
	"JMS 0010   ;Jump to subroutine ZP 0010", // op 4010
	"JMS 0011   ;Jump to subroutine ZP 0011", // op 4011
	"JMS 0012   ;Jump to subroutine ZP 0012", // op 4012
	"JMS 0013   ;Jump to subroutine ZP 0013", // op 4013
	"JMS 0014   ;Jump to subroutine ZP 0014", // op 4014
	"JMS 0015   ;Jump to subroutine ZP 0015", // op 4015
	"JMS 0016   ;Jump to subroutine ZP 0016", // op 4016
	"JMS 0017   ;Jump to subroutine ZP 0017", // op 4017
	"JMS 0020   ;Jump to subroutine ZP 0020", // op 4020
	"JMS 0021   ;Jump to subroutine ZP 0021", // op 4021
	"JMS 0022   ;Jump to subroutine ZP 0022", // op 4022
	"JMS 0023   ;Jump to subroutine ZP 0023", // op 4023
	"JMS 0024   ;Jump to subroutine ZP 0024", // op 4024
	"JMS 0025   ;Jump to subroutine ZP 0025", // op 4025
	"JMS 0026   ;Jump to subroutine ZP 0026", // op 4026
	"JMS 0027   ;Jump to subroutine ZP 0027", // op 4027
	"JMS 0030   ;Jump to subroutine ZP 0030", // op 4030
	"JMS 0031   ;Jump to subroutine ZP 0031", // op 4031
	"JMS 0032   ;Jump to subroutine ZP 0032", // op 4032
	"JMS 0033   ;Jump to subroutine ZP 0033", // op 4033
	"JMS 0034   ;Jump to subroutine ZP 0034", // op 4034
	"JMS 0035   ;Jump to subroutine ZP 0035", // op 4035
	"JMS 0036   ;Jump to subroutine ZP 0036", // op 4036
	"JMS 0037   ;Jump to subroutine ZP 0037", // op 4037
	"JMS 0040   ;Jump to subroutine ZP 0040", // op 4040
	"JMS 0041   ;Jump to subroutine ZP 0041", // op 4041
	"JMS 0042   ;Jump to subroutine ZP 0042", // op 4042
	"JMS 0043   ;Jump to subroutine ZP 0043", // op 4043
	"JMS 0044   ;Jump to subroutine ZP 0044", // op 4044
	"JMS 0045   ;Jump to subroutine ZP 0045", // op 4045
	"JMS 0046   ;Jump to subroutine ZP 0046", // op 4046
	"JMS 0047   ;Jump to subroutine ZP 0047", // op 4047
	"JMS 0050   ;Jump to subroutine ZP 0050", // op 4050
	"JMS 0051   ;Jump to subroutine ZP 0051", // op 4051
	"JMS 0052   ;Jump to subroutine ZP 0052", // op 4052
	"JMS 0053   ;Jump to subroutine ZP 0053", // op 4053
	"JMS 0054   ;Jump to subroutine ZP 0054", // op 4054
	"JMS 0055   ;Jump to subroutine ZP 0055", // op 4055
	"JMS 0056   ;Jump to subroutine ZP 0056", // op 4056
	"JMS 0057   ;Jump to subroutine ZP 0057", // op 4057
	"JMS 0060   ;Jump to subroutine ZP 0060", // op 4060
	"JMS 0061   ;Jump to subroutine ZP 0061", // op 4061
	"JMS 0062   ;Jump to subroutine ZP 0062", // op 4062
	"JMS 0063   ;Jump to subroutine ZP 0063", // op 4063
	"JMS 0064   ;Jump to subroutine ZP 0064", // op 4064
	"JMS 0065   ;Jump to subroutine ZP 0065", // op 4065
	"JMS 0066   ;Jump to subroutine ZP 0066", // op 4066
	"JMS 0067   ;Jump to subroutine ZP 0067", // op 4067
	"JMS 0070   ;Jump to subroutine ZP 0070", // op 4070
	"JMS 0071   ;Jump to subroutine ZP 0071", // op 4071
	"JMS 0072   ;Jump to subroutine ZP 0072", // op 4072
	"JMS 0073   ;Jump to subroutine ZP 0073", // op 4073
	"JMS 0074   ;Jump to subroutine ZP 0074", // op 4074
	"JMS 0075   ;Jump to subroutine ZP 0075", // op 4075
	"JMS 0076   ;Jump to subroutine ZP 0076", // op 4076
	"JMS 0077   ;Jump to subroutine ZP 0077", // op 4077
	"JMS 0100   ;Jump to subroutine ZP 0100", // op 4100
	"JMS 0101   ;Jump to subroutine ZP 0101", // op 4101
	"JMS 0102   ;Jump to subroutine ZP 0102", // op 4102
	"JMS 0103   ;Jump to subroutine ZP 0103", // op 4103
	"JMS 0104   ;Jump to subroutine ZP 0104", // op 4104
	"JMS 0105   ;Jump to subroutine ZP 0105", // op 4105
	"JMS 0106   ;Jump to subroutine ZP 0106", // op 4106
	"JMS 0107   ;Jump to subroutine ZP 0107", // op 4107
	"JMS 0110   ;Jump to subroutine ZP 0110", // op 4110
	"JMS 0111   ;Jump to subroutine ZP 0111", // op 4111
	"JMS 0112   ;Jump to subroutine ZP 0112", // op 4112
	"JMS 0113   ;Jump to subroutine ZP 0113", // op 4113
	"JMS 0114   ;Jump to subroutine ZP 0114", // op 4114
	"JMS 0115   ;Jump to subroutine ZP 0115", // op 4115
	"JMS 0116   ;Jump to subroutine ZP 0116", // op 4116
	"JMS 0117   ;Jump to subroutine ZP 0117", // op 4117
	"JMS 0120   ;Jump to subroutine ZP 0120", // op 4120
	"JMS 0121   ;Jump to subroutine ZP 0121", // op 4121
	"JMS 0122   ;Jump to subroutine ZP 0122", // op 4122
	"JMS 0123   ;Jump to subroutine ZP 0123", // op 4123
	"JMS 0124   ;Jump to subroutine ZP 0124", // op 4124
	"JMS 0125   ;Jump to subroutine ZP 0125", // op 4125
	"JMS 0126   ;Jump to subroutine ZP 0126", // op 4126
	"JMS 0127   ;Jump to subroutine ZP 0127", // op 4127
	"JMS 0130   ;Jump to subroutine ZP 0130", // op 4130
	"JMS 0131   ;Jump to subroutine ZP 0131", // op 4131
	"JMS 0132   ;Jump to subroutine ZP 0132", // op 4132
	"JMS 0133   ;Jump to subroutine ZP 0133", // op 4133
	"JMS 0134   ;Jump to subroutine ZP 0134", // op 4134
	"JMS 0135   ;Jump to subroutine ZP 0135", // op 4135
	"JMS 0136   ;Jump to subroutine ZP 0136", // op 4136
	"JMS 0137   ;Jump to subroutine ZP 0137", // op 4137
	"JMS 0140   ;Jump to subroutine ZP 0140", // op 4140
	"JMS 0141   ;Jump to subroutine ZP 0141", // op 4141
	"JMS 0142   ;Jump to subroutine ZP 0142", // op 4142
	"JMS 0143   ;Jump to subroutine ZP 0143", // op 4143
	"JMS 0144   ;Jump to subroutine ZP 0144", // op 4144
	"JMS 0145   ;Jump to subroutine ZP 0145", // op 4145
	"JMS 0146   ;Jump to subroutine ZP 0146", // op 4146
	"JMS 0147   ;Jump to subroutine ZP 0147", // op 4147
	"JMS 0150   ;Jump to subroutine ZP 0150", // op 4150
	"JMS 0151   ;Jump to subroutine ZP 0151", // op 4151
	"JMS 0152   ;Jump to subroutine ZP 0152", // op 4152
	"JMS 0153   ;Jump to subroutine ZP 0153", // op 4153
	"JMS 0154   ;Jump to subroutine ZP 0154", // op 4154
	"JMS 0155   ;Jump to subroutine ZP 0155", // op 4155
	"JMS 0156   ;Jump to subroutine ZP 0156", // op 4156
	"JMS 0157   ;Jump to subroutine ZP 0157", // op 4157
	"JMS 0160   ;Jump to subroutine ZP 0160", // op 4160
	"JMS 0161   ;Jump to subroutine ZP 0161", // op 4161
	"JMS 0162   ;Jump to subroutine ZP 0162", // op 4162
	"JMS 0163   ;Jump to subroutine ZP 0163", // op 4163
	"JMS 0164   ;Jump to subroutine ZP 0164", // op 4164
	"JMS 0165   ;Jump to subroutine ZP 0165", // op 4165
	"JMS 0166   ;Jump to subroutine ZP 0166", // op 4166
	"JMS 0167   ;Jump to subroutine ZP 0167", // op 4167
	"JMS 0170   ;Jump to subroutine ZP 0170", // op 4170
	"JMS 0171   ;Jump to subroutine ZP 0171", // op 4171
	"JMS 0172   ;Jump to subroutine ZP 0172", // op 4172
	"JMS 0173   ;Jump to subroutine ZP 0173", // op 4173
	"JMS 0174   ;Jump to subroutine ZP 0174", // op 4174
	"JMS 0175   ;Jump to subroutine ZP 0175", // op 4175
	"JMS 0176   ;Jump to subroutine ZP 0176", // op 4176
	"JMS 0177   ;Jump to subroutine ZP 0177", // op 4177
	"JMS @@00   ;Jump to subroutine Current page @@00", // op 4200
	"JMS @@01   ;Jump to subroutine Current page @@01", // op 4201
	"JMS @@02   ;Jump to subroutine Current page @@02", // op 4202
	"JMS @@03   ;Jump to subroutine Current page @@03", // op 4203
	"JMS @@04   ;Jump to subroutine Current page @@04", // op 4204
	"JMS @@05   ;Jump to subroutine Current page @@05", // op 4205
	"JMS @@06   ;Jump to subroutine Current page @@06", // op 4206
	"JMS @@07   ;Jump to subroutine Current page @@07", // op 4207
	"JMS @@10   ;Jump to subroutine Current page @@10", // op 4210
	"JMS @@11   ;Jump to subroutine Current page @@11", // op 4211
	"JMS @@12   ;Jump to subroutine Current page @@12", // op 4212
	"JMS @@13   ;Jump to subroutine Current page @@13", // op 4213
	"JMS @@14   ;Jump to subroutine Current page @@14", // op 4214
	"JMS @@15   ;Jump to subroutine Current page @@15", // op 4215
	"JMS @@16   ;Jump to subroutine Current page @@16", // op 4216
	"JMS @@17   ;Jump to subroutine Current page @@17", // op 4217
	"JMS @@20   ;Jump to subroutine Current page @@20", // op 4220
	"JMS @@21   ;Jump to subroutine Current page @@21", // op 4221
	"JMS @@22   ;Jump to subroutine Current page @@22", // op 4222
	"JMS @@23   ;Jump to subroutine Current page @@23", // op 4223
	"JMS @@24   ;Jump to subroutine Current page @@24", // op 4224
	"JMS @@25   ;Jump to subroutine Current page @@25", // op 4225
	"JMS @@26   ;Jump to subroutine Current page @@26", // op 4226
	"JMS @@27   ;Jump to subroutine Current page @@27", // op 4227
	"JMS @@30   ;Jump to subroutine Current page @@30", // op 4230
	"JMS @@31   ;Jump to subroutine Current page @@31", // op 4231
	"JMS @@32   ;Jump to subroutine Current page @@32", // op 4232
	"JMS @@33   ;Jump to subroutine Current page @@33", // op 4233
	"JMS @@34   ;Jump to subroutine Current page @@34", // op 4234
	"JMS @@35   ;Jump to subroutine Current page @@35", // op 4235
	"JMS @@36   ;Jump to subroutine Current page @@36", // op 4236
	"JMS @@37   ;Jump to subroutine Current page @@37", // op 4237
	"JMS @@40   ;Jump to subroutine Current page @@40", // op 4240
	"JMS @@41   ;Jump to subroutine Current page @@41", // op 4241
	"JMS @@42   ;Jump to subroutine Current page @@42", // op 4242
	"JMS @@43   ;Jump to subroutine Current page @@43", // op 4243
	"JMS @@44   ;Jump to subroutine Current page @@44", // op 4244
	"JMS @@45   ;Jump to subroutine Current page @@45", // op 4245
	"JMS @@46   ;Jump to subroutine Current page @@46", // op 4246
	"JMS @@47   ;Jump to subroutine Current page @@47", // op 4247
	"JMS @@50   ;Jump to subroutine Current page @@50", // op 4250
	"JMS @@51   ;Jump to subroutine Current page @@51", // op 4251
	"JMS @@52   ;Jump to subroutine Current page @@52", // op 4252
	"JMS @@53   ;Jump to subroutine Current page @@53", // op 4253
	"JMS @@54   ;Jump to subroutine Current page @@54", // op 4254
	"JMS @@55   ;Jump to subroutine Current page @@55", // op 4255
	"JMS @@56   ;Jump to subroutine Current page @@56", // op 4256
	"JMS @@57   ;Jump to subroutine Current page @@57", // op 4257
	"JMS @@60   ;Jump to subroutine Current page @@60", // op 4260
	"JMS @@61   ;Jump to subroutine Current page @@61", // op 4261
	"JMS @@62   ;Jump to subroutine Current page @@62", // op 4262
	"JMS @@63   ;Jump to subroutine Current page @@63", // op 4263
	"JMS @@64   ;Jump to subroutine Current page @@64", // op 4264
	"JMS @@65   ;Jump to subroutine Current page @@65", // op 4265
	"JMS @@66   ;Jump to subroutine Current page @@66", // op 4266
	"JMS @@67   ;Jump to subroutine Current page @@67", // op 4267
	"JMS @@70   ;Jump to subroutine Current page @@70", // op 4270
	"JMS @@71   ;Jump to subroutine Current page @@71", // op 4271
	"JMS @@72   ;Jump to subroutine Current page @@72", // op 4272
	"JMS @@73   ;Jump to subroutine Current page @@73", // op 4273
	"JMS @@74   ;Jump to subroutine Current page @@74", // op 4274
	"JMS @@75   ;Jump to subroutine Current page @@75", // op 4275
	"JMS @@76   ;Jump to subroutine Current page @@76", // op 4276
	"JMS @@77   ;Jump to subroutine Current page @@77", // op 4277
	"JMS @@00   ;Jump to subroutine Current page @@00", // op 4300
	"JMS @@01   ;Jump to subroutine Current page @@01", // op 4301
	"JMS @@02   ;Jump to subroutine Current page @@02", // op 4302
	"JMS @@03   ;Jump to subroutine Current page @@03", // op 4303
	"JMS @@04   ;Jump to subroutine Current page @@04", // op 4304
	"JMS @@05   ;Jump to subroutine Current page @@05", // op 4305
	"JMS @@06   ;Jump to subroutine Current page @@06", // op 4306
	"JMS @@07   ;Jump to subroutine Current page @@07", // op 4307
	"JMS @@10   ;Jump to subroutine Current page @@10", // op 4310
	"JMS @@11   ;Jump to subroutine Current page @@11", // op 4311
	"JMS @@12   ;Jump to subroutine Current page @@12", // op 4312
	"JMS @@13   ;Jump to subroutine Current page @@13", // op 4313
	"JMS @@14   ;Jump to subroutine Current page @@14", // op 4314
	"JMS @@15   ;Jump to subroutine Current page @@15", // op 4315
	"JMS @@16   ;Jump to subroutine Current page @@16", // op 4316
	"JMS @@17   ;Jump to subroutine Current page @@17", // op 4317
	"JMS @@20   ;Jump to subroutine Current page @@20", // op 4320
	"JMS @@21   ;Jump to subroutine Current page @@21", // op 4321
	"JMS @@22   ;Jump to subroutine Current page @@22", // op 4322
	"JMS @@23   ;Jump to subroutine Current page @@23", // op 4323
	"JMS @@24   ;Jump to subroutine Current page @@24", // op 4324
	"JMS @@25   ;Jump to subroutine Current page @@25", // op 4325
	"JMS @@26   ;Jump to subroutine Current page @@26", // op 4326
	"JMS @@27   ;Jump to subroutine Current page @@27", // op 4327
	"JMS @@30   ;Jump to subroutine Current page @@30", // op 4330
	"JMS @@31   ;Jump to subroutine Current page @@31", // op 4331
	"JMS @@32   ;Jump to subroutine Current page @@32", // op 4332
	"JMS @@33   ;Jump to subroutine Current page @@33", // op 4333
	"JMS @@34   ;Jump to subroutine Current page @@34", // op 4334
	"JMS @@35   ;Jump to subroutine Current page @@35", // op 4335
	"JMS @@36   ;Jump to subroutine Current page @@36", // op 4336
	"JMS @@37   ;Jump to subroutine Current page @@37", // op 4337
	"JMS @@40   ;Jump to subroutine Current page @@40", // op 4340
	"JMS @@41   ;Jump to subroutine Current page @@41", // op 4341
	"JMS @@42   ;Jump to subroutine Current page @@42", // op 4342
	"JMS @@43   ;Jump to subroutine Current page @@43", // op 4343
	"JMS @@44   ;Jump to subroutine Current page @@44", // op 4344
	"JMS @@45   ;Jump to subroutine Current page @@45", // op 4345
	"JMS @@46   ;Jump to subroutine Current page @@46", // op 4346
	"JMS @@47   ;Jump to subroutine Current page @@47", // op 4347
	"JMS @@50   ;Jump to subroutine Current page @@50", // op 4350
	"JMS @@51   ;Jump to subroutine Current page @@51", // op 4351
	"JMS @@52   ;Jump to subroutine Current page @@52", // op 4352
	"JMS @@53   ;Jump to subroutine Current page @@53", // op 4353
	"JMS @@54   ;Jump to subroutine Current page @@54", // op 4354
	"JMS @@55   ;Jump to subroutine Current page @@55", // op 4355
	"JMS @@56   ;Jump to subroutine Current page @@56", // op 4356
	"JMS @@57   ;Jump to subroutine Current page @@57", // op 4357
	"JMS @@60   ;Jump to subroutine Current page @@60", // op 4360
	"JMS @@61   ;Jump to subroutine Current page @@61", // op 4361
	"JMS @@62   ;Jump to subroutine Current page @@62", // op 4362
	"JMS @@63   ;Jump to subroutine Current page @@63", // op 4363
	"JMS @@64   ;Jump to subroutine Current page @@64", // op 4364
	"JMS @@65   ;Jump to subroutine Current page @@65", // op 4365
	"JMS @@66   ;Jump to subroutine Current page @@66", // op 4366
	"JMS @@67   ;Jump to subroutine Current page @@67", // op 4367
	"JMS @@70   ;Jump to subroutine Current page @@70", // op 4370
	"JMS @@71   ;Jump to subroutine Current page @@71", // op 4371
	"JMS @@72   ;Jump to subroutine Current page @@72", // op 4372
	"JMS @@73   ;Jump to subroutine Current page @@73", // op 4373
	"JMS @@74   ;Jump to subroutine Current page @@74", // op 4374
	"JMS @@75   ;Jump to subroutine Current page @@75", // op 4375
	"JMS @@76   ;Jump to subroutine Current page @@76", // op 4376
	"JMS @@77   ;Jump to subroutine Current page @@77", // op 4377
	"JMS I 0000 ;Jump to subroutine Indexed ZP 0000", // op 4400
	"JMS I 0001 ;Jump to subroutine Indexed ZP 0001", // op 4401
	"JMS I 0002 ;Jump to subroutine Indexed ZP 0002", // op 4402
	"JMS I 0003 ;Jump to subroutine Indexed ZP 0003", // op 4403
	"JMS I 0004 ;Jump to subroutine Indexed ZP 0004", // op 4404
	"JMS I 0005 ;Jump to subroutine Indexed ZP 0005", // op 4405
	"JMS I 0006 ;Jump to subroutine Indexed ZP 0006", // op 4406
	"JMS I 0007 ;Jump to subroutine Indexed ZP 0007", // op 4407
	"JMS I 0010 ;Jump to subroutine Indexed ZP 0010 [Auto pre-inc]", // op 4410
	"JMS I 0011 ;Jump to subroutine Indexed ZP 0011 [Auto pre-inc]", // op 4411
	"JMS I 0012 ;Jump to subroutine Indexed ZP 0012 [Auto pre-inc]", // op 4412
	"JMS I 0013 ;Jump to subroutine Indexed ZP 0013 [Auto pre-inc]", // op 4413
	"JMS I 0014 ;Jump to subroutine Indexed ZP 0014 [Auto pre-inc]", // op 4414
	"JMS I 0015 ;Jump to subroutine Indexed ZP 0015 [Auto pre-inc]", // op 4415
	"JMS I 0016 ;Jump to subroutine Indexed ZP 0016 [Auto pre-inc]", // op 4416
	"JMS I 0017 ;Jump to subroutine Indexed ZP 0017 [Auto pre-inc]", // op 4417
	"JMS I 0020 ;Jump to subroutine Indexed ZP 0020", // op 4420
	"JMS I 0021 ;Jump to subroutine Indexed ZP 0021", // op 4421
	"JMS I 0022 ;Jump to subroutine Indexed ZP 0022", // op 4422
	"JMS I 0023 ;Jump to subroutine Indexed ZP 0023", // op 4423
	"JMS I 0024 ;Jump to subroutine Indexed ZP 0024", // op 4424
	"JMS I 0025 ;Jump to subroutine Indexed ZP 0025", // op 4425
	"JMS I 0026 ;Jump to subroutine Indexed ZP 0026", // op 4426
	"JMS I 0027 ;Jump to subroutine Indexed ZP 0027", // op 4427
	"JMS I 0030 ;Jump to subroutine Indexed ZP 0030", // op 4430
	"JMS I 0031 ;Jump to subroutine Indexed ZP 0031", // op 4431
	"JMS I 0032 ;Jump to subroutine Indexed ZP 0032", // op 4432
	"JMS I 0033 ;Jump to subroutine Indexed ZP 0033", // op 4433
	"JMS I 0034 ;Jump to subroutine Indexed ZP 0034", // op 4434
	"JMS I 0035 ;Jump to subroutine Indexed ZP 0035", // op 4435
	"JMS I 0036 ;Jump to subroutine Indexed ZP 0036", // op 4436
	"JMS I 0037 ;Jump to subroutine Indexed ZP 0037", // op 4437
	"JMS I 0040 ;Jump to subroutine Indexed ZP 0040", // op 4440
	"JMS I 0041 ;Jump to subroutine Indexed ZP 0041", // op 4441
	"JMS I 0042 ;Jump to subroutine Indexed ZP 0042", // op 4442
	"JMS I 0043 ;Jump to subroutine Indexed ZP 0043", // op 4443
	"JMS I 0044 ;Jump to subroutine Indexed ZP 0044", // op 4444
	"JMS I 0045 ;Jump to subroutine Indexed ZP 0045", // op 4445
	"JMS I 0046 ;Jump to subroutine Indexed ZP 0046", // op 4446
	"JMS I 0047 ;Jump to subroutine Indexed ZP 0047", // op 4447
	"JMS I 0050 ;Jump to subroutine Indexed ZP 0050", // op 4450
	"JMS I 0051 ;Jump to subroutine Indexed ZP 0051", // op 4451
	"JMS I 0052 ;Jump to subroutine Indexed ZP 0052", // op 4452
	"JMS I 0053 ;Jump to subroutine Indexed ZP 0053", // op 4453
	"JMS I 0054 ;Jump to subroutine Indexed ZP 0054", // op 4454
	"JMS I 0055 ;Jump to subroutine Indexed ZP 0055", // op 4455
	"JMS I 0056 ;Jump to subroutine Indexed ZP 0056", // op 4456
	"JMS I 0057 ;Jump to subroutine Indexed ZP 0057", // op 4457
	"JMS I 0060 ;Jump to subroutine Indexed ZP 0060", // op 4460
	"JMS I 0061 ;Jump to subroutine Indexed ZP 0061", // op 4461
	"JMS I 0062 ;Jump to subroutine Indexed ZP 0062", // op 4462
	"JMS I 0063 ;Jump to subroutine Indexed ZP 0063", // op 4463
	"JMS I 0064 ;Jump to subroutine Indexed ZP 0064", // op 4464
	"JMS I 0065 ;Jump to subroutine Indexed ZP 0065", // op 4465
	"JMS I 0066 ;Jump to subroutine Indexed ZP 0066", // op 4466
	"JMS I 0067 ;Jump to subroutine Indexed ZP 0067", // op 4467
	"JMS I 0070 ;Jump to subroutine Indexed ZP 0070", // op 4470
	"JMS I 0071 ;Jump to subroutine Indexed ZP 0071", // op 4471
	"JMS I 0072 ;Jump to subroutine Indexed ZP 0072", // op 4472
	"JMS I 0073 ;Jump to subroutine Indexed ZP 0073", // op 4473
	"JMS I 0074 ;Jump to subroutine Indexed ZP 0074", // op 4474
	"JMS I 0075 ;Jump to subroutine Indexed ZP 0075", // op 4475
	"JMS I 0076 ;Jump to subroutine Indexed ZP 0076", // op 4476
	"JMS I 0077 ;Jump to subroutine Indexed ZP 0077", // op 4477
	"JMS I 0100 ;Jump to subroutine Indexed ZP 0100", // op 4500
	"JMS I 0101 ;Jump to subroutine Indexed ZP 0101", // op 4501
	"JMS I 0102 ;Jump to subroutine Indexed ZP 0102", // op 4502
	"JMS I 0103 ;Jump to subroutine Indexed ZP 0103", // op 4503
	"JMS I 0104 ;Jump to subroutine Indexed ZP 0104", // op 4504
	"JMS I 0105 ;Jump to subroutine Indexed ZP 0105", // op 4505
	"JMS I 0106 ;Jump to subroutine Indexed ZP 0106", // op 4506
	"JMS I 0107 ;Jump to subroutine Indexed ZP 0107", // op 4507
	"JMS I 0110 ;Jump to subroutine Indexed ZP 0110", // op 4510
	"JMS I 0111 ;Jump to subroutine Indexed ZP 0111", // op 4511
	"JMS I 0112 ;Jump to subroutine Indexed ZP 0112", // op 4512
	"JMS I 0113 ;Jump to subroutine Indexed ZP 0113", // op 4513
	"JMS I 0114 ;Jump to subroutine Indexed ZP 0114", // op 4514
	"JMS I 0115 ;Jump to subroutine Indexed ZP 0115", // op 4515
	"JMS I 0116 ;Jump to subroutine Indexed ZP 0116", // op 4516
	"JMS I 0117 ;Jump to subroutine Indexed ZP 0117", // op 4517
	"JMS I 0120 ;Jump to subroutine Indexed ZP 0120", // op 4520
	"JMS I 0121 ;Jump to subroutine Indexed ZP 0121", // op 4521
	"JMS I 0122 ;Jump to subroutine Indexed ZP 0122", // op 4522
	"JMS I 0123 ;Jump to subroutine Indexed ZP 0123", // op 4523
	"JMS I 0124 ;Jump to subroutine Indexed ZP 0124", // op 4524
	"JMS I 0125 ;Jump to subroutine Indexed ZP 0125", // op 4525
	"JMS I 0126 ;Jump to subroutine Indexed ZP 0126", // op 4526
	"JMS I 0127 ;Jump to subroutine Indexed ZP 0127", // op 4527
	"JMS I 0130 ;Jump to subroutine Indexed ZP 0130", // op 4530
	"JMS I 0131 ;Jump to subroutine Indexed ZP 0131", // op 4531
	"JMS I 0132 ;Jump to subroutine Indexed ZP 0132", // op 4532
	"JMS I 0133 ;Jump to subroutine Indexed ZP 0133", // op 4533
	"JMS I 0134 ;Jump to subroutine Indexed ZP 0134", // op 4534
	"JMS I 0135 ;Jump to subroutine Indexed ZP 0135", // op 4535
	"JMS I 0136 ;Jump to subroutine Indexed ZP 0136", // op 4536
	"JMS I 0137 ;Jump to subroutine Indexed ZP 0137", // op 4537
	"JMS I 0140 ;Jump to subroutine Indexed ZP 0140", // op 4540
	"JMS I 0141 ;Jump to subroutine Indexed ZP 0141", // op 4541
	"JMS I 0142 ;Jump to subroutine Indexed ZP 0142", // op 4542
	"JMS I 0143 ;Jump to subroutine Indexed ZP 0143", // op 4543
	"JMS I 0144 ;Jump to subroutine Indexed ZP 0144", // op 4544
	"JMS I 0145 ;Jump to subroutine Indexed ZP 0145", // op 4545
	"JMS I 0146 ;Jump to subroutine Indexed ZP 0146", // op 4546
	"JMS I 0147 ;Jump to subroutine Indexed ZP 0147", // op 4547
	"JMS I 0150 ;Jump to subroutine Indexed ZP 0150", // op 4550
	"JMS I 0151 ;Jump to subroutine Indexed ZP 0151", // op 4551
	"JMS I 0152 ;Jump to subroutine Indexed ZP 0152", // op 4552
	"JMS I 0153 ;Jump to subroutine Indexed ZP 0153", // op 4553
	"JMS I 0154 ;Jump to subroutine Indexed ZP 0154", // op 4554
	"JMS I 0155 ;Jump to subroutine Indexed ZP 0155", // op 4555
	"JMS I 0156 ;Jump to subroutine Indexed ZP 0156", // op 4556
	"JMS I 0157 ;Jump to subroutine Indexed ZP 0157", // op 4557
	"JMS I 0160 ;Jump to subroutine Indexed ZP 0160", // op 4560
	"JMS I 0161 ;Jump to subroutine Indexed ZP 0161", // op 4561
	"JMS I 0162 ;Jump to subroutine Indexed ZP 0162", // op 4562
	"JMS I 0163 ;Jump to subroutine Indexed ZP 0163", // op 4563
	"JMS I 0164 ;Jump to subroutine Indexed ZP 0164", // op 4564
	"JMS I 0165 ;Jump to subroutine Indexed ZP 0165", // op 4565
	"JMS I 0166 ;Jump to subroutine Indexed ZP 0166", // op 4566
	"JMS I 0167 ;Jump to subroutine Indexed ZP 0167", // op 4567
	"JMS I 0170 ;Jump to subroutine Indexed ZP 0170", // op 4570
	"JMS I 0171 ;Jump to subroutine Indexed ZP 0171", // op 4571
	"JMS I 0172 ;Jump to subroutine Indexed ZP 0172", // op 4572
	"JMS I 0173 ;Jump to subroutine Indexed ZP 0173", // op 4573
	"JMS I 0174 ;Jump to subroutine Indexed ZP 0174", // op 4574
	"JMS I 0175 ;Jump to subroutine Indexed ZP 0175", // op 4575
	"JMS I 0176 ;Jump to subroutine Indexed ZP 0176", // op 4576
	"JMS I 0177 ;Jump to subroutine Indexed ZP 0177", // op 4577
	"JMS I @@00 ;Jump to subroutine Indexed Current page @@00", // op 4600
	"JMS I @@01 ;Jump to subroutine Indexed Current page @@01", // op 4601
	"JMS I @@02 ;Jump to subroutine Indexed Current page @@02", // op 4602
	"JMS I @@03 ;Jump to subroutine Indexed Current page @@03", // op 4603
	"JMS I @@04 ;Jump to subroutine Indexed Current page @@04", // op 4604
	"JMS I @@05 ;Jump to subroutine Indexed Current page @@05", // op 4605
	"JMS I @@06 ;Jump to subroutine Indexed Current page @@06", // op 4606
	"JMS I @@07 ;Jump to subroutine Indexed Current page @@07", // op 4607
	"JMS I @@10 ;Jump to subroutine Indexed Current page @@10", // op 4610
	"JMS I @@11 ;Jump to subroutine Indexed Current page @@11", // op 4611
	"JMS I @@12 ;Jump to subroutine Indexed Current page @@12", // op 4612
	"JMS I @@13 ;Jump to subroutine Indexed Current page @@13", // op 4613
	"JMS I @@14 ;Jump to subroutine Indexed Current page @@14", // op 4614
	"JMS I @@15 ;Jump to subroutine Indexed Current page @@15", // op 4615
	"JMS I @@16 ;Jump to subroutine Indexed Current page @@16", // op 4616
	"JMS I @@17 ;Jump to subroutine Indexed Current page @@17", // op 4617
	"JMS I @@20 ;Jump to subroutine Indexed Current page @@20", // op 4620
	"JMS I @@21 ;Jump to subroutine Indexed Current page @@21", // op 4621
	"JMS I @@22 ;Jump to subroutine Indexed Current page @@22", // op 4622
	"JMS I @@23 ;Jump to subroutine Indexed Current page @@23", // op 4623
	"JMS I @@24 ;Jump to subroutine Indexed Current page @@24", // op 4624
	"JMS I @@25 ;Jump to subroutine Indexed Current page @@25", // op 4625
	"JMS I @@26 ;Jump to subroutine Indexed Current page @@26", // op 4626
	"JMS I @@27 ;Jump to subroutine Indexed Current page @@27", // op 4627
	"JMS I @@30 ;Jump to subroutine Indexed Current page @@30", // op 4630
	"JMS I @@31 ;Jump to subroutine Indexed Current page @@31", // op 4631
	"JMS I @@32 ;Jump to subroutine Indexed Current page @@32", // op 4632
	"JMS I @@33 ;Jump to subroutine Indexed Current page @@33", // op 4633
	"JMS I @@34 ;Jump to subroutine Indexed Current page @@34", // op 4634
	"JMS I @@35 ;Jump to subroutine Indexed Current page @@35", // op 4635
	"JMS I @@36 ;Jump to subroutine Indexed Current page @@36", // op 4636
	"JMS I @@37 ;Jump to subroutine Indexed Current page @@37", // op 4637
	"JMS I @@40 ;Jump to subroutine Indexed Current page @@40", // op 4640
	"JMS I @@41 ;Jump to subroutine Indexed Current page @@41", // op 4641
	"JMS I @@42 ;Jump to subroutine Indexed Current page @@42", // op 4642
	"JMS I @@43 ;Jump to subroutine Indexed Current page @@43", // op 4643
	"JMS I @@44 ;Jump to subroutine Indexed Current page @@44", // op 4644
	"JMS I @@45 ;Jump to subroutine Indexed Current page @@45", // op 4645
	"JMS I @@46 ;Jump to subroutine Indexed Current page @@46", // op 4646
	"JMS I @@47 ;Jump to subroutine Indexed Current page @@47", // op 4647
	"JMS I @@50 ;Jump to subroutine Indexed Current page @@50", // op 4650
	"JMS I @@51 ;Jump to subroutine Indexed Current page @@51", // op 4651
	"JMS I @@52 ;Jump to subroutine Indexed Current page @@52", // op 4652
	"JMS I @@53 ;Jump to subroutine Indexed Current page @@53", // op 4653
	"JMS I @@54 ;Jump to subroutine Indexed Current page @@54", // op 4654
	"JMS I @@55 ;Jump to subroutine Indexed Current page @@55", // op 4655
	"JMS I @@56 ;Jump to subroutine Indexed Current page @@56", // op 4656
	"JMS I @@57 ;Jump to subroutine Indexed Current page @@57", // op 4657
	"JMS I @@60 ;Jump to subroutine Indexed Current page @@60", // op 4660
	"JMS I @@61 ;Jump to subroutine Indexed Current page @@61", // op 4661
	"JMS I @@62 ;Jump to subroutine Indexed Current page @@62", // op 4662
	"JMS I @@63 ;Jump to subroutine Indexed Current page @@63", // op 4663
	"JMS I @@64 ;Jump to subroutine Indexed Current page @@64", // op 4664
	"JMS I @@65 ;Jump to subroutine Indexed Current page @@65", // op 4665
	"JMS I @@66 ;Jump to subroutine Indexed Current page @@66", // op 4666
	"JMS I @@67 ;Jump to subroutine Indexed Current page @@67", // op 4667
	"JMS I @@70 ;Jump to subroutine Indexed Current page @@70", // op 4670
	"JMS I @@71 ;Jump to subroutine Indexed Current page @@71", // op 4671
	"JMS I @@72 ;Jump to subroutine Indexed Current page @@72", // op 4672
	"JMS I @@73 ;Jump to subroutine Indexed Current page @@73", // op 4673
	"JMS I @@74 ;Jump to subroutine Indexed Current page @@74", // op 4674
	"JMS I @@75 ;Jump to subroutine Indexed Current page @@75", // op 4675
	"JMS I @@76 ;Jump to subroutine Indexed Current page @@76", // op 4676
	"JMS I @@77 ;Jump to subroutine Indexed Current page @@77", // op 4677
	"JMS I @@00 ;Jump to subroutine Indexed Current page @@00", // op 4700
	"JMS I @@01 ;Jump to subroutine Indexed Current page @@01", // op 4701
	"JMS I @@02 ;Jump to subroutine Indexed Current page @@02", // op 4702
	"JMS I @@03 ;Jump to subroutine Indexed Current page @@03", // op 4703
	"JMS I @@04 ;Jump to subroutine Indexed Current page @@04", // op 4704
	"JMS I @@05 ;Jump to subroutine Indexed Current page @@05", // op 4705
	"JMS I @@06 ;Jump to subroutine Indexed Current page @@06", // op 4706
	"JMS I @@07 ;Jump to subroutine Indexed Current page @@07", // op 4707
	"JMS I @@10 ;Jump to subroutine Indexed Current page @@10", // op 4710
	"JMS I @@11 ;Jump to subroutine Indexed Current page @@11", // op 4711
	"JMS I @@12 ;Jump to subroutine Indexed Current page @@12", // op 4712
	"JMS I @@13 ;Jump to subroutine Indexed Current page @@13", // op 4713
	"JMS I @@14 ;Jump to subroutine Indexed Current page @@14", // op 4714
	"JMS I @@15 ;Jump to subroutine Indexed Current page @@15", // op 4715
	"JMS I @@16 ;Jump to subroutine Indexed Current page @@16", // op 4716
	"JMS I @@17 ;Jump to subroutine Indexed Current page @@17", // op 4717
	"JMS I @@20 ;Jump to subroutine Indexed Current page @@20", // op 4720
	"JMS I @@21 ;Jump to subroutine Indexed Current page @@21", // op 4721
	"JMS I @@22 ;Jump to subroutine Indexed Current page @@22", // op 4722
	"JMS I @@23 ;Jump to subroutine Indexed Current page @@23", // op 4723
	"JMS I @@24 ;Jump to subroutine Indexed Current page @@24", // op 4724
	"JMS I @@25 ;Jump to subroutine Indexed Current page @@25", // op 4725
	"JMS I @@26 ;Jump to subroutine Indexed Current page @@26", // op 4726
	"JMS I @@27 ;Jump to subroutine Indexed Current page @@27", // op 4727
	"JMS I @@30 ;Jump to subroutine Indexed Current page @@30", // op 4730
	"JMS I @@31 ;Jump to subroutine Indexed Current page @@31", // op 4731
	"JMS I @@32 ;Jump to subroutine Indexed Current page @@32", // op 4732
	"JMS I @@33 ;Jump to subroutine Indexed Current page @@33", // op 4733
	"JMS I @@34 ;Jump to subroutine Indexed Current page @@34", // op 4734
	"JMS I @@35 ;Jump to subroutine Indexed Current page @@35", // op 4735
	"JMS I @@36 ;Jump to subroutine Indexed Current page @@36", // op 4736
	"JMS I @@37 ;Jump to subroutine Indexed Current page @@37", // op 4737
	"JMS I @@40 ;Jump to subroutine Indexed Current page @@40", // op 4740
	"JMS I @@41 ;Jump to subroutine Indexed Current page @@41", // op 4741
	"JMS I @@42 ;Jump to subroutine Indexed Current page @@42", // op 4742
	"JMS I @@43 ;Jump to subroutine Indexed Current page @@43", // op 4743
	"JMS I @@44 ;Jump to subroutine Indexed Current page @@44", // op 4744
	"JMS I @@45 ;Jump to subroutine Indexed Current page @@45", // op 4745
	"JMS I @@46 ;Jump to subroutine Indexed Current page @@46", // op 4746
	"JMS I @@47 ;Jump to subroutine Indexed Current page @@47", // op 4747
	"JMS I @@50 ;Jump to subroutine Indexed Current page @@50", // op 4750
	"JMS I @@51 ;Jump to subroutine Indexed Current page @@51", // op 4751
	"JMS I @@52 ;Jump to subroutine Indexed Current page @@52", // op 4752
	"JMS I @@53 ;Jump to subroutine Indexed Current page @@53", // op 4753
	"JMS I @@54 ;Jump to subroutine Indexed Current page @@54", // op 4754
	"JMS I @@55 ;Jump to subroutine Indexed Current page @@55", // op 4755
	"JMS I @@56 ;Jump to subroutine Indexed Current page @@56", // op 4756
	"JMS I @@57 ;Jump to subroutine Indexed Current page @@57", // op 4757
	"JMS I @@60 ;Jump to subroutine Indexed Current page @@60", // op 4760
	"JMS I @@61 ;Jump to subroutine Indexed Current page @@61", // op 4761
	"JMS I @@62 ;Jump to subroutine Indexed Current page @@62", // op 4762
	"JMS I @@63 ;Jump to subroutine Indexed Current page @@63", // op 4763
	"JMS I @@64 ;Jump to subroutine Indexed Current page @@64", // op 4764
	"JMS I @@65 ;Jump to subroutine Indexed Current page @@65", // op 4765
	"JMS I @@66 ;Jump to subroutine Indexed Current page @@66", // op 4766
	"JMS I @@67 ;Jump to subroutine Indexed Current page @@67", // op 4767
	"JMS I @@70 ;Jump to subroutine Indexed Current page @@70", // op 4770
	"JMS I @@71 ;Jump to subroutine Indexed Current page @@71", // op 4771
	"JMS I @@72 ;Jump to subroutine Indexed Current page @@72", // op 4772
	"JMS I @@73 ;Jump to subroutine Indexed Current page @@73", // op 4773
	"JMS I @@74 ;Jump to subroutine Indexed Current page @@74", // op 4774
	"JMS I @@75 ;Jump to subroutine Indexed Current page @@75", // op 4775
	"JMS I @@76 ;Jump to subroutine Indexed Current page @@76", // op 4776
	"JMS I @@77 ;Jump to subroutine Indexed Current page @@77", // op 4777
	"JMP 0000   ;Jump ZP 0000 ", // op 5000
	"JMP 0001   ;Jump ZP 0001", // op 5001
	"JMP 0002   ;Jump ZP 0002", // op 5002
	"JMP 0003   ;Jump ZP 0003", // op 5003
	"JMP 0004   ;Jump ZP 0004", // op 5004
	"JMP 0005   ;Jump ZP 0005", // op 5005
	"JMP 0006   ;Jump ZP 0006", // op 5006
	"JMP 0007   ;Jump ZP 0007", // op 5007
	"JMP 0010   ;Jump ZP 0010", // op 5010
	"JMP 0011   ;Jump ZP 0011", // op 5011
	"JMP 0012   ;Jump ZP 0012", // op 5012
	"JMP 0013   ;Jump ZP 0013", // op 5013
	"JMP 0014   ;Jump ZP 0014", // op 5014
	"JMP 0015   ;Jump ZP 0015", // op 5015
	"JMP 0016   ;Jump ZP 0016", // op 5016
	"JMP 0017   ;Jump ZP 0017", // op 5017
	"JMP 0020   ;Jump ZP 0020", // op 5020
	"JMP 0021   ;Jump ZP 0021", // op 5021
	"JMP 0022   ;Jump ZP 0022", // op 5022
	"JMP 0023   ;Jump ZP 0023", // op 5023
	"JMP 0024   ;Jump ZP 0024", // op 5024
	"JMP 0025   ;Jump ZP 0025", // op 5025
	"JMP 0026   ;Jump ZP 0026", // op 5026
	"JMP 0027   ;Jump ZP 0027", // op 5027
	"JMP 0030   ;Jump ZP 0030", // op 5030
	"JMP 0031   ;Jump ZP 0031", // op 5031
	"JMP 0032   ;Jump ZP 0032", // op 5032
	"JMP 0033   ;Jump ZP 0033", // op 5033
	"JMP 0034   ;Jump ZP 0034", // op 5034
	"JMP 0035   ;Jump ZP 0035", // op 5035
	"JMP 0036   ;Jump ZP 0036", // op 5036
	"JMP 0037   ;Jump ZP 0037", // op 5037
	"JMP 0040   ;Jump ZP 0040", // op 5040
	"JMP 0041   ;Jump ZP 0041", // op 5041
	"JMP 0042   ;Jump ZP 0042", // op 5042
	"JMP 0043   ;Jump ZP 0043", // op 5043
	"JMP 0044   ;Jump ZP 0044", // op 5044
	"JMP 0045   ;Jump ZP 0045", // op 5045
	"JMP 0046   ;Jump ZP 0046", // op 5046
	"JMP 0047   ;Jump ZP 0047", // op 5047
	"JMP 0050   ;Jump ZP 0050", // op 5050
	"JMP 0051   ;Jump ZP 0051", // op 5051
	"JMP 0052   ;Jump ZP 0052", // op 5052
	"JMP 0053   ;Jump ZP 0053", // op 5053
	"JMP 0054   ;Jump ZP 0054", // op 5054
	"JMP 0055   ;Jump ZP 0055", // op 5055
	"JMP 0056   ;Jump ZP 0056", // op 5056
	"JMP 0057   ;Jump ZP 0057", // op 5057
	"JMP 0060   ;Jump ZP 0060", // op 5060
	"JMP 0061   ;Jump ZP 0061", // op 5061
	"JMP 0062   ;Jump ZP 0062", // op 5062
	"JMP 0063   ;Jump ZP 0063", // op 5063
	"JMP 0064   ;Jump ZP 0064", // op 5064
	"JMP 0065   ;Jump ZP 0065", // op 5065
	"JMP 0066   ;Jump ZP 0066", // op 5066
	"JMP 0067   ;Jump ZP 0067", // op 5067
	"JMP 0070   ;Jump ZP 0070", // op 5070
	"JMP 0071   ;Jump ZP 0071", // op 5071
	"JMP 0072   ;Jump ZP 0072", // op 5072
	"JMP 0073   ;Jump ZP 0073", // op 5073
	"JMP 0074   ;Jump ZP 0074", // op 5074
	"JMP 0075   ;Jump ZP 0075", // op 5075
	"JMP 0076   ;Jump ZP 0076", // op 5076
	"JMP 0077   ;Jump ZP 0077", // op 5077
	"JMP 0100   ;Jump ZP 0100", // op 5100
	"JMP 0101   ;Jump ZP 0101", // op 5101
	"JMP 0102   ;Jump ZP 0102", // op 5102
	"JMP 0103   ;Jump ZP 0103", // op 5103
	"JMP 0104   ;Jump ZP 0104", // op 5104
	"JMP 0105   ;Jump ZP 0105", // op 5105
	"JMP 0106   ;Jump ZP 0106", // op 5106
	"JMP 0107   ;Jump ZP 0107", // op 5107
	"JMP 0110   ;Jump ZP 0110", // op 5110
	"JMP 0111   ;Jump ZP 0111", // op 5111
	"JMP 0112   ;Jump ZP 0112", // op 5112
	"JMP 0113   ;Jump ZP 0113", // op 5113
	"JMP 0114   ;Jump ZP 0114", // op 5114
	"JMP 0115   ;Jump ZP 0115", // op 5115
	"JMP 0116   ;Jump ZP 0116", // op 5116
	"JMP 0117   ;Jump ZP 0117", // op 5117
	"JMP 0120   ;Jump ZP 0120", // op 5120
	"JMP 0121   ;Jump ZP 0121", // op 5121
	"JMP 0122   ;Jump ZP 0122", // op 5122
	"JMP 0123   ;Jump ZP 0123", // op 5123
	"JMP 0124   ;Jump ZP 0124", // op 5124
	"JMP 0125   ;Jump ZP 0125", // op 5125
	"JMP 0126   ;Jump ZP 0126", // op 5126
	"JMP 0127   ;Jump ZP 0127", // op 5127
	"JMP 0130   ;Jump ZP 0130", // op 5130
	"JMP 0131   ;Jump ZP 0131", // op 5131
	"JMP 0132   ;Jump ZP 0132", // op 5132
	"JMP 0133   ;Jump ZP 0133", // op 5133
	"JMP 0134   ;Jump ZP 0134", // op 5134
	"JMP 0135   ;Jump ZP 0135", // op 5135
	"JMP 0136   ;Jump ZP 0136", // op 5136
	"JMP 0137   ;Jump ZP 0137", // op 5137
	"JMP 0140   ;Jump ZP 0140", // op 5140
	"JMP 0141   ;Jump ZP 0141", // op 5141
	"JMP 0142   ;Jump ZP 0142", // op 5142
	"JMP 0143   ;Jump ZP 0143", // op 5143
	"JMP 0144   ;Jump ZP 0144", // op 5144
	"JMP 0145   ;Jump ZP 0145", // op 5145
	"JMP 0146   ;Jump ZP 0146", // op 5146
	"JMP 0147   ;Jump ZP 0147", // op 5147
	"JMP 0150   ;Jump ZP 0150", // op 5150
	"JMP 0151   ;Jump ZP 0151", // op 5151
	"JMP 0152   ;Jump ZP 0152", // op 5152
	"JMP 0153   ;Jump ZP 0153", // op 5153
	"JMP 0154   ;Jump ZP 0154", // op 5154
	"JMP 0155   ;Jump ZP 0155", // op 5155
	"JMP 0156   ;Jump ZP 0156", // op 5156
	"JMP 0157   ;Jump ZP 0157", // op 5157
	"JMP 0160   ;Jump ZP 0160", // op 5160
	"JMP 0161   ;Jump ZP 0161", // op 5161
	"JMP 0162   ;Jump ZP 0162", // op 5162
	"JMP 0163   ;Jump ZP 0163", // op 5163
	"JMP 0164   ;Jump ZP 0164", // op 5164
	"JMP 0165   ;Jump ZP 0165", // op 5165
	"JMP 0166   ;Jump ZP 0166", // op 5166
	"JMP 0167   ;Jump ZP 0167", // op 5167
	"JMP 0170   ;Jump ZP 0170", // op 5170
	"JMP 0171   ;Jump ZP 0171", // op 5171
	"JMP 0172   ;Jump ZP 0172", // op 5172
	"JMP 0173   ;Jump ZP 0173", // op 5173
	"JMP 0174   ;Jump ZP 0174", // op 5174
	"JMP 0175   ;Jump ZP 0175", // op 5175
	"JMP 0176   ;Jump ZP 0176", // op 5176
	"JMP 0177   ;Jump ZP 0177", // op 5177
	"JMP @@00   ;Jump Current page @@00", // op 5200
	"JMP @@01   ;Jump Current page @@01", // op 5201
	"JMP @@02   ;Jump Current page @@02", // op 5202
	"JMP @@03   ;Jump Current page @@03", // op 5203
	"JMP @@04   ;Jump Current page @@04", // op 5204
	"JMP @@05   ;Jump Current page @@05", // op 5205
	"JMP @@06   ;Jump Current page @@06", // op 5206
	"JMP @@07   ;Jump Current page @@07", // op 5207
	"JMP @@10   ;Jump Current page @@10", // op 5210
	"JMP @@11   ;Jump Current page @@11", // op 5211
	"JMP @@12   ;Jump Current page @@12", // op 5212
	"JMP @@13   ;Jump Current page @@13", // op 5213
	"JMP @@14   ;Jump Current page @@14", // op 5214
	"JMP @@15   ;Jump Current page @@15", // op 5215
	"JMP @@16   ;Jump Current page @@16", // op 5216
	"JMP @@17   ;Jump Current page @@17", // op 5217
	"JMP @@20   ;Jump Current page @@20", // op 5220
	"JMP @@21   ;Jump Current page @@21", // op 5221
	"JMP @@22   ;Jump Current page @@22", // op 5222
	"JMP @@23   ;Jump Current page @@23", // op 5223
	"JMP @@24   ;Jump Current page @@24", // op 5224
	"JMP @@25   ;Jump Current page @@25", // op 5225
	"JMP @@26   ;Jump Current page @@26", // op 5226
	"JMP @@27   ;Jump Current page @@27", // op 5227
	"JMP @@30   ;Jump Current page @@30", // op 5230
	"JMP @@31   ;Jump Current page @@31", // op 5231
	"JMP @@32   ;Jump Current page @@32", // op 5232
	"JMP @@33   ;Jump Current page @@33", // op 5233
	"JMP @@34   ;Jump Current page @@34", // op 5234
	"JMP @@35   ;Jump Current page @@35", // op 5235
	"JMP @@36   ;Jump Current page @@36", // op 5236
	"JMP @@37   ;Jump Current page @@37", // op 5237
	"JMP @@40   ;Jump Current page @@40", // op 5240
	"JMP @@41   ;Jump Current page @@41", // op 5241
	"JMP @@42   ;Jump Current page @@42", // op 5242
	"JMP @@43   ;Jump Current page @@43", // op 5243
	"JMP @@44   ;Jump Current page @@44", // op 5244
	"JMP @@45   ;Jump Current page @@45", // op 5245
	"JMP @@46   ;Jump Current page @@46", // op 5246
	"JMP @@47   ;Jump Current page @@47", // op 5247
	"JMP @@50   ;Jump Current page @@50", // op 5250
	"JMP @@51   ;Jump Current page @@51", // op 5251
	"JMP @@52   ;Jump Current page @@52", // op 5252
	"JMP @@53   ;Jump Current page @@53", // op 5253
	"JMP @@54   ;Jump Current page @@54", // op 5254
	"JMP @@55   ;Jump Current page @@55", // op 5255
	"JMP @@56   ;Jump Current page @@56", // op 5256
	"JMP @@57   ;Jump Current page @@57", // op 5257
	"JMP @@60   ;Jump Current page @@60", // op 5260
	"JMP @@61   ;Jump Current page @@61", // op 5261
	"JMP @@62   ;Jump Current page @@62", // op 5262
	"JMP @@63   ;Jump Current page @@63", // op 5263
	"JMP @@64   ;Jump Current page @@64", // op 5264
	"JMP @@65   ;Jump Current page @@65", // op 5265
	"JMP @@66   ;Jump Current page @@66", // op 5266
	"JMP @@67   ;Jump Current page @@67", // op 5267
	"JMP @@70   ;Jump Current page @@70", // op 5270
	"JMP @@71   ;Jump Current page @@71", // op 5271
	"JMP @@72   ;Jump Current page @@72", // op 5272
	"JMP @@73   ;Jump Current page @@73", // op 5273
	"JMP @@74   ;Jump Current page @@74", // op 5274
	"JMP @@75   ;Jump Current page @@75", // op 5275
	"JMP @@76   ;Jump Current page @@76", // op 5276
	"JMP @@77   ;Jump Current page @@77", // op 5277
	"JMP @@00   ;Jump Current page @@00", // op 5300
	"JMP @@01   ;Jump Current page @@01", // op 5301
	"JMP @@02   ;Jump Current page @@02", // op 5302
	"JMP @@03   ;Jump Current page @@03", // op 5303
	"JMP @@04   ;Jump Current page @@04", // op 5304
	"JMP @@05   ;Jump Current page @@05", // op 5305
	"JMP @@06   ;Jump Current page @@06", // op 5306
	"JMP @@07   ;Jump Current page @@07", // op 5307
	"JMP @@10   ;Jump Current page @@10", // op 5310
	"JMP @@11   ;Jump Current page @@11", // op 5311
	"JMP @@12   ;Jump Current page @@12", // op 5312
	"JMP @@13   ;Jump Current page @@13", // op 5313
	"JMP @@14   ;Jump Current page @@14", // op 5314
	"JMP @@15   ;Jump Current page @@15", // op 5315
	"JMP @@16   ;Jump Current page @@16", // op 5316
	"JMP @@17   ;Jump Current page @@17", // op 5317
	"JMP @@20   ;Jump Current page @@20", // op 5320
	"JMP @@21   ;Jump Current page @@21", // op 5321
	"JMP @@22   ;Jump Current page @@22", // op 5322
	"JMP @@23   ;Jump Current page @@23", // op 5323
	"JMP @@24   ;Jump Current page @@24", // op 5324
	"JMP @@25   ;Jump Current page @@25", // op 5325
	"JMP @@26   ;Jump Current page @@26", // op 5326
	"JMP @@27   ;Jump Current page @@27", // op 5327
	"JMP @@30   ;Jump Current page @@30", // op 5330
	"JMP @@31   ;Jump Current page @@31", // op 5331
	"JMP @@32   ;Jump Current page @@32", // op 5332
	"JMP @@33   ;Jump Current page @@33", // op 5333
	"JMP @@34   ;Jump Current page @@34", // op 5334
	"JMP @@35   ;Jump Current page @@35", // op 5335
	"JMP @@36   ;Jump Current page @@36", // op 5336
	"JMP @@37   ;Jump Current page @@37", // op 5337
	"JMP @@40   ;Jump Current page @@40", // op 5340
	"JMP @@41   ;Jump Current page @@41", // op 5341
	"JMP @@42   ;Jump Current page @@42", // op 5342
	"JMP @@43   ;Jump Current page @@43", // op 5343
	"JMP @@44   ;Jump Current page @@44", // op 5344
	"JMP @@45   ;Jump Current page @@45", // op 5345
	"JMP @@46   ;Jump Current page @@46", // op 5346
	"JMP @@47   ;Jump Current page @@47", // op 5347
	"JMP @@50   ;Jump Current page @@50", // op 5350
	"JMP @@51   ;Jump Current page @@51", // op 5351
	"JMP @@52   ;Jump Current page @@52", // op 5352
	"JMP @@53   ;Jump Current page @@53", // op 5353
	"JMP @@54   ;Jump Current page @@54", // op 5354
	"JMP @@55   ;Jump Current page @@55", // op 5355
	"JMP @@56   ;Jump Current page @@56", // op 5356
	"JMP @@57   ;Jump Current page @@57", // op 5357
	"JMP @@60   ;Jump Current page @@60", // op 5360
	"JMP @@61   ;Jump Current page @@61", // op 5361
	"JMP @@62   ;Jump Current page @@62", // op 5362
	"JMP @@63   ;Jump Current page @@63", // op 5363
	"JMP @@64   ;Jump Current page @@64", // op 5364
	"JMP @@65   ;Jump Current page @@65", // op 5365
	"JMP @@66   ;Jump Current page @@66", // op 5366
	"JMP @@67   ;Jump Current page @@67", // op 5367
	"JMP @@70   ;Jump Current page @@70", // op 5370
	"JMP @@71   ;Jump Current page @@71", // op 5371
	"JMP @@72   ;Jump Current page @@72", // op 5372
	"JMP @@73   ;Jump Current page @@73", // op 5373
	"JMP @@74   ;Jump Current page @@74", // op 5374
	"JMP @@75   ;Jump Current page @@75", // op 5375
	"JMP @@76   ;Jump Current page @@76", // op 5376
	"JMP @@77   ;Jump Current page @@77", // op 5377
	"JMP I 0000 ;Jump Indexed ZP 0000", // op 5400
	"JMP I 0001 ;Jump Indexed ZP 0001", // op 5401
	"JMP I 0002 ;Jump Indexed ZP 0002", // op 5402
	"JMP I 0003 ;Jump Indexed ZP 0003", // op 5403
	"JMP I 0004 ;Jump Indexed ZP 0004", // op 5404
	"JMP I 0005 ;Jump Indexed ZP 0005", // op 5405
	"JMP I 0006 ;Jump Indexed ZP 0006", // op 5406
	"JMP I 0007 ;Jump Indexed ZP 0007", // op 5407
	"JMP I 0010 ;Jump Indexed ZP 0010 [Auto pre-inc]", // op 5410
	"JMP I 0011 ;Jump Indexed ZP 0011 [Auto pre-inc]", // op 5411
	"JMP I 0012 ;Jump Indexed ZP 0012 [Auto pre-inc]", // op 5412
	"JMP I 0013 ;Jump Indexed ZP 0013 [Auto pre-inc]", // op 5413
	"JMP I 0014 ;Jump Indexed ZP 0014 [Auto pre-inc]", // op 5414
	"JMP I 0015 ;Jump Indexed ZP 0015 [Auto pre-inc]", // op 5415
	"JMP I 0016 ;Jump Indexed ZP 0016 [Auto pre-inc]", // op 5416
	"JMP I 0017 ;Jump Indexed ZP 0017 [Auto pre-inc]", // op 5417
	"JMP I 0020 ;Jump Indexed ZP 0020", // op 5420
	"JMP I 0021 ;Jump Indexed ZP 0021", // op 5421
	"JMP I 0022 ;Jump Indexed ZP 0022", // op 5422
	"JMP I 0023 ;Jump Indexed ZP 0023", // op 5423
	"JMP I 0024 ;Jump Indexed ZP 0024", // op 5424
	"JMP I 0025 ;Jump Indexed ZP 0025", // op 5425
	"JMP I 0026 ;Jump Indexed ZP 0026", // op 5426
	"JMP I 0027 ;Jump Indexed ZP 0027", // op 5427
	"JMP I 0030 ;Jump Indexed ZP 0030", // op 5430
	"JMP I 0031 ;Jump Indexed ZP 0031", // op 5431
	"JMP I 0032 ;Jump Indexed ZP 0032", // op 5432
	"JMP I 0033 ;Jump Indexed ZP 0033", // op 5433
	"JMP I 0034 ;Jump Indexed ZP 0034", // op 5434
	"JMP I 0035 ;Jump Indexed ZP 0035", // op 5435
	"JMP I 0036 ;Jump Indexed ZP 0036", // op 5436
	"JMP I 0037 ;Jump Indexed ZP 0037", // op 5437
	"JMP I 0040 ;Jump Indexed ZP 0040", // op 5440
	"JMP I 0041 ;Jump Indexed ZP 0041", // op 5441
	"JMP I 0042 ;Jump Indexed ZP 0042", // op 5442
	"JMP I 0043 ;Jump Indexed ZP 0043", // op 5443
	"JMP I 0044 ;Jump Indexed ZP 0044", // op 5444
	"JMP I 0045 ;Jump Indexed ZP 0045", // op 5445
	"JMP I 0046 ;Jump Indexed ZP 0046", // op 5446
	"JMP I 0047 ;Jump Indexed ZP 0047", // op 5447
	"JMP I 0050 ;Jump Indexed ZP 0050", // op 5450
	"JMP I 0051 ;Jump Indexed ZP 0051", // op 5451
	"JMP I 0052 ;Jump Indexed ZP 0052", // op 5452
	"JMP I 0053 ;Jump Indexed ZP 0053", // op 5453
	"JMP I 0054 ;Jump Indexed ZP 0054", // op 5454
	"JMP I 0055 ;Jump Indexed ZP 0055", // op 5455
	"JMP I 0056 ;Jump Indexed ZP 0056", // op 5456
	"JMP I 0057 ;Jump Indexed ZP 0057", // op 5457
	"JMP I 0060 ;Jump Indexed ZP 0060", // op 5460
	"JMP I 0061 ;Jump Indexed ZP 0061", // op 5461
	"JMP I 0062 ;Jump Indexed ZP 0062", // op 5462
	"JMP I 0063 ;Jump Indexed ZP 0063", // op 5463
	"JMP I 0064 ;Jump Indexed ZP 0064", // op 5464
	"JMP I 0065 ;Jump Indexed ZP 0065", // op 5465
	"JMP I 0066 ;Jump Indexed ZP 0066", // op 5466
	"JMP I 0067 ;Jump Indexed ZP 0067", // op 5467
	"JMP I 0070 ;Jump Indexed ZP 0070", // op 5470
	"JMP I 0071 ;Jump Indexed ZP 0071", // op 5471
	"JMP I 0072 ;Jump Indexed ZP 0072", // op 5472
	"JMP I 0073 ;Jump Indexed ZP 0073", // op 5473
	"JMP I 0074 ;Jump Indexed ZP 0074", // op 5474
	"JMP I 0075 ;Jump Indexed ZP 0075", // op 5475
	"JMP I 0076 ;Jump Indexed ZP 0076", // op 5476
	"JMP I 0077 ;Jump Indexed ZP 0077", // op 5477
	"JMP I 0100 ;Jump Indexed ZP 0100", // op 5500
	"JMP I 0101 ;Jump Indexed ZP 0101", // op 5501
	"JMP I 0102 ;Jump Indexed ZP 0102", // op 5502
	"JMP I 0103 ;Jump Indexed ZP 0103", // op 5503
	"JMP I 0104 ;Jump Indexed ZP 0104", // op 5504
	"JMP I 0105 ;Jump Indexed ZP 0105", // op 5505
	"JMP I 0106 ;Jump Indexed ZP 0106", // op 5506
	"JMP I 0107 ;Jump Indexed ZP 0107", // op 5507
	"JMP I 0110 ;Jump Indexed ZP 0110", // op 5510
	"JMP I 0111 ;Jump Indexed ZP 0111", // op 5511
	"JMP I 0112 ;Jump Indexed ZP 0112", // op 5512
	"JMP I 0113 ;Jump Indexed ZP 0113", // op 5513
	"JMP I 0114 ;Jump Indexed ZP 0114", // op 5514
	"JMP I 0115 ;Jump Indexed ZP 0115", // op 5515
	"JMP I 0116 ;Jump Indexed ZP 0116", // op 5516
	"JMP I 0117 ;Jump Indexed ZP 0117", // op 5517
	"JMP I 0120 ;Jump Indexed ZP 0120", // op 5520
	"JMP I 0121 ;Jump Indexed ZP 0121", // op 5521
	"JMP I 0122 ;Jump Indexed ZP 0122", // op 5522
	"JMP I 0123 ;Jump Indexed ZP 0123", // op 5523
	"JMP I 0124 ;Jump Indexed ZP 0124", // op 5524
	"JMP I 0125 ;Jump Indexed ZP 0125", // op 5525
	"JMP I 0126 ;Jump Indexed ZP 0126", // op 5526
	"JMP I 0127 ;Jump Indexed ZP 0127", // op 5527
	"JMP I 0130 ;Jump Indexed ZP 0130", // op 5530
	"JMP I 0131 ;Jump Indexed ZP 0131", // op 5531
	"JMP I 0132 ;Jump Indexed ZP 0132", // op 5532
	"JMP I 0133 ;Jump Indexed ZP 0133", // op 5533
	"JMP I 0134 ;Jump Indexed ZP 0134", // op 5534
	"JMP I 0135 ;Jump Indexed ZP 0135", // op 5535
	"JMP I 0136 ;Jump Indexed ZP 0136", // op 5536
	"JMP I 0137 ;Jump Indexed ZP 0137", // op 5537
	"JMP I 0140 ;Jump Indexed ZP 0140", // op 5540
	"JMP I 0141 ;Jump Indexed ZP 0141", // op 5541
	"JMP I 0142 ;Jump Indexed ZP 0142", // op 5542
	"JMP I 0143 ;Jump Indexed ZP 0143", // op 5543
	"JMP I 0144 ;Jump Indexed ZP 0144", // op 5544
	"JMP I 0145 ;Jump Indexed ZP 0145", // op 5545
	"JMP I 0146 ;Jump Indexed ZP 0146", // op 5546
	"JMP I 0147 ;Jump Indexed ZP 0147", // op 5547
	"JMP I 0150 ;Jump Indexed ZP 0150", // op 5550
	"JMP I 0151 ;Jump Indexed ZP 0151", // op 5551
	"JMP I 0152 ;Jump Indexed ZP 0152", // op 5552
	"JMP I 0153 ;Jump Indexed ZP 0153", // op 5553
	"JMP I 0154 ;Jump Indexed ZP 0154", // op 5554
	"JMP I 0155 ;Jump Indexed ZP 0155", // op 5555
	"JMP I 0156 ;Jump Indexed ZP 0156", // op 5556
	"JMP I 0157 ;Jump Indexed ZP 0157", // op 5557
	"JMP I 0160 ;Jump Indexed ZP 0160", // op 5560
	"JMP I 0161 ;Jump Indexed ZP 0161", // op 5561
	"JMP I 0162 ;Jump Indexed ZP 0162", // op 5562
	"JMP I 0163 ;Jump Indexed ZP 0163", // op 5563
	"JMP I 0164 ;Jump Indexed ZP 0164", // op 5564
	"JMP I 0165 ;Jump Indexed ZP 0165", // op 5565
	"JMP I 0166 ;Jump Indexed ZP 0166", // op 5566
	"JMP I 0167 ;Jump Indexed ZP 0167", // op 5567
	"JMP I 0170 ;Jump Indexed ZP 0170", // op 5570
	"JMP I 0171 ;Jump Indexed ZP 0171", // op 5571
	"JMP I 0172 ;Jump Indexed ZP 0172", // op 5572
	"JMP I 0173 ;Jump Indexed ZP 0173", // op 5573
	"JMP I 0174 ;Jump Indexed ZP 0174", // op 5574
	"JMP I 0175 ;Jump Indexed ZP 0175", // op 5575
	"JMP I 0176 ;Jump Indexed ZP 0176", // op 5576
	"JMP I 0177 ;Jump Indexed ZP 0177", // op 5577
	"JMP I @@00 ;Jump Indexed Current page @@00", // op 5600
	"JMP I @@01 ;Jump Indexed Current page @@01", // op 5601
	"JMP I @@02 ;Jump Indexed Current page @@02", // op 5602
	"JMP I @@03 ;Jump Indexed Current page @@03", // op 5603
	"JMP I @@04 ;Jump Indexed Current page @@04", // op 5604
	"JMP I @@05 ;Jump Indexed Current page @@05", // op 5605
	"JMP I @@06 ;Jump Indexed Current page @@06", // op 5606
	"JMP I @@07 ;Jump Indexed Current page @@07", // op 5607
	"JMP I @@10 ;Jump Indexed Current page @@10", // op 5610
	"JMP I @@11 ;Jump Indexed Current page @@11", // op 5611
	"JMP I @@12 ;Jump Indexed Current page @@12", // op 5612
	"JMP I @@13 ;Jump Indexed Current page @@13", // op 5613
	"JMP I @@14 ;Jump Indexed Current page @@14", // op 5614
	"JMP I @@15 ;Jump Indexed Current page @@15", // op 5615
	"JMP I @@16 ;Jump Indexed Current page @@16", // op 5616
	"JMP I @@17 ;Jump Indexed Current page @@17", // op 5617
	"JMP I @@20 ;Jump Indexed Current page @@20", // op 5620
	"JMP I @@21 ;Jump Indexed Current page @@21", // op 5621
	"JMP I @@22 ;Jump Indexed Current page @@22", // op 5622
	"JMP I @@23 ;Jump Indexed Current page @@23", // op 5623
	"JMP I @@24 ;Jump Indexed Current page @@24", // op 5624
	"JMP I @@25 ;Jump Indexed Current page @@25", // op 5625
	"JMP I @@26 ;Jump Indexed Current page @@26", // op 5626
	"JMP I @@27 ;Jump Indexed Current page @@27", // op 5627
	"JMP I @@30 ;Jump Indexed Current page @@30", // op 5630
	"JMP I @@31 ;Jump Indexed Current page @@31", // op 5631
	"JMP I @@32 ;Jump Indexed Current page @@32", // op 5632
	"JMP I @@33 ;Jump Indexed Current page @@33", // op 5633
	"JMP I @@34 ;Jump Indexed Current page @@34", // op 5634
	"JMP I @@35 ;Jump Indexed Current page @@35", // op 5635
	"JMP I @@36 ;Jump Indexed Current page @@36", // op 5636
	"JMP I @@37 ;Jump Indexed Current page @@37", // op 5637
	"JMP I @@40 ;Jump Indexed Current page @@40", // op 5640
	"JMP I @@41 ;Jump Indexed Current page @@41", // op 5641
	"JMP I @@42 ;Jump Indexed Current page @@42", // op 5642
	"JMP I @@43 ;Jump Indexed Current page @@43", // op 5643
	"JMP I @@44 ;Jump Indexed Current page @@44", // op 5644
	"JMP I @@45 ;Jump Indexed Current page @@45", // op 5645
	"JMP I @@46 ;Jump Indexed Current page @@46", // op 5646
	"JMP I @@47 ;Jump Indexed Current page @@47", // op 5647
	"JMP I @@50 ;Jump Indexed Current page @@50", // op 5650
	"JMP I @@51 ;Jump Indexed Current page @@51", // op 5651
	"JMP I @@52 ;Jump Indexed Current page @@52", // op 5652
	"JMP I @@53 ;Jump Indexed Current page @@53", // op 5653
	"JMP I @@54 ;Jump Indexed Current page @@54", // op 5654
	"JMP I @@55 ;Jump Indexed Current page @@55", // op 5655
	"JMP I @@56 ;Jump Indexed Current page @@56", // op 5656
	"JMP I @@57 ;Jump Indexed Current page @@57", // op 5657
	"JMP I @@60 ;Jump Indexed Current page @@60", // op 5660
	"JMP I @@61 ;Jump Indexed Current page @@61", // op 5661
	"JMP I @@62 ;Jump Indexed Current page @@62", // op 5662
	"JMP I @@63 ;Jump Indexed Current page @@63", // op 5663
	"JMP I @@64 ;Jump Indexed Current page @@64", // op 5664
	"JMP I @@65 ;Jump Indexed Current page @@65", // op 5665
	"JMP I @@66 ;Jump Indexed Current page @@66", // op 5666
	"JMP I @@67 ;Jump Indexed Current page @@67", // op 5667
	"JMP I @@70 ;Jump Indexed Current page @@70", // op 5670
	"JMP I @@71 ;Jump Indexed Current page @@71", // op 5671
	"JMP I @@72 ;Jump Indexed Current page @@72", // op 5672
	"JMP I @@73 ;Jump Indexed Current page @@73", // op 5673
	"JMP I @@74 ;Jump Indexed Current page @@74", // op 5674
	"JMP I @@75 ;Jump Indexed Current page @@75", // op 5675
	"JMP I @@76 ;Jump Indexed Current page @@76", // op 5676
	"JMP I @@77 ;Jump Indexed Current page @@77", // op 5677
	"JMP I @@00 ;Jump Indexed Current page @@00", // op 5700
	"JMP I @@01 ;Jump Indexed Current page @@01", // op 5701
	"JMP I @@02 ;Jump Indexed Current page @@02", // op 5702
	"JMP I @@03 ;Jump Indexed Current page @@03", // op 5703
	"JMP I @@04 ;Jump Indexed Current page @@04", // op 5704
	"JMP I @@05 ;Jump Indexed Current page @@05", // op 5705
	"JMP I @@06 ;Jump Indexed Current page @@06", // op 5706
	"JMP I @@07 ;Jump Indexed Current page @@07", // op 5707
	"JMP I @@10 ;Jump Indexed Current page @@10", // op 5710
	"JMP I @@11 ;Jump Indexed Current page @@11", // op 5711
	"JMP I @@12 ;Jump Indexed Current page @@12", // op 5712
	"JMP I @@13 ;Jump Indexed Current page @@13", // op 5713
	"JMP I @@14 ;Jump Indexed Current page @@14", // op 5714
	"JMP I @@15 ;Jump Indexed Current page @@15", // op 5715
	"JMP I @@16 ;Jump Indexed Current page @@16", // op 5716
	"JMP I @@17 ;Jump Indexed Current page @@17", // op 5717
	"JMP I @@20 ;Jump Indexed Current page @@20", // op 5720
	"JMP I @@21 ;Jump Indexed Current page @@21", // op 5721
	"JMP I @@22 ;Jump Indexed Current page @@22", // op 5722
	"JMP I @@23 ;Jump Indexed Current page @@23", // op 5723
	"JMP I @@24 ;Jump Indexed Current page @@24", // op 5724
	"JMP I @@25 ;Jump Indexed Current page @@25", // op 5725
	"JMP I @@26 ;Jump Indexed Current page @@26", // op 5726
	"JMP I @@27 ;Jump Indexed Current page @@27", // op 5727
	"JMP I @@30 ;Jump Indexed Current page @@30", // op 5730
	"JMP I @@31 ;Jump Indexed Current page @@31", // op 5731
	"JMP I @@32 ;Jump Indexed Current page @@32", // op 5732
	"JMP I @@33 ;Jump Indexed Current page @@33", // op 5733
	"JMP I @@34 ;Jump Indexed Current page @@34", // op 5734
	"JMP I @@35 ;Jump Indexed Current page @@35", // op 5735
	"JMP I @@36 ;Jump Indexed Current page @@36", // op 5736
	"JMP I @@37 ;Jump Indexed Current page @@37", // op 5737
	"JMP I @@40 ;Jump Indexed Current page @@40", // op 5740
	"JMP I @@41 ;Jump Indexed Current page @@41", // op 5741
	"JMP I @@42 ;Jump Indexed Current page @@42", // op 5742
	"JMP I @@43 ;Jump Indexed Current page @@43", // op 5743
	"JMP I @@44 ;Jump Indexed Current page @@44", // op 5744
	"JMP I @@45 ;Jump Indexed Current page @@45", // op 5745
	"JMP I @@46 ;Jump Indexed Current page @@46", // op 5746
	"JMP I @@47 ;Jump Indexed Current page @@47", // op 5747
	"JMP I @@50 ;Jump Indexed Current page @@50", // op 5750
	"JMP I @@51 ;Jump Indexed Current page @@51", // op 5751
	"JMP I @@52 ;Jump Indexed Current page @@52", // op 5752
	"JMP I @@53 ;Jump Indexed Current page @@53", // op 5753
	"JMP I @@54 ;Jump Indexed Current page @@54", // op 5754
	"JMP I @@55 ;Jump Indexed Current page @@55", // op 5755
	"JMP I @@56 ;Jump Indexed Current page @@56", // op 5756
	"JMP I @@57 ;Jump Indexed Current page @@57", // op 5757
	"JMP I @@60 ;Jump Indexed Current page @@60", // op 5760
	"JMP I @@61 ;Jump Indexed Current page @@61", // op 5761
	"JMP I @@62 ;Jump Indexed Current page @@62", // op 5762
	"JMP I @@63 ;Jump Indexed Current page @@63", // op 5763
	"JMP I @@64 ;Jump Indexed Current page @@64", // op 5764
	"JMP I @@65 ;Jump Indexed Current page @@65", // op 5765
	"JMP I @@66 ;Jump Indexed Current page @@66", // op 5766
	"JMP I @@67 ;Jump Indexed Current page @@67", // op 5767
	"JMP I @@70 ;Jump Indexed Current page @@70", // op 5770
	"JMP I @@71 ;Jump Indexed Current page @@71", // op 5771
	"JMP I @@72 ;Jump Indexed Current page @@72", // op 5772
	"JMP I @@73 ;Jump Indexed Current page @@73", // op 5773
	"JMP I @@74 ;Jump Indexed Current page @@74", // op 5774
	"JMP I @@75 ;Jump Indexed Current page @@75", // op 5775
	"JMP I @@76 ;Jump Indexed Current page @@76", // op 5776
	"JMP I @@77 ;Jump Indexed Current page @@77", // op 5777
	"SKON       ;KK8-E: Skip if interrupts on, and turn interrupts off", // op 6000
	"ION        ;KK8-E: Interrupts enabled", // op 6001
	"IOF        ;KK8-E: Interrupts disabled", // op 6002
	"SRQ        ;KK8-E: Skip interrupt request", // op 6003
	"GTF        ;KK8-E: Get interrupt flags", // op 6004
	"RTF        ;KK8-E: Restore interrupt flags", // op 6005
	"SGT        ;KK8-E: Skip on greater-than flag", // op 6006
	"CAF        ;KK8-E: Clear all flags", // op 6007
	"RPE        ;PR8-E: Set ReaderPunch Interrupt Enable", // op 6010
	"RSF        ;PR8-E: Skip on Reader Flag", // op 6011
	"RRB        ;PR8-E: Read Reader Buffer", // op 6012
	"           ;", // op 6013
	"RFC        ;PR8-E: Reader Fetch Character", // op 6014
	"           ;", // op 6015
	"RRB, RFC   ;PR8-E: Read Reader Buffer & Fetch New Character", // op 6016
	"           ;", // op 6017
	"PCE        ;PR8-E: Clear ReaderPunch Interrupt Enable", // op 6020
	"PSF        ;PP8-E: Skip on Punch Flag", // op 6021
	"PCF        ;PP8-E: Clear Punch Flag", // op 6022
	"           ; ", // op 6023
	"@@C        ;PP8-E: Load Punch Buffer and Punch Character", // op 6024
	"           ; ", // op 6025
	"PLS        ;PP8-E: Load Punch Sequence", // op 6026
	"           ; ", // op 6027
	"KCF        ;KL8-E: Clear Keyboard Flag", // op 6030
	"KSF        ;KL8-E: Skip on Keyboard Flag", // op 6031
	"KCC        ;KL8-E: Clear Keyboard Flag", // op 6032
	"           ; ", // op 6033
	"KRS        ;KL8-E: Read Keyboard Buffer Static", // op 6034
	"KIE        ;KL8-E: SetClear Keyboard Interrupt Enable", // op 6035
	"KRB        ;KL8-E: Read Keyboard Buffer Dynamic", // op 6036
	"           ; ", // op 6037
	"TFL        ;KL8-E: Set Teleprinter Flag", // op 6040
	"TSF        ;KL8-E: Skip on Teleprinter Flag", // op 6041
	"TCF        ;KL8-E: Clear Teleprinter Flag", // op 6042
	"           ; ", // op 6043
	"TPC        ;KL8-E: Load Teleprinter and Print", // op 6044
	"TSK        ;KL8-E: Skip on Keyboard or Printer Flag", // op 6045
	"TLS        ;KL8-E: Load Teleprinter Sequence", // op 6046
	"           ; ", // op 6047
	"DILC       ;VC8-E: Clear All Logic", // op 6050
	"DICD       ;VC8-E: Clear Done Flag", // op 6051
	"DISD       ;VC8-E: Skip on Done Flag", // op 6052
	"DILX       ;VC8-E: Load X Register", // op 6053
	"DILY       ;VC8-E: Load Y Register", // op 6054
	"           ;", // op 6055
	"DILE       ;VC8-E: Load Enable", // op 6056
	"DIRE       ;VC8-E: Read EnableStatus Register", // op 6057
	"           ;", // op 6060
	"WTCD       ;VW01: Clear Data Ready Flag", // op 6061
	"WTRY       ;VW01: Read Y", // op 6062
	"           ;", // op 6063
	"WTMN       ;VW01: Clear Set XY", // op 6064
	"           ;", // op 6065
	"           ;", // op 6066
	"           ;", // op 6067
	"           ;", // op 6070
	"WTSK       ;VW01: Writing Tablet Skip", // op 6071
	"WTRS       ;VW01: Read Status", // op 6072
	"           ;", // op 6073
	"WTSE       ;VW01: Select Tablet", // op 6074
	"           ;", // op 6075
	"           ;", // op 6076
	"           ;", // op 6077
	"DPI        ;MP8-E: Disable Memory Parity Interrupt", // op 6100
	"SMP        ;MP8-E: Skip on No Memory Parity Error", // op 6101
	"SPL        ;KP8-E: Skip on Low Power", // op 6102
	"EPI        ;MP8-E: Enable Memory Parity Error Interrupt", // op 6103
	"CMP        ;MP8-E: Clear Memory Parity Error Flag", // op 6104
	"SMP CMP    ;MP8-E: Skip on no Memory Parity Error & Clear Memory Parity Error Flag", // op 6105
	"CEP        ;MP8-E: Check for Even Parity", // op 6106
	"SPO        ;MP8-E: Skip on Memory Parity Option", // op 6107
	"RCTV       ;DP8-EP: Test VRC and Skip", // op 6110
	"RCRH       ;DP8-EP: Read BCC High", // op 6111
	"RLRL       ;DP8-EP: Read BCC Low", // op 6112
	"RCCV       ;DP8-EP: Compute VRC", // op 6113
	"RCGB       ;DP8-EP: Generate BCC", // op 6114
	"MINT       ;DC02-F: Set Interrupt Flip-Flop", // op 6115
	"RCCB       ;DP8-EP: Clear BCC Accumulation", // op 6116
	"RCTC       ;DP8-EP: Maintenance Test Clock", // op 6117
	"           ;", // op 6120
	"MTSF       ;DC02-F: Skip on Transmitter Flag", // op 6121
	"MTCF       ;DC02-F: Clear Transmitter Flag", // op 6122
	"MTKF       ;DC02-F: Read Receiver Flag Status", // op 6123
	"MTPC       ;DC02-F: Transmit Operation", // op 6124
	"MINS       ;DC02-F: Skip on Interrupt Request", // op 6125
	"MTCF MTPC  ;DC02-F: Combined MTCF & MTPC", // op 6126
	"MTRS       ;DC02-F: Read Station Status", // op 6127
	"CLZE       ;DK8-EC: Clear Clock Enable Register per AC", // op 6130
	"CLEI       ;DK8-EA: Enable clock Interrupt", // op 6131
	"CLDI       ;DK8-EA: Disable Clock Interrupt;Octal error)", // op 6132
	"CLSK       ;DK8-EA: Skip on Clock Flag an Clear Flag", // op 6133
	"CLEN       ;DK8-EC: Load Clock Enable Register", // op 6134
	"CLSA       ;DK8-EC: Clock Status to AC", // op 6135
	"CLBA       ;DK8-EC: Clock Buffer to AC", // op 6136
	"CLCA       ;DK8-EC: Clock Counter to AC", // op 6137
	"           ;", // op 6140
	"           ;", // op 6141
	"           ;", // op 6142
	"           ;", // op 6143
	"           ;", // op 6144
	"           ;", // op 6145
	"           ;", // op 6146
	"           ;", // op 6147
	"           ;", // op 6150
	"           ;", // op 6151
	"           ;", // op 6152
	"           ;", // op 6153
	"           ;", // op 6154
	"           ;", // op 6155
	"           ;", // op 6156
	"           ;", // op 6157
	"           ;", // op 6160
	"           ;", // op 6161
	"           ;", // op 6162
	"           ;", // op 6163
	"           ;", // op 6164
	"           ;", // op 6165
	"           ;", // op 6166
	"           ;", // op 6167
	"           ;", // op 6170
	"           ;", // op 6171
	"           ;", // op 6172
	"           ;", // op 6173
	"           ;", // op 6174
	"           ;", // op 6175
	"           ;", // op 6176
	"           ;", // op 6177
	"           ;", // op 6200
	"CDF 0      ;KM8-E: Change to Data Field 0", // op 6201
	"CIF 0      ;KM8-E: Change to Instruction Field 0", // op 6202
	"CDF 0 CIF 0;KM8-E: Change to Data and Instruction Field 0", // op 6203
	"CINT       ;KM8-E: Clear User Interrupt", // op 6204
	"           ;", // op 6205
	"           ;", // op 6206
	"           ;", // op 6207
	"           ;", // op 6210
	"CDF 1      ;KM8-E: Change to Data Field 1", // op 6211
	"CIF 1      ;KM8-E: Change to Instruction Field 1", // op 6212
	"CDF 1 CIF 1;KM8-E: Change to Data and Instruction Field 1", // op 6213
	"RDF        ;KM8-E: Read Data Field", // op 6214
	"           ;", // op 6215
	"           ;", // op 6216
	"           ;", // op 6217
	"           ;", // op 6220
	"CDF 2      ;KM8-E: Change to Data Field 2", // op 6221
	"CIF 2      ;KM8-E: Change to Instruction Field 2", // op 6222
	"CDF 2 CIF 2;KM8-E: Change to Data and Instruction Field 2", // op 6223
	"RIF        ;KM8-E: Read Instruction Field", // op 6224
	"           ;", // op 6225
	"           ;", // op 6226
	"           ;", // op 6227
	"           ;", // op 6230
	"CDF 3      ;KM8-E: Change to Data Field 3", // op 6231
	"CIF 3      ;KM8-E: Change to Instruction Field 3", // op 6232
	"CDF 3 CIF 3;KM8-E: Change to Data and Instruction Field 3", // op 6233
	"RIB        ;KM8-E: Read Interrupt Buffer", // op 6234
	"           ;", // op 6235
	"           ;", // op 6236
	"           ;", // op 6237
	"           ;", // op 6240
	"CDF 4      ;KM8-E: Change to Data Field 4", // op 6241
	"CIF 4      ;KM8-E: Change to Instruction Field 4", // op 6242
	"CDF 4 CIF 4;KM8-E: Change to Data and Instruction Field 4", // op 6243
	"RMF        ;KM8-E: Restore Memory Field", // op 6244
	"           ;", // op 6245
	"           ;", // op 6246
	"           ;", // op 6247
	"           ;", // op 6250
	"CDF 5      ;KM8-E: Change to Data Field 5", // op 6251
	"CIF 5      ;KM8-E: Change to Instruction Field 5", // op 6252
	"CDF 5 CIF 5;KM8-E: Change to Data and Instruction Field 5", // op 6253
	"SINT       ;KM8-E: Skip on User Interrupt", // op 6254
	"           ;", // op 6255
	"           ;", // op 6256
	"           ;", // op 6257
	"           ;", // op 6260
	"CDF 6      ;KM8-E: Change to Data Field 6", // op 6261
	"CIF 6      ;KM8-E: Change to Instruction Field 6", // op 6262
	"CDF 6 CIF 6;KM8-E: Change to Data and Instruction Field 6", // op 6263
	"CUF        ;KM8-E: Clear User Flag", // op 6264
	"           ;", // op 6265
	"           ;", // op 6266
	"           ;", // op 6267
	"           ;", // op 6270
	"CDF 7      ;KM8-E: Change to Data Field 7", // op 6271
	"CIF 7      ;KM8-E: Change to Instruction Field 7", // op 6272
	"CDF 7 CIF 7;KM8-E: Change to Data and Instruction Field 7", // op 6273
	"UNKNOWN!   ;KM8-E: Set User Buffer Flip Flop, Block Interrupts", // op 6274
	"           ;", // op 6275
	"           ;", // op 6276
	"           ;", // op 6277
	"           ;", // op 6300
	"           ;", // op 6301
	"           ;", // op 6302
	"           ;", // op 6303
	"           ;", // op 6304
	"           ;", // op 6305
	"           ;", // op 6306
	"           ;", // op 6307
	"           ;", // op 6310
	"           ;", // op 6311
	"           ;", // op 6312
	"           ;", // op 6313
	"           ;", // op 6314
	"           ;", // op 6315
	"           ;", // op 6316
	"           ;", // op 6317
	"           ;", // op 6320
	"           ;", // op 6321
	"           ;", // op 6322
	"           ;", // op 6323
	"           ;", // op 6324
	"           ;", // op 6325
	"           ;", // op 6326
	"           ;", // op 6327
	"           ;", // op 6330
	"           ;", // op 6331
	"           ;", // op 6332
	"           ;", // op 6333
	"           ;", // op 6334
	"           ;", // op 6335
	"           ;", // op 6336
	"           ;", // op 6337
	"           ;", // op 6340
	"           ;", // op 6341
	"           ;", // op 6342
	"           ;", // op 6343
	"           ;", // op 6344
	"           ;", // op 6345
	"           ;", // op 6346
	"           ;", // op 6347
	"           ;", // op 6350
	"UDSS       ;UDC: Skip on Scan Not Busy", // op 6351
	"           ;", // op 6352
	"UDSC       ;UDC: Start Interrupt Scan", // op 6353
	"           ;", // op 6354
	"           ;", // op 6355
	"UDRA       ;UDC: Read Address and Generic Type", // op 6356
	"UDLS       ;UDC: Load Previous Status", // op 6357
	"           ;", // op 6360
	"UDSF       ;UDC: Skip on UDC Flag and Clear Flag", // op 6361
	"GCTF       ;BB08-P: Clear Transmit Flag", // op 6362
	"UDLA       ;UDC: Load Address", // op 6363
	"UDEI       ;UDC: Enable UDC Interrupt Flasg", // op 6364
	"UDDI       ;UDC: Disable UDC Interrupt Flag", // op 6365
	"UDRD       ;UDC: Clear AC and Read Data", // op 6366
	"UDLD       ;UDC: Load Data and Clear AC", // op 6367
	"           ;", // op 6370
	"GRSF       ;BB08-P: Skip on Receive Flag", // op 6371
	"GCRF       ;BB08-P: Clear Receive Flag", // op 6372
	"           ;", // op 6373
	"GRDB       ;BB08-P: Read Device Buffer", // op 6374
	"           ;", // op 6375
	"           ;", // op 6376
	"           ;", // op 6377
	"SSCD       ;DP8-EAEB: Skip if Character Detected", // op 6400
	"SCSI       ;DP8-EAEB: Clear Synchrnous Interface", // op 6401
	"SSRO       ;DP8-EAEB: Skip If Receive Word Count Overflow", // op 6402
	"SSTO       ;DP8-EAEB: Skip of Transmit Word Count Overflow", // op 6403
	"SGRR       ;DP8-EAEB: Receuve Go", // op 6404
	"SGTT       ;DP8-EAEB: Transmit Go", // op 6405
	"SCSD       ;DP8-EAEB: Clear Sync Detect", // op 6406
	"SRTA       ;DP8-EAEB: Read Transfer Address Register", // op 6407
	"SSRG       ;DP8-EAEB: Skip if Ring Flag", // op 6410
	"SSCA       ;DP8-EAEB: Skip if CarrierAGC Flag", // op 6411
	"SLCC       ;DP8-EAEB: Load Control", // op 6412
	"SLFL       ;DP8-EAEB: Load Field", // op 6413
	"SRS2       ;DP8-EAEB: Read Status 2", // op 6414
	"SRS1       ;DP8-EAEB: Read Status 1", // op 6415
	"SSBE       ;DP8-EAEB: Skip on Bus Error", // op 6416
	"SRCD       ;DP8-EAEB: Read Character Detected", // op 6417
	"SSCD       ;DP8-EAEB: Skip if Character Detected", // op 6420
	"SCSI       ;DP8-EAEB: Clear Synchrnous Interface", // op 6421
	"           ;", // op 6422
	"SSTO       ;DP8-EAEB: Skip of Transmit Word Count Overflow", // op 6423
	"SGRR       ;DP8-EAEB: Receuve Go", // op 6424
	"SGTT       ;DP8-EAEB: Transmit Go", // op 6425
	"SCSD       ;DP8-EAEB: Clear Sync Detect", // op 6426
	"SRTA       ;DP8-EAEB: Read Transfer Address Register", // op 6427
	"SSRG       ;DP8-EAEB: Skip if Ring Flag", // op 6430
	"SSCA       ;DP8-EAEB: Skip if CarrierAGC Flag", // op 6431
	"SLCC       ;DP8-EAEB: Load Control", // op 6432
	"SLFL       ;DP8-EAEB: Load Field", // op 6433
	"SRS2       ;DP8-EAEB: Read Status 2", // op 6434
	"SRS1       ;DP8-EAEB: Read Status 1", // op 6435
	"SSBE       ;DP8-EAEB: Skip on Bus Error", // op 6436
	"SRCD       ;DP8-EAEB: Read Character Detected", // op 6437
	"SSCD       ;DP8-EAEB: Skip if Character Detected", // op 6440
	"SCSI       ;DP8-EAEB: Clear Synchrnous Interface", // op 6441
	"           ;", // op 6442
	"SSTO       ;DP8-EAEB: Skip of Transmit Word Count Overflow", // op 6443
	"SGRR       ;DP8-EAEB: Receuve Go", // op 6444
	"SGTT       ;DP8-EAEB: Transmit Go", // op 6445
	"SCSD       ;DP8-EAEB: Clear Sync Detect", // op 6446
	"SRTA       ;DP8-EAEB: Read Transfer Address Register", // op 6447
	"SSRG       ;DP8-EAEB: Skip if Ring Flag", // op 6450
	"SSCA       ;DP8-EAEB: Skip if CarrierAGC Flag", // op 6451
	"SLCC       ;DP8-EAEB: Load Control", // op 6452
	"SLFL       ;DP8-EAEB: Load Field", // op 6453
	"SRS2       ;DP8-EAEB: Read Status 2", // op 6454
	"SRS1       ;DP8-EAEB: Read Status 1", // op 6455
	"SSBE       ;DP8-EAEB: Skip on Bus Error", // op 6456
	"SRCD       ;DP8-EAEB: Read Character Detected", // op 6457
	"SSCD       ;DP8-EAEB: Skip if Character Detected", // op 6460
	"SCSI       ;DP8-EAEB: Clear Synchrnous Interface", // op 6461
	"           ;", // op 6462
	"SSTO       ;DP8-EAEB: Skip of Transmit Word Count Overflow", // op 6463
	"SSRO       ;DP8-EAEB: Skip If Receive Word Count Overflow", // op 6464
	"SGTT       ;DP8-EAEB: Transmit Go", // op 6465
	"SCSD       ;DP8-EAEB: Clear Sync Detect", // op 6466
	"SRTA       ;DP8-EAEB: Read Transfer Address Register", // op 6467
	"SSRG       ;DP8-EAEB: Skip if Ring Flag", // op 6470
	"SSCA       ;DP8-EAEB: Skip if CarrierAGC Flag", // op 6471
	"SLCC       ;DP8-EAEB: Load Control", // op 6472
	"SLFL       ;DP8-EAEB: Load Field", // op 6473
	"SRS2       ;DP8-EAEB: Read Status 2", // op 6474
	"SRS1       ;DP8-EAEB: Read Status 1", // op 6475
	"SSBE       ;DP8-EAEB: Skip on Bus Error", // op 6476
	"SRCD       ;DP8-EAEB: Read Character Detected", // op 6477
	"PLCE       ;XY8E: Clear Interrupt Enable", // op 6500
	"PLSF       ;XY8E: Skip on Plotter Flag", // op 6501
	"PLCF       ;XY8E: Clear Plotter Flag", // op 6502
	"PLPU       ;XY8E: Pen Up", // op 6503
	"PLLR       ;XY8E: Load Direction Flag, Set Flag", // op 6504
	"PLPD       ;XY8E: Pen Down", // op 6505
	"PLCF PLLR  ;XY8E: Clear Plotter Flag, Load Direction Flag, Set Flag", // op 6506
	"PLSE       ;XY8E: Set Interrupt Enable Flag", // op 6507
	"           ;", // op 6510
	"           ;", // op 6511
	"           ;", // op 6512
	"           ;", // op 6513
	"           ;", // op 6514
	"           ;", // op 6515
	"           ;", // op 6516
	"           ;", // op 6517
	"           ;", // op 6520
	"           ;", // op 6521
	"           ;", // op 6522
	"           ;", // op 6523
	"           ;", // op 6524
	"           ;", // op 6525
	"           ;", // op 6526
	"           ;", // op 6527
	"ADCL       ;AD8-EA: Clear All", // op 6530
	"ADLM       ;AD8-EA: Load Multiplexer", // op 6531
	"ADST       ;AD8-EA: Start Conversion", // op 6532
	"ADRB       ;AD8-EA: Read AD Buffer", // op 6533
	"ADSK       ;AD8-EA: Skip on AD Done", // op 6534
	"ADSE       ;AD8-EA: Skip on Error", // op 6535
	"ADLE       ;AD8-EA: Load Enable Register", // op 6536
	"ADRS       ;AD8-EA: Read Status Register", // op 6537
	"           ;", // op 6540
	"VCNV       ;AF04A: Select Channel and Convert", // op 6541
	"ADSG       ;AFC8: Set Multiplexer Gain", // op 6542
	"           ;", // op 6543
	"ADSA       ;AFC8: Set Multiplexer Address", // op 6544
	"           ;", // op 6545
	"           ;", // op 6546
	"           ;", // op 6547
	"DBDI       ;DR8-EA: Disable Interrupt", // op 6550
	"FPINT      ;FPP-12: Skip on FPP Interrupt Request Flag", // op 6551
	"FPICL      ;FPP-12: Clear FPP Interrupt Request Flag", // op 6552
	"FPCOM      ;FPP-12: If FPP not running, copy AC to Command Register", // op 6553
	"FPHLT      ;FPP-12: Halt FPP after current Instruction", // op 6554
	"FPST       ;FPP-12: Skip and copy AC to Active Param Table if not running", // op 6555
	"FPRST      ;FPP-12: Read FPP Status Register", // op 6556
	"FPIST      ;FPP-12: Skip on FPP Interrupt Rewuest Flag, and Clear Flag", // op 6557
	"           ;", // op 6560
	"VSDR       ;AF04A: Skip on Data Ready", // op 6561
	"VRD        ;AF04A: Read Data and Clear Flag", // op 6562
	"           ;", // op 6563
	"VBA        ;AF04A: Byte Advance", // op 6564
	"           ;", // op 6565
	"           ;", // op 6566
	"           ;", // op 6567
	"           ;", // op 6570
	"VSCC       ;AF04A: Sample Current Channel", // op 6571
	"           ;", // op 6572
	"           ;", // op 6573
	"           ;", // op 6574
	"           ;", // op 6575
	"           ;", // op 6576
	"           ;", // op 6577
	"           ;", // op 6600
	"DCMA       ;DF32-D: Clear Disk Address Register", // op 6601
	"           ;", // op 6602
	"DMAR       ;DF32-D: Load Disk Memory Address Register and Read", // op 6603
	"           ;", // op 6604
	"DMAW       ;DF32-D: Load Disk Memory Address Register and Write", // op 6605
	"           ;", // op 6606
	"           ;", // op 6607
	"           ;", // op 6610
	"DCEA       ;DF32-D: Clear Disk Extended Address Register", // op 6611
	"DSAC       ;DF32-D: Skip on Address Confirmed Flag", // op 6612
	"           ;", // op 6613
	"           ;", // op 6614
	"DEAL       ;DF32-D: Load Disk Extended Address", // op 6615
	"DEAC       ;DF32-D: Read Disk Extended Address Register", // op 6616
	"           ;", // op 6617
	"           ;", // op 6620
	"DFSE       ;DF32-D: Skip on Zero Error Flag", // op 6621
	"DFSC       ;DF32-D: Skip on Data Completion Flag", // op 6622
	"DISK       ;RF08: Skip on Error or Completion Flag", // op 6623
	"           ;", // op 6624
	"           ;", // op 6625
	"DMAC       ;DF32-D: Read Disk Memory Address Register", // op 6626
	"           ;", // op 6627
	"           ;", // op 6630
	"RCSF       ;CR8-E: Skip on Rdata Ready", // op 6631
	"RCRA       ;CR8-E: Read Alphanumeric", // op 6632
	"           ;", // op 6633
	"RCRB       ;CR8-E: Read Binary", // op 6634
	"RCNO       ;CR8-E: Read Conditions out to Card Reader", // op 6635
	"RCRC       ;CR8-E: Read Compressed", // op 6636
	"RCNI       ;CR8-E: Read Conditions in from Card Reader", // op 6637
	"           ;", // op 6640
	"DCXA       ;RF08: Clear High Order Address Register", // op 6641
	"           ;", // op 6642
	"DXAL       ;RF08: Clear and Load High Order Address Register", // op 6643
	"           ;", // op 6644
	"DXAC       ;RF08: Clear AC and Load DAR into AC", // op 6645
	"DMMT       ;RF08: Initiate Maintenance Register", // op 6646
	"           ;", // op 6647
	"           ;", // op 6650
	"           ;", // op 6651
	"           ;", // op 6652
	"           ;", // op 6653
	"           ;", // op 6654
	"           ;", // op 6655
	"           ;", // op 6656
	"           ;", // op 6657
	"           ;", // op 6660
	"PSKF       ;LE8-0: Skip on Character Flag", // op 6661
	"PCLF       ;LE8-0: Clear Character Flag", // op 6662
	"PSKE       ;LE8-0: Skip on Error", // op 6663
	"PSTB       ;LE8-0: Load Printer Buffer, Print on Full Buffer or Control Character", // op 6664
	"PSIE       ;LE8-0: Set Program Interrupt Enable Flag", // op 6665
	"PCLF PSTB  ;LE8-0: Clear Character Flag, Load Printer Buffer and Print", // op 6666
	"PCIE       ;LE8-0: Clear Program Interrupt Enable Flag", // op 6667
	"           ;", // op 6670
	"RCSD       ;CR8-E: Skip on Card Done Flag", // op 6671
	"RCSE       ;CR8-E: Select Card Reader and Skip if Ready", // op 6672
	"           ;", // op 6673
	"RCRD       ;CR8-E: Clear Card Done Flag", // op 6674
	"RCSI       ;CR8-E: Skip if Interrupt Being Generated", // op 6675
	"           ;", // op 6676
	"RCTF       ;CR8-E: Clear Transition Flags", // op 6677
	"           ;", // op 6700
	"LWCR       ;TM8-E: Load Word Count Register", // op 6701
	"CWCR       ;TM8-E: Clear Word Count Register", // op 6702
	"LCAR       ;TM8-E: Load Current Address Register", // op 6703
	"CCAR       ;TM8-E: Clear Current Address", // op 6704
	"LCMR       ;TM8-E: Load Command Register", // op 6705
	"LFGR       ;TM8-E: Load Function Register", // op 6706
	"LDBR       ;TM8-E: Load Data Buffer Register", // op 6707
	"           ;", // op 6710
	"RWCR       ;TM8-E: Read Word Count Register", // op 6711
	"CLT        ;TM8-E: Clear Transport", // op 6712
	"RCAR       ;TM8-E: Read Current Address Register", // op 6713
	"RMSR       ;TM8-E: Read Main Status Register", // op 6714
	"RCMR       ;TM8-E: Read Command Register", // op 6715
	"RFSR       ;TM8-E: Read Function Register & Status", // op 6716
	"RDBR       ;TM8-E: Read Data Buffer", // op 6717
	"           ;", // op 6720
	"SKEF       ;TM8-E: Skip if Error Flag", // op 6721
	"SKCB       ;TM8-E: Skip if Control not Busy", // op 6722
	"SKJD       ;TM8-E: Skip When Job Done", // op 6723
	"SKTR       ;TM8-E: Skip When Tape Ready", // op 6724
	"CLF        ;TM8-E: Clear Controller and Master", // op 6725
	"           ;", // op 6726
	"           ;", // op 6727
	"           ;", // op 6730
	"DLDA       ;RK08-P: Load Disk Address, Maintenance only)", // op 6731
	"DLDC       ;RK08-P: Load Command Register", // op 6732
	"DLDR       ;RK08-P: Load Disk Address and Read", // op 6733
	"DRDA       ;RK08-P: Read Disk Address", // op 6734
	"DLDW       ;RK08-P: Load Disk Address and Write", // op 6735
	"DRDC       ;RK08-P: Read Disk Command Register", // op 6736
	"DCHP       ;RK08-P: Load Disk Address and Check Parity", // op 6737
	"           ;", // op 6740
	"DRDS       ;RK08-P: Read Disk Status Register", // op 6741
	"DCLS       ;RK08-P: Clear Status Register", // op 6742
	"DMNT       ;RK08-P: Load Maintenance Register", // op 6743
	"           ;", // op 6744
	"DSKD       ;RK08-P: Skip on Disk Done", // op 6745
	"           ;", // op 6746
	"DSKE       ;RK08-P: Skip on Disk Error", // op 6747
	"           ;", // op 6750
	"DCLA       ;RK08-P: Clear All", // op 6751
	"DRWC       ;RK08-P: Read Word Count Register", // op 6752
	"DLWC       ;RK08-P: Load Word Count Register", // op 6753
	"SER        ;RX8E: Skip on Error Flag, Clear Flag", // op 6754
	"DLCA       ;RK08-P: Load Current Address Register", // op 6755
	"INTR       ;RX8E: Enable or Disable Disk Interrupts", // op 6756
	"DRCA       ;RK08-P: Read Current Address Register", // op 6757
	"           ;", // op 6760
	"DTRA       ;TC08-P: Read Status Register A", // op 6761
	"DTCA       ;TC08-P: Clear Status Register A", // op 6762
	"SDSQ1      ;TD8-E: Skip if Quad Line Flag Set, Unti 1", // op 6763
	"DTXA       ;TC08-P: Load Status Register A", // op 6764
	"SDLD1      ;TD8-E: Load Data Register from AC, Unit 1", // op 6765
	"DTLA       ;TC08-P: Clear and Load Status Register A", // op 6766
	"SDRD1      ;TD8-E: Load Data Register into AC, Unit 1", // op 6767
	"           ;", // op 6770
	"DTSF       ;TC08-P: Skip on Flag", // op 6771
	"DTRB       ;TC08-P: Read Status Register B", // op 6772
	"SDSQ0      ;TD8-E: Skip if Quad Line Flag Set, Unti 0", // op 6773
	"DTXB       ;TC08-P: Load Status Register B", // op 6774
	"SDLD0      ;TD8-E: Load Data Register from AC, Unit 0", // op 6775
	"SDRC0      ;TD8-E: Load Command, Mark Track, Status Regs into AC, Unit 0", // op 6776
	"SDRD0      ;TD8-E: Load Data Register into AC, Unit 0", // op 6777
	"No operation;", // op 7000
	"IAC;Increment AC", // op 7001
	"BSW        ;Byte Swap AC", // op 7002
	"           ;", // op 7003
	"RAL        ;Rotate AC & L left", // op 7004
	"           ;", // op 7005
	"RTL        ;Rotate AC & L left twice", // op 7006
	"           ;", // op 7007
	"RAR        ;Rotate AC & L right", // op 7010
	"           ;", // op 7011
	"RTR        ;Rotate AC & L right twice", // op 7012
	"           ;", // op 7013
	"           ;", // op 7014
	"           ;", // op 7015
	"           ;", // op 7016
	"           ;", // op 7017
	"CML        ;Complement L", // op 7020
	"CML IAC    ;Complement L, Increment AC", // op 7021
	"CML BSW    ;Complement L, Byte Swap AC", // op 7022
	"CML        ;Complement L", // op 7023
	"CML RAL    ;Complement L, Rotate AC & L left", // op 7024
	"CML        ;Complement L", // op 7025
	"CML RTL    ;Complement L, Rotate AC & L left twice", // op 7026
	"CML        ;Complement L", // op 7027
	"CML RAR    ;Complement L, Rotate AC & L right", // op 7030
	"CML        ;Complement L", // op 7031
	"CML RTR    ;Complement L, Rotate AC & L right twice", // op 7032
	"CML        ;Complement L", // op 7033
	"CML        ;Complement L", // op 7034
	"CML        ;Complement L", // op 7035
	"CML        ;Complement L", // op 7036
	"CML        ;Complement L", // op 7037
	"CMA        ;Complement AC", // op 7040
	"CIA        ;2s Complement AC", // op 7041
	"           ;", // op 7042
	"           ;", // op 7043
	"           ;", // op 7044
	"           ;", // op 7045
	"           ;", // op 7046
	"           ;", // op 7047
	"           ;", // op 7050
	"           ;", // op 7051
	"           ;", // op 7052
	"           ;", // op 7053
	"           ;", // op 7054
	"           ;", // op 7055
	"           ;", // op 7056
	"           ;", // op 7057
	"CML CMA    ;Complement L, Complement AC", // op 7060
	"CML CIA    ;Complement L, 2s Complement AC", // op 7061
	"CML        ;Complement L", // op 7062
	"CML        ;Complement L", // op 7063
	"CML        ;Complement L", // op 7064
	"CML        ;Complement L", // op 7065
	"CML        ;Complement L", // op 7066
	"CML        ;Complement L", // op 7067
	"CML        ;Complement L", // op 7070
	"CML        ;Complement L", // op 7071
	"CML        ;Complement L", // op 7072
	"CML        ;Complement L", // op 7073
	"CML        ;Complement L", // op 7074
	"CML        ;Complement L", // op 7075
	"CML        ;Complement L", // op 7076
	"CML        ;Complement L", // op 7077
	"CLL        ;Clear L", // op 7100
	"CLL IAC    ;Clear L, Increment AC", // op 7101
	"CLL BSW    ;Clear L, Byte Swap AC", // op 7102
	"CLL        ;Clear L", // op 7103
	"CLL RAL    ;Clear L, Rotate AC & L left", // op 7104
	"CLL        ;Clear L", // op 7105
	"CLL RTL    ;Clear L, Rotate AC & L left twice", // op 7106
	"CLL        ;Clear L", // op 7107
	"CLL RAR    ;Clear L, Rotate AC & L right", // op 7110
	"CLL        ;Clear L", // op 7111
	"CLL RTR    ;Clear L, Rotate AC & L right twice", // op 7112
	"CLL        ;Clear L", // op 7113
	"CLL        ;Clear L", // op 7114
	"CLL        ;Clear L", // op 7115
	"CLL        ;Clear L", // op 7116
	"CLL        ;Clear L", // op 7117
	"STL        ;Set L", // op 7120
	"STL IAC    ;Set L, Increment AC", // op 7121
	"STL BSW    ;Set L, Byte Swap AC", // op 7122
	"STL        ;Set L", // op 7123
	"STL RAL    ;Set L, Rotate AC & L left", // op 7124
	"STL        ;Set L", // op 7125
	"STL RTL    ;Set L, Rotate AC & L left twice", // op 7126
	"STL        ;Set L", // op 7127
	"STL RAR    ;Set L, Rotate AC & L right", // op 7130
	"STL        ;Set L", // op 7131
	"STL RTR    ;Set L, Rotate AC & L right twice", // op 7132
	"STL        ;Set L", // op 7133
	"STL        ;Set L", // op 7134
	"STL        ;Set L", // op 7135
	"STL        ;Set L", // op 7136
	"STL        ;Set L", // op 7137
	"CLL CMA    ;Clear L, Complement AC", // op 7140
	"CLL CIA    ;Clear L, 2s Complement AC", // op 7141
	"CLL        ;Clear L", // op 7142
	"CLL        ;Clear L", // op 7143
	"CLL        ;Clear L", // op 7144
	"CLL        ;Clear L", // op 7145
	"CLL        ;Clear L", // op 7146
	"CLL        ;Clear L", // op 7147
	"CLL        ;Clear L", // op 7150
	"CLL        ;Clear L", // op 7151
	"CLL        ;Clear L", // op 7152
	"CLL        ;Clear L", // op 7153
	"CLL        ;Clear L", // op 7154
	"CLL        ;Clear L", // op 7155
	"CLL        ;Clear L", // op 7156
	"CLL        ;Clear L", // op 7157
	"STL CMA    ;Set L, Complement AC", // op 7160
	"STL CIA    ;Set L, 2s Complement AC", // op 7161
	"STL        ;Set L", // op 7162
	"STL        ;Set L", // op 7163
	"STL        ;Set L", // op 7164
	"STL        ;Set L", // op 7165
	"STL        ;Set L", // op 7166
	"STL        ;Set L", // op 7167
	"STL        ;Set L", // op 7170
	"STL        ;Set L", // op 7171
	"STL        ;Set L", // op 7172
	"STL        ;Set L", // op 7173
	"STL        ;Set L", // op 7174
	"STL        ;Set L", // op 7175
	"STL        ;Set L", // op 7176
	"STL        ;Set L", // op 7177
	"CLA        ;Clear AC", // op 7200
	"CLA IAC    ;Set AC to 0001", // op 7201
	"CLA BSW    ;Clear AC", // op 7202
	"CLA IAC BSW;Set AC to 0100", // op 7203
	"GLK        ;Move L into AC", // op 7204
	"CLA IAC RAL;Set AC to 0001, Rotate AC & L left", // op 7205
	"CLA RTL    ;Move L into AC bit 10", // op 7206
	"CLA IAC RTL;Set AC to 0001, Rotate AC & L left twice", // op 7207
	"CLA RAR    ;Move L into AC bit 0", // op 7210
	"CLA IAC RAR;Set AC to 0001, Rotate AC & L right", // op 7211
	"CLA RTR    ;Move L into AC bit 1", // op 7212
	"CLA IAC RTR;Set AC to 0001, Rotate AC & L right twice", // op 7213
	"           ;", // op 7214
	"           ;", // op 7215
	"           ;", // op 7216
	"           ;", // op 7217
	"CML CLA    ;Complement L, Clear AC", // op 7220
	"CML CLA IAC;Complement L, Set AC to 0001", // op 7221
	"CML CLA BSW;Complement L, Clear AC", // op 7222
	"CML CLA IAC BSW;Complement L, set AC to 0100", // op 7223
	"CML GLK    ;Complement L, Move L into AC", // op 7224
	"CML CLA IAC RAL;Complement L, Set AC to 0001, Rotate AC & L left", // op 7225
	"CML CLA RTL;Complement L, Move L into AC bit 10", // op 7226
	"CML CLA IAC RTL;Complement L, Set AC to 0001, Rotate AC & L left twice", // op 7227
	"CML CLA RAR;Complement L, Move L into AC bit 0", // op 7230
	"CML CLA IAC RAR;Complement L, Set AC to 0001, Rotate AC & L right", // op 7231
	"CML CLA RTR;Complement L, Move L into AC bit 1", // op 7232
	"CML CLA IAC RTR;Complement L, Set AC to 0001, Rotate AC & L right twice", // op 7233
	"CML        ;Complement L", // op 7234
	"CML        ;Complement L", // op 7235
	"CML        ;Complement L", // op 7236
	"CML        ;Complement L", // op 7237
	"STA        ;Set AC to 7777", // op 7240
	"CLA CMA IAC;Set AC to 0, set L", // op 7241
	"CLA CMA BSW;Set AC to 7777, Byte swap AC", // op 7242
	"CLA CMA IAC BSW;Clear AC", // op 7243
	"CLA CMA RAL;Set AC to 7777, rotate AC & L left", // op 7244
	"CLA CMA IAC RAL;Move L into AC", // op 7245
	"CLA CMA RTL;Set AC to 7777, rotate AC & L left twice", // op 7246
	"CLA CMA IAC RTL;Move L into AC bit 10", // op 7247
	"CLA CMA RAR;Set AC to 7777, rotate AC & L right", // op 7250
	"CLA CMA IAC RAR;Move L into AC bit 0", // op 7251
	"CLA CMA RTR;Set AC to 7777, rotate AC & L right twice", // op 7252
	"CLA CMA IAC RTR;Move L into AC bit 1", // op 7253
	"           ;", // op 7254
	"           ;", // op 7255
	"           ;", // op 7256
	"           ;", // op 7257
	"CML STA    ;Complement L, Set AC to 7777", // op 7260
	"CML CLA CMA IAC;Complement L, Set AC to 0, set L", // op 7261
	"CML CLA CMA BSW;Complement L, Set AC to 7777, Byte swap AC", // op 7262
	"CML CLA CMA IAC BSW;Complement L, Clear AC", // op 7263
	"CML CLA CMA RAL;Complement L, Set AC to 7777, rotate AC & L left", // op 7264
	"CML CLA CMA IAC RAL;Complement L, Move L into AC", // op 7265
	"CML CLA CMA RTL;Complement L, Set AC to 7777, rotate AC & L left twice", // op 7266
	"CML CLA CMA IAC RTL;Complement L, Move L into AC bit 10", // op 7267
	"CML CLA CMA RAR;Complement L, Set AC to 7777, rotate AC & L right", // op 7270
	"CML CLA CMA IAC RAR;Complement L, Move L into AC bit 0", // op 7271
	"CML CLA CMA RTR;Complement L, Set AC to 7777, rotate AC & L right twice", // op 7272
	"CML CLA CMA IAC RTR;Complement L, Move L into AC bit 1", // op 7273
	"CML        ;Complement L", // op 7274
	"CML        ;Complement L", // op 7275
	"CML        ;Complement L", // op 7276
	"CML        ;Complement L", // op 7277
	"CLL CLA    ;Clear L, Clear AC", // op 7300
	"CLL CLA IAC;Clear L, Set AC to 0001", // op 7301
	"CLL CLA BSW;Clear L, Clear AC", // op 7302
	"CLL CLA IAC BSW;Clear L, set AC to 0100", // op 7303
	"CLL GLK    ;Clear L, Move L into AC", // op 7304
	"CLL CLA IAC RAL;Clear L, Set AC to 0001, Rotate AC & L left", // op 7305
	"CLL CLA RTL;Clear L, Move L into AC bit 10", // op 7306
	"CLL CLA IAC RTL;Clear L, Set AC to 0001, Rotate AC & L left twice", // op 7307
	"CLL CLA RAR;Clear L, Move L into AC bit 0", // op 7310
	"CLL CLA IAC RAR;Clear L, Set AC to 0001, Rotate AC & L right", // op 7311
	"CLL CLA RTR;Clear L, Move L into AC bit 1", // op 7312
	"CLL CLA IAC RTR;Clear L, Set AC to 0001, Rotate AC & L right twice", // op 7313
	"CLL        ;Clear L", // op 7314
	"CLL        ;Clear L", // op 7315
	"CLL        ;Clear L", // op 7316
	"CLL        ;Clear L", // op 7317
	"STL CLA    ;Set L, Clear AC", // op 7320
	"STL CLA IAC;Set L, Set AC to 0001", // op 7321
	"STL CLA BSW;Set L, Clear AC", // op 7322
	"STL CLA IAC BSW;Set L, set AC to 0100", // op 7323
	"STL GLK    ;Set L, Move L into AC", // op 7324
	"STL CLA IAC RAL;Set L, Set AC to 0001, Rotate AC & L left", // op 7325
	"STL CLA RTL;Set L, Move L into AC bit 10", // op 7326
	"STL CLA IAC RTL;Set L, Set AC to 0001, Rotate AC & L left twice", // op 7327
	"STL CLA RAR;Set L, Move L into AC bit 0", // op 7330
	"STL CLA IAC RAR;Set L, Set AC to 0001, Rotate AC & L right", // op 7331
	"STL CLA RTR;Set L, Move L into AC bit 1", // op 7332
	"STL CLA IAC RTR;Set L, Set AC to 0001, Rotate AC & L right twice", // op 7333
	"STL        ;Set L", // op 7334
	"STL        ;Set L", // op 7335
	"STL        ;Set L", // op 7336
	"STL        ;Set L", // op 7337
	"CLL STA    ;Clear L, Set AC to 7777", // op 7340
	"CLL CLA CMA IAC;Clear L, Set AC to 0, set L", // op 7341
	"CLL CLA CMA BSW;Clear L, Set AC to 7777, Byte swap AC", // op 7342
	"CLL CLA CMA IAC BSW;Clear L, Clear AC", // op 7343
	"CLL CLA CMA RAL;Clear L, Set AC to 7777, rotate AC & L left", // op 7344
	"CLL CLA CMA IAC RAL;Clear L, Move L into AC", // op 7345
	"CLL CLA CMA RTL;Clear L, Set AC to 7777, rotate AC & L left twice", // op 7346
	"CLL CLA CMA IAC RTL;Clear L, Move L into AC bit 10", // op 7347
	"CLL CLA CMA RAR;Clear L, Set AC to 7777, rotate AC & L right", // op 7350
	"CLL CLA CMA IAC RAR;Clear L, Move L into AC bit 0", // op 7351
	"CLL CLA CMA RTR;Clear L, Set AC to 7777, rotate AC & L right twice", // op 7352
	"CLL CLA CMA IAC RTR;Clear L, Move L into AC bit 1", // op 7353
	"CLL        ;Clear L", // op 7354
	"CLL        ;Clear L", // op 7355
	"CLL        ;Clear L", // op 7356
	"CLL        ;Clear L", // op 7357
	"STL STA    ;Set L, Set AC to 7777", // op 7360
	"STL CLA CMA IAC;Set L, Set AC to 0, set L", // op 7361
	"STL CLA CMA BSW;Set L, Set AC to 7777, Byte swap AC", // op 7362
	"STL CLA CMA IAC BSW;Set L, Clear AC", // op 7363
	"STL CLA CMA RAL;Set L, Set AC to 7777, rotate AC & L left", // op 7364
	"STL CLA CMA IAC RAL;Set L, Move L into AC", // op 7365
	"STL CLA CMA RTL;Set L, Set AC to 7777, rotate AC & L left twice", // op 7366
	"STL CLA CMA IAC RTL;Set L, Move L into AC bit 10", // op 7367
	"STL CLA CMA RAR;Set L, Set AC to 7777, rotate AC & L right", // op 7370
	"STL CLA CMA IAC RAR;Set L, Move L into AC bit 0", // op 7371
	"STL CLA CMA RTR;Set L, Set AC to 7777, rotate AC & L right twice", // op 7372
	"STL CLA CMA IAC RTR;Set L, Move L into AC bit 1", // op 7373
	"STL        ;Set L", // op 7374
	"STL        ;Set L", // op 7375
	"STL        ;Set L", // op 7376
	"STL        ;Set L", // op 7377
	"NOP        ;No Operation", // op 7400
	"NOP        ;Group 3 NOP", // op 7401
	"HLT        ;Halt", // op 7402
	"SCL        ;Step counter load;depends on mode)", // op 7403
	"OSR        ;OR Switch Register with AC", // op 7404
	"MUY        ;Multiply", // op 7405
	"OSR HLT    ;OR Switch Register with AC, Halt", // op 7406
	"DVI        ;Divide", // op 7407
	"SKP        ;Skip unconditionally", // op 7410
	"NMI        ;Normalize", // op 7411
	"SKP HLTNMI ;Skip unconditionally, HaltNormalize", // op 7412
	"SHL        ;Shift left", // op 7413
	"SKP OSR    ;Skip unconditionally, OR Switch Register with AC", // op 7414
	"ASR        ;Arithmetic shift right", // op 7415
	"SKP OSR HLT;Skip unconditionally, OR Switch Register with AC, Halt", // op 7416
	"LSR        ;Logical shift right", // op 7417
	"SNL        ;Skip on L <> 0", // op 7420
	"MQL        ;Load MQ from AC then clear AC", // op 7421
	"SNL HLT    ;Skip on L <> 0, Halt", // op 7422
	"MQL SCL    ;Load MQ from AC then clear AC, Step counter load", // op 7423
	"SNL OSR    ;Skip on L <> 0, OR Switch Register with AC", // op 7424
	"MQL MUY    ;Load MQ from AC then clear AC, Multiply", // op 7425
	"SNL OSR HLT;Skip on L <> 0, OR Switch Register with AC, Halt", // op 7426
	"MQL DVI    ;Load MQ from AC then clear AC, Divide", // op 7427
	"SZL        ;Skip on L = 0", // op 7430
	"SWAB       ;Switch mode from A to B", // op 7431
	"SZL HLT    ;Skip on L = 0, Halt", // op 7432
	"MQL SHL    ;Shift left", // op 7433
	"SZL OSR    ;Skip on L = 0, OR Switch Register with AC", // op 7434
	"MQL ASR    ;Arithmetic shift right", // op 7435
	"SZL OSR HLT;Skip on L = 0, OR Switch Register with AC, Halt", // op 7436
	"MQL LSR    ;Logical shift right", // op 7437
	"SZA        ;Skip on AC = 0", // op 7440
	"SCA        ;OR step counter with AC", // op 7441
	"SZA HLT    ;Skip on AC = 0, Halt", // op 7442
	"DAD        ;Double precision add;Mode B)", // op 7443
	"SZA OSR    ;Skip on AC = 0, OR Switch Register with AC", // op 7444
	"DST        ;Double precision store;Mode B)", // op 7445
	"SZA OSR HLT;Skip on AC = 0, OR Switch Register with AC, Halt", // op 7446
	"SWBA       ;Switch mode from B to A", // op 7447
	"SNA        ;Skip on AC <> 0", // op 7450
	"DPSZ       ;Double precision skip if zero;Mode B)", // op 7451
	"SNA HLT    ;Skip on AC <> 0, Halt", // op 7452
	"UNKNOWN!   ;**Unknown**", // op 7453
	"SNA OSR    ;Skip on AC <> 0, OR Switch Register with AC", // op 7454
	"UNKNOWN!   ;**Unknown**", // op 7455
	"SNA OSR HLT;Skip on AC <> 0, OR Switch Register with AC, Halt", // op 7456
	"SAM        ;Subtract AC from MQ", // op 7457
	"SZA SNL    ;Skip on AC = 0 or L <> 0", // op 7460
	"UNKNOWN!   ;**Unknown**", // op 7461
	"SZA SNL HLT;Skip on AC = 0 or L <> 0, Halt", // op 7462
	"UNKNOWN!   ;**Unknown**", // op 7463
	"SZA SNL OSR;Skip on AC = 0 or L <> 0, OR Switch Register with AC", // op 7464
	"UNKNOWN!   ;**Unknown**", // op 7465
	"SZA SNL OSR HLT;Skip on AC = 0 or L <> 0, OR Switch Register with AC, Halt", // op 7466
	"UNKNOWN!   ;**Unknown**", // op 7467
	"SNA SZL    ;Skip on AC <> 0 and L = 0", // op 7470
	"UNKNOWN!   ;ILLEGAL!", // op 7471
	"SNA SZL HLT;Skip on AC <> 0 and L = 0, Halt", // op 7472
	"UNKNOWN!   ;**Unknown**", // op 7473
	"SNA SZL OSR;Skip on AC <> 0 and L = 0, OR Switch Register with AC", // op 7474
	"UNKNOWN!   ;**Unknown**", // op 7475
	"SNA SZL OSR HLT;Skip on AC <> 0 and L = 0, OR Switch Register with AC, Halt", // op 7476
	"UNKNOWN!   ;**Unknown**", // op 7477
	"SMA        ;Skip on AC < 0", // op 7500
	"MQA        ;OR MQ with AC", // op 7501
	"SMA HLT    ;Skip on AC < 0, Halt", // op 7502
	"MQA SCL    ;OR MQ with AC, Step counter load", // op 7503
	"SMA OSR    ;Skip on AC < 0, OR Switch Register with AC", // op 7504
	"MQA MUY    ;OR MQ with AC, Multiply", // op 7505
	"SMA OSR HLT;Skip on AC < 0, OR Switch Register with AC, Halt", // op 7506
	"MQA DVI    ;OR MQ with AC, Divide", // op 7507
	"SPA        ;Skip on AC >= 0", // op 7510
	"UNKNOWN!   ;ILLEGAL!", // op 7511
	"SPA HLT    ;Skip on AC >= 0, Halt", // op 7512
	"MQA SHL    ;OR MQ with AC, Shift left", // op 7513
	"SPA OSR    ;Skip on AC >= 0, OR Switch Register with AC", // op 7514
	"MQA ASR    ;OR MQ with AC, Arithmetic shift right", // op 7515
	"SPA OSR HLT;Skip on AC >= 0, OR Switch Register with AC, Halt", // op 7516
	"MQA LSR    ;OR MQ with AC, Logical shift right", // op 7517
	"SMA SNL    ;Skip on AC < 0 or L <> 0", // op 7520
	"SWP        ;Swap MQ and AC", // op 7521
	"SMA SNL HLT;Skip on AC < 0 or L <> 0, Halt", // op 7522
	"SWP SCL    ;Swap AC and MQ, Step counter load", // op 7523
	"SMA SNL OSR;Skip on AC < 0 or L <> 0, OR Switch Register with AC", // op 7524
	"SWP MUY    ;Swap AC and MQ, Multiply", // op 7525
	"SMA SNL OSR HLT;Skip on AC < 0 or L <> 0, OR Switch Register with AC, Halt", // op 7526
	"SWP DVI    ;Swap AC and MQ, Divide", // op 7527
	"SPA SZL    ;Skip on AC >= 0 and L = 0", // op 7530
	"UNKNOWN!   ;ILLEGAL!", // op 7531
	"SPA SZL HLT;Skip on AC >= 0 and L = 0, Halt", // op 7532
	"SWP SHL    ;Swap AC and MQ, Shift left", // op 7533
	"SPA SZL OSR;Skip on AC >= 0 and L = 0, OR Switch Register with AC", // op 7534
	"SWP ASR    ;Swap AC and MQ, Arithmetic shift right", // op 7535
	"SPA SZL OSR HLT;Skip on AC >= 0 and L = 0, OR Switch Register with AC, Halt", // op 7536
	"SWP LSR    ;Swap AC and MQ, Logical shift right", // op 7537
	"SMA SZA    ;Skip on AC <= 0", // op 7540
	"UNKNOWN!   ;**Unknown**", // op 7541
	"SMA SZA HLT;Skip on AC <= 0, Halt", // op 7542
	"UNKNOWN!   ;**Unknown**", // op 7543
	"SMA SZA OSR;Skip on AC <= 0, OR Switch Register with AC", // op 7544
	"UNKNOWN!   ;**Unknown**", // op 7545
	"SMA SZA OSR HLT;Skip on AC <= 0, OR Switch Register with AC, Halt", // op 7546
	"UNKNOWN!   ;**Unknown**", // op 7547
	"SPA SNA    ;Skip on AC > 0", // op 7550
	"UNKNOWN!   ;ILLEGAL!", // op 7551
	"SPA SNA HLT;Skip on AC > 0, Halt", // op 7552
	"UNKNOWN!   ;**Unknown**", // op 7553
	"SPA SNA OSR;Skip on AC > 0, OR Switch Register with AC", // op 7554
	"UNKNOWN!   ;**Unknown**", // op 7555
	"SPA SNA OSR HLT;Skip on AC > 0, OR Switch Register with AC, Halt", // op 7556
	"UNKNOWN!   ;**Unknown**", // op 7557
	"SMA SZA SNL;Skip on AC <= 0 or L <> 0", // op 7560
	"UNKNOWN!   ;**Unknown**", // op 7561
	"SMA SZA SNL HLT;Skip on AC <= 0 or L <> 0, Halt", // op 7562
	"UNKNOWN!   ;**Unknown**", // op 7563
	"SMA SZA SNL OSR;Skip on AC <= 0 or L <> 0, OR Switch Register with AC", // op 7564
	"UNKNOWN!   ;**Unknown**", // op 7565
	"SMA SZA SNL OSR HLT;Skip on AC <= 0 or L <> 0, OR Switch Register with AC, Halt", // op 7566
	"UNKNOWN!   ;**Unknown**", // op 7567
	"SPA SNA SZL;Skip on AC > 0 and L=0", // op 7570
	"UNKNOWN!   ;ILLEGAL!", // op 7571
	"SPA SNA SZL HLT;Skip on AC > 0 and L=0, Halt", // op 7572
	"DPIC       ;Double precision increment;Mode B)", // op 7573
	"SPA SNA SZL OSR;Skip on AC > 0 and L=0, OR Switch Register with AC", // op 7574
	"DCM        ;Double precision complement;Mode B)", // op 7575
	"SPA SNA SZL OSR HLT;Skip on AC > 0 and L=0, OR Switch Register with AC, Halt", // op 7576
	"UNKNOWN!   ;**Unknown**", // op 7577
	"CLA        ;Clear AC", // op 7600
	"CLA        ;Clear AC", // op 7601
	"CLA HLT    ;Clear AC, Halt", // op 7602
	"CLA SCL    ;Clear AC, Step counter load", // op 7603
	"LAS        ;Load AC from the Switch Register", // op 7604
	"CLA MUY    ;Clear AC, Multiply", // op 7605
	"LAS HLT    ;Load AC from the Switch Register, Halt", // op 7606
	"CLA DVI    ;Clear AC, Divide", // op 7607
	"SKP CLA    ;Skip unconditionally, Clear AC", // op 7610
	"UNKNOWN!   ;ILLEGAL!", // op 7611
	"SKP CLA HLT;Skip unconditionally, Clear AC, Halt", // op 7612
	"CLA SHL    ;Clear AC, Shift left", // op 7613
	"SKP LAS    ;Skip unconditionally, Load AC from the Switch Register", // op 7614
	"CLA ASR    ;Clear AC, Arithmetic shift right", // op 7615
	"SKP LAS HLT;Skip unconditionally, Load AC from the Switch Register, Halt", // op 7616
	"CLA LSR    ;Clear AC, Logical shift right", // op 7617
	"SNL CLA    ;Skip on L <> 0, Clear AC", // op 7620
	"CAM        ;Clear AC and MQ", // op 7621
	"SNL CLA HLT;Skip on L <> 0, Clear AC, Halt", // op 7622
	"CAM SCL   ;Clear AC and MQ, Step counter load", // op 7623
	"SNL LAS    ;Skip on L <> 0, Load AC from the Switch Register", // op 7624
	"CAM MUY   ;Clear AC and MQ, Multiply", // op 7625
	"SNL LAS HLT;Skip on L <> 0, Load AC from the Switch Register, Halt", // op 7626
	"CAM DVI   ;Clear AC and MQ, Divide", // op 7627
	"SZL CLA    ;Skip on L = 0, Clear AC", // op 7630
	"UNKNOWN!   ;ILLEGAL!", // op 7631
	"SZL CLA HLT;Skip on L = 0, Clear AC, Halt", // op 7632
	"CAM SHL   ;Clear AC and MQ, Shift left", // op 7633
	"SZL LAS    ;Skip on L = 0, Load AC from the Switch Register", // op 7634
	"CAM ASR   ;Clear AC and MQ, Arithmetic shift right", // op 7635
	"SZL LAS HLT;Skip on L = 0, Load AC from the Switch Register, Halt", // op 7636
	"CAM LSR    ;Clear AC and MQ, Logical shift right", // op 7637
	"SZA CLA    ;Skip on AC = 0, Clear AC", // op 7640
	"CLA SCA    ;Step counter to AC", // op 7641
	"SZA CLA HLT;Skip on AC = 0, Clear AC, Halt", // op 7642
	"CLA DAD    ;Clear AC, Double precision add    ;Mode B)", // op 7643
	"SZA LAS    ;Skip on AC = 0, Load AC from the Switch Register", // op 7644
	"CLA DST    ;Clear AC, Double precision store;Mode B)", // op 7645
	"SZA LAS HLT;Skip on AC = 0, Load AC from the Switch Register, Halt", // op 7646
	"CLA SWBA   ;Clear AC, Switch mode from B to A", // op 7647
	"SNA CLA    ;Skip on AC <> 0, Clear AC", // op 7650
	"UNKNOWN!   ILLEGAL!", // op 7651
	"SNA CLA HLT;Skip on AC <> 0, Clear AC, Halt", // op 7652
	"UNKNOWN!   ;**Unknown**", // op 7653
	"SNA LAS    ;Skip on AC <> 0, Load AC from the Switch Register", // op 7654
	"UNKNOWN!   ;**Unknown**", // op 7655
	"SNA LAS HLT;Skip on AC <> 0, Load AC from the Switch Register, Halt", // op 7656
	"CLA SAM    ;Clear AC, Subtract AC from MQ", // op 7657
	"SZA SNL CLA;Skip on AC = 0 or L <> 0, Clear AC", // op 7660
	"UNKNOWN!   ;**Unknown**", // op 7661
	"SZA SNL CLA HLT;Skip on AC = 0 or L <> 0, Clear AC, Halt", // op 7662
	"UNKNOWN!   ;**Unknown**", // op 7663
	"SZA SNL LAS;Skip on AC = 0 or L <> 0, Load AC from the Switch Register", // op 7664
	"UNKNOWN!   ;**Unknown**", // op 7665
	"SZA SNL LAS HLT;Skip on AC = 0 or L <> 0, Load AC from the Switch Register, Halt", // op 7666
	"UNKNOWN!   ;**Unknown**", // op 7667
	"SNA SZL CLA;Skip on AC <> 0 and L = 0, Clear AC", // op 7670
	"UNKNOWN!   ;ILLEGAL!", // op 7671
	"SNA SZL CLA HLT;Skip on AC <> 0 and L = 0, Clear AC, Halt", // op 7672
	"UNKNOWN!   ;**Unknown**", // op 7673
	"SNA SZL LAS;Skip on AC <> 0 and L = 0, Load AC from the Switch Register", // op 7674
	"UNKNOWN!   ;**Unknown**", // op 7675
	"SNA SZL LAS HLT;Skip on AC <> 0 and L = 0, Load AC from the Switch Register, Halt", // op 7676
	"UNKNOWN!   ;**Unknown**", // op 7677
	"SMA CLA    ;Skip on AC < 0, Clear AC", // op 7700
	"ACL        ;Load MQ into AC", // op 7701
	"SMA CLA HLT;Skip on AC < 0, Clear AC, Halt", // op 7702
	" ACL SCL   ;Load MQ into AC, Step counter load", // op 7703
	"SMA LAS    ;Skip on AC < 0, Load AC from the Switch Register", // op 7704
	" ACL MUY   ;Load MQ into AC, Multiply", // op 7705
	"SMA LAS HLT;Skip on AC < 0, Load AC from the Switch Register, Halt", // op 7706
	"ACL DVI    ;Load MQ into AC, Divide", // op 7707
	"SPA CLA    ;Skip on AC >= 0, Clear AC", // op 7710
	"UNKNOWN!   ;ILLEGAL!", // op 7711
	"SPA CLA HLT;Skip on AC >= 0, Clear AC, Halt", // op 7712
	" ACL SHL   ;Load MQ into AC, Shift left", // op 7713
	"SPA LAS    ;Skip on AC >= 0, Load AC from the Switch Register", // op 7714
	" ACL ASR   ;Load MQ into AC, Arithmetic shift right", // op 7715
	"SPA LAS HLT;Skip on AC >= 0, Load AC from the Switch Register, Halt", // op 7716
	" ACL LSR   ;Load MQ into AC, Logical shift right", // op 7717
	"SMA SNL CLA;Skip on AC < 0 or L <> 0, Clear AC", // op 7720
	"CLA SWP    ;Load AC from MQ then clear MQ", // op 7721
	"SMA SNL CLA HLT;Skip on AC < 0 or L <> 0, Clear AC, Halt", // op 7722
	"CLA SWP SCL;Load AC from MQ then clear MQ, Step counter load", // op 7723
	"SMA SNL LAS;Skip on AC < 0 or L <> 0, Load AC from the Switch Register", // op 7724
	"CLA SWP MUY;Load AC from MQ then clear MQ, Multiply", // op 7725
	"SMA SNL LAS HLT;Skip on AC < 0 or L <> 0, Load AC from the Switch Register, Halt", // op 7726
	"CLA SWP DVI;Load AC from MQ then clear MQ, Divide", // op 7727
	"SPA SZL CLA;Skip on AC >= 0 and L = 0, Clear AC", // op 7730
	"UNKNOWN!   ;ILLEGAL!", // op 7731
	"SPA SZL CLA HLT;Skip on AC >= 0 and L = 0, Clear AC, Halt", // op 7732
	"CLA SWP SHL;Load AC from MQ then clear MQ, Shift left", // op 7733
	"SPA SZL LAS;Skip on AC >= 0 and L = 0, Load AC from the Switch Register", // op 7734
	"CLA SWP ASR;Load AC from MQ then clear MQ, Arithmetic shift right", // op 7735
	"SPA SZL LAS HLT;Skip on AC >= 0 and L = 0, Load AC from the Switch Register, Halt", // op 7736
	"CLA SWP LSR;Load AC from MQ then clear MQ, Logical shift right", // op 7737
	"SMA SZA CLA;Skip on AC <= 0, Clear AC", // op 7740
	"UNKNOWN!   ;**Unknown**", // op 7741
	"SMA SZA CLA HLT;Skip on AC <= 0, Clear AC, Halt", // op 7742
	"UNKNOWN!   ;**Unknown**", // op 7743
	"SMA SZA LAS;Skip on AC <= 0, Load AC from the Switch Register", // op 7744
	"UNKNOWN!   ;**Unknown**", // op 7745
	"SMA SZA LAS HLT;Skip on AC <= 0, Load AC from the Switch Register, Halt", // op 7746
	"UNKNOWN!   ;**Unknown**", // op 7747
	"SPA SNA CLA;Skip on AC > 0, Clear AC", // op 7750
	"UNKNOWN!   ;ILLEGAL!", // op 7751
	"SPA SNA CLA HLT;Skip on AC > 0, Clear AC, Halt", // op 7752
	"UNKNOWN!   ;**Unknown**", // op 7753
	"SPA SNA LAS;Skip on AC > 0, Load AC from the Switch Register", // op 7754
	"UNKNOWN!   ;**Unknown**", // op 7755
	"SPA SNA LAS HLT;Skip on AC > 0, Load AC from the Switch Register, Halt", // op 7756
	"UNKNOWN!   ;**Unknown**", // op 7757
	"SMA SZA SNL CLA;Skip on AC <= 0 or L <> 0, Clear AC", // op 7760
	"UNKNOWN!   ;**Unknown**", // op 7761
	"SMA SZA SNL CLA HLT;Skip on AC <= 0 or L <> 0, Clear AC, Halt", // op 7762
	"DLD;Double precision load;Mode B)", // op 7763
	"SMA SZA SNL LAS;Skip on AC <= 0 or L <> 0, Load AC from the Switch Register", // op 7764
	"UNKNOWN!   ;**Unknown**", // op 7765
	"SMA SZA SNL LAS HLT;Skip on AC <= 0 or L <> 0, Load AC from the Switch Register, Halt", // op 7766
	"UNKNOWN!   ;**Unknown**", // op 7767
	"SPA SNA SZL CLA;Skip on AC > 0 and L=0, Clear AC", // op 7770
	"UNKNOWN!   ;ILLEGAL!", // op 7771
	"SPA SNA SZL CLA HLT;Skip on AC > 0 and L=0, Clear AC, Halt", // op 7772
	"CLA DPIC   ;Clear AC, Double precision increment;Mode B)", // op 7773
	"SPA SNA SZL LAS;Skip on AC > 0 and L=0, Load AC from the Switch Register", // op 7774
	"CLA DCM    ;Clear AC, Double precision complement;Mode B)", // op 7775
	"SPA SNA SZL LAS HLT;Skip on AC > 0 and L=0, Load AC from the Switch Register, Halt", // op 7776
	"UNKNOWN!   ;**Unknown**", // op 7777
};
