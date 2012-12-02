/*
 * device.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef DEVICE_H_
#define DEVICE_H_



// template only conveniant for common device, RCLEFGHVI.
// add valuetype with model
// how to construct device ?



/*
typedef Device<TwoNode,ResValue> DeviceRes;
typedef Device<TwoNode,CapValue> DeviceCap;
typedef Device<TwoNode,IndValue> DeviceInd;
typedef Device<TwoNode,SourceValue> DeviceSource;
typedef Device<FourNode,CommonEFGHValue> DeviceEFGH;
*/

template<DeviceType type,typename _DeviceNode,typename _DeviceValue,
		typename _StampTrait,typename _SymbolTrait=void>
class TpDevice:public Elem
{
public:
	TpDevice()=default;
	// two node normal device
	TpDevice(string nm,int p,int n,double v/*,ParamExpr* expr=nullptr*/);
	// four node normal device
	TpDevice(string nm,int p,int n,int pp,int nn,double v=0/*,ParamExpr* expr=nullptr*/);
	// two node source device
	TpDevice(string nm,int p,int n,double v0,double v1);

	string Name()	{	return name;}
	void Name(string s) { name = s;}

	void Print(ostream& out){
			out << "name: ";
			out << name << "\t";
			out << node << "\t";
			out << value << " ";
			return out;
		}

/* Dc Analysis interface, sup & load */
	void InitDcSup(vector<int>& row,vector<int>& col)
	{
		StampTrait::DcStamp::sup sup;
		sup(node,row,col);
	}

	void DcLoad(MNA<double>& mna,vector<int>& row,vector<int>& col)
	{
		StampTrait::DcStamp::load load;
		load(node,value.DcValue(),mna,row,col);
	}

/* Ac Analysis interface, sup & load */
	void InitAcSup(vector<int>& row,vector<int>& col)
	{
		StampTrait::AcStamp::sup sup;
		sup(node,row,col);
	}

	void AcLoad(CPLX& s,MNA<CPLX>& mna,vector<int>& row,vector<int>& col)
	{
		StampTrait::AcStamp::load load;
		load(node,value.AcValue(s),mna,row,col);
	}

/* Tran Analysis interface, sup & load */
	void InitTranSup(vector<int>& row,vector<int>& col)
	{
		StampTrait::TranStamp::sup sup;
		sup(node,row,col);
	}

 	void TranLoad(double t,double h,MNA<double>& mna,vector<int>& row,vector<int>& col)
	{
 		StampTrait::TranStamp::load load;
		load(node,value.TranValue(h,t),mna,row,col);
	}

/* Symbol interface */


private:
	string name;
public:
	_DeviceNode node;
	_DeviceValue value;
	typedef _StampTrait StampTrait;
	typedef _SymbolTrait SymbolTrait;
};

/* construct for different device, source, RCL, & EFGH */

template<DeviceType type,typename _StampTrait>
TpDevice<type,TwoNode,SourceValue,_StampTrait,void>::TpDevice(string nm,int p,int n,double v0,double v1)
	:Elem(type),name(nm), node(TwoNode(p, n)), value(SourceValue(v0,v1))
{
}

template<DeviceType type,typename _DeviceValue,typename _StampTrait,typename _SymbolTrait>
TpDevice<type,TwoNode, _DeviceValue, _StampTrait,_SymbolTrait>::TpDevice(string nm, int p, int n,
		double v/*,ParamExpr* expr=nullptr*/) :
		Elem(type),name(nm), node(TwoNode(p, n)), value(_DeviceValue(v))
{
}

template<typename _DeviceValue, typename _StampTrait, typename _SymbolTrait>
TpDevice<FourNode, _DeviceValue, _StampTrait, _SymbolTrait>::TpDevice(string nm, int p,
		int n, int pp, int nn, double v/*,ParamExpr* expr=nullptr*/) :
	name(nm), node(FourNode(p, n, pp, nn)), value(_DeviceValue(v))
{
}


#endif /* DEVICE_H_ */
