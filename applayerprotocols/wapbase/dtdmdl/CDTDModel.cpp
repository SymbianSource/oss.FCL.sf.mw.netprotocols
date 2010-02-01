// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

// header
#include <cdtdmodel.h>

#include <attrlut.h>

// debug macros
#ifdef DEBUG_DTD
#include <e32svr.h>
_LIT(KDebugInvalidStream, "Stream is INVALID - Unmatched: %S\n");
_LIT(KDebugValidStream, "Stream is VALID!"));
_LIT(KDebugMatchedText, "Matched \"%S\"\n");
#define DEBUG_MATCHED(A) { HBufC* matched = aParser.MarkedL(); CleanupStack::PushL(matched); RDebug::Print(KDebugMatchedText, &_L(A)); CleanupStack::PopAndDelete(); }
#define DEBUG_MARK()	 { aParser.Mark(); } // Mark can leave
#define DEBUG_UNMARK()	 { aParser.DeleteMark(); }
#else
#define DEBUG_MATCHED(A) {}
#define DEBUG_MARK()	 {}
#define DEBUG_UNMARK()	 {}
#endif


// constants
const TInt KValidCharMatchStartStingLength=2;

// parser strings
#define _STRING(X) _LIT(K ## X, #X)
_STRING(Match0Plus);
_STRING(Rules);
_STRING(Rule);
_STRING(Comment);
_STRING(optS);
_STRING(Space);
_STRING(Optional);
_STRING(Match1Plus);
_STRING(Or);
_STRING(And);
_STRING(GroupStart);
_STRING(GroupEnd);
_STRING(SStringStart);
_STRING(SStringEnd);
_STRING(DStringStart);
_STRING(DStringEnd);
_STRING(Pop);
_STRING(Letter);
_STRING(Digit);
_STRING(HexNumber);
_STRING(DecNumber);
_STRING(HexChar);
_STRING(DecChar);
_STRING(ValidHexChar);
_STRING(BaseChar);
_STRING(ValidChar);
_STRING(SStringChar);
_STRING(DStringChar);
_STRING(Identifier);
_STRING(SString);
_STRING(DString);
_STRING(String);
_STRING(Primary);
_STRING(Factor);
_STRING(Definition);
_STRING(Definitions);
_STRING(DataReference);
_STRING(AttlistLine);
_STRING(EntityDefinition);
_STRING(CharRefStream);
_STRING(EntityDef);
_STRING(ReferenceName);
_STRING(EntityDeclName);
_STRING(ElementDeclName);
_STRING(NameChar);
_STRING(AttlistDeclName);
_STRING(EntityDecl);
_STRING(ElementDecl);
_STRING(AttDefRequired);
_STRING(AttDefImplied);
_STRING(AttDefFixed);
_STRING(AttDefault);
_STRING(Reference);
_STRING(AttlistReference);
_STRING(AttlistDef);
_STRING(AttlistDecl);
_STRING(CommentChar);
_STRING(AttfieldName);
_STRING(AttfieldTypeName);
_STRING(AttValue);
_STRING(GEContentS);
_STRING(GEContentD);
_STRING(GEDecl);
_STRING(PEDecl);

_LIT(KRefNameInitialChar,	"%");
_LIT(KRefNameEndChar,		";");
_LIT(KEntityInitialChar,	"%");
_LIT(KWhiteSpaceString,		" \n\r\t");
_LIT(KQMark,				"?");
_LIT(KStar,					"*");
_LIT(KPlus,					"+");
_LIT(KSpaceChar,			" ");
_LIT(KOrCharacter,			"|");
_LIT(KAndCharacter,			",");
_LIT(KGroupStartChar,		"(");
_LIT(KGroupEndChar,			")");
_LIT(KStringStartCharacter,	"'");
_LIT(KStringEndCharacter,	"'");
_LIT(KStringStartCharacter2,"\"");
_LIT(KStringEndCharacter2,	"\"");
_LIT(KHexCharacterStart,	"&#");
_LIT(KHexCharacterEnd,		";");
_LIT(KDecCharacterStart,	"#");
_LIT(KBaseCharacters,		"-()+,./:=?;!*#@$_%^&<>|");
_LIT(KOtherValidCharacters,	"'\"");
_LIT(KStringCharacters,		"\"[]");
_LIT(KStringCharacters2,	"'[]");
_LIT(KCharRefStreamAmpersand, "&");
_LIT(KNameOtherCharacters,	".-_:");
_LIT(KGEContentCharacters,	"^<&'");
_LIT(KGEContentDCharacters, "^<&\"");
_LIT(KEntityDeclStart,		"<!ENTITY");
_LIT(KEntityDeclEnd,		">");
_LIT(KElementDeclStart,		"<!ELEMENT");
_LIT(KElementDeclEnd,		">");
_LIT(KRequired,				"#REQUIRED");
_LIT(KImplied,				"#IMPLIED");
_LIT(KFixed,				"#FIXED");
_LIT(KCData,				"CDATA");
_LIT(KPCData,				"#PCDATA");
_LIT(KNMToken,				"NMTOKEN");
_LIT(KID,					"ID");
_LIT(KAttributeListStart,	"<!ATTLIST");
_LIT(KAttlistDeclNameCharacters,"AttlistDeclName");
_LIT(KAttributeListEnd,		">");
_LIT(KCommentStart,			"<!--");
_LIT(KCommentCharacter,		"-");
_LIT(KCommentEnd,			"-->");

_LIT(KCharRefMatchingPattern, "*&#*;*");


/** Allocates and constructs a new DTD parser.

@return New parser
@param aLUT Attribute lookup table in which to store attributes for the rule tree
*/
EXPORT_C CDTDModel* CDTDModel::NewL(CAttributeLookupTable& aLUT)
	{ // static
	CDTDModel* model = new (ELeave) CDTDModel(aLUT);
	return (model);
	}

/** Constructor.

@param aLUT Attribute lookup table in which to store attributes for the rule tree
*/
EXPORT_C CDTDModel::CDTDModel(CAttributeLookupTable& aLUT)
: CBNFParser(aLUT)
	{
	}


/** Destructor. */
EXPORT_C CDTDModel::~CDTDModel()
	{
	DoClear();
	}

EXPORT_C void CDTDModel::ResetL()
/** Reset the parser to a state where it can accept and parse new input.
	Any existing state of parsing and input data is destroyed. */
	{
	CBNFParser::ResetL();
	DoClear();
	}


void CDTDModel::DoClear()
	{
	delete iNewAttfieldName;
	iNewAttfieldName = NULL;
	delete iNewElementAttribute;
	iNewElementAttribute = NULL;
	delete iNewTree;
	iNewTree = NULL;
	iNewRuleStack.Clear();

	iInPEDecl = EFalse;
	iCharRefCheck = EFalse;
	delete iGEContent;
	iGEContent = NULL;
	}

/** Called when the parser starts a conditional point (i.e. And/Or rules) in the BNF grammar.

This overrides CBNFParser::StartConditional().

@param aRuleType Rule type
*/
EXPORT_C void CDTDModel::StartConditional(TParserNodeTypes aRuleType)
	{
	if (aRuleType == EOr)
		iNewRuleStack.MarkL(EConditionalMark);
	}

/** Called when the parser ends a conditional point (i.e. And/Or rules) in the BNF grammar.

This overrides CBNFParser::EndConditional().

@param aRuleType Rule type
@param aSuccess True if processing was sucessful, otherwise false
*/
EXPORT_C void  CDTDModel::EndConditional(TParserNodeTypes aRuleType, TBool aSuccess)
	{
	if (aRuleType == EOr)
		{
		if (aSuccess)
			iNewRuleStack.RemoveMark(EConditionalMark);
		else
			iNewRuleStack.DeleteToMark(EConditionalMark);
		}
	}

void CDTDModel::PreRulesL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	me.iNewTree = me.NewBNFL();
	}


#ifdef DEBUG_DTD
const TInt KMaxMatchTextDebugOuput=100;

void CDTDModel::PostRulesL(CBNFParser& aParser)
	{ // static
	if (aParser.RuleMatched())
		{
		if (!aParser.Valid())
			{
			HBufC* unmatched = aParser.StringL();
			RDebug::Print(KDebugInvalidStream, unmatched->Left(Min(KMaxMatchTextDebugOuput, unmatched->Length())));
			delete unmatched;
			}
		else
			RDebug::Print(KDebugValidStream);
		}
	}
#else
void CDTDModel::PostRulesL(CBNFParser& /*aParser*/)
	{ // static
	}
#endif // DEBUG_DTD



void CDTDModel::PreElementDeclL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	DEBUG_MARK(); // Mark can leave
	me.iNewRuleStack.MarkL(ERuleStartMark);
	}

void CDTDModel::PostElementDeclL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postElementDecl");

		me.iNewRuleStack.MergeToMarkL(ERuleStartMark);
		CBNFNode* currentRule=me.iNewRuleStack.Pop();
		CleanupStack::PushL(currentRule);
		currentRule->ReparentL(me.iNewRule);
		CleanupStack::Pop(); // currentRule
		}
	else
		me.iNewRuleStack.DeleteToMark(ERuleStartMark);
	DEBUG_UNMARK();
	}


void CDTDModel::PreEntityDeclL(CBNFParser& aParser)
	{ // static
	DEBUG_MARK(); // Mark can leave
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);	
	me.iNewRuleStack.MarkL(ERuleStartMark);
	}

void CDTDModel::PostEntityDeclL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postEntityDecl");

		if (!me.iNewRuleStack.IsEmpty())
			{
			me.iNewRuleStack.MergeToMarkL(ERuleStartMark);
			CBNFNode* currentRule = me.iNewRuleStack.Pop();
			CleanupStack::PushL(currentRule);
			currentRule->ReparentL(me.iNewRule);
			CleanupStack::Pop(); // currentRule
			me.iNewRule->SetType(EAnd);
			}
		}
	else
		me.iNewRuleStack.DeleteToMark(ERuleStartMark);

	DEBUG_UNMARK();
	}


void CDTDModel::PreAttlistLineL(CBNFParser& aParser)
	{ // static
	DEBUG_MARK(); // Mark can leave
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	me.iNewRuleStack.MarkL(ERuleStartMark);
	if (me.iNewElementAttribute == NULL)
		me.iNewElementAttribute = new (ELeave) CDTDElementAttribute();
	}

void CDTDModel::PostAttlistLineL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postAttlistLine");

		me.iNewRuleStack.MergeToMarkL(ERuleStartMark);
		CBNFNode *currentRule = me.iNewRuleStack.Pop();
		if (currentRule)
			me.iNewElementAttribute->iType = currentRule;
		const HBufC* attNameId = me.AttributeLUT().Des2IDL(*me.iNewAttfieldName);
		delete me.iNewAttfieldName;
		me.iNewAttfieldName = NULL;

		me.iNewRule->AddAttributeL(attNameId, me.iNewElementAttribute);
		me.iNewElementAttribute = NULL;
		}
	else
		me.iNewRuleStack.DeleteToMark(ERuleStartMark);

	DEBUG_UNMARK();
	}

void CDTDModel::PostIdentifierL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postIdentifier");
		HBufC* matched=me.MarkedL();
		CleanupStack::PushL(matched);
		CBNFNode* newRule = me.NewComponentL(me.iNewTree, *matched);
		me.iNewRuleStack.PushL(newRule);
		CleanupStack::PopAndDestroy(); // matched
		}

	me.DeleteMark();
	}

