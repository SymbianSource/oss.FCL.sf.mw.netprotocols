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

#include <cbnfparser.h>

#include <cnodeleteattribute.h>
#include <attrlut.h>
#include <estatus.h>
#include <e32base.h>

#include "BNFPanicCodes.h"


/** Allocates and constructs a new BNF parser.

@return New parser
@param aLUT Attribute lookup table in which to store attributes for 
the rule tree
*/
EXPORT_C CBNFParser* CBNFParser::NewL(CAttributeLookupTable& aLUT)
	{
	CBNFParser* model = new (ELeave) CBNFParser(aLUT);
	return (model);
	}

/** Constructor.

@param aLUT Attribute lookup table
*/
EXPORT_C CBNFParser::CBNFParser(CAttributeLookupTable& aLUT)
    : iLUT(aLUT)
	{
// store pointers to these to avoid continual unnecessary calls to the CBNFNode DLL
	iReferenceString=CBNFNode::KReference();
	iRangeStart=CBNFNode::KRangeStart();
	iRangeEnd=CBNFNode::KRangeEnd();
	iMoreMinimum=CBNFNode::KNMoreMinimum();
	iMoreCount=CBNFNode::KNMoreCount();
	iMoreMaximum=CBNFNode::KNMoreMaximum();
	iPreRuleCallback=CBNFNode::KPreRuleCallback();
	iPostRuleCallback=CBNFNode::KPostRuleCallback();
	}

/** Destructor.
*/
EXPORT_C CBNFParser::~CBNFParser()
	{
	delete iTree;
	}

/**  Reset the parser to a state where it can accept and parse new input.

If no BNF tree yet exists the virtual method TreeL() is called to obtain the BNF tree for this parser.
Any existing state of parsing and input data is destroyed. */
EXPORT_C void CBNFParser::ResetL()
	{
	if (iTree == NULL)
		iTree = TreeL();

	iRuleStack.Clear();
	iRuleStack.PushL(iTree);

	iSubRuleMatched = EFalse;
	iSubRule = NULL;
	iStringComplete = EFalse;
	iString.Reset();
	}

/** Gets the attribute look-up table used by this parser.

@return Attribute look-up table
*/
EXPORT_C CAttributeLookupTable& CBNFParser::AttributeLUT() const
    {
	return iLUT;
	}

/** Processes a reference rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::ReferenceL(CBNFNode& aRule, CFragmentedString::TStringMatch& /*aMatched*/)
// Handle a reference node.
// The pointer to the refered rule is stored as an attribute to a node which
// is stored as an attribute to the reference node. Nice and simple, isn't it? :-)
    {
    // new to this Reference, or returning from a sub match?
    if (iSubRule)
		// No operation is required when returning from the refered rule - continue up.
        return ETrue;
    else
        {
        CNoDeleteAttributeT<CBNFNode*>* attributeNode = REINTERPRET_CAST(CNoDeleteAttributeT<CBNFNode*>*, aRule.Attribute(iReferenceString));
//        iRuleStack.Pop(); //Optimisation not possible as other bnf parts use this 'rule' node to find the next sibling (i.e. And, Or etc)
        iRuleStack.PushL(attributeNode->Attribute());
        }
    return EFalse;
    }

