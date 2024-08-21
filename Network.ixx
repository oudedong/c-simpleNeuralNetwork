module;
#include<vector>

export module Network;
using namespace std;
import Layer;
import Matrix;

export{
	class Network {
	protected:
		vector<Layer*> layers;
		double step = 0.0001;
	public:
		Network(const vector<Layer*>& list, double step) { layers = list; this->step = step; }
		const Matrix<type>* train(const Matrix<type>& traindata, const Matrix<type>& label) {

			auto ret = &traindata;

			for (auto& layer : layers) {
				if (layer->getType() == LOSS)
					dynamic_cast<CrossEntropyLayer*>(layer)->setLabel(label);
				ret = &layer->forward(*ret);
			}
			Scala<type> s(1);
			const Matrix<type>* back = &s;
			for (int i = layers.size() - 1; i >= 0; i--) {
				if (layers[i]->getType() != INPUT)
					back = &layers[i]->backward(*back);
			}
			for (auto& layer : layers) {
				if (layer->getType() != AFFINE)
					continue;
				layer->updateValue(step);
			}
			return ret;
		}
		const Matrix<type>* test(const Matrix<type>& test) {

			auto ret = &test;

			for (auto& layer : layers) {
				if (layer->getType() == LOSS)
					break;
				ret = &layer->forward(*ret);
			}
			return ret;
		}
	};
	class NetBuilder {
		vector<Layer*> layers;
		type step = 0.001;
	public:
		void appendLayer(Layer* layer) {
			layers.push_back(layer);
		}
		void setStep(double s) {
			step = s;
		}
		Network* build() {
			auto temp = new Network(layers, step);
			layers.clear();
			return temp;
		}
	};
}