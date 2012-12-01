/*
 * stamp.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef STAMP_H_
#define STAMP_H_


/*
 * Device,
 * 	policy trait:
 * 		_DcLoad,
 * 		_AcLoad,
 *
 * 		_DcSup,
 * 		_AcSup,
 *
 */

template<MnaAnalysisType type,typename _Sup,typename _Load>
class TpStamp
{
public:
	typedef _Sup sup;
	typedef _Load load;
};


template<typename _Device>
class Stamp
{
public:
	//@{
	/**
	 * @brief load device into MNA, according to device type
	 */
	void DcLoad(_Device& device,MNA<double>& mna,vector<int>& row,vector<int>& col)
	{
		_Device::_Trait::_DcLoad load;
		load(device.node,device.DcValue(),mna,row,col);
	}

/*
 	void TranLoad(Device& ,device,double t,double h,MNA<double>& mna,vector<int>& row,vector<int>& col)
	{
		_Device::_Trait::_TranLoad load;
		load(device.node,device.TranValue(h,t),mna,row,col);
	}
*/
	void AcLoad(Device& device,CPLX& s,MNA<CPLX>& mna,vector<int>& row,vector<int>& col)
	{
		_Device::_Trait::_AcLoad load;
		load(device.node,device.AcValue(s),mna,row,col);
	}
	//@}
	//@{
	/**
	 * @brief sup shorted node, and re-index row and column
	 */
	void InitDcSup(Device& device,vector<int>& row,vector<int>& col)
	{
		_Device::_Trait::_DcSup sup;
		sup(device.node,row,col);
	}
//	void InitTranSup(Device& device,vector<int>& row,vector<int>& col);
	void InitAcSup(Device& device,vector<int>& row,vector<int>& col)
	{
		_Device::_Trait::_AcSup sup;
		sup(device.node,row,col);
	}
	//@}
};

// empty stamp
template<>
class Stamp<void>
{
};


#endif /* STAMP_H_ */
