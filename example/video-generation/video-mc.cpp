#include <mcpp/mcpp.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

struct Pixel
{
    uint8_t r, g, b;
};

class Video
{
    public:
        Video(const std::string &filename, int width, int height, int scaleFactor, int frameRate);
        ~Video()
        {
            if(_file.is_open())
            {
                _file.close();
            }
        }

        void Play(mcpp::MinecraftConnection &mc);

    private:
        void DisplayFrame(size_t index, mcpp::MinecraftConnection &mc);
        mcpp::BlockType GetBestBlock(const Pixel& pixel);

        std::ifstream _file;
        std::vector<std::vector<Pixel>> _frames;

        int _width;
        int _height;
        int _scaleFactor;
        int _frameRate;
        int _frameDelay;
        int _frameSize;

        mcpp::Coordinate _position;
        std::vector<mcpp::BlockType> _options;
        std::vector<Pixel> _optionColors;
};

int main(int argc, char *argv[]) {
    if(argc < 6)
    {
        std::cerr << "Usage: " << argv[0] << " filename.rgb width height scale frame_rate" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    if(filename.find(".mp4") != std::string::npos)
    {
        std::cerr << "Be sure to convert mp4 files to rgb files before use." << std::endl << "Usage: ffmpeg -i filename.mp4 -f rawvideo -pix_fmt rgb24 filename.rgb" << std::endl;
        return 1;
    }

    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);
    int scaleFactor = std::stoi(argv[4]);
    int frameRate = std::stoi(argv[5]);

    mcpp::MinecraftConnection mc;

    Video *video = new Video(filename, width, height, scaleFactor, frameRate);
    video->Play(mc);

    return 0;
}

Video::Video(const std::string &filename, int width, int height, int scaleFactor, int frameRate)
: _width(width), _height(height), _scaleFactor(scaleFactor), _frameRate(frameRate), 
_frameDelay(1000 / _frameRate), _frameSize(_width * _height * 3)
{
    _file.open(filename, std::ios::binary);
    if(!_file.is_open())
    {
        throw std::runtime_error("Error opening file");
    }
    
    while(_file)
    {
        std::vector<Pixel> frame;
        frame.resize(_width * _height);
        _file.read(reinterpret_cast<char*>(frame.data()), _frameSize);

        if(_file.gcount() == _frameSize)
        {
            _frames.push_back(frame);
        }
        else
        {
            break;
        }
    }
}

