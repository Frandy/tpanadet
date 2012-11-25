/*
 * symbol.h
 *
 *  Created on: Nov 23, 2012
 *      Author: chjd
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

template <typename Device>
class Symbol
{
public:
	void CreateGraphPair(Device& device,Graph& lgph,Graph& rgph);
	bool Include(/*Device& device,*/Graph& lgph,Graph& rgph,int& sign);
	bool Exclude(/*Device& device,*/Graph& lgph,Graph& rgph,int& sign);
private:
	int edge_index; // ?
};

// empty symbol
template<>
class Symbol<void>
{
};

#endif /* SYMBOL_H_ */