void CDTDModel::PostReferenceNameL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postReferenceName");
		HBufC* matched = me.MarkedWithInitialTextL(KRefNameInitialChar);
		CleanupStack::PushL(matched);
		CBNFNode* newRule = me.NewComponentL(me.iNewTree, *matched);
		me.iNewRuleStack.PushL(newRule);
		CleanupStack::PopAndDestroy(); // matched
		}

	me.DeleteMark();
	}

void 
CDTDModel::PostAttlistReferenceL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		me.iNewRuleStack.MergeToMarkL(ERuleStartMark);
		CBNFNode *currentRule = me.iNewRuleStack.Pop();
		if (currentRule)
			me.iNewElementAttribute->iType = currentRule;
        me.iNewElementAttribute->iValueType = CDTDElementAttribute::EReference;
        const TDesC* unique = REINTERPRET_CAST(const TDesC*, me.iNewElementAttribute);
		me.iNewRule->AddAttributeL(unique, me.iNewElementAttribute);
		me.iNewElementAttribute = NULL;
		}
	else
		me.iNewRuleStack.DeleteToMark(ERuleStartMark);
	}

void CDTDModel::PostEntityDeclNameL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postEntityDeclName");

		HBufC* matched=NULL;
		if(me.iInPEDecl)
			matched=me.MarkedWithInitialTextL(KEntityInitialChar);
		else
			matched=me.MarkedL();
		CleanupStack::PushL(matched);
		CBNFNode& newRule = me.NewRuleL(me.iNewTree, *matched, EIncomplete, NULL, NULL, NULL);
		me.iNewRule = &newRule;
		CleanupStack::PopAndDestroy(); // matched	
		}
	
	me.DeleteMark();
	}

void CDTDModel::PostElementDeclNameL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postElementDeclName");

		HBufC* matched = me.MarkedL();
		CleanupStack::PushL(matched);
		CBNFNode& newRule = me.NewRuleL(me.iNewTree, *matched, EAnd, NULL, NULL, NULL);
		me.iNewRule = &newRule;
		CleanupStack::PopAndDestroy(); // matched		
		}

	me.DeleteMark();
	}


void CDTDModel::PostAttlistDeclNameL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		DEBUG_MATCHED("postAttlistDeclName");

	me.DeleteMark();
	}

void CDTDModel::PostAttfieldNameL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postAttfieldName");
		HBufC* matched=me.MarkedL();
		delete me.iNewAttfieldName;
		me.iNewAttfieldName = matched;
		}

	me.DeleteMark();
	}

void CDTDModel::PreAttValueL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	me.iNewRuleStack.MarkL(ERuleStartMark);
	}

void  CDTDModel::PostAttValueL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		me.iNewRuleStack.MergeToMarkL(ERuleStartMark);
		CBNFNode *currentRule = me.iNewRuleStack.Pop();
        // ASSUME me.iNewElementAttribute != NULL
		if (currentRule)
			me.iNewElementAttribute->iValue = currentRule;
		}
	else
		me.iNewRuleStack.DeleteToMark(ERuleStartMark);
	}

void CDTDModel::PostRequiredL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
        // ASSUME me.iNewElementAttribute != NULL
        me.iNewElementAttribute->iValueType = CDTDElementAttribute::ERequired;
		}
	}

void CDTDModel::PostImpliedL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
        // ASSUME me.iNewElementAttribute != NULL
        me.iNewElementAttribute->iValueType = CDTDElementAttribute::EImplied;
		}
	}
void 
CDTDModel::PostFixedL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
        // ASSUME me.iNewElementAttribute != NULL
        me.iNewElementAttribute->iValueType = CDTDElementAttribute::EFixed;
		}
	}

void CDTDModel::PostAndL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		CBNFNode *currentRule = me.iNewRuleStack.Head();
		if (currentRule->Type() != EAnd)
			{
			CBNFNode* newRule = me.NewComponentL(EAnd);
			CleanupStack::PushL(newRule);
			CBNFNode* currentRule = me.iNewRuleStack.Pop();
			CleanupStack::PushL(currentRule);
			currentRule->ReparentL(newRule);
			CleanupStack::Pop(2); // currentRule, newRule
			me.iNewRuleStack.PushL(newRule);
			}
		}
	}



void CDTDModel::PostRuleL(CBNFParser& aParser, TParserNodeTypes aType, const TDesC* aAttribute)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (me.RuleMatched())
		{
		CBNFNode *currentRule = me.iNewRuleStack.Head();
		if (currentRule->Type()!=aType)
			{
			CBNFNode* newRule = me.NewComponentL(aType);
			CleanupStack::PushL(newRule);

			if (aAttribute)
				me.AddComponentAttributeL(*newRule, aAttribute, 1);

			CBNFNode* currentRule = me.iNewRuleStack.Pop();
			CleanupStack::PushL(currentRule);
			currentRule->ReparentL(newRule);
			CleanupStack::Pop(2); // currentRule, newRule
			me.iNewRuleStack.PushL(newRule);
			}
		}
	}


void CDTDModel::PostOrL(CBNFParser& aParser)
	{ // static
	PostRuleL(aParser, EOr, NULL);
	}

void CDTDModel::PostOptionalL(CBNFParser& aParser)
	{ // static
	PostRuleL(aParser, EOptional, NULL);
	}

void CDTDModel::PostMatch0PlusL(CBNFParser& aParser)
	{ // static
	PostRuleL(aParser, ENMore, NULL);
	}

void CDTDModel::PostMatch1PlusL(CBNFParser& aParser)
	{ // static
	PostRuleL(aParser, ENMore, CBNFNode::KNMoreMinimum());
	}
	
void CDTDModel::PreGroupL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	me.iNewRuleStack.MarkL(EGroupMark);
	}

void CDTDModel::PostGroupL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);
	if (aParser.RuleMatched())
		me.iNewRuleStack.MergeToMarkL(EGroupMark);
	else
		me.iNewRuleStack.RemoveMark(EGroupMark);
	}


void CDTDModel::PostStringL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	if (me.RuleMatched() && !me.iCharRefCheck)
		{
		DEBUG_MATCHED("postString");
		HBufC* matched = me.MarkedL();
		CBNFNode* newRule = me.NewComponentL(EExact, matched);
		me.iNewRuleStack.PushL(newRule);
		}

	me.DeleteMark();
	}


void CDTDModel::PostValidCharL(CBNFParser& aParser, TRadix aRadix)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	if (me.RuleMatched())
		{
		DEBUG_MATCHED("postValidChar");		
		HBufC* matched = me.MarkedL();
		CleanupStack::PushL(matched);
		TLex lex(matched->Mid(KValidCharMatchStartStingLength));
		TUint32 value;
		lex.Val(value, aRadix);
		HBufC* hexChar = HBufC::NewL(1);
		hexChar->Des().Append(TChar(value));
		CBNFNode* newRule = me.NewComponentL(EExact, hexChar);
		me.iNewRuleStack.PushL(newRule);
		CleanupStack::PopAndDestroy(); // matched
		}

	me.DeleteMark();
	}

void CDTDModel::PostValidHexCharL(CBNFParser& aParser)
	{ // static
	PostValidCharL(aParser, EHex);
	}

void CDTDModel::PostValidDecCharL(CBNFParser& aParser)
	{ // static
	PostValidCharL(aParser, EDecimal);
	}


void CDTDModel::PreGEContentL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	me.iCharRefCheck = ETrue;
	me.Mark();	 // Mark can leave
	}

void CDTDModel::PostGEContentL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	if (me.RuleMatched())
		{
		HBufC* matched=me.MarkedL();
		delete me.iGEContent;
		me.iGEContent=matched;
		TPtr string=me.iGEContent->Des();
		me.CharRefReplacement(string);
		}
	me.iCharRefCheck=EFalse;
	me.DeleteMark();
	}

void CDTDModel::PostGEDeclL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	if (me.RuleMatched())
		{
		CBNFNode* newRule = me.NewComponentL(EExact, me.iGEContent);
		me.iGEContent=NULL;
		me.iNewRuleStack.PushL(newRule);		
		}
	}

void CDTDModel::PrePEDeclL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	me.iInPEDecl = ETrue;
	}

void CDTDModel::PostPEDeclL(CBNFParser& aParser)
	{ // static
	CDTDModel& me = REINTERPRET_CAST(CDTDModel&, aParser);

	me.iInPEDecl = EFalse;
	}

