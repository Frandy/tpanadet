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

template<DeviceType type,typename _DeviceNode,typename _DeviceValue,
		typename _StampTrait,typename _SymbolTrait=void>
class TpDevice:public Elem
{
public:
	TpDevice()=default;

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
	void InitDcSup(StampIndex& stampIndex)
	{
		StampTrait::DcStamp::sup sup;
		sup(node,stampIndex);
	}

	void DcLoad(MNA<double>& mna,StampIndex& stampIndex)
	{
		StampTrait::DcStamp::load load;
		load(node,value.DcValue(),mna,stampIndex);
	}

/* Ac Analysis interface, sup & load */
	void InitAcSup(StampIndex& stampIndex)
	{
		StampTrait::AcStamp::sup sup;
		sup(node,stampIndex);
	}

	void AcLoad(CPLX& s,MNA<CPLX>& mna,StampIndex& stampIndex)
	{
		StampTrait::AcStamp::load load;
		load(node,value.AcValue(s),mna,stampIndex);
	}

/* Tran Analysis interface, sup & load */

	void InitTranSup(StampIndex& stampIndex)
	{
		StampTrait::TranStamp::sup sup;
		sup(node,stampIndex);
	}

	template<IntegrationType type>
 	void TranLoad(double t,double h,MNA<double>& mna,StampIndex& stampIndex,TranHistory& history)
	{
 		StampTrait::TranStamp::load load;
		load<type>(node,value.TranValue(h,t),mna,stampIndex,history);
	}

/* Symbol interface */


private:
	string name;
public:
	_DeviceNode node;
	_DeviceValue value;
	typedef _DeviceNode NodeType;
	typedef _DeviceValue ValueType;
	typedef _StampTrait StampTrait;
	typedef _SymbolTrait SymbolTrait;
};


/* construct for different device, source, RCL, & EFGH */
// not support partial specify for member function

/*
template<DeviceType type,typename _StampTrait>
TpDevice<type,TwoNode,SourceValue,_StampTrait,void>::TpDevice(string nm,int p,int n,double v0,double v1)
	:Elem(type),name(nm), node(TwoNode(p, n)), value(SourceValue(v0,v1))
{
}

template<DeviceType type,typename _DeviceValue,typename _StampTrait,typename _SymbolTrait>
TpDevice<type,TwoNode, _DeviceValue, _StampTrait,_SymbolTrait>::TpDevice(string nm, int p, int n,
		double v,ParamExpr* expr=nullptr) :
		Elem(type),name(nm), node(TwoNode(p, n)), value(_DeviceValue(v))
{
}

template<typename _DeviceValue, typename _StampTrait, typename _SymbolTrait>
TpDevice<FourNode, _DeviceValue, _StampTrait, _SymbolTrait>::TpDevice(string nm, int p,
		int n, int pp, int nn, double v,ParamExpr* expr=nullptr) :
	name(nm), node(FourNode(p, n, pp, nn)), value(_DeviceValue(v))
{
}
*/

typedef TpDevice<DeviceType::R,TwoNode,ResValue,ResStamp> ElemRes;
typedef TpDevice<DeviceType::C,TwoNode,CapValue,CapStamp> ElemCap;
typedef TpDevice<DeviceType::L,TwoNode,IndValue,IndStamp> ElemInd;
typedef TpDevice<DeviceType::E,FourNode,NormalEFGHValue,EStamp> ElemE;
typedef TpDevice<DeviceType::F,FourNode,NormalEFGHValue,FStamp> ElemF;
typedef TpDevice<DeviceType::G,FourNode,NormalEFGHValue,GStamp> ElemG;
typedef TpDevice<DeviceType::H,FourNode,NormalEFGHValue,HStamp> ElemH;
typedef ElemE ElemVCVS;
typedef ElemF ElemCCCS;
typedef ElemG ElemVCCS;
typedef ElemH ElemCCVS;

typedef TpDevice<DeviceType::V,TwoNode,SourceValue,VStamp> ElemVs;
typedef TpDevice<DeviceType::I,TwoNode,SourceValue,VStamp> ElemIs;

typedef TpDevice<DeviceType::OPAMP,FourNode,NormalEFGHValue,OpampStamp> ElemOpamp;


// two node normal device
//TpDevice(string nm,int p,int n,double v/*,ParamExpr* expr=nullptr*/);
// four node normal device
//TpDevice(string nm,int p,int n,int pp,int nn,double v=0/*,ParamExpr* expr=nullptr*/);
// two node source device
//TpDevice(string nm,int p,int n,double v0,double v1);


/*
 * R,C,L
 */
template<typename Device,DeviceType type>
Device* makeDevice(string nm, int p, int n,double v,ParamExpr* expr=nullptr)
{
	Device* device = new Device(type,nm);
	device->node[0] =  p;
	device->node[1] = n;
	device->value = Device::ValueType(v,expr);
	return device;
}

/*
 * V,I
 */

/*
 * E,F,G,H, OPAMP
 */
template<typename Device,DeviceType type>
Device* makeDevice(string nm, int p, int n,int pp,int nn,double v=0,ParamExpr* expr=nullptr)
{
	Device* device = new Device(type,nm);
	device->node[0] =  p;
	device->node[1] = n;
	device->node[2] = pp;
	device->node[3] = nn;
	device->value = Device::ValueType(v,expr);
	return device;
}



#endif /* DEVICE_H_ */
