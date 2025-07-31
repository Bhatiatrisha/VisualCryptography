#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

#if CV_MAJOR_VERSION == 4 && CV_MINOR_VERSION == 12 && CV_SUBMINOR_VERSION == 0
#ifdef _DEBUG
#pragma comment(lib, "opencv_world4120d.lib")
#else
#pragma comment(lib, "opencv_world4120.lib")
#endif
#else
#error "OpenCV version must be 4.12.0"
#endif

class VisualCryptography {
private:
    cv::Mat original;
    int width, height;
    std::mt19937 rng;
    static const size_t MAX_WIDTH = 400;
    static const size_t MAX_HEIGHT = 400;

public:
    VisualCryptography(const cv::Mat& input_image) {
        try {
            if (input_image.empty()) {
                throw std::runtime_error("Input image is empty");
            }

            // Resize if image is too large
            cv::Mat resized;
            if (input_image.cols > MAX_WIDTH || input_image.rows > MAX_HEIGHT) {
                double scale = std::min(
                    static_cast<double>(MAX_WIDTH) / input_image.cols,
                    static_cast<double>(MAX_HEIGHT) / input_image.rows
                );
                cv::resize(input_image, resized, cv::Size(), scale, scale);
            }
            else {
                resized = input_image.clone();
            }

            // Convert to binary
            if (resized.channels() == 3) {
                cv::Mat gray;
                cv::cvtColor(resized, gray, cv::COLOR_BGR2GRAY);
                cv::threshold(gray, original, 128, 255, cv::THRESH_BINARY);
            }
            else {
                cv::threshold(resized, original, 128, 255, cv::THRESH_BINARY);
            }

            width = original.cols;
            height = original.rows;
            rng.seed(std::random_device()());

            std::cout << "Image processed. Size: " << width << "x" << height << std::endl;
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error in constructor: " + std::string(e.what()));
        }
    }

    std::pair<cv::Mat, cv::Mat> generateShares() {
        try {
            cv::Mat share1 = cv::Mat::zeros(height * 2, width * 2, CV_8UC1);
            cv::Mat share2 = cv::Mat::zeros(height * 2, width * 2, CV_8UC1);

            const uchar patterns[2][2][2] = {
                {{255, 0}, {0, 255}},  // Pattern 1
                {{0, 255}, {255, 0}}   // Pattern 2
            };

            std::uniform_int_distribution<int> dist(0, 1);

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    uchar pixel = original.at<uchar>(i, j);
                    int pattern_idx = dist(rng);

                    for (int x = 0; x < 2; x++) {
                        for (int y = 0; y < 2; y++) {
                            int row = i * 2 + x;
                            int col = j * 2 + y;

                            if (pixel == 255) { // White pixel
                                share1.at<uchar>(row, col) = patterns[pattern_idx][x][y];
                                share2.at<uchar>(row, col) = patterns[pattern_idx][x][y];
                            }
                            else { // Black pixel
                                share1.at<uchar>(row, col) = patterns[pattern_idx][x][y];
                                share2.at<uchar>(row, col) = patterns[pattern_idx][x][y] == 255 ? 0 : 255;
                            }
                        }
                    }
                }
            }

            return std::make_pair(share1, share2);
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error generating shares: " + std::string(e.what()));
        }
    }

    cv::Mat overlayShares(const cv::Mat& share1, const cv::Mat& share2) {
        try {
            if (share1.size() != share2.size()) {
                throw std::runtime_error("Shares have different sizes");
            }

            cv::Mat result = cv::Mat::zeros(share1.size(), CV_8UC1);
            cv::bitwise_and(share1, share2, result);
            return result;
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Error in overlay: " + std::string(e.what()));
        }
    }
};

// Function to create test image
cv::Mat createTestImage(int width = 100, int height = 100) {
    cv::Mat image = cv::Mat::ones(height, width, CV_8UC1) * 255;
    cv::putText(image, "TEST", cv::Point(10, height / 2),
        cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0), 2);
    return image;
}

int main() {
    try {
        std::cout << "Visual Cryptography Demo\n" << std::endl;

        // Create test image
        std::cout << "Creating test image..." << std::endl;
        cv::Mat testImage = createTestImage();
        if (testImage.empty()) {
            throw std::runtime_error("Failed to create test image");
        }

        // Initialize visual cryptography
        std::cout << "Initializing visual cryptography..." << std::endl;
        VisualCryptography vc(testImage);

        // Generate shares
        std::cout << "Generating shares..." << std::endl;
        auto shares = vc.generateShares();
        cv::Mat& share1 = shares.first;
        cv::Mat& share2 = shares.second;

        // Save shares
        std::cout << "Saving shares..." << std::endl;
        cv::imwrite("share1.png", share1);
        cv::imwrite("share2.png", share2);

        // Create overlay
        std::cout << "Creating overlay..." << std::endl;
        cv::Mat result = vc.overlayShares(share1, share2);
        cv::imwrite("result.png", result);

        // Display images
        cv::namedWindow("Original", cv::WINDOW_NORMAL);
        cv::namedWindow("Share 1", cv::WINDOW_NORMAL);
        cv::namedWindow("Share 2", cv::WINDOW_NORMAL);
        cv::namedWindow("Result", cv::WINDOW_NORMAL);

        cv::resizeWindow("Original", 400, 400);
        cv::resizeWindow("Share 1", 400, 400);
        cv::resizeWindow("Share 2", 400, 400);
        cv::resizeWindow("Result", 400, 400);

        cv::imshow("Original", testImage);
        cv::imshow("Share 1", share1);
        cv::imshow("Share 2", share2);
        cv::imshow("Result", result);

        std::cout << "\nImages have been generated and saved:" << std::endl;
        std::cout << "- share1.png" << std::endl;
        std::cout << "- share2.png" << std::endl;
        std::cout << "- result.png" << std::endl;
        std::cout << "\nPress any key to exit..." << std::endl;

        cv::waitKey(0);
        cv::destroyAllWindows();
    }
    catch (const cv::Exception& e) {
        std::cerr << "OpenCV error: " << e.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}