/** Processes an EExact rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::ExactL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Handle an EExact rule
// Attempts to match a string with the stream.
    {
    if ((aMatched=iString.Match(REINTERPRET_CAST(HBufC*, aRule.Data())->Des()))==CFragmentedString::EMatch)
        iString.ConsumeMatched();
    return ETrue;
    }

/** Processes an ERange rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::RangeL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Handle ERange rule
// Checks if the next character in the stream fits into the chracter range defined by the rule
    {
    CNoDeleteAttributeT<TInt>* rangeStartAttribute = REINTERPRET_CAST(CNoDeleteAttributeT<TInt>*, aRule.Attribute(iRangeStart));
    TInt rangeStart;
    if (rangeStartAttribute)
        rangeStart = rangeStartAttribute->Attribute();
    else
        rangeStart = 0;
    
    CNoDeleteAttributeT<TInt>* rangeEndAttribute = REINTERPRET_CAST(CNoDeleteAttributeT<TInt>*, aRule.Attribute(iRangeEnd));
    TInt rangeEnd;
    if (rangeEndAttribute)
        rangeEnd = rangeEndAttribute->Attribute();
    else
        rangeEnd = 0;
    
    if ((aMatched = iString.MatchRange(rangeStart, rangeEnd)) == CFragmentedString::EMatch)
        iString.ConsumeMatched();
    return ETrue;
    }


/** Processes an ESelect rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::SelectL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Handle Select rule
// Check if the next character in the stream matches to any of the chracters included
// into the set defined by the select rule.
// The selection can also be performed as inverted selection (by using "^" as the first char)
// in which case all the other characters but the ones defined in the set match correctly to the stream.
    {
    if ((*aRule.Data())[0] == '^')
        aMatched = iString.MatchNotSelect((*REINTERPRET_CAST(HBufC*, aRule.Data())).Mid(1));
    else
        aMatched = iString.MatchSelect(*REINTERPRET_CAST(HBufC*, aRule.Data()));

//!	if (aMatched == CFragmentedString::EMatch)
    iString.ConsumeMatched();
    return ETrue;
    }

/** Processes an EWithout rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::WithoutL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Handle EWithout rule
// An example Without rule: [A-Z] - B
// Here we are trying to match all characters from A-Z except character B.
// The rule is performed here by first executing the latter part of the rule (in the example
// by trying to match B). If the latter rule matches, then the stream contains the chracter
// string we didn't want it to include and hence the Without rule is false. However, if the
// latter rule doesn't match we proceed to matching the first part of the rule and if it
// does match the whole Without rule is true.
// The Without rule node has two children of which the first one (child on index 0)
// is the rule we are trying to match and the second child is the "except" part of the rule.
    {
    TBool performedMatch = EFalse;
    // new to this Without, or returning from a sub match?
    CBNFNode* newRule = NULL;
    if (iSubRule)
        {
        if (iSubRule == aRule.Child(1))
            {
            iString.ResetToMark();
            iString.DeleteMark();
            if (!iSubRuleMatched)
                newRule = REINTERPRET_CAST(CBNFNode*, aRule.Child(0));
            else
                aMatched = CFragmentedString::ENoMatch;
            }
        }
    else
        {
        iString.Mark(); // Mark can leave
        StartConditional((enum TParserNodeTypes)aRule.Type());
        newRule = REINTERPRET_CAST(CBNFNode*, aRule.Child(1));
        }
    if (newRule)
        {
        iRuleStack.PushL(newRule);
        iSubRule = NULL; // going down
        }
    else
        {
        EndConditional((enum TParserNodeTypes)aRule.Type(), iSubRuleMatched);
        performedMatch = ETrue;
        }
    return performedMatch;
    }

/** Processes an EAnd rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::AndL(CBNFNode& aRule, CFragmentedString::TStringMatch& /*aMatched*/)
// Handle And rule
// An EAnd rule is true if all of its children are true. Processing the child nodes of
// the And rule continues until all the children has been processed of one of them doesn't
// match. Notice, that the order of the child rules has signifigance, since we are
// matching a chracter stream and the first child is matched first, followed by the second etc.
    {
    TBool performedMatch = EFalse;
    // new to this And, or returning from a sub match?
    CBNFNode* newRule = NULL;
    if (iSubRule)
        {
        if (iSubRuleMatched)
            {
            newRule = REINTERPRET_CAST(CBNFNode*, iSubRule->NextSibling());
            if (!newRule)
                performedMatch = ETrue;
            }
        else
            performedMatch = ETrue;
        }
    else
        {
        newRule = REINTERPRET_CAST(CBNFNode*, aRule.Child(0));
        StartConditional((enum TParserNodeTypes)aRule.Type());
        }
    if (newRule)
        {
        iRuleStack.PushL(newRule);
        iSubRule = NULL; // going down
        }
    else
		{
        EndConditional((enum TParserNodeTypes)aRule.Type(), iSubRuleMatched);
//!		performedMatch = ETrue;
		}
    return performedMatch;
    }


