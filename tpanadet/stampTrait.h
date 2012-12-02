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
		typename _AcSup,typename _AcLoad,
		typename _TranSup,typename _TranLoad>
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

typedef TpStampTrait<EmptySup2,> ResStamp;

#endif /* STAMPTRAIT_H_ */