void Video::Play(mcpp::MinecraftConnection &mc)
{
    _options.push_back(mcpp::Blocks::RED_CONCRETE);
    _options.push_back(mcpp::Blocks::BLUE_CONCRETE);
    _options.push_back(mcpp::Blocks::CYAN_CONCRETE);
    _options.push_back(mcpp::Blocks::GRAY_CONCRETE);
    _options.push_back(mcpp::Blocks::LIME_CONCRETE);
    _options.push_back(mcpp::Blocks::PINK_CONCRETE);
    _options.push_back(mcpp::Blocks::BLACK_CONCRETE);
    _options.push_back(mcpp::Blocks::BROWN_CONCRETE);
    _options.push_back(mcpp::Blocks::GREEN_CONCRETE);
    _options.push_back(mcpp::Blocks::WHITE_CONCRETE);
    _options.push_back(mcpp::Blocks::ORANGE_CONCRETE);
    _options.push_back(mcpp::Blocks::PURPLE_CONCRETE);
    _options.push_back(mcpp::Blocks::YELLOW_CONCRETE);
    _options.push_back(mcpp::Blocks::MAGENTA_CONCRETE);
    _options.push_back(mcpp::Blocks::LIGHT_BLUE_CONCRETE);
    _options.push_back(mcpp::Blocks::LIGHT_GRAY_CONCRETE);

    _options.push_back(mcpp::Blocks::RED_WOOL);
    _options.push_back(mcpp::Blocks::BLUE_WOOL);
    _options.push_back(mcpp::Blocks::CYAN_WOOL);
    _options.push_back(mcpp::Blocks::GRAY_WOOL);
    _options.push_back(mcpp::Blocks::LIME_WOOL);
    _options.push_back(mcpp::Blocks::PINK_WOOL);
    _options.push_back(mcpp::Blocks::BLACK_WOOL);
    _options.push_back(mcpp::Blocks::BROWN_WOOL);
    _options.push_back(mcpp::Blocks::GREEN_WOOL);
    _options.push_back(mcpp::Blocks::WHITE_WOOL);
    _options.push_back(mcpp::Blocks::ORANGE_WOOL);
    _options.push_back(mcpp::Blocks::PURPLE_WOOL);
    _options.push_back(mcpp::Blocks::YELLOW_WOOL);
    _options.push_back(mcpp::Blocks::MAGENTA_WOOL);
    _options.push_back(mcpp::Blocks::LIGHT_BLUE_WOOL);
    _options.push_back(mcpp::Blocks::LIGHT_GRAY_WOOL);
    
    _optionColors.push_back(Pixel{142, 33, 33}); // RED CONCRETE
    _optionColors.push_back(Pixel{45, 47, 143}); // BLUE CONCRETE
    _optionColors.push_back(Pixel{21, 119, 136}); // CYAN CONCRETE
    _optionColors.push_back(Pixel{55, 58, 62}); // GRAY CONCRETE
    _optionColors.push_back(Pixel{94, 169, 24}); // LIME CONCRETE
    _optionColors.push_back(Pixel{213, 101, 143}); // PINK CONCRETE
    _optionColors.push_back(Pixel{8, 10, 15}); // BLACK CONCRETE
    _optionColors.push_back(Pixel{96, 60, 32}); // BROWN CONCRETE
    _optionColors.push_back(Pixel{73, 91, 36}); // GREEN CONCRETE
    _optionColors.push_back(Pixel{255, 255, 255}); // WHITE CONCRETE
    _optionColors.push_back(Pixel{224, 97, 1}); // ORANGE CONCRETE
    _optionColors.push_back(Pixel{100, 32, 156}); // PURPLE CONCRETE
    _optionColors.push_back(Pixel{241, 175, 21}); // YELLOW CONCRETE
    _optionColors.push_back(Pixel{169, 48, 159}); // MAGENTA CONCRETE
    _optionColors.push_back(Pixel{36, 137, 199}); // LIGHT BLUE CONCRETE
    _optionColors.push_back(Pixel{125, 125, 115}); // LIGHT GRAY CONCRETE

    _optionColors.push_back(Pixel{158, 43, 39}); // RED WOOL
    _optionColors.push_back(Pixel{37, 49, 147}); // BLUE WOOL
    _optionColors.push_back(Pixel{38, 113, 145}); // CYAN WOOL
    _optionColors.push_back(Pixel{65, 65, 65}); // GRAY WOOL
    _optionColors.push_back(Pixel{57, 186, 46}); // LIME WOOL
    _optionColors.push_back(Pixel{217, 129, 153}); // PINK WOOL
    _optionColors.push_back(Pixel{24, 20, 20}); // BLACK WOOL
    _optionColors.push_back(Pixel{86, 51, 28}); // BROWN WOOL
    _optionColors.push_back(Pixel{54, 75, 24}); // GREEN WOOL
    _optionColors.push_back(Pixel{228, 228, 228}); // WHITE WOOL
    _optionColors.push_back(Pixel{235, 126, 54}); // ORANGE WOOL
    _optionColors.push_back(Pixel{126, 52, 191}); // PURPLE WOOL
    _optionColors.push_back(Pixel{194, 181, 28}); // YELLOW WOOL
    _optionColors.push_back(Pixel{190, 73, 201}); // MAGENTA WOOL
    _optionColors.push_back(Pixel{228, 236, 253}); // LIGHT BLUE WOOL
    _optionColors.push_back(Pixel{160, 167, 167}); // LIGHT GRAY WOOL

    _position = mc.getPlayerPosition();
    _position.y += (_height / _scaleFactor) / 2;
    _position.z -= std::max((_width / _scaleFactor) / 2, (_height / _scaleFactor) / 2);
    _position.x += std::min((_width / _scaleFactor) / 2, 16);

    for(size_t i = 0; i < _frames.size(); i++)
    {
        DisplayFrame(i, mc);
        std::this_thread::sleep_for(std::chrono::milliseconds(_frameDelay));
    }
}

void Video::DisplayFrame(size_t index, mcpp::MinecraftConnection &mc)
{
    if(index < _frames.size())
    {
        const std::vector<Pixel>& frame = _frames[index];

        if(!frame.empty())
        {
            for(size_t i = 0; i < frame.size(); i += _scaleFactor)
            {
                const Pixel& pixel = frame[i];
                if(index > 0)
                {
                    const Pixel& lastPixel = _frames[index - 1][i];
                    if(
                        pixel.r == lastPixel.r
                        && pixel.g == lastPixel.g
                        && pixel.b == lastPixel.b
                    )
                    {
                        continue;
                    }
                }

                mcpp::BlockType blockType = GetBestBlock(pixel);

                mcpp::Coordinate pixelPosition = mcpp::Coordinate(_position);
                pixelPosition.z += (i % _width) / _scaleFactor;
                pixelPosition.y -= (i / _width) / _scaleFactor;

                mc.setBlock(pixelPosition, blockType);
            }
        }
    }
}

mcpp::BlockType Video::GetBestBlock(const Pixel& pixel)
{
    int r = pixel.r;
    int g = pixel.g;
    int b = pixel.b;

    int minDistance = 256;
    int closestOption = -1;

    for(size_t i = 0; i < _optionColors.size(); i++)
    {
        int dr = std::abs(r - _optionColors[i].r);
        int dg = std::abs(g - _optionColors[i].g);
        int db = std::abs(b - _optionColors[i].b);

        int distance = std::sqrt(dr * dr + dg * dg + db * db);

        if(distance < minDistance)
        {
            minDistance = distance;
            closestOption = i;
        }
    }

    if(closestOption!= -1)
    {
        return _options[closestOption];
    }
    else
    {
        std::cerr << "Color error" << std::endl;
    }

    return mcpp::Blocks::GLASS; // fallback; shouldn't happen
}