/** Processes an EOr rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::OrL(CBNFNode& aRule, CFragmentedString::TStringMatch& /*aMatched*/)
// Handle Or rule
// We attempt to match the child rules of the Or node and stop when we have the first match.
// With the Or type rule each of the child rules are matched agaist the same bit of input
// stream. Before we start processing the first child rule we set a mark to the input stream
// and if a child rule doesn't match, we rollback the input stream to the mark and try
// to match the next child.
    {
    TBool performedMatch = EFalse;
    CBNFNode* newRule = NULL;
    // new to this Or, or returning from a sub match?
    if (iSubRule)
        {
        if (!iSubRuleMatched)
            {
            newRule = REINTERPRET_CAST(CBNFNode*, iSubRule->NextSibling());
            }
        EndConditional((enum TParserNodeTypes)aRule.Type(), iSubRuleMatched);
        }
    else
        {
        newRule = REINTERPRET_CAST(CBNFNode*, aRule.Child(0));
        iString.Mark(); // Mark can leave
        }
    if (newRule)
        {
        iRuleStack.PushL(newRule);
        iSubRule = NULL; // going down
        iString.ResetToMark();
        StartConditional((enum TParserNodeTypes)aRule.Type());
        }
    else
        {
        performedMatch = ETrue;
        if (!iSubRuleMatched)
            iString.ResetToMark();
        iString.DeleteMark();
        }
    return performedMatch;
    }

EXPORT_C TBool CBNFParser::OptionalL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Handle Optional rule
// Process the subrule attached to the optional rule.
// An optional rule is always successfully processed regardless to success or failure of matching
// the subrule. This is because of the nature of the optional rule - the subrule here either may
// or may not be there.
    {
    TBool performedMatch = EFalse;
    // new to this Optional, or returning from a sub match?
    if (iSubRule)
        {
        performedMatch = ETrue;
        aMatched = CFragmentedString::EMatch;
        iOptionalMatched = iSubRuleMatched;
        if (!iSubRuleMatched)
            iString.ResetToMark();
        iString.DeleteMark();
        }
    else
        {
        iString.Mark(); // Mark can leave
        iRuleStack.PushL(REINTERPRET_CAST(CBNFNode*, aRule.Child(0)));
        iSubRule = NULL;
        }
    return performedMatch;
    }    



/** Processes an ENMore rule node.

It is called by PerformRuleL().

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule The rule node being processed
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C TBool CBNFParser::NMoreL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Handle NMore rule
// NMore rule implements the "multiplier" elements of BNF (* and +).
// We attempt to match the subrule and if it matches we try again until it doesn't match
// or we run into the upperlimit set to this rule.
// An NMore rule may contain lower and upper limits for the number of times the matching
// can or needs to be done. These are included as attributes to the NMore node. The NMore
// node also holds a counter attribute, which holds the amount of times this rule has been
// matched.
    {
    TBool performedMatch = EFalse;
    // new to this NMore, or returning from a sub match?
    if (iSubRule)
        {
        CNoDeleteAttributeT<TInt>* countAttribute = REINTERPRET_CAST(CNoDeleteAttributeT<TInt>*, aRule.Attribute(iMoreCount));
        CNoDeleteAttributeT<TInt>* maxCountAttribute = REINTERPRET_CAST(CNoDeleteAttributeT<TInt>*, aRule.Attribute(iMoreMaximum));
        
        if (iSubRuleMatched)
            countAttribute->SetAttribute(countAttribute->Attribute()+1);
        else
            {
            performedMatch = ETrue;
            CNoDeleteAttributeT<TInt>* minCountAttribute = REINTERPRET_CAST(CNoDeleteAttributeT<TInt>*, aRule.Attribute(iMoreMinimum));
            if ((minCountAttribute == NULL) || (countAttribute->Attribute() >= minCountAttribute->Attribute()))
                aMatched = CFragmentedString::EMatch;
            iString.ResetToMark();
            }
        
        if ((maxCountAttribute) && (countAttribute->Attribute() >= maxCountAttribute->Attribute()))
            performedMatch = ETrue;
        
        iString.DeleteMark();
        }
    else
        {
        aRule.DeleteAttribute(iMoreCount);
        CNoDeleteAttributeT<TInt>* countAttribute = new (ELeave) CNoDeleteAttributeT<TInt>(0);
        CleanupStack::PushL(countAttribute);
        aRule.AddAttributeL(iMoreCount, countAttribute);
        CleanupStack::Pop();
        }
    if (!performedMatch)
        {
        iRuleStack.PushL(REINTERPRET_CAST(CBNFNode*, aRule.Child(0)));
        iSubRule = NULL;
        iString.Mark(); // Mark can leave
        }
    return performedMatch;
    }

/** Executes a pre-rule callback function.

@param aRule Node specifying the callback
*/
EXPORT_C
void
CBNFParser::ExecutePreRuleCallbackL(CBNFNode& aRule)
    {
    CNoDeleteAttributeT<TRuleCallback*>* preRuleAttribute;
    if ((preRuleAttribute= REINTERPRET_CAST(CNoDeleteAttributeT<TRuleCallback*>*, aRule.Attribute(iPreRuleCallback)))!=NULL)
        (*preRuleAttribute->Attribute())(*this);
    }

/** Executes a post-rule callback function.

@param aRule Node specifying the callback
*/
EXPORT_C
void
CBNFParser::ExecutePostRuleCallbackL(CBNFNode& aRule)
    {
    CNoDeleteAttributeT<TRuleCallback*>* postRuleAttribute;
    if ((postRuleAttribute = REINTERPRET_CAST(CNoDeleteAttributeT<TRuleCallback*>*, aRule.Attribute(iPostRuleCallback)))!=NULL)
        (*postRuleAttribute->Attribute())(*this);
    }

/** Handles a node in the rule tree.

It calls the appropriate handler method for the rule type.

@return ETrue if the function completed processing this node, otherwise EFalse
@param aRule Rule node
@param aMatched On return, flag indicating if input stream matched the rule
*/
EXPORT_C
TBool
CBNFParser::PerformRuleL(CBNFNode& aRule, CFragmentedString::TStringMatch& aMatched)
// Dispatches a rule node of given type to the appropriate handler
	{
    TBool performedMatch;
    switch (aRule.Type())
        {
    case EReference:
        performedMatch = ReferenceL(aRule, aMatched);
        break;
    case EExact:
        performedMatch = ExactL(aRule, aMatched);
        break;
    case ERange:
        performedMatch = RangeL(aRule, aMatched);
        break;
    case ESelect:
        performedMatch = SelectL(aRule, aMatched);
        break;
    case EWithout:
        performedMatch = WithoutL(aRule, aMatched);
        break;
    case ERoot:
    case EAnd:
        performedMatch = AndL(aRule, aMatched);
        break;
    case EOr:
        performedMatch = OrL(aRule, aMatched);
        break;
    case EOptional:
        performedMatch = OptionalL(aRule, aMatched);
        break;
    case ENMore:
        performedMatch = NMoreL(aRule, aMatched);
        break;
    case EIncomplete:
    default:
        __ASSERT_DEBUG(EFalse, User::Panic(_L("CBNFPARSE"), 1));
        // Oh dear an incomplete node in the rule tree. FAIL NOW!!!
        performedMatch = ETrue;
        aMatched = CFragmentedString::ENoMatch;
        break;
        }
    return performedMatch;
    }

