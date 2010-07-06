/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Additional common string tables
*
*/



// INCLUDE FILES
#include    "httpfiltercommonstringsext.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
_STLIT8(K1, "multipart/form-data");
_STLIT8(K2, "form-data");
_STLIT8(K3, "name");
_STLIT8(K4, "boundary");
_STLIT8(K5, "utf-8");
_STLIT8(K6, "us-ascii");
_STLIT8(K7, "iso-10646-ucs-2");
_STLIT8(K8, "iso-8859-1");
_STLIT8(K9, "Proxy-Authenticate");
_STLIT8(K10, "Proxy-Authorization");
_STLIT8(K11, "Proxy-Username");
_STLIT8(K12, "Proxy-Password");
_STLIT8(K13, "Proxy-Realm");
_STLIT8(K14, "application/xhtml+xml");
_STLIT8(K15, "application/vnd.wap.xhtml+xml");
_STLIT8(K16, "text/css");
_STLIT8(K17, "ahd");	// Accept Headers Done
_STLIT8(K18, "big5");
_STLIT8(K19, "iso-8859-2");
_STLIT8(K20, "iso-8859-3");
_STLIT8(K21, "iso-8859-4");
_STLIT8(K22, "iso-8859-5");
_STLIT8(K23, "iso-8859-6");
_STLIT8(K24, "iso-8859-7");
_STLIT8(K25, "iso-8859-8");
_STLIT8(K26, "iso-8859-9");
_STLIT8(K27, "shift_JIS");
_STLIT8(K28, "utf-16");
_STLIT8(K29, "utf-16be");
_STLIT8(K30, "utf-16le");
_STLIT8(K31, "image/vnd.wap.wbmp");
_STLIT8(K32, "image/gif");
_STLIT8(K33, "application/wml+xml");
_STLIT8(K34, "application/vnd.wap.wbxml");
_STLIT8(K35, "application/vnd.wap.wmlc");
_STLIT8(K36, "application/vnd.wap.wmlscriptc");
_STLIT8(K37, "application/vnd.oma.drm.message");
_STLIT8(K38, "from-cache");
_STLIT8(K39, "Proxy-Filter");
_STLIT8(K40, "Connection-Status");
_STLIT8(K41, "NewConn-Flag");
_STLIT8(K42, "load-type");
_STLIT8(K43, "filter-cache-mode");
_STLIT8(K44, "Cache-Filter");
_STLIT8(K45, "windows-1255");
_STLIT8(K46, "windows-1256");
_STLIT8(K47, "gb2312");
_STLIT8(K48, "ConnHandle-Filter");
_STLIT8(K49, "IOP-Filter");
_STLIT8(K50, "windows-1250");
_STLIT8(K51, "windows-1251");
_STLIT8(K52, "windows-1253");
_STLIT8(K53, "windows-1254");
_STLIT8(K54, "windows-1257");
_STLIT8(K55, "utf-7");
_STLIT8(K56, "tis-620");
_STLIT8(K57, "jis_x0201-1997");
_STLIT8(K58, "jis_x0208-1997");
_STLIT8(K59, "iso-2022-jp");
_STLIT8(K60, "euc-jp");
_STLIT8(K61, "imei-notify");
_STLIT8(K62, "windows-874");
_STLIT8(K63, "koi8-r");
_STLIT8(K64, "koi8-u");
_STLIT8(K65, "AccessPointID");
_STLIT8(K66, "TransactionCallback");
_STLIT8(K67, "x-gzip");
_STLIT8(K68, "ContentInflated");
_STLIT8(K69, "ConnectionCallback");
_STLIT8(K70, "iso-8859-15");
_STLIT8(K71, "windows-1252");
_STLIT8(K72, "application/vnd.oma.drm.content");
_STLIT8(K73, "NTLM");
_STLIT8(K74, "Negotiate");
_STLIT8(K75, "SelfPtr");
_STLIT8(K76, "euc-kr");
_STLIT8(K77, "ks_c_5601-1987");
_STLIT8(K78, "SelfDownloadCallback");

