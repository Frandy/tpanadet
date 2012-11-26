/*
 * deviceValue.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef DEVICEVALUE_H_
#define DEVICEVALUE_H_

/*
 * three kinds of values for device, dc, tran, ac;
 * for normal device, only dc and ac value, while tran is extra value for sources;
 * dc value, two kinds, impedance and admittance;
 * ac value, s order, 0, 1, -1;
 * and for normal device,
 * 		E,F,G,H <- admittance, 0
 * 		R 		<- impedance, 0
 * 		C		<- admittance, 1
 * 		L		<- impedance, -1
 * while, there are also tran value for C & L,
 * 		tran value is easy, for C, L, both are C/h, L/h.
 * in fact, ac & tran value are not value of device, but for analysis
 * and both MNA & GPDD use ac value, so hold it in device ?
 * 		no, mna use ac value once, only gpdd need save ac value
 */


class Impedance
{
public:
	double operator()(double d){ return 1.0/d;}
};

class Admittance
{
public:
	double operator()(double d) { return d;}
};

class AcZeroOrder
{
public:
	Cplx operator()(Cplx s,double d) {return Cplx(d,0);}
};

class AcPlusOrder
{
public:
	Cplx operator()(Cplx s,double d){return Cplx(d,0) * s;}
};

class AcMinusOrder
{
public:
	Cplx operator()(Cplx s,double d){return Cplx(d,0) / s;}
};

typedef Admittance TranNormal;

class TranTime
{
public:
	double operator()(double h,double d){ return d/h;}
};

template<typename _DcValue,typename _AcValue,typename _TranValue>
class DeviceValue
{
public:
	DeviceValue()=default;
	DeviceValue(double d){admittance = dcFunc(d);}
	DeviceValue(const DeviceValue& value){
		admittance = value->Admittance();
		dcFunc = _DcValue();
		acFunc = _AcValue();
		tranFunc = _TranValue();
	}
/*
	friend ostream& operator << (ostream& out,DeviceValue& value)
	{
		out << " value: ";
		// default precision is 6, is ok
		out << value.Value() << " ";
		return out;
	}
*/
	// set/get device value, saved as admittance
	void Value(double d){admittance = dcFunc(d);}
	double Value() {return dcFunc(admittance);}

	// return admittance, for dc analysis
	double DcValue()	const { return admittance;}

	// return cplx admittance, for ac analysis
	Cplx AcValue(Cplx s)	{	return acFunc(s,admittance);}

	// return tran value, for tran analysis
	double TranValue(double h){ return tranFunc(h,admittance);}
private:
	double admittance;
	_DcValue dcFunc;
	_AcValue acFunc;
	_TranValue tranFunc;
};

typedef DeviceValue<Admittance,AcZeroOrder,TranNormal> NormalEFGHValue;
typedef DeviceValue<Impedance,AcZeroOrder,TranNormal> ResValue;
typedef DeviceValue<Admittance,AcPlusOrder,TranTime> CapValue;
typedef DeviceValue<Impedance,AcMinusOrder,TranTime> IndValue;


// not good to use wave as template param, which make not easy to modify wave property

class SourceValue
{
public:
	SourceValue()=default;
	SourceValue(double dc,Cplx ac):dcValue(dc),acValue(ac){}

	// dc analysis
	double DcValue()	{	return dcValue;}
	void DcValue(double d) { dcValue = d;}

	// ac analysis
	Cplx AcValue()	{	return acValue;}
	void AcValue(Cplx v) { acValue = v;}

	// tran analysis
	void WaveFunc(std::function<double(vector<double>&)> wave){ waveFunc = wave;}
	void WaveParam(int N,...){}
	double TranValue(double t) { return waveFunc(waveparam);}
	double TranValue()	{	return dcValue;}
private:
	double dcValue;
	Cplx acValue;
	std::function<double(vector<double>&)> wavefunc;
	vector<double> waveparam;
};

typedef vector<double> ArrayValue;

class ModelValue
{
public:
	ModelValue(string nm,double param_0,...){}
private:
	string model_name;
	ValueArray model_param;
};

// not good, same code generated many times
template<typename DeviceValue>
ostream& operator << (ostream& out,DeviceValue& value)
{
	out << " value: ";
	// default precision is 6, is ok
	out << value.Value() << " ";
	return out;
}

ostream& operator << (ostream& out,SourceValue& value)
{
	out << " value: ";
	// default precision is 6, is ok
	out << "(dc)" << value.DcValue() << " ";
	out << "(ac)" << value.AcValue() << " ";
	return out;
}


#endif /* DEVICEVALUE_H_ */