EXPORT_C TBool CBNFParser::ParseL()
// Performs the actual parsing.
// Consumes as much of the input data as it can using the iTree BNF tree.
// Whilst parsing the appropriate callbacks are invoked on the parsed rules allowing actions to be performed.
// Should the input data be fully consumed and the parser has not been notified that there is no more input data
// this method returns leaving the parser in a state waiting for more input.
	{
	iParsing = EActive;
	while ((State() == EActive) && (!iRuleStack.IsEmpty()))
		{
		// find next rule
		CBNFNode *currentRule = iRuleStack.Head();

		// are we going up or down the tree?
 		if (iSubRule == NULL)
            ExecutePreRuleCallbackL(*currentRule);

        if (PerformRuleL(*currentRule, iMatched))
			{
			switch (iMatched)
				{
            case CFragmentedString::EMatch:
                // we matched take ourselves off the stack
                iRuleStack.Pop();
                iSubRuleMatched = ETrue;
                iSubRule = currentRule;
                break;
            case CFragmentedString::EInsufficientData:
                // we ran out of data doing the match, should we try again later or is it a no match
                if (!iStringComplete)
                    {
                    iParsing= EStopped;
                    break;
                    }
            case CFragmentedString::ENoMatch:
                // we didn't match take ourselves off the stack
                iRuleStack.Pop();
                iSubRuleMatched = EFalse;
                iSubRule = currentRule;
                break;
            default:
                // naughty, naughty.
                break;
				}
            if( iMatched != CFragmentedString::EInsufficientData || iStringComplete)
				ExecutePostRuleCallbackL(*currentRule);
			}
		// have we finished all the rules!?!?!
		if (iRuleStack.IsEmpty())
			iParsing= EStopped;
		}
	return (iSubRuleMatched);
	}


//
// assist BNF tree building
/** Creates a new rule tree root node.

It creates a new single instance of CBNFNode as the root node of the rule tree. 
All the top-level rules are attached as attributes to this node. The root node 
should have single child node, which should be a reference to the logical root 
of the rule tree. This can be done be attaching the logical root rule as a component 
to the root rule.

@return New rule tree root node
*/
EXPORT_C CBNFNode* CBNFParser::NewBNFL()
// Generate a new ROOT node for a BNF tree.
	{
	return CBNFNode::NewL(ERoot);
	}

/** Creates a new rule node and adds it to the root of the rule tree. 

This overload takes ownership of the node data.

@return The newly created rule node in the rule tree
@param aRootRule Pointer to the root BNF node, created with NewBNFL()
@param aRuleName Reference to a string identifying this rule. The string is used to make references to this rule from other rule's subtrees.
@param aRuleType Rule type
@param aData Rule data pointer. This is used with EExact and ESelect type rules to match actual text strings.
@param aPreRule Function pointer to a pre-rule function that gets called before the parser starts processing this rule and its children (i.e. the rule subtree).
@param aPostRule Function pointer to a post-rule function that gets called after the parser has processed this rule and its subtree.
*/
EXPORT_C CBNFNode&  CBNFParser::NewRuleL(CBNFNode* aRootRule, const TDesC& aRuleName, TParserNodeTypes aRuleType, HBufC* aData, TRuleCallback* aPreRule, TRuleCallback* aPostRule)
// Add a new rule to the BNF tree, the new rule is given the name aRuleName (so it can be referred by another rule).
// The parameter aData is used by the RuleTypes: Exact & Select.
	{
	// taken ownership of aData
	CleanupStack::PushL(aData);
	const HBufC* id = iLUT.Des2IDL(aRuleName);

	CBNFNode* terminal = REINTERPRET_CAST(CBNFNode*, aRootRule->Attribute(id));

	if (terminal)
		terminal->SetType(aRuleType);
	else
		{
		terminal = CBNFNode::NewL(aRuleType);
        CleanupStack::PushL(terminal);
		aRootRule->AddAttributeL(id, terminal);
        CleanupStack::Pop();
		}

	if (aData)
		terminal->SetDataL(aData);
	CleanupStack::Pop();

    AddRuleCallbackL(*terminal, iPreRuleCallback, aPreRule);
    AddRuleCallbackL(*terminal, iPostRuleCallback, aPostRule);

	return *terminal;
	}

