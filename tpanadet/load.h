/*
 * load.h
 *
 *  Created on: Nov 26, 2012
 *      Author: chjd
 */

#ifndef LOAD_H_
#define LOAD_H_

/*
 * stamp index with branch index, as default StampIndex
 * for,
 * 	generalized MNA,
 * 	partial improved MNA
 * 	transient C, L
 * also for,
 * 	improved MNA
 *
 */

class kStampIndex
{
public:
	kStampIndex()=default;
	kStampIndex(int N) :
		k(0)
	{
		row = vector<int> (N + 1, 0);
		col = vector<int> (N + 1, 0);
		for (int i = 0; i <= N; i++)
		{
			row[i] = col[i] = i;
		}
	}
	kStampIndex(kStampIndex& stamp)
	{
		row = stamp.row;
		col = stamp.col;
		index = stamp.index;
		k = stamp.k;
	}
	int GetK()
	{
		assert(k < index.size());
		return index[k++];
	}
	void IncK()
	{
		index.push_back(k++);
	}
	void Reset()
	{
		k = 0;
	}
	int Size()
	{
		return row.size() + index.size();
	}
public:
	vector<int> row;
	vector<int> col;
	vector<int> index;
	int k;
};

typedef kStampIndex StampIndex;

/*
 * transient history for x
 * use deque
 */

class TranHistory
{
public:
	TranHistory(int n) :
		max_record(n)
	{
	}
	void Push(vector<double>& x)
	{
		his.push_back(x);
		if (his.size() > max_record)
			his.pop_front();
	}
	double GetX(int a, int b)
	{
		return his[a][b];
	}
	double operator()(int a, int b)
	{
		return his[a][b];
	}
public:
	deque<vector<double> > his;
	const int max_record;
};

/*
 * integration type
 */
enum class IntegrationType
{
	BE, TP
};

/*
 * empty load,
 * for device:
 * 	OPAMP, -> DC, AC, TRAN
 * 	C,L -> DC
 */

