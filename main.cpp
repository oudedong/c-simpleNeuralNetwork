#include<iostream>
#include<filesystem>

import Layer;
import Matrix;
import visualTask;
import Network;
using namespace std;

void getTrainData(TagContainer<type, Matrix2D<type>>& tags) {
	ImageFileReader<type> reader;
	ImageLinearReader<type> linear{ reader };
	InputBufferedReader<type> ig0{ linear };
	InputBufferedReader<type> ig1{ linear };
	InputBufferedReader<type> ig2{ linear };
	InputBufferedReader<type> ig3{ linear };
	InputBufferedReader<type> ig4{ linear };
	InputBufferedReader<type> ig5{ linear };
	InputBufferedReader<type> ig6{ linear };
	InputBufferedReader<type> ig7{ linear };
	InputBufferedReader<type> ig8{ linear };
	InputBufferedReader<type> ig9{ linear };
	ig0.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\0");
	ig1.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\1");
	ig2.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\2");
	ig3.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\3");
	ig4.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\4");
	ig5.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\5");
	ig6.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\6");
	ig7.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\7");
	ig8.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\8");
	ig9.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\training\\9");
	while (ig0.hasNext())
		tags.put(ig0.next(), Matrix2D<type>(vector<type>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
	while (ig1.hasNext())
		tags.put(ig1.next(), Matrix2D<type>(vector<type>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0}));
	while (ig2.hasNext())
		tags.put(ig2.next(), Matrix2D<type>(vector<type>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0}));
	while (ig3.hasNext())
		tags.put(ig3.next(), Matrix2D<type>(vector<type>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
	while (ig4.hasNext())
		tags.put(ig4.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0}));
	while (ig5.hasNext())
		tags.put(ig5.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 1, 0, 0, 0, 0}));
	while (ig6.hasNext())
		tags.put(ig6.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 1, 0, 0, 0}));
	while (ig7.hasNext())
		tags.put(ig7.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0}));
	while (ig8.hasNext())
		tags.put(ig8.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0}));
	while (ig9.hasNext())
		tags.put(ig9.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
	tags.shuffle();
}
void gettestData(TagContainer<type, Matrix2D<type>>& tags) {
	ImageFileReader<type> reader;
	ImageLinearReader<type> linear{ reader };
	InputBufferedReader<type> ig0{ linear };
	InputBufferedReader<type> ig1{ linear };
	InputBufferedReader<type> ig2{ linear };
	InputBufferedReader<type> ig3{ linear };
	InputBufferedReader<type> ig4{ linear };
	InputBufferedReader<type> ig5{ linear };
	InputBufferedReader<type> ig6{ linear };
	InputBufferedReader<type> ig7{ linear };
	InputBufferedReader<type> ig8{ linear };
	InputBufferedReader<type> ig9{ linear };
	ig0.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\0");
	ig1.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\1");
	ig2.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\2");
	ig3.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\3");
	ig4.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\4");
	ig5.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\5");
	ig6.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\6");
	ig7.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\7");
	ig8.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\8");
	ig9.read("C:\\Users\\hhs10\\OneDrive\\πŸ≈¡ »≠∏È\\mnist_png\\testing\\9");
	while (ig0.hasNext())
		tags.put(ig0.next(), Matrix2D<type>(vector<type>{1, 0, 0, 0, 0, 0, 0, 0, 0, 0}));
	while (ig1.hasNext())
		tags.put(ig1.next(), Matrix2D<type>(vector<type>{0, 1, 0, 0, 0, 0, 0, 0, 0, 0}));
	while (ig2.hasNext())
		tags.put(ig2.next(), Matrix2D<type>(vector<type>{0, 0, 1, 0, 0, 0, 0, 0, 0, 0}));
	while (ig3.hasNext())
		tags.put(ig3.next(), Matrix2D<type>(vector<type>{0, 0, 0, 1, 0, 0, 0, 0, 0, 0}));
	while (ig4.hasNext())
		tags.put(ig4.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 1, 0, 0, 0, 0, 0}));
	while (ig5.hasNext())
		tags.put(ig5.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 1, 0, 0, 0, 0}));
	while (ig6.hasNext())
		tags.put(ig6.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 1, 0, 0, 0}));
	while (ig7.hasNext())
		tags.put(ig7.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 0, 1, 0, 0}));
	while (ig8.hasNext())
		tags.put(ig8.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 0, 0, 1, 0}));
	while (ig9.hasNext())
		tags.put(ig9.next(), Matrix2D<type>(vector<type>{0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
	tags.shuffle();
}
void testDraw() {
	PaletteReader<type> p{28, 28};
	p.read();
	p.getMat().print();
}
void test1() {

	TagContainer<type, Matrix2D<type>> traindata;
	TagContainer<type, Matrix2D<type>> testdata;
	getTrainData(traindata);
	gettestData(testdata);

	NetBuilder netBuilder;
	Network* net = nullptr;

	netBuilder.appendLayer(new InputLayer);
	netBuilder.appendLayer(new AffineLayer2D(784, 100, Uniform_Initer<type>(-0.01, 0.01)));
	netBuilder.appendLayer(new ReluLayer());
	netBuilder.appendLayer(new AffineLayer2D(100, 10, Uniform_Initer<type>(-0.01, 0.01)));
	netBuilder.appendLayer(new SoftMaxLayer());
	netBuilder.appendLayer(new CrossEntropyLayer());
	netBuilder.setStep(0.0001);
	net = netBuilder.build();

	for (int i = 0; i < 60'000; i++) {
		auto& cur = traindata.get(i);
		if (i % 2000 == 0){
			net->train(*cur.first, cur.second)->print();
		}
		else
			net->train(*cur.first, cur.second);

	}
	for (int i = 0; i < 0; i++) {
		auto& cur = testdata.get(i);
		net->test(*cur.first)->print();
	}

	PaletteReader<type> draw{ 28, 28 };
	ImageLinearReader<type> test{ draw };

	while (true) {
		test.read();
		const auto& ret = *(dynamic_cast<const Matrix2D<type>*>(net->test(test.getMat())));
		ret.print();
		int max = 0;
		for (int i = 1; i < 10; i++)
			if (ret[0][i] > ret[0][max])
				max = i;
		cout << "guessed: " << max << endl;
	}
}
int main() {

	test1();
	//testDraw();
}