/** Creates a new rule node and adds it to the root of the rule tree. 

This overload takes a reference to the node data instead of owning it.

@return The newly created rule node in the rule tree
@param aRootRule Pointer to the root BNF node, created with NewBNFL()
@param aRuleName Reference to a string identifying this rule. The string is used to make references to this rule from other rule's subtrees.
@param aRuleType Rule type
@param aData Rule data pointer. This is used with EExact and ESelect type rules to match actual text strings.
@param aPreRule Function pointer to a pre-rule function that gets called before the parser starts processing this rule and its children (i.e. the rule subtree).
@param aPostRule Function pointer to a post-rule function that gets called after the parser has processed this rule and its subtree.
*/
EXPORT_C CBNFNode& CBNFParser::NewRuleL(CBNFNode* aRootRule, const TDesC& aRuleName, TParserNodeTypes aRuleType, const TDesC& aData, TRuleCallback* aPreRule, TRuleCallback* aPostRule)
	{
    return NewRuleL(aRootRule, aRuleName, aRuleType, aData.AllocL(), aPreRule, aPostRule);
	}

/** Adds a callback to a rule.

@param aRule The rule to which the callback is to be added
@param aCallbackID Callback type: either CBNFNode::KPreRuleCallback() or CBNFNode::KPostRuleCallback()
@param aCallback Callback function
*/
EXPORT_C void CBNFParser::AddRuleCallbackL(CBNFNode& aRule, const TDesC* aCallbackID, TRuleCallback* aCallback)
    {
    if (aCallback)
        {
        CNoDeleteAttributeT<TRuleCallback*>* ruleAttribute = new (ELeave) CNoDeleteAttributeT<TRuleCallback*>(aCallback);
        CleanupStack::PushL(ruleAttribute);
        aRule.AddAttributeL(aCallbackID, ruleAttribute);
        CleanupStack::Pop();
        }
    }

/** Creates a new reference rule node.

@return The new reference rule node
@param aRootRule Root node
@param aRuleName Rule name
*/
EXPORT_C CBNFNode* CBNFParser::NewComponentL(CBNFNode* aRootRule, const TDesC& aRuleName)
	{
	CBNFNode *terminalNode = (CBNFNode*)aRootRule->Attribute(iLUT.Des2IDL(aRuleName));
	if (terminalNode == NULL)
		terminalNode = &NewRuleL(aRootRule, aRuleName, EIncomplete, NULL, NULL, NULL);

	CBNFNode* rule = CBNFNode::NewL(EReference);
	CleanupStack::PushL(rule);
	CNoDeleteAttributeT<CBNFNode*>* attributeNode = new (ELeave) CNoDeleteAttributeT<CBNFNode*>(terminalNode);
    CleanupStack::PushL(attributeNode);
	rule->AddAttributeL(iReferenceString, attributeNode);
	CleanupStack::Pop(2);

	return rule;
	}


/** Creates a new rule node, but does not add it to the tree.

This overload sets no rule callbacks.

@return The new rule node
@param aRuleType Rule type
@param aData Rule data reference. This is used with EExact and ESelect type rules to match actual text strings.
*/
EXPORT_C CBNFNode* CBNFParser::NewComponentL(TParserNodeTypes aRuleType, const TDesC& aData)
// Adds a new rule component to the given ParentRule.
	{
    return NewComponentL(aRuleType, aData.AllocL());
	}