/** Creates a BNF rule tree to parse the input stream.

This overrides CBNFParser::TreeL().

@return BNF rule tree
*/
EXPORT_C CBNFNode* CDTDModel::TreeL()
	{
    CBNFNode* root = NewBNFL();
	CleanupStack::PushL(root);

	NewComponentL(root, *root, KRules);

	// Rules ::= ((S? Comment) | Rule )+ S?
//#pragma message(__FILE__ " note : Parameter entity references in the document body")
// It is possible, that there is a parameter entity reference in
// document body. This should be replaced with its corresponding replacement
// text and then processed.
	CBNFNode& Rules = NewRuleL(root, KRules, EAnd, NULL, PreRulesL, PostRulesL);
	CBNFNode& Rules1 = NewComponentL(Rules, ENMore);
	CBNFNode& Rules2 = NewComponentL(Rules1, EOr);
	CBNFNode& Rules3 = NewComponentL(Rules2, EAnd);
	NewComponentL(root, Rules3, KoptS);
	NewComponentL(root, Rules3, KComment);
	NewComponentL(root, Rules2, KRule);
	AddComponentAttributeL(Rules1, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, Rules, KoptS);

	// Space ::= #x20 | #x9 | #x d | #xa
	NewRuleL(root, KSpace, ESelect, KWhiteSpaceString, NULL, NULL);
	// OptionalSpace ::= Space* 
	CBNFNode& optSpace = NewRuleL(root, KoptS, ENMore, NULL, NULL, NULL);
	NewComponentL(root, optSpace, KSpace);

	// Optional ::= '?'
	NewRuleL(root, KOptional, EExact, KQMark, NULL, PostOptionalL);
	// Match 0 or More := '*'
	NewRuleL(root, KMatch0Plus, EExact, KStar, NULL, PostMatch0PlusL);
	// Match 1 or More := '+'
	NewRuleL(root, KMatch1Plus, EExact, KPlus, NULL, PostMatch1PlusL);
	// Or ::= '|'
	NewRuleL(root, KOr, EExact, KOrCharacter, NULL, PostOrL);
	// And ::= ','
	NewRuleL(root, KAnd, EExact, KAndCharacter, NULL, PostAndL);
	// GroupStart ::= '('
	NewRuleL(root, KGroupStart, EExact, KGroupStartChar, PreGroupL, NULL);
	// GroupEnd ::= ')'
	NewRuleL(root, KGroupEnd, EExact, KGroupEndChar, PostGroupL, NULL);
	// SStringStart ::= '''
	NewRuleL(root, KSStringStart, EExact, KStringStartCharacter, NULL, NULL);
	// SStringEnd ::= '''
	NewRuleL(root, KSStringEnd, EExact, KStringEndCharacter, NULL, NULL);
	// DStringStart ::= '"'
	NewRuleL(root, KDStringStart, EExact, KStringStartCharacter2, NULL, NULL);
	// DStringEnd ::= '"'
	NewRuleL(root, KDStringEnd, EExact, KStringEndCharacter2, NULL, NULL);

	// PostOp ::= Optional | Macth0Plus | Match1Plus
    CBNFNode& Pop = NewRuleL(root, KPop, EOr, NULL, NULL, NULL);
	NewComponentL(root, Pop, KOptional);
	NewComponentL(root, Pop, KMatch0Plus);
	NewComponentL(root, Pop, KMatch1Plus);

	// Letter ::= [A-Za-z]
	CBNFNode& letter = NewRuleL(root, KLetter, EOr, NULL, NULL, NULL);
	CBNFNode& letter1 = NewComponentL(letter, ERange);
	AddComponentAttributeL(letter1, CBNFNode::KRangeStart(), 'A');
	AddComponentAttributeL(letter1, CBNFNode::KRangeEnd(), 'Z');
	CBNFNode& letter2 = NewComponentL(letter, ERange);
	AddComponentAttributeL(letter2, CBNFNode::KRangeStart(), 'a');
	AddComponentAttributeL(letter2, CBNFNode::KRangeEnd(), 'z');

	// Digit ::= [0-9]
	CBNFNode& Digit = NewRuleL(root, KDigit, ERange, NULL, NULL, NULL);
	AddComponentAttributeL(Digit, CBNFNode::KRangeStart(), '0');
	AddComponentAttributeL(Digit, CBNFNode::KRangeEnd(), '9');

	// HexNumber ::= (Digit | [a-fA-f])+
    CBNFNode& HexNumber = NewRuleL(root, KHexNumber, ENMore, NULL, NULL, NULL);
	AddComponentAttributeL(HexNumber, CBNFNode::KNMoreMinimum(), 1);
	CBNFNode& HexNumber1 = NewComponentL(HexNumber, EOr);
	NewComponentL(root, HexNumber1, KDigit);
	CBNFNode& HexNumber2 = NewComponentL(HexNumber1, ERange);
	AddComponentAttributeL(HexNumber2, CBNFNode::KRangeStart(), 'a');
	AddComponentAttributeL(HexNumber2, CBNFNode::KRangeEnd(), 'f');
	CBNFNode& HexNumber3 = NewComponentL(HexNumber1, ERange);
	AddComponentAttributeL(HexNumber3, CBNFNode::KRangeStart(), 'A');
	AddComponentAttributeL(HexNumber3, CBNFNode::KRangeEnd(), 'F');

	// DecNumber ::= Digit+
    CBNFNode& DecNumber = NewRuleL(root, KDecNumber, ENMore, NULL, NULL, NULL);
	AddComponentAttributeL(DecNumber, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, DecNumber, KDigit);

	// HexChar ::= "#x" HexNumber ";"
    CBNFNode& HexChar = NewRuleL(root, KHexChar, EAnd, NULL, NULL, NULL);
	NewComponentL(HexChar, EExact, KHexCharacterStart);
	NewComponentL(root, HexChar, KHexNumber);
	NewComponentL(HexChar, EExact, KHexCharacterEnd);

	// DecChar ::= "#" DecNumber ";"
    CBNFNode& DecChar = NewRuleL(root, KDecChar, EAnd, NULL, NULL, NULL);
	NewComponentL(DecChar, EExact, KDecCharacterStart);
	NewComponentL(root, DecChar, KDecNumber);
	NewComponentL(DecChar, EExact, KHexCharacterEnd);

	// ValidHexChar ::= HexChar
    CBNFNode& ValidHexChar = NewRuleL(root, KValidHexChar, EAnd, NULL, MarkCallback, PostValidHexCharL);
	NewComponentL(root, ValidHexChar, KHexChar);

	// BaseChar ::= ' ' | Letter | Digit | [-()+,./:=?;!*#@$_%^&<>|]
    CBNFNode& BaseChar = NewRuleL(root, KBaseChar, EOr, NULL, NULL, NULL);
	NewComponentL(BaseChar, EExact, KSpaceChar);
	NewComponentL(root, BaseChar, KLetter);
	NewComponentL(root, BaseChar, KDigit);
	NewComponentL(BaseChar, ESelect, KBaseCharacters);

	// ValidChar ::= BaseChar | ValidHexChar | ['"]
    CBNFNode& ValidChar = NewRuleL(root, KValidChar, EOr, NULL, NULL, NULL);
	NewComponentL(root, ValidChar, KBaseChar);
	NewComponentL(root, ValidChar, KValidHexChar);
	NewComponentL(ValidChar, ESelect, KOtherValidCharacters);

	// SStringChar ::= BaseChar | ["[]] | S
    CBNFNode& SStringChar = NewRuleL(root, KSStringChar, EOr, NULL, NULL, NULL);
	NewComponentL(root, SStringChar, KBaseChar);
	NewComponentL(SStringChar, ESelect, KStringCharacters);
	NewComponentL(root, SStringChar, KSpace);

	// DStringChar ::= BaseChar | ['[]] | S
    CBNFNode& DStringChar = NewRuleL(root, KDStringChar, EOr, NULL, NULL, NULL);
	NewComponentL(root, DStringChar, KBaseChar);
	NewComponentL(DStringChar, ESelect, KStringCharacters2);
	NewComponentL(root, DStringChar, KSpace);

	// Identifier ::= Letter (Letter | Digit)*
    CBNFNode& identifier = NewRuleL(root, KIdentifier, EAnd, NULL, MarkCallback, PostIdentifierL);
	NewComponentL(root, identifier, KLetter);
	CBNFNode& identifier1 = NewComponentL(identifier, ENMore);
	CBNFNode& identifier2 = NewComponentL(identifier1, EOr);
	NewComponentL(root, identifier2, KLetter);
	NewComponentL(root, identifier2, KDigit);

	// SString ::= SStringChar+
    CBNFNode& sstring = NewRuleL(root, KSString, ENMore, NULL, MarkCallback, PostStringL);
	AddComponentAttributeL(sstring, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, sstring, KSStringChar);

	// DString ::= DStringChar+
    CBNFNode& dstring = NewRuleL(root, KDString, ENMore, NULL, MarkCallback, PostStringL);
	AddComponentAttributeL(dstring, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, dstring, KDStringChar);

	// String ::= SStringStart SString SStringEnd
	//            | DStringStart DString DStringEnd
	//            | ValidHexChar
    CBNFNode& string = NewRuleL(root, KString, EOr, NULL, NULL, NULL);
	CBNFNode& string1 = NewComponentL(string, EAnd);
	NewComponentL(root, string1, KSStringStart);
	NewComponentL(root, string1, KSString);
	NewComponentL(root, string1, KSStringEnd);
	CBNFNode& string2 = NewComponentL(string, EAnd);
	NewComponentL(root, string2, KDStringStart);
	NewComponentL(root, string2, KDString);
	NewComponentL(root, string2, KDStringEnd);
	NewComponentL(root, string, KValidHexChar);

    // Primary ::= S? (GroupStart Definitions S? GroupEnd
	//				  | DataReference
	//                | Identifier
	//                | String)
	CBNFNode& Primary = NewRuleL(root, KPrimary, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Primary, KoptS);
	CBNFNode& Primary0 = NewComponentL(Primary, EOr);

	CBNFNode& Primary4 = NewComponentL(Primary0, EAnd);
	NewComponentL(root, Primary4, KGroupStart);
	NewComponentL(root, Primary4, KDefinitions);
	NewComponentL(root, Primary4, KoptS);
	NewComponentL(root, Primary4, KGroupEnd);

	NewComponentL(root, Primary0, KDataReference);
	NewComponentL(root, Primary0, KIdentifier);
	NewComponentL(root, Primary0, KString);

	// Factor ::= Primary Pop?
	CBNFNode& Factor = NewRuleL(root, KFactor, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Factor, KPrimary);
	CBNFNode& Factor1 = NewComponentL(Factor, EOptional);
	NewComponentL(root, Factor1, KPop);
	
	// Definition ::= Factor (S? And Factor)*
	CBNFNode& Definition = NewRuleL(root, KDefinition, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Definition, KFactor);
	CBNFNode& Definition1 = NewComponentL(Definition, ENMore);
	CBNFNode& Definition2 = NewComponentL(Definition1, EAnd);
	NewComponentL(root, Definition2, KoptS);
	NewComponentL(root, Definition2, KAnd);
	NewComponentL(root, Definition2, KFactor);

	// Definitions ::= Definition (S? Or Definition)*
	CBNFNode& Definitions = NewRuleL(root, KDefinitions, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Definitions, KDefinition);
	CBNFNode& Definitions1 = NewComponentL(Definitions, ENMore);
	CBNFNode& Definitions2 = NewComponentL(Definitions1, EAnd);
	NewComponentL(root, Definitions2, KoptS);
	NewComponentL(root, Definitions2, KOr);
	NewComponentL(root, Definitions2, KDefinition);

	// EntityDefinition ::= (S? AttlistLine)+ | Definition
    CBNFNode& EntityDefinition = NewRuleL(root, KEntityDefinition, EOr, NULL, NULL, NULL);
	CBNFNode& EntityDefinition1 = NewComponentL(EntityDefinition, ENMore);
	CBNFNode& EntityDefinition11 = NewComponentL(EntityDefinition1, EAnd);
	NewComponentL(root, EntityDefinition11, KoptS);
	NewComponentL(root, EntityDefinition11, KAttlistLine);
	AddComponentAttributeL(EntityDefinition1, CBNFNode::KNMoreMinimum(), 1);
	NewComponentL(root, EntityDefinition, KDefinitions);

	// CharRefStream ::= "&" (DecChar | HexChar)+
    CBNFNode& CharRefStream = NewRuleL(root, KCharRefStream, EAnd, NULL, MarkCallback, PostStringL);
	NewComponentL(CharRefStream, EExact, KCharRefStreamAmpersand);
	CBNFNode& CharRefStream1 = NewComponentL(CharRefStream, ENMore);
	AddComponentAttributeL(CharRefStream1, CBNFNode::KNMoreMinimum(), 1);
	CBNFNode& CharRefStream2 = NewComponentL(CharRefStream1, EOr);
	NewComponentL(root, CharRefStream2, KDecChar);
	NewComponentL(root, CharRefStream2, KHexChar);

	// EntityDef ::= DStringStart (CharRefStream | EntityDefinition) DStringEnd
//#pragma message(__FILE__ " note : An entity (also a Parameter entity) can be surrounded with single quotes, too")
    CBNFNode& EntityDef = NewRuleL(root, KEntityDef, EAnd, NULL, NULL, NULL);
	NewComponentL(root, EntityDef, KDStringStart);
	CBNFNode& EntityDef1 = NewComponentL(EntityDef, EOr);
	NewComponentL(root, EntityDef1, KCharRefStream);
	NewComponentL(root, EntityDef1, KEntityDefinition);
	NewComponentL(root, EntityDef, KDStringEnd);

	// ReferenceName ::= Identifier
    CBNFNode& ReferenceName = NewRuleL(root, KReferenceName, EAnd, NULL, MarkCallback, PostReferenceNameL);
	NewComponentL(root, ReferenceName, KLetter);
	CBNFNode& ReferenceName1 = NewComponentL(ReferenceName, ENMore);
	CBNFNode& ReferenceName2 = NewComponentL(ReferenceName1, EOr);
	NewComponentL(root, ReferenceName2, KLetter);
	NewComponentL(root, ReferenceName2, KDigit);

	// EntityDeclName ::= Identifier
    CBNFNode& EntityDeclName = NewRuleL(root, KEntityDeclName, EAnd, NULL, MarkCallback, PostEntityDeclNameL);
	NewComponentL(root, EntityDeclName, KLetter);
	CBNFNode& EntityDeclName1 = NewComponentL(EntityDeclName, ENMore);
	CBNFNode& EntityDeclName2 = NewComponentL(EntityDeclName1, EOr);
	NewComponentL(root, EntityDeclName2, KLetter);
	NewComponentL(root, EntityDeclName2, KDigit);

	// ElementDeclName ::= Letter (Letter | Digit)*
    CBNFNode& ElementDeclName = NewRuleL(root, KElementDeclName, EAnd, NULL, MarkCallback, PostElementDeclNameL);
	NewComponentL(root, ElementDeclName, KLetter);
	CBNFNode& ElementDeclName1 = NewComponentL(ElementDeclName, ENMore);
	CBNFNode& ElementDeclName2 = NewComponentL(ElementDeclName1, EOr);
	NewComponentL(root, ElementDeclName2, KLetter);
	NewComponentL(root, ElementDeclName2, KDigit);

	// NameChar ::= Letter | Digit | [.-_:]
    CBNFNode& NameChar = NewRuleL(root, KNameChar, EOr, NULL, NULL, NULL);
	NewComponentL(root, NameChar, KLetter);
	NewComponentL(root, NameChar, KDigit);
	NewComponentL(NameChar, ESelect, KNameOtherCharacters);

	// AttlistDeclName ::= Letter (Letter | Digit)*
    CBNFNode& AttlistDeclName = NewRuleL(root, KAttlistDeclName, EAnd, NULL, MarkCallback, PostAttlistDeclNameL);
	NewComponentL(root, AttlistDeclName, KLetter);
	CBNFNode& AttlistDeclName1 = NewComponentL(AttlistDeclName, ENMore);
	CBNFNode& AttlistDeclName2 = NewComponentL(AttlistDeclName1, EOr);
	NewComponentL(root, AttlistDeclName2, KLetter);
	NewComponentL(root, AttlistDeclName2, KDigit);

	// AttfieldName ::= NameChar (NameChar | Digit)*
    CBNFNode& AttfieldName = NewRuleL(root, KAttfieldName, EAnd, NULL, MarkCallback, PostAttfieldNameL);
	NewComponentL(root, AttfieldName, KNameChar);
	CBNFNode& AttfieldName1 = NewComponentL(AttfieldName, ENMore);
	CBNFNode& AttfieldName2 = NewComponentL(AttfieldName1, EOr);
	NewComponentL(root, AttfieldName2, KNameChar);
	NewComponentL(root, AttfieldName2, KDigit);

	// AttfieldTypeName ::= Letter (Letter | Digit)*
    CBNFNode& AttfieldTypeName = NewRuleL(root, KAttfieldTypeName, EAnd, NULL, NULL, NULL);
	NewComponentL(root, AttfieldTypeName, KLetter);
	CBNFNode& AttfieldTypeName1 = NewComponentL(AttfieldTypeName, ENMore);
	CBNFNode& AttfieldTypeName2 = NewComponentL(AttfieldTypeName1, EOr);
	NewComponentL(root, AttfieldTypeName2, KLetter);
	NewComponentL(root, AttfieldTypeName2, KDigit);

	// AttValue ::= SStringStart (CharRefStream | ReferenceName) SStringEnd
	//            | DStringStart (CharRefStream | ReferenceName) DStringEnd
//#pragma message(__FILE__ " note : AttValue may contain several references")
// It is possible for the attribute value to contain several character
// references and parameter references, too. The complete attribute value
// should be stored as whole string, so that it can be used in the actual
// parser
    CBNFNode& AttValue = NewRuleL(root, KAttValue, EOr, NULL, PreAttValueL, PostAttValueL);
	CBNFNode& AttValue1 = NewComponentL(AttValue, EAnd);
	NewComponentL(root, AttValue1, KSStringStart);
	CBNFNode& AttValue11 = NewComponentL(AttValue1, EOr);
	NewComponentL(root, AttValue11, KCharRefStream);
	NewComponentL(root, AttValue11, KReferenceName);
	NewComponentL(root, AttValue11, KSString);
	NewComponentL(root, AttValue1, KSStringEnd);
	CBNFNode& AttValue2 = NewComponentL(AttValue, EAnd);
	NewComponentL(root, AttValue2, KDStringStart);
	CBNFNode& AttValue21 = NewComponentL(AttValue2, EOr);
	NewComponentL(root, AttValue21, KCharRefStream);
	NewComponentL(root, AttValue11, KReferenceName);
	NewComponentL(root, AttValue21, KDString);
	NewComponentL(root, AttValue2, KDStringEnd);

	CBNFNode& GEContentS = NewRuleL(root, KGEContentS, ENMore, NULL, PreGEContentL, PostGEContentL);
	CBNFNode& GEContentS1 = NewComponentL(GEContentS, EOr);
		NewComponentL(GEContentS1, ESelect, KGEContentCharacters);
		NewComponentL(root, GEContentS1, KCharRefStream);
//		NewComponentL(root, GEContent, PEReference);

	CBNFNode& GEContentD = NewRuleL(root, KGEContentD, ENMore, NULL, PreGEContentL, PostGEContentL);
	CBNFNode& GEContentD1 = NewComponentL(GEContentD, EOr);
		NewComponentL(GEContentD1, ESelect, KGEContentDCharacters);
		NewComponentL(root, GEContentD1, KCharRefStream);

	// GEDecl ::= EntityDeclName S? 
    CBNFNode& GEDecl = NewRuleL(root, KGEDecl, EAnd, NULL, NULL, PostGEDeclL);
	NewComponentL(root, GEDecl, KEntityDeclName);
	NewComponentL(root, GEDecl, KoptS);
	CBNFNode& GEDecl3 = NewComponentL(GEDecl, EOr);
		CBNFNode& GEDecl31 = NewComponentL(GEDecl3, EAnd);
			NewComponentL(root, GEDecl31, KSStringStart);
			NewComponentL(root, GEDecl31, KGEContentS);
			NewComponentL(root, GEDecl31, KSStringEnd);
		CBNFNode& GEDecl32 = NewComponentL(GEDecl3, EAnd);
			NewComponentL(root, GEDecl32, KDStringStart);
			NewComponentL(root, GEDecl32, KGEContentD);
			NewComponentL(root, GEDecl32, KDStringEnd);

	// PEDecl ::= "%" S? GEDecl  --- old ---
	// PEDecl ::= "%" S? EntityDeclName S? EntityDef
   	CBNFNode& PEDecl = NewRuleL(root, KPEDecl, EAnd, NULL, PrePEDeclL, PostPEDeclL);
	NewComponentL(PEDecl, EExact, KEntityInitialChar);
	NewComponentL(root, PEDecl, KoptS);
	NewComponentL(root, PEDecl, KEntityDeclName);
	NewComponentL(root, PEDecl, KoptS);
	NewComponentL(root, PEDecl, KEntityDef);

	// EntityDecl ::= "<!ENTITY" S (GEDecl | PEDecl) S? ">"
    CBNFNode& EntityDecl = NewRuleL(root, KEntityDecl, EAnd, NULL, PreEntityDeclL, PostEntityDeclL);
	NewComponentL(EntityDecl, EExact, KEntityDeclStart);
	NewComponentL(root, EntityDecl, KoptS);
	CBNFNode& EntityDecl1 = NewComponentL(EntityDecl, EOr);
	NewComponentL(root, EntityDecl1, KPEDecl);
	NewComponentL(root, EntityDecl1, KGEDecl);
	NewComponentL(root, EntityDecl, KoptS);
	NewComponentL(EntityDecl, EExact, KEntityDeclEnd);

	// ElementDecl ::= "<!ELEMENT" S ElementDeclName S Definitions S? ">"
    CBNFNode& ElementDecl = NewRuleL(root, KElementDecl, EAnd, NULL, PreElementDeclL, PostElementDeclL);
	NewComponentL(ElementDecl, EExact, KElementDeclStart);
	NewComponentL(root, ElementDecl, KoptS);
	NewComponentL(root, ElementDecl, KElementDeclName);
	NewComponentL(root, ElementDecl, KoptS);
	NewComponentL(root, ElementDecl, KDefinitions);
	NewComponentL(root, ElementDecl, KoptS);
	NewComponentL(ElementDecl, EExact, KElementDeclEnd);

	// AttDefRequired ::= "#REQUIRED"
    NewRuleL(root, KAttDefRequired, EExact, KRequired, NULL, PostRequiredL);
	// AttDefImplied ::= "#IMPLIED"
    NewRuleL(root, KAttDefImplied, EExact, KImplied, NULL, PostImpliedL);
	// AttDefFixed ::= "#FIXED"
    NewRuleL(root, KAttDefFixed, EExact, KFixed, NULL, PostFixedL);

	// AttDefault ::= AttDefRequired | AttDefImplied | (AttDefFixed S?)? AttValue
    CBNFNode& AttDefault = NewRuleL(root, KAttDefault, EOr, NULL, NULL, NULL);
	NewComponentL(root, AttDefault, KAttDefRequired);
	NewComponentL(root, AttDefault, KAttDefImplied);
	CBNFNode& AttDefault1 = NewComponentL(AttDefault, EAnd);
	CBNFNode& AttDefault11 = NewComponentL(AttDefault1, EOptional);
	CBNFNode& AttDefault111 = NewComponentL(AttDefault11, EAnd);
	NewComponentL(root, AttDefault111, KAttDefFixed);
	NewComponentL(root, AttDefault111, KoptS);
	NewComponentL(root, AttDefault1, KAttValue);

	// CDATA ::= "CDATA"
	NewRuleL(root, KCData, EExact, KCData, MarkCallback, PostIdentifierL);
	// PCDATA ::= "#PCDATA"
	NewRuleL(root, KPCData, EExact, KPCData, MarkCallback, PostIdentifierL);
	// NMTOKEN ::= "NMTOKEN"
	NewRuleL(root, KNMToken, EExact, KNMToken, MarkCallback, PostIdentifierL);
	// ID ::= "ID"
	NewRuleL(root, KID, EExact, KID, MarkCallback, PostIdentifierL);

	// Reference ::= "%" ReferenceName ";" 
    CBNFNode& Reference = NewRuleL(root, KReference, EAnd, NULL, NULL, NULL);
	NewComponentL(Reference, EExact, KRefNameInitialChar);
	NewComponentL(root, Reference, KReferenceName);
	NewComponentL(Reference, EExact, KRefNameEndChar);

	// AttlistReference ::= Reference
    CBNFNode& AttlistReference = NewRuleL(root, KAttlistReference, EAnd, NULL, PreAttlistLineL, PostAttlistReferenceL);
	NewComponentL(root, AttlistReference, KReference);

	// DataReference ::= CDATA | NMTOKEN | #PCDATA | ID | Reference
	CBNFNode& DataReference = NewRuleL(root, KDataReference, EOr, NULL, NULL, NULL);
	NewComponentL(root, DataReference, KCData);
	NewComponentL(root, DataReference, KNMToken);
	NewComponentL(root, DataReference, KPCData);
	NewComponentL(root, DataReference, KID);
	NewComponentL(root, DataReference, KReference);

	// AttlistLine ::= (AttfieldName S? Definition S? AttDefault)
	CBNFNode& AttlistLine = NewRuleL(root, KAttlistLine, EOr, NULL, PreAttlistLineL, PostAttlistLineL);
	CBNFNode& AttlistLine1 = NewComponentL(AttlistLine, EAnd);
	NewComponentL(root, AttlistLine1, KAttfieldName);
	NewComponentL(root, AttlistLine1, KoptS);
	NewComponentL(root, AttlistLine1, KDefinition);
	NewComponentL(root, AttlistLine1, KoptS);
	NewComponentL(root, AttlistLine1, KAttDefault);

	// AttlistDef ::=  AttlistLine | Reference
    CBNFNode& AttlistDef = NewRuleL(root, KAttlistDef, EOr, NULL, NULL, NULL);
	NewComponentL(root, AttlistDef, KAttlistLine);
	NewComponentL(root, AttlistDef, KAttlistReference);

	// AttlistDecl ::= "<!ATTLIST" S? AttlistDeclName S? (AttlistDef S?)* ">"
    CBNFNode& AttlistDecl = NewRuleL(root, KAttlistDecl, EAnd, NULL, NULL, NULL);
	NewComponentL(AttlistDecl, EExact, KAttributeListStart);
	NewComponentL(root, AttlistDecl, KoptS);
	NewComponentL(root, AttlistDecl, KAttlistDeclNameCharacters);
	NewComponentL(root, AttlistDecl, KoptS);
	CBNFNode& AttlistDecl1 = NewComponentL(AttlistDecl, ENMore);
	CBNFNode& AttlistDecl11 = NewComponentL(AttlistDecl1, EAnd);
	NewComponentL(root, AttlistDecl11, KAttlistDef);
	NewComponentL(root, AttlistDecl11, KoptS);
	NewComponentL(AttlistDecl, EExact, KAttributeListEnd);

	// CommentChar ::= S | [#x20-#xD7FF]
    CBNFNode& CommentChar = NewRuleL(root, KCommentChar, EOr, NULL, NULL, NULL);
	NewComponentL(root, CommentChar, KSpace);
	CBNFNode& CommentChar1 = NewComponentL(CommentChar, ERange);
	AddComponentAttributeL(CommentChar1, CBNFNode::KRangeStart(), 0x20);
	AddComponentAttributeL(CommentChar1, CBNFNode::KRangeEnd(), 0xD7FF);

	// Comment ::= "<!--" ((CommentChar - '-') | ('-' (CommentChar - '-')))* "-->"
	CBNFNode& Comment = NewRuleL(root, KComment, EAnd, NULL, NULL, NULL);
	NewComponentL(Comment, EExact, KCommentStart);
	CBNFNode& Comment0 = NewComponentL(Comment, ENMore);
	CBNFNode& Comment1 = NewComponentL(Comment0, EOr);

	CBNFNode& Comment11 = NewComponentL(Comment1, EWithout);
	NewComponentL(root, Comment11, KCommentChar);
	NewComponentL(Comment11, EExact, KCommentCharacter);
	CBNFNode& Comment12 = NewComponentL(Comment1, EAnd);
	NewComponentL(Comment12, EExact, KCommentCharacter);
	CBNFNode& Comment13 = NewComponentL(Comment12, EWithout);
	NewComponentL(root, Comment13, KCommentChar);
	NewComponentL(Comment13, EExact, KCommentCharacter);
	NewComponentL(Comment, EExact, KCommentEnd);

	// Rule ::= S? Comment? S? (EntityDecl | ElementDecl | AttlistDecl)
	CBNFNode& Rule = NewRuleL(root, KRule, EAnd, NULL, NULL, NULL);
	NewComponentL(root, Rule, KoptS);
	CBNFNode& Rule1 = NewComponentL(Rule, EOptional);
	NewComponentL(root, Rule1, KComment);
	NewComponentL(root, Rule, KoptS);
	CBNFNode& Rule2 = NewComponentL(Rule, EOr);
	NewComponentL(root, Rule2, KEntityDecl);
	NewComponentL(root, Rule2, KElementDecl);
	NewComponentL(root, Rule2, KAttlistDecl);

	CleanupStack::Pop(); // root
	return (root);
	}

