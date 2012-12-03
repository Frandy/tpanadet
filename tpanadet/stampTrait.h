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
		/*typename _TranSup,typename _TranLoad*/
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
	/*typedef TpStamp<_TranSup,_TranLoad> TranStamp;*/
};

typedef TpStampTrait<EmptySup2,dNormalLoad,EmptySup2,cNormalLoad> ResStamp;
typedef TpStampTrait<EmptySup2,dEmptyLoad2,EmptySup2,cNormalLoad> CapStamp;
typedef TpStampTrait<ShortSup,dEmptyLoad2,EmptySup2,cNormalLoad> IndStamp;

typedef TpStampTrait<ESup,dELoad,ESup,cELoad> EStamp;
typedef TpStampTrait<FSup,dFLoad,FSup,cFLoad> FStamp;
typedef TpStampTrait<EmptySup4,dGLoad,EmptySup4,cGLoad> GStamp;
typedef TpStampTrait<HSup,dHLoad,HSup,cHLoad> HStamp;

typedef TpStampTrait<EmptySup2,dVLoad,EmptySup2,cVLoad> VStamp;
typedef TpStampTrait<EmptySup2,dILoad,EmptySup2,cILoad> IStamp;


typedef TpStampTrait<OpampSup,dEmptyLoad4,OpampSup,cEmptyLoad4> OpampStamp;


#endif /* STAMPTRAIT_H_ */
