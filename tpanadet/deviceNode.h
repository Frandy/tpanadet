/*
 * deviceNode.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef DEVICENODE_H_
#define DEVICENODE_H_

/*
 * TpDeviceNode:
 * 		TwoNode,
 * 		FourNode,
 * 		ArrayNode
 */


// array<type,size>,
typedef array<int,2> TwoNode;
typedef array<int,4> FourNode;
typedef vector<int> ArrayNode;

template<typename TpDeviceNode>
ostream& operator << (ostream& out,const TpDeviceNode& node)
{
	out << " node:";
	for(unsigned int i=0,t=node.size();i<t;i++)
		out << setw(4) << node[i] << " ";
	return out;
}

#endif /* DEVICENODE_H_ */