const void * const KMoreStringPointers[] =
{
    (const void*)&K1,
    (const void*)&K2,
    (const void*)&K3,
    (const void*)&K4,
    (const void*)&K5,
    (const void*)&K6,
    (const void*)&K7,
    (const void*)&K8,
    (const void*)&K9,
    (const void*)&K10,
    (const void*)&K11,
    (const void*)&K12,
    (const void*)&K13,
    (const void*)&K14,
    (const void*)&K15,
    (const void*)&K16,
    (const void*)&K17,
    (const void*)&K18,
    (const void*)&K19,
    (const void*)&K20,
    (const void*)&K21,
    (const void*)&K22,
    (const void*)&K23,
    (const void*)&K24,
    (const void*)&K25,
    (const void*)&K26,
    (const void*)&K27,
    (const void*)&K28,
    (const void*)&K29,
    (const void*)&K30,
    (const void*)&K31,
    (const void*)&K32,
    (const void*)&K33,
    (const void*)&K34,
    (const void*)&K35,
    (const void*)&K36,
    (const void*)&K37,
    (const void*)&K38,
    (const void*)&K39,
    (const void*)&K40,
    (const void*)&K41,
    (const void*)&K42,
    (const void*)&K43,
    (const void*)&K44,
    (const void*)&K45,
    (const void*)&K46,
    (const void*)&K47,
    (const void*)&K48,
    (const void*)&K49,
    (const void*)&K50,
    (const void*)&K51,
    (const void*)&K52,
    (const void*)&K53,
    (const void*)&K54,
    (const void*)&K55,
    (const void*)&K56,
    (const void*)&K57,
    (const void*)&K58,
    (const void*)&K59,
    (const void*)&K60,
    (const void*)&K61,
    (const void*)&K62,
    (const void*)&K63,
    (const void*)&K64,
	(const void*)&K65,
	(const void*)&K66,
	(const void*)&K67,
	(const void*)&K68,
	(const void*)&K69,
    (const void*)&K70,
    (const void*)&K71,
    (const void*)&K72,
    (const void*)&K73,
    (const void*)&K74,
    (const void*)&K75,
    (const void*)&K76,
    (const void*)&K77,
    (const void*)&K78
};

const TStringTable HttpFilterCommonStringsExt::iTable =
    {
    sizeof(KMoreStringPointers) / sizeof(void*), KMoreStringPointers, EFalse
    };

/////////////////// Language String Table //////////////////

/*
Language list from ISO-639
*/

