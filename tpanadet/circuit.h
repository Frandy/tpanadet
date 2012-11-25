/*
 * circuit.h
 *
 *  Created on: Nov 25, 2012
 *      Author: chjd
 */

#ifndef CIRCUIT_H_
#define CIRCUIT_H_

class Circuit
{
public:
	list<Elem*> elemList;
	list<Task*> taskList;
	// map
	unordered_map<string,int> nodeMap;
	unordered_map<string,Subckt*> subcktMap;
/*	unordered_map<string,ParamNode*> paramMap;
	unordered_map<string,Model*> modelMap;
	unordered_map<string,vector<double>* > dataMap;
*/
};


#endif /* CIRCUIT_H_ */