/** Creates a new rule node, but does not add it to the tree.

This overload allows rule callbacks to be set.

@return The new rule node
@param aRuleType Rule type
@param aData Rule data pointer. This is used with EExact and ESelect type rules to match actual text strings.
@param aPreRule Function pointer to a pre-rule function that gets called before the parser starts processing this rule and its subtree.
@param aPostRule The new rule node
*/
EXPORT_C CBNFNode* CBNFParser::NewComponentL(TParserNodeTypes aRuleType, HBufC* aData/*=NULL*/, TRuleCallback* aPreRule/*=NULL*/, TRuleCallback* aPostRule/*=NULL*/)
	{
	// taken ownership of aData
	CleanupStack::PushL(aData);
	CBNFNode* terminal = CBNFNode::NewL(aRuleType);
	CleanupStack::PushL(terminal);
	if (aData)
		terminal->SetDataL(aData);
	// correct cleanupstack
	CleanupStack::Pop(2);
	CleanupStack::PushL(terminal);

    AddRuleCallbackL(*terminal, iPreRuleCallback, aPreRule);
    AddRuleCallbackL(*terminal, iPostRuleCallback, aPostRule);

	CleanupStack::Pop(); //terminal
	return terminal;
	}


/** Creates a new sub-rule, and makes it a child of a specified parent rule.

This overload sets no rule callbacks.

@return The new sub-rule node
@param aParentRule The rule to the new sub-rule shall be added as a child
@param aRuleType Rule type
@param aData Rule data reference. This is used with EExact and ESelect type rules to match actual text strings.
*/
EXPORT_C CBNFNode&  CBNFParser::NewComponentL(CBNFNode &aParentRule, TParserNodeTypes aRuleType, const TDesC& aData)
// Adds a new rule component to the given ParentRule.
	{
    return NewComponentL(aParentRule, aRuleType, aData.AllocL());
	}

/** Creates a new sub-rule, and makes it a child of a specified parent rule.

This overload sets rule callbacks.

@return The new sub-rule node
@param aParentRule The rule to the new sub-rule shall be added as a child
@param aRuleType Rule type
@param aData Rule data pointer. This is used with EExact and ESelect type rules to match actual text strings.
@param aPreRule Function pointer to a pre-rule function that gets called before the parser starts processing this rule and its subtree.
@param aPostRule Function pointer to a post-rule function that gets called after the parser has processed this rule and its subtree.
*/
EXPORT_C CBNFNode&  CBNFParser::NewComponentL(CBNFNode &aParentRule, TParserNodeTypes aRuleType, HBufC* aData, TRuleCallback* aPreRule, TRuleCallback* aPostRule)
	{
	// take ownership of aData
    CleanupStack::PushL(aData);
	CBNFNode& terminal = REINTERPRET_CAST(CBNFNode&, aParentRule.AppendNodeL(aRuleType));
    AddRuleCallbackL(terminal, iPreRuleCallback, aPreRule);
    AddRuleCallbackL(terminal, iPostRuleCallback, aPostRule);

	if (aData)
		terminal.SetDataL(aData);
    CleanupStack::Pop();

	return terminal;
	}


/** Creates a new reference rule node, and adds it as a child of the specified parent.

Note that the function succeeds even if the target rule aRuleName does not yet exist.

@return The new reference rule node
@param aRootRule Root node: the function needs this to find the target rule identified by aRuleName
@param aParentRule Parent rule to which to attach the new rule as a child
@param aRuleName The name of the rule that the new node refers to
*/
EXPORT_C CBNFNode&  CBNFParser::NewComponentL(CBNFNode* aRootRule, CBNFNode &aParentRule, const TDesC& aRuleName)
    {
	CBNFNode *terminalNode = (CBNFNode*)aRootRule->Attribute(iLUT.Des2IDL(aRuleName));
	if (terminalNode == NULL)
		terminalNode = &NewRuleL(aRootRule, aRuleName, EIncomplete, NULL, NULL, NULL);

	CBNFNode& rule = REINTERPRET_CAST(CBNFNode&, aParentRule.AppendNodeL(EReference));
	CNoDeleteAttributeT<CBNFNode*>* attributeNode = new (ELeave) CNoDeleteAttributeT<CBNFNode*>(terminalNode);
    CleanupStack::PushL(attributeNode);
	rule.AddAttributeL(iReferenceString, attributeNode);
    CleanupStack::Pop();

	return rule;
	}

