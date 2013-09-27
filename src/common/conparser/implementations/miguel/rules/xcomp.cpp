    //"VP < (S=target !$- (NN < order) < (VP < TO))",    // used to have !> (VP < (VB|AUX < be))
    bool buildXComp1() {
    	if (node.constituent==PENN_CON_VP){
    		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
    		while(childsVp!=0){
    			const CStateNode* sTarg=childsVp->node;
    			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))){
    				//A $- B: A is the immediate right sister of B
    				bool sisterCondition=false;
    				if (childsVp->previous!=0){
    					const CStateNode* leftSisterS=childsVp->previous->node;
    					if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
    						CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
    						while(childsNN!=0){
    							const CStateNode* orderChild=childsNN->node;
    							if ((*words)[orderChild->lexical_head].word==g_word_order){
    								sisterCondition=true;
    							}
    							childsNN=childsNN->next;
    						}
    					}
    				}
    				if (sisterCondition){
    					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
    					while(childsS!=0){
    						const CStateNode* vpChild=childsS->node;
    						if (vpChild->constituent==PENN_CON_VP){
    							CStateNodeList* childsVps=vpChild->m_umbinarizedSubNodes;
    							while(childsVps!=0){
    								if ((*words)[childsVps->node->lexical_head].tag.code()==PENN_TAG_TO) {
    									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
    									if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
    										addLinked(&node,sTarg);
    									    return true;
    									}
    								}
    								childsVps=childsVps->next;
    							}
    						}
    						childsS=childsS->next;
    					}
    				}
    			}
    			childsVp=childsVp->next;
    		}
    	}
    	return false;

    }

    //"ADJP < (S=target <, (VP <, TO))",
       bool buildXComp2() {
       	if (node.constituent==PENN_CON_ADJP){
       		CStateNodeList* childsADJP=node.m_umbinarizedSubNodes;
       		while(childsADJP!=0){
       			const CStateNode* sTarg=childsADJP->node;
       			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))){
       				//A <, B 	B is the first child of A
       				CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
       				if (childsS!=0){//only first child
       					const CStateNode* vpChildS=childsS->node;
       					if (vpChildS->constituent==PENN_CON_VP){
       						CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
       						if (childsVp!=0){//only first child
       							if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_TO)){
       								CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
       								if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
       									addLinked(&node,sTarg);
       									return true;
       								}
       							}
       						}
       					}
       				}

       			}
       			childsADJP=childsADJP->next;
       		}
       	}
       	return false;

       }

//"VP < (S=target !$- (NN < order) < (NP $+ NP|ADJP))",
bool buildXComp3() {
	if (node.constituent==PENN_CON_VP){
		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
		while(childsVp!=0){
			const CStateNode* sTarg=childsVp->node;
			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))){
				//A $- B: A is the immediate right sister of B
				bool sisterCondition=false;
				if (childsVp->previous!=0){
					const CStateNode* leftSisterS=childsVp->previous->node;
					if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
						CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
						while(childsNN!=0){
							const CStateNode* orderChild=childsNN->node;
							if ((*words)[orderChild->lexical_head].word==g_word_order){
								sisterCondition=true;
							}
							childsNN=childsNN->next;
						}
					}
				}
				if (sisterCondition){
					CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
					while(childsS!=0){
						const CStateNode* npChildS=childsS->node;
						if (npChildS->constituent==PENN_CON_NP){
							//A $+ B 	A is the immediate left sister of B
							if (childsS->next!=0){
								if (childsS->next->node->constituent==PENN_CON_NP ||childsS->next->node->constituent==PENN_CON_ADJP){
									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
									if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
										addLinked(&node,sTarg);
									    return true;
									}
								}
							}
						}
						childsS=childsS->next;
					}
				}
			}
			childsVp=childsVp->next;
		}
	}
	return false;

}

//"VP < (/^(?:VB|AUX)/ $+ (VP=target < VB < NP))",
   bool buildXComp4() {
   	if (node.constituent==PENN_CON_VP){
   		bool firstCondition=false;
   		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
   		while(childsVp!=0){
   			if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB)){
   				firstCondition=true;
   			}
   			childsVp=childsVp->next;
   		}
   		if (firstCondition){
   			//A $+ B 	A is the immediate left sister of B
   			if (childsVp->next!=0){
   				const CStateNode* vpTarg=childsVp->next->node;
   				if ((vpTarg->constituent==PENN_CON_VP) && (!isLinked(&node,vpTarg))){
   					CStateNodeList* childsVpTarg=vpTarg->m_umbinarizedSubNodes;
   					while(childsVpTarg!=0){
   						const CStateNode* vbChildVp=childsVpTarg->node;
   						if (((*words)[vbChildVp->lexical_head].tag.code()==PENN_TAG_VERB)){
   							CStateNodeList* childsVb=vbChildVp->m_umbinarizedSubNodes;
   							while(childsVb!=0){
   								if (childsVb->node->constituent==PENN_CON_NP){
   									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
   									if (buildStanfordLink(label, vpTarg->lexical_head, node.lexical_head)) {
   										addLinked(&node,vpTarg);
   										return true;
   									}
   								}
   								childsVb=childsVb->next;
   							}
   						}
   						childsVpTarg=childsVpTarg->next;
   					}
   				}

   			}
   		}
   	}
   	return false;

   }


