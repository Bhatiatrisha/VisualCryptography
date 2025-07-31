# Visual Cryptography Implementation

## Overview
This project implements Visual Cryptography, a cryptographic technique that allows visual information (images, text, etc.) to be encrypted in a way that the decryption can be performed by the human visual system, without the aid of computers.

## What is Visual Cryptography?
Visual Cryptography is a method of encrypting a visual information (picture, text, etc.) in such a way that:
- The original information is divided into two or more pieces (shares)
- Each individual share reveals no information about the original image
- When the shares are properly aligned and overlaid, the original information becomes visible

### Basic Principle
For a binary image (black and white):
- Each pixel is divided into 2×2 sub-pixels
- For a white pixel:
  Share 1: [1 0] Share 2: [1 0] Overlay: [1 0] = White pixel
[0 1] [0 1] [0 1]

- For a black pixel:
Share 1: [1 0] Share 2: [0 1] Overlay: [0 0] = Black pixel
[0 1] [1 0] [0 0]

## Features
- Converts input images to binary (black and white) format
- Generates two complementary shares
- Supports image overlay to reveal the secret
- Handles various image sizes with automatic scaling
- Includes error handling and memory management
- Provides visual feedback through OpenCV windows

## Requirements
- Visual Studio 2019/2022
- OpenCV 4.12.0
- C++17 or later
- Windows OS (for current implementation)

## Installation & Setup

### 1. Install OpenCV
1. Download OpenCV 4.12.0 from the official website
2. Extract to `C:\opencv`
3. Add OpenCV bin directory to system PATH:
C:\opencv\build\x64\vc16\bin

### 2. Project Setup in Visual Studio
1. Create new Empty C++ Project
2. Set platform to x64
3. Configure project properties:

a. Include Directories:
C:\opencv\build\include

b. Library Directories:
C:\opencv\build\x64\vc16\lib

c. Additional Dependencies:
opencv_world4120d.lib (Debug)
opencv_world4120.lib (Release)

### 3. DLL Setup
Copy from `C:\opencv\build\x64\vc16\bin`:
- For Debug: `opencv_world4120d.dll`
- For Release: `opencv_world4120.dll`

To your project's output directory:
YourProject\x64\Debug
YourProject\x64\Release\

## Usage

### Basic Usage
// Create or load an image
cv::Mat inputImage = cv::imread("input.png", cv::IMREAD_GRAYSCALE);

// Initialize Visual Cryptography
VisualCryptography vc(inputImage);

// Generate shares
auto shares = vc.generateShares();
cv::Mat share1 = shares.first;
cv::Mat share2 = shares.second;

// Create overlay (decrypt)
cv::Mat result = vc.overlayShares(share1, share2);
Running the Demo
Build the project in Visual Studio
Run the executable
## The program will:
Create a test image
Generate two shares
Save shares as PNG files
Display all images in windows
Create and show the overlay result
Technical Details
Implementation Logic
## Image Processing:
- Convert input to binary (black/white) format
- Scale image if needed
- Process image in memory-efficient chunks
## Share Generation:

- Each pixel is expanded to 2×2 sub-pixels
- Random pattern selection for white pixels
- Complementary patterns for black pixels
- Shares are stored as separate images
## Overlay Process:

- Bitwise AND operation between shares
- Processed in chunks for memory efficiency
- Results in visible secret image
## Memory Management
- Automatic image scaling for large inputs
- Chunk-based processing for memory efficiency
- Proper cleanup of OpenCV resources
## Limitations
- Works with binary (black and white) images only
- Output images are 2x larger than input
- Some loss of resolution due to pixel expansion

## Contributing
Feel free to fork this repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

## License
MIT License

## Acknowledgments
- OpenCV team for the computer vision library
- Visual Cryptography concept by Moni Naor and Adi Shamir (1994)
