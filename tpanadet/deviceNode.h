/*
 * deviceNode.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef DEVICENODE_H_
#define DEVICENODE_H_

/*
 * DeviceNode:
 * 		TwoNode,
 * 		FourNode,
 * 		ArrayNode
 */


// use fisrt instead of nodep, nodepp, not easy misuse

/*
class TwoNode
{
public:
	TwoNode()=default;
	TwoNode(int p,int n):nodep(p),noden(n){}
	TwoNode(const TwoNode& node){
		nodep= node.First();
		noden = node.Second();}
	int First() const { return nodep;}
	int Second() const { return noden;}
protected:
	int nodep;
	int noden;
};
*/
/*
class FourNode:public TwoNode
{
public:
	FourNode()=default;
	FourNode(int p,int n,int pp,int nn):TwoNode(p,n),nodepp(pp),nodenn(n){}
	FourNode(const FourNode& node){
		nodep = node.First();
		noden = node.Second();
		nodepp = node.Third();
		nodenn = node.Fourth();}
	int Third() const { return nodepp;}
	int Fourth() const { return nodenn;}
private:
	int nodepp;
	int nodenn;
};
*/

/*
subckt_node:
	node
			$$ = new VaryNode();
			$$->push_back(node);
	| subckt_node node,
			$$ = $1->push_back(node)
*/

// array<type,size>, not good for initialize
typedef array<int,2> TwoNode;
typedef array<int,4> FourNode;
typedef vector<int> ArrayNode;


ostream& operator << (ostream& out,const TwoNode& node)
{
	out << " node: ";
	out << setw(4) << node[0] << " " << node[1] << " ";
	return out;
}

ostream& operator << (ostream& out,const FourNode& node)
{
	out << " node: ";
	out << setw(4) << node[0] << " " << node[1] << "  ";
	out << setw(4) << node[2] << " " << node[3] << " ";
	return out;
}

ostream& operator << (ostream& out,const ArrayNode& node)
{
	out << " node:";
	for(unsigned int i=0,t=node.size();i<t;i++)
		out << setw(4) << node[i] << " ";
	return out;
}

#endif /* DEVICENODE_H_ */
