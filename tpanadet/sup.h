/*
 * sup.h
 *
 *  Created on: Nov 26, 2012
 *      Author: chjd
 */

#ifndef SUP_H_
#define SUP_H_

/*
 * sup algorithm:
 * 		initial row & col vector 0~N
 * 		sup elem & re-index, only for nullor, switch, & shorted elem
 * 		only sup for F,G,H,Vs, no re-index
 *		initial stamp entry
 * example:
 * 		nullor  4 0 0 3,  Vs 1 0, out 5 0,
 * 		initila: row 0 1 2 3 4 5
 * 				 col 0 1 2 3 4 5
 *		supe: nullor col 3 -> 0, row 4 -> 0
 *			row 0 1 2 3 0 4
 *			col 0 1 2 0 3 4
 *			  Vs row 1 -> 0, & add row k=1
 *			row 0 0 2 3 0 4
 *		    	note: 1->0 but no re-index, since 1 will be used as branch
 *		    		and others know 1->0
 *		so, Vs  0 1       rhs
 *		      0 .
 *		      1 . 1  v1   Vs
 *		stamp Vs	col: col[p] 	col[n]
 *					row: row[small] row[large]
 *		for other elements
 *			  H		col: 		col[small(a,b)]  col[large(a,b)] col[c] col[d]
 *			  		row:  row[a]				1
 *			  			  row[b]				-1
 *			  			  row[small(c,d)]
 *			  			  row[large(c,d)]		H				 -1		1
 *
 *
 *		stamp saved in stamp, not change device node
 *		stamp vector, vector<pair<int,int> > pos, vs. vector<int> colrow.
 */

class SupReindex
{
public:

	void operator()(int a, int b, vector<int>& nodeset)
	{
		if (b == a)
			return;
		if (b < a)
			swap(a, b);
		// b > a
		{
			for (int i = 0, t = nodeset.size(); i < t; i++)
			{
				if (nodeset[i] > nodeset[b])
					nodeset[i] = nodeset[i] - 1;
			}
			nodeset[b] = nodeset[a];
		}
	}
};

class SupOnly
{
public:
	void operator()(int a, int b, vector<int>& nodeset)
	{
		if (b == a)
			return;
		if (b < a)
			swap(a, b);
		nodeset[b] = nodeset[a];
	}
};

// only need for TwoNode
class EmptySup
{
public:
	void operator()(TwoNode& node,vector<int>& row,vector<int>& col)
	{
	}
};

class ESup
{
public:
	void operator()(FourNode& node,vector<int>& row,vector<int>& col)
	{
		SupOnly sup;
		sup(node[2],node[3],row);
	}
};

class FSup
{
public:
	void operator()(FourNode& node,vector<int>& row,vector<int>& col)
	{
		SupOnly sup;
		sup(node[0],node[1],col);
	}
};

class HSup
{
public:
	void operator()(FourNode& node,vector<int>& row,vector<int>& col)
	{
		SupOnly sup;
		sup(node[0],node[1],col);
		sup(node[2],node[3],row);
	}
};

class OpampSup
{
public:
	void operator()(FourNode& node,vector<int>& row,vector<int>& col)
	{
		SupReindex sup;
		sup(node[0],node[1],col);
		sup(node[2],node[3],row);
	}
};

class ShortSup
{
public:
	void operator()(Two& node,vector<int>& row,vector<int>& col)
	{
		SupReindex sup;
		sup(node[0],node[1],col);
		sup(node[0],node[1],row);
	}
};

#endif /* SUP_H_ */