//AFAR            AA     HAMITIC
_STLIT8(L1, "aa");
//ABKHAZIAN       AB     IBERO-CAUCASIAN
_STLIT8(L2, "ab");
//AFRIKAANS       AF     GERMANIC
_STLIT8(L3, "af");
//AMHARIC         AM     SEMITIC
_STLIT8(L4, "am");
//ARABIC          AR     SEMITIC
_STLIT8(L5, "ar");
//ASSAMESE        AS     INDIAN
_STLIT8(L6, "as");
//AYMARA          AY     AMERINDIAN
_STLIT8(L7, "ay");
//AZERBAIJANI     AZ     TURKIC/ALTAIC
_STLIT8(L8, "az");
//BASHKIR         BA     TURKIC/ALTAIC
_STLIT8(L9, "ba");
//BYELORUSSIAN    BE     SLAVIC
_STLIT8(L10, "be");
//BULGARIAN       BG     SLAVIC
_STLIT8(L11, "bg");
//BIHARI          BH     INDIAN
_STLIT8(L12, "bh");
//BISLAMA         BI     [not given]
_STLIT8(L13, "bi");
//BENGALI;BANGLA  BN     INDIAN
_STLIT8(L14, "bn");
//TIBETAN         BO     ASIAN
_STLIT8(L15, "bo");
//BRETON          BR     CELTIC
_STLIT8(L16, "br");
//CATALAN         CA     ROMANCE
_STLIT8(L17, "ca");
//CORSICAN        CO     ROMANCE
_STLIT8(L18, "co");
//CZECH           CS     SLAVIC
_STLIT8(L19, "cs");
//WELSH           CY     CELTIC
_STLIT8(L20, "cy");
//DANISH          DA     GERMANIC
_STLIT8(L21, "da");
//GERMAN          DE     GERMANIC
_STLIT8(L22, "de");
//BHUTANI         DZ     ASIAN
_STLIT8(L23, "dz");
//GREEK           EL     LATIN/GREEK
_STLIT8(L24, "el");
//ENGLISH         EN     GERMANIC
_STLIT8(L25, "en");
//ESPERANTO       EO     INTERNATIONAL AUX.
_STLIT8(L26, "eo");
//SPANISH         ES     ROMANCE
_STLIT8(L27, "es");
//ESTONIAN        ET     FINNO-UGRIC
_STLIT8(L28, "et");
//BASQUE          EU     BASQUE
_STLIT8(L29, "eu");
//PERSIAN (farsi) FA     IRANIAN
_STLIT8(L30, "fa");
//FINNISH         FI     FINNO-UGRIC
_STLIT8(L31, "fi");
//FIJI            FJ     OCEANIC/INDONESIAN
_STLIT8(L32, "fj");
//FAROESE         FO     GERMANIC
_STLIT8(L33, "fo");
//FRENCH          FR     ROMANCE
_STLIT8(L34, "fr");
//FRISIAN         FY     GERMANIC
_STLIT8(L35, "fy");
//IRISH           GA     CELTIC
_STLIT8(L36, "ga");
//SCOTS GAELIC    GD     CELTIC
_STLIT8(L37, "gd");
//GALICIAN        GL     ROMANCE
_STLIT8(L38, "gl");
//GUARANI         GN     AMERINDIAN
_STLIT8(L39, "gn");
//GUJARATI        GU     INDIAN
_STLIT8(L40, "gu");
//HAUSA           HA     NEGRO-AFRICAN
_STLIT8(L41, "ha");
//HEBREW          HE     SEMITIC [*Changed 1989 from original ISO 639:1988, IW]
_STLIT8(L42, "he");
//HINDI           HI     INDIAN
_STLIT8(L43, "hi");
//CROATIAN        HR     SLAVIC
_STLIT8(L44, "hr");
//HUNGARIAN       HU     FINNO-UGRIC
_STLIT8(L45, "hu");
//ARMENIAN        HY     INDO-EUROPEAN (OTHER)
_STLIT8(L46, "hy");
//INTERLINGUA     IA     INTERNATIONAL AUX.
_STLIT8(L47, "ia");
//INTERLINGUE     IE     INTERNATIONAL AUX.
_STLIT8(L48, "ie");
//INUPIAK         IK     ESKIMO
_STLIT8(L49, "ik");
//INDONESIAN      ID     OCEANIC/INDONESIAN [*Changed 1989 from original ISO 639:1988, IN]
_STLIT8(L50, "id");
//ICELANDIC       IS     GERMANIC
_STLIT8(L51, "is");
//ITALIAN         IT     ROMANCE
_STLIT8(L52, "it");
//INUKTITUT       IU     [        ]
_STLIT8(L53, "iu");
//JAPANESE        JA     ASIAN
_STLIT8(L54, "ja");
//JAVANESE        JV     OCEANIC/INDONESIAN
_STLIT8(L55, "jv");
//GEORGIAN        KA     IBERO-CAUCASIAN
_STLIT8(L56, "ka");
//KAZAKH          KK     TURKIC/ALTAIC
_STLIT8(L57, "kk");
//GREENLANDIC     KL     ESKIMO
_STLIT8(L58, "kl");
//CAMBODIAN       KM     ASIAN
_STLIT8(L59, "km");
//KANNADA         KN     DRAVIDIAN
_STLIT8(L60, "kn");
//KOREAN          KO     ASIAN
_STLIT8(L61, "ko");
//KASHMIRI        KS     INDIAN
_STLIT8(L62, "ks");
//KURDISH         KU     IRANIAN
_STLIT8(L63, "ku");
//KIRGHIZ         KY     TURKIC/ALTAIC
_STLIT8(L64, "ky");
//LATIN           LA     LATIN/GREEK
_STLIT8(L65, "la");
//LINGALA         LN     NEGRO-AFRICAN
_STLIT8(L66, "ln");
//LAOTHIAN        LO     ASIAN
_STLIT8(L67, "lo");
//LITHUANIAN      LT     BALTIC
_STLIT8(L68, "lt");
//LATVIAN;LETTISH LV     BALTIC
_STLIT8(L69, "lv");
//MALAGASY        MG     OCEANIC/INDONESIAN
_STLIT8(L70, "mg");
//MAORI           MI     OCEANIC/INDONESIAN
_STLIT8(L71, "mi");
//MACEDONIAN      MK     SLAVIC
_STLIT8(L72, "mk");
//MALAYALAM       ML     DRAVIDIAN
_STLIT8(L73, "ml");
//MONGOLIAN       MN     [not given]
_STLIT8(L74, "mn");
//MOLDAVIAN       MO     ROMANCE
_STLIT8(L75, "mo");
//MARATHI         MR     INDIAN
_STLIT8(L76, "mr");
//MALAY           MS     OCEANIC/INDONESIAN
_STLIT8(L77, "ms");
//MALTESE         MT     SEMITIC
_STLIT8(L78, "mt");
//BURMESE         MY     ASIAN
_STLIT8(L79, "my");
//NAURU           NA     [not given]
_STLIT8(L80, "na");
//NEPALI          NE     INDIAN
_STLIT8(L81, "ne");
//DUTCH           NL     GERMANIC
_STLIT8(L82, "nl");
//NORWEGIAN       NO     GERMANIC
_STLIT8(L83, "no");
//OCCITAN         OC     ROMANCE
_STLIT8(L84, "oc");
//AFAN (OROMO)    OM     HAMITIC
_STLIT8(L85, "om");
//ORIYA           OR     INDIAN
_STLIT8(L86, "or");
//PUNJABI         PA     INDIAN
_STLIT8(L87, "pa");
//POLISH          PL     SLAVIC
_STLIT8(L88, "pl");
//PASHTO;PUSHTO   PS     IRANIAN
_STLIT8(L89, "ps");
//PORTUGUESE      PT     ROMANCE
_STLIT8(L90, "pt");
//QUECHUA         QU     AMERINDIAN
_STLIT8(L91, "qu");
//RHAETO-ROMANCE  RM     ROMANCE
// Hack: 1st letter set to uppercase for email login @ www.rediff.com, which
// sets cookie with name "Rm" and its server incorrectly treats cookie names
// as case sensitive.
_STLIT8(L92, "Rm");
//KURUNDI         RN     NEGRO-AFRICAN
_STLIT8(L93, "rn");
//ROMANIAN        RO     ROMANCE
_STLIT8(L94, "ro");
//RUSSIAN         RU     SLAVIC
_STLIT8(L95, "ru");
//KINYARWANDA     RW     NEGRO-AFRICAN
_STLIT8(L96, "rw");
//SANSKRIT        SA     INDIAN
_STLIT8(L97, "sa");
//SINDHI          SD     INDIAN
_STLIT8(L98, "sd");
//SANGHO          SG     NEGRO-AFRICAN
_STLIT8(L99, "sg");
//SERBO-CROATIAN  SH     SLAVIC
_STLIT8(L100, "sh");
//SINGHALESE      SI     INDIAN
_STLIT8(L101, "si");
//SLOVAK          SK     SLAVIC
_STLIT8(L102, "sk");
//SLOVENIAN       SL     SLAVIC
_STLIT8(L103, "sl");
//SAMOAN          SM     OCEANIC/INDONESIAN
_STLIT8(L104, "sm");
//SHONA           SN     NEGRO-AFRICAN
_STLIT8(L105, "sn");
//SOMALI          SO     HAMITIC
_STLIT8(L106, "so");
//ALBANIAN        SQ     INDO-EUROPEAN (OTHER)
_STLIT8(L107, "sq");
//SERBIAN         SR     SLAVIC
_STLIT8(L108, "sr");
//SISWATI         SS     NEGRO-AFRICAN
_STLIT8(L109, "ss");
//SESOTHO         ST     NEGRO-AFRICAN
_STLIT8(L110, "st");
//SUNDANESE       SU     OCEANIC/INDONESIAN
_STLIT8(L111, "su");
//SWEDISH         SV     GERMANIC
_STLIT8(L112, "sv");
//SWAHILI         SW     NEGRO-AFRICAN
_STLIT8(L113, "sw");
//TAMIL           TA     DRAVIDIAN
_STLIT8(L114, "ta");
//TELUGU          TE     DRAVIDIAN
_STLIT8(L115, "te");
//TAJIK           TG     IRANIAN
_STLIT8(L116, "tg");
//THAI            TH     ASIAN
_STLIT8(L117, "th");
//TIGRINYA        TI     SEMITIC
_STLIT8(L118, "ti");
//TURKMEN         TK     TURKIC/ALTAIC
_STLIT8(L119, "tk");
//TAGALOG         TL     OCEANIC/INDONESIAN
_STLIT8(L120, "tl");
//SETSWANA        TN     NEGRO-AFRICAN
_STLIT8(L121, "tn");
//TONGA           TO     OCEANIC/INDONESIAN
_STLIT8(L122, "to");
//TURKISH         TR     TURKIC/ALTAIC
_STLIT8(L123, "tr");
//TSONGA          TS     NEGRO-AFRICAN
_STLIT8(L124, "ts");
//TATAR           TT     TURKIC/ALTAIC
_STLIT8(L125, "tt");
//TWI             TW     NEGRO-AFRICAN
_STLIT8(L126, "tw");
//UIGUR           UG     [       ]
_STLIT8(L127, "ug");
//UKRAINIAN       UK     SLAVIC
_STLIT8(L128, "uk");
//URDU            UR     INDIAN
_STLIT8(L129, "ur");
//UZBEK           UZ     TURKIC/ALTAIC
_STLIT8(L130, "uz");
//VIETNAMESE      VI     ASIAN
_STLIT8(L131, "vi");
//VOLAPUK         VO     INTERNATIONAL AUX.
_STLIT8(L132, "vo");
//WOLOF           WO     NEGRO-AFRICAN
_STLIT8(L133, "wo");
//XHOSA           XH     NEGRO-AFRICAN
_STLIT8(L134, "xh");
//YIDDISH         YI     GERMANIC [*Changed 1989 from original ISO 639:1988, JI]
_STLIT8(L135, "yi");
//YORUBA          YO     NEGRO-AFRICAN
_STLIT8(L136, "yo");
//ZHUANG          ZA     [       ]
_STLIT8(L137, "za");
//PRCCHINESE      ZH     ASIAN
_STLIT8(L138, "zh-cn, zh");
//ZULU            ZU     NEGRO-AFRICAN
_STLIT8(L139, "zu");
//TAIWANCHINESE   ZH     ASIAN
_STLIT8(L140, "zh-tw, zh");
//HONGKONGCHINESE ZH     ASIAN
_STLIT8(L141, "zh-hk, zh");
//BRAZILIANPORTUGUESE    PT     ROMANCE
_STLIT8(L142, "pt-br, pt");
//US ENGLISH			 EN		GERMANIC
_STLIT8(L143, "en-us, en");
//CANADIAN ENGLISH		 EN		GERMANIC
_STLIT8(L144, "en-ca, en");
//AUSTRALIAN ENGLISH	 EN		GERMANIC
_STLIT8(L145, "en-au, en");
//NEWZEALAND ENGLISH	 EN		GERMANIC
_STLIT8(L146, "en-nz, en");
//SA ENGLISH			 EN		GERMANIC
_STLIT8(L147, "en-za, en");
//CANADIAN FRENCH		 FR 	ROMANCE
_STLIT8(L148, "fr-ca, fr");
//BELGIAN FRENCH		 FR 	ROMANCE
_STLIT8(L149, "fr-be, fr");
//FINLAND SWEDISH		 SV 	GERMANIC
_STLIT8(L150, "sv-fi, sv");
//SWISS ITALIAN			 IT		ROMANCE		
_STLIT8(L151, "it-ch, it");
//BELARUSSIAN			 BE		SLAVIC
_STLIT8(L152, "be, ru");