void CDTDModel::CharRefReplacement(TPtr& aString)
// A method to replace character references to the given string with the referenced character
// This method does minimal set of checks since when this method is called the string has
// already been parsed and hence the references it contains are valid (this is ensured by the
// reference parsing rules). Therefore, this method should be called just before the data
// is being attached as an attribute value or as CDATA to a node. The reason why we don't replace
// the values during the parsing (i.e. when the values are checked) is that if the rule would
// later fail and the string would be re-parsed then the replaced characters might cause
// errorneous situations and unintended references to appear.
	{
	TPtrC examinable = aString;
	TInt examinableOffset = 0;
	TInt referenceOffset = examinable.Match(KCharRefMatchingPattern);
	while( referenceOffset != KErrNotFound )
		{
		TPtrC reference = examinable.Mid(referenceOffset);
		TInt referenceValueOffset = 2;
		TRadix system = EDecimal;
		if( reference[2] == 'x' )
			{
			referenceValueOffset++;
			system = EHex;
			}
		TLex valueString(reference.Mid(referenceValueOffset, reference.Locate(';')-referenceValueOffset) );
		TUint16 referenceValue = 32;	// just in case something fails, we shall insert space
		valueString.Val(referenceValue, system);
		aString.Delete(examinableOffset+referenceOffset, reference.Locate(';') );
		aString[examinableOffset+referenceOffset] = referenceValue;

		examinable.Set(aString.Mid(examinableOffset+referenceOffset+1));
		examinableOffset += referenceOffset+1;
		referenceOffset = examinable.Match(KCharRefMatchingPattern);
		}
	} 


