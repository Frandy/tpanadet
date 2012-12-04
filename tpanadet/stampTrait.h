/*
 * stampTrait.h
 *
 *  Created on: Dec 1, 2012
 *      Author: chjd
 */

#ifndef STAMPTRAIT_H_
#define STAMPTRAIT_H_

/*
 * Trait design
 */

/*
 * Policy may be more appropriate than Trait
 */

template<typename _DcSup,typename _DcLoad,
		typename _AcSup,typename _AcLoad
		typename _TranSup,typename _TranLoad
		>
class TpStampTrait
{
public:
	template<StampType type,typename _Sup,typename _Load>
	class TpStamp
	{
	public:
		typedef _Sup sup;
		typedef _Load load;
	};
public:
	typedef TpStamp<_DcSup,_DcLoad> DcStamp;
	typedef TpStamp<_AcSup,_AcLoad> AcStamp;
	typedef TpStamp<_TranSup,_TranLoad> TranStamp;
};

typedef TpStampTrait<EmptySup2,dNormalLoad,EmptySup2,cNormalLoad,EmptySup2,tNormalLoad> ResStamp;
typedef TpStampTrait<EmptySup2,dEmptyLoad2,EmptySup2,cNormalLoad,CLSupK,tCLoad> CapStamp;
typedef TpStampTrait<ShortSup,dEmptyLoad2,EmptySup2,cNormalLoad,CLSupK,tLLoad> IndStamp;

typedef TpStampTrait<ESup,dELoad,ESup,cELoad,ESup,tELoad> EStamp;
typedef TpStampTrait<FSup,dFLoad,FSup,cFLoad,FSup,tFLoad> FStamp;
typedef TpStampTrait<EmptySup4,dGLoad,EmptySup4,cGLoad,EmptySup4,tGLoad> GStamp;
typedef TpStampTrait<HSup,dHLoad,HSup,cHLoad,HSup,tHLoad> HStamp;

typedef TpStampTrait<VSupK,dVLoad,VSupK,cVLoad,VSupK,tVLoad> VStamp;
typedef TpStampTrait<EmptySup2,dILoad,EmptySup2,cILoad,EmptySup2,tILoad> IStamp;


typedef TpStampTrait<OpampSup,dEmptyLoad4,OpampSup,cEmptyLoad4,OpampSup,tEmptyLoad4> OpampStamp;


#endif /* STAMPTRAIT_H_ */