/** Adds an additional attribute to an existing rule node.

For example, this is used with range rules, which specify the range boundaries using start and end attributes.

@param aRule Rule node on which to set the attribute
@param aAttribute Attribute type
@param aInt Attribute value
*/
EXPORT_C void CBNFParser::AddComponentAttributeL(CBNFNode& aRule, CBNFNodeAttributeType aAttribute, TInt aInt)
// Adds a restriction to a rule component, this is used for instance with Range restrictions (Start/End)
	{
	CNoDeleteAttributeT<TInt>* aTIntAttribute = new (ELeave) CNoDeleteAttributeT<TInt>(aInt);
    CleanupStack::PushL(aTIntAttribute);
	aRule.AddAttributeL(aAttribute, aTIntAttribute);
    CleanupStack::Pop();
	}

/** Inserts a mark to the current position of the stream being processed.

Adding a mark is a very common callback operation before starting to process a rule, so the method is provided by the parser.

@param aParser Parser processing the stream
*/
EXPORT_C void CBNFParser::MarkCallback(CBNFParser& aParser)
// Provide a very common preaction callback to mark the input stream before the next rule attempts to parse it.
	{// static
	aParser.Mark(); // Mark can leave
	}


//
// Data Provider access
/** Called by the data provider to add data for the parser to process.

This implements MDataProviderObserver::ProcessDataL().

@param aData The data to process
*/
EXPORT_C void CBNFParser::ProcessDataL(HBufC8& aData)
// Add new data as the input to the parser, ownership is taken of the aData.
	{
	HBufC* newData = HBufC::NewL(aData.Length());
	newData->Des().Copy(aData.Des());
	
	iString.AddStringL(newData);

	ParseL();
	}

/** Called by the data provider to report its status to its observer.

This implements MDataProviderObserver::SetStatus().

@param aStatusCode Status code
*/
EXPORT_C void CBNFParser::SetStatus(TInt aStatusCode)
// Notify the parser that there is no more (new) input data to be added/parsed.
	{
	if (aStatusCode == EPluginComplete)
		{
		// No more data will be pushed at us
		// so do what we need to finish and set our
		// status
		iStringComplete = ETrue;
		}
	}

EXPORT_C void CBNFParser::SetDocumentTypeL(const TDesC& /*aDocType*/)
    {
    }

EXPORT_C void CBNFParser::SetDocumentTypeL(const TDesC&, const TDesC&)
    {
    }

EXPORT_C void CBNFParser::SetDataExpected(TInt)
    {
    }

EXPORT_C void CBNFParser::SetBaseUriL(const TDesC* /*aBaseUri*/)
	{
	}

/** Notifies the parser that all the data has been passed in.

It causes the parser to parse any of the input stream not already parsed.
*/
EXPORT_C void CBNFParser::CommitL()
	{
	if (iStringComplete)
	    ParseL();
	}

EXPORT_C  CBNFNode* CBNFParser::TreeL()
    {
    return NULL;
    }

EXPORT_C void CBNFParser::StartConditional(TParserNodeTypes /*aRuleType*/)
    {
    }

EXPORT_C void CBNFParser::EndConditional(TParserNodeTypes /*aRuleType*/, TBool /*aSuccess*/)
    {
    }


// Provide an implementation for the interface class
EXPORT_C void CBNFParser::MDataProviderObserverReserved1() 
	{
	User::Panic(_L("Reserved"),KErrNotSupported);
	}

EXPORT_C void CBNFParser::MDataProviderObserverReserved2()
	{
	User::Panic(_L("Reserved"),KErrNotSupported);
	}