/** Gets the root node of the tree generated to process the DTD.

It transfers ownership of the tree to the caller.

@return Root node of the tree
*/
EXPORT_C CBNFNode* CDTDModel::GeneratedTree()
	{ 
	CBNFNode* tree=iNewTree; 
	iNewTree=NULL;
	return tree; 
	}

void CDTDModel::AddNewAttributeL(CBNFNode& aNode, const TDesC* aAttributeId, const TDesC& aAttributeType, CDTDElementAttribute::KValueType aAttributeValueType, CBNFNode* aRootNode)
	{
	CDTDElementAttribute* newAttribute = new(ELeave)CDTDElementAttribute();
	CleanupStack::PushL(newAttribute);
	newAttribute->iValueType = aAttributeValueType;
	newAttribute->iType = NewComponentL(aRootNode, aAttributeType);
	aNode.AddAttributeL(aAttributeId, newAttribute);
	CleanupStack::Pop(); // newAttribute
	}

void CDTDModel::AddNewReferenceAttributeL(CBNFNode& aNode, const TDesC& aAttributeType, CBNFNode* aRootNode)
	{
	CDTDElementAttribute* newAttribute = new(ELeave)CDTDElementAttribute();
	CleanupStack::PushL(newAttribute);
	newAttribute->iValueType = CDTDElementAttribute::EReference;
	newAttribute->iType = NewComponentL(aRootNode, aAttributeType);
	aNode.AddAttributeL(REINTERPRET_CAST(const TDesC*, newAttribute), newAttribute);
	CleanupStack::Pop(); // newAttribute
	}

void CDTDModel::AddNewAttributeWithValueL(CBNFNode& aNode, const TDesC* aAttributeId, const TDesC& aAttributeType, const TDesC& aAttributeDefaultValue, CBNFNode* aRootNode)
	{
	CDTDElementAttribute* newAttribute = new(ELeave)CDTDElementAttribute();
	CleanupStack::PushL(newAttribute);
	newAttribute->iType = NewComponentL(aRootNode, aAttributeType);
	newAttribute->iValue = NewComponentL(EExact, aAttributeDefaultValue);
	aNode.AddAttributeL(aAttributeId, newAttribute);
	CleanupStack::Pop(); // newAttribute, 
	}

