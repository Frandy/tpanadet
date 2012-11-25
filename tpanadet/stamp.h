/*
 * stamp.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef STAMP_H_
#define STAMP_H_

template<typename Device>
class Stamp
{
public:
	//@{
	/**
	 * @brief load device into MNA, according to device type
	 */
	void LoadDc(Device& device,MNA<double>& mna);
	void LoadTran(Device&,double t,double h,device,MNA<double>& mna);
	void LoadAc(Device& device,CPLX& s,MNA<CPLX>& mna);
	//@}
	//@{
	/**
	 * @ brief sup shorted node, and re-index row and column
	 */
	void InitDcEntry(Device& device,MNA<double>& mna);
	void InitTranEntry(Device& device,MNA<double>& mna);
	void InitAcEntry(Device& device,MNA<CPLX>& mna);
	//@}
	//@{
	/**
	 * @ brief initial stamp entry
	 */
	void AddDcEntry(Device& device,MNA<double>& mna);
	void AddTranEntry(Device& device,MNA<double>& mna);
	void AddAcEntry(Device& device,MNA<CPLX>& mna);
	//@}
private:
	vector<pair<int,int> > dcentry;
	vector<pair<int,int> > tranentry;
	vector<pair<int,int> > acentry;
};

// empty stamp
template<>
class Stamp<void>
{
};


#endif /* STAMP_H_ */
