/*
 * analysis.h
 *
 *  Created on: Dec 2, 2012
 *      Author: chjd
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

template<AnalysisType type>
class Analysis
{
public:
	Analysis()=default;
public:
	Circuit* ckt;
	/* workspace */
	list<Elem*> elemList;
	unordered_map<string,int> nodeMap;

};


#endif /* ANALYSIS_H_ */