/** Builds a parser tree for the WML1.1 DTD.

@param aPackageRootNode A root node to which the generated tree is attached
*/
EXPORT_C void CDTDModel::BuildWml11DTDL(CBNFNode& aPackageRootNode)
{
	CBNFNode* dtdRoot = NewBNFL();
	dtdRoot->ReparentL(&aPackageRootNode);

	_LIT(KCDATA,"CDATA");
	_LIT(Klength,"%length");
	_LIT(Kvdata,"%vdata");
	_LIT(KHREF,"%HREF");
	_LIT(Ktrue,"true");
	_LIT(Kfalse,"false");
	_LIT(Kboolean,"%boolean");
	_LIT(KNMTOKEN,"NMTOKEN");
	_LIT(Knumber,"%number");
	_LIT(Kcoreattrs,"%coreattrs");
	_LIT(Kem,"em");
	_LIT(Kstrong,"strong");
	_LIT(Kb,"b");
	_LIT(Ki,"i");
	_LIT(Ku,"u");
	_LIT(Kbig,"big");
	_LIT(Ksmall,"small");
	_LIT(Kemph,"%emph");
	_LIT(Kbr,"br");
	_LIT(Klayout,"%layout");
	_LIT(KPCDATA,"#PCDATA");
	_LIT(Ktext,"%text");
	_LIT(Kimg,"img");
	_LIT(Kanchor,"anchor");
	_LIT(Ka,"a");
	_LIT(Ktable,"table");
	_LIT(Knoop,"noop");
	_LIT(Ktask,"%task");
	_LIT(Kdo,"do");
	_LIT(Knavelmts,"%navelmts");
	_LIT(Konevent,"onevent");
	_LIT(Kflow,"%flow");
	_LIT(KEMPTY,"EMPTY");
	_LIT(Kbottom,"bottom");
	_LIT(KIAlign,"%IAlign");
	_LIT(Kgo,"go");
	_LIT(Kprev,"prev");
	_LIT(Krefresh,"refresh");
	_LIT(Ktr,"tr");
	_LIT(Kpostfield,"postfield");
	_LIT(Ksetvar,"setvar");
	_LIT(Khead,"head");
	_LIT(Ktemplate,"template");
	_LIT(Kcard,"card");
	_LIT(Kwml,"wml");
	_LIT(Kaccess,"access");
	_LIT(Kmeta,"meta");
	_LIT(Kcardev,"%cardev");
	_LIT(Kp,"p");
	_LIT(Ktimer,"timer");
	_LIT(Kinput,"input");
	_LIT(Kselect,"select");
	_LIT(Kfieldset,"fieldset");
	_LIT(Kfields,"%fields");
	_LIT(Koption,"option");
	_LIT(Koptgroup,"optgroup");
	_LIT(KTAlign,"%TAlign");
	_LIT(KWrapMode,"%WrapMode");
	_LIT(Ktd,"td");

	_LIT(Kleft,"left");
	_LIT(Ktop,"top");
	_LIT(Kmiddle,"middle");
	_LIT(Kright,"right");
	_LIT(Kcenter,"center");
	_LIT(Kwrap,"wrap");
	_LIT(Knowrap,"nowrap");


	_LIT(Kxmllang, "xml:lang");
	const TDesC* xmllangAttributeId = iLUT.Des2IDL(Kxmllang);
	_LIT(Ktitle, "title");
	const TDesC* titleAttributeId = iLUT.Des2IDL(Ktitle);
	_LIT(Khref, "href");
	const TDesC* hrefAttributeId = iLUT.Des2IDL(Khref);
	_LIT(Kvalue, "value");
	const TDesC* valueAttributeId = iLUT.Des2IDL(Kvalue);
	_LIT(Ktabindex, "tabindex");
	const TDesC* tabindexAttributeId = iLUT.Des2IDL(Ktabindex);
	_LIT(Ktype, "type");
	const TDesC* typeAttributeId = iLUT.Des2IDL(Ktype);
	_LIT(Kname, "name");
	const TDesC* nameAttributeId = iLUT.Des2IDL(Kname);
	_LIT(Kalign, "align");
	const TDesC* alignAttributeId = iLUT.Des2IDL(Kalign);


	// <!ENTITY % length  "CDATA">
	CBNFNode& length = NewRuleL(dtdRoot, Klength, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, length,KCDATA);


	// <!ENTITY % vdata   "CDATA">
	CBNFNode& vdata = NewRuleL(dtdRoot, Kvdata, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, vdata,KCDATA);


	// <!ENTITY % HREF    "%vdata;">
	CBNFNode& href = NewRuleL(dtdRoot, KHREF, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, href,Kvdata);


	// <!ENTITY % boolean	"(true|false)">
	CBNFNode& boolean = NewRuleL(dtdRoot, Kboolean, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, boolean, Ktrue);
	NewComponentL(dtdRoot, boolean, Kfalse);


	// <!ENTITY % number  "NMTOKEN">  <!-- a number, with format [0-9]+ -->
	CBNFNode& number = NewRuleL(dtdRoot, Knumber, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, number, KNMTOKEN);

	CDTDElementAttribute* newAttribute = NULL;


	// !ENTITY % coreattrs  "id    ID     #IMPLIED  class CDATA  #IMPLIED">
	CBNFNode& coreattrs = NewRuleL(dtdRoot, Kcoreattrs, EIncomplete, NULL, NULL, NULL);
	// Attributes
	//  id ID #IMPLIED
	_LIT(KidAttrName, "id");
	_LIT(KID, "ID");
	AddNewAttributeL(coreattrs, iLUT.Des2IDL(KidAttrName), KID, CDTDElementAttribute::EImplied, dtdRoot);
	// class CDATA #IMPLIED
	_LIT(Kclass, "class");
	AddNewAttributeL(coreattrs, iLUT.Des2IDL(Kclass), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);


	// <!ENTITY % emph    "em | strong | b | i | u | big | small">
	CBNFNode& emph = NewRuleL(dtdRoot, Kemph, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, emph, Kem);
	NewComponentL(dtdRoot, emph, Kstrong);
	NewComponentL(dtdRoot, emph, Kb);
	NewComponentL(dtdRoot, emph, Ki);
	NewComponentL(dtdRoot, emph, Ku);
	NewComponentL(dtdRoot, emph, Kbig);
	NewComponentL(dtdRoot, emph, Ksmall);

	// <!ENTITY % layout  "br">
	CBNFNode& layout = NewRuleL(dtdRoot, Klayout, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, layout, Kbr);


	// <!ENTITY % text     "#PCDATA | %emph;">
	CBNFNode& text = NewRuleL(dtdRoot, Ktext, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, text, KPCDATA);
	NewComponentL(dtdRoot, text, Kemph);


	// <!ENTITY % flow     "%text; | %layout; | img | anchor | a | table">
	CBNFNode& flow = NewRuleL(dtdRoot, Kflow, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, flow, Ktext);
	NewComponentL(dtdRoot, flow, Klayout);
	NewComponentL(dtdRoot, flow, Kimg);
	NewComponentL(dtdRoot, flow, Kanchor);
	NewComponentL(dtdRoot, flow, Ka);
	NewComponentL(dtdRoot, flow, Ktable);

	// <!ENTITY % task   "go | prev | noop | refresh">
	CBNFNode& task = NewRuleL(dtdRoot, Ktask, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, task, Kgo);
	NewComponentL(dtdRoot, task, Kprev);
	NewComponentL(dtdRoot, task, Knoop);
	NewComponentL(dtdRoot, task, Krefresh);


	// <!ENTITY % navelmts "do | onevent">
	CBNFNode& navelmts = NewRuleL(dtdRoot, Knavelmts, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, navelmts, Kdo);
	NewComponentL(dtdRoot, navelmts, Konevent);


	// <!ELEMENT wml ( head?, template?, card+ )>
	CBNFNode& wml = NewRuleL(dtdRoot, Kwml, EAnd, NULL, NULL, NULL);
	CBNFNode& wml1 = NewComponentL( wml, EOptional);
		NewComponentL(dtdRoot, wml1,Khead);
	CBNFNode& wml2 = NewComponentL( wml, EOptional);
		NewComponentL(dtdRoot, wml2, Ktemplate);
	CBNFNode& wml3 = NewComponentL( wml, ENMore);
	AddComponentAttributeL( wml3, CBNFNode::KNMoreMinimum(), 1);
		NewComponentL(dtdRoot, wml3, Kcard);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(wml, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(wml, Kcoreattrs, dtdRoot);


	// <!ENTITY % cardev
	CBNFNode& cardev = NewRuleL(dtdRoot, Kcardev, EIncomplete, NULL, NULL, NULL);
	// Attributes
	// onenterforward %HREF #IMPLIED
	_LIT(Konenterforward, "onenterforward");
	AddNewAttributeL(cardev, iLUT.Des2IDL(Konenterforward), KHREF, CDTDElementAttribute::EImplied, dtdRoot);
	// onenterbackward %HREF #IMPLIED
	_LIT(Konenterbackward, "onenterbackward");
	AddNewAttributeL(cardev, iLUT.Des2IDL(Konenterbackward), KHREF, CDTDElementAttribute::EImplied, dtdRoot);
	// ontimer %HREF #IMPLIED
	_LIT(Kontimer, "ontimer");
	AddNewAttributeL(cardev, iLUT.Des2IDL(Kontimer), KHREF, CDTDElementAttribute::EImplied, dtdRoot);


	// <!ENTITY % fields  "%flow; | input | select | fieldset">
	CBNFNode& fields = NewRuleL(dtdRoot, Kfields, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, fields, Kflow);
	NewComponentL(dtdRoot, fields, Kinput);
	NewComponentL(dtdRoot, fields, Kselect);
	NewComponentL(dtdRoot, fields, Kfieldset);


	// <!ELEMENT card (onevent*, timer?, (do | p)*)>
	CBNFNode& card = NewRuleL(dtdRoot, Kcard, EAnd, NULL, NULL, NULL);
	CBNFNode& card1 = NewComponentL( card, ENMore);
		NewComponentL(dtdRoot, card1, Konevent);
	CBNFNode& card2 = NewComponentL( card, EOptional);
		NewComponentL(dtdRoot, card2, Ktimer);
	CBNFNode& card3 = NewComponentL( card, ENMore);
		CBNFNode& card31 = NewComponentL( card3, EOr);
			NewComponentL(dtdRoot, card31, Kdo);
			NewComponentL(dtdRoot, card31, Kp);
	// Attributes
	// title %vdata #IMPLIED
	AddNewAttributeL(card, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// newcontext %boolean "false"
	_LIT(Knewcontext, "newcontext");
	AddNewAttributeWithValueL(card, iLUT.Des2IDL(Knewcontext), Kboolean, Kfalse, dtdRoot);
	// ordered %boolean "true"
	_LIT(Kordered, "ordered");
	AddNewAttributeWithValueL(card, iLUT.Des2IDL(Kordered), Kboolean, Ktrue, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(card, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %cardev
	AddNewReferenceAttributeL(card, Kcardev, dtdRoot );
	// %coreattrs
	AddNewReferenceAttributeL(card, Kcoreattrs, dtdRoot);


	// <!ELEMENT do (%task;)>
	CBNFNode& doElement = NewRuleL(dtdRoot, Kdo, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, doElement, Ktask);
	// Attributes
	// type CDATA #REQUIRED
	AddNewAttributeL(doElement, typeAttributeId, KCDATA, CDTDElementAttribute::ERequired, dtdRoot);
	// label %vdata #IMPLIED
	_LIT(Klabel, "label");
	AddNewAttributeL(doElement, iLUT.Des2IDL(Klabel), Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// name NMTOKEN #IMPLIED
	AddNewAttributeL(doElement, nameAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// optional %boolean "false"
	_LIT(Koptional, "optional");
	AddNewAttributeWithValueL(doElement, iLUT.Des2IDL(Koptional), Kboolean, Kfalse, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(doElement, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(doElement, Kcoreattrs, dtdRoot);


	// <!ELEMENT onevent (%task;)>
	CBNFNode& onevent = NewRuleL(dtdRoot, Konevent, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, onevent, Ktask);
	// Attributes
	// type CDATA #REQUIRED
	AddNewAttributeL(onevent, typeAttributeId, KCDATA, CDTDElementAttribute::ERequired, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(onevent, Kcoreattrs, dtdRoot);


	// <!ELEMENT head ( access | meta )+>
	CBNFNode& head = NewRuleL(dtdRoot, Khead, EAnd, NULL, NULL, NULL);
	CBNFNode& head1 = NewComponentL( head, ENMore);
	AddComponentAttributeL( head1, CBNFNode::KNMoreMinimum(), 1);
		CBNFNode& head11 = NewComponentL( head1, EOr);
			NewComponentL(dtdRoot, head11, Kaccess);
			NewComponentL(dtdRoot, head11, Kmeta);
	// Attributes
	// %coreattrs
	AddNewReferenceAttributeL(head, Kcoreattrs, dtdRoot);


	// <!ELEMENT template (%navelmts;)*>
	CBNFNode& templateElement = NewRuleL(dtdRoot, Ktemplate, EAnd, NULL, NULL, NULL);
	CBNFNode& templateElement1 = NewComponentL( templateElement, ENMore);
		NewComponentL(dtdRoot, templateElement1, Knavelmts);
	// Attributes
	// %cardev
	AddNewReferenceAttributeL(templateElement, Kcardev, dtdRoot );
	// %coreattrs
	AddNewReferenceAttributeL(templateElement, Kcoreattrs, dtdRoot);


	// <!ELEMENT access EMPTY>
	CBNFNode& access = NewRuleL(dtdRoot, Kaccess, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, access, KEMPTY);
	// Attributes
	// domain CDATA #IMPLIED
	_LIT(Kdomain, "domain");
	AddNewAttributeL(access, iLUT.Des2IDL(Kdomain), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// path CDATA #IMPLIED
	_LIT(Kpath, "path");
	AddNewAttributeL(access, iLUT.Des2IDL(Kpath), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(access, Kcoreattrs, dtdRoot);


	// <!ELEMENT meta EMPTY>
	CBNFNode& meta = NewRuleL(dtdRoot, Kmeta, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, meta, KEMPTY);
	// Attributes
	// http-equiv CDATA #IMPLIED
	_LIT(Khttpequiv, "http-equiv");
	AddNewAttributeL(meta, iLUT.Des2IDL(Khttpequiv), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// name CDATA #IMPLIED
	AddNewAttributeL(meta, nameAttributeId, KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// forua %boolean #IMPLIED
	_LIT(Kforua, "forua");
	AddNewAttributeL(meta, iLUT.Des2IDL(Kforua), Kboolean, CDTDElementAttribute::EImplied, dtdRoot);
	// content CDATA #REQUIRED
	_LIT(Kcontent, "content");
	AddNewAttributeL(meta, iLUT.Des2IDL(Kcontent), KCDATA, CDTDElementAttribute::ERequired, dtdRoot);
	// scheme CDATA #IMPLIED
	_LIT(Kscheme, "scheme");
	AddNewAttributeL(meta, iLUT.Des2IDL(Kscheme), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(meta, Kcoreattrs, dtdRoot);


	// <!ELEMENT go (postfield | setvar)*>
	CBNFNode& go = NewRuleL(dtdRoot, Kgo, EAnd, NULL, NULL, NULL);
	CBNFNode& go1 = NewComponentL( go, ENMore);
		CBNFNode& go11 = NewComponentL( go1, EOr);
			NewComponentL(dtdRoot, go11, Kpostfield);
			NewComponentL(dtdRoot, go11, Ksetvar);
	// Attributes
	// href %HREF #REQUIRED
	AddNewAttributeL(go, hrefAttributeId, KHREF, CDTDElementAttribute::ERequired, dtdRoot);
	// sendreferer %boolean "false"
	_LIT(Ksendreferer, "sendreferer");
	AddNewAttributeWithValueL(go, iLUT.Des2IDL(Ksendreferer), Kboolean, Kfalse, dtdRoot);
	// method post | get "get"
	newAttribute = new(ELeave) CDTDElementAttribute();
	CleanupStack::PushL(newAttribute);
	_LIT(Kpost, "post");
	_LIT(Kget, "get");
	CBNFNode* methodTypeTree = NewComponentL(EOr);
	newAttribute->iType = methodTypeTree;
		NewComponentL(dtdRoot, *methodTypeTree, Kpost);
		NewComponentL(dtdRoot, *methodTypeTree, Kget);
	newAttribute->iValue = NewComponentL(EExact, Kget);
	_LIT(Kmethod, "method");
	go.AddAttributeL(iLUT.Des2IDL(Kmethod), newAttribute);
	CleanupStack::Pop(); // newAttribute
	// accept-charset CDATA #IMPLIED
	_LIT(Kacceptcharset, "accept-charset");
	AddNewAttributeL(go, iLUT.Des2IDL(Kacceptcharset), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(go, Kcoreattrs, dtdRoot);


	// <!ELEMENT prev (setvar)*>
	CBNFNode& prev = NewRuleL(dtdRoot, Kprev, EAnd, NULL, NULL, NULL);
	CBNFNode& prev1 = NewComponentL( prev, ENMore);
		NewComponentL(dtdRoot, prev1, Ksetvar);
	// Attributes
	// %coreattrs
	AddNewReferenceAttributeL(prev, Kcoreattrs, dtdRoot);


	// <!ELEMENT refresh (setvar)*>
	CBNFNode& refresh = NewRuleL(dtdRoot, Krefresh, EAnd, NULL, NULL, NULL);
	CBNFNode& refresh1 = NewComponentL( refresh, ENMore);
		NewComponentL(dtdRoot, refresh1,Ksetvar);
	// Attributes
	// %coreattrs
	AddNewReferenceAttributeL(refresh, Kcoreattrs, dtdRoot);


	// <!ELEMENT noop EMPTY>
	CBNFNode& noop = NewRuleL(dtdRoot, Knoop, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, noop, KEMPTY);
	// Attributes
	// %coreattrs
	AddNewReferenceAttributeL(noop, Kcoreattrs, dtdRoot);


	// <!ELEMENT postfield EMPTY>
	CBNFNode& postfield = NewRuleL(dtdRoot, Kpostfield, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, postfield, KEMPTY);
	// Attributes
	// name %vdata #REQUIRED
	AddNewAttributeL(postfield, nameAttributeId, Kvdata, CDTDElementAttribute::ERequired, dtdRoot);
	// value %vdata #REQUIRED
	AddNewAttributeL(postfield, valueAttributeId, Kvdata, CDTDElementAttribute::ERequired, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(postfield, Kcoreattrs, dtdRoot);


	// <!ELEMENT setvar EMPTY>
	CBNFNode& setvar = NewRuleL(dtdRoot, Ksetvar, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, setvar, KEMPTY);
	// Attributes
	// name %vdata #REQUIRED
	AddNewAttributeL(setvar, nameAttributeId, Kvdata, CDTDElementAttribute::ERequired, dtdRoot);
	// value %vdata #REQUIRED
	AddNewAttributeL(setvar, valueAttributeId, Kvdata, CDTDElementAttribute::ERequired, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(setvar, Kcoreattrs, dtdRoot);


	// <!ELEMENT select (optgroup|option)+>
	CBNFNode& select = NewRuleL(dtdRoot, Kselect, EAnd, NULL, NULL, NULL);
	CBNFNode& select1 = NewComponentL( select, ENMore);
	AddComponentAttributeL( select1, CBNFNode::KNMoreMinimum(), 1);
		CBNFNode& select12 = NewComponentL( select1, EOr);
			NewComponentL(dtdRoot, select12, Koptgroup);
			NewComponentL(dtdRoot, select12, Koption);
	// Attributes
	// title %vdata #IMPLIED
	AddNewAttributeL(select, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// name NMTOKEN #IMPLIED
	AddNewAttributeL(select, nameAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// value %vdata #IMPLIED
	AddNewAttributeL(select, valueAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// iname NMTOKEN #IMPLIED
	_LIT(Kiname, "iname");
	AddNewAttributeL(select, iLUT.Des2IDL(Kiname), KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// ivalue %vdata #IMPLIED
	_LIT(Kivalue, "ivalue");
	AddNewAttributeL(select, iLUT.Des2IDL(Kivalue), Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// multiple %boolean "false"
	_LIT(Kmultiple, "multiple");
	AddNewAttributeWithValueL(select, iLUT.Des2IDL(Kmultiple), Kboolean, Kfalse, dtdRoot);
	// tabindex %number #IMPLIED
	AddNewAttributeL(select, tabindexAttributeId, Knumber, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(select, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(select, Kcoreattrs, dtdRoot);


	// <!ELEMENT optgroup (optgroup|option)+ >
	CBNFNode& optgroup = NewRuleL(dtdRoot, Koptgroup, EAnd, NULL, NULL, NULL);
	CBNFNode& optgroup1 = NewComponentL( optgroup, ENMore);
	AddComponentAttributeL( optgroup1, CBNFNode::KNMoreMinimum(), 1);
		CBNFNode& optgroup11 = NewComponentL( optgroup1, EOr);
			NewComponentL(dtdRoot, optgroup11, Koptgroup);
			NewComponentL(dtdRoot, optgroup11, Koption);
	// Attributes
	// title %vdata #IMPLIED
	AddNewAttributeL(optgroup, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(optgroup, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(optgroup, Kcoreattrs, dtdRoot);


	// <!ELEMENT option (#PCDATA | onevent)*>
	CBNFNode& option = NewRuleL(dtdRoot, Koption, EAnd, NULL, NULL, NULL);
	CBNFNode& option1 = NewComponentL( option, ENMore);
		CBNFNode& option11 = NewComponentL( option1, EOr);
			NewComponentL(dtdRoot, option11, KPCDATA);
			NewComponentL(dtdRoot, option11, Konevent);
	// Attributes
	// value %vdata #IMPLIED
	AddNewAttributeL(option, valueAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// title %vdata #IMPLIED
	AddNewAttributeL(option, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// onpick %HREF #IMPLIED
	_LIT(Konpick, "onpick");
	AddNewAttributeL(option, iLUT.Des2IDL(Konpick), KHREF, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(option, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(option, Kcoreattrs, dtdRoot);


	// <!ELEMENT input EMPTY>
	CBNFNode& input = NewRuleL(dtdRoot, Kinput, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, input,KEMPTY);
	// Attributes
	// name NMTOKEN #REQUIRED
	AddNewAttributeL(input, nameAttributeId, KNMTOKEN, CDTDElementAttribute::ERequired, dtdRoot);
	// type text | password "text"
	newAttribute = new(ELeave) CDTDElementAttribute();
	CleanupStack::PushL(newAttribute);
	_LIT(KtextLit, "text");
	_LIT(Kpassword, "password");
	CBNFNode* inputtypeTypeTree = NewComponentL(EOr);
	newAttribute->iType = inputtypeTypeTree;
		NewComponentL(dtdRoot, *inputtypeTypeTree, KtextLit);
		NewComponentL(dtdRoot, *inputtypeTypeTree, Kpassword);
	newAttribute->iValue = NewComponentL(EExact, KtextLit);
	input.AddAttributeL(typeAttributeId, newAttribute);
	CleanupStack::Pop(); // newAttribute
	// value %vdata #IMPLIED
	AddNewAttributeL(input, valueAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// format CDATA #IMPLIED
	_LIT(Kformat, "format");
	AddNewAttributeL(input, iLUT.Des2IDL(Kformat), KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// emptyok %boolean "false"
	_LIT(Kemptyok, "emptyok");
	AddNewAttributeWithValueL(input, iLUT.Des2IDL(Kemptyok), Kboolean, Kfalse, dtdRoot);
	// size %number #IMPLIED
	_LIT(Ksize, "size");
	AddNewAttributeL(input, iLUT.Des2IDL(Ksize), Knumber, CDTDElementAttribute::EImplied, dtdRoot);
	// maxlength %number #IMPLIED
	_LIT(Kmaxlength, "maxlength");
	AddNewAttributeL(input, iLUT.Des2IDL(Kmaxlength), Knumber, CDTDElementAttribute::EImplied, dtdRoot);
	// tabindex %number #IMPLIED
	AddNewAttributeL(input, tabindexAttributeId, Knumber, CDTDElementAttribute::EImplied, dtdRoot);
	// title %vdata #IMPLIED
	AddNewAttributeL(input, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(input, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(input, Kcoreattrs, dtdRoot);


	// <!ELEMENT fieldset (%fields; | do)* >
	CBNFNode& fieldset = NewRuleL(dtdRoot, Kfieldset, EAnd, NULL, NULL, NULL);
	CBNFNode& fieldset1 = NewComponentL( fieldset, ENMore);
		CBNFNode& fieldset11 = NewComponentL( fieldset1, EOr);
			NewComponentL(dtdRoot, fieldset11, Kfields);
			NewComponentL(dtdRoot, fieldset11, Kdo);
	// Attributes
	// title %vdata #IMPLIED
	AddNewAttributeL(fieldset, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(fieldset, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(fieldset, Kcoreattrs, dtdRoot);


	// <!ELEMENT timer EMPTY>
	CBNFNode& timer = NewRuleL(dtdRoot, Ktimer, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, timer, KEMPTY);
	// Attributes
	// name NMTOKEN #IMPLIED
	AddNewAttributeL(timer, nameAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// value %vdata #REQUIRED
	AddNewAttributeL(timer, valueAttributeId, Kvdata, CDTDElementAttribute::ERequired, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(timer, Kcoreattrs, dtdRoot);


	// <!ENTITY % IAlign "(top|middle|bottom)" >
	CBNFNode& ialign = NewRuleL(dtdRoot, KIAlign, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, ialign,Ktop);
	NewComponentL(dtdRoot, ialign, Kmiddle);
	NewComponentL(dtdRoot, ialign, Kbottom);


	// <!ELEMENT img EMPTY>
	CBNFNode& img = NewRuleL(dtdRoot, Kimg, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, img, KEMPTY);
	// Attributes
	// alt %vdata #REQUIRED
	_LIT(Kalt, "alt");
	AddNewAttributeL(img, iLUT.Des2IDL(Kalt), Kvdata, CDTDElementAttribute::ERequired, dtdRoot);
	// src %HREF #REQUIRED
	_LIT(Ksrc, "src");
	AddNewAttributeL(img, iLUT.Des2IDL(Ksrc), KHREF, CDTDElementAttribute::ERequired, dtdRoot);
	// localsrc %vdata #IMPLIED
	_LIT(Klocalsrc, "localsrc");
	AddNewAttributeL(img, iLUT.Des2IDL(Klocalsrc), Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// vspace %length "0"
	_LIT(KNILL, "0");
	_LIT(Kvspace, "vspace");
	AddNewAttributeWithValueL(img, iLUT.Des2IDL(Kvspace), Klength, KNILL, dtdRoot);
	// hspace %length "0"
	_LIT(Khspace, "hspace");
	AddNewAttributeWithValueL(img, iLUT.Des2IDL(Khspace), Klength, KNILL, dtdRoot);
	// align %IAlign "bottom"
	AddNewAttributeWithValueL(img, alignAttributeId, KIAlign, Kbottom, dtdRoot);
	// height %length #IMPLIED
	_LIT(Kheight , "height");
	AddNewAttributeL(img, iLUT.Des2IDL(Kheight), Klength, CDTDElementAttribute::EImplied, dtdRoot);
	// width %length #IMPLIED
	_LIT(Kwidth, "width");
	AddNewAttributeL(img, iLUT.Des2IDL(Kwidth), Klength, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(img, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(img, Kcoreattrs, dtdRoot);


	// <!ELEMENT anchor ( #PCDATA | br | img | go | prev | refresh )*>
	CBNFNode& anchor = NewRuleL(dtdRoot, Kanchor, EAnd, NULL, NULL, NULL);
	CBNFNode& anchor1 = NewComponentL( anchor, ENMore);
	CBNFNode& anchor11 = NewComponentL( anchor1, EOr);
		NewComponentL(dtdRoot, anchor11, KPCDATA);
		NewComponentL(dtdRoot, anchor11, Kbr);
		NewComponentL(dtdRoot, anchor11, Kimg);
		NewComponentL(dtdRoot, anchor11, Kgo);
		NewComponentL(dtdRoot, anchor11, Kprev);
		NewComponentL(dtdRoot, anchor11,Krefresh);
	// Attributes
	// title %vdata #IMPLIED
	AddNewAttributeL(anchor, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(anchor, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(anchor, Kcoreattrs, dtdRoot);


	// <!ELEMENT a ( #PCDATA | br | img )*>
	CBNFNode& a = NewRuleL(dtdRoot, Ka, EAnd, NULL, NULL, NULL);
	CBNFNode& a1 = NewComponentL( a, ENMore);
		CBNFNode& a11 = NewComponentL( a1, EOr);
			NewComponentL(dtdRoot, a11, KPCDATA);
			NewComponentL(dtdRoot, a11, Kbr);
			NewComponentL(dtdRoot, a11, Kimg);
	// Attributes
	// href %HREF #REQUIRED
	AddNewAttributeL(a, hrefAttributeId, KHREF, CDTDElementAttribute::ERequired, dtdRoot);
	// title %vdata #IMPLIED
	AddNewAttributeL(a, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(a, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(a, Kcoreattrs, dtdRoot);


	// <!ELEMENT table (tr)+>
	CBNFNode& table = NewRuleL(dtdRoot, Ktable, EAnd, NULL, NULL, NULL);
	CBNFNode& table1 = NewComponentL( table, ENMore);
		AddComponentAttributeL( table1, CBNFNode::KNMoreMinimum(), 1);
		NewComponentL(dtdRoot, table1, Ktr);
	// Attributes
	// title %vdata #IMPLIED
	AddNewAttributeL(table, titleAttributeId, Kvdata, CDTDElementAttribute::EImplied, dtdRoot);
	// align CDATA #IMPLIED
	AddNewAttributeL(table, alignAttributeId, KCDATA, CDTDElementAttribute::EImplied, dtdRoot);
	// columns %number #REQUIRED
	_LIT(Kcolumns, "columns");
	AddNewAttributeL(table, iLUT.Des2IDL(Kcolumns), Knumber, CDTDElementAttribute::ERequired, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(table, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(table, Kcoreattrs, dtdRoot);


	// <!ELEMENT tr (td)+>
	CBNFNode& tr = NewRuleL(dtdRoot, Ktr, EAnd, NULL, NULL, NULL);
	CBNFNode& tr1 = NewComponentL( tr, ENMore);
	AddComponentAttributeL( tr1, CBNFNode::KNMoreMinimum(), 1);
		NewComponentL(dtdRoot, tr1, Ktd);
	// Attributes
	// %coreattrs
	AddNewReferenceAttributeL(tr, Kcoreattrs, dtdRoot);


	// <!ELEMENT td ( %text; | %layout; | img | anchor | a )*>
	CBNFNode& td = NewRuleL(dtdRoot, Ktd, EAnd, NULL, NULL, NULL);
	CBNFNode& td1 = NewComponentL( td, ENMore);
		CBNFNode& td11 = NewComponentL( td1, EOr);
			NewComponentL(dtdRoot, td11, Ktext);
			NewComponentL(dtdRoot, td11, Klayout);
			NewComponentL(dtdRoot, td11, Kimg);
			NewComponentL(dtdRoot, td11, Kanchor);
			NewComponentL(dtdRoot, td11, Ka);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(td, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(td, Kcoreattrs, dtdRoot);


	// <!ELEMENT em     (%flow;)*>
	CBNFNode& em = NewRuleL(dtdRoot, Kem, EAnd, NULL, NULL, NULL);
	CBNFNode& em1 = NewComponentL( em, ENMore);
	NewComponentL(dtdRoot, em1, Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(em, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(em, Kcoreattrs, dtdRoot);


	// <!ELEMENT strong (%flow;)*>
	CBNFNode& strong = NewRuleL(dtdRoot, Kstrong, EAnd, NULL, NULL, NULL);
	CBNFNode& strong1 = NewComponentL( strong, ENMore);
	NewComponentL(dtdRoot, strong1, Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(strong, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(strong, Kcoreattrs, dtdRoot);


	//<!ELEMENT b      (%flow;)*>
	CBNFNode& b = NewRuleL(dtdRoot, Kb, EAnd, NULL, NULL, NULL);
	CBNFNode& b1 = NewComponentL( b, ENMore);
	NewComponentL(dtdRoot, b1, Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(b, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(b, Kcoreattrs, dtdRoot);


	// <!ELEMENT i      (%flow;)*>
	CBNFNode& i = NewRuleL(dtdRoot, Ki, EAnd, NULL, NULL, NULL);
	CBNFNode& i1 = NewComponentL( i, ENMore);
	NewComponentL(dtdRoot, i1, Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(i, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(i, Kcoreattrs, dtdRoot);


	// <!ELEMENT u      (%flow;)*>
	CBNFNode& u = NewRuleL(dtdRoot, Ku, EAnd, NULL, NULL, NULL);
	CBNFNode& u1 = NewComponentL( u, ENMore);
	NewComponentL(dtdRoot, u1, Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(u, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(u, Kcoreattrs, dtdRoot);


	// <!ELEMENT big    (%flow;)*>
	CBNFNode& big = NewRuleL(dtdRoot, Kbig, EAnd, NULL, NULL, NULL);
	CBNFNode& big1 = NewComponentL( big, ENMore);
	NewComponentL(dtdRoot, big1,Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(big, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(big, Kcoreattrs, dtdRoot);


	// <!ELEMENT small  (%flow;)*>
	CBNFNode& small = NewRuleL(dtdRoot, Ksmall, EAnd, NULL, NULL, NULL);
	CBNFNode& small1 = NewComponentL( small, ENMore);
	NewComponentL(dtdRoot, small1,Kflow);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(small, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(small, Kcoreattrs, dtdRoot);


	// <!ENTITY % TAlign "(left|right|center)">
	CBNFNode& talign = NewRuleL(dtdRoot, KTAlign, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, talign,Kleft);
	NewComponentL(dtdRoot, talign, Kright);
	NewComponentL(dtdRoot, talign, Kcenter);


	// <!ENTITY % WrapMode "(wrap|nowrap)" >
	CBNFNode& wrapmode = NewRuleL(dtdRoot, KWrapMode, EOr, NULL, NULL, NULL);
	NewComponentL(dtdRoot, wrapmode, Kwrap);
	NewComponentL(dtdRoot, wrapmode, Knowrap);


	// <!ELEMENT p (%fields; | do)*>
	CBNFNode& p = NewRuleL(dtdRoot, Kp, EAnd, NULL, NULL, NULL);
	CBNFNode& p1 = NewComponentL( p, ENMore);
		CBNFNode& p11 = NewComponentL( p1, EOr);
			NewComponentL(dtdRoot, p11, Kfields);
			NewComponentL(dtdRoot, p11,Kdo);
	// Attributes
	// align %TAlign "left"
	AddNewAttributeWithValueL(p, alignAttributeId, KTAlign, Kleft, dtdRoot);
	// mode %WrapMode #IMPLIED
	_LIT(Kmode, "mode");
	AddNewAttributeL(p, iLUT.Des2IDL(Kmode), KWrapMode, CDTDElementAttribute::EImplied, dtdRoot);
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(p, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(p, Kcoreattrs, dtdRoot);


	// <!ELEMENT br EMPTY>
	CBNFNode& br = NewRuleL(dtdRoot, Kbr, EAnd, NULL, NULL, NULL);
	NewComponentL(dtdRoot, br, KEMPTY);
	// Attributes
	// xml:lang NMTOKEN #IMPLIED
	AddNewAttributeL(br, xmllangAttributeId, KNMTOKEN, CDTDElementAttribute::EImplied, dtdRoot);
	// %coreattrs
	AddNewReferenceAttributeL(br, Kcoreattrs, dtdRoot);


	_LIT(Kquot,"quot");
	CBNFNode& quot = NewRuleL(dtdRoot, Kquot, EAnd, NULL, NULL, NULL);
		_LIT(Kquote , "\"");
		NewComponentL( quot, EExact, Kquote );


	_LIT(Kamp,"amp");
	CBNFNode& amp = NewRuleL(dtdRoot, Kamp, EAnd, NULL, NULL, NULL);
		_LIT(Kampersand , "&#38;");
		NewComponentL( amp, EExact, Kampersand );

	_LIT(Kapos,"apos");
	CBNFNode& apos = NewRuleL(dtdRoot, Kapos, EAnd, NULL, NULL, NULL);
		_LIT(Kapostrophe , "'");
		NewComponentL( apos, EExact, Kapostrophe );

	_LIT(Klt,"lt");
	CBNFNode& lt = NewRuleL(dtdRoot, Klt, EAnd, NULL, NULL, NULL);
		_LIT(Klessthan , "&#60;");
		NewComponentL( lt, EExact, Klessthan );

	_LIT(Kgt,"gt");
	CBNFNode& gt = NewRuleL(dtdRoot, Kgt, EAnd, NULL, NULL, NULL);
		_LIT(Kgreaterthan , ">");
		NewComponentL( gt, EExact, Kgreaterthan );

	_LIT(Knbsp,"nbsp");
	CBNFNode& nbsp = NewRuleL(dtdRoot, Knbsp, EAnd, NULL, NULL, NULL);
		_LIT(Knonbreakingspace , "\xA0");
		NewComponentL( nbsp, EExact, Knonbreakingspace );

	_LIT(Kshy,"shy");
	CBNFNode& shy = NewRuleL(dtdRoot, Kshy, EAnd, NULL, NULL, NULL);
		_LIT(Kshorthyphen , "\xad");
		NewComponentL( shy, EExact, Kshorthyphen );
}
