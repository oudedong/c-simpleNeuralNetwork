module;
#include<vector>
#include<utility>
#include<iostream>
#include<cassert>
#include<iomanip>
#include<cmath>
#include<random>

export module Matrix;
using namespace std;

export{

	template<typename T>
	class BinaryOper {
	public:
		virtual T operator()(T l, T r) const = 0;
	};
	template<typename T>
	class Add : public BinaryOper<T> {
	public:
		T operator()(T l, T r) const override { return l + r; }
	};
	template<typename T>
	class Sub : public BinaryOper<T> {
	public:
		T operator()(T l, T r) const override { return l - r; }
	};
	template<typename T>
	class Div : public BinaryOper<T> {
	public:
		T operator()(T l, T r) const override { return l / r; }
	};
	template<typename T>
	class Mul : public BinaryOper<T> {
	public:
		T operator()(T l, T r) const override { return l * r; }
	};

	template<typename T>
	class UnaryOper {
	public:
		virtual T operator()(T a) const = 0;
	};
	template<typename T>
	class Exp : public UnaryOper<T>{
	public:
		T operator()(T a) const override { return (T)exp(a); }
	};
	template<typename T>
	class Reverse : public UnaryOper<T> {
	public:
		T operator()(T a) const override { return (T)1/a; }
	};
	template<typename T>
	class ApplyNatureLog : public UnaryOper<T> {
	public:
		T operator()(T a) const override { return (T)log(a); }
	};
	template<typename T>
	class ReLuForwardOper : public UnaryOper<T> {
	public:
		T operator()(T a) const override { 
			if (a > 0) return a;
			else return 0;
		}
	};
	template<typename T>
	class ReLuBackwardOper : public UnaryOper<T> {
	public:
		T operator()(T a) const override {
			if (a > 0) return 1;
			else return 0;
		}
	};
	template<typename T>
	class Initer : public UnaryOper<T> {
	protected:
		virtual T _init() const = 0;
		random_device rd;
		mutable mt19937 mt{ rd() };
	public:
		T operator()(T a) const override final { return (T)_init(); }
	};
	template<typename T>
	class Uniform_Initer : public Initer<T> {
	private:
		uniform_real_distribution<T>* ud = nullptr;
	protected:
		T _init() const final { return ud->operator()(this->mt); }
	public:
		Uniform_Initer(T low, T high) { 
			ud = new uniform_real_distribution<T>(low, high);
		}
	};

	template<typename T>
	class Matrix {
	public:
		using dataType = T;
		virtual ~Matrix(){}
		virtual bool isScala() const { return false; }
		virtual void Toperation() = 0;
		virtual void dot(const Matrix<T>& mat) = 0;
		virtual void binaryOper(const Matrix<T>& right, const BinaryOper<T>& oper) = 0;
		virtual void unaryOper(const UnaryOper<T>& oper) = 0;
		virtual void print() const = 0;
		virtual int getRow() const = 0;
		virtual int getCol() const = 0;
		virtual Matrix<T>& getSum() const = 0;
		virtual Matrix<T>& getMax() const = 0;
		static Matrix<T>& binaryOper(const Matrix<T>& left, const Matrix<T>& right, const BinaryOper<T>& oper) {
			
			if(left.isScala() && !right.isScala()){
				Matrix<T>& temp = right.getCopy();
				temp.binaryOper(left, oper);
				return temp;
			}
			else{
				Matrix<T>& temp = left.getCopy();
				temp.binaryOper(right, oper);
				return temp;
			}
		}
		static Matrix<T>& dot(const Matrix<T>& left, const Matrix<T>& right) {
			
			Matrix<T>& temp = left.getCopy();

			temp.dot(right);
			return temp;
		}
		static Matrix<T>& unaryOper(const Matrix<T>& left, const UnaryOper<T>& oper) {

			Matrix<T>& temp = left.getCopy(); 

			temp.unaryOper(oper);
			return temp;
		}
		virtual Matrix<T>& getCopy() const = 0; //팩토리 메소드?
	};
	template<typename T>
	class Scala : public Matrix<T> {
	private:
		T value;
	public:

		Scala() = delete;
		Scala(T v) { value = v; }
		Scala(const Scala<T>& s) { value = s.value; }

		bool isScala() const override { return true; }
		void Toperation() override { 
			assert(false && "wrong operation at Scala..."); 
		}
		void dot(const Matrix<T>& mat) override { assert(false && "wrong operation at Scala..."); }
		void binaryOper(const Matrix<T>& right, const BinaryOper<T>& oper) override {
			if(!right.isScala())
				assert(false && "wrong operation at Scala...");
			const Scala<T>& scal = dynamic_cast<const Scala<T>&>(right);
			value = oper(value, scal.value);
		}
		void unaryOper(const UnaryOper<T>& oper) override {
			value = oper(value);
		}
		void print() const override {
			cout.precision(5);
			cout << "scala: " << value << endl;
		}
		int getRow() const override { assert(false && "wrong operation at Scala..."); return 0;}
		int getCol() const override { assert(false && "wrong operation at Scala..."); return 0; }
		Matrix<T>& getSum() const override { assert(false && "wrong operation at Scala..."); return *(new Scala<T>(0)); }
		Matrix<T>& getMax() const override { assert(false && "wrong operation at Scala..."); return *(new Scala<T>(0)); }
		Matrix<T>& getCopy() const override { return *(new Scala<T>(value)); }
		operator T() const { return value; }
	};
	template<typename T>
	class Matrix2D : public Matrix<T>{
	private:
		vector<vector<T>> mat;
		Matrix2D() = default;
	public:
		Matrix2D(const vector<T>& v) { mat = vector<vector<T>>(1, v); }
		Matrix2D(const vector<vector<T>>& v) { mat = v; }
		Matrix2D(vector<vector<T>>&& v) { mat = move(v); }
		Matrix2D(int row, int col, T init = 0) { mat = vector<vector<T>>(row, vector<T>(col, init)); }
		Matrix2D(const Matrix2D<T>& mat) { this->mat = mat.mat; }
		Matrix2D(Matrix2D<T>&& mat) { this->mat = move(mat.mat); }
		~Matrix2D() override {}

		vector<T>& operator[](int dex) { return mat[dex]; }
		const vector<T>& operator[](int dex) const { 
			return mat[dex]; 
		}
		void operator=(const Matrix2D<T>& m) {
			mat = m.mat;
		}

		void Toperation() override {

			vector<vector<T>> temp(mat[0].size(), vector<T>(mat.size()));

			for (int row = 0; row < temp.size(); row++) {
				for (int col = 0; col < temp[0].size(); col++)
					temp[row][col] = mat[col][row];
			}
			mat = move(temp);
		}
		void dot(const Matrix<T>& mat) override {

			assert(mat.getRow() == this->getCol() && "row and col doesnt match!");

			vector<vector<T>> temp(this->mat.size(), vector<T>(mat.getCol(), 0));
			const Matrix2D<T>& matt = dynamic_cast<const Matrix2D<T>&>(mat);

			for (int row = 0; row < temp.size(); row++) {
				for (int col = 0; col < temp[0].size(); col++) {
					for (int cur = 0; cur < this->getCol(); cur++)
						temp[row][col] += this->mat[row][cur] * matt[cur][col];
				}
			}
			this->mat = move(temp);
		}
		void binaryOper(const Matrix<T>& right, const BinaryOper<T>& oper) override {

			if (!right.isScala()) {
				assert(this->getCol() == right.getCol() && this->getRow() == right.getRow() && "size doesnt match!");
				const Matrix2D<T>& matt = dynamic_cast<const Matrix2D<T>&>(right);

				for (int row = 0; row < this->getRow(); row++) {
					for (int col = 0; col < this->getCol(); col++)
						mat[row][col] = oper(mat[row][col], matt[row][col]);
				}
				return;
			}
			const Scala<T>& scal = dynamic_cast<const Scala<T>&>(right);

			for (int row = 0; row < this->getRow(); row++) {
				for (int col = 0; col < this->getCol(); col++)
					mat[row][col] = oper(mat[row][col], scal);
			}
		}
		void unaryOper(const UnaryOper<T>& oper) override {
			for (int row = 0; row < this->getRow(); row++) {
				for (int col = 0; col < this->getCol(); col++)
					mat[row][col] = oper(mat[row][col]);
			}
		}
		void print() const {
			cout << "row: " << this->getRow() << ", col: " << this->getCol() << endl;
			cout << fixed;
			cout.precision(5);
			for (int row = 0; row < this->getRow(); row++) {
				for (int col = 0; col < this->getCol(); col++)
					cout << setw(5) << fixed << mat[row][col] << ',';
				cout << endl;
			}
		}
		int getRow() const override { return mat.size(); }
		int getCol() const override { return mat[0].size(); }
		Matrix<T>& getSum() const override {

			T sum = 0;

			for (int row = 0; row < this->getRow(); row++) {
				for (int col = 0; col < this->getCol(); col++)
					sum += mat[row][col];
			}
			return *(new Scala<T>(sum));
		}
		Matrix<T>& getMax() const override {

			T max = mat[0][0];

			for (int row = 0; row < this->getRow(); row++) {
				for (int col = 0; col < this->getCol(); col++)
					if (mat[row][col] > max)
						max = mat[row][col];
			}
			return *(new Scala<T>(max));
		}
		Matrix<T>& getCopy() const override { return *(new Matrix2D<T>(*this)); }
	};
}