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
 * TpStampTrait
 */

template<typename _DcStamp,typename _AcStamp,typename _TranStamp>
class TpStampTrait
{
public:
	typedef _DcStamp DcStamp;
	typedef _AcStamp AcStamp;
	typedef _TranStamp TranStamp;
};


#endif /* STAMPTRAIT_H_ */
