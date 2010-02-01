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
// This class provides a mechanism to use a BNF tree to parse an input stream.
// The notation of the EBNF is based upon that described in the XML1.0 specification.
// The BNF tree form used is a variation on Extended BNF and has the following rule types,
// where the input stream must:
// , Exact - match exactly with the provided string.
// , Range - next character must be in the specified range.
// , Select - next character must exist in the selected string.
// If the select string starts with ^ it is a NOT Select.
// , And - match all of the given sub rules
// , Or - match one of the given sub rules
// , NMore - match N or more times the the SINGLE subrule.
// , Optional - match 0/1 times to the SINGLE subrule.
// , Without - match the first subrule but NOT the second.
// , Reference - match the referred to rule.
// The iterative parser not only validates an input stream against the 
// BNF grammer but allows pre/post actions to be performed during the parsing.
// Partial parsing is also allowed in that the input stream does not have to
// completed before parsing can begin. As soon as data is added the parser
// attempts to parse it.
// Numerous methods are provided to assist in the building of the BNF Tree this parser uses.
// To use this class:
// Create a derivation and implement the virtual method TreeL() to creat a BNF rule tree
// (the assistance methods NewBNF/NewRule etc should be used) - see DTDModel 
// To use your new parser invoke Reset and pass input data using the ProcessData method.
// 
//

#ifndef __CBNFPARSER_H__
#define __CBNFPARSER_H__

#include <e32std.h>
#include <mdataproviderobserver.h>
#include <cstack.h>
#include <cfragmentedstring.h>
#include <cbnfnode.h>

//
// forward class declarations
//
class CAttributeLookupTable;


// Rule Tree node type definitions
/** Defines types of node in a BNF tree (CBNFParser).

Except for ERoot, EIncomplete, EReference, and ELastParserNodeType, the 
types define different types of rule that the input stream must meet to 
satisfy the grammar. */
enum TParserNodeTypes
	{
	/** Root node. */
	ERoot, 
	/** Incomplete node. */
	EIncomplete, 
	/** Exact rule: match exactly with the provided string. */
	EExact, 
	/** Range rule: next character must be in the specified range.

	The start of the range is specified by a CBNFNode::KRangeStart() 
	attribute; the end by a CBNFNode::KRangeEnd() attribute. */
	ERange, 
	/** Select rule: next character must exist in the selected string.

	If the select string starts with ^, it is a NOT Select. */
	ESelect, 
	/** And rule: match all of the given sub-rules.

	Sub-rules are defined by the child nodes of the AND rule node. */
	EAnd, 
	/** Or rule: match one of the given sub-rules.

	Sub-rules are defined by the child nodes of the OR rule node. */
	EOr, 
	/** NMore rule: match a single subrule N or more times.

	A minimum is specified by a CBNFNode::KNMoreMinimum() attribute; a maximum by 
	a CBNFNode::KNMoreMaximum() attribute; an exact figure by a CBNFNode::KNMoreCount() attribute. */
	ENMore, 
	/** Optional rule: match a single sub-rule 0/1 times.

	A sub-rule is defined by the child node of the Optional rule node. */
	EOptional, 
	/** Without rule: match the first sub-rule but not the second.

	Sub-rules are defined by the child nodes of the Without rule node. */
	EWithout, 
	/** Reference rule: match the referred to rule.

	The target rule name is identified by a CBNFNode::KReference() attribute. */
	EReference, 
	/** Indicates final node type. */
	ELastParserNodeType	
	};

// Parser states
// 
// When a  the state is EActive. 
// Setting the parser state to something else in a pre-/post-rule callback function
// causes the parser to exit on next loop in ParseL. If the state is set to EStopped
// we have finished the parser operation (e.g. in event of an error), in state EPaused
// we are likely to resume the parser operation after some external operations.
/** CBNFParser parser states. */
enum TParseState
	{
	/** Parser has stopped. */
	EStopped, 
	/** Rarser is running. */
	EActive, 
	/** Parser has paused: e.g. waiting for further input to continue. */
	EPaused
	};



