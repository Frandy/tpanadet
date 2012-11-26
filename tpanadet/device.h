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

template<typename _DeviceNode,
		typename _DeviceValue,
		typename _DeviceTrait>
class Device
{
public:
	Device()=default;
	// two node normal device
	Device(string nm,int p,int n,double v/*,ParamExpr* expr=nullptr*/);
	// four node normal device
	Device(string nm,int p,int n,int pp,int nn,double v=0/*,ParamExpr* expr=nullptr*/);
	// two node source device
	Device(string nm,int p,int n,double v0,double v1);

	string Name()	{	return name;}
	void Name(string s) { name = s;}

	friend ostream& operator << (ostream& out,Device& device){
			out << "name: ";
			out << device.Name() << "\t";
			out << device.node << "\t";
			out << device.value << " ";
			return out;
		}

private:
	string name;
public:
	_DeviceNode node;
	_DeviceValue value;
	typedef _DeviceTrait _Trait;
};

template<typename _DeviceTrait>
Device<TwoNode,SourceValue,_DeviceTrait>::Device(string nm,int p,int n,double v0,double v1)
	:name(nm), node(TwoNode(p, n)), value(SourceValue(v0,v1))
{
}

template<typename _DeviceValue,typename _DeviceTrait>
Device<TwoNode, _DeviceValue, _DeviceTrait>::Device(string nm, int p, int n,
		double v/*,ParamExpr* expr=nullptr*/) :
	name(nm), node(TwoNode(p, n)), value(_DeviceValue(v))
{
}

template<typename _DeviceValue, typename _Stamp, typename _Symbol>
Device<FourNode, _DeviceValue, _Stamp, _Symbol>::Device(string nm, int p,
		int n, int pp, int nn, double v/*,ParamExpr* expr=nullptr*/) :
	name(nm), node(FourNode(p, n, pp, nn)), value(_DeviceValue(v))
{
}


#endif /* DEVICE_H_ */
