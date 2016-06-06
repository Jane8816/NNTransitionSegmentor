/*
 * DenseFeatureChar.h
 *
 *  Created on: Jan 25, 2016
 *      Author: mason
 */

#ifndef FEATURE_DENSEFEATURECHAR_H_
#define FEATURE_DENSEFEATURECHAR_H_

#include "N3L.h"

template<typename xpu>
class DenseFeatureChar {
public:
	//all state inter dependent features
	vector<int> _charIds, _bicharIds;
	Tensor<xpu, 3, dtype> _charprime, _bicharprime;
	Tensor<xpu, 3, dtype> _charpre, _charpreMask;
	Tensor<xpu, 3, dtype> _charInput, _charHidden;
	vector<Tensor<xpu, 3, dtype> > _charLeftRNNHiddenBuf, _charRightRNNHiddenBuf;
	Tensor<xpu, 3, dtype> _charLeftRNNHidden, _charRightRNNHidden;
	Tensor<xpu, 2, dtype> _charRNNHiddenDummy;

	Tensor<xpu, 3, dtype> _charprime_Loss, _bicharprime_Loss, _charpre_Loss;
	Tensor<xpu, 3, dtype> _charInput_Loss, _charHidden_Loss;
	Tensor<xpu, 3, dtype> _charLeftRNNHidden_Loss, _charRightRNNHidden_Loss;
	Tensor<xpu, 2, dtype> _charRNNHiddenDummy_Loss;

	bool _bTrain;
	int _charnum;
	int _buffer;

public:
	DenseFeatureChar() {
		_bTrain = false;
		_charnum = 0;
		_buffer = 0;
	}

	~DenseFeatureChar() {
		clear();
	}

public:
	inline void init(int charnum, int charDim, int bicharDim, int charcontext, int charHiddenDim, int charRNNHiddenDim, int buffer = 0, bool bTrain = false) {
		clear();

		_charnum = charnum;
		_bTrain = bTrain;
		_buffer = buffer;

		if (_charnum > 0) {
			int charwindow = 2 * charcontext + 1;
			int charRepresentDim = (charDim + bicharDim) * charwindow;

			_charIds.resize(charnum);
			_bicharIds.resize(charnum);
			_charprime = NewTensor<xpu>(Shape3(_charnum, 1, charDim), d_zero);
			_bicharprime = NewTensor<xpu>(Shape3(_charnum, 1, bicharDim), d_zero);
			_charpre = NewTensor<xpu>(Shape3(_charnum, 1, charDim + bicharDim), d_zero);
			_charInput = NewTensor<xpu>(Shape3(_charnum, 1, charRepresentDim), d_zero);
			_charHidden = NewTensor<xpu>(Shape3(_charnum, 1, charHiddenDim), d_zero);
			if (_buffer > 0) {
				_charLeftRNNHiddenBuf.resize(_buffer);
				_charRightRNNHiddenBuf.resize(_buffer);
				for (int idk = 0; idk < _buffer; idk++) {
					_charLeftRNNHiddenBuf[idk] = NewTensor<xpu>(Shape3(_charnum, 1, charRNNHiddenDim), d_zero);
					_charRightRNNHiddenBuf[idk] = NewTensor<xpu>(Shape3(_charnum, 1, charRNNHiddenDim), d_zero);
				}
			}
			_charLeftRNNHidden = NewTensor<xpu>(Shape3(_charnum, 1, charRNNHiddenDim), d_zero);
			_charRightRNNHidden = NewTensor<xpu>(Shape3(_charnum, 1, charRNNHiddenDim), d_zero);
			_charRNNHiddenDummy = NewTensor<xpu>(Shape2(1, charRNNHiddenDim), d_zero);

			if (_bTrain) {
				_charpreMask = NewTensor<xpu>(Shape3(_charnum, 1, charDim + bicharDim), d_zero);
				_charprime_Loss = NewTensor<xpu>(Shape3(_charnum, 1, charDim), d_zero);
				_bicharprime_Loss = NewTensor<xpu>(Shape3(_charnum, 1, bicharDim), d_zero);
				_charpre_Loss = NewTensor<xpu>(Shape3(_charnum, 1, charDim + bicharDim), d_zero);
				_charInput_Loss = NewTensor<xpu>(Shape3(_charnum, 1, charRepresentDim), d_zero);
				_charHidden_Loss = NewTensor<xpu>(Shape3(_charnum, 1, charHiddenDim), d_zero);
				_charLeftRNNHidden_Loss = NewTensor<xpu>(Shape3(_charnum, 1, charRNNHiddenDim), d_zero);
				_charRightRNNHidden_Loss = NewTensor<xpu>(Shape3(_charnum, 1, charRNNHiddenDim), d_zero);
				_charRNNHiddenDummy_Loss = NewTensor<xpu>(Shape2(1, charRNNHiddenDim), d_zero);
			}
		}

	}

	inline void clear() {
		if (_charnum > 0) {
			_charIds.clear();
			_bicharIds.clear();

			FreeSpace(&_charprime);
			FreeSpace(&_bicharprime);
			FreeSpace(&_charpre);
			FreeSpace(&_charInput);
			FreeSpace(&_charHidden);
			if (_buffer > 0) {
				for (int idk = 0; idk < _buffer; idk++) {
					FreeSpace(&(_charLeftRNNHiddenBuf[idk]));
					FreeSpace(&(_charRightRNNHiddenBuf[idk]));
				}
				_charLeftRNNHiddenBuf.clear();
				_charRightRNNHiddenBuf.clear();
			}
			FreeSpace(&_charLeftRNNHidden);
			FreeSpace(&_charRightRNNHidden);
			FreeSpace(&_charRNNHiddenDummy);

			if (_bTrain) {
				FreeSpace(&_charprime_Loss);
				FreeSpace(&_bicharprime_Loss);
				FreeSpace(&_charpreMask);
				FreeSpace(&_charpre_Loss);
				FreeSpace(&_charInput_Loss);
				FreeSpace(&_charHidden_Loss);
				FreeSpace(&_charLeftRNNHidden_Loss);
				FreeSpace(&_charRightRNNHidden_Loss);
				FreeSpace(&_charRNNHiddenDummy_Loss);
			}

		}

		_bTrain = false;
		_charnum = 0;
		_buffer = 0;
	}

};

#endif /* FEATURE_DENSEFEATURECHAR_H_ */
