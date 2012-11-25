/*
 * subckt.h
 *
 *  Created on: Nov 25, 2012
 *      Author: chjd
 */

#ifndef SUBCKT_H_
#define SUBCKT_H_

class Subckt
{
public:
	Subckt(string nm):name(nm){}
	void AddNode(int p){node.push_back(p);}
	void AddParam(string nm,double v=0){
		paramMap.insert(make_pair(nm,new ParamNode(v)));
	}
	void AddElem(Elem* elem){elemList.push_back(elem);}
	~Subckt(){
		for(auto it=paramMap.begin(),et=paramMap.end();it!=et;it++)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}
private:
	// subckt name
	string name;
public:
	ArrayNode node;
	list<Elem*> elemList;
	unordered_map<string,ParamNode*> paramMap;
};


#endif /* SUBCKT_H_ */