const void * const KLanguageStringPointers[] =
{
    (const void*)&L1,
    (const void*)&L2,
    (const void*)&L3,
    (const void*)&L4,
    (const void*)&L5,
    (const void*)&L6,
    (const void*)&L7,
    (const void*)&L8,
    (const void*)&L9,
    (const void*)&L10,
    (const void*)&L11,
    (const void*)&L12,
    (const void*)&L13,
    (const void*)&L14,
    (const void*)&L15,
    (const void*)&L16,
    (const void*)&L17,
    (const void*)&L18,
    (const void*)&L19,
    (const void*)&L20,
    (const void*)&L21,
    (const void*)&L22,
    (const void*)&L23,
    (const void*)&L24,
    (const void*)&L25,
    (const void*)&L26,
    (const void*)&L27,
    (const void*)&L28,
    (const void*)&L29,
    (const void*)&L30,
    (const void*)&L31,
    (const void*)&L32,
    (const void*)&L33,
    (const void*)&L34,
    (const void*)&L35,
    (const void*)&L36,
    (const void*)&L37,
    (const void*)&L38,
    (const void*)&L39,
    (const void*)&L40,
    (const void*)&L41,
    (const void*)&L42,
    (const void*)&L43,
    (const void*)&L44,
    (const void*)&L45,
    (const void*)&L46,
    (const void*)&L47,
    (const void*)&L48,
    (const void*)&L49,
    (const void*)&L50,
    (const void*)&L51,
    (const void*)&L52,
    (const void*)&L53,
    (const void*)&L54,
    (const void*)&L55,
    (const void*)&L56,
    (const void*)&L57,
    (const void*)&L58,
    (const void*)&L59,
    (const void*)&L60,
    (const void*)&L61,
    (const void*)&L62,
    (const void*)&L63,
    (const void*)&L64,
    (const void*)&L65,
    (const void*)&L66,
    (const void*)&L67,
    (const void*)&L68,
    (const void*)&L69,
    (const void*)&L70,
    (const void*)&L71,
    (const void*)&L72,
    (const void*)&L73,
    (const void*)&L74,
    (const void*)&L75,
    (const void*)&L76,
    (const void*)&L77,
    (const void*)&L78,
    (const void*)&L79,
    (const void*)&L80,
    (const void*)&L81,
    (const void*)&L82,
    (const void*)&L83,
    (const void*)&L84,
    (const void*)&L85,
    (const void*)&L86,
    (const void*)&L87,
    (const void*)&L88,
    (const void*)&L89,
    (const void*)&L90,
    (const void*)&L91,
    (const void*)&L92,
    (const void*)&L93,
    (const void*)&L94,
    (const void*)&L95,
    (const void*)&L96,
    (const void*)&L97,
    (const void*)&L98,
    (const void*)&L99,
    (const void*)&L100,
    (const void*)&L101,
    (const void*)&L102,
    (const void*)&L103,
    (const void*)&L104,
    (const void*)&L105,
    (const void*)&L106,
    (const void*)&L107,
    (const void*)&L108,
    (const void*)&L109,
    (const void*)&L110,
    (const void*)&L111,
    (const void*)&L112,
    (const void*)&L113,
    (const void*)&L114,
    (const void*)&L115,
    (const void*)&L116,
    (const void*)&L117,
    (const void*)&L118,
    (const void*)&L119,
    (const void*)&L120,
    (const void*)&L121,
    (const void*)&L122,
    (const void*)&L123,
    (const void*)&L124,
    (const void*)&L125,
    (const void*)&L126,
    (const void*)&L127,
    (const void*)&L128,
    (const void*)&L129,
    (const void*)&L130,
    (const void*)&L131,
    (const void*)&L132,
    (const void*)&L133,
    (const void*)&L134,
    (const void*)&L135,
    (const void*)&L136,
    (const void*)&L137,
    (const void*)&L138,
    (const void*)&L139,
    (const void*)&L140,
    (const void*)&L141,
    (const void*)&L142,
    (const void*)&L143,
    (const void*)&L144,
    (const void*)&L145,
    (const void*)&L146,
    (const void*)&L147,
    (const void*)&L148,
    (const void*)&L149,
    (const void*)&L150,
    (const void*)&L151,
    (const void*)&L152
};

