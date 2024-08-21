module;
#include <opencv2\opencv.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <utility>
#include <random>

export module visualTask;
import Matrix;
using namespace std;
using namespace cv;
namespace fs = filesystem;
export{
	template<typename T>
	class InputReader {
	public:
		virtual void read(const fs::path&) = 0;
		virtual Matrix<T>& getMat() const = 0;
	};
	template<typename T>
	class InputFlatReader : public InputReader<T> {
	protected:
		Matrix2D<T>* image = nullptr;
	public:
		int getHeight() const { return this->image->getRow(); }
		int getWide() const { return this->image->getCol(); }
		Matrix<T>& getMat() const override { return *(this->image); }
	};
	template<typename T, typename tagType>
	class TagContainer {
	private:
		vector<pair<Matrix<T>*, tagType>> container;
	public:
		void put(Matrix<T>& mat, tagType t) {
			container.emplace_back(&mat, t);
		}
		const pair<Matrix<T>*, tagType>& get(int index) const {
			return container[index];
		}
		int getSize() const { return container.size(); }
		void shuffle() {
			std::random_device rd;
			std::mt19937 g(rd()); 
			std::shuffle(container.begin(), container.end(), g);
		}
	};
	template<typename T>
	class ImageFileReader : public InputFlatReader<T> {
	public:
		void read(const fs::path& cur) override {
			if (this->image != nullptr)delete this->image;
			Mat img = imread(cur.string(), IMREAD_GRAYSCALE);
			this->image = new Matrix2D<T>(img.rows, img.cols, 0);
			for (int row = 0; row < img.rows; row++) {
				for (int col = 0; col < img.cols; col++) {
					auto& temp = *(this->image);
					temp[row][col] = img.at<uchar>(row, col);
				}
			}
		}
	};
	template<typename T>
	class ImageLinearReader : public InputFlatReader<T> {
	private:
		InputFlatReader<T>* reader = nullptr;
	public:
		ImageLinearReader(InputFlatReader<T>& reader) {
			this->reader = &reader;
		}
		void read(const fs::path& cur = "") override {
			if (this->image != nullptr)
				delete this->image;
			reader->read(cur);
			this->image = new Matrix2D<T>(1, reader->getHeight() * reader->getWide(), 0);

			auto& temp = dynamic_cast<Matrix2D<T>&>(reader->getMat());
			auto& temp2 = *(this->image);

			for (int row = 0, pos = 0; row < reader->getHeight(); row++) {
				for (int col = 0; col < reader->getWide(); col++, pos++)
					temp2[0][pos] = temp[row][col];
			}
		}
	};
	template<typename T>
	class PaletteReader : public InputFlatReader<T> {
	private:
		int height, width;
	public:
		PaletteReader(int h, int w) {
			height = h;
			width = w;
		}
		void read(const fs::path& cur = " ") override {

			if (this->image != nullptr)
				delete this->image;
			this->image = new Matrix2D<T>(height, width);

			Mat draw = Mat::zeros(height, width, CV_8UC1);

			namedWindow("Drawing", WINDOW_NORMAL);
			resizeWindow("Drawing", height*10, width*10); // 10배 확대하여 표시

			setMouseCallback("Drawing", [](int event, int x, int y, int flags, void* userdata)
			{
				if (event == EVENT_LBUTTONDOWN || (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON))
				{
					Mat* img = (Mat*)userdata;
					rectangle(*img, Point(x, y), Point(x, y), Scalar(255), -1);
					imshow("Drawing", *img);
				}
			}
			, &draw);

			while (true)
			{
				imshow("Drawing", draw);
				int key = waitKey(1);

				if (key == 13) // enter 키를 누르면 종료
					break;
				else if (key == 'c')
					draw = Mat::zeros(28, 28, CV_8UC1);
				for (int row = 0; row < height; row++) {
					for (int col = 0; col < width; col++){
						auto& temp = *(this->image);
						temp[row][col] = draw.at<uchar>(row, col);
					}
				}
			}

			destroyAllWindows();
		}
	};
	template<typename T>
	class InputBufferedReader {
	private:
		vector<Matrix<T>*> group;
		InputReader<T>* reader;
		mutable int curPos = 0;
	public:
		InputBufferedReader(InputReader<T>& reader) {
			this->reader = &reader; 
		}
		void read(const fs::path& cur) {
			for (const auto& entry : fs::directory_iterator(cur)) {
				if (!entry.is_regular_file())
					continue;
				reader->read(entry.path());
				group.push_back(&(reader->getMat().getCopy()));
			}
		}
		Matrix<T>& next() const {
			if (curPos >= group.size())
				assert(false && "noting to return");
			return *(group[curPos++]);
		}
		bool hasNext(){
			if (curPos < group.size())
				return true;
			return false;
		}
	};
}