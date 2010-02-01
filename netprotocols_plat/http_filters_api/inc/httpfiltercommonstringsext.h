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



#ifndef HTTPFILTERCOMMONSTRINGSEXT_H
#define HTTPFILTERCOMMONSTRINGSEXT_H

//  INCLUDES
#include <stringpool.h>
#include <stringtablesupport.h>
#include <httpstringconstants.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Additional string tables used by both the filters and their clients
*
*  @lib httpcommon.lib
*  @since 2.0
*/
class HttpFilterCommonStringsExt
    {
    public:   // Constructors and destructor


    public:  // New functions

        /**
        * Get the string table containing strings used by both filters and their clients.
        * @since 2.0
        * @param 
        * @return The string table
        */
        IMPORT_C static const TStringTable& GetTable();

        /**
        * Get the language string table containing Symbian supported language codes.
        * @since 2.0
        * @param 
        * @return The languages string table
        */
        IMPORT_C static const TStringTable& GetLanguageTable();

        /**
        * Get the current used language from the Symbian OS and convert it to the corressponding string
        * @since 2.0
        * @param aStringPool The string pool to use for creating the language string
        * @return The language string table for the current used language
        */
        IMPORT_C static RStringF GetLocaleLanguage(RStringPool aStringPool);


        /**
        * Get the the corressponding string from languageCode passed in
        * @since 2.6
        * @param aStringPool The string pool to use for creating the language string
        * @param aLanguageCode which is one of the installed languages
	    * @return The language string for one of the installed languages
        */
		IMPORT_C static RStringF GetLanguageString(RStringPool aStringPool, TInt aLanguageCode);

    public:      // Data
        // Enumeration of the strings in the string table
        enum TStrings
        {
            EApplicationXWwwMultipartFormData,
            EFormData,
            EName,
            EBoundary,
            EUtf8,
            EUsAscii,
            EIso10646Ucs2,
            EIso88591,
            EProxyAuthenticate,
            EProxyAuthorization,
            EProxyUsername,
            EProxyPassword,
            EProxyRealm,
            EApplicationXhtmlXml,
            EApplicationVndWapXhtmlXml,
            ETextCss,
            EAcceptHeadersDone,
            EBig5,
            EIso88592,
            EIso88593,
            EIso88594,
            EIso88595,
            EIso88596,
            EIso88597,
            EIso88598,
            EIso88599,
            EShiftJIS,
            EUtf16,
            EUtf16be,
            EUtf16le,
            EImageVndWapWbmp,
            EImageGif,
            EApplicationWmlXml,
            EApplicationVndWapWbxml,
            EApplicationVndWapWmlc,
            EApplicationVndWapWmlscriptc,
            EApplicationVndOmaDrm,
            EFromCache,
            EProxyFilter,
            EConnectionStatus,
            EHttpNewConnFlag,
            ELoadType,   
            EFilterCacheMode,
            ECacheFilter,
            EWindows1255,
            EWindows1256,
            EGb2312,
            EConnHandlerFilter,
            EIopFilter,
            EWindows1250,
            EWindows1251,
            EWindows1253,
            EWindows1254,
            EWindows1257,
            EUtf7,
            ETis620,
            EJisX0201_1997,
            EJisX0208_1997,
            EIso2022Jp,
            EEucJp,
            EIMEINotify,
            EWindows874,
            EKoi8_r,
            EKoi8_u,
			EAccessPointID,
			ETransactionCallback,
			EXGzip,
			EContentInflated,
            EConnectionCallback,
            EIso885915,
            EWindows1252,
            EApplicationVndOmaDrmContent,
			ENtlm,
			ENegotiate,
            ESelfPtr,
            EEucKr,
            EKsc5601,
            ESelfDownloadCallback
    };

        static const TStringTable iTable;

        // Enumeration of the languages strings in the language string table
        enum TLanguageStrings
        {
            EAfar,
            EAbkhazian,
            EAfrikaans,
            EAmharic,
            EArabic,
            EAssamese,
            EAymara,
            EAzerbaijani,
            EBashkir,
            EByelorussian,
            EBulgarian,
            EBihari,
            EBislama,
            EBengali,
            ETibetan,
            EBreton,
            ECatalan,
            ECorsican,
            ECzech,
            EWelsh,
            EDanish,
            EGerman,
            EBhutani,
            EGreek,
            EEnglish,
            EEsperanto,
            ESpanish,
            EEstonian,
            EBasque,
            EPersian,
            EFinnish,
            EFiji,
            EFaroese,
            EFrench,
            EFrisian,
            EIrish,
            EScotsGaelic,
            EGalician,
            EGuarani,
            EGujarati,
            EHausa,
            EHebrew,
            EHindi,
            ECroatian,
            EHungarian,
            EArmenian,
            EInterlingua,
            EInterlingue,
            EInupiak,
            EIndonesian,
            EIcelandic,
            EItalian,
            EInuktitut,
            EJapanese,
            EJavanese,
            EGeorgian,
            EKazakh,
            EGreenlandic,
            ECambodian,
            EKannada,
            EKorean,
            EKashmiri,
            EKurdish,
            EKirghiz,
            ELatin,
            ELingala,
            ELaothian,
            ELithuanian,
            ELatvian,
            EMalagasy,
            EMaori,
            EMacedonian,
            EMalayalam,
            EMongolian,
            EMoldavian,
            EMarathi,
            EMalay,
            EMaltese,
            EBurmese,
            ENauru,
            ENepali,
            EDutch,
            ENorwegian,
            EOccitan,
            EAfan,
            EOriya,
            EPunjabi,
            EPolish,
            EPashto,
            EPortuguese,
            EQuechua,
            ERhaeto,
            EKurundi,
            ERomanian,
            ERussian,
            EKinyarwanda,
            ESanskrit,
            ESindhi,
            ESangho,
            ESerbo,
            ESinghalese,
            ESlovak,
            ESlovenian,
            ESamoan,
            EShona,
            ESomali,
            EAlbanian,
            ESerbian,
            ESiswati,
            ESesotho,
            ESundanese,
            ESwedish,
            ESwahili,
            ETamil,
            ETelugu,
            ETajik,
            EThai,
            ETigrinya,
            ETurkmen,
            ETagalog,
            ESetswana,
            ETonga,
            ETurkish,
            ETsonga,
            ETatar,
            ETwi,
            EUigur,
            EUkrainian,
            EUrdu,
            EUzbek,
            EVietnamese,
            EVolapuk,
            EWolof,
            EXhosa,
            EYiddish,
            EYoruba,
            EZhuang,
            EPrcChinese,
            EZulu,
            ETaiwanChinese,
            EHongKongChinese,
            EBrazilianPortuguese,
            EAmerican,
            ECanadianEnglish,
            EAustralian,
            ENewZealand,
            ESouthAfricanEnglish,
            ECanadianFrench,
            EBelgianFrench,
            EFinlandSwedish,
            ESwissItalian,
            EBelarussian
    };

        static const TStringTable iLanguageTable;

    private:      // internal function

       /**
        * Get the index from a language code
        * @since 2.6
        * @param aLanguage which is one of the installed languages
	    * @return The index
        */
		static TLanguageStrings GetIndexFromLanguage(TLanguage aLanguage);


    };

#endif      // HTTPFILTERCOMMONSTRINGSEXT_H

// End of File
