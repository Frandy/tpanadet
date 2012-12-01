/*
 * elem.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef ELEM_H_
#define ELEM_H_

// interface class, base virtual class

class Elem
{
public:
	Elem()=default;
	Elem(DeviceType tp):type(tp){}
	DeviceType Type() const { return type;}
	void Type(DeviceType tp) { type = tp;}  // may be this is not needed, but type is needed,
											// not all operation can be done as virtual function

	// interface

protected:
	DeviceType type;
};


#endif /* ELEM_H_ */