//"VP < (SBAR=target < (S !$- (NN < order) < (VP < TO))) !> (VP < (VB|AUX < be)) ",
bool buildXComp5() {}


//"VP < (S=target !$- (NN < order) <: NP) > VP",
//the second vp is the head. Not the first one, because the first one is the one located to the right of >

bool buildXComp6() {
	if (node.constituent==PENN_CON_VP){
		CStateNodeList* childsfstVp=node.m_umbinarizedSubNodes;
		while(childsfstVp!=0){
			const CStateNode* vpNode=childsfstVp->node;
			if (vpNode->constituent==PENN_CON_VP){
				CStateNodeList* childsVp=vpNode->m_umbinarizedSubNodes;
				while(childsVp!=0){
					const CStateNode* sTarg=childsVp->node;
					if (sTarg->constituent==PENN_CON_S && (!isLinked(vpNode,sTarg))) {
						bool firstCondition=true;
						//A $- B 	A is the immediate right sister of B
						if (childsVp->previous!=0){
							const CStateNode* leftSisterS=childsVp->previous->node;
							if (((*words)[leftSisterS->lexical_head].tag.code()==PENN_TAG_NOUN)) {
								CStateNodeList* childsNN=leftSisterS->m_umbinarizedSubNodes;
								while(childsNN!=0){
									const CStateNode* orderChild=childsNN->node;
									if ((*words)[orderChild->lexical_head].word==g_word_order){
										firstCondition=false;
									}
									childsNN=childsNN->next;
								 }
							 }
						}
						if (firstCondition){
							//S=target <: NP
							//A <: B 	B is the only child of A
							CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
							if (childsS!=0){
								if ((childsS->node->constituent==PENN_CON_NP) && (childsS->next ==0)){
									CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
									if (buildStanfordLink(label, sTarg->lexical_head, vpNode->lexical_head)) {
										//addLinked(vpNode,sTarg);
										addLinked(&node,sTarg); //I think this is not correct, in this specific case.
										return true;
									}
								}
							}
						}

					}
					childsVp=childsVp->next;
				}
			}
			childsfstVp=childsfstVp->next;
		}
	}
	return false;

}


//"(VP < (S=target < (VP < VBG ) !< NP !$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)]) !$-- /^:$/))",
  bool buildXComp7() {
  	if (node.constituent==PENN_CON_VP){
  		CStateNodeList* childsVp=node.m_umbinarizedSubNodes;
  		while(childsVp!=0){
  			const CStateNode* sTarg=childsVp->node;
  			if (sTarg->constituent==PENN_CON_S && (!isLinked(&node,sTarg))) {
  				bool secondCondition=true;// !<NP
  				bool thirdCondition=true; //!$- (/^,$/ [$- @NP  |$- (@PP $-- @NP ) |$- (@ADVP $-- @NP)])
  				bool fourthCondition=true; //!$-- /^:$/
  				CStateNodeList* childsS=sTarg->m_umbinarizedSubNodes;
  				while(childsS!=0){
  					if (childsS->node->constituent==PENN_CON_NP) {
  						secondCondition=false;
  					}
  					childsS=childsS->next;
  				}
  				if (secondCondition){
  					//A $-- B 	A is a right sister of B
  					CStateNodeList* leftSistersS=childsVp->previous;
  					while(leftSistersS!=0){
  						if ((*words)[leftSistersS->node->lexical_head].word==g_word_two_dots){
  							fourthCondition=false;
  						}
  						leftSistersS=leftSistersS->previous;
  					}
  					if (fourthCondition){
  						if (childsVp->previous!=0){
  							const CStateNode* commaLeftSisterS=childsVp->previous->node;
  							if ((*words)[commaLeftSisterS->lexical_head].word==g_word_comma){
  								//and now,
  								///^,$/ $- @NP $- (@PP $-- @NP )
  								//or
  								///^,$/ $- @NP $- (@ADVP $-- @NP)
  								//how is even possible that the inmediate left sister of comma is at the same time @NP and @PP...
  								bool firstInCondition=false;
  								bool secondInCondition=false;

  								//how come???

  								if (firstInCondition||secondInCondition){
  									thirdCondition=false;
  								}
  							}
  						}
  						if (thirdCondition){
  							childsS=sTarg->m_umbinarizedSubNodes;
  							while(childsS!=0){
  								const CStateNode* vpChildS=childsS->node;
  								if (vpChildS->constituent==PENN_CON_VP){
  									CStateNodeList* childsVp=vpChildS->m_umbinarizedSubNodes;
  									while(childsVp!=0){
  										if (((*words)[childsVp->node->lexical_head].tag.code()==PENN_TAG_VERB_PROG)) {
  											CDependencyLabel* label=new CDependencyLabel(STANFORD_DEP_XCOMP);
  											if (buildStanfordLink(label, sTarg->lexical_head, node.lexical_head)) {
  												//addLinked(vpNode,sTarg);
  												addLinked(&node,sTarg);
  												return true;
  											}
  										}
  										childsVp=childsVp->next;
  									}
  								}
  								childsS=childsS->next;
  							}
  						}
  					}
  				}
  			}
  			childsVp=childsVp->next;
  		}
  	}
  	return false;
  }
