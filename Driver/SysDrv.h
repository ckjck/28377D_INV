/*==========================================================*/
/* Title		:	SysDrv.h								*/
/* Description	: 	Class_SysDrv definition					*/
/* Date			:	Apr.2008								*/
/*==========================================================*/

#ifndef		SYSDRV_H 
             
#define     SYSDRV_H
//
// Defines
//

//
// The following are values that can be passed to the
// InitSysPll() & InitAuxPll() to select SYSPLL/AUXPLL integer multiplier
//
#define   IMULT_0         0
#define   IMULT_1         1
#define   IMULT_2         2
#define   IMULT_3         3
#define   IMULT_4         4
#define   IMULT_5         5
#define   IMULT_6         6
#define   IMULT_7         7
#define   IMULT_8         8
#define   IMULT_9         9
#define   IMULT_10        10
#define   IMULT_11        11
#define   IMULT_12        12
#define   IMULT_13        13
#define   IMULT_14        14
#define   IMULT_15        15
#define   IMULT_16        16
#define   IMULT_17        17
#define   IMULT_18        18
#define   IMULT_19        19
#define   IMULT_20        20
#define   IMULT_21        21
#define   IMULT_22        22
#define   IMULT_23        23
#define   IMULT_24        24
#define   IMULT_25        25
#define   IMULT_26        26
#define   IMULT_27        27
#define   IMULT_28        28
#define   IMULT_29        29
#define   IMULT_30        30
#define   IMULT_31        31
#define   IMULT_32        32
#define   IMULT_33        33
#define   IMULT_34        34
#define   IMULT_35        35
#define   IMULT_36        36
#define   IMULT_37        37
#define   IMULT_38        38
#define   IMULT_39        39
#define   IMULT_40        40
#define   IMULT_41        41
#define   IMULT_42        42
#define   IMULT_43        43
#define   IMULT_44        44
#define   IMULT_45        45
#define   IMULT_46        46
#define   IMULT_47        47
#define   IMULT_48        48
#define   IMULT_49        49
#define   IMULT_50        50
#define   IMULT_51        51
#define   IMULT_52        52
#define   IMULT_53        53
#define   IMULT_54        54
#define   IMULT_55        55
#define   IMULT_56        56
#define   IMULT_57        57
#define   IMULT_58        58
#define   IMULT_59        59
#define   IMULT_60        60
#define   IMULT_61        61
#define   IMULT_62        62
#define   IMULT_63        63
#define   IMULT_64        64
#define   IMULT_65        65
#define   IMULT_66        66
#define   IMULT_67        67
#define   IMULT_68        68
#define   IMULT_69        69
#define   IMULT_70        70
#define   IMULT_71        71
#define   IMULT_72        72
#define   IMULT_73        73
#define   IMULT_74        74
#define   IMULT_75        75
#define   IMULT_76        76
#define   IMULT_77        77
#define   IMULT_78        78
#define   IMULT_79        79
#define   IMULT_80        80
#define   IMULT_81        81
#define   IMULT_82        82
#define   IMULT_83        83
#define   IMULT_84        84
#define   IMULT_85        85
#define   IMULT_86        86
#define   IMULT_87        87
#define   IMULT_88        88
#define   IMULT_89        89
#define   IMULT_90        90
#define   IMULT_91        91
#define   IMULT_92        92
#define   IMULT_93        93
#define   IMULT_94        94
#define   IMULT_95        95
#define   IMULT_96        96
#define   IMULT_97        97
#define   IMULT_98        98
#define   IMULT_99        99
#define   IMULT_100       100
#define   IMULT_101       101
#define   IMULT_102       102
#define   IMULT_103       103
#define   IMULT_104       104
#define   IMULT_105       105
#define   IMULT_106       106
#define   IMULT_107       107
#define   IMULT_108       108
#define   IMULT_109       109
#define   IMULT_110       110
#define   IMULT_111       111
#define   IMULT_112       112
#define   IMULT_113       113
#define   IMULT_114       114
#define   IMULT_115       115
#define   IMULT_116       116
#define   IMULT_117       117
#define   IMULT_118       118
#define   IMULT_119       119
#define   IMULT_120       120
#define   IMULT_121       121
#define   IMULT_122       122
#define   IMULT_123       123
#define   IMULT_124       124
#define   IMULT_125       125
#define   IMULT_126       126
#define   IMULT_127       127