const TStringTable HttpFilterCommonStringsExt::iLanguageTable =
    {
        sizeof(KLanguageStringPointers) / sizeof(void*), KLanguageStringPointers, EFalse
    };

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HttpFilterCommonStringsExt::GetTable
// Get the string table containing strings used by both filters and their clients.
// -----------------------------------------------------------------------------
//
EXPORT_C const TStringTable& HttpFilterCommonStringsExt::GetTable()
    {
    return iTable;
    }

// -----------------------------------------------------------------------------
// HttpFilterCommonStringsExt::GetLanguageTable
// Get the language string table containing Symbian supported language codes.
// -----------------------------------------------------------------------------
//
EXPORT_C const TStringTable& HttpFilterCommonStringsExt::GetLanguageTable()
    {
    return iLanguageTable;
    }

// -----------------------------------------------------------------------------
// HttpFilterCommonStringsExt::GetLocaleLanguage
// Get the current used language from the Symbian OS and convert it to the
// corressponding string
// -----------------------------------------------------------------------------
//
EXPORT_C RStringF HttpFilterCommonStringsExt::GetLocaleLanguage(RStringPool aStringPool)
    {
    TLanguage lang = User::Language();
    TLanguageStrings index = GetIndexFromLanguage ( lang );
    return aStringPool.StringF(index, HttpFilterCommonStringsExt::GetLanguageTable());
    }