class CBNFParser : public CBase, public MDataProviderObserver
/** Base class for parsers that use a BNF tree to parse an input stream.

The BNF tree form used is a variation on Extended BNF described in the XML1.0 
specification. The general form of the tree is as follows:

Each node in the tree defines a rule that the input stream must meet to satisfy the grammar.

1. a node type is set to the rule type, as defined in TParserNodeTypes

2. node data stores any string required by the rule: e.g. for a comparison rule, the string 
	to match against

3. the parser allows callback functions to be called either before or after the rule is processed.
	 If these are present, they are stored as attributes of the node.

4. some rules allow sub-rules: for example, the AND rule expects a number of sub-rules, all 
	of which must be successful if the AND rule itself is to succeed. Each sub-rule is 
	represented as a child node of the parent rule. Sub-rules in turn can have sub-rules.

5. reference rule nodes are also allowed: these do not define themselves rules, but direct the 
	parser to another rule. They can link rules to each other and so build rule sequences more 
	complex than a simple tree.

All the top-level rules are stored as attributes of the root node. The attribute type is a string
 that names the rule; the attribute value is a pointer to the node that implements the rule.

The class supplies functions that encapsulate adding rules appropriately to the tree. The parser 
provider creates a derived class that implements the virtual method TreeL() that uses these 
functions to create a BNF rule tree.

The user of the parser initialises the parser with ResetL(), and then passes input data to the 
parser using ProcessData(). The parser supports partial parsing: the input stream does not have 
to completed before parsing can begin. As soon as data is added, the parser attempts to parse it.

	@publishedAll
	@deprecated

*/
	{
protected:
	/** Defines a type to handle a stack of rules. */
	typedef CStack<CBNFNode, EFalse> CRuleStack;

	/** Type definition for a callback function pointer
		Callback functions need to get a reference to the parser as parameter
		and they need to be static. */
	typedef void (TRuleCallback)(CBNFParser&);

public:
	// Constructor for a new parser instance
	//
	// Input:
	// aLUT - reference to attribute lookuptable; used to store all the stuff in the parser rule tree
	//
	//##ModelId=3B6669EA00F8
	IMPORT_C static CBNFParser* NewL(CAttributeLookupTable& aLUT);

	//##ModelId=3B6669EA00F7
	IMPORT_C virtual ~CBNFParser();

	// Prepare the parser to take in fresh stream of data.
	// THIS METHOD MUST BE CALLED BEFORE DATA CAN BE PROCESSED BY THE PARSER!!
	// Calls TreeL in order to create the parsing rule tree if no tree already
	// exists.
	//##ModelId=3B6669EA00EF
	IMPORT_C virtual void ResetL();

	/** Checks if the input stream was completely processed
	@return ETrue if all of the data was processed, EFalse if the data didn't match to the parsing rules
	*/
	//##ModelId=3B6669EA00EE
	TBool Valid() const { return iStringComplete && (iString.Length() == 0); }

	/** Concatenates the rest of the input stream (which hasn't yet been processed)
	into a single string. The ownership of the string is given to the caller.
	@return String containing the remaining data to be parsed. OWNERSHIP PASSED TO CALLED. */
	//##ModelId=3B6669EA00ED
	HBufC* StringL() const { return iString.StringL(); }

	/** Gets a pointer to the rule node currently being processed.
	@return Rule node */
	//##ModelId=3B6669EA00E3
	CBNFNode* CurrentRule() { return iCurrentRule; }

	// Set reference to an attribute lookup table
	//##ModelId=3B6669EA00C5
	void SetAttributeLookupTable(CAttributeLookupTable& aAttributeLookupTable);

	// methods to allow the input stream to be marked so that the callbacks
	// can determine those parts which successfully matched

	/** Set a mark to the current position of the input stream. 

	The mark acts as a tag in the stream currently being processed.
	As we process further along the stream after adding the mark, we can perform
	a rollback to the most previously set mark and start processing again (e.g. OR rule
	works this way). The string fragments won't be consumed (deleted) until
	all the marks on a fragment (and fragments before that) are deleted. */
	//##ModelId=3B6669EA00BC
	void Mark() { iString.Mark(); }; // **Mark can leave**

	/** Get string between the "cursor position" and the latest mark on the stream.
	
	@return Pointer to the string from the previous mark on to the current position
			of processed string. OWNERSHIP OF THE STRING GIVEN TO THE CALLER. */
	//##ModelId=3B6669EA00BB
	HBufC* MarkedL() { return iString.MarkedL(); };

	/** Gets the marked string with a string added before the mached string.
	@see MarkedL()
	@return A string cosisting of aInitialText appended with the marked string.
	          OWNERSHIP OF THE CONSTRUCTED STRING IS GIVEN TO THE CALLER. */
	//##ModelId=3B6669EA009E
	HBufC* MarkedWithInitialTextL(const TDesC& aInitialText) { return iString.MarkedWithInitialTextL(aInitialText); };

	/** Removes the latest mark. All the marks are stored in a stack and this removes
	the topmost mark.*/
	//##ModelId=3B6669EA009D
	void DeleteMark() { iString.DeleteMark(); };

	// methods to determine it the used rule actually matched (typically used in post callbacks)
	/** Tests if the used rule matched.

	This is typically used in post-rule callbacks.

	@return True if the used rule matched; otherwise false
	*/
	//##ModelId=3B6669EA0094
	TBool RuleMatched() const { return iSubRuleMatched; };
	/** Tests if an Optional node sub-rule matched.

	@return True if the sub- rule matched; otherwise false
	*/
	//##ModelId=3B6669EA0093
	TBool OptionalMatched() const { return iOptionalMatched; };

	// Create new rule tree root node.
	// This method creates a new single instance of CBNFNode, which shall act as the root
	// node of the rule tree, which implements the BNF rules for parsing the input stream.
	// All the other rules are attached as attributes to this node.
	// The root node should have single child node, which should be a reference to the
	// "logical root" of the rule tree. This can be done be attaching the logical root
	// rule as a component to the root rule.
	//##ModelId=3B6669EA0089
	IMPORT_C CBNFNode* NewBNFL();

	// Add a new rule to a rule tree.
	//
	// Input:
	//	aRootRule - Pointer to the root bnf node (created with NewBNFL() ).
	//	aRuleName - Reference to a string identifying this rule. The string is used
	//				to make references to this rule from other rule's subtrees.
	//	aData	  - Pointer to a data string; used with EExact and ESelect type rules
	//              to match actual text strings.
	//	aPreRule  - Function pointer to a prerule function that gets called _BEFORE_
	//				we start processing this rule and its children (i.e. the rule subtree)
	//	aPostRule - Function pointer to a postrule function which is called _AFTER_
	//              we have processed this rule (i.e. when we return up from the subtree
	//              and this rule is finished).
	//
	// Return:
	//	CBNFNode& - Reference to the newly created rule node in the rule tree
	//
	//##ModelId=3B6669E90326
	IMPORT_C CBNFNode& NewRuleL(CBNFNode* aRootRule, 
					const TDesC& aRuleName, 
					TParserNodeTypes aRuleType, 
					HBufC* aData, 
					TRuleCallback* aPreRule, 
					TRuleCallback* aPostRule);

	// Overridden version of the NewRuleL. Takes reference to the data instead of owning it.
	//##ModelId=3B6669E903D1
	IMPORT_C CBNFNode& NewRuleL(CBNFNode* aRootRule, 
					const TDesC& aRuleName, 
					TParserNodeTypes aRuleType, 
					const TDesC& aData, 
					TRuleCallback* aPreRule, 
					TRuleCallback* aPostRule);

	// construct a new rule component not attached to a rule.
	//##ModelId=3B6669E9018C
	IMPORT_C CBNFNode* NewComponentL(TParserNodeTypes aRuleType, const TDesC& aData);
	//##ModelId=3B6669E901B4
	IMPORT_C CBNFNode* NewComponentL(TParserNodeTypes aRuleType, HBufC* aData = NULL, TRuleCallback* aPreRule = NULL, TRuleCallback* aPostRule = NULL);

	// create a reference component to the rule of the given name
	// which is not attached to any rule.
	//##ModelId=3B6669E90204
	IMPORT_C CBNFNode* NewComponentL(CBNFNode* aRootRule, const TDesC& aRuleName);

	// Methods to create a new subrule to the given parent rule.
	// These methods can be used to build the subtrees to the "main rules" attached to the root node.
	//
	// Input:
	//	aParentRule - The rule for which the new rule shall be added as a child
	//  aRuleType - Type of the new rule
	//  aData - Data for the rule; the string to match for an EExact rule, the selection character set for ESelect
	//
	//	aPreRule - Pre rule callback function pointer
	//  aPostRule - Post rule callback function pointer
	// Return:
	//	CBNFNode& - reference to the new rule
	//
	//##ModelId=3B6669E9022C
	IMPORT_C CBNFNode& NewComponentL(CBNFNode &aParentRule, TParserNodeTypes aRuleType, const TDesC& aData);
	//##ModelId=3B6669E90268
	IMPORT_C CBNFNode& NewComponentL(CBNFNode &aParentRule, TParserNodeTypes aRuleType, HBufC* aData = NULL, TRuleCallback* aPreRule = NULL, TRuleCallback* aPostRule = NULL);
	
	// Create a reference to another rule and attach this reference as a child of the given parent.
	// Creates a child node of type EReference for the parent. This reference node
	// hold the pointer to the rule we are refering to.
	// Using references we can link rules to each other and build complex rule trees
	// even though they don't physically form a complete tree.
	// Notice, that the rule we are refering to does not necessarily need to exist, yet!
	//
	// Input:
	//	aRootRule - The Root node to the rule tree (created with NewBNFL). This is needed to
	//				find the rule we are refering to with the string.
	//	aParentRule - The parent rule of the newly created reference 
	//	aRuleName - The "id string" of the rule we are refering to.
	//##ModelId=3B6669E902CC
	IMPORT_C CBNFNode& NewComponentL(CBNFNode* aRootRule, CBNFNode &aParentRule, const TDesC& aRuleName);
	
	// add additional attributes to components of rules (i.e. range values)
	//##ModelId=3B6669E900F6
	IMPORT_C void AddComponentAttributeL(CBNFNode& aRule, CBNFNodeAttributeType aAttribute, TInt aInt);
	
	// re-implementations of MDataProviderObserver methods
	//##ModelId=3B6669E900D8
	IMPORT_C virtual void ProcessDataL(HBufC8& aData);
	//##ModelId=3B6669E900AF
	IMPORT_C virtual void SetStatus(TInt aStatus = KErrNone);
	//##ModelId=3B6669E90069
	IMPORT_C virtual void SetDocumentTypeL(const TDesC&);
	//##ModelId=3B6669E90087
	IMPORT_C virtual void SetDocumentTypeL(const TDesC&, const TDesC&);
	//##ModelId=3B6669E90055
	IMPORT_C virtual void SetDataExpected(TInt);
	//##ModelId=3B6669E90041
	IMPORT_C virtual void SetBaseUriL(const TDesC* aBaseUri);
	//##ModelId=3B6669E90038
	IMPORT_C virtual void MDataProviderObserverReserved1();
	//##ModelId=3B6669E90037
	IMPORT_C virtual void MDataProviderObserverReserved2();

	// Tell the parser, that we all the data has been passed in.
	// This method attempts to parse what ever is left of the input stream if it wasn't
	// already finished.
	//##ModelId=3B6669E9002E
	IMPORT_C void CommitL();

	/** Get the current state of the parser. 
	@return Parser state */
	//##ModelId=3B6669E9002D
	TParseState State() const {return(iParsing);};

protected:
	IMPORT_C CBNFParser(CAttributeLookupTable& aLUT);

	// Each of the following functions is a handler method for a specific type of a rule
	// node. For example, ReferenceL handles reference nodes etc.
	// These methods are called by PerformRuleL.
	//
	// Input:
	//	aRule - reference to the rule being processed
	//	aMatched - reference to a CFragmentedString::TStringMatch variable, which holds
	//             the information if the string or character we previously were trying to
	//             match actually matched.
	// Return:
	//	TBool - We return ETrue if we have completed processing this node. If the processing
	//          still continues we return EFalse. For example, an EAnd rule would return
	//          ETrue if all of its chidren had matched or if a rule didn't match. In the first
	//          case the EAnd rule would have turned out to be true (aMatched = EMatched) since
	//          all of its children were true, but in the latter case we can stop processing the
	//          EAnd rule, since a subrule to the And didn't match and this means that the And
	//          expression can not be true. Either way, the processing of the And ends and we
	//          may return ETrue;
	//
	//##ModelId=3B6669E90005
    IMPORT_C virtual TBool ReferenceL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E803BB
    IMPORT_C virtual TBool ExactL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E80389
    IMPORT_C virtual TBool RangeL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E80343
    IMPORT_C virtual TBool SelectL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E80311
    IMPORT_C virtual TBool WithoutL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E802D5
    IMPORT_C virtual TBool AndL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E80299
    IMPORT_C virtual TBool OrL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E80271
    IMPORT_C virtual TBool OptionalL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);
	//##ModelId=3B6669E8023F
    IMPORT_C virtual TBool NMoreL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);

	// A method to add a callback to a rule
	//
	// Input:
	//	aRule - The rule to which the callback is to be added
	//	aCallbackID - Either CBNFNode::KPreRuleCallback() or CBNFNode::KPostRuleCallback()
	//                Defines the type of the callback function (i.e. is it to be called before
	//                or after the rule has been processed).
	//	aCallback - The callback function pointer
	//
	//##ModelId=3B6669E80203
    IMPORT_C virtual void AddRuleCallbackL(CBNFNode& aRule, const TDesC* aCallbackID, TRuleCallback* aCallback);
	//##ModelId=3B6669E801EF
    IMPORT_C virtual void ExecutePreRuleCallbackL(CBNFNode& aRule);
	//##ModelId=3B6669E801D1
    IMPORT_C virtual void ExecutePostRuleCallbackL(CBNFNode& aRule);

	// the method TreeL() should be reimplemented to generate a BNF rule tree and return
	// ownership of it. This is the rule tree which will be to parse the input stream.
	// See XmlPars.cpp or DTDMDL.cpp for example.
	//##ModelId=3B6669E801D0
	IMPORT_C virtual CBNFNode* TreeL();

	// methods which are invoked when the parser encounters a conditional
	// point in the BNF grammar (i.e. And/Or)
	//##ModelId=3B6669E801B2
    IMPORT_C virtual void StartConditional(TParserNodeTypes aRuleType);
	//##ModelId=3B6669E80180
	IMPORT_C virtual void EndConditional(TParserNodeTypes aRuleType, TBool aSuccess);

	// A callback function to insert a mark to the current position of the stream
	// being processed. Adding mark is a very common callback operation befor starting
	// to process a rule, hence the method is provided by the parser.
	//##ModelId=3B6669E8016C
	IMPORT_C static void MarkCallback(CBNFParser& aParser);

	// returns the LUT used by this parser.
	//##ModelId=3B6669E80163
	IMPORT_C CAttributeLookupTable& AttributeLUT() const;

	// method which does the actual iterative parsing
	//##ModelId=3B6669E80162
	IMPORT_C TBool ParseL();

	// A rule to handle a node in the rule tree. This method just calls the appropriate
	// handler method according to the rule type.
	//##ModelId=3B6669E8013A
    IMPORT_C virtual TBool PerformRuleL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched);

	//##ModelId=3B6669E8011C
	/** Sets the parser state.

	@param aState Parser state
	*/
	void SetState(TParseState aState) {iParsing=aState;};

