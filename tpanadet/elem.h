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
	virtual void Print(ostream& out){}
};

#endif /* ELEM_H_ */