// -----------------------------------------------------------------------------
// HttpFilterCommonStringsExt::GetLanguageString
// Get the the corressponding string from languageCode passed in
// -----------------------------------------------------------------------------
//
EXPORT_C RStringF HttpFilterCommonStringsExt::GetLanguageString(RStringPool aStringPool, TInt aLanguageCode)
    {
    TLanguage lang = (TLanguage) aLanguageCode;
    enum TLanguageStrings index = GetIndexFromLanguage ( lang );
    return aStringPool.StringF(index, HttpFilterCommonStringsExt::GetLanguageTable());
    }

// -----------------------------------------------------------------------------
// HttpFilterCommonStringsExt::GetIndexFromLanguage
// Get the index from a language code
// -----------------------------------------------------------------------------
//
HttpFilterCommonStringsExt::TLanguageStrings HttpFilterCommonStringsExt::GetIndexFromLanguage(TLanguage aLanguage)
    {
    TLanguageStrings index = EEnglish; // English is default language
    switch (aLanguage)
        {
        /* Language enums are found in e32std.h */
        case ELangEnglish:                   // 01
        index = EEnglish;
        break;
        case ELangFrench:                    // 02
        index = EFrench;
        break;
        case ELangGerman:                    // 03
        index = EGerman;
        break;
        case ELangSpanish:                   // 04
        index = ESpanish;
        break;
        case ELangItalian:                   // 05
        index = EItalian;
        break;
        case ELangSwedish:                   // 06
        index = ESwedish;
        break;
        case ELangDanish:                    // 07
        index = EDanish;
        break;
        case ELangNorwegian:                 // 08
        index = ENorwegian;
        break;
        case ELangFinnish:                   // 09
        index = EFinnish;
        break;
        case ELangAmerican:                  // 10
        index = EAmerican;
        break;
        case ELangPortuguese:                // 13
        index = EPortuguese;
        break;
        case ELangTurkish:                   // 14
        index = ETurkish;
        break;
        case ELangIcelandic:                 // 15
        index = EIcelandic;
        break;
        case ELangRussian:                   // 16
        index = ERussian;
        break;
        case ELangHungarian:                 // 17
        index = EHungarian;
        break;
        case ELangDutch:                     // 18
        index = EDutch;
        break;
        case ELangBelgianFrench:             // 21
        index = EBelgianFrench;
        break;
        case ELangInternationalFrench:       // 24
        index = EFrench;
        break;
        case ELangCzech:                     // 25
        index = ECzech;
        break;
        case ELangSlovak:                    // 26
        index = ESlovak;
        break;
        case ELangPolish:                    // 27
        index = EPolish;
        break;
        case ELangSlovenian:                 // 28
        index = ESlovenian;
        break;
        case ELangTaiwanChinese:             // 29
        index = ETaiwanChinese;
        break;
        case ELangHongKongChinese:           // 30
        index = EHongKongChinese;
        break;
        case ELangPrcChinese:                // 31
        index = EPrcChinese;
        break;
        case ELangJapanese:                  // 32
        index = EJapanese;
        break;
        case ELangThai:                      // 33
        index = EThai;
        break;
        case ELangAfrikaans:                 // 34
        index = EAfrikaans;
        break;
        case ELangAlbanian:                  // 35
        index = EAlbanian;
        break;
        case ELangAmharic:                   // 36
        index = EAmharic;
        break;
        case ELangArabic:                    // 37
        index = EArabic;
        break;
        case ELangArmenian:                  // 38
        index = EArmenian;
        break;
        case ELangBelarussian:               // 40
        index = EBelarussian;
        break;
        case ELangBengali:                   // 41
        index = EBengali;
        break;
        case ELangBulgarian:                 // 42
        index = EBulgarian;
        break;
        case ELangBurmese:                   // 43
        index = EBurmese;
        break;
        case ELangCatalan:                   // 44
        index = ECatalan;
        break;
        case ELangCroatian:                  // 45
        index = ECroatian;
        break;
        case ELangCanadianEnglish:           // 46
        index = ECanadianEnglish;
        break;
        case ELangAustralian:
        index = EAustralian;
        break;
        case ELangNewZealand:
        index = ENewZealand;
        break;
        case ELangInternationalEnglish:      // 47
        index = EEnglish;
        break;
        case ELangSouthAfricanEnglish:       // 48
        index = ESouthAfricanEnglish;
        break;
        case ELangEstonian:                  // 49
        index = EEstonian;
        break;
        case ELangFarsi:                     // 50
        index = EPersian;
        break;
        case ELangCanadianFrench:            // 51
        index = ECanadianFrench;
        break;
        case ELangScotsGaelic:               // 52
        index = EScotsGaelic;
        break;
        case ELangGeorgian:                  // 53
        index = EGeorgian;
        break;
        case ELangGreek:                     // 54
        index = EGreek;
        break;
        case ELangCyprusGreek:               // 55
        index = EGreek;
        break;
        case ELangGujarati:                  // 56
        index = EGujarati;
        break;
        case ELangHebrew:                    // 57
        index = EHebrew;
        break;
        case ELangHindi:                     // 58
        index = EHindi;
        break;
        case ELangIndonesian:                // 59
        index = EIndonesian;
        break;
        case ELangIrish:                     // 60
        index = EIrish;
        break;
        case ELangSwissItalian:              // 61
        index = ESwissItalian;
        break;
        case ELangKannada:                   // 62
        index = EKannada;
        break;
        case ELangKazakh:                    // 63
        index = EKazakh;
        break;
        case ELangKorean:                    // 65
        index = EKorean;
        break;
        case ELangLao:                       // 66
        index = ELaothian;
        break;
        case ELangLatvian:                   // 67
        index = ELatvian;
        break;
        case ELangLithuanian:                // 68
        index = ELithuanian;
        break;
        case ELangMacedonian:                // 69
        index = EMacedonian;
        break;
        case ELangMalay:                     // 70
        case ELangMalay_Apac:                // 326
        index = EMalay;
        break;
        case ELangMalayalam:                 // 71
        index = EMalayalam;
        break;
        case ELangMarathi:                   // 72
        index = EMarathi;
        break;
        case ELangMoldavian:                 // 73
        index = EMoldavian;
        break;
        case ELangMongolian:                 // 74
        index = EMongolian;
        break;
        case ELangNorwegianNynorsk:          // 75
        index = ENorwegian;
        break;
        case ELangBrazilianPortuguese:       // 76
        index = EBrazilianPortuguese;
        break;
        case ELangPunjabi:                   // 77
        index = EPunjabi;
        break;
        case ELangRomanian:                  // 78
        index = ERomanian;
        break;
        case ELangSerbian:                   // 79
        index = ESerbian;
        break;
        case ELangSinhalese:                 // 80
        index = ESinghalese;
        break;
        case ELangSomali:                    // 81
        index = ESomali;
        break;
        case ELangInternationalSpanish:      // 82
        case ELangLatinAmericanSpanish:      // 83
        index = ESpanish;
        break;
        case ELangSwahili:                   // 84
        index = ESwahili;
        break;
        case ELangFinlandSwedish:            // 85
        index = EFinlandSwedish;
        break;
        case ELangTamil:                     // 87
        index = ETamil;
        break;
        case ELangTelugu:                    // 88
        index = ETelugu;
        break;
        case ELangTibetan:                   // 89
        index = ETibetan;
        break;
        case ELangTigrinya:                  // 90
        index = ETigrinya;
        break;
        case ELangCyprusTurkish:             // 91
        index = ETurkish;
        break;
        case ELangTurkmen:                   // 92
        index = ETurkmen;
        break;
        case ELangUkrainian:                 // 93
        index = EUkrainian;
        break;
        case ELangUrdu:                      // 94
        index = EUrdu;
        break;
        case ELangVietnamese:                // 96
        index = EVietnamese;
        break;
        case ELangWelsh:                     // 97
        index = EWelsh;
        break;
        case ELangZulu:                      // 98
        index = EZulu;
        break;
        case ELangOther:                     // 99
        default:
        break;
        }
    return index;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
