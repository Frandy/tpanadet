/*
 * load.h
 *
 *  Created on: Nov 26, 2012
 *      Author: chjd
 */

#ifndef LOAD_H_
#define LOAD_H_


/*
 * empty load,
 * for device:
 * 	OPAMP, -> DC, AC, TRAN
 * 	C,L -> DC
 */

template<typename Node,typename vType>
class EmptyLoad
{
public:
	void operator()(Node& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
	}
};

/*
 * NormalLoad
 * 			col[0]	col[1]
 * 	row[0]	value	-value
 * 	row[1]	-value	value
 *
 * for device:
 * 	R -> DC,AC,TRAN   C, L -> AC
 *
 * but if device was short,
 * 	choice, ignore and load as normal, value will be sum canceled, maybe numerical error
 * 			skip load, so need to judge whether was short
 */

template<typename vType>
class NormalLoad
{
public:
	void operator()(TwoNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		// assert(row.size()==2 && col.size()==2);
		int rowa = row[node[0]];
		int rowb = row[node[1]];
		int cola = col[node[0]];
		int colb = col[node[1]];
		if(rowa==rowb || cola==colb)
		{
			assert(rowa==rowb && cola==colb);
			return;
		}
		mna.CellAdd(rowa,cola,value);
		mna.CellAdd(rowa,colb,-value);
		mna.CellAdd(rowb,colb,-value);
		mna.CellAdd(rowb,colb,value);
	}
};

class TranLoad
{
// a little complex, related with last value
// add branch for C/L, related with initial value or last-step value and step
};


/*
 * improved MNA Load
 * for device:
 * 	E, F, H -> DC,AC,TRAN
 */

/*
 * control source entry, improved MNA
 * 		source edge: 	c, d  <- 0, 1
 * 		control edge:	a, b  <- 2, 3
 * 	E entry:
 * 			col:		c	d	a	b
 * 			row:  	(c,d)
 * 				  	tk	-1	1	E	-E
 * 				  	a
 * 				  	b
 */

/*
 * get the min and max together, only once comparison
 */

// not used
/*
void minmax(int a,int b,int& mi,int& ma)
{
	if(a<b)
	{
		mi = a;
		ma = b;
	}
	else
	{
		mi = b;
		ma = a;
	}
}
*/

template<typename vType>
class ELoad
{
public:
	void operator()(FourNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		// assert(row.size()==4 && col.size()==4)
/*
		int cd,tk;
		minmax(node[0],node[1],cd,tk);
*/
		int tk = max(node[0],node[1]);
		int rowk = row[tk];
		int colc = col[node[0]];
		int cold = col[node[1]];
		int cola = col[node[2]];
		int colb = col[node[3]];

		mna.CellSet(rowk,cola, value);
		mna.CellSet(rowk,colb,-value);
		mna.CellSet(rowk,colc,vType(-1));
		mna.CellSet(rowk,cold,vType(1));
	}
};

/*
 * F entry:
 * 			col:		c	d	(a,b)	iab
 * 			row:	c			 F
 * 					d			-F
 * 					a			 1
 * 					b			-1
 */

template<typename vType>
class FLoad
{
public:
	void operator()(FourNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		// assert(row.size()==4 && col.size()==4)

		int colk = col[max(node[2],node[3])];
		int rowc = row[node[0]];
		int rowd = row[node[1]];
		int rowa = row[node[2]];
		int rowb = row[node[3]];

		mna.CellSet(rowa,colk,vType(1));
		mna.CellSet(rowb,colk,vType(-1));
		mna.CellSet(rowc,colk, value);
		mna.CellSet(rowd,colk,-value);
	}
};

/*
 * G entry:
 * 		col:		c	d	a	b
 * 		row:	c			 G	-G
 * 				d			-G	 G
 * 				a
 * 				b
 */


template<typename vType>
class GLoad
{
public:
	void operator()(FourNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		// assert(row.size()==4 && col.size()==4)

		int rowc = row[node[0]];
		int rowd = row[node[1]];
		int cola = row[node[2]];
		int colb = row[node[3]];

		mna.CellSet(rowc,cola, value);
		mna.CellSet(rowd,colb,-value);
		mna.CellSet(rowc,cola,-value);
		mna.CellSet(rowd,colb, value);
	}
};

/*
 * H entry:
 * 		col:		c	d	(a,b)	iab
 * 		row:	(c,d)
 * 				tk	-1	1			 H
 * 				a					 1
 * 				b					-1
 */


template<typename vType>
class HLoad
{
public:
	void operator()(FourNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		// assert(row.size()==4 && col.size()==4)
		int rowk = row[max(node[0],node[1])];
		int colk = col[max(node[2],node[3])];

		int rowa = row[node[2]];
		int rowb = row[node[3]];
		int colc = col[node[0]];
		int cold = col[node[1]];

		mna.CellSet(rowa,colk,vType(1));
		mna.CellSet(rowb,colk,vType(-1));
		mna.CellSet(rowk,colk,value);
		mna.CellSet(rowk,colc,vType(-1));
		mna.CellSet(rowk,cold,vType(1));
	}
};

/*
 * improved MNA Load
 * for device:
 * 		voltage & current source -> DC,AC,TRAN
 */

/*
 * V entry:
 * 		col:		a	b		rhs
 * 		row:	(a,b)
 * 				tk	1	-1		Vs
 */

template<typename vType>
class VLoad
{
public:
	void operator()(TwoNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		int rowk = row[max(node[0],node[1])];
		int cola = col[node[0]];
		int colb = col[node[1]];

		mna.CellSet(rowk,cola,vType(1));
		mna.CellSet(rowk,colb,vType(-1));
		mna.RhsSet(rowk,value);
	}
};

/*
 * I entry:
 * 		col:		rhs
 * 		row:	a	-I
 * 				b	 I
 */

template<typename vType>
class ILoad
{
public:
	void operator()(TwoNode& node,vType value,MNA<vType>& mna,vector<int>& row,vector<int>& col)
	{
		int rowa = row[node[0]];
		int rowb = row[node[1]];

		mna.RhsAdd(rowa,-value);
		mna.RhsAdd(rowb, value);
	}
};


#endif /* LOAD_H_ */