//
// The following are values that can be passed to the
// InitSysPll() & InitAuxPll() to select SYSPLL/AUXPLL fractional multiplier
//
#define   FMULT_0         0
#define   FMULT_0pt25     1
#define   FMULT_0pt5      2
#define   FMULT_0pt75     3

//
// The following are values that can be passed to the
// InitSysPll() to select divsel for SYSPLL
//
#define   PLLCLK_BY_1      0
#define   PLLCLK_BY_2      1
#define   PLLCLK_BY_4      2
#define   PLLCLK_BY_6      3
#define   PLLCLK_BY_8      4
#define   PLLCLK_BY_10     5
#define   PLLCLK_BY_12     6
#define   PLLCLK_BY_14     7
#define   PLLCLK_BY_16     8
#define   PLLCLK_BY_18     9
#define   PLLCLK_BY_20     10
#define   PLLCLK_BY_22     11
#define   PLLCLK_BY_24     12
#define   PLLCLK_BY_26     13
#define   PLLCLK_BY_28     14
#define   PLLCLK_BY_30     15
#define   PLLCLK_BY_32     16
#define   PLLCLK_BY_34     17
#define   PLLCLK_BY_36     18
#define   PLLCLK_BY_38     19
#define   PLLCLK_BY_40     20
#define   PLLCLK_BY_42     21
#define   PLLCLK_BY_44     22
#define   PLLCLK_BY_46     23
#define   PLLCLK_BY_48     24
#define   PLLCLK_BY_50     25
#define   PLLCLK_BY_52     26
#define   PLLCLK_BY_54     27
#define   PLLCLK_BY_56     28
#define   PLLCLK_BY_58     29
#define   PLLCLK_BY_60     30
#define   PLLCLK_BY_62     31
#define   PLLCLK_BY_64     32
#define   PLLCLK_BY_66     33
#define   PLLCLK_BY_68     34
#define   PLLCLK_BY_70     35
#define   PLLCLK_BY_72     36
#define   PLLCLK_BY_74     37
#define   PLLCLK_BY_76     38
#define   PLLCLK_BY_78     39
#define   PLLCLK_BY_80     40
#define   PLLCLK_BY_82     41
#define   PLLCLK_BY_84     42
#define   PLLCLK_BY_86     43
#define   PLLCLK_BY_88     44
#define   PLLCLK_BY_90     45
#define   PLLCLK_BY_92     46
#define   PLLCLK_BY_94     47
#define   PLLCLK_BY_96     48
#define   PLLCLK_BY_98     49
#define   PLLCLK_BY_100    50
#define   PLLCLK_BY_102    51
#define   PLLCLK_BY_104    52
#define   PLLCLK_BY_106    53
#define   PLLCLK_BY_108    54
#define   PLLCLK_BY_110    55
#define   PLLCLK_BY_112    56
#define   PLLCLK_BY_114    57
#define   PLLCLK_BY_116    58
#define   PLLCLK_BY_118    59
#define   PLLCLK_BY_120    60
#define   PLLCLK_BY_122    61
#define   PLLCLK_BY_124    62
#define   PLLCLK_BY_126    63

//
// The following are values that can be passed to the
// InitAuxPll() to select divsel for AUXPLL
//
#define   AUXPLLRAWCLK_BY_1    0
#define   AUXPLLRAWCLK_BY_2    1
#define   AUXPLLRAWCLK_BY_4    2
#define   AUXPLLRAWCLK_BY_8    3

//
// The following are values that can be passed to the
// IntOsc2Sel() & XtalOscSel() to select system PLL (or) AUX PLL
//
#define   SYSTEM_PLL    (Uint16) 0
#define   AUX_PLL       (Uint16) 1

//
// The following are values that can be passed to the
// InitSysPll() & InitAuxPll() to select clock source
//
#define   INT_OSC2     0
#define   XTAL_OSC     1
#define   INT_OSC1     2
#define   AUXCLKIN     4


class Class_SysDrv
	{
	
	public:
		Uint16 	Drv_InitPll(Uint16 clock_source, Uint16 imult, Uint16 fmult, Uint16 divsel);
		void 	Drv_InitSysCtrl(void);
		void 	Drv_InitPieCtrl(void);
		void 	Drv_InitPieVectTable(void);

	private:
		void	InitPeripheralClock(void);
		void	CPUSELConfig(void);
		void	GSxMSelConfig(void);
		
	};
	
#endif	
	
//===========================================================================
// End of file.
//===========================================================================