protected:
	/** Storage object for all the attributes and identifiers in a tree */
	//##ModelId=3B6669E80108
	CAttributeLookupTable& iLUT;	

	/** An utility object which stores all the buffers passed into the parser
	and represents them as if they would form a single, continuous string.
	This class also performs the actual physical matching/selection of the strings
	and holds the marks set onto the string.*/
	//##ModelId=3B6669E800EA
	CFragmentedString iString;
	/** Flag indicating if the input stream has been completely processed. */ 
	//##ModelId=3B6669E800D6
	TBool iStringComplete; // more input stream has completed

	/** The BNF tree the parser is using to parse the input stream.*/
	//##ModelId=3B6669E800C2
	CBNFNode* iTree;        // the BNF tree we are using to parse the input stream

	/** A stack of rules from iTree which are waiting to be completed.
	The stack basically holds the path along the rule tree. */
	//##ModelId=3B6669E800AE
	CRuleStack iRuleStack;  
	/** The BNF rule that is currently being processed. */
	//##ModelId=3B6669E80090
	CBNFNode* iCurrentRule; // the BNF rule we are currently using

	// when returning to a rule in the rulestack this indicates 
	// if the child rule matched correctly
	/** Flag that indicates when returning to a rule in the rulestack if the child rule matched correctly. */
	//##ModelId=3B6669E8007C
	TBool iSubRuleMatched;
	/** Flag that indicates when returning to a rule in the rulestack if an optional rule matched correctly. */
	//##ModelId=3B6669E8006A
	TBool iOptionalMatched;

	/** The child rule we are returning from (if any). 
	If this is NULL we are new to this BNF rule.*/
	//##ModelId=3B6669E80054
	CBNFNode* iSubRule;     

	/** Parser state. */
	//##ModelId=3B6669E8004A
	TParseState iParsing;
	/** Input stream matched rule flag. */
	//##ModelId=3B6669E80038
    CFragmentedString::TStringMatch iMatched;

	// Storage pointers for strings identifying certain attributes on the rule nodes
	/** Stores attribute identifier for reference string attributes. */
	//##ModelId=3B6669E8002C
	const TDesC* iReferenceString;
	/** Stores attribute identifier for range start attributes. */
	//##ModelId=3B6669E8001A
	const TDesC* iRangeStart;
	/** Stores attribute identifier for range end attributes. */
	//##ModelId=3B6669E80010
	const TDesC* iRangeEnd;
	/** Stores attribute identifier for nmore minimum attributes. */
	//##ModelId=3B6669E80006
	const TDesC* iMoreMinimum;
	/** Stores attribute identifier for nmore count attributes. */
	//##ModelId=3B6669E703DA
	const TDesC* iMoreCount;
	/** Stores attribute identifier for nmore maximum attributes. */
	//##ModelId=3B6669E703D0
	const TDesC* iMoreMaximum;
	/** Stores attribute identifier for pre-rule callback attributes. */
	//##ModelId=3B6669E703C6
	const TDesC* iPreRuleCallback;
	/** Stores attribute identifier for post-rule callback attributes. */
	//##ModelId=3B6669E703BC
	const TDesC* iPostRuleCallback;
	};

#endif
