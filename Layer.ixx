module;
#include<iostream>
#include<cassert>

import Matrix;
export module Layer;
export using type = long double;

class Calculator {
protected:
	Matrix<type>* ret = nullptr, * diff = nullptr;
public:

	Calculator() = default;

	virtual void forward(const Matrix<type>& mat1, const Matrix<type>& mat2) = 0;
	virtual void forward(const Matrix<type>& mat1) = 0;
	void backward(const Matrix<type>& mat1) {
		diff = &mat1.getCopy();
		_backward();
	}
	const Matrix<type>* getForward() { return ret; }
	virtual const Matrix<type>* getUpperBackward() = 0;
	virtual const Matrix<type>* getLowerBackward() = 0;
	virtual const Matrix<type>* getBackward() = 0;
protected:
	virtual void _forward() = 0;
	virtual void _backward() = 0;
	virtual void clear() = 0;
};
class BinaryCalculator : public Calculator {
protected:
	Matrix<type>* upperMat = nullptr, * lowerMat = nullptr, * upperDiff = nullptr, * lowerDiff = nullptr;
public:
	void forward(const Matrix<type>& mat1, const Matrix<type>& mat2) override {
		clear();
		upperMat = &mat1.getCopy();
		lowerMat = &mat2.getCopy();
		_forward();
	}
	void forward(const Matrix<type>& mat1) override { assert(false && "undifined behavior"); }
	const Matrix<type>* getUpperBackward() override { return upperDiff; }
	const Matrix<type>* getLowerBackward() override { return lowerDiff; }
	const Matrix<type>* getBackward() override { assert(false && "undifined behavior"); return nullptr; }
protected:
	void clear() override {
		if (upperMat != nullptr) {
			delete upperMat;
			upperMat = nullptr;
		}
		if (lowerMat != nullptr) 
		{
			delete lowerMat;
			lowerMat = nullptr;
		}
		if (upperDiff != nullptr) 
		{
			delete upperDiff;
			upperDiff = nullptr;
		}
		if (lowerDiff != nullptr) 
		{
			delete lowerDiff;
			lowerDiff = nullptr;
		}
		if (ret != nullptr) 
		{
			delete ret;
			ret = nullptr;
		}
		if (diff != nullptr) 
		{
			delete diff;
			diff = nullptr;
		}
	}
};
class Adder final : public BinaryCalculator {
public:

	Adder() = default;

protected:
	void _forward() override { ret = &Matrix<type>::binaryOper(*upperMat, *lowerMat, Add<type>()); }
	void _backward() override {
		upperDiff = &diff->getCopy();
		lowerDiff = &diff->getCopy();
	}
	const Matrix<type>* getBackward() override { 
		return diff;
	}
};
class Multipler final : public BinaryCalculator {
public:

	Multipler() = default;

protected:
	void _forward() override { ret = &Matrix<type>::binaryOper(*upperMat, *lowerMat, Mul<type>()); }
	void _backward() override {
		upperDiff = &Matrix<type>::binaryOper(*lowerMat, *diff, Mul<type>());
		lowerDiff = &Matrix<type>::binaryOper(*upperMat, *diff, Mul<type>());
	}
};
class Divider final : public BinaryCalculator {
public:

	Divider() = default;

protected:
	void _forward() override { ret = &Matrix<type>::binaryOper(*upperMat, *lowerMat, Div<type>()); }
	void _backward() override {

		auto* temp = &Matrix<type>::unaryOper(*lowerMat, Reverse<type>());
		auto* temp2 = &Matrix<type>::binaryOper(*temp, *diff, Mul<type>());
		upperDiff = temp2;
		delete temp;

		temp = &Matrix<type>::binaryOper(*ret, *lowerMat, Div<type>());
		temp2 = &Matrix<type>::binaryOper(*temp, Scala<type>(-1), Mul<type>());
		auto* temp3 = &Matrix<type>::binaryOper(*temp2, *diff, Mul<type>()); 
		lowerDiff = temp3;
		delete temp;
		delete temp2;
	}
};
class Doter final : public BinaryCalculator {
public:

	Doter() = default;

protected:
	void _forward() override { ret = &Matrix<type>::dot(*upperMat, *lowerMat); }
	void _backward() override {
		lowerMat->Toperation();
		upperMat->Toperation();
		upperDiff = &Matrix<type>::dot(*diff, *lowerMat);
		lowerDiff = &Matrix<type>::dot(*upperMat, *diff);
	}
};
class UnaryCalculator : public Calculator {
protected:
	Matrix<type>* mat = nullptr, * _diff = nullptr;
public:
	void forward(const Matrix<type>& mat1, const Matrix<type>& mat2) override { assert(false && "undifined behavior"); }
	void forward(const Matrix<type>& mat1) override {
		clear();
		mat = &mat1.getCopy();
		_forward();
	}
	const Matrix<type>* getUpperBackward() override { assert(false && "undifined behavior"); return nullptr; }
	const Matrix<type>* getLowerBackward() override { assert(false && "undifined behavior"); return nullptr; }
	const Matrix<type>* getBackward() override { return _diff; }
protected:
	void clear() override {
		if (mat != nullptr) 
		{
			delete mat;
			mat = nullptr;
		}
		if (_diff != nullptr) 
		{
			delete _diff;
			_diff = nullptr;

		}
		if (ret != nullptr) 
		{
			delete ret;
			ret = nullptr;
		}
		if (diff != nullptr) 
		{
			delete diff;
			diff = nullptr;
		}
	}
};
class Exper final : public UnaryCalculator {
public:

	Exper() = default;

protected:
	void _forward() override { ret = &Matrix<type>::unaryOper(*mat, Exp<type>()); }
	void _backward() override { _diff = &Matrix<type>::binaryOper(*diff, *ret, Mul<type>()); }
};
class Sumer final : public UnaryCalculator {
public:

	Sumer() = default;

protected:
	void _forward() override { ret = &mat->getSum(); }
	void _backward() override { _diff = &diff->getCopy(); }
};
class Loger final : public UnaryCalculator {
public:

	Loger() = default;

protected:
	void _forward() override { ret = &Matrix<type>::unaryOper(*mat, ApplyNatureLog<type>());}
	void _backward() override { 
		_diff = &diff->getCopy(); 
		_diff->binaryOper(*mat, Div<type>());
	}
};
export{
	
	enum LayerType{INPUT, AFFINE, ACTIVATION, SOFTMAX, LOSS};

	class Layer {
	protected:
		int Layertype;
	public:
		virtual const Matrix<type>& forward(const Matrix<type>& inputMat) const { assert(false && "undefined..."); return Scala<type>(0); }
		virtual const Matrix<type>& backward(const Matrix<type>& inputMat) const { assert(false && "undefined..."); return Scala<type>(0); }
		virtual void updateValue(double step) { assert(false && "undefined..."); }
		int getType() { return Layertype; }
	};

	class InputLayer : public Layer {
	public:
		InputLayer() { Layertype = INPUT; }
		const Matrix<type>& forward(const Matrix<type>& inputMat) const override { return inputMat; }
	};
	class AffineLayer : public Layer {
	private:
		Matrix<type>* bias = nullptr, * weight = nullptr;
		Calculator* doter = nullptr, * adder = nullptr;
	public:
		AffineLayer(const Matrix<type>& weight, const Matrix<type>& bias, const Initer<type>& initer) {
			doter = new Doter(); 
			adder = new Adder(); 
			this->weight = &weight.getCopy();
			this->bias = &bias.getCopy();
			this->weight->unaryOper(initer);
			this->bias->unaryOper(initer);
			Layertype = AFFINE;
		}
		virtual const Matrix<type>& forward(const Matrix<type>& inputMat) const override {
			doter->forward(inputMat, *weight);
			adder->forward(*doter->getForward(), *bias);
			return *adder->getForward();
		}
		virtual const Matrix<type>& backward(const Matrix<type>& inputMat) const override {
			adder->backward(inputMat);
			doter->backward(*adder->getBackward());
			return *doter->getUpperBackward(); //doter의 upperMat가 이전 계층과 연결되있으므로...
			//return *bias;
		}
		virtual void updateValue(double step) override {

			auto temp = &Matrix<type>::binaryOper(*adder->getBackward(), Scala<type>(step), Mul<type>());
			bias->binaryOper(*temp, Sub<type>());
			delete temp;

			temp = &Matrix<type>::binaryOper(*doter->getLowerBackward(), Scala<type>(step), Mul<type>());
			weight->binaryOper(*temp, Sub<type>());
			delete temp;
		}
	};
	class ReluLayer : public Layer {
	protected:
		mutable Matrix<type>* cache = nullptr;
	public:
		ReluLayer() { Layertype = ACTIVATION; }
		const Matrix<type>& forward(const Matrix<type>& inputMat) const override {
			if (cache != nullptr)
				delete cache;
			cache = &inputMat.getCopy();
			cache->unaryOper(ReLuForwardOper<type>());
			return *cache;
		}
		const Matrix<type>& backward(const Matrix<type>& inputMat) const override {
			cache->unaryOper(ReLuBackwardOper<type>());
			cache->binaryOper(inputMat, Mul<type>());
			return *cache;
		}
	};
	class SoftMaxLayer : public Layer {
	protected:
		Calculator* sumer = nullptr, * exper = nullptr, * divider = nullptr;
	public:
		SoftMaxLayer() { sumer = new Sumer(); exper = new Exper(); divider = new Divider(); Layertype = SOFTMAX; }
		virtual const Matrix<type>& forward(const Matrix<type>& inputMat) const {

			auto tempmax = &inputMat.getMax();
			auto temp = &Matrix<type>::binaryOper(inputMat, *tempmax, Sub<type>()); //why????????????????????????? sibal
			exper->forward(*temp);
			sumer->forward(*exper->getForward());
			divider->forward(*exper->getForward(), *sumer->getForward());
			delete temp;
			delete tempmax;
			return *divider->getForward();
		}
		virtual const Matrix<type>& backward(const Matrix<type>& inputMat) const {

			divider->backward(inputMat);
			//sumer->backward(*divider->getLowerBackward());
			auto tempmax = &divider->getLowerBackward()->getSum();
			auto temp = &Matrix<type>::binaryOper(*divider->getUpperBackward(), *tempmax, Add<type>());
			exper->backward(*temp);
			delete temp;
			delete tempmax;

			return *exper->getBackward();
		}
	};
	class CrossEntropyLayer : public Layer {
	protected:
		const Matrix<type>* label = nullptr;
		Calculator* loger = nullptr, * multipler1 = nullptr, * sumer = nullptr, * multipler2 = nullptr;
	public:
		CrossEntropyLayer() { loger = new Loger(); multipler1 = new Multipler();  multipler2 = new Multipler();  sumer = new Sumer(); Layertype = LOSS; }
		const Matrix<type>& forward(const Matrix<type>& inputMat) const override {

			auto* temp = &Matrix<type>::binaryOper(inputMat, Scala<type>(0.00000001), Add<type>());

			loger->forward(*temp);
			multipler1->forward(*label, *loger->getForward());
			sumer->forward(*multipler1->getForward());
			multipler2->forward(Scala<type>(-1), *sumer->getForward());
			delete temp;
			return *multipler2->getForward();
		}
		const Matrix<type>& backward(const Matrix<type>& inputMat = Scala<type>(1)) const override {
			multipler2->backward(inputMat);
			sumer->backward(*multipler2->getLowerBackward());
			multipler1->backward(*sumer->getBackward());
			loger->backward(*multipler1->getLowerBackward());
			return *loger->getBackward();
		}
		void setLabel(const Matrix<type>& label) {
			if (this->label != nullptr){
				delete this->label;
				this->label = nullptr;
			}

			this->label = &label.getCopy();
		}
	};
	class AffineLayer2D : public AffineLayer {
	private:
		int inputLen, outputLen;
	public:
		AffineLayer2D(int prev, int next, const Initer<type>& initer) : AffineLayer(Matrix2D<type>(prev, next, 0), Matrix2D<type>(1, next, 0), initer){}
	};
}