template<typename Node, typename vType>
class EmptyLoad
{
public:
	void operator()(Node& node, vType value, MNA<vType>& mna, vector<int>& row,
			vector<int>& col)
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
	void operator()(TwoNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		// assert(row.size()==2 && col.size()==2);
		int rowa = stampIndex.row[node[0]];
		int rowb = stampIndex.row[node[1]];
		int cola = stampIndex.col[node[0]];
		int colb = stampIndex.col[node[1]];
		if (rowa == rowb || cola == colb)
		{
			assert(rowa == rowb && cola == colb);
			return;
		}
		mna.CellAdd(rowa, cola, value);
		mna.CellAdd(rowa, colb, -value);
		mna.CellAdd(rowb, colb, -value);
		mna.CellAdd(rowb, colb, value);
	}
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
	void operator()(FourNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		// assert(row.size()==4 && col.size()==4)
		/*
		 int cd,tk;
		 minmax(node[0],node[1],cd,tk);
		 */
		int tk = max(node[0], node[1]);
		int rowk = stampIndex.row[tk];
		int colc = stampIndex.col[node[0]];
		int cold = stampIndex.col[node[1]];
		int cola = stampIndex.col[node[2]];
		int colb = stampIndex.col[node[3]];

		mna.CellSet(rowk, cola, value);
		mna.CellSet(rowk, colb, -value);
		mna.CellSet(rowk, colc, vType(-1));
		mna.CellSet(rowk, cold, vType(1));
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
	void operator()(FourNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		// assert(row.size()==4 && col.size()==4)

		int colk = stampIndex.col[max(node[2], node[3])];
		int rowc = stampIndex.row[node[0]];
		int rowd = stampIndex.row[node[1]];
		int rowa = stampIndex.row[node[2]];
		int rowb = stampIndex.row[node[3]];

		mna.CellSet(rowa, colk, vType(1));
		mna.CellSet(rowb, colk, vType(-1));
		mna.CellSet(rowc, colk, value);
		mna.CellSet(rowd, colk, -value);
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
	void operator()(FourNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		// assert(row.size()==4 && col.size()==4)

		int rowc = stampIndex.row[node[0]];
		int rowd = stampIndex.row[node[1]];
		int cola = stampIndex.col[node[2]];
		int colb = stampIndex.col[node[3]];

		mna.CellSet(rowc, cola, value);
		mna.CellSet(rowd, colb, -value);
		mna.CellSet(rowc, cola, -value);
		mna.CellSet(rowd, colb, value);
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
	void operator()(FourNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		// assert(row.size()==4 && col.size()==4)
		int rowk = stampIndex.row[max(node[0], node[1])];
		int colk = stampIndex.col[max(node[2], node[3])];

		int rowa = stampIndex.row[node[2]];
		int rowb = stampIndex.row[node[3]];
		int colc = stampIndex.col[node[0]];
		int cold = stampIndex.col[node[1]];

		mna.CellSet(rowa, colk, vType(1));
		mna.CellSet(rowb, colk, vType(-1));
		mna.CellSet(rowk, colk, value);
		mna.CellSet(rowk, colc, vType(-1));
		mna.CellSet(rowk, cold, vType(1));
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
	void operator()(TwoNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		int rowk = stampIndex.row[max(node[0], node[1])];
		int cola = stampIndex.col[node[0]];
		int colb = stampIndex.col[node[1]];

		mna.CellSet(rowk, cola, vType(1));
		mna.CellSet(rowk, colb, vType(-1));
		mna.RhsSet(rowk, value);
	}
};

template<typename vType>
class VLoadK
{
public:
	void operator()(TwoNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		int bk = stampIndex.GetK();
		int rowa = stampIndex.row[node[0]];
		int rowb = stampIndex.row[node[1]];
		int cola = stampIndex.col[node[0]];
		int colb = stampIndex.col[node[1]];

		mna.CellSet(rowa,bk,vType(1));
		mna.CellSet(rowb,bk,vType(-1));
		mna.CellSet(bk, cola, vType(1));
		mna.CellSet(bk, colb, vType(-1));
		mna.RhsSet(bk, value);
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
	void operator()(TwoNode& node, vType value, MNA<vType>& mna,
			StampIndex& stampIndex)
	{
		int rowa = stampIndex.row[node[0]];
		int rowb = stampIndex.row[node[1]];

		mna.RhsAdd(rowa, -value);
		mna.RhsAdd(rowb, value);
	}
};

/*
 * integration method
 */

/*
 * transient C Load,
 * BE,
 * 		N+		N-		ik		rhs
 * 	N+			 		1
 * 	N-					-1
 * 	bk	c/h		-c/h	-1		C/h * v[t-h]
 */
template<IntegrationType integration>
class CTranLoad
{
public:
	void operator()(TwoNode& node, double value, MNA<double>& mna,
			StampIndex& stampIndex, TranHistory& history);
};

/*
 * C, BE
 * 	(v[t] - v[t-h]) / h = i[t] / C
 * 	C/h . v[t] - i[t] = C/h . v[t-h]
 */

template<>
void CTranLoad<IntegrationType::BE>::operator()(TwoNode& node, double value,
		MNA<double>& mna, StampIndex& stampIndex, TranHistory& history)
{
	int rowa = stampIndex.row[node[0]];
	int rowb = stampIndex.row[node[1]];

	int cola = stampIndex.col[node[0]];
	int colb = stampIndex.col[node[1]];

	int bk = stampIndex.GetK();

	// rhs = C/h * v[t-h]
	double rhs = value * (history.GetX(0, cola) - history.GetX(0, colb));

	mna.CellSet(rowa, bk, 1);
	mna.CellSet(rowb, bk, -1);
	mna.CellSet(bk, cola, value);
	mna.CellSet(bk, colb, -value);
	mna.CellSet(bk, bk, -1);
	mna.RhsSet(bk, rhs);
}

/*
 * C, TP
 * 	(v[t] - v[t-h]) / h = 1/2 . (i[t]-i[t-h]) / C
 * 	C/h . v[t] - 1/2 . i[t] = C/h . v[t-h] - 1/2 . i[t-h]
 */
template<>
void CTranLoad<IntegrationType::TP>::operator()(TwoNode& node, double value,
		MNA<double>& mna, StampIndex& stampIndex, TranHistory& history)
{
	int rowa = stampIndex.row[node[0]];
	int rowb = stampIndex.row[node[1]];

	int cola = stampIndex.col[node[0]];
	int colb = stampIndex.col[node[1]];

	int bk = stampIndex.GetK();

	// rhs = C/h . v[t-h] - 1/2 . i[t-h]
	double rhs = value * (history.GetX(0, cola) - history.GetX(0, colb)) - 1.0
			/ 2 * history.GetX(0, bk);

	mna.CellSet(rowa, bk, 1);
	mna.CellSet(rowb, bk, -1);
	mna.CellSet(bk, cola, value);
	mna.CellSet(bk, colb, -value);
	mna.CellSet(bk, bk, -1.0 / 2);
	mna.RhsSet(bk, rhs);
}

/*
 * transient L Load,
 * BE,
 * 		N+		N-		ik		rhs
 * 	N+			 		1
 * 	N-					-1
 * 	bk	-1		1		L/h		L/h * i[t-h]
 */
template<IntegrationType integration>
class LTranLoad
{
public:
	void operator()(TwoNode& node, double value, MNA<double>& mna,
			StampIndex& stampIndex, TranHistory& history);
};

/*
 * L, TP
 * 	(i[t] - i[t-h]) . L / h = v[t]
 * 	- v[t] + L/h . i[t] = L/h . i[t-h]
 */
template<>
void LTranLoad<IntegrationType::BE>::operator()(TwoNode& node, double value,
		MNA<double>& mna, StampIndex& stampIndex, TranHistory& history)
{
	int rowa = stampIndex.row[node[0]];
	int rowb = stampIndex.row[node[1]];

	int cola = stampIndex.col[node[0]];
	int colb = stampIndex.col[node[1]];

	int bk = stampIndex.GetK();

	// rhs = C/h * i[t-h]
	double rhs = value * (history.GetX(0, bk));

	mna.CellSet(rowa, bk, 1);
	mna.CellSet(rowb, bk, -1);
	mna.CellSet(bk, cola, -1);
	mna.CellSet(bk, colb, 1);
	mna.CellSet(bk, bk, value);
	mna.RhsSet(bk, rhs);
}

/*
 * L, TP
 * 	(i[t] - i[t-h]) L / h = 1/2 . (v[t]-v[t-h])
 * 	- 1/2 . v[t] + L/h . i[t] = L/h . i[t-h] - 1/2 . v[t-h]
 */
template<>
void LTranLoad<IntegrationType::TP>::operator()(TwoNode& node, double value,
		MNA<double>& mna, StampIndex& stampIndex, TranHistory& history)
{
	int rowa = stampIndex.row[node[0]];
	int rowb = stampIndex.row[node[1]];

	int cola = stampIndex.col[node[0]];
	int colb = stampIndex.col[node[1]];

	int bk = stampIndex.GetK();

	// rhs = L/h . i[t-h] - 1/2 . v[t-h]
	double rhs = -1.0 / 2 * (history.GetX(0, cola) - history.GetX(0, colb))
			+ value * history.GetX(0, bk);

	mna.CellSet(rowa, bk, 1);
	mna.CellSet(rowb, bk, -1);
	mna.CellSet(bk, cola, -1.0 / 2);
	mna.CellSet(bk, colb, 1.0 / 2);
	mna.CellSet(bk, bk, value);
	mna.RhsSet(bk, rhs);
}

/*
 * transient load for C, L, include all kinds of integration methods stamp
 */
template<typename TLoad>
class TranLoad
{
public:
	// default load
	template<IntegrationType integration>
	void operator()(TwoNode& node, double value, MNA<double>& mna,
			StampIndex& stampIndex, TranHistory& history)
	{
		tload<integration> load;
		load(node, value, mna, stampIndex, history);
	}
public:
	typedef TLoad tload;
};

// R, E, F, G, H, reuse dc load
template<typename Node, typename tLoad>
class NormalTranLoad
{
public:
	template<IntegrationType integration>
	void operator()(Node& node, double value, MNA<double>& mna,
			StampIndex& stampIndex, TranHistory& history)
	{
		tLoad load;
		load(node, value, mna, stampIndex);
	}
};

/*
 * empty load
 * for
 * 	OPAMP, Switch,
 * 	dc C, L
 */
typedef EmptyLoad<TwoNode, double> dEmptyLoad2;
typedef EmptyLoad<TwoNode, Cplx> cEmptyLoad2;
typedef NormalTranLoad<TwoNode, dEmptyLoad2> tEmptyLoad2;

typedef EmptyLoad<FourNode, double> dEmptyLoad4;
typedef EmptyLoad<FourNode, Cplx> cEmptyLoad4;
typedef NormalTranLoad<FourNode, dEmptyLoad4> tEmptyLoad4;

/*
 * normal load
 * for
 *  R,
 *  ac C, L
 */

typedef NormalLoad<double> dNormalLoad;
typedef NormalLoad<Cplx> cNormalLoad;
typedef NormalTranLoad<TwoNode, dNormalLoad> tNormalLoad;

/*
 * EFGH load
 * for
 * 	E, F, G, H
 */

typedef ELoad<double> dELoad;
typedef ELoad<Cplx> cELoad;
typedef NormalTranLoad<FourNode, dELoad> tELoad;

typedef FLoad<double> dFLoad;
typedef FLoad<Cplx> cFLoad;
typedef NormalTranLoad<FourNode, dFLoad> tFLoad;

typedef GLoad<double> dGLoad;
typedef GLoad<Cplx> cGLoad;
typedef NormalTranLoad<FourNode, dGLoad> tGLoad;

typedef HLoad<double> dHLoad;
typedef HLoad<Cplx> cHLoad;
typedef NormalTranLoad<FourNode, dHLoad> tHLoad;

/*
 * source load
 * for
 * 	V, I
 */

typedef VLoad<double> dVLoad;
typedef VLoad<Cplx> cVLoad;
typedef NormalTranLoad<TwoNode, dVLoad> tVLoad;

typedef ILoad<double> dILoad;
typedef ILoad<Cplx> cILoad;
typedef NormalTranLoad<TwoNode, dILoad> tILoad;

/*
 * transient C,L load
 * for
 * 	tran C,L
 */

typedef TranLoad<CBELoad> tCLoad;
typedef TranLoad<LBELoad> tLLoad;

#endif /* LOAD_H